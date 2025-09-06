// header-only, safe to put in your resolver header
#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <cstdint>
#include <cstddef>
#include <cstdio>    // _snprintf_s
#include <cstring>   // _stricmp

// Opaque types
struct Il2CppDomain; struct Il2CppAssembly; struct Il2CppImage; struct Il2CppClass; struct MethodInfo;

// Exports we use
using il2cpp_domain_get_t                 = Il2CppDomain* (*)();
using il2cpp_domain_get_assemblies_t      = const Il2CppAssembly** (*)(const Il2CppDomain*, size_t*);
using il2cpp_assembly_get_image_t         = const Il2CppImage* (*)(const Il2CppAssembly*);
using il2cpp_class_from_name_t            = Il2CppClass* (*)(const Il2CppImage*, const char*, const char*);
using il2cpp_class_get_method_from_name_t = const MethodInfo* (*)(Il2CppClass*, const char*, int);

// Optional (for name-only enumeration helper below)
using il2cpp_class_get_methods_t          = const MethodInfo* (*)(Il2CppClass*, void**);
using il2cpp_method_get_name_t            = const char* (*)(const MethodInfo*);
using il2cpp_method_get_param_count_t     = uint32_t (*)(const MethodInfo*);

struct MethodInfo_Min { void* methodPointer; };

template<class T>
inline T GetExport(const char* n) {
    HMODULE h = GetModuleHandleA("GameAssembly.dll");
    return h ? reinterpret_cast<T>(GetProcAddress(h, n)) : nullptr;
}

// Inline globals (header-only safe)
inline il2cpp_domain_get_t                 p_domain_get                 = nullptr;
inline il2cpp_domain_get_assemblies_t      p_domain_get_assemblies      = nullptr;
inline il2cpp_assembly_get_image_t         p_assembly_get_image         = nullptr;
inline il2cpp_class_from_name_t            p_class_from_name            = nullptr;
inline il2cpp_class_get_method_from_name_t p_class_get_method_from_name = nullptr;

// Optional enums
inline il2cpp_class_get_methods_t          p_class_get_methods          = nullptr;
inline il2cpp_method_get_name_t            p_method_get_name            = nullptr;
inline il2cpp_method_get_param_count_t     p_method_get_param_count     = nullptr;

inline bool EnsureExports() {
    if (!GetModuleHandleA("GameAssembly.dll")) return false;
    if (!p_domain_get)                 p_domain_get                 = GetExport<il2cpp_domain_get_t>("il2cpp_domain_get");
    if (!p_domain_get_assemblies)      p_domain_get_assemblies      = GetExport<il2cpp_domain_get_assemblies_t>("il2cpp_domain_get_assemblies");
    if (!p_assembly_get_image)         p_assembly_get_image         = GetExport<il2cpp_assembly_get_image_t>("il2cpp_assembly_get_image");
    if (!p_class_from_name)            p_class_from_name            = GetExport<il2cpp_class_from_name_t>("il2cpp_class_from_name");
    if (!p_class_get_method_from_name) p_class_get_method_from_name = GetExport<il2cpp_class_get_method_from_name_t>("il2cpp_class_get_method_from_name");

    // optional helpers (they might be null on some builds; we guard uses)
    if (!p_class_get_methods)          p_class_get_methods          = GetExport<il2cpp_class_get_methods_t>("il2cpp_class_get_methods");
    if (!p_method_get_name)            p_method_get_name            = GetExport<il2cpp_method_get_name_t>("il2cpp_method_get_name");
    if (!p_method_get_param_count)     p_method_get_param_count     = GetExport<il2cpp_method_get_param_count_t>("il2cpp_method_get_param_count");

    return p_domain_get && p_domain_get_assemblies && p_assembly_get_image && p_class_from_name && p_class_get_method_from_name;
}

