//
// Created by Jingtao Huang on 11/6/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_UNIFY_INDEX_BUFFER_H
#define MOSHAPE_UNIFY_INDEX_BUFFER_H

#include <Eigen/Dense>
#include <map>
#include <vector>

using namespace Eigen;

namespace MoShape {

struct PackedVertex
{
    Vector3f position;
    Vector3f normal;
    bool operator<(const PackedVertex that) const
    {
        return memcmp((void*)this, (void*)&that, sizeof(PackedVertex)) > 0;
    };
};

bool get_similar_vertex_index(
    PackedVertex& packed,
    std::map<PackedVertex, unsigned short>& vertex_to_index,
    unsigned short& result);

void unify_index_buffer(
    std::vector<Vector3f>& in_vertices,
    std::vector<Vector3f>& in_normals,
    std::vector<unsigned short>& out_indices,
    std::vector<Vector3f>& out_vertices,
    std::vector<Vector3f>& out_normals);
} // namespace MoShape

#endif // MOSHAPE_UNIFY_INDEX_BUFFER_H
