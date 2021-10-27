#include <iostream>

#include <spdlog/spdlog.h>

#include "core/Model.h"
#include "io/read_mat.h"
#include "io/read_model.h"

using namespace MoShape;

int main()
{
    // Read human shape model
    auto model = read_model("../data/model.dat");

    // Read shape space eigenvectors
    Vector2i dims(20, 19347);
    auto evectors = read_mat("../data/evectors.mat", dims);

    // Construct shape vector and pose vector
    Shape shape(20, 1);
    Pose pose(31, 0);

    // Change shape and pose
    model.change_shape_and_pose(shape, evectors, pose);

    spdlog::info("Hello, World!");
    return 0;
}
