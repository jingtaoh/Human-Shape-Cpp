//
// Created by Jingtao Huang on 10/23/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#include "Mesh.h"
namespace MoShape {

Mesh::Mesh(std::vector<SkinnedVertex> vertices, std::vector<Vector3i> faces)
    : m_vertices(vertices)
    , m_vertices_mean_shape(vertices)
    , m_faces(faces)
{}

void Mesh::change_shape(Shape& shape_vector, MatrixXd& eigenvectors)
{
    // shape_vector: 1 x 20
    // eigenvectors: 20 x 19347 i.e., 20 x 6449 x 3
    // m_vertices: 64449 x 3

    for (int i = 0; i < get_num_vertices(); i++) {
        Vector3d vertex_pos = m_vertices_mean_shape[i].get_position();

        Vector3d offset = Vector3d::Zero();
        for (int j = 0; j < shape_vector.size(); j++) {
            Vector3d eigenvector = Vector3d(
                eigenvectors(j, i),
                eigenvectors(j, i + get_num_vertices()),
                eigenvectors(j, i + get_num_vertices() * 2));
            offset += shape_vector[j] * SKEL_SCALE_FACTOR * eigenvector;
        }

        Vector3d new_vertex_pos = vertex_pos + offset;
        m_vertices[i].set_position(new_vertex_pos);
    }
}


} // namespace MoShape