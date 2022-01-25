//
// Created by Jingtao Huang on 10/26/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_VIEWER_H
#define MOSHAPE_VIEWER_H

#include <string>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include "../core/Model.h"

#define DATA_DIR "data"

namespace MoShape {

class Viewer
{
public:
    struct WindowOptions
    {
        std::string title = "";

        /// Initial window width
        int width = 1024;
        /// Initial window height
        int height = 768;

        /// Major OpenGL Version
        int gl_version_major = 3;
        /// Minor OpenGL Version
        int gl_version_minor = 3;
    };
    /**
     * @brief Creates a window with given title and size and default options
     *
     * @param [in] window_title Window Title
     * @param [in] window_width Initial window width
     * @param [in] window_height Initial window height
     */
    Viewer(const std::string& window_title, int window_width, int window_height);
    /**
     * @brief Creates a window with given options
     *
     * @param [in] window_options Window creation options
     */
    Viewer(const WindowOptions& window_options);
    ~Viewer();

    /**
     * Start a new frame
     */
    void begin_frame();

    /**
     * End current frame and renders it
     */
    void end_frame();

    /**
     * UI action wants the window to close
     *
     * @return window should close
     */
    bool should_close() const;

    /**
     * Runs the viewer until closed by the user or main loop
     */
    bool run();

    /**
     * Returns true if viewer initialized successfully
     */
     bool is_initialized() const;

     GLFWwindow* get_window() const;

     double get_window_ratio() const;

     int get_window_height() const;

     void detect_window_dimension_change();

private:
    bool init_glfw(const WindowOptions& options);
    bool init_imgui(const WindowOptions& options);
    inline void set_up_imgui_style();

    WindowOptions m_initial_window_options;
    GLFWwindow* m_window;

    bool m_initialized = false;

    int m_width;
    int m_height;

};
} // namespace MoShape

#endif // MOSHAPE_VIEWER_H
