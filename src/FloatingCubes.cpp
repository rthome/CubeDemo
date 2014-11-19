#include "FloatingCubes.hpp"

#include <vector>

#include <glm/gtc/constants.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Util.hpp"
#include "Shaders.hpp"

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

	static const std::vector<glm::vec3> CUBE_NORMALS
	{
		// front
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		{ 0.0f, 0.0f, 1.0f },
		// back
		{ 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f, -1.0f },
		{ 0.0f, 0.0f, -1.0f },
		// top
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		{ 0.0f, 1.0f, 0.0f },
		// bottom
		{ 0.0f, -1.0f, 0.0f },
		{ 0.0f, -1.0f, 0.0f },
		{ 0.0f, -1.0f, 0.0f },
		{ 0.0f, -1.0f, 0.0f },
		{ 0.0f, -1.0f, 0.0f },
		{ 0.0f, -1.0f, 0.0f },
		// left
		{ -1.0f, 0.0f, 0.0f },
		{ -1.0f, 0.0f, 0.0f },
		{ -1.0f, 0.0f, 0.0f },
		{ -1.0f, 0.0f, 0.0f },
		{ -1.0f, 0.0f, 0.0f },
		{ -1.0f, 0.0f, 0.0f },
		// right
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
		{ 1.0f, 0.0f, 0.0f },
	};

	FloatingCubesRenderer::FloatingCubesRenderer()
		: m_instanceCount{ 0 }
	{
		// generate buffers and textures
		gl::GenVertexArrays(1, &m_vao);
		gl::GenBuffers(1, &m_positionsVBO);
		gl::GenBuffers(1, &m_normalsVBO);
		gl::GenBuffers(1, &m_instancePositionsVBO);
		gl::GenTextures(1, &m_positionBufferTexture);
		GL_CHECK_ERRORS;

		// set up shader
		m_shader.attachShaderFromSource(gl::VERTEX_SHADER, CUBE_SHADER_VERTEX);
		m_shader.attachShaderFromSource(gl::FRAGMENT_SHADER, CUBE_SHADER_FRAGMENT);
		m_shader.link();
		m_shader.addAttribute("position");
		m_shader.addAttribute("normal");
		m_shader.addUniform("MVP");
		m_shader.addUniform("InstancePositions");
		GL_CHECK_ERRORS;

		// set up vao
		gl::BindVertexArray(m_vao);
		{
			// "base" positions without instance offsets
			gl::BindBuffer(gl::ARRAY_BUFFER, m_positionsVBO);
			gl::BufferData(gl::ARRAY_BUFFER, sizeof(glm::vec3) * CUBE_POSITIONS.size(), CUBE_POSITIONS.data(), gl::STATIC_DRAW);
			gl::EnableVertexAttribArray(m_shader["position"]);
			gl::VertexAttribPointer(m_shader["position"], 3, gl::FLOAT, gl::FALSE_, 0, nullptr);
			GL_CHECK_ERRORS;

			// normals
			gl::BindBuffer(gl::ARRAY_BUFFER, m_normalsVBO);
			gl::BufferData(gl::ARRAY_BUFFER, sizeof(glm::vec3) * CUBE_NORMALS.size(), CUBE_NORMALS.data(), gl::STATIC_DRAW);
			gl::EnableVertexAttribArray(m_shader["normal"]);
			gl::VertexAttribPointer(m_shader["normal"], 3, gl::FLOAT, gl::FALSE_, 0, nullptr);
			GL_CHECK_ERRORS;
		}
		gl::BindVertexArray(0);
		
		// set up texture buffer data for instance offsets
		gl::BindBuffer(gl::TEXTURE_BUFFER, m_instancePositionsVBO);
		{
			glm::vec3 offset{ 0.0f };
			gl::BufferData(gl::TEXTURE_BUFFER, sizeof(glm::vec3), glm::value_ptr(offset), gl::STREAM_DRAW);
		}
		gl::BindBuffer(gl::TEXTURE_BUFFER, 0);
		GL_CHECK_ERRORS;
	}

	FloatingCubesRenderer::~FloatingCubesRenderer()
	{
		GL_CHECK_ERRORS;
		gl::DeleteTextures(1, &m_positionBufferTexture);
		gl::DeleteBuffers(1, &m_instancePositionsVBO);
		gl::DeleteBuffers(1, &m_normalsVBO);
		gl::DeleteBuffers(1, &m_positionsVBO);
		gl::DeleteVertexArrays(1, &m_vao);
		GL_CHECK_ERRORS;
	}

	void FloatingCubesRenderer::update(const FloatingCubes & cubes)
	{
		// TODO: update texture buffer data
		m_instanceCount = cubes.count();
	}

	void FloatingCubesRenderer::render()
	{
		auto mvp = glm::perspective(glm::quarter_pi<float>(), 1280.0f / 720.0f, 1.0f, 1000.0f)
			* glm::lookAt(glm::vec3(10.0f, 3.0f, 10.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		gl::BindVertexArray(m_vao);
		m_shader.use();
		{
			GL_CHECK_ERRORS;
			gl::ActiveTexture(gl::TEXTURE0);
			gl::BindTexture(gl::TEXTURE_BUFFER, m_positionBufferTexture);
			gl::TexBuffer(gl::TEXTURE_BUFFER, gl::RGBA32F, m_instancePositionsVBO);
			gl::Uniform1i(m_shader("InstancePositions"), 0);
			gl::UniformMatrix4fv(m_shader("MVP"), 1, gl::FALSE_, glm::value_ptr(mvp));

			gl::DrawArraysInstanced(gl::TRIANGLES, 0, CUBE_POSITIONS.size(), m_instanceCount);
			GL_CHECK_ERRORS;
		}
		m_shader.unuse();
		gl::BindVertexArray(0);
	}
}
