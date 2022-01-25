#include <imgui.h>
#include <spdlog/spdlog.h>

#include "core/Model.h"
#include "io/read_mat.h"
#include "io/read_model.h"
#include "ui/Renderer.h"
#include "ui/Viewer.h"

using namespace MoShape;

int main()
{
    // Read human shape model
    auto model = read_model(DATA_DIR "/caesar/model.dat");
    model.center_model();
    model.update_skeleton();

    // Read shape space eigenvalues and eigenvectors
    Vector2i dims(1, 20);
    auto eigenvalues = read_mat(DATA_DIR "/caesar/evalues.mat", dims);
    dims = Vector2i(20, 19347);
    auto eigenvectors = read_mat(DATA_DIR "/caesar/evectors.mat", dims);
    model.set_eigenvalues_and_eigenvectors(eigenvalues, eigenvectors);

    // Construct shape vector and pose vector
    Shape shape_vectors(20, 0);
    Pose pose_vectors(31, 0);
    std::vector<std::string> pose_labels(31);
    std::vector<std::string> shape_labels(20);

    {
        shape_labels = {"Short -> Tall", "Fat -> Thin"};
        pose_labels = {
            "global rotation x",
            "global rotation y",
            "global rotation z",
            "global translation x",
            "global translation y",
            "global translation z",
            "always 0",
            "left hip rotation x",
            "left hip rotation y",
            "left knee",
            "left foot",
            "right hip rotation x",
            "right hip rotation y",
            "right knee",
            "right foot",
            "thorax z",
            "thorax x",
            "upper neck",
            "left shoulder x",
            "left shoulder y",
            "left shoulder z",
            "left elbow",
            "left wrist",
            "right shoulder x",
            "right shoulder y",
            "right shoulder z",
            "right elbow",
            "right wrist",
            "left hip z",
            "right hip z",
            "thorax y"};
    }

    // Create opengl window
    float window_width = 1280;
    float window_height = 1024;

    Viewer viewer("MoShape demo", window_width, window_height);
    // Must initialize after the viewer
    Camera camera = Camera::default_camera(window_width, window_height);
    Renderer mr(model, camera);

    // Render options
    bool render_mesh = true;
    bool render_skeleton = true;

    while (!viewer.should_close()) {
        viewer.begin_frame();

        // Gui
        {
            ImGui::Begin("Controls");

            ImGui::PushItemWidth(100);

            ImGui::Text("Overall FPS: %.0f", ImGui::GetIO().Framerate);

            // Change render options
            ImGui::Checkbox("Render Mesh", &render_mesh);
            ImGui::SameLine();
            ImGui::Checkbox("Render Skeleton", &render_skeleton);

            // Manipulate shape vectors
            if (ImGui::CollapsingHeader("Shape Vector")) {
                for (int i = 0; i < shape_vectors.size(); i++) {
                    std::string label = "shape[" + std::to_string(i) + "] " + shape_labels[i];
                    if (ImGui::InputDouble(label.c_str(), &shape_vectors[i], 1.0, 1.0, "%.f")) {
                        mr.get_model().set_shape_vector_by_id(
                            i,
                            shape_vectors[i] * sqrt(eigenvalues(i)));
                        mr.get_model().set_dirty(true);
                    }
                }
            }

            // Manipulate pose vectors
            if (ImGui::CollapsingHeader("Pose Vector")) {
                for (int i = 0; i < 3; i++) {
                    std::string label = "pose[" + std::to_string(i) + "] " + pose_labels[i];
                    if (ImGui::InputDouble(label.c_str(), &pose_vectors[i], 30, 30, "%.f deg")) {
                        mr.get_model().set_pose_vector_by_id(i, glm::radians(pose_vectors[i]));
                        mr.get_model().set_dirty(true);
                    }
                }
                for (int i = 3; i < 6; i++) {
                    std::string label = "pose[" + std::to_string(i) + "] " + pose_labels[i];
                    if (ImGui::InputDouble(label.c_str(), &pose_vectors[i], 5.0, 10.0, "%.1f")) {
                        mr.get_model().set_pose_vector_by_id(i, glm::radians(pose_vectors[i]));
                        mr.get_model().set_dirty(true);
                    }
                }
                for (int i = 7; i < pose_vectors.size(); i++) {
                    std::string label = "pose[" + std::to_string(i) + "] " + pose_labels[i];
                    if (ImGui::InputDouble(label.c_str(), &pose_vectors[i], 30, 30, "%.f deg")) {
                        mr.get_model().set_pose_vector_by_id(i, glm::radians(pose_vectors[i]));
                        mr.get_model().set_dirty(true);
                    }
                }
            }

            if (ImGui::Button("Save Mesh"))
            {
                spdlog::info("save mesh");
                mr.save_mesh(DATA_DIR "/model.obj");
            }

            ImGui::End();
        }

        mr.update_camera(viewer.get_window());
        mr.update_light();
        if (mr.get_model().is_dirty()) mr.update_model();
        mr.render_model(render_mesh, render_skeleton);
        viewer.end_frame();
    }
    return 0;
}
