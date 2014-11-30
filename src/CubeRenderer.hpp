#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>

#include "gl_core_3_3.hpp"
#include "GLShader.hpp"
#include "GLTextureBuffer.hpp"
#include "GameTime.hpp"
#include "Spiral.hpp"
#include "NonCopyable.hpp"
#include "CubeController.hpp"

namespace cubedemo
{
    // Renders cubes from FloatingCubes
    class CubeRenderer : private NonCopyable
    {
    public:
        // This should be set to a system-appropriate value
        const float GAMMA = 2.2f;

    private:
        GLuint m_vao; // Vertex array object
        GLuint m_positionsVBO; // VBO for base position data
        GLuint m_normalsVBO; // VBO for normal data
        GLuint m_indices; // EBO for cube indices
        GLShader m_shader; // GLSL shader program

        size_t m_instanceCount; // Count of instances to render
        GLTextureBuffer m_positionsBuffer; // Instance Positions
        GLTextureBuffer m_opacitiesBuffer; // Instance Opacities
        GLTextureBuffer m_scalesBuffer; // Instance size adjustment

        // Matrices
        glm::mat4 m_projectionMatrix;
        glm::mat4 m_modelviewMatrix;

    public:
        CubeRenderer();
        ~CubeRenderer();

        void onWindowSizeChanged(size_t width, size_t height); // Notify the renderer of a changed window size, to allow it to update the projection matrix

        void update(const CubeController& cubes); // Update renderer state, pulling data from a FloatingCubes instance
        void render(); // Draw latest cube data to the screen
    };
}
