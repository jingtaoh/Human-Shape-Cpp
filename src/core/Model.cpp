//
// Created by Jingtao Huang on 10/23/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#include "Model.h"
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#include <algorithm>
#include "convert_axis_angle_and_translation_to_matrix.h"
#include "convert_axis_angle_to_matrix.h"

namespace MoShape {
Model::Model(Mesh& mesh, Skeleton& skel)
    : m_mesh(mesh)
    , m_skeleton(skel)
    , m_is_dirty(false)
{
    // Construct skinning weight matrix
    int num_vertices = get_num_vertices();
    int num_joints = get_num_joints();
    int num_bound_joints_per_vertex = mesh.get_num_bound_joints_per_vertex();

    m_skinning_weights.resize(num_vertices, num_joints);
    m_skinning_weights.setZero();

    for (int i = 0; i < num_vertices; i++) {
        auto joint_indices = m_mesh.get_bound_joint_indices_by_id(i);
        auto joint_weights = m_mesh.get_bound_joint_weights_by_id(i);
        for (int j = 0; j < num_bound_joints_per_vertex; j++) {
            m_skinning_weights(i, joint_indices[j]) += joint_weights[j];
        }
    }

    // Compute center so that we can move it to origin
    m_center = Vector3d::Zero();
    for (int i = 0; i < num_vertices; i++) {
        m_center += m_mesh.get_mean_shape_vertex_pos_by_id(i);
    }
    m_center /= num_vertices;

    m_shape_vector = Shape(20, 0.0f);
    m_pose_vector = Pose(31, 0.0f);
}

void Model::center_model()
{
    for (int i = 0; i < get_num_vertices(); i++) {
        m_mesh.set_vertex_pos_by_id(i, m_mesh.get_vertex_pos_by_id(i) - m_center);
        m_mesh.set_mean_shape_vertex_pos_by_id(
            i,
            m_mesh.get_mean_shape_vertex_pos_by_id(i) - m_center);
    }
    m_center.setZero();
}

void Model::update_model()
{
    change_shape_and_pose(m_shape_vector, m_pose_vector);
}

void Model::change_shape_and_pose(Shape& shape_vector, Pose& pose_vector)
{
    change_shape_to_mesh(shape_vector);
    update_skeleton();
    change_pose(pose_vector);
    m_is_dirty = false;
}

void Model::change_shape_to_mesh(Shape& shape_vector)
{
    m_mesh.change_shape(shape_vector, m_eigenvectors);
    m_shape_vector = shape_vector;
}

void Model::update_skeleton()
{
    int num_vertices = get_num_vertices();
    int num_joints = get_num_joints();

    MatrixXd weights(num_vertices, num_joints);
    weights.setZero();

    // new joint position is the average weighted sum of all the vertices
    for (int j = 0; j < num_joints; j++) {
        Vector3d weighted_sum;
        weighted_sum.setZero();
        double sum_of_weights = 0;
        for (int i = 0; i < num_vertices; i++) {
            weights(i, j) = (j == 0)
                                ? m_skinning_weights(i, j)
                                : (std::min(
                                      m_skinning_weights(i, j),
                                      m_skinning_weights(i, m_skeleton.get_fake_parent_by_id(j))));
            weighted_sum += weights(i, j) * m_mesh.get_vertex_pos_by_id(i);
            sum_of_weights += weights(i, j);
        }
        Vector3d new_joint_pos = weighted_sum / sum_of_weights;
        m_skeleton.set_joint_pos_by_id(j, new_joint_pos);
    }

    // copy overlapping joints over
    copy_joint_pos(2, 1); // 2 -> 1
    copy_joint_pos(6, 5); // 6 -> 5
    copy_joint_pos(10, 9); // 10 -> 9
    copy_joint_pos(14, 12); // 14 -> 12
    copy_joint_pos(14, 13); // 14 -> 13
    copy_joint_pos(19, 17); // 19 -> 17
    copy_joint_pos(19, 18); // 19 -> 18
    copy_joint_pos(6, 22); // 6 -> 22
    copy_joint_pos(2, 23); // 2 -> 23
    copy_joint_pos(10, 24); // 10 -> 24

    m_skeleton.update_joints_mean_pose();
}

void Model::copy_joint_pos(int from, int to)
{
    m_skeleton.set_joint_pos_by_id(to, m_skeleton.get_joint_by_id(from).get_position());
}

void Model::change_pose(Pose& pose_vector)
{
    // pose: 1 x 31
    // [0..2] specify the global rotation
    // [3..5] specify the global translation
    // [6..31] specify the local transform of each joint (jid = id - 6)

    // Compute global transform
    Vector3d global_rot_vector(pose_vector[0], pose_vector[1], pose_vector[2]);
    double angle = sqrt(
        pow(global_rot_vector.x(), 2) + pow(global_rot_vector.y(), 2) +
        pow(global_rot_vector.z(), 2));
    auto axis = (angle == 0) ? Vector3d(0, 0, 0) : global_rot_vector / angle;

    Vector3d global_trans_vector(pose_vector[3], pose_vector[4], pose_vector[5]);

    auto global_transform =
        convert_axis_angle_and_translation_to_matrix(axis, angle, global_trans_vector);

    // Compute local transform of each joint
    std::vector<Matrix4d> local_transforms(get_num_joints());
    for (int j = 0; j < get_num_joints(); j++) {
        auto joint = m_skeleton.get_joint_by_id(j);
        local_transforms[j] = convert_axis_angle_to_matrix(
            joint.get_direction(),
            joint.get_position(),
            pose_vector[j + 6]);
    }

    // Compute global transform of each joint
    std::vector<Matrix4d> global_transforms(get_num_joints());
    // This update order makes sure parent joints' transform are computed before children
    std::vector<int> update_order{0,  23, 1,  2,  3,  4,  22, 5,  6,  7,  8,  9, 24,
                                  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21};
    global_transforms[0] = global_transform * local_transforms[0];
    for (int j = 1; j < update_order.size(); j++) {
        int jid = update_order[j];
        global_transforms[jid] =
            global_transforms[m_skeleton.get_parent_by_id(jid)] * local_transforms[jid];
    }

    // Apply transforms to vertices
    for (int i = 0; i < get_num_vertices(); i++) {
        Vector3d vertex_pos = m_mesh.get_vertex_pos_by_id(i);

        auto joint_indices = m_mesh.get_bound_joint_indices_by_id(i);
        auto joint_weights = m_mesh.get_bound_joint_weights_by_id(i);

        Matrix4d accumulated_transform;
        accumulated_transform.setZero();

        for (int j = 0; j < joint_indices.size(); j++) {
            int jid = joint_indices[j];
            double weight = joint_weights[j];
            accumulated_transform += weight * global_transforms[jid];
        }
        Vector4d temp(vertex_pos(0), vertex_pos(1), vertex_pos(2), 1);
        temp = accumulated_transform * temp;
        vertex_pos = temp.head(3);

        m_mesh.set_vertex_pos_by_id(i, vertex_pos);
    }

    // Apply transforms to joints
    for (int j = 0; j < get_num_joints(); j++) {
        auto mean_joint = m_skeleton.get_mean_joint_by_id(j);
        auto mean_joint_pos = mean_joint.get_position();
        auto mean_joint_dir = mean_joint.get_direction();

        Matrix3d R = global_transforms[j].block<3, 3>(0, 0);
        Vector3d t(
            global_transforms[j](0, 3),
            global_transforms[j](1, 3),
            global_transforms[j](2, 3));

        auto new_joint_pos = R * mean_joint_pos + t;
        auto new_joint_dir = R * mean_joint_dir;

        m_skeleton.set_joint_pos_dir_by_id(j, new_joint_pos, new_joint_dir);
    }

    m_pose_vector = pose_vector;
}
} // namespace MoShape