//
// Created by Jingtao Huang on 11/6/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#include "unify_index_buffer.h"

namespace MoShape {
bool get_similar_vertex_index(
    PackedVertex& packed,
    std::map<PackedVertex, unsigned int>& vertex_to_index,
    unsigned int& result)
{
    auto it = vertex_to_index.find(packed);
    if (it == vertex_to_index.end()) {
        return false;
    } else {
        result = it->second;
        return true;
    }
}

void unify_index_buffer(
    std::vector<Vector3f>& in_vertices,
    std::vector<Vector3f>& in_normals,
    std::vector<Color>& in_colors,
    std::vector<unsigned int>& out_indices,
    std::vector<PackedVertex>& out_vertices)
{
    std::map<PackedVertex, unsigned int> vertex_to_out_index;

    for (int i = 0; i < in_vertices.size(); i++) {
        PackedVertex packed = {in_vertices[i], in_normals[i], in_colors[i]};

        // Try to find a similar vertex
        unsigned int index;
        bool found = get_similar_vertex_index(packed, vertex_to_out_index, index);

        if (found) {
            out_indices.push_back(index);
        } else {
            out_vertices.push_back(packed);
            unsigned int new_index = (unsigned int)out_vertices.size() - 1;
            out_indices.push_back(new_index);
            vertex_to_out_index[packed] = new_index;
        }
    }
}
} // namespace MoShape