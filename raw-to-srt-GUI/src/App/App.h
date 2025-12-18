#pragma once

#include "../AppConfig/AppConfig.h"
#include "../../vendors/srt-to-ndi/rawtondi.h"
#include "../../vendors/imgui/imgui_impl_glfw.h"
#include "../../vendors/imgui/imgui_impl_vulkan.h"
#include "imgui.h"

class App {
private:
    ConfigHelper::Config m_config;
    GLFWwindow* window;
    ImGui_ImplVulkanH_Window* wd;
    ImGuiIO* io;

    void Init();

public:
    App();
    ~App();

    void run();
};
