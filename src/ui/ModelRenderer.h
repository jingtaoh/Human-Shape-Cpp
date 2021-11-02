//
// Created by Jingtao Huang on 10/31/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_MODELRENDERER_H
#define MOSHAPE_MODELRENDERER_H

#include "../core/Model.h"
#include "Shader.h"

namespace MoShape {
class ModelRenderer
{
public:
    ModelRenderer(const Model& model);
    void render_model(bool render_mesh, bool render_skeleton) const;
    void render_mesh() const;
    void render_skeleton() const;
private:
    Model m_model;

    // Shader
    Shader m_vertex_shader;
    Shader m_fragment_shader;
    ShaderProgram m_shader_program;

    // VAO/VBO/EBO
    GLuint m_vao, m_vbo, m_ebo;
};
} // namespace MoShape


#endif // MOSHAPE_MODELRENDERER_H
