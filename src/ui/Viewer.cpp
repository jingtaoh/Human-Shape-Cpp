//
// Created by Jingtao Huang on 10/26/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#include "Viewer.h"
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <spdlog/spdlog.h>
#include "Color.h"

namespace MoShape {
Viewer::Viewer(const std::string& window_title, int window_width, int window_height)
    : Viewer(WindowOptions{window_title, window_width, window_height})
{}

Viewer::Viewer(const WindowOptions& window_options)
    : m_initial_window_options(window_options)
{
    if (!init_glfw(window_options)) return;

    if (!init_imgui(window_options)) return;

    m_width = window_options.width;
    m_height = window_options.height;

    m_initialized = true;
}

Viewer::~Viewer()
{
    // Cleanup for ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Viewer::begin_frame()
{
    assert(m_window);
    glfwMakeContextCurrent(m_window);
    assert(m_window);

    // Poll and handle events
    glfwPollEvents();

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

//    ImGui::ShowDemoWindow();

    // Clear default frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    Color bg_color = Color(0.22, 0.22, 0.22, 1);
    glClearColor(bg_color.r(), bg_color.g(), bg_color.b(), bg_color.a());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Viewer::end_frame()
{
    // Rendering
    ImGui::Render(); // note: this renders to imgui's vertex buffers, not to screen
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // render to screen buffer

    // Swap Front and Back buffers (double buffering)
    glfwSwapBuffers(m_window);
}

bool Viewer::should_close() const
{
    return glfwWindowShouldClose(m_window);
}

bool Viewer::run()
{
    if (!is_initialized()) return false;

    while (!should_close()) {
        begin_frame();
        end_frame();
    }
    return true;
}

GLFWwindow* Viewer::get_window() const
{
    return m_window;
}

bool Viewer::is_initialized() const
{
    return m_initialized;
}

double Viewer::get_window_ratio() const
{
    return (double)m_width / (double)m_height;
}

int Viewer::get_window_height() const
{
    return m_height;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool Viewer::init_glfw(const WindowOptions& options)
{
    // Initialize the GLFW library
    if (!glfwInit()) {
        spdlog::error("Failed to initialize GLFW");
        return false;
    }

    // Set the opengl version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, options.gl_version_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, options.gl_version_minor);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const char* glfw_version = glfwGetVersionString();
    spdlog::info("GLFW compile time version: {}", glfw_version);
    spdlog::info(
        "Requested context: {}.{}, GLSL {}",
        options.gl_version_major,
        options.gl_version_minor,
        "#version 330");

    // Create the window
    m_window =
        glfwCreateWindow(options.width, options.height, options.title.c_str(), nullptr, nullptr);

    if (!m_window) {
        glfwTerminate();
        spdlog::error("Failed to create window");
        return false;
    }

    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);

    // Make the window's context current
    glfwMakeContextCurrent(m_window);

    gl3wInit();

    spdlog::info("OpenGL Driver");
    spdlog::info("Vendor: {}", glGetString(GL_VENDOR));
    spdlog::info("Renderer: {}", glGetString(GL_RENDERER));
    spdlog::info("Version: {}", glGetString(GL_VERSION));
    spdlog::info("Shading language version: {}", glGetString(GL_SHADING_LANGUAGE_VERSION));

    return true;
}

bool Viewer::init_imgui(const WindowOptions& options)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.Fonts->AddFontFromFileTTF(DATA_DIR "/bmonofont-i18n.ttf", 15.0f);

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    set_up_imgui_style();

    // Setup Platform/Renderer backends
    const char* glsl_version = "#version 330";
    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    return true;
}

void Viewer::set_up_imgui_style()
{
    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowRounding = 5.0f;
    style.FrameRounding = 5.0f;
    style.PopupRounding = 5.0f;
    style.Colors[ImGuiCol_WindowBg]               = ImVec4(0.26f, 0.26f, 0.26f, 0.94f);
    style.Colors[ImGuiCol_PopupBg]                = ImVec4(0.13f, 0.13f, 0.13f, 0.94f);
    style.Colors[ImGuiCol_Border]                 = ImVec4(0.12f, 0.12f, 0.12f, 0.50f);
    style.Colors[ImGuiCol_FrameBg]                = ImVec4(0.35f, 0.35f, 0.35f, 0.54f);
    style.Colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.50f, 0.50f, 0.50f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive]          = ImVec4(0.50f, 0.50f, 0.50f, 0.67f);
    style.Colors[ImGuiCol_TitleBg]                = ImVec4(0.22f, 0.22f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_TitleBgActive]          = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
    style.Colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_CheckMark]              = ImVec4(0.78f, 0.53f, 0.32f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]             = ImVec4(0.78f, 0.53f, 0.32f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.78f, 0.53f, 0.32f, 1.00f);
    style.Colors[ImGuiCol_Button]                 = ImVec4(0.41f, 0.41f, 0.41f, 0.40f);
    style.Colors[ImGuiCol_ButtonHovered]          = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]           = ImVec4(0.43f, 0.43f, 0.43f, 1.00f);
    style.Colors[ImGuiCol_Header]                 = ImVec4(0.26f, 0.26f, 0.26f, 0.31f);
    style.Colors[ImGuiCol_HeaderHovered]          = ImVec4(0.26f, 0.26f, 0.26f, 0.31f);
    style.Colors[ImGuiCol_HeaderActive]           = ImVec4(0.26f, 0.26f, 0.26f, 0.31f);
    style.Colors[ImGuiCol_Separator]              = ImVec4(0.14f, 0.14f, 0.14f, 0.50f);
    style.Colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.14f, 0.14f, 0.14f, 0.78f);
    style.Colors[ImGuiCol_SeparatorActive]        = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip]             = ImVec4(0.78f, 0.53f, 0.32f, 0.20f);
    style.Colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.78f, 0.53f, 0.32f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.78f, 0.53f, 0.32f, 0.95f);
    style.Colors[ImGuiCol_Tab]                    = ImVec4(0.14f, 0.14f, 0.14f, 0.86f);
    style.Colors[ImGuiCol_TabHovered]             = ImVec4(0.26f, 0.26f, 0.26f, 0.80f);
    style.Colors[ImGuiCol_TabActive]              = ImVec4(0.26f, 0.26f, 0.26f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]         = ImVec4(1.00f, 0.60f, 0.00f, 0.35f);
}

} // namespace MoShape