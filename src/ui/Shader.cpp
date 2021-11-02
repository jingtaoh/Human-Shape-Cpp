//
// Created by Jingtao Huang on 10/30/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#include "Shader.h"
#include <spdlog/spdlog.h>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

namespace MoShape {

// file reading
void get_file_contents(const char* filename, std::vector<char>& buffer)
{
    std::ifstream file(filename, std::ios_base::binary);
    if (file) {
        file.seekg(0, std::ios_base::end);
        std::streamsize size = file.tellg();
        if (size > 0) {
            file.seekg(0, std::ios_base::beg);
            buffer.resize(static_cast<size_t>(size));
            file.read(&buffer[0], size);
        }
        buffer.push_back('\0');
    } else {
        spdlog::error(std::string("The file ") + filename + " doesn't exists");
    }
}

Shader::Shader(const std::string& filename, GLenum type)
{
    // file loading
    std::vector<char> file_content;
    get_file_contents(filename.c_str(), file_content);

    // creation
    handle = glCreateShader(type);
    if (handle == 0) spdlog::error("Failed to create a new Shader");

    // code source assignation
    const char* shader_text(&file_content[0]);
    glShaderSource(handle, 1, (const GLchar**)&shader_text, NULL);

    // compilation
    glCompileShader(handle);

    // compilation check
    GLint compile_status;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &compile_status);
    if (compile_status != GL_TRUE) {
        spdlog::error("Compilation error: " + filename);

        GLsizei log_size = 0;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &log_size);

        char* log = new char[log_size + 1];
        glGetShaderInfoLog(handle, log_size, &log_size, log);

        spdlog::error(log);

        exit(EXIT_FAILURE);
    } else {
        spdlog::info("Shader" + filename + " compiled successfully");
    }
}
Shader::~Shader() {}

GLuint Shader::get_handle() const
{
    return handle;
}

ShaderProgram::ShaderProgram()
{
    handle = glCreateProgram();
    if (!handle) spdlog::error("Failed to create a new shader program");
}

ShaderProgram::ShaderProgram(std::initializer_list<Shader> shader_list)
    : ShaderProgram()
{
    for (auto& s : shader_list) glAttachShader(handle, s.get_handle());

    link();
}

ShaderProgram::~ShaderProgram() {}

void ShaderProgram::use() const
{
    glUseProgram(handle);
}

void ShaderProgram::unuse() const
{
    glUseProgram(0);
}

GLint ShaderProgram::attribute(const std::string& name)
{
    GLint attrib = glGetAttribLocation(handle, name.c_str());
    if (attrib == GL_INVALID_OPERATION || attrib < 0)
        spdlog::error("Attribute " + name + " doesn't exist in program");

    return attrib;
}

void ShaderProgram::set_attribute(
    const std::string& name,
    GLint size,
    GLsizei stride,
    GLuint offset,
    GLboolean normalize,
    GLenum type)
{
    GLint loc = attribute(name);
    glEnableVertexAttribArray(loc);
    glVertexAttribPointer(loc, size, type, normalize, stride, reinterpret_cast<void*>(offset));
}

void ShaderProgram::set_attribute(
    const std::string& name,
    GLint size,
    GLsizei stride,
    GLuint offset)
{
    set_attribute(name, size, stride, offset, false, GL_FLOAT);
}

GLint ShaderProgram::uniform(const std::string& name)
{
    auto it = uniforms.find(name);
    if (it == uniforms.end()) {
        // uniform that is not referenced
        GLint r = glGetUniformLocation(handle, name.c_str());
        if (r == GL_INVALID_OPERATION || r < 0)
            spdlog::error("Uniform " + name + " doesn't exist in program");
        // add it anyways
        uniforms[name] = r;

        return r;
    } else
        return it->second;
}

void ShaderProgram::set_uniform(const std::string& name, const glm::mat4& m)
{
    glUniformMatrix4fv(uniform(name), 1, GL_FALSE, value_ptr(m));
}

void ShaderProgram::link()
{
    glLinkProgram(handle);
    GLint result;
    glGetProgramiv(handle, GL_LINK_STATUS, &result);
    if (result != GL_TRUE) {
        spdlog::error("Linkage error");

        GLsizei log_size = 0;
        glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &log_size);

        char* log = new char[log_size];
        glGetProgramInfoLog(handle, log_size, &log_size, log);

        spdlog::error(log);
    } else
    {
        spdlog::info("Shader programs linked successfully");
    }
}
} // namespace MoShape