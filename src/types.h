#pragma once

namespace System
{
	class String
	{
	public:
		char zpad[0x10];
		int size;
		wchar_t buffer[256];
    };

    struct Single_array {
    void*     klass;       // 0x00
    void*     monitor;     // 0x08
    void*     bounds;      // 0x10 (Il2CppArrayBounds*, null for 1D but still present)
    uint64_t  max_length;  // 0x18
    float     m_Items[1];  // 0x20
    };


};

