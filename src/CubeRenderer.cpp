#include "CubeRenderer.hpp"

#include <vector>
#include <algorithm>
#include <iterator>
#include <cmath>

#include <glm/gtc/constants.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include "Util.hpp"
#include "ShaderSources.hpp"
#include "MeshData.hpp"

namespace cubedemo
{
    static glm::vec3 calculateLightPosition(const glm::vec3& center, const GameTimePoint& time, float radius, float speed)
    {
        static const auto TWO_PI = glm::pi<float>() * 2.0f;
        
        auto t = speed * TWO_PI * time.total();
        auto x = center.x + (cosf(t) * radius);
        auto z = center.z + (sinf(t) * radius);

        return glm::vec3{ x, center.y, z };
    }

    CubeRenderer::CubeRenderer()
        : m_instanceCount{ 0 },
        m_positionsBuffer{ gl::RGBA32F },
        m_opacitiesBuffer{ gl::R32F },
        m_scalesBuffer{ gl::R32F },
        m_rotationsBuffer{ gl::RGBA32F },
        m_modelviewMatrix{ glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 1.0f, 0.0f)) },
        m_lightPosition{ 0.0f }
    {
        // generate buffers and textures
        gl::GenVertexArrays(1, &m_vao);
        gl::GenBuffers(1, &m_positionsVBO);
        gl::GenBuffers(1, &m_normalsVBO);
        gl::GenBuffers(1, &m_indices);
        GL_CHECK_ERRORS;

        // set up shader
        m_shader.attachShaderFromSource(gl::VERTEX_SHADER, shaderSourceCubesVert());
        m_shader.attachShaderFromSource(gl::FRAGMENT_SHADER, shaderSourceCubesFrag());
        m_shader.link();
        m_shader.addAttributes({ "position", "normal" });
        m_shader.addUniforms({ "MVP", "InstancePositions" , "ModelViewMatrix", "ProjectionMatrix", "NormalMatrix", "LightPosition", "LightIntensity", "Kd", "Ka", "Ks", "Shininess", "Gamma", "InstanceOpacities", "InstanceScales", "InstanceRotations" });
        GL_CHECK_ERRORS;

        // set up vao
        gl::BindVertexArray(m_vao);
        {
            // "base" positions without instance offsets
            gl::BindBuffer(gl::ARRAY_BUFFER, m_positionsVBO);
            gl::BufferData(gl::ARRAY_BUFFER, sizeof(float) * 3 * ROUNDED_CUBE_VERTICES.size(), ROUNDED_CUBE_VERTICES.data(), gl::STATIC_DRAW);
            gl::EnableVertexAttribArray(m_shader["position"]);
            gl::VertexAttribPointer(m_shader["position"], 3, gl::FLOAT, gl::FALSE_, 0, nullptr);
            GL_CHECK_ERRORS;

            // normals
            gl::BindBuffer(gl::ARRAY_BUFFER, m_normalsVBO);
            gl::BufferData(gl::ARRAY_BUFFER, sizeof(float) * 3 * ROUNDED_CUBE_NORMALS.size(), ROUNDED_CUBE_NORMALS.data(), gl::STATIC_DRAW);
            gl::EnableVertexAttribArray(m_shader["normal"]);
            gl::VertexAttribPointer(m_shader["normal"], 3, gl::FLOAT, gl::FALSE_, 0, nullptr);
            GL_CHECK_ERRORS;

            // indices
            gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, m_indices);
            gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * ROUNDED_CUBE_INDICES.size(), ROUNDED_CUBE_INDICES.data(), gl::STATIC_DRAW);
        }
        gl::BindVertexArray(0);
    }

    CubeRenderer::~CubeRenderer()
    {
        gl::DeleteBuffers(1, &m_indices);
        gl::DeleteBuffers(1, &m_normalsVBO);
        gl::DeleteBuffers(1, &m_positionsVBO);
        gl::DeleteVertexArrays(1, &m_vao);
        GL_CHECK_ERRORS;
    }

    void CubeRenderer::onWindowSizeChanged(size_t width, size_t height)
    {
        m_projectionMatrix = glm::perspective(glm::quarter_pi<float>(), float(width) / height, 0.1f, 1000.0f);
    }

    void CubeRenderer::update(const GameTimePoint& time, const CubeController& cubes)
    {
        m_lightPosition = calculateLightPosition(glm::vec3(0.0f, 0.0f, 150.0f), time, 225.0f, 0.20f);

        m_instanceCount = cubes.count();
        std::vector<glm::vec4> processedPositions;
        std::vector<glm::quat> processedRotations;
        processedPositions.reserve(m_instanceCount);
        processedRotations.reserve(m_instanceCount);

        // Process vec3's to vec4's
        // Note: w component is always 0 because this is an offset
        // which will be added onto another vec4
        auto positionSource = cubes.cubePositions();
        std::transform(positionSource, positionSource + cubes.count(), std::back_inserter(processedPositions),
            [](const glm::vec3& v) { return glm::vec4(v, 0.0f); });

        // process axis, rotation speed, and time into quaternion rotations
        auto rotationAxisSource = cubes.cubeRotationAxes();
        auto rotationSpeedSource = cubes.cubeRotationSpeeds();
        for (size_t i = 0; i < m_instanceCount; i++)
        {
            float angle = time.total() * rotationSpeedSource[i];
            glm::quat rotation = glm::angleAxis(angle, rotationAxisSource[i]);
            processedRotations.push_back(rotation);
        }

        m_positionsBuffer.updateData(sizeof(glm::vec4) * processedPositions.size(), processedPositions.data(), gl::STREAM_DRAW);
        m_opacitiesBuffer.updateData(sizeof(float) * cubes.count(), cubes.cubeOpacities(), gl::STREAM_DRAW);
        m_scalesBuffer.updateData(sizeof(float) * cubes.count(), cubes.cubeScales(), gl::STREAM_DRAW);
        m_rotationsBuffer.updateData(sizeof(glm::quat) * processedRotations.size(), processedRotations.data(), gl::STREAM_DRAW);
    }

    void CubeRenderer::render()
    {
        const float n = 0.1f;
        const float SHININESS = 16.0f;
        const glm::vec3 AMBIENT_COLOR{ 0.46f*n, 0.24f*n, 0.16f*n };
        const glm::vec3 DIFFUSE_COLOR{ 0.45f*n, 0.26f*n, 0.12f*n };
        const glm::vec3 SPECULAR_COLOR{ 0.56f, 0.25f, 0.16f };
        
        auto mvp = m_projectionMatrix * m_modelviewMatrix;
        auto normalMat = glm::inverseTranspose(glm::mat3(m_modelviewMatrix));

        glm::vec3 lightIntensity{ 1.0f };

        gl::BindVertexArray(m_vao);
        m_shader.use();
        {
            m_positionsBuffer.bind(0, m_shader("InstancePositions")); // Position buffer texture
            m_opacitiesBuffer.bind(1, m_shader("InstanceOpacities")); // Opacity buffer texture
            m_scalesBuffer.bind(2, m_shader("InstanceScales")); // Scale buffer texture
            m_rotationsBuffer.bind(3, m_shader("InstanceRotations")); // Rotation buffer texture

            // Uniforms
            gl::UniformMatrix4fv(m_shader("MVP"), 1, gl::FALSE_, glm::value_ptr(mvp));
            gl::UniformMatrix4fv(m_shader("ModelViewMatrix"), 1, gl::FALSE_, glm::value_ptr(m_modelviewMatrix));
            gl::UniformMatrix4fv(m_shader("ProjectionMatrix"), 1, gl::FALSE_, glm::value_ptr(m_projectionMatrix));
            gl::UniformMatrix3fv(m_shader("NormalMatrix"), 1, gl::FALSE_, glm::value_ptr(normalMat));
            gl::Uniform1f(m_shader("Shininess"), SHININESS);
            gl::Uniform3fv(m_shader("LightPosition"), 1, glm::value_ptr(m_lightPosition));
            gl::Uniform3fv(m_shader("LightIntensity"), 1, glm::value_ptr(lightIntensity));
            gl::Uniform3fv(m_shader("Kd"), 1, glm::value_ptr(DIFFUSE_COLOR));
            gl::Uniform3fv(m_shader("Ka"), 1, glm::value_ptr(AMBIENT_COLOR));
            gl::Uniform3fv(m_shader("Ks"), 1, glm::value_ptr(SPECULAR_COLOR));
            gl::Uniform1f(m_shader("Gamma"), GAMMA);
            GL_CHECK_ERRORS;

            // Draw instanced elements
            gl::DrawElementsInstanced(gl::TRIANGLES, GLsizei(ROUNDED_CUBE_INDICES.size()), gl::UNSIGNED_INT, nullptr, GLsizei(m_instanceCount));
            GL_CHECK_ERRORS;
        }
        m_shader.unuse();
        gl::BindVertexArray(0);
    }
}
