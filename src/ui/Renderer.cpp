//
// Created by Jingtao Huang on 10/31/21.
// Copyright (c) 2021 University of Southern California. All rights reserved.
//

#include "Renderer.h"
#include <GLFW/glfw3.h>
#include <Eigen/Dense>
#include <glm/gtc/matrix_transform.hpp>
#include "gl_check_error.h"
#include "utils/compute_face_normal.h"
#include "utils/unify_index_buffer.h"
#define SHADER_DIR "../../src/ui/shaders"

using namespace Eigen;

namespace MoShape {
typedef std::vector<Vector4f> vector;
Renderer::Renderer(Model& model, const Camera& camera)
    : m_model(model)
    , m_camera(camera)
    , m_mesh(
          init_mesh(model.get_mesh(), Color::white()),
          Shader(SHADER_DIR "/default.vert", GL_VERTEX_SHADER),
          Shader(SHADER_DIR "/phong.frag", GL_FRAGMENT_SHADER),
          Primitive::TRIANGLES)
    , m_skeleton(
          init_skeleton(model.get_skeleton(), Color::white(), Color::white()),
          Shader(SHADER_DIR "/default.vert", GL_VERTEX_SHADER),
          Shader(SHADER_DIR "/phong.frag", GL_FRAGMENT_SHADER),
          Primitive::TRIANGLES)
{
    glfwWindowHint(GLFW_SAMPLES, 4);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

MeshData Renderer::init_mesh(Mesh& mesh, const Color& color)
{
    // Creation of the mesh
    std::vector<Vector3f> old_vertices;
    std::vector<Vector3i> old_indices;

    for (int i = 0; i < mesh.get_num_vertices(); i++) {
        Vector3f vertex_pos = mesh.get_vertex_pos_by_id(i).cast<float>();
        old_vertices.emplace_back(vertex_pos);
    }


    for (int i = 0; i < mesh.get_num_faces(); i++) {
        Vector3i face_idx = mesh.get_face_indices_by_id(i);
        old_indices.emplace_back(face_idx);
    }

    std::vector<Vector3f> new_vertices;
    std::vector<Vector3f> new_normals;
    std::vector<Color> new_colors;
    std::vector<unsigned int> new_indices;

    for (int i = 0; i < old_indices.size(); i++) {
        for (int j = 0; j < 3; j++) {
            new_vertices.push_back(old_vertices[old_indices[i](j)]);
        }
        Vector3f face_normal;
        compute_face_normal(
            new_vertices[i * 3],
            new_vertices[i * 3 + 1],
            new_vertices[i * 3 + 2],
            face_normal);
        for (int j = 0; j < 3; j++) {
            new_normals.push_back(face_normal);
            new_colors.push_back(color);
            new_indices.push_back((unsigned int)old_indices[i](j));
        }
    }

    MeshData res;
    unify_index_buffer(new_vertices, new_normals, new_colors, res.m_indices, res.m_vertices);


    return res;
}

MeshData Renderer::init_skeleton(Skeleton& skel, const Color& joint_color, const Color& bone_color)
{
    MeshData res;
    for (int jid = 0; jid < skel.get_num_joints(); jid++) {
        Vector3f joint_pos = skel.get_joint_by_id(jid).get_position().cast<float>();
        draw_joint(joint_pos, 2, joint_color, 10, 5, res);

        Vector3f joint_dir = skel.get_joint_by_id(jid).get_direction().cast<float>();
        int pid = skel.get_parent_by_id(jid);
        if (pid == -1) continue;
        Vector3f parent_pos = skel.get_joint_by_id(pid).get_position().cast<float>();

        draw_bone(parent_pos, joint_pos, joint_dir, bone_color, res);
    }
    return res;
}

void Renderer::render_model(bool render_mesh, bool render_skeleton)
{
    if (render_mesh && render_skeleton) {
        Renderer::render_mesh(true);
        Renderer::render_skeleton();
    } else if (render_mesh) {
        Renderer::render_mesh(false);
    } else if (render_skeleton) {
        Renderer::render_skeleton();
    }
}

void Renderer::render_mesh(bool wireframe) const
{
    m_mesh.draw(wireframe);
}

void Renderer::render_skeleton() const
{
    m_skeleton.draw(false);
}

void Renderer::update_model()
{
    m_model.update_model();
    m_mesh.re_init(init_mesh(m_model.get_mesh(), Color::white()));
    m_skeleton.re_init(init_skeleton(m_model.get_skeleton(), Color::white(), Color::white()));
}

void Renderer::update_light()
{
    auto mesh_program = m_mesh.get_shader_program();

    mesh_program.use();
    gl_check_error(__FILE__, __LINE__);

    mesh_program.set_uniform("light.dir", glm::vec3(0, 0, -1));
    mesh_program.set_uniform("light.ambient", glm::vec3(0.1));
    mesh_program.set_uniform("light.diffuse", glm::vec3(0.7));
    mesh_program.set_uniform("light.specular", glm::vec3(0.1));
    gl_check_error(__FILE__, __LINE__);

    auto skeleton_program = m_skeleton.get_shader_program();

    skeleton_program.use();
    gl_check_error(__FILE__, __LINE__);

    skeleton_program.set_uniform("light.dir", glm::vec3(0, 0, -1));
    skeleton_program.set_uniform("light.ambient", glm::vec3(0.1));
    skeleton_program.set_uniform("light.diffuse", glm::vec3(0.7));
    skeleton_program.set_uniform("light.specular", glm::vec3(0.1));
    gl_check_error(__FILE__, __LINE__);
}

void Renderer::update_camera(GLFWwindow* window)
{
    m_camera.detect_window_dimension_change(window);

    auto model = glm::mat4(1.0f);
    model = glm::rotate(model, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1, 0, 0));

    auto view = m_camera.get_view();
    auto projection = m_camera.get_projection();

    auto normal_matrix = glm::transpose(glm::inverse(view * model));

    auto mesh_program = m_mesh.get_shader_program();
    auto skeleton_program = m_skeleton.get_shader_program();

    mesh_program.use();
    gl_check_error(__FILE__, __LINE__);

    mesh_program.set_uniform("model", model);
    mesh_program.set_uniform("view", view);
    mesh_program.set_uniform("normal_matrix", normal_matrix);
    mesh_program.set_uniform("projection", projection);

    skeleton_program.use();
    gl_check_error(__FILE__, __LINE__);

    skeleton_program.set_uniform("model", model);
    skeleton_program.set_uniform("view", view);
    skeleton_program.set_uniform("normal_matrix", normal_matrix);
    skeleton_program.set_uniform("projection", projection);
}

void Renderer::draw_triangle(
    Vector3f& v0,
    Vector3f& v1,
    Vector3f& v2,
    Color color,
    MeshData& mesh_data)
{
    Vector3f a = v1 - v0;
    Vector3f b = v2 - v0;
    Vector3f n = a.cross(b);
    auto idx = (unsigned int)mesh_data.m_vertices.size();
    mesh_data.m_vertices.emplace_back(v0, n, color);
    mesh_data.m_vertices.emplace_back(v1, n, color);
    mesh_data.m_vertices.emplace_back(v2, n, color);
    mesh_data.m_indices.emplace_back(idx);
    mesh_data.m_indices.emplace_back(idx + 1);
    mesh_data.m_indices.emplace_back(idx + 2);
}

void Renderer::draw_bone(
    Vector3f& head,
    Vector3f& tail,
    Vector3f& joint_dir,
    Color color,
    MeshData& mesh_data)
{
    Vector3f dummy_n = Vector3f(0, 0, 1);
    mesh_data.m_vertices.emplace_back(head, dummy_n, color);
    mesh_data.m_vertices.emplace_back(tail, dummy_n, color);

    float length_ratio = 1 / 10.0f;
    Vector3f bone = tail - head;
    float half_side_length = bone.norm() / 18.f;
    Vector3f cut_point = length_ratio * bone + head;
    Vector3f bn = (bone.cross(joint_dir)).normalized();
    Vector3f n = (bn.cross(bone)).normalized();

    Vector3f c0 = cut_point - half_side_length * bn - half_side_length * n;
    Vector3f c1 = cut_point - half_side_length * bn + half_side_length * n;
    Vector3f c2 = cut_point + half_side_length * bn - half_side_length * n;
    Vector3f c3 = cut_point + half_side_length * bn + half_side_length * n;

    draw_triangle(head, c0, c2, color, mesh_data);
    draw_triangle(head, c2, c3, color, mesh_data);
    draw_triangle(head, c3, c1, color, mesh_data);
    draw_triangle(head, c1, c0, color, mesh_data);

    draw_triangle(tail, c0, c1, color, mesh_data);
    draw_triangle(tail, c2, c0, color, mesh_data);
    draw_triangle(tail, c3, c2, color, mesh_data);
    draw_triangle(tail, c1, c3, color, mesh_data);
    // indices
    //  c1---c3
    //  | \ / |
    //  |  c  |
    //  | / \ |
    //  c0---c2
}

// http://www.songho.ca/opengl/gl_sphere.html
void Renderer::draw_joint(
    Vector3f center,
    float radius,
    Color color,
    int sector_count,
    int stack_count,
    MeshData& mesh_data)
{
    unsigned int current_idx = mesh_data.m_vertices.size();

    float x, y, z, xy; // vertex position
    float nx, ny, nz, radius_inv = 1.0f / radius; // vertex normal

    float sector_step = 2 * M_PI / sector_count;
    float stack_step = M_PI / stack_count;
    float sector_angle, stack_angle;

    for (int i = 0; i <= stack_count; ++i) {
        stack_angle = M_PI_2 - i * stack_step; // starting from pi/2 to -pi/2

        xy = radius * std::cos(stack_angle); // r * cos(u)
        z = radius * std::sin(stack_angle); // r * sin(u)

        // Add (sector_count + 1) vertices per stack
        for (int j = 0; j <= sector_count; ++j) {
            sector_angle = j * sector_step; // starting from 0 to 2pi

            // vertex position
            x = xy * std::cos(sector_angle); // r * cos(u) * cos(v)
            y = xy * std::sin(sector_angle); // r * cos(u) * sin(v)

            // normalized vertex normal
            nx = x * radius_inv;
            ny = y * radius_inv;
            nz = z * radius_inv;

            mesh_data.m_vertices.emplace_back(
                center + Vector3f(x, y, z),
                Vector3f(nx, ny, nz),
                color);
        }
    }

    // indices
    //  k1--k1+1
    //  |  / |
    //  | /  |
    //  k2--k2+1
    unsigned int k1, k2;

    for (int i = 0; i < stack_count; ++i) {
        k1 = i * (sector_count + 1); // beginning of current stack
        k2 = k1 + sector_count + 1; // beginning of next stack

        for (int j = 0; j < sector_count; ++j, ++k1, ++k2) {
            // 2 triangles per sector excluding 1st and last stacks
            if (i != 0) {
                mesh_data.m_indices.push_back(k1 + current_idx);
                mesh_data.m_indices.push_back(k2 + current_idx);
                mesh_data.m_indices.push_back(k1 + 1 + current_idx);
            }

            if (i != stack_count - 1) {
                mesh_data.m_indices.push_back(k1 + 1 + current_idx);
                mesh_data.m_indices.push_back(k2 + current_idx);
                mesh_data.m_indices.push_back(k2 + 1 + current_idx);
            }
        }
    }
}

void Renderer::save_mesh(fs::path path)
{
    std::ofstream outfile(path, std::ios::out);

    assert(outfile && "Cannot open file for writing");

    int num_vertices = m_mesh.m_mesh_data.m_vertices.size();
    int num_faces = m_mesh.m_mesh_data.m_indices.size() / 3;

    // Write vertices
    for (int i = 0; i < num_vertices; i++) {
        Vector3f pos = m_mesh.m_mesh_data.m_vertices[i].position;
        outfile << "v " << pos.x() << " " << pos.y() << " " << pos.z() << std::endl;
    }

    // Write faces
    for (int i = 0; i < num_faces; i++) {
        outfile << "f ";
        for (int j = 0; j < 3; j++) {
            outfile << m_mesh.m_mesh_data.m_indices[i * 3 + j] + 1 << " ";
        }
        outfile << std::endl;
    }
}
} // namespace MoShape