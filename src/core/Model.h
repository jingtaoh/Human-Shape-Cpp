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
    Model(Mesh& mesh, Skeleton& skel);

    void center_model();
    void update_skeleton();
    void change_shape_and_pose(Shape& shape_vector, Pose& pose_vector);
    void update_model();

    inline int get_num_vertices() const { return m_mesh.get_num_vertices(); }
    inline int get_num_faces() const { return m_mesh.get_num_faces(); }
    inline int get_num_joints() const { return m_skeleton.get_num_joints(); }

    inline Mesh& get_mesh() { return m_mesh; }
    inline Skeleton& get_skeleton() { return m_skeleton; }

    inline double get_shape_vector_by_id(int sid) const { return m_shape_vector[sid]; }
    inline double get_pose_vector_by_id(int pid) const { return m_pose_vector[pid]; }

    inline void set_shape_vector_by_id(int sid, double value) { m_shape_vector[sid] = value; }
    inline void set_pose_vector_by_id(int pid, double value) { m_pose_vector[pid] = value; }

    inline void set_eigenvalues_and_eigenvectors(MatrixXd& eigenvalues, MatrixXd& eigenvectors)
    {
        m_eigenvalues = eigenvalues;
        m_eigenvectors = eigenvectors;
    }

    inline bool is_dirty() const { return m_is_dirty; }
    inline void set_dirty(bool value) { m_is_dirty = value; }

protected:
    void change_shape_to_mesh(Shape& shape_vector);
    inline void copy_joint_pos(int from, int to);
    void change_pose(Pose& pose_vector);

    MatrixXd m_skinning_weights;

    Mesh m_mesh;
    Vector3d m_center;
    Shape m_shape_vector;
    MatrixXd m_eigenvalues;
    MatrixXd m_eigenvectors;

    Skeleton m_skeleton;
    Pose m_pose_vector;

    bool m_is_dirty;
};
} // namespace MoShape


#endif // MOSHAPE_MODEL_H
