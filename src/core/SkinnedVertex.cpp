//
// Created by Jingtao Huang on 10/24/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#include "SkinnedVertex.h"
namespace MoShape {

SkinnedVertex::SkinnedVertex(
    const Vector3d& pos,
    const std::vector<int>& joint_indices,
    const std::vector<double>& joint_weights)
    : m_pos(pos)
    , m_joint_indices(joint_indices)
    , m_joint_weights(joint_weights)
{}

} // namespace MoShape