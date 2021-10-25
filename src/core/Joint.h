//
// Created by Jingtao Huang on 10/23/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_JOINT_H
#define MOSHAPE_JOINT_H

#include <Eigen/Dense>

using namespace Eigen;

class Joint
{
public:
    Joint();
    Joint(const Vector3d& pos, const Vector3d& dir, int parent);
    ~Joint() {}

    inline Vector3d get_position() const { return m_pos; }
    inline Vector3d get_direction() const { return m_dir; }
    inline Vector3d get_moment() const { return m_moment; }
    inline int get_parent_id() const { return m_parent; }

protected:
    Vector3d m_pos;
    Vector3d m_dir;
    Vector3d m_moment;
    int m_parent;
};


#endif // MOSHAPE_JOINT_H
