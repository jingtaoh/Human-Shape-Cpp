//
// Created by Jingtao Huang on 11/8/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_DRAWABLE_H
#define MOSHAPE_DRAWABLE_H

#include <Eigen/Dense>
#include <vector>
#include "Color.h"
#include "Shader.h"

using namespace Eigen;

namespace MoShape {

struct PackedVertex
{
    PackedVertex(Vector3f p, Vector3f n, Color c)
        : position(p)
        , normal(n)
        , color(c)
    {}
    bool operator<(const PackedVertex that) const
    {
        return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) > 0;
    };
    Vector3f position;
    Vector3f normal = Vector3f(0, 0, 1);
    Color color = Color(1, 1, 1, 1);
};

struct MeshData
{
    std::vector<PackedVertex> m_vertices;
    std::vector<unsigned int> m_indices;
};

enum class Primitive { POINTS, TRIANGLES };

class Drawable
{
public:
    // Mesh data
    MeshData m_mesh_data;

    // Shader
    Shader m_vertex_shader;
    Shader m_fragment_shader;
    ShaderProgram m_shader_program;

    Drawable(
        const MeshData& mesh_data,
        const Shader& vertex_shader,
        const Shader& fragment_shader,
        const Primitive& p,
        bool wireframe = false);
    void draw() const;
    ShaderProgram get_shader_program() const;

private:
    // VAO/VBO/EBO
    GLuint m_vao, m_vbo, m_ebo;
    Primitive m_primitive;
    bool m_wireframe;
    void init();
};
} // namespace MoShape


#endif // MOSHAPE_DRAWABLE_H