// Resolve by (namespace, class, method, argc); searches all images.
// Logs GameAssembly base, VA and RVA for clarity.
template<class T>
inline T ResolveMethodPtr_AllImages(const char* nspace, const char* klass, const char* method, int argc,
                                    DWORD poll_ms = 100, DWORD timeout_ms = 60000)
{
    // 1) Wait for GameAssembly.dll
    DWORD waited = 0;
    while (!GetModuleHandleA("GameAssembly.dll")) {
        if (waited >= timeout_ms) return nullptr;
        Sleep(poll_ms); waited += poll_ms;
    }

    // 2) Resolve exports
    waited = 0;
    while (!EnsureExports()) {
        if (waited >= timeout_ms) return nullptr;
        Sleep(poll_ms); waited += poll_ms;
    }

    // 3) Wait until assemblies exist
    const Il2CppAssembly** asms = nullptr; size_t count = 0;
    waited = 0;
    do {
        auto dom = p_domain_get();
        asms = p_domain_get_assemblies(dom, &count);
        if (count) break;
        if (waited >= timeout_ms) return nullptr;
        Sleep(poll_ms); waited += poll_ms;
    } while (true);

    // 4) Search each image for the class; then get the method
    for (size_t i = 0; i < count; ++i) {
        auto img = p_assembly_get_image(asms[i]);
        if (!img) continue;
        auto c = p_class_from_name(img, nspace, klass);
        if (!c) continue;

        auto mi = p_class_get_method_from_name(c, method, argc);
        if (!mi) continue;

        auto p = reinterpret_cast<const MethodInfo_Min*>(mi)->methodPointer;
        if (p) {
            char fullname[256];
            _snprintf_s(fullname, _TRUNCATE, "%s.%s::%s(%d)", nspace, klass, method, argc);
            auto addr = reinterpret_cast<uintptr_t>(p);
            auto base = reinterpret_cast<uintptr_t>(GetModuleHandleA("GameAssembly.dll"));
            auto rva  = addr - base;

            // Print base once then VA/RVA each time (replace Menu::AddLog with your logger)
            static bool printedBase = false;
            

            Menu::AddLog("  $ [il2cpp] %-40s => (GameAssembly+0x%llX)\n",
                        fullname,
                        static_cast<unsigned long long>(rva));
            return reinterpret_cast<T>(p);
        }
        // if p == nullptr (generics/virtual), try later or hook vtable
    }
    return nullptr;
}

// Convenience: try both "Foo" and "get_Foo" (property getter) with a given argc.
template<class T>
inline T ResolveMethodOrGetter(const char* nspace, const char* klass, const char* name, int argc)
{
    if (auto p = ResolveMethodPtr_AllImages<T>(nspace, klass, name, argc)) return p;
    char buf[128]; _snprintf_s(buf, _TRUNCATE, "get_%s", name);
    return ResolveMethodPtr_AllImages<T>(nspace, klass, buf, argc);
}

// Optional: resolve by name only (no argc) by enumerating methods and picking the first match.
// Returns nullptr if multiple overloads exist or no pointer is ready.
template<class T>
inline T ResolveMethod_ByNameAuto(const char* nspace, const char* klass, const char* name,
                                DWORD poll_ms = 100, DWORD timeout_ms = 60000)
{
    // quick attempt for 0-arg
    if (auto p0 = ResolveMethodPtr_AllImages<T>(nspace, klass, name, 0, poll_ms, 0)) return p0;

    // Ensure optional exports (for enumeration)
    DWORD waited = 0;
    while (!EnsureExports()) { if (waited >= timeout_ms) return nullptr; Sleep(poll_ms); waited += poll_ms; }
    if (!p_class_get_methods || !p_method_get_name) return nullptr;

    // find class
    size_t count = 0;
    const Il2CppAssembly** asms = p_domain_get_assemblies(p_domain_get(), &count);
    Il2CppClass* c = nullptr;
    for (size_t i = 0; i < count && !c; ++i) {
        auto img = p_assembly_get_image(asms[i]);
        if (!img) continue;
        c = p_class_from_name(img, nspace, klass);
    }
    if (!c) return nullptr;

    // enumerate and pick the first with non-null pointer
    void* iter = nullptr;
    const MethodInfo* hit = nullptr; uint32_t hitArgc = 0; int matches = 0;
    while (const MethodInfo* mi = p_class_get_methods(c, &iter)) {
        const char* nm = p_method_get_name(mi);
        if (!nm || _stricmp(nm, name)) continue;
        ++matches;
        uint32_t argc = p_method_get_param_count ? p_method_get_param_count(mi) : 0;
        auto ptr = reinterpret_cast<const MethodInfo_Min*>(mi)->methodPointer;
        if (ptr && !hit) { hit = mi; hitArgc = argc; }
    }
    if (matches == 1 && hit) {

        char fullname[256];
        _snprintf_s(fullname, _TRUNCATE, "%s.%s::%s(%d)", nspace, klass, name, hitArgc);
        auto addr = reinterpret_cast<uintptr_t>(reinterpret_cast<const MethodInfo_Min*>(hit)->methodPointer);
        auto base = reinterpret_cast<uintptr_t>(GetModuleHandleA("GameAssembly.dll"));
        auto rva  = addr - base;
        Menu::AddLog("  $ [il2cpp] %-40s => (GameAssembly+0x%llX)\n",
                        fullname,
                        static_cast<unsigned long long>(rva));


        return reinterpret_cast<T>(reinterpret_cast<const MethodInfo_Min*>(hit)->methodPointer);
    }
    // ambiguous or not ready
    return nullptr;
}
