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

    for (int i = 0; i < model.get_num_vertices(); i++) {
        Vector3f vertex_pos = model.get_mesh().get_vertex_pos_by_id(i).cast<float>();
        old_vertices.emplace_back(vertex_pos);
    }


    for (int i = 0; i < model.get_num_faces(); i++) {
        Vector3i face_idx = model.get_mesh().get_face_indices_by_id(i);
        old_indices.emplace_back(face_idx);
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

    // vao
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // vbo
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        m_vertices.size() * (sizeof(Vector3f) + sizeof(Vector3f)),
        nullptr,
        GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, m_vertices.size() * sizeof(Vector3f), m_vertices.data());
    glBufferSubData(
        GL_ARRAY_BUFFER,
        m_vertices.size() * sizeof(Vector3f),
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
    m_shader_program.set_attribute(
        "in_normal",
        3,
        sizeof(Vector3f),
        m_vertices.size() * sizeof(Vector3f));

    m_shader_program.use();
    gl_check_error(__FILE__, __LINE__);

    m_shader_program.set_uniform("material.ambient", glm::vec3(1));
    m_shader_program.set_uniform("material.diffuse", glm::vec3(1));
    m_shader_program.set_uniform("material.specular", glm::vec3(1));
    m_shader_program.set_uniform("material.shininess", 32.0f);

    gl_check_error(__FILE__, __LINE__);

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

void Renderer::render_ground() {}

void Renderer::update_light()
{
    m_shader_program.set_uniform("light.pos", glm::vec3(0, 0, 100));
    m_shader_program.set_uniform("light.ambient", glm::vec3(0.1));
    m_shader_program.set_uniform("light.diffuse", glm::vec3(0.7));
    m_shader_program.set_uniform("light.specular", glm::vec3(0.1));
    gl_check_error(__FILE__, __LINE__);
}

void Renderer::update_camera(GLFWwindow* window)
{
    m_camera.detect_window_dimension_change(window);

    auto model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));

    auto view = m_camera.get_view();
    auto projection = m_camera.get_projection();

    auto normal_matrix = glm::transpose(glm::inverse(view * model));

    gl_check_error(__FILE__, __LINE__);
    m_shader_program.set_uniform("model", model);
    m_shader_program.set_uniform("view", view);
    m_shader_program.set_uniform("normal_matrix", normal_matrix);
    m_shader_program.set_uniform("projection", projection);
}
} // namespace MoShape