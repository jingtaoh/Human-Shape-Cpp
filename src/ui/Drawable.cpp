//
// Created by Jingtao Huang on 11/8/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#include "Drawable.h"
#include "gl_check_error.h"

namespace MoShape {
Drawable::Drawable(
    const MeshData& mesh_data,
    const Shader& vertex_shader,
    const Shader& fragment_shader,
    const Primitive& p,
    bool wireframe)
    : m_mesh_data(mesh_data)
    , m_vertex_shader(vertex_shader)
    , m_fragment_shader(fragment_shader)
    , m_shader_program({m_vertex_shader, m_fragment_shader})
    , m_primitive(p)
    , m_wireframe(wireframe)
{
    init();
}

void Drawable::draw() const
{
    const auto gl_primitive = [](const Primitive& p) {
        switch (p) {
        case Primitive::POINTS: return GL_POINTS;
        case Primitive::TRIANGLES: return GL_TRIANGLES;
        }
        return 0;
    };
    if (m_wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    gl_check_error(__FILE__, __LINE__);

    glBindVertexArray(m_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glDrawElements(gl_primitive(m_primitive), m_mesh_data.m_indices.size(), GL_UNSIGNED_INT, 0);

    if (m_wireframe) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

void Drawable::init()
{
    // vao
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);

    // vbo
    glGenBuffers(1, &m_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        m_mesh_data.m_vertices.size() * (sizeof(PackedVertex)),
        m_mesh_data.m_vertices.data(),
        GL_STATIC_DRAW);

    // ebo
    glGenBuffers(1, &m_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        m_mesh_data.m_indices.size() * sizeof(unsigned int),
        m_mesh_data.m_indices.data(),
        GL_STATIC_DRAW);

    // Pass data to GPU
    m_shader_program.set_attribute("in_pos", 3, sizeof(PackedVertex), 0);
    m_shader_program
        .set_attribute("in_normal", 3, sizeof(PackedVertex), offsetof(PackedVertex, normal));
    m_shader_program
        .set_attribute("in_color", 4, sizeof(PackedVertex), offsetof(PackedVertex, color));
    gl_check_error(__FILE__, __LINE__);

    glBindVertexArray(0);
}

ShaderProgram Drawable::get_shader_program() const
{
    return m_shader_program;
}
} // namespace MoShape