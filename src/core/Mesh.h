//
// Created by Jingtao Huang on 10/23/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_MESH_H
#define MOSHAPE_MESH_H

#include "SkinnedVertex.h"

#include <Eigen/Dense>
#include <vector>

using namespace Eigen;

namespace MoShape {

using Shape = std::vector<double>;

class Mesh
{
public:
    Mesh(std::vector<SkinnedVertex> vertices, std::vector<Vector3i> faces);

    void change_shape(const Shape& shape_vector, const MatrixXd& eigenvectors);

    inline int get_num_vertices() const { return m_vertices.size(); };
    inline int get_num_faces() const { return m_faces.size(); }
    inline int get_num_bound_joints_per_vertex() const
    {
        return m_vertices[0].get_num_bound_joints_per_vertex();
    }
    inline std::vector<int> get_bound_joint_indices_by_id(int vid) const
    {
        return m_vertices[vid].get_joint_indices();
    }
    inline std::vector<double> get_bound_joint_weights_by_id(int vid) const
    {
        return m_vertices[vid].get_joint_weights();
    }
    inline auto get_mean_shape_vertex_pos_by_id(int vid) const
    {
        return m_vertices_mean_shape[vid].get_position();
    }
    inline void set_vertex_pos_by_id(int vid, const Vector3d& pos)
    {
        m_vertices[vid].set_position(pos);
    }

protected:
    std::vector<SkinnedVertex> m_vertices;
    std::vector<SkinnedVertex> m_vertices_mean_shape;
    std::vector<Vector3i> m_faces;
};
} // namespace MoShape

#endif // MOSHAPE_MESH_H