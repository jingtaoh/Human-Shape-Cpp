add_executable(MoShape
        main.cpp

        core/Joint.h core/Joint.cpp

        core/Skeleton.h core/Skeleton.cpp

        core/Mesh.h core/Mesh.cpp

        core/Model.h core/Model.cpp

        core/Motion.h core/Motion.cpp

        core/SkinnedVertex.cpp core/SkinnedVertex.h

        io/read_model.h io/read_mat.h

        core/convert_axis_angle_to_matrix.h core/convert_axis_angle_and_translation_to_matrix.h

        ui/Viewer.cpp ui/Viewer.h)

target_link_libraries(MoShape PUBLIC Eigen3::Eigen spdlog matio gl3w glfw)
