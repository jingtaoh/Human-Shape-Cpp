//
// Created by Jingtao Huang on 10/24/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_READ_MAT_H
#define MOSHAPE_READ_MAT_H

#include <stdio.h>
#include <stdlib.h>
#include <filesystem>

#include <matio.h>
#include <spdlog/spdlog.h>
#include <Eigen/Dense>

using namespace Eigen;
namespace fs = std::filesystem;

namespace MoShape {

/**
 * @file
 * @brief Read variable data from a MAT file,the variable should have
 * the same name as the file
 *
 * @param [in] path path of the MAT file
 * @param [in] dims dimensions of the result data matrix
 * @return result data matrix
 */
MatrixXd read_mat(fs::path path, Vector2i dims)
{
    mat_t* matfp;
    matvar_t* matvar;

    matfp = Mat_Open(path.c_str(), MAT_ACC_RDONLY);
    assert(NULL != matfp && "Cannot open file");

    matvar = Mat_VarRead(matfp, path.stem().c_str());
    assert(NULL != matvar && "Unable to read matrix");

    assert(matvar->rank == 2 && "Matrix is not rank-2");
    assert(
        (dims[0] <= matvar->dims[0] && dims[1] <= matvar->dims[1]) && "Dimensions out of bounds");

    assert(
        (matvar->data_type == MAT_T_DOUBLE || matvar->data_type == MAT_T_SINGLE) &&
        "Datatype is not float or double");

    MatrixXd res(dims[0], dims[1]);
    char* data = (char*)matvar->data;
    size_t stride = Mat_SizeOf(matvar->data_type);
    for (int i = 0; i < dims[0]; i++) {
        for (int j = 0; j < dims[1]; j++) {
            size_t idx = matvar->dims[0] * j + i;
            switch (matvar->data_type) {
            case MAT_T_DOUBLE: res(i, j) = *(double*)(data + idx * stride); break;
            case MAT_T_SINGLE: res(i, j) = *(float*)(data + idx * stride); break;
            default: break;
            }
        }
    }

    Mat_VarFree(matvar);
    Mat_Close(matfp);

    return res;
}
} // namespace MoShape

#endif // MOSHAPE_READ_MAT_H
