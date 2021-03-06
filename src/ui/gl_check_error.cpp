//
// Created by Jingtao Huang on 11/8/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#include "gl_check_error.h"

namespace MoShape {
void gl_check_error(const char* file, unsigned int line)
{
    GLenum error_code = glGetError();

    while (error_code != GL_NO_ERROR) {
        std::string file_string(file);
        std::string error = "unknown error";

        switch (error_code) {
        case GL_INVALID_ENUM: error = "GL_INVALID_ENUM"; break;
        case GL_INVALID_VALUE: error = "GL_INVALID_VALUE"; break;
        case GL_INVALID_OPERATION: error = "GL_INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW: error = "GL_STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW: error = "GL_STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY: error = "GL_OUT_OF_MEMORY"; break;
        }

        spdlog::error("OpenGL error: file={}  line={} error: {}", file, line, error);
        error_code = glGetError();
    }
}
} // namespace MoShape