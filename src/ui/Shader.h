//
// Created by Jingtao Huang on 10/30/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#ifndef MOSHAPE_SHADER_H
#define MOSHAPE_SHADER_H

#include <GL/gl3w.h>
#include <glm/glm.hpp>
#include <initializer_list>
#include <map>
#include <string>

namespace MoShape {
class Shader
{
public:
    Shader(const std::string& filename, GLenum type);
    ~Shader();

    GLuint get_handle() const;

private:
    // opengl program identifier
    GLuint handle;
};

class ShaderProgram
{
public:
    ShaderProgram(std::initializer_list<Shader> shader_list);
    ~ShaderProgram();

    void use() const;
    void unuse() const;

    // provide attributes informations.
    GLint attribute(const std::string& name);

    void set_attribute(const std::string& name, GLint size, GLsizei stride, GLuint offset, GLboolean normalize, GLenum type);
    void set_attribute(const std::string& name, GLint size, GLsizei stride, GLuint offset);

    // provide uniform location
    GLint uniform(const std::string& name);

    void set_uniform(const std::string& name, const glm::mat4& m);

private:
    ShaderProgram();

    std::map<std::string, GLint> uniforms;

    GLuint handle;

    void link();
};

} // namespace MoShape


#endif // MOSHAPE_SHADER_H
