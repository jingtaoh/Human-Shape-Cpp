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
    MeshData init_mesh(const Mesh& mesh, const Color& color);
    MeshData init_skeleton(const Skeleton& skel, const Color& joint_color, const Color& bone_color);

    void render_model(bool render_mesh, bool render_skeleton);
    void render_mesh(bool wireframe) const;
    void render_skeleton() const;

    void draw_triangle(Vector3f& v0, Vector3f& v1, Vector3f& v2, Color color, MeshData& mesh_data);
    void draw_bone(
        Vector3f& head,
        Vector3f& tail,
        Vector3f& joint_dir,
        Color color,
        MeshData& mesh_data);
    void draw_joint(
        Vector3f center,
        float radius,
        Color color,
        int sector_count,
        int stack_count,
        MeshData& mesh_data);

    void update_light();
    void update_camera(GLFWwindow* window);

private:
    Model m_model;
    Camera m_camera;
    Drawable m_mesh;
    Drawable m_skeleton;
};
} // namespace MoShape


#endif // MOSHAPE_RENDERER_H
