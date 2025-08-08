#pragma warning(disable: 4819)


#include "menu.hpp"
#include <cstdio>  

#include "../dependencies/imgui/imgui.h"
#include "../dependencies/imgui/imgui_impl_win32.h"
#include "GuiStyle.h"
#include<vector>
#include <Patapon2Class.h>
#include <cmath>
#include <string>
namespace ig = ImGui;
static ImFont* smallFont ;
static ImFont* smallFontK ;


Weather Menu::WeatherSetting;
GameHack Menu::GameHackSetting;

// "0. Forward: Pata-Pata-Pata-Pon", 
// "1. Attack:  Pon-Pon-Pata-Pon",
// "2. Defense: Chaka-Chaka-Pata-Pon",
// "3. Retreat: Pon-Pata-Pon-Pata",
// "4. Charge:  Pon-Pon-Chaka-Chaka",
// "5. Jump:    Don-Don-Chaka-Chaka",
// "6. Miracle: Don-DoDon-DoDon",
// "7. Stop Move and Clear Command"
uint8_t Menu::KeyConfig[8] = {'W', 'A', 'D', 'R', 'C', 'J', 'M', 'Q'}; 

namespace Menu {
    void InitializeContext(HWND hwnd) {
        if (ig::GetCurrentContext( ))
            return;

        ImGui::CreateContext( );
        ImGui_ImplWin32_Init(hwnd);

        ImGuiIO& io = ImGui::GetIO();
        ImGui::GetIO().FontGlobalScale = 2.0f;
        // Normal UI font (scaled 2×)
        ImGui::GetIO().Fonts->AddFontDefault();  // default is ~13.0f → appears ~26px

        smallFont  = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\consola.ttf", 10.0f);  // Used for log view
        // smallFontK = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\consola.ttf", 11.0f);  // Used for log view


        io.IniFilename = io.LogFilename = nullptr;

        io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;  // Ensure mouse is allowed
        io.MouseDrawCursor = true;                   // Let OS handle the cursor

        SetupImGuiStyle();







    }

    void AddLog(const char* fmt, ...) {
        va_list args;
        va_start(args, fmt);
        logBuffer.appendfv(fmt, args);
        scrollToBottom = true;
        va_end(args);

        va_start(args, fmt);            // Reset args for printf
        vprintf(fmt, args);             // Print to console
        va_end(args);
        
    }



    
    void drawMainWindow()
    {
        //start draw
        ImGuiStyle& style = ImGui::GetStyle();



        //window
        ImGui::SetNextWindowPos(ImVec2(5, 400), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(500, -1), ImGuiCond_Always);
        ImGui::Begin(mainWindowName,nullptr,ImGuiWindowFlags_NoFocusOnAppearing|ImGuiWindowFlags_NoResize);     

    if (ImGui::BeginTabBar("x"))
    {
        if (ImGui::BeginTabItem("Main"))//layout
        {

            //beat
            ImGui::Text(reinterpret_cast<const char*>(u8"[Beat Wave]\n")); 
            static float Ticks[TickSize]; 
            ImVec2 graphSize(0.0f, 50.0f);
            char buffer[128];
            auto r_size = Sound_BeatCommander::TickRecord.size();
            if(r_size>0)
                sprintf_s(buffer,reinterpret_cast<const char*>(u8"%u"),Sound_BeatCommander::TickRecord[r_size-1]);
            else
                sprintf_s(buffer,reinterpret_cast<const char*>(u8""));
                
            for(int i=0;i<Sound_BeatCommander::TickRecord.size();i++)
            {
                Ticks[Sound_BeatCommander::TickRecord.size()-i]=(Sound_BeatCommander::TickRecord[i]*1.0f);
            }

            ImGui::PlotHistogram(buffer, Ticks, TickSize, 0, NULL, 0.0f, 1200.0f,graphSize);



            //setting
            ImGui::Separator();
            if (ImGui::CollapsingHeader("Setting", ImGuiTreeNodeFlags_DefaultOpen))
            {
                // ImGui::Text(reinterpret_cast<const char*>(u8"[setting]"));     
                //transparency
                static float backgroundAlpha = 25*1.0f;    
                auto Sstate = ImGui::SliderFloat(reinterpret_cast<const char*>(u8"Gui Alpha"), &backgroundAlpha    , 0.0f, 100.0f, "%.0f%%"); 
                style.Colors[ImGuiCol_WindowBg]       =  ImVec4(221.0f/255.f, 221.0f/255.f, 221.0f/255.f, (1-backgroundAlpha/100.0f));   
                style.Colors[ImGuiCol_FrameBg]        =  ImVec4(221.0f/255.f, 221.0f/255.f, 221.0f/255.f, (1-backgroundAlpha/100.0f)); //widgets background like slider.
                style.Colors[ImGuiCol_FrameBgHovered] =  ImVec4(221.0f/255.f, 221.0f/255.f, 221.0f/255.f, (1-backgroundAlpha/100.0f)*1.7f); //widgets background like slider.
                style.Colors[ImGuiCol_FrameBgActive]  =  ImVec4(221.0f/255.f, 221.0f/255.f, 221.0f/255.f, (1-backgroundAlpha/100.0f)*1.7f); //widgets background like slider.
                style.Colors[ImGuiCol_PopupBg] = ImVec4(221.0f/255.f, 221.0f/255.f, 221.0f/255.f,(1-backgroundAlpha/100.0f)*1.7f+0.5f);

                //Key Press
                const char* items[] = { "Key Injection", "XboxCon Mimic", "Keyboard Mimic" };
                if (ImGui::BeginCombo("Control", items[GameHackSetting.KeyMethod])) // Label and preview
                {
                    for (int n = 0; n < IM_ARRAYSIZE(items); n++)
                    {
                        bool is_selected = (GameHackSetting.KeyMethod == n);
                        if (ImGui::Selectable(items[n], is_selected))
                            GameHackSetting.KeyMethod = n;

                    }
                    ImGui::EndCombo();
                }

                
                ImGui::Checkbox("Repeat Command", &autoAppendCommand);

                ImGui::Checkbox("Perfect Miracle", &GameHackSetting.PerfectSubGame);

                ImGui::Checkbox("Invincible", &GameHackSetting.Invincible);
                ImGui::Checkbox("Item Num > 0", &GameHackSetting.NoZeroItem);
                


                // --- Damage Multiplier (sqrt nonlinear slider) ---
                ImGui::SetNextItemWidth(120.0f);
                {
                    float curveSlider = sqrtf(sqrtf(GameHackSetting.DamageMultiplier));  // always reflect current value
                    if (ImGui::SliderFloat("##DamageMultiplierSlider", &curveSlider, sqrtf(sqrtf(0.2f)), sqrtf(sqrtf(100.0f)), "")) {
                        GameHackSetting.DamageMultiplier = curveSlider * curveSlider * curveSlider * curveSlider;
                    }
                    ImGui::SameLine();
                    ImGui::Text("Damage Multiplier = %.1f", GameHackSetting.DamageMultiplier);
                }


                // --- Miracle TimeScale (sqrt nonlinear slider) ---
                ImGui::SetNextItemWidth(120.0f);
                {
                    float sqrtValue = sqrtf(sqrtf(GameHackSetting.MiracleScale));  // always reflect current value
                    if (ImGui::SliderFloat("##MiracleTimeScaleSlider", &sqrtValue, sqrtf(sqrtf(0.5f)), sqrtf(sqrtf(200.0f)), "")) {
                        GameHackSetting.MiracleScale = sqrtValue * sqrtValue * sqrtValue * sqrtValue;
                    }
                    ImGui::SameLine();
                    ImGui::Text("Miracle TimeScale = %.1f", GameHackSetting.MiracleScale);
                }



            }

            ImGui::Separator();    
            if (ImGui::CollapsingHeader("Weather", ImGuiTreeNodeFlags_DefaultOpen))
            {
                //weather
                uint32_t minValue = 0;
                uint32_t maxValue = 4;

                ImGui::SetNextItemWidth(130.0f); ImGui::Checkbox("Enable -> ", &WeatherSetting.Enable);  ImGui::SameLine(); 
                // ImGui::SetNextItemWidth(130.0f); ImGui::SliderScalar("Weather Id", ImGuiDataType_U32, &WeatherSetting.WeatherId,  &minValue, &maxValue);
                ImGui::SetNextItemWidth(250.0f);
                const char* items2[] = { "0: Usual/Fogy", "1: Rainy", "2: Snow" };
                if (ImGui::BeginCombo("##cw", items2[WeatherSetting.WeatherId])) // Label and preview
                {
                    for (int n = 0; n < IM_ARRAYSIZE(items2); n++)
                    {
                        bool is_selected2 = (WeatherSetting.WeatherId == n);
                        if (ImGui::Selectable(items2[n], is_selected2))
                            WeatherSetting.WeatherId = n;
                        if (is_selected2)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }


                ImGui::Separator();
                
                ImGui::SetNextItemWidth(130.0f); ImGui::SliderScalar("Wind  ",  ImGuiDataType_U32, &WeatherSetting.WindLevel,   &minValue, &maxValue);
                ImGui::Text("Wind_Dir:");ImGui::SameLine(); 
                ImGui::SetNextItemWidth(135.0f); ImGui::InputFloat("Vx ", &WeatherSetting.WindDirection_X, 0.1f, 1.0f, "%.1f");ImGui::SameLine(); 
                ImGui::SetNextItemWidth(135.0f); ImGui::InputFloat("Vy",   &WeatherSetting.WindDirection_Y, 0.1f, 1.0f, "%.1f");

                ImGui::Separator();

                ImGui::SetNextItemWidth(130.0f); ImGui::SliderScalar("Rainy  ", ImGuiDataType_U32, &WeatherSetting.RainyLevel,  &minValue, &maxValue);ImGui::SameLine(); 
                ImGui::SetNextItemWidth(130.0f); ImGui::SliderScalar("Cloudy ", ImGuiDataType_U32, &WeatherSetting.CloudyLevel, &minValue, &maxValue);
                ImGui::SetNextItemWidth(130.0f); ImGui::SliderScalar("Snow   ", ImGuiDataType_U32, &WeatherSetting.SnowLevel,   &minValue, &maxValue);ImGui::SameLine(); 
                ImGui::SetNextItemWidth(130.0f); ImGui::SliderScalar("Fog    ", ImGuiDataType_U32, &WeatherSetting.FogLevel,    &minValue, &maxValue);
                ImGui::SetNextItemWidth(130.0f); ImGui::SliderScalar("Sand   ", ImGuiDataType_U32, &WeatherSetting.SandLevel,   &minValue, &maxValue);ImGui::SameLine(); 
                ImGui::SetNextItemWidth(130.0f); ImGui::SliderScalar("Thunder", ImGuiDataType_U32, &WeatherSetting.ThunderLevel,&minValue, &maxValue);

            }
            
            // Inside your rendering logic
            if (ImGui::CollapsingHeader("Log", ImGuiTreeNodeFlags_DefaultOpen))
            {
                logFilter.Draw("##logfilter", 100); ImGui::SameLine(); ImGui::Text("Filter"); 
                ImGui::Checkbox("Auto-scroll", &autoScroll);


                //button striker section
                {


                    ImVec2 lineStart = ImGui::GetCursorScreenPos();  // Current line start (top-left)
                    float radius = 20.0f;
                    auto bg_color = IM_COL32(15,30, 40, 180);
                    int key_flash = 255;
                    auto now = std::chrono::system_clock::now();
                    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(now - Sound_BeatCommander::lastMoveTime).count();


                    auto x0=300;
                    auto y0=-50;


                    //Y key
                    ImVec2 center  = ImVec2(lineStart.x + x0 + radius*3.5f + radius,   lineStart.y + y0 + radius);
                    auto key_color = IM_COL32(255, 205, 5,200);
                    ImGui::GetWindowDrawList()->AddCircleFilled(center, radius, bg_color);
                    ImGui::GetWindowDrawList()->AddCircle      (center, radius, key_color, 32, 4.0f);

                    auto flash_color =  IM_COL32((key_color >> 0)  & 0xFF, (key_color >> 8)  & 0xFF, (key_color >> 16)  & 0xFF, 255-std::sqrt(diff));
                    if(diff>0 && diff<key_flash && Sound_BeatCommander::lastMove==4)
                    {
                        ImGui::GetWindowDrawList()->AddCircleFilled(center, radius,   flash_color);
                        ImGui::GetWindowDrawList()->AddCircle      (center, radius+6, flash_color, 32, 3.0f);
                        
                    }


                    ImGui::SetCursorScreenPos(ImVec2(center.x-8, center.y-13));

                    ImGui::PushStyleColor(ImGuiCol_Text, key_color); 
                    ImGui::Text("Y");
                    ImGui::PopStyleColor();  // Revert text color

                    //X key
                    center  = ImVec2(lineStart.x + x0 + radius*1.5f + radius,   lineStart.y + y0 + 3.0f*radius);
                    key_color = IM_COL32(66, 133, 244, 255);
                    ImGui::GetWindowDrawList()->AddCircleFilled(center, radius, bg_color);
                    ImGui::GetWindowDrawList()->AddCircle      (center, radius, key_color, 32, 4.0f);
                    flash_color =  IM_COL32((key_color >> 0)  & 0xFF, (key_color >> 8)  & 0xFF, (key_color >> 16)  & 0xFF, 255-std::sqrt(diff));

                    if(diff>0 && diff<key_flash && Sound_BeatCommander::lastMove==3)
                    {
                        ImGui::GetWindowDrawList()->AddCircleFilled(center, radius,   flash_color);
                        ImGui::GetWindowDrawList()->AddCircle      (center, radius+6, flash_color, 32, 3.0f);
                    }

                    ImGui::SetCursorScreenPos(ImVec2(center.x-8, center.y-13));
                    
                    ImGui::PushStyleColor(ImGuiCol_Text, key_color ); 
                    ImGui::Text("X");
                    ImGui::PopStyleColor();  // Revert text color

                    //B key
                    center  = ImVec2(lineStart.x + x0 + radius*5.5f + radius,   lineStart.y + y0 + 3.0f*radius);
                    key_color = IM_COL32(252,50,5, 255);
                    ImGui::GetWindowDrawList()->AddCircleFilled(center, radius, bg_color);
                    ImGui::GetWindowDrawList()->AddCircle      (center, radius, key_color, 32, 4.0f);
                    flash_color =  IM_COL32((key_color >> 0)  & 0xFF, (key_color >> 8)  & 0xFF, (key_color >> 16)  & 0xFF, 255-std::sqrt(diff));

                    if(diff>0 && diff<key_flash && Sound_BeatCommander::lastMove==1)
                    {
                        ImGui::GetWindowDrawList()->AddCircleFilled(center, radius,   flash_color);
                        ImGui::GetWindowDrawList()->AddCircle      (center, radius+6, flash_color, 32, 3.0f);
                    }

                    ImGui::SetCursorScreenPos(ImVec2(center.x-8, center.y-13));
                    
                    ImGui::PushStyleColor(ImGuiCol_Text, key_color ); 
                    ImGui::Text("B");
                    ImGui::PopStyleColor();  // Revert text color


                    //A key
                    center  = ImVec2(lineStart.x + x0 + radius*3.5f + radius,   lineStart.y + y0 + 5.0f*radius);
                    key_color = IM_COL32(42, 242, 12, 255);
                    ImGui::GetWindowDrawList()->AddCircleFilled(center, radius, bg_color);
                    ImGui::GetWindowDrawList()->AddCircle      (center, radius, key_color, 32, 4.0f);
                    flash_color =  IM_COL32((key_color >> 0)  & 0xFF, (key_color >> 8)  & 0xFF, (key_color >> 16)  & 0xFF, 255-std::sqrt(diff));

                    if(diff>0 && diff<key_flash && Sound_BeatCommander::lastMove==2)
                    {
                        ImGui::GetWindowDrawList()->AddCircleFilled(center, radius,   flash_color);
                        ImGui::GetWindowDrawList()->AddCircle      (center, radius+6, flash_color, 32, 3.0f);
                    }

                    ImGui::SetCursorScreenPos(ImVec2(center.x-8, center.y-13));
                    
                    ImGui::PushStyleColor(ImGuiCol_Text, key_color ); 
                    ImGui::Text("A");
                    ImGui::PopStyleColor();  // Revert text color




                }



                ImGui::NewLine();

                ImGui::Separator();

                ImGui::BeginChild("LogRegion", ImVec2(0, 200), true, ImGuiWindowFlags_HorizontalScrollbar);
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 1));

                const char* buf = logBuffer.begin();
                const char* buf_end = logBuffer.end();

                const char* last_line = nullptr;

                if (logFilter.IsActive())
                {
                    for (const char* line_start = buf; line_start < buf_end;)
                    {
                        const char* line_end = strchr(line_start, '\n');
                        if (!line_end) line_end = buf_end;

                        if (logFilter.PassFilter(line_start, line_end))
                        {
                            if (smallFont) ImGui::PushFont(smallFont);
                            ImGui::PushTextWrapPos(0.0f);
                            ImGui::TextUnformatted(line_start, line_end);
                            ImGui::PopTextWrapPos();
                            if (smallFont) ImGui::PopFont();

                            last_line = line_start; // Track last rendered line
                        }

                        line_start = line_end + 1;
                    }
                }
                else
                {
                    for (const char* line_start = buf; line_start < buf_end;)
                    {
                        const char* line_end = strchr(line_start, '\n');
                        if (!line_end) line_end = buf_end;

                        if (smallFont) ImGui::PushFont(smallFont);
                        ImGui::PushTextWrapPos(0.0f);
                        ImGui::TextUnformatted(line_start, line_end);
                        ImGui::PopTextWrapPos();
                        if (smallFont) ImGui::PopFont();

                        last_line = line_start; // Track last rendered line
                        line_start = line_end + 1;
                    }
                }

                // Scroll to last line
                if (autoScroll && scrollToBottom && last_line)
                    ImGui::SetScrollHereY(1.0f);

                // Update scroll lock based on user action
                scrollToBottom = (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 5.0f);

                ImGui::PopStyleVar();
                ImGui::EndChild();

            }

        ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Inventory"))
        {            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(120.0f/255.0f, 0.0f, 0.0f, 1.0f)); // Red color (RGBA)
            ImGui::Text("Inventory Operation May Cause");
            ImGui::Text("Mem-leak Issues ! ");
            ImGui::Text("Enter Inventory to View");
            ImGui::PopStyleColor();
            ImGui::Separator();
            static bool autoFocusItem=true;
            static int addItemBy=1;

            ImGui::Checkbox("Auto-focus   ", &autoFocusItem);
            ImGui::SameLine();
            ImGui::SetNextItemWidth(130.0f); ImGui::SliderInt("Add Item", &addItemBy, 1, 9, "%i");

            ImVec2 child_size = ImVec2(0, 500); // fixed or max height
            ImGui::BeginChild("Items", child_size, true, ImGuiWindowFlags_AlwaysVerticalScrollbar);

                for (const auto& pair : GameSystem_Item_GlobalData::inventory)
                {
                        auto slot = pair.first;
                        auto num = pair.second.second;
                        auto itemId = pair.second.first;

                        // draw the selected slot
                        if(Bases_Item_Slot:: selectedSlot==slot)
                        {

                                char buf[16];
                                snprintf(buf, sizeof(buf), "%03d", num);

                                // Get text size and position
                                ImVec2 textPos = ImGui::GetCursorScreenPos();
                                ImVec2 textSize = ImGui::CalcTextSize(buf);
                                // Draw background rectangle (e.g., semi-transparent blue)
                                ImDrawList* draw_list = ImGui::GetWindowDrawList();
                                ImU32 bgColor = IM_COL32(0, 100, 200, 100); // RGBA
                                ImU32 borderCol = IM_COL32(222, 0, 0, 255);    // Solid red border
                                float padding = 4.0f;
                                draw_list->AddRectFilled(
                                    ImVec2(textPos.x - padding, textPos.y - padding),
                                    ImVec2(textPos.x + textSize.x + padding, textPos.y + textSize.y + padding),
                                    bgColor, 4.0f  // corner rounding
                                );
                                // Draw red border
                                draw_list->AddRect(
                                    ImVec2(textPos.x - padding, textPos.y - padding),
                                    ImVec2(textPos.x + textSize.x + padding, textPos.y + textSize.y + padding),
                                    borderCol, 4.0f, 0, 4.0f // thickness = 1.5
                                );

                                if(autoFocusItem) ImGui::SetScrollHereY();  // Scrolls this item into view vertically
                                //  Bases_Item_Slot:: selectedSlot = -1;
                        }
                        ImGui::Text("%03d", num);

                        ImGui::SameLine();    
                        std::string buttonId = "+" + std::to_string(addItemBy) +"##"+std::to_string(slot);
                        if (ImGui::Button(buttonId.c_str()))
                        {
                            GameSystem_Item_Operator::TestAddItem(itemId,addItemBy);
                        }
                        if( (1+pair.first) %4!=0)
                        {       
                            ImGui::SameLine(); 
                            ImGui::Dummy(ImVec2(28, 0)); 
                            ImGui::SameLine();
                        }
                        else
                        {
                            ImGui::Separator();

                        }
                }

            ImGui::EndChild();

            ImGui::EndTabItem();
        }

        
        if (ImGui::BeginTabItem("Control Config"))
        {  


                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(120.0f/255.0f, 0.0f, 0.0f, 1.0f)); // Red color (RGBA)
                    ImGui::Text("Click a Key to Change Setting.");
                    ImGui::Text("Don't Use The Key That is ");
                    ImGui::Text(" Already Used by Game.");
                    ImGui::PopStyleColor();
                    ImGui::Separator();

                static int CommandIndex=0;
                const char* items3[] = {
                    "0. Forward: Pata-Pata-Pata-Pon", 
                    "1. Attack:  Pon-Pon-Pata-Pon",
                    "2. Defense: Chaka-Chaka-Pata-Pon",
                    "3. Retreat: Pon-Pata-Pon-Pata",
                    "4. Charge:  Pon-Pon-Chaka-Chaka",
                    "5. Jump:    Don-Don-Chaka-Chaka",
                    "6. Miracle: Don-DoDon-DoDon",
                    "7. Stop Move and Clear Command"
                };

                if (ImGui::BeginCombo("##cc", items3[CommandIndex])) // Label and preview
                {
                    for (int n = 0; n < IM_ARRAYSIZE(items3); n++)
                    {
                        bool is_selected3 = (CommandIndex == n);
                        if (ImGui::Selectable(items3[n], is_selected3)) // <-- FIX HERE
                            CommandIndex = n;
                        if (is_selected3)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
                
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(120.0f/255.0f, 0.0f, 0.0f, 1.0f)); // Red color (RGBA)
                char buf[2] = { KeyConfig[CommandIndex], '\0' };
                ImGui::SameLine(); ImGui::Dummy(ImVec2(20, 0)); ImGui::SameLine(); ImGui::Text(buf);
                ImGui::PopStyleColor();
                
                ImGui::Separator();
                ImGui::Text("Current Key in Use:");
                for(int i=0; i<8;i++)
                {
                    char buf[4] = { '[', KeyConfig[i], ']','\0' };
                    ImGui::Text(buf);
                    ImGui::SameLine();

                }
                ImGui::NewLine();

                ImGui::Separator();


                ImGui::NewLine();

                if (smallFont) ImGui::PushFont(smallFont);


                const char row1[] = { 'Q','W','E','R','T','Y','U','I','O','P' };
                const char row2[] = { 'A','S','D','F','G','H','J','K','L' };
                const char row3[] = { 'Z','X','C','V','B','N','M' };

                DrawKeyRow(row1, sizeof(row1), CommandIndex);

                ImGui::Dummy(ImVec2(15, 0)); ImGui::SameLine();
                DrawKeyRow(row2, sizeof(row2), CommandIndex);

                ImGui::Dummy(ImVec2(30, 0)); ImGui::SameLine();
                DrawKeyRow(row3, sizeof(row3), CommandIndex);


                if (smallFont) ImGui::PopFont();



            ImGui::EndTabItem();
        }

    }

        ImGui::End();
    }//main window end
    


    void DrawKeyRow(const char* keys, int count, int CommandIndex)
    {
        

        bool pushed = false;

        for (int i = 0; i < count; i++)
        {
            char label[16];
            snprintf(label, sizeof(label), " %c ##KeyBtn", keys[i]); // Unique ID to avoid ImGui conflicts

                if(keys[i]==KeyConfig[CommandIndex])
                {
                    ImGui::PushStyleColor(ImGuiCol_Button,       ImVec4(0.85f, 0.20f, 0.20f, 1.0f)); // Normal: rich red
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.95f, 0.30f, 0.30f, 1.0f)); // Active: lighter red
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4(0.90f, 0.25f, 0.25f, 1.0f)); // Hover: mid red
                    pushed = true;
                }
                else
                {
                    ImGui::PushStyleColor(ImGuiCol_Button,       ImVec4(233.0f/255.0f, 224.0f/255.0f, 210.0f/255.0f, 1.0f)); 
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(243.0f/255.0f, 234.0f/255.0f, 220.0f/255.0f, 1.0f)); 
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,ImVec4(213.0f/255.0f, 204.0f/255.0f, 190.0f/255.0f, 1.0f)); 
                }


            if (ImGui::Button(label))
                Menu::KeyConfig[CommandIndex] = keys[i];


                    ImGui::PopStyleColor(); 
                    ImGui::PopStyleColor(); 
                    ImGui::PopStyleColor(); 
            


            if (i != count - 1) // spacing
                ImGui::SameLine();
        }
    }

        void Render()
        {
            if (!bShowMenu)
                return;

            if (Menu::bShowMenu)
            {
                // Force cursor visible
                while (ShowCursor(TRUE) < 0);
                // SetCursor(LoadCursor(NULL, IDC_ARROW));

                // Cancel any capture or confinement
                ReleaseCapture();
                SetCapture(NULL);
                ClipCursor(NULL);
            }

            drawMainWindow();
        }
} // namespace Menu
