//
// Created by Jingtao Huang on 10/31/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#include "ModelRenderer.h"
#include <Eigen/Dense>
#include "gl_check_error.h"

#define SHADER_DIR "../../src/ui/shaders"

using namespace Eigen;

namespace MoShape {
ModelRenderer::ModelRenderer(const Model& model)
    : m_model(model)
    , m_vertex_shader(SHADER_DIR "/shader.vert", GL_VERTEX_SHADER)
    , m_fragment_shader(SHADER_DIR "/shader.frag", GL_FRAGMENT_SHADER)
    , m_shader_program({m_vertex_shader, m_fragment_shader})
{
    gl_check_error(__FILE__, __LINE__);

    // Creation of the mesh
    std::vector<Vector3f> vertices;
    std::vector<Vector3i> indices;

//    for (int i = 0; i < model.get_num_vertices(); i++) {
//        Vector3f vertex_pos = model.get_mesh().get_mean_shape_vertex_pos_by_id(i).cast<float>();
//        vertices.emplace_back(vertex_pos);
//    }

    vertices.emplace_back(0.5, 0.5, 0);
    vertices.emplace_back(0.5, -0.5, 0);
    vertices.emplace_back(-0.5, -0.5, 0);
    vertices.emplace_back(-0.5, 0.5, 0);

//    for (int i = 0; i < model.get_num_faces(); i++) {
//        Vector3i face_idx = model.get_mesh().get_face_indices_by_id(i);
//        indices.emplace_back(face_idx);
//    }

    indices.emplace_back(0, 1, 3);
    indices.emplace_back(1, 2, 3);

    // vao
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // vbo
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        vertices.size() * sizeof(Vector3f),
        vertices.data(),
        GL_STATIC_DRAW);

    // ebo
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        indices.size() * sizeof(Vector3i),
        indices.data(),
        GL_STATIC_DRAW);

    // Pass data to GPU
    m_shader_program.set_attribute("aPos", 3, sizeof(Vector3f), 0);
}

void ModelRenderer::render_model(bool render_mesh, bool render_skeleton) const
{
    //    spdlog::info("render model()");

    // clear
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_shader_program.use();

    gl_check_error(__FILE__, __LINE__);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    gl_check_error(__FILE__, __LINE__);

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    //    glDrawElements(GL_TRIANGLES, m_model.get_num_faces() * 3, GL_UNSIGNED_INT, 0);

    if (render_mesh) ModelRenderer::render_mesh();
    if (render_skeleton) ModelRenderer::render_skeleton();
}

void ModelRenderer::render_mesh() const
{
    //    spdlog::info("render mesh()");
}

void ModelRenderer::render_skeleton() const
{
    //    spdlog::info("render skeleton()");
}
} // namespace MoShape