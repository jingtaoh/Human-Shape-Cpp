//
// Created by Jingtao Huang on 11/2/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

namespace MoShape {
Camera::Camera()
{
    m_projection = glm::mat4(1.0f);
    m_view = glm::mat4(1.0f);

    m_pos = glm::vec3(0, 0, -180);
    m_up = glm::vec3(0, 1, 0);
    m_lookat = glm::vec3(0, 0, 0);

    m_width = 1024;
    m_height = 768;
}
Camera::~Camera() {}

Camera Camera::default_camera(float width, float height)
{
    Camera c;
    c.set_up(glm::vec3(0, 1, 0));
//    c.set_position(glm::vec3(80, 80, 180));
    c.set_position(glm::vec3(0, 0, 180));
    c.set_lookat(glm::vec3(0, 0, 0));
    c.set_window_dimensions(width, height);
    c.set_fov(glm::radians(60.0f));
    c.set_planes(0.1f, 500.0f);

    return c;
}

void Camera::set_position(const glm::vec3& pos)
{
    m_pos = pos;
    update_view();
}

void Camera::set_lookat(const glm::vec3& lookat)
{
    m_lookat = lookat;
    update_view();
}

void Camera::set_up(const glm::vec3& up)
{
    m_up = up;
    update_view();
}

void Camera::set_window_dimensions(float width, float height)
{
    m_width = width;
    m_height = height;
    set_aspect_ratio(m_width, m_height);
}

void Camera::set_aspect_ratio(float width, float height)
{
    m_aspect_ratio = float(width) / float(height);
    update_perspective();
}

void Camera::set_fov(float fov)
{
    m_fov = fov;
    update_perspective();
}

void Camera::set_planes(float znear, float zfar)
{
    m_znear = znear;
    m_zfar = zfar;
    update_perspective();
}

glm::mat4 Camera::get_view() const
{
    return m_view;
}

glm::mat4 Camera::get_projection() const
{
    return m_projection;
}

void Camera::update_view()
{
    m_view = glm::lookAt(m_pos, m_lookat, m_up);
}

void Camera::update_perspective()
{
    m_projection = glm::perspective(m_fov, m_aspect_ratio, m_znear, m_zfar);
}

void Camera::detect_window_dimension_change(GLFWwindow* window)
{
    int w, h;
    glfwGetWindowSize(window, &w, &h);
    dimension_changed = (w != m_width || h != m_height);
    if (dimension_changed) {
        m_width = w;
        m_height = h;
        set_window_dimensions(w, h);
    }
}

} // namespace MoShape