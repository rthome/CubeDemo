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

namespace cubedemo
{
    // The states a cube can be in:
    // FadeIn - Just spawned, fading it in
    // Moving - Moving around
    // FadeOut - Despawning, but still fading out
    // Dead - Ready to be destroyed
    enum class CubeState
    {
        Dead,
        FadeIn,
        Moving,
        FadeOut,
    };

    // Contains per-cube state for a collection of cubes
    struct CubeStates
    {
        CubeState *states; // The state each vector is in
        HelixData *helices; // Helix data for each cube
        glm::vec3 *positions; // The center of each cube after applying any mapping and movement
        glm::quat *rotations; // The rotation of each cube
        float     *opacities; // The opacity of each cube, used for fade in and fade out
        float     *scales; // Adjusts the size of each cube

        CubeStates(size_t size);
        ~CubeStates();
    };

    // Collects the state of a bunch of cubes, floating in space
    class FloatingCubes
    {
    private:
        size_t m_cubeCount; // Amount of managed cubes
        CubeStates m_cubeStates; // Per-cube state

    public:
        FloatingCubes(size_t count);

        inline size_t count() const { return m_cubeCount; }
        inline const CubeState* cubeStates() const { return m_cubeStates.states; }
        inline const glm::vec3* cubePositions() const { return m_cubeStates.positions; }
        inline const glm::quat* cubeRotations() const { return m_cubeStates.rotations; }
        inline const float* cubeOpacities() const { return m_cubeStates.opacities; }
        inline const float* cubeScales() const { return m_cubeStates.scales; }

        void update(const GameTime& time); // Update the state of each cube
    };

    // Renders cubes from FloatingCubes
    class FloatingCubesRenderer : private NonCopyable
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
        FloatingCubesRenderer();
        ~FloatingCubesRenderer();

        void onWindowSizeChanged(size_t width, size_t height); // Notify the renderer of a changed window size, to allow it to update the projection matrix

        void update(const FloatingCubes& cubes); // Update renderer state, pulling data from a FloatingCubes instance
        void render(); // Draw latest cube data to the screen
    };
}
