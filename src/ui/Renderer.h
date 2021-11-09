//
// Created by Jingtao Huang on 10/31/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_RENDERER_H
#define MOSHAPE_RENDERER_H

#include "../core/Model.h"
#include "Camera.h"
#include "Drawable.h"

namespace MoShape {
class Renderer
{
public:
    Renderer(const Model& model, const Camera& camera);
    MeshData init_mesh(const Mesh& mesh);
    MeshData init_skeleton(const Skeleton& skel);
    void render_model(bool render_mesh, bool render_skeleton);
    void render_mesh() const;
    void render_skeleton() const;

    void update_light();
    void update_camera(GLFWwindow* window);

private:
    Model m_model;
    Camera m_camera;
    Drawable m_mesh;
};
} // namespace MoShape


#endif // MOSHAPE_RENDERER_H
