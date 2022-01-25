add_executable(demo
        main.cpp
        core/Joint.h core/Joint.cpp
        core/Skeleton.h core/Skeleton.cpp
        core/Mesh.h core/Mesh.cpp
        core/Model.h core/Model.cpp
        core/SkinnedVertex.cpp core/SkinnedVertex.h
        io/read_model.h io/read_mat.h
        core/convert_axis_angle_to_matrix.h core/convert_axis_angle_and_translation_to_matrix.h
        ui/Viewer.cpp ui/Viewer.h
        ui/Color.h
        ui/Shader.cpp ui/Shader.h
        ui/gl_check_error.cpp ui/gl_check_error.h
        ui/Renderer.cpp ui/Renderer.h
        ui/Drawable.cpp ui/Drawable.h
        ui/Camera.cpp ui/Camera.h
        ui/utils/unify_index_buffer.cpp ui/utils/unify_index_buffer.h ui/utils/compute_face_normal.h)

target_link_libraries(demo PUBLIC Eigen3::Eigen spdlog matio gl3w glfw glm imgui)
