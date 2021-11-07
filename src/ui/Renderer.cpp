//
// Created by Jingtao Huang on 10/31/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <Eigen/Dense>
#include <glm/gtc/matrix_transform.hpp>
#include "gl_check_error.h"
#include "utils/compute_face_normal.h"
#include "utils/unify_index_buffer.h"

#define SHADER_DIR "../../src/ui/shaders"

using namespace Eigen;

namespace MoShape {
typedef std::vector<Vector4f> vector;
Renderer::Renderer(const Model& model, const Camera& camera)
    : m_model(model)
    , m_vertex_shader(SHADER_DIR "/shader.vert", GL_VERTEX_SHADER)
    , m_fragment_shader(SHADER_DIR "/shader.frag", GL_FRAGMENT_SHADER)
    , m_shader_program({m_vertex_shader, m_fragment_shader})
    , m_camera(camera)
{
    gl_check_error(__FILE__, __LINE__);

    // Creation of the mesh
    std::vector<Vector3f> old_vertices;
    std::vector<Vector3i> old_indices;

    {
        //    for (int i = 0; i < model.get_num_vertices(); i++) {
        //        Vector3f vertex_pos =
        //        model.get_mesh().get_mean_shape_vertex_pos_by_id(i).cast<float>();
        //        old_vertices.emplace_back(vertex_pos);
        //    }
    }

    {
        old_vertices.emplace_back(-0.5, -0.5, -0.5);
        old_vertices.emplace_back(0.5, -0.5, -0.5);
        old_vertices.emplace_back(0.5, 0.5, -0.5);
        old_vertices.emplace_back(-0.5, 0.5, -0.5);
        old_vertices.emplace_back(-0.5, -0.5, 0.5);
        old_vertices.emplace_back(0.5, -0.5, 0.5);
        old_vertices.emplace_back(0.5, 0.5, 0.5);
        old_vertices.emplace_back(-0.5, 0.5, 0.5);
    }


    {
        //    for (int i = 0; i < model.get_num_faces(); i++) {
        //        Vector3i face_idx = model.get_mesh().get_face_indices_by_id(i);
        //        old_indices.emplace_back(face_idx);
        //    }
    }

    {
        old_indices.emplace_back(0, 2, 1);
        old_indices.emplace_back(0, 3, 2);
        old_indices.emplace_back(4, 5, 6);
        old_indices.emplace_back(4, 6, 7);
        old_indices.emplace_back(1, 2, 6);
        old_indices.emplace_back(1, 6, 5);
        old_indices.emplace_back(3, 0, 7);
        old_indices.emplace_back(7, 0, 4);
        old_indices.emplace_back(2, 3, 7);
        old_indices.emplace_back(2, 7, 6);
        old_indices.emplace_back(0, 1, 4);
        old_indices.emplace_back(4, 1, 5);
    }

    std::vector<Vector3f> new_vertices;
    std::vector<Vector3f> new_normals;
    std::vector<unsigned short> new_indices;

    for (int i = 0; i < old_indices.size(); i++) {
        for (int j = 0; j < 3; j++) {
            new_vertices.push_back(old_vertices[old_indices[i](j)]);
        }
        Vector3f face_normal;
        compute_face_normal(
            new_vertices[i * 3],
            new_vertices[i * 3 + 1],
            new_vertices[i * 3 + 2],
            face_normal);
        for (int j = 0; j < 3; j++) {
            new_normals.push_back(face_normal);
        }
        for (int j = 0; j < 3; j++) {
            new_indices.push_back((unsigned short)old_indices[i](j));
        }
    }

    unify_index_buffer(new_vertices, new_normals, m_indices, m_vertices, m_normals);

    for (int i = 0; i < m_vertices.size(); i++) m_colors.emplace_back(Color::green());

    // vao
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // vbo
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        m_vertices.size() * (sizeof(Vector3f) + sizeof(Color) + sizeof(Vector3f)),
        nullptr,
        GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(Vector3f), m_vertices.data());
    glBufferSubData(
        GL_ARRAY_BUFFER,
        m_vertices.size() * sizeof(Vector3f),
        m_vertices.size() * sizeof(Color),
        m_colors.data());
    glBufferSubData(
        GL_ARRAY_BUFFER,
        m_vertices.size() * (sizeof(Vector3f) + sizeof(Color)),
        m_vertices.size() * sizeof(Vector3f),
        m_normals.data());

    // ebo
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        m_indices.size() * sizeof(unsigned short),
        m_indices.data(),
        GL_STATIC_DRAW);

    // Pass data to GPU
    m_shader_program.set_attribute("in_pos", 3, sizeof(Vector3f), 0);
    m_shader_program
        .set_attribute("in_color", 4, sizeof(Color), m_vertices.size() * sizeof(Vector3f));
    m_shader_program.set_attribute(
        "in_normal",
        3,
        sizeof(Vector3f),
        m_vertices.size() * (sizeof(Vector3f) + sizeof(Color)));

    glEnable(GL_DEPTH_TEST);
}

void Renderer::render_model(bool render_mesh, bool render_skeleton)
{
    //    spdlog::info("render model()");

    gl_check_error(__FILE__, __LINE__);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    gl_check_error(__FILE__, __LINE__);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_SHORT, 0);

    if (render_mesh) Renderer::render_mesh();
    if (render_skeleton) Renderer::render_skeleton();
}

void Renderer::render_mesh() const
{
    //    spdlog::info("render mesh()");
}

void Renderer::render_skeleton() const
{
    //    spdlog::info("render skeleton()");
}

void Renderer::update_camera(GLFWwindow* window)
{
    m_camera.detect_window_dimension_change(window);

    auto model = glm::mat4(1.0f);
    model =
        glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

    auto view = m_camera.get_view();
    auto projection = m_camera.get_projection();

    auto normal_matrix = glm::transpose(glm::inverse(view * model));

    m_shader_program.use();

    m_shader_program.set_uniform("model", model);
    m_shader_program.set_uniform("view", view);
    m_shader_program.set_uniform("normal_matrix", normal_matrix);
    m_shader_program.set_uniform("projection", projection);
}
} // namespace MoShape