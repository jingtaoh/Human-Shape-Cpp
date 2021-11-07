//
// Created by Jingtao Huang on 11/2/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_CAMERA_H
#define MOSHAPE_CAMERA_H

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace MoShape {
class Camera
{
public:
    Camera();
    ~Camera();

    static Camera default_camera(float width, float height);

    void set_position(const glm::vec3& pos);
    void set_lookat(const glm::vec3& lookat);
    void set_up(const glm::vec3& up);
    void set_window_dimensions(float width, float height);
    void set_aspect_ratio(float width, float height);
    void set_fov(float fov);
    void set_planes(float znear, float zfar);

    glm::mat4 get_view() const;
    glm::mat4 get_projection() const;

    void detect_window_dimension_change(GLFWwindow* window);

protected:
    void update_view();
    void update_perspective();

    glm::mat4 m_projection;
    glm::mat4 m_view;

    glm::vec3 m_pos;
    glm::vec3 m_up;
    glm::vec3 m_lookat;

    float m_aspect_ratio = 1.0f;
    float m_fov = glm::radians(45.0f);
    float m_znear = 0.1f;
    float m_zfar = 100.0f;

    int m_width;
    int m_height;

    bool dimension_changed;
};
} // namespace MoShape


#endif // MOSHAPE_CAMERA_H
