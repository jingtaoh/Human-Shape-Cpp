//
// Created by Jingtao Huang on 10/31/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_RENDERER_H
#define MOSHAPE_RENDERER_H

#include "../core/Model.h"
#include "Shader.h"
#include "Camera.h"
#include "Color.h"

namespace MoShape {
class Renderer
{
public:
    Renderer(const Model& model, const Camera& camera);
    void init_model();
    void render_model(bool render_mesh, bool render_skeleton);
    void render_mesh() const;
    void render_skeleton() const;

    // TODO: render ground
    void render_ground();
    void update_light();
    void update_camera(GLFWwindow* window);
private:
    Model m_model;

    std::vector<Vector3f> m_vertices;
    std::vector<Vector3f> m_normals;
    std::vector<unsigned short> m_indices;

    // Shader
    Shader m_vertex_shader;
    Shader m_fragment_shader;
    ShaderProgram m_shader_program;

    Camera m_camera;

    // VAO/VBO/EBO
    GLuint m_vao, m_vbo, m_ebo;
};
} // namespace MoShape


#endif // MOSHAPE_RENDERER_H
