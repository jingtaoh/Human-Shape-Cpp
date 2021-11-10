#include <iostream>

#include <spdlog/spdlog.h>
#include <imgui.h>

#include "core/Model.h"
#include "io/read_mat.h"
#include "io/read_model.h"
#include "ui/Renderer.h"
#include "ui/Viewer.h"

using namespace MoShape;

int main()
{
    // Read human shape model
    auto model = read_model(DATA_DIR "/model.dat");
    model.center_model();

    // Read shape space eigenvalues and eigenvectors
    Vector2i dims(1, 20);
    auto eigenvalues = read_mat(DATA_DIR "/evalues.mat", dims);
    dims = Vector2i(20, 19347);
    auto eigenvectors = read_mat(DATA_DIR "/evectors.mat", dims);

    // Construct shape vector and pose vector
    Shape shape(20, 0);
    Pose pose(31, 0);

    // Change shape and pose
    int sid = 0;
    shape[sid] = 3 * sqrt(eigenvalues(sid)); // change height to 3 st.d. from mean
    //    pose[23] = glm::radians(60.0f);           // rotate right shoulder by 60 degrees
    //    pose[0] = 10.0f;                          // translate (10, 0, 0)
    model.change_shape_and_pose(shape, eigenvectors, pose);

    float window_width = 1024;
    float window_height = 768;

    Viewer viewer("MoShape demo", window_width, window_height);
    // Must initialize after the viewer
    Camera camera = Camera::default_camera(window_width, window_height);
    Renderer mr(model, camera);

    bool render_mesh = true;
    bool render_skeleton = false;

    while (!viewer.should_close()) {
        viewer.begin_frame();

        ImGui::Begin("Render Options");
        ImGui::Checkbox("Render Mesh", &render_mesh);
        ImGui::SameLine();
        ImGui::Checkbox("Render Skeleton", &render_skeleton);
        ImGui::End();

        mr.update_camera(viewer.get_window());
        mr.update_light();
        mr.render_model(render_mesh, render_skeleton);
        viewer.end_frame();
    }
    return 0;
}
