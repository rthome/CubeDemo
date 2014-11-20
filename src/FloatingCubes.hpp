#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/quaternion.hpp>

#include "gl_core_3_3.hpp"
#include "GLShader.hpp"

namespace cubedemo
{
	// The states a cube can be in:
	// Spawned - Just spawned, not yet fading in
	// FadeIn - Just spawned, fading it in
	// Moving - Moving around
	// FadeOut - Despawning, but still fading out
	// Dead - Ready to be destroyed
	enum class CubeState
	{
		Dead,
		Spawned,
		FadeIn,
		Moving,
		FadeOut,
	};

	// Collects the state of a bunch of cubes, floating in space
	class FloatingCubes
	{
	private:
		size_t m_cubeCount; // Amount of managed cubes
        
        // global cube state
        glm::vec4 m_cubeColor;
        
        // per-cube state
		CubeState *m_cubeStates; // The state each cube is in
		glm::vec3 *m_cubePositions; // Position of each cube
		glm::vec3 *m_cubeVelocities; // Movement direction and speed of each cube
		glm::quat *m_cubeRotations; // Rotation of each cube

	public:
        FloatingCubes(size_t count);
		~FloatingCubes();

		inline size_t count() const { return m_cubeCount; }
        inline const glm::vec4& cubeColor() const { return m_cubeColor; }
		inline CubeState* cubeStates() const { return m_cubeStates; }
		inline glm::vec3* cubePositions() const { return m_cubePositions; }
		inline glm::quat* cubeRotations() const { return m_cubeRotations; }

		void update(double deltaTime); // Update the state of each cube and
	};

	// Renders cubes from FloatingCubes
	class FloatingCubesRenderer
	{
	private:
		GLuint m_vao; // Vertex array object
		GLuint m_positionsVBO; // VBO for base position data
		GLuint m_normalsVBO; // VBO for normal data
		GLuint m_indices; // EBO for cube indices
		GLShader m_shader; // GLSL shader program

		size_t m_instanceCount; // Count of instances to render
		GLuint m_positionBufferTexture; // Buffer texture for instance position offsets
		GLuint m_instancePositionsVBO; // Backing VBO for instance position offset data

		// Matrices
		glm::mat4 m_projectionMatrix;
		glm::mat4 m_modelviewMatrix;

	public:
		FloatingCubesRenderer();
		~FloatingCubesRenderer();

		void onWindowSizeChanged(size_t width, size_t height); // Notify the renderer of a changed window size, to allow it to update the projection matrix

		void update(const FloatingCubes& cubes); // Update renderer state, pulling data from a FloatingCubes instance
		void render(double deltaTime); // Draw latest cube data to the screen
	};
}
