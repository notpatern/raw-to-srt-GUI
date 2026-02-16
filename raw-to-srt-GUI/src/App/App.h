#pragma once

#include "../AppConfig/AppConfig.h"
#include "../../vendors/srt-to-ndi/rawtondi.h"
#include "../../vendors/imgui/imgui_impl_glfw.h"
#include "../../vendors/imgui/imgui_impl_vulkan.h"
#include "imgui.h"
#include <atomic>
#include <string_view>
#include <thread>
#include <mutex>
#include <deque>

class App {
private:
    ConfigHelper::Config m_config;
    GLFWwindow* window;
    ImGui_ImplVulkanH_Window* wd;
    ImGuiIO* io;

    std::thread runThread;
    std::atomic_bool streamRunning{false};
    RawToSrt::Runner runner;

    std::deque<std::string> consoleBuffer;
    std::mutex consoleMutex;
    const size_t MAX_CONSOLE_LINES = 1000;

    void BufferConsoleOutput(const std::string& line);
    void Init();

public:
    App();
    ~App();

    void run();
};
