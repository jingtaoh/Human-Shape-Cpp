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
    , m_camera(camera)
    , m_mesh(
          init_mesh(model.get_mesh()),
          Shader(SHADER_DIR "/mesh.vert", GL_VERTEX_SHADER),
          Shader(SHADER_DIR "/mesh.frag", GL_FRAGMENT_SHADER))
{
    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

MeshData Renderer::init_mesh(const Mesh& mesh)
{
    gl_check_error(__FILE__, __LINE__);

    // Creation of the mesh
    std::vector<Vector3f> old_vertices;
    std::vector<Vector3i> old_indices;

    for (int i = 0; i < mesh.get_num_vertices(); i++) {
        Vector3f vertex_pos = mesh.get_vertex_pos_by_id(i).cast<float>();
        old_vertices.emplace_back(vertex_pos);
    }


    for (int i = 0; i < mesh.get_num_faces(); i++) {
        Vector3i face_idx = mesh.get_face_indices_by_id(i);
        old_indices.emplace_back(face_idx);
    }

    std::vector<Vector3f> new_vertices;
    std::vector<Vector3f> new_normals;
    std::vector<Color> new_colors;
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
            new_colors.push_back(Color::white());
            new_indices.push_back((unsigned short)old_indices[i](j));
        }
    }

    std::vector<PackedVertex> vertices;
    std::vector<unsigned short> indices;

    unify_index_buffer(new_vertices, new_normals, new_colors, indices, vertices);

    MeshData res = {vertices, indices};
    return res;
}

void Renderer::render_model(bool render_mesh, bool render_skeleton)
{
    //    spdlog::info("render model()");

    if (render_mesh) Renderer::render_mesh();
    if (render_skeleton) Renderer::render_skeleton();
}

void Renderer::render_mesh() const
{
    //    spdlog::info("render mesh()");
    m_mesh.draw();
}

void Renderer::render_skeleton() const
{
    //    spdlog::info("render skeleton()");
    // TODO: implement
    // Render joint position


    // Render connection between joints
}

void Renderer::update_light()
{
    auto pipeline = m_mesh.get_shader_program();

    pipeline.use();
    gl_check_error(__FILE__, __LINE__);

    pipeline.set_uniform("light.dir", glm::vec3(0, 0, -1));
    pipeline.set_uniform("light.ambient", glm::vec3(0.1));
    pipeline.set_uniform("light.diffuse", glm::vec3(0.7));
    pipeline.set_uniform("light.specular", glm::vec3(0.1));
    gl_check_error(__FILE__, __LINE__);
}

void Renderer::update_camera(GLFWwindow* window)
{
    m_camera.detect_window_dimension_change(window);

    auto model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));

    auto view = m_camera.get_view();
    auto projection = m_camera.get_projection();

    auto normal_matrix = glm::transpose(glm::inverse(view * model));

    auto pipeline = m_mesh.get_shader_program();

    pipeline.use();
    gl_check_error(__FILE__, __LINE__);

    pipeline.set_uniform("model", model);
    pipeline.set_uniform("view", view);
    pipeline.set_uniform("normal_matrix", normal_matrix);
    pipeline.set_uniform("projection", projection);
}
} // namespace MoShape