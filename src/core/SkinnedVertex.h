//
// Created by Jingtao Huang on 10/24/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_SKINNEDVERTEX_H
#define MOSHAPE_SKINNEDVERTEX_H

#include <Eigen/Dense>
#include <vector>

using namespace Eigen;

namespace MoShape {

class SkinnedVertex
{
public:
    SkinnedVertex(
        const Vector3d& pos,
        const std::vector<int>& joint_indices,
        const std::vector<double>& joint_weights);

    inline Vector3d get_position() const { return m_pos; };
    inline std::vector<int> get_joint_indices() const { return m_joint_indices; }
    inline std::vector<double> get_joint_weights() const { return m_joint_weights; };
    inline int get_num_bound_joints_per_vertex() const { return m_joint_indices.size(); }
    inline void set_position(Vector3d pos) { m_pos = pos; };

protected:
    Vector3d m_pos;
    std::vector<int> m_joint_indices;
    std::vector<double> m_joint_weights;
};
} // namespace MoShape


#endif // MOSHAPE_SKINNEDVERTEX_H
