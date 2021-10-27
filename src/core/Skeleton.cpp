//
// Created by Jingtao Huang on 10/23/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#include "Skeleton.h"

namespace MoShape {
Skeleton::Skeleton(std::vector<Joint> joint_list)
    : m_joint_list(joint_list)
    , m_joint_list_mean_pose(joint_list)
{}

} // namespace MoShape
