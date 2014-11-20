#include "FloatingCubes.hpp"

#include <vector>
#include <algorithm>
#include <iterator>

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

    FloatingCubes::FloatingCubes(size_t count, const glm::vec4& cubeColor)
        : m_cubeCount{ count }, m_cubeColor{ cubeColor }, m_cubeStates{ new CubeState[count] }, m_cubePositions{ new glm::vec3[count] }, m_cubeRotations{ new glm::quat[count] }
	{
		for (size_t i = 0; i < m_cubeCount; i++)
			m_cubeStates[i] = CubeState::Dead;
	}

	FloatingCubes::~FloatingCubes()
	{
		delete[] m_cubeStates;
		delete[] m_cubePositions;
		delete[] m_cubeRotations;
	}

	void FloatingCubes::update(double time)
	{
		for (size_t i = 0; i < m_cubeCount; i++)
		{
			if (m_cubeStates[i] == CubeState::Dead)
			{
				m_cubePositions[i] = glm::vec3((float)i * 2, 0.0f, 0.0f);
				m_cubeStates[i] = CubeState::Moving;
			}

			m_cubePositions[i].y += (float)(0.00025 * time);

			if (m_cubePositions[i].y > 5.0f)
				m_cubePositions[i].y = 0.0f;
		}
	}

	// // //
	// FloatingCubesRenderer implementation
	// // //

	const std::vector<glm::vec3> CUBE_POSITIONS
	{
		{ -1.0f, -1.0f, -1.0f },
		{  1.0f, -1.0f, -1.0f },
		{ -1.0f,  1.0f, -1.0f },
		{  1.0f,  1.0f, -1.0f },
		{ -1.0f, -1.0f,  1.0f },
		{  1.0f, -1.0f,  1.0f },
		{ -1.0f,  1.0f,  1.0f },
		{  1.0f,  1.0f,  1.0f },
	};

	static const std::vector<glm::vec3> CUBE_NORMALS
	{
		glm::normalize(glm::vec3{ -1.0f, -1.0f, -1.0f }),
		glm::normalize(glm::vec3{ 1.0f, -1.0f, -1.0f }),
		glm::normalize(glm::vec3{ -1.0f,  1.0f, -1.0f }),
		glm::normalize(glm::vec3{ 1.0f,  1.0f, -1.0f }),
		glm::normalize(glm::vec3{ -1.0f, -1.0f,  1.0f }),
		glm::normalize(glm::vec3{ 1.0f, -1.0f,  1.0f }),
		glm::normalize(glm::vec3{ -1.0f,  1.0f,  1.0f }),
		glm::normalize(glm::vec3{ 1.0f,  1.0f,  1.0f }),
	};

	static const std::vector<unsigned int> CUBE_INDICES
	{
		// front
		0, 1, 3,
		0, 3, 2,
		// back
		5, 4, 6,
		5, 6, 7,
		// top
		2, 3, 7,
		2, 7, 6,
		// bottom
		4, 5, 1,
		4, 1, 0,
		// left
		4, 0, 2,
		4, 2, 6,
		// right
		1, 5, 7,
		1, 7, 3,
	};

	FloatingCubesRenderer::FloatingCubesRenderer()
		: m_instanceCount{ 0 }
	{
		// generate buffers and textures
		gl::GenVertexArrays( 1.0f, &m_vao);
		gl::GenBuffers(1, &m_positionsVBO);
		gl::GenBuffers(1, &m_normalsVBO);
		gl::GenBuffers(1, &m_indices);
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
        m_shader.addUniform("CubeColor");
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
            //gl::BindBuffer(gl::ARRAY_BUFFER, m_normalsVBO);
            //gl::BufferData(gl::ARRAY_BUFFER, sizeof(glm::vec3) * CUBE_NORMALS.size(), CUBE_NORMALS.data(), gl::STATIC_DRAW);
            //gl::EnableVertexAttribArray(m_shader["normal"]);
            //gl::VertexAttribPointer(m_shader["normal"], 3, gl::FLOAT, gl::FALSE_, 0, nullptr);
            //GL_CHECK_ERRORS;

			// indices
			gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, m_indices);
			gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * CUBE_INDICES.size(), CUBE_INDICES.data(), gl::STATIC_DRAW);
		}
		gl::BindVertexArray(0);
	}

	FloatingCubesRenderer::~FloatingCubesRenderer()
	{
		gl::DeleteTextures(1, &m_positionBufferTexture);
		gl::DeleteBuffers(1, &m_instancePositionsVBO);
		gl::DeleteBuffers(1, &m_indices);
		gl::DeleteBuffers(1, &m_normalsVBO);
		gl::DeleteBuffers(1, &m_positionsVBO);
		gl::DeleteVertexArrays(1, &m_vao);
		GL_CHECK_ERRORS;
	}

	void FloatingCubesRenderer::update(const FloatingCubes & cubes)
	{
		m_instanceCount = cubes.count();
		std::vector<glm::vec4> processedPositions;

		auto source = cubes.cubePositions();
		std::transform(source, source + cubes.count(), std::back_inserter(processedPositions),
			[](glm::vec3& v) { return glm::vec4(v, 0.0f); });

		gl::BindBuffer(gl::TEXTURE_BUFFER, m_instancePositionsVBO);
		{
			gl::BufferData(gl::TEXTURE_BUFFER, sizeof(glm::vec4) * processedPositions.size(), processedPositions.data(), gl::STREAM_DRAW);
		}
		gl::BindBuffer(gl::TEXTURE_BUFFER, 0);
        GL_CHECK_ERRORS;
        
        // set cube color
        m_shader.use();
        gl::Uniform4fv(m_shader("CubeColor"), 1, glm::value_ptr(cubes.cubeColor()));
        m_shader.unuse();
        GL_CHECK_ERRORS;
	}

	void FloatingCubesRenderer::render()
	{
		auto mvp = glm::perspective(glm::quarter_pi<float>(), 1280.0f / 720.0f, 1.0f, 1000.0f)
			* glm::lookAt(glm::vec3(0.0f, 3.0f, 20.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

		gl::BindVertexArray(m_vao);
		m_shader.use();
		{
			GL_CHECK_ERRORS;
			gl::ActiveTexture(gl::TEXTURE0);
			gl::BindTexture(gl::TEXTURE_BUFFER, m_positionBufferTexture);
			gl::TexBuffer(gl::TEXTURE_BUFFER, gl::RGBA32F, m_instancePositionsVBO);
			gl::Uniform1i(m_shader("InstancePositions"), 0);
			gl::UniformMatrix4fv(m_shader("MVP"), 1, gl::FALSE_, glm::value_ptr(mvp));

			gl::DrawElementsInstanced(gl::TRIANGLES, CUBE_INDICES.size(), gl::UNSIGNED_INT, nullptr, m_instanceCount);
			GL_CHECK_ERRORS;
		}
		m_shader.unuse();
		gl::BindVertexArray(0);
	}
}
