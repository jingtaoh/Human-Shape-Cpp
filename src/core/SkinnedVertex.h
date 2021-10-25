//
// Created by Jingtao Huang on 10/24/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_SKINNEDVERTEX_H
#define MOSHAPE_SKINNEDVERTEX_H

#include <Eigen/Dense>
#include <vector>

using namespace Eigen;

class SkinnedVertex
{
public:
    SkinnedVertex(const Vector3d& pos, const std::vector<int>& joint_indices, const std::vector<double>& joint_weights);
protected:
    Vector3d m_pos;
    std::vector<int> m_joint_indices;
    std::vector<double> m_joint_weights;
};


#endif // MOSHAPE_SKINNEDVERTEX_H
