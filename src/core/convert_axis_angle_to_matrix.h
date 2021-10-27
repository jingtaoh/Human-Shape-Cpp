//
// Created by Jingtao Huang on 10/25/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_CONVERT_AXIS_ANGLE_TO_MATRIX_H
#define MOSHAPE_CONVERT_AXIS_ANGLE_TO_MATRIX_H

#include <Eigen/Dense>

using namespace Eigen;

/**
 *
 * @param [in] axis
 * @param [in] pos
 * @param angle in radians
 * @return
 */
Matrix4d convert_axis_angle_to_matrix(const Vector3d& axis, const Vector3d& pos, double angle)
{
    auto I = Matrix3d::Identity();

    Matrix3d axis_Hat;
    axis_Hat << 0, -axis(2), axis(1),
        axis(2), 0, -axis(0),
        -axis(1), axis(0), 0;

    // rotation matrix
    Matrix3d R = I + sin(angle) * axis_Hat + (1 - cos(angle)) * axis_Hat * axis_Hat;

    // translation vector
    Vector3d t = (I - R) * pos;

    Matrix4d M;
    M << R(0, 0), R(0, 1), R(0, 2), t(0),
         R(1, 0), R(1, 1), R(1, 2), t(1),
         R(2, 0), R(2, 1), R(2, 2), t(2),
         0, 0, 0, 1;
    return M;
}

#endif // MOSHAPE_CONVERT_AXIS_ANGLE_TO_MATRIX_H
