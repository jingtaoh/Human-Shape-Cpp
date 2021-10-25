//
// Created by Jingtao Huang on 10/23/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#include "Mesh.h"

Mesh::Mesh(std::vector<SkinnedVertex> vertices, std::vector<Vector3i> faces) {
    m_vertices = vertices;
    m_faces = faces;
}