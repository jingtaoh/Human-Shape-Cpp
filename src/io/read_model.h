//
// Created by Jingtao Huang on 10/24/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_READ_MODEL_H
#define MOSHAPE_READ_MODEL_H

#include <fstream>
#include <filesystem>
#include "../core/Model.h"

namespace fs = std::filesystem;

/**
 * @file
 * @brief read human shape model from disk
 *
 * @param [in] path path of the model file
 * @return model with mean body shape and standard posture
 */
Model read_model(fs::path path)
{
    std::ifstream infile(path, std::ios::in);

    assert(infile && "Cannot open file");

    // Read the number of vertices, faces, joints, and bound joints
    int num_vertices, num_faces, num_joints, num_bound_joints;
    infile >> num_vertices;
    infile >> num_faces;
    infile >> num_joints;
    infile >> num_bound_joints;

    // Read vertex positions, their bound joints and skinning weights
    std::vector<SkinnedVertex> vertices;
    for (int i = 0; i < num_vertices; i++) {
        // vertex position
        Vector3d pos;
        infile >> pos.x() >> pos.y() >> pos.z();

        // bound joints and skinning weights
        std::vector<int> joint_indices(num_bound_joints);
        std::vector<double> joint_weights(num_bound_joints);
        for (int j = 0; j < num_bound_joints; j++) {
            infile >> joint_indices[j] >> joint_weights[j];
        }
        vertices.emplace_back(pos, joint_indices, joint_weights);
    }

    // Read faces
    std::vector<Vector3i> faces;
    for (int i = 0; i < num_faces; i++) {
        Vector3i face;
        infile >> face[0] >> face[1] >> face[2];
        faces.emplace_back(face);
    }

    Mesh mesh(vertices, faces);

    // Read joints
    std::vector<Joint> joints;
    for (int i = 0; i < num_joints; i++) {
        int dummy_id;
        infile >> dummy_id;

        // Joint direction
        Vector3d joint_dir;
        infile >> joint_dir.x() >> joint_dir.y() >> joint_dir.z();

        // Joint position
        Vector3d joint_pos;
        infile >> joint_pos.x() >> joint_pos.y() >> joint_pos.z();

        // Parent
        int parent;
        infile >> parent;

        joints.emplace_back(joint_pos, joint_dir, parent - 1);
    }

    Skeleton skel(joints);

    return Model(mesh, skel);
}

#endif // MOSHAPE_READ_MODEL_H
