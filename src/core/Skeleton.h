//
// Created by Jingtao Huang on 10/23/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_SKELETON_H
#define MOSHAPE_SKELETON_H

#include "Joint.h"

#include <vector>

namespace MoShape {

using Pose = std::vector<double>;

class Skeleton
{
public:
    Skeleton(std::vector<Joint> joint_list);
    ~Skeleton(){};

    inline Joint get_joint_by_id(int jid) const { return m_joint_list[jid]; };
    inline Joint get_mean_joint_by_id(int jid) const { return m_joint_list_mean_pose[jid]; };
    inline Joint get_root_joint() const { return get_joint_by_id(0); }
    inline int get_num_joints() const { return m_joint_list.size(); }
    inline int get_parent_by_id(int jid) const { return get_joint_by_id(jid).get_parent_id(); }

    inline void set_joint_pos_by_id(int jid, const Vector3d& pos)
    {
        m_joint_list[jid].set_position(pos);
    }
    inline void set_joint_pos_dir_by_id(int jid, const Vector3d& pos, const Vector3d& dir)
    {
        m_joint_list[jid].set_position_dir(pos, dir);
    }
    inline void update_joints_mean_pose() { m_joint_list_mean_pose = m_joint_list; }

protected:
    std::vector<Joint> m_joint_list;
    std::vector<Joint> m_joint_list_mean_pose;
};
} // namespace MoShape


#endif // MOSHAPE_SKELETON_H
