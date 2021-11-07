#include <iostream>

#include <spdlog/spdlog.h>

#include "core/Model.h"
#include "io/read_mat.h"
#include "io/read_model.h"
#include "ui/Renderer.h"
#include "ui/Viewer.h"

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

    float window_width = 1024;
    float window_height = 768;

    Viewer viewer("MoShape demo", window_width, window_height);
    // Must initialize after the viewer
    Camera camera = Camera::default_camera(window_width, window_height);
    Renderer mr(model, camera);

    while (!viewer.should_close()) {
        viewer.begin_frame();
        mr.update_camera(viewer.get_window());
        mr.render_model(true, true);
        viewer.end_frame();
    }
    return 0;
}
