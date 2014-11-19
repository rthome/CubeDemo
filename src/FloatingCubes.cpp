#include "FloatingCubes.hpp"

#include <vector>

namespace cubedemo
{
	// // //
	// FloatingCubes implementation
	// // //

	FloatingCubes::FloatingCubes(size_t count)
		: m_cubeCount{ count }, m_cubeStates{ new CubeState[count] }, m_cubePositions{ new glm::vec3[count] }, m_cubeRotations{ new glm::quat[count] }
	{

	}

	FloatingCubes::~FloatingCubes()
	{
		delete[] m_cubeStates;
		delete[] m_cubePositions;
		delete[] m_cubeRotations;
	}

	void FloatingCubes::update(double time)
	{

	}

	// // //
	// FloatingCubesRenderer implementation
	// // //

	static const std::vector<glm::vec3> CUBE_POSITIONS
	{
		// front
		{ -1.0f,  1.0f, -1.0f },
		{ -1.0f, -1.0f, -1.0f },
		{  1.0f, -1.0f, -1.0f },

		{  1.0f, -1.0f, -1.0f },
		{  1.0f,  1.0f, -1.0f },
		{ -1.0f,  1.0f, -1.0f },
		// back
		{ -1.0f,  1.0f,  1.0f },
		{ -1.0f, -1.0f,  1.0f },
		{  1.0f, -1.0f,  1.0f },

		{  1.0f, -1.0f,  1.0f },
		{  1.0f,  1.0f,  1.0f },
		{ -1.0f,  1.0f,  1.0f },
		// top
		{ -1.0f,  1.0f,  1.0f },
		{ -1.0f,  1.0f, -1.0f },
		{  1.0f,  1.0f, -1.0f },

		{  1.0f,  1.0f, -1.0f },
		{  1.0f,  1.0f,  1.0f },
		{ -1.0f,  1.0f,  1.0f },
		// bottom
		{ -1.0f, -1.0f,  1.0f },
		{ -1.0f, -1.0f, -1.0f },
		{  1.0f, -1.0f, -1.0f },

		{  1.0f, -1.0f, -1.0f },
		{  1.0f, -1.0f,  1.0f },
		{ -1.0f, -1.0f,  1.0f },
		// left
		{ -1.0f,  1.0f,  1.0f },
		{ -1.0f,  1.0f, -1.0f },
		{ -1.0f, -1.0f, -1.0f },

		{ -1.0f, -1.0f, -1.0f },
		{ -1.0f, -1.0f,  1.0f },
		{ -1.0f,  1.0f,  1.0f },
		// right
		{  1.0f,  1.0f,  1.0f },
		{  1.0f,  1.0f, -1.0f },
		{  1.0f, -1.0f, -1.0f },

		{  1.0f, -1.0f, -1.0f },
		{  1.0f, -1.0f,  1.0f },
		{  1.0f,  1.0f,  1.0f },
	};

	FloatingCubesRenderer::FloatingCubesRenderer()
		: m_instanceCount{ 0 }
	{
		// generate buffers and textures
		gl::GenVertexArrays(1, &m_vao);
		gl::GenBuffers(1, &m_positionsVBO);
		gl::GenBuffers(1, &m_instancePositionsVBO);
		gl::GenTextures(1, &m_positionBufferTexture);

		// set up vao
		gl::BindVertexArray(m_vao);
		{
			// "base" positions without instance offsets
			gl::BindBuffer(gl::ARRAY_BUFFER, m_positionsVBO);
			gl::BufferData(gl::ARRAY_BUFFER, sizeof(glm::vec3) * CUBE_POSITIONS.size(), CUBE_POSITIONS.data(), gl::STATIC_DRAW);
			gl::EnableVertexAttribArray(m_shader["position"]);
			gl::VertexAttribPointer(m_shader["position"], 3, gl::FLOAT, gl::TRIANGLE_FAN, 0, nullptr);
		}
		gl::BindVertexArray(0);
	}

	FloatingCubesRenderer::~FloatingCubesRenderer()
	{
		gl::DeleteTextures(1, &m_positionBufferTexture);
		gl::DeleteBuffers(1, &m_instancePositionsVBO);
		gl::DeleteBuffers(1, &m_positionsVBO);
		gl::DeleteVertexArrays(1, &m_vao);
	}

	void FloatingCubesRenderer::update(const FloatingCubes & cubes)
	{

	}

	void FloatingCubesRenderer::render()
	{
		gl::BindVertexArray(m_vao);
		m_shader.use();
		{
			// TODO: Update uniforms
			// TODO: Bind texture buffer

			gl::DrawArraysInstanced(gl::TRIANGLES, 0, CUBE_POSITIONS.size(), m_instanceCount);
		}
		m_shader.unuse();
		gl::BindVertexArray(0);
	}
}
