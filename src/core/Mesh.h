//
// Created by Jingtao Huang on 10/23/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_MESH_H
#define MOSHAPE_MESH_H

#include "SkinnedVertex.h"

#include <Eigen/Dense>
#include <vector>

using namespace Eigen;

class Mesh
{
public:
    Mesh(std::vector<SkinnedVertex> vertices, std::vector<Vector3i> faces);

    inline int get_num_vertices() const { return m_vertices.size(); }
    inline int get_num_faces() const { return m_faces.size(); }

protected:
    std::vector<SkinnedVertex> m_vertices;
    std::vector<Vector3i> m_faces;
};


#endif // MOSHAPE_MESH_H
