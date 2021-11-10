//
// Created by Jingtao Huang on 10/23/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#include "Joint.h"

namespace MoShape {
Joint::Joint()
{
    Vector3d zero(0, 0, 0);
    m_pos = zero;
    m_dir = zero;
    m_moment = zero;
    m_parent = -1;
}

Joint::Joint(const Vector3d& pos, const Vector3d& dir, int parent, int fake_parent)
    : m_pos(pos)
    , m_dir(dir)
    , m_parent(parent)
    , m_fake_parent(fake_parent)
{
    m_moment = pos.cross(dir);
}
} // namespace MoShape