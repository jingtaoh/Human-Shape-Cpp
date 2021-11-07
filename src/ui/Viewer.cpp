//
// Created by Jingtao Huang on 10/26/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#include "Viewer.h"
#include <spdlog/spdlog.h>
#include "Color.h"

namespace MoShape {
Viewer::Viewer(const std::string& window_title, int window_width, int window_height)
    : Viewer(WindowOptions{window_title, window_width, window_height})
{}

Viewer::Viewer(const WindowOptions& window_options)
{
    if (!init_glfw(window_options)) return;

    m_width = window_options.width;
    m_height = window_options.height;

    m_initialized = true;
}

Viewer::~Viewer()
{
    glfwDestroyWindow(m_window);
    glfwTerminate();
}

void Viewer::begin_frame()
{
    assert(m_window);
    glfwMakeContextCurrent(m_window);
    assert(m_window);

    // Clear default frame buffer
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    Color bg_color = Color(0, 0, 0, 0);
    glClearColor(bg_color.r(), bg_color.g(), bg_color.b(), bg_color.a());
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Viewer::end_frame()
{
    // Swap Front and Back buffers (double buffering)
    glfwSwapBuffers(m_window);

    // Pool and process events
    glfwPollEvents();
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

} // namespace MoShape