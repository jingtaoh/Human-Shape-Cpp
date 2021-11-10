//
// Created by Jingtao Huang on 11/6/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_UNIFY_INDEX_BUFFER_H
#define MOSHAPE_UNIFY_INDEX_BUFFER_H

#include <Eigen/Dense>
#include <map>
#include <vector>
#include "../Drawable.h"

using namespace Eigen;

namespace MoShape {

bool get_similar_vertex_index(
    PackedVertex& packed,
    std::map<PackedVertex, unsigned int>& vertex_to_index,
    unsigned int& result);

void unify_index_buffer(
    std::vector<Vector3f>& in_vertices,
    std::vector<Vector3f>& in_normals,
    std::vector<Color>& in_colors,
    std::vector<unsigned int>& out_indices,
    std::vector<PackedVertex>& out_vertices);
} // namespace MoShape

#endif // MOSHAPE_UNIFY_INDEX_BUFFER_H
