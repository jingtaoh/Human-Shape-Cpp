//
// Created by Jingtao Huang on 10/23/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_MODEL_H
#define MOSHAPE_MODEL_H

#include "Mesh.h"
#include "Skeleton.h"

struct Shape
{
protected:
    std::vector<double> m_scales;
};

struct Pose
{
protected:
    std::vector<double> m_angles;
};

class Model
{
public:
    Model(const Mesh& mesh, const Skeleton& skel);
protected:
    Mesh m_mesh;
    Shape m_shape_vector;
    Skeleton m_skeleton;
    Pose m_pose_vector;
};


#endif // MOSHAPE_MODEL_H
