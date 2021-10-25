//
// Created by Jingtao Huang on 10/23/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_SKELETON_H
#define MOSHAPE_SKELETON_H

#include "Joint.h"

#include <vector>

class Skeleton
{
public:
    Skeleton(std::vector<Joint> joint_list);
    ~Skeleton(){};


    inline Joint get_joint_by_id(int jid) const { return m_joint_list[jid]; }
    inline Joint get_root_joint() const { return get_joint_by_id(0); }
    inline int get_num_joints() const { return m_joint_list.size(); }

protected:
    std::vector<Joint> m_joint_list;
};


#endif // MOSHAPE_SKELETON_H
