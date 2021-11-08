# eigen
add_subdirectory(eigen)

# spdlog
add_subdirectory(spdlog)

# matio
add_subdirectory(matio)

# glfw
add_subdirectory(glfw)

# gl3w
add_subdirectory(gl3w)

# glm
add_subdirectory(glm)

# imgui
set(IMGUI_ROOT imgui)
file(GLOB IMGUI_SRC ${IMGUI_ROOT}/*.cpp
        ${IMGUI_ROOT}/backends/imgui_impl_opengl3.cpp
        ${IMGUI_ROOT}/backends/imgui_impl_glfw.cpp
        )
add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC ${IMGUI_ROOT})
target_link_libraries(imgui PUBLIC glfw gl3w)