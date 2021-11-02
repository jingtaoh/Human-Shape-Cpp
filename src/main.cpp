#include <iostream>

#include <spdlog/spdlog.h>

#include "core/Model.h"
#include "io/read_mat.h"
#include "io/read_model.h"
#include "ui/Viewer.h"
#include "ui/ModelRenderer.h"

#define DATA_DIR "../data/"

using namespace MoShape;

int main()
{
    // Read human shape model
    auto model = read_model(DATA_DIR "/model.dat");

    // Read shape space eigenvectors
    Vector2i dims(20, 19347);
    auto evectors = read_mat(DATA_DIR "/evectors.mat", dims);

    // Construct shape vector and pose vector
    Shape shape(20, 1);
    Pose pose(31, 0);

    // Change shape and pose
    model.change_shape_and_pose(shape, evectors, pose);

    Viewer viewer("MoShape demo", 1024, 768);
    // Must initialize after the viewer
    ModelRenderer mr(model);

    while (!viewer.should_close()) {
        viewer.begin_frame();
        mr.render_model(true, true);
        viewer.end_frame();
    }
    return 0;
}
