//
// Created by Jingtao Huang on 10/23/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_MODEL_H
#define MOSHAPE_MODEL_H

#include "Mesh.h"
#include "Skeleton.h"

namespace MoShape {

using AABB = Eigen::AlignedBox3d;

class Model
{
public:
    Model(const Mesh& mesh, const Skeleton& skel);

    void center_model();

    void change_shape_and_pose(
        const Shape& shape_vector,
        const MatrixXd& eigenvectors,
        const Pose& pose_vector);

    inline int get_num_vertices() const { return m_mesh.get_num_vertices(); }
    inline int get_num_faces() const { return m_mesh.get_num_faces(); }
    inline int get_num_joints() const { return m_skeleton.get_num_joints(); }
    inline const Mesh& get_mesh() const { return m_mesh; }
    inline const Skeleton& get_skeleton() const { return m_skeleton; }

protected:
    void change_shape_to_mesh(const Shape& shape_vector, const MatrixXd& eigenvectors);
    void update_skeleton();
    inline void copy_joint_pos(int from, int to);
    void change_pose(const Pose& pose_vector);

    Mesh m_mesh;
    Shape m_shape_vector;
    Skeleton m_skeleton;
    Pose m_pose_vector;

    AABB m_bound;
    Vector3d m_center;

    MatrixXd m_skinning_weights;
};
} // namespace MoShape


#endif // MOSHAPE_MODEL_H
