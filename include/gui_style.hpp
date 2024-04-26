#pragma once
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

void styleGUI(){
    ImGuiStyle& style = ImGui::GetStyle();
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.f,0.f,0.f,0.f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.f,0.f,0.f,0.f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.f,0.f,0.f,0.f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.f,0.f,0.f,0.f);
    style.Colors[ImGuiCol_Button] = ImVec4(222.f/255,49.f/255,99.f/255,102.f/255);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(222.f/255.f,49.f/255.f,99.f/255.f,254.f/255.f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(222.f/255.f,49.f/255.f,99.f/255.f,254.f/255.f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(0.f,0.f,0.f,100.f/255.f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.f,0.f,0.f,254.f/255.f);
}