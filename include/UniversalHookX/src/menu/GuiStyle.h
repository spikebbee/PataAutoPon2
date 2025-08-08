#include "../dependencies/imgui/imgui.h"

// From https://github.com/procedural/gpulib/blob/master/gpulib_imgui.h
struct ImVec3 { float x, y, z; ImVec3(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) { x = _x; y = _y; z = _z; } };






ImVec4 player_color = ImVec4(17.0f/255.f, 152.0f/255.f, 222.0f/255.f, 1.0f);
void SetupImGuiStyle()
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.Colors[ImGuiCol_Text]             = ImVec4( 0.0,0.0,0.0,1.0);  //text
    style.Colors[ImGuiCol_TextDisabled ]    = ImVec4( 0.0,0.0,0.0,1.0);
    style.Colors[ImGuiCol_WindowBg]         = ImVec4(221.0f/255.f, 221.0f/255.f, 221.0f/255.f, 110.f/255.f );   //window under header

    style.Colors[ImGuiCol_TitleBg]          = player_color;  // Header color
    style.Colors[ImGuiCol_TitleBgCollapsed] = player_color;  // Header color when hovered
    style.Colors[ImGuiCol_TitleBgActive]    = player_color;  // Header color when active

    style.Colors[ImGuiCol_SliderGrab]       = player_color; 
    style.Colors[ImGuiCol_SliderGrabActive] = player_color; 
    style.Colors[ImGuiCol_PlotHistogram]   =  ImVec4(252.0f/255.f, 163.0f/255.f, 17.0f/255.f,1.0f);

    style.Colors[ImGuiCol_Separator] = player_color; 
    
    // style.Colors[ImGuiCol_Button        ]  = ImVec4( 221.f/225.f, 115.f/225.f, 71.f/225.f, 0.0f);  
    // style.Colors[ImGuiCol_ButtonHovered ]  = ImVec4( 255.f/225.f, 135.f/225.f,  0.f/225.f, 0.0f);  
    // style.Colors[ImGuiCol_ButtonActive  ]  = ImVec4( 221.f/225.f, 115.f/225.f, 71.f/225.f, 0.0f);  


    style.Colors[ImGuiCol_Button        ]  = ImVec4( 221.f/225.f, 115.f/225.f, 71.f/225.f, 0.8f);  
    style.Colors[ImGuiCol_ButtonHovered ]  = ImVec4( 255.f/225.f, 135.f/225.f,  0.f/225.f, 1.0f);  
    style.Colors[ImGuiCol_ButtonActive  ]  = ImVec4( 221.f/225.f, 115.f/225.f, 71.f/225.f, 1.0f);  


    //change button to right
    style.WindowMenuButtonPosition = ImGuiDir_Left;
    style.WindowBorderSize = 1.0f;    // Set window border size
    style.FrameBorderSize  = 0.0f;     // Set frame border size
    style.WindowRounding = 5.0f;      // Round the corners of the window
    style.FrameRounding = 2.0f;
    style.FramePadding   = ImVec2(2.87f,2.87f); // 
    style.WindowPadding  = ImVec2(6.40f, 6.4f); // 



}