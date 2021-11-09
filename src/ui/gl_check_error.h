//
// Created by Jingtao Huang on 10/30/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_GL_CHECK_ERROR_H
#define MOSHAPE_GL_CHECK_ERROR_H

#include <GL/gl3w.h>
#include <spdlog/spdlog.h>
#include <string>

namespace MoShape {
void gl_check_error(const char* file, unsigned int line);
} // namespace MoShape

#endif // MOSHAPE_GL_CHECK_ERROR_H
