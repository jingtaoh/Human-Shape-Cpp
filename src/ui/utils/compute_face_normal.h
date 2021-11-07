//
// Created by Jingtao Huang on 11/6/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_COMPUTE_FACE_NORMAL_H
#define MOSHAPE_COMPUTE_FACE_NORMAL_H

#include <Eigen/Dense>

using namespace Eigen;

namespace MoShape {
void compute_face_normal(
    const Vector3f& v0,
    const Vector3f& v1,
    const Vector3f& v2,
    Vector3f& normal)
{
    Vector3f a = v1 - v0;
    Vector3f b = v2 - v0;
    normal = a.cross(b);
}

} // namespace MoShape

#endif // MOSHAPE_COMPUTE_FACE_NORMAL_H
