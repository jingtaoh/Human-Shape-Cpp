# Human Shape - C++

This is the c++ implementation with OpenGL of displaying and manipulating statistical human body shape and pose model
described in the paper [Building Statistical Shape Spaces for 3D Human Modeling](https://arxiv.org/abs/1503.05860)
by [Leonid Pishchulin](https://scholar.google.de/citations?user=lECZKZsAAAAJ&hl=en) et al. Users can control the shape
and posture of a human body by adjusting the shape and pose vectors from GUI.

| Change shape | Change Pose |
| :---: | :---: |
| ![Change shape](images/change-shape.gif) | ![Change pose](images/change-pose.gif)|

## Dependencies

- [Eigen](https://gitlab.com/libeigen/eigen): Linear algebra library.
- [GL3W](https://github.com/adobe/lagrange-gl3w): OpenGL core profile loading.
- [GLFW](https://github.com/glfw/glfw): Platform-independent API for creating windows, contexts and surfaces, reading
  input, handling events, etc.
- [GLM](https://github.com/glfw/glfw): Mathematics library based on the GLSL specifications.
- [ImGUI](https://github.com/ocornut/imgui): User interface library.
- [MATIO](https://github.com/tbeu/matio): MATLAB MAT file I/O library .
- [Spdlog](https://github.com/gabime/spdlog): Logging library.

## Data Preparation

Download the models from [MPII human shape](http://humanshape.mpi-inf.mpg.de/) website.

```bash
    cd data
    wget http://datasets.d2.mpi-inf.mpg.de/humanshape/caesar.zip
    unzip caesar.zip && rm -f caesar.zip
```

## Build & Run

Make sure you have [CMake](https://cmake.org/) installed.

```bash
    mkdir build && cd build
    cmake ..
    cmake --build . 
    ./demo
```
