#include "FloatingCubes.hpp"

#include <vector>
#include <algorithm>
#include <iterator>
#include <random>

#include <glm/gtc/constants.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/noise.hpp>

#include "Util.hpp"
#include "Shaders.hpp"
#include "Spiral.hpp"

namespace cubedemo
{
	// // //
	// CubeStates implementation
	// // //

	CubeStates::CubeStates(size_t size)
		: states{ new CubeState[size] }, helices{ new HelixData[size] }, positions{ new glm::vec3[size] }, rotations{ new glm::quat[size] }
	{
		std::fill(states, states + size, CubeState::Dead);
	}

	CubeStates::~CubeStates()
	{
		delete[] states;
		delete[] helices;
		delete[] positions;
		delete[] rotations;
	}

	// // //
	// FloatingCubes implementation
	// // //

	FloatingCubes::FloatingCubes(size_t count)
		: m_cubeCount{ count }, m_cubeStates{ count }
	{
	}

	void FloatingCubes::update(const GameTime& time)
	{
		static std::default_random_engine randEngine;
		static std::uniform_real_distribution<float> startRandDistrib{ -300.0f, 300.0f };
        static std::uniform_real_distribution<float> directionRandDistrib { -1.0f, 1.0f };
        static std::normal_distribution<float> movementRandDistrib { 1.0f, 2.0f };

		for (size_t i = 0; i < m_cubeCount; i++)
		{
			if (m_cubeStates.states[i] == CubeState::Dead)
			{
				m_cubeStates.states[i] = CubeState::Moving;
                m_cubeStates.helices[i].r = 2 + 10.0f * abs(movementRandDistrib(randEngine));
				m_cubeStates.helices[i].h = 0.5f + 2.0f * movementRandDistrib(randEngine);
                m_cubeStates.helices[i].t0 = movementRandDistrib(randEngine);
				m_cubeStates.helices[i].position = glm::vec3(startRandDistrib(randEngine), startRandDistrib(randEngine), startRandDistrib(randEngine));
                m_cubeStates.helices[i].direction = glm::vec3 { directionRandDistrib(randEngine), directionRandDistrib(randEngine), directionRandDistrib(randEngine) };
			}

			m_cubeStates.positions[i] = mapOntoHelix(m_cubeStates.helices[i], 0.0001f * (float)time.totalTime.count());

			if (glm::length(m_cubeStates.positions[i] - glm::vec3{ 0.0f }) > 600.0f)
				m_cubeStates.states[i] = CubeState::Dead;
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

	static const std::vector<uint8_t> CUBE_INDICES
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
		: m_instanceCount{ 0 }, m_modelviewMatrix{ glm::lookAt(glm::vec3(0.0f, 3.0f, 20.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) }
	{
		// generate buffers and textures
		gl::GenVertexArrays(1, &m_vao);
		gl::GenBuffers(1, &m_positionsVBO);
		gl::GenBuffers(1, &m_normalsVBO);
		gl::GenBuffers(1, &m_indices);
		gl::GenBuffers(1, &m_instancePositionsVBO);
		gl::GenTextures(1, &m_positionBufferTexture);
		GL_CHECK_ERRORS;

		// set up shader
		m_shader.attachShaderFromSource(gl::VERTEX_SHADER, CUBE_SHADER_CUBES_VERTEX);
		m_shader.attachShaderFromSource(gl::FRAGMENT_SHADER, CUBE_SHADER_CUBES_FRAGMENT);
		m_shader.link();
		m_shader.addAttributes({ "position", "normal" });
		m_shader.addUniforms({ "MVP", "InstancePositions" , "ModelViewMatrix", "ProjectionMatrix", "NormalMatrix", "LightPosition", "LightIntensity", "Kd", "Ka", "Ks", "Shininess", "Gamma" });
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

	void cubedemo::FloatingCubesRenderer::onWindowSizeChanged(size_t width, size_t height)
	{
		m_projectionMatrix = glm::perspective(glm::quarter_pi<float>(), (float)width / height, 0.1f, 1000.0f);
	}

	void FloatingCubesRenderer::update(const FloatingCubes & cubes)
	{
		m_instanceCount = cubes.count();
		std::vector<glm::vec4> processedPositions;

		auto source = cubes.cubePositions();
		std::transform(source, source + cubes.count(), std::back_inserter(processedPositions),
			[](const glm::vec3& v) { return glm::vec4(v, 0.0f); });

		gl::BindBuffer(gl::TEXTURE_BUFFER, m_instancePositionsVBO);
		{
			gl::BufferData(gl::TEXTURE_BUFFER, sizeof(glm::vec4) * processedPositions.size(), processedPositions.data(), gl::STREAM_DRAW);
		}
		gl::BindBuffer(gl::TEXTURE_BUFFER, 0);
		GL_CHECK_ERRORS;
	}

	void FloatingCubesRenderer::render()
	{
		const float SHININESS = 10.0f;
		const glm::vec3 AMBIENT_COLOR{ 0.1f, 0.1f, 0.1f };
		const glm::vec3 DIFFUSE_COLOR{ 0.0f, 0.75f, 0.75f };
		const glm::vec3 SPECULAR_COLOR{ 0.5f, 0.5f, 0.5f };

		auto mvp = m_projectionMatrix * m_modelviewMatrix;
		auto normalMat = glm::inverseTranspose(glm::mat3(m_modelviewMatrix));

		glm::vec3 lightPos{ 0.0f };
		glm::vec3 lightIntensity{ 1.0f };

		gl::BindVertexArray(m_vao);
		m_shader.use();
		{
			gl::ActiveTexture(gl::TEXTURE0);
			gl::BindTexture(gl::TEXTURE_BUFFER, m_positionBufferTexture);
			gl::TexBuffer(gl::TEXTURE_BUFFER, gl::RGBA32F, m_instancePositionsVBO);

			gl::Uniform1i(m_shader("InstancePositions"), 0);
			gl::UniformMatrix4fv(m_shader("MVP"), 1, gl::FALSE_, glm::value_ptr(mvp));
			gl::UniformMatrix4fv(m_shader("ModelViewMatrix"), 1, gl::FALSE_, glm::value_ptr(m_modelviewMatrix));
			gl::UniformMatrix4fv(m_shader("ProjectionMatrix"), 1, gl::FALSE_, glm::value_ptr(m_projectionMatrix));
			gl::UniformMatrix3fv(m_shader("NormalMatrix"), 1, gl::FALSE_, glm::value_ptr(normalMat));
			gl::Uniform1f(m_shader("Shininess"), SHININESS);
			gl::Uniform3fv(m_shader("LightPosition"), 1, glm::value_ptr(lightPos));
			gl::Uniform3fv(m_shader("LightIntensity"), 1, glm::value_ptr(lightIntensity));
			gl::Uniform3fv(m_shader("Kd"), 1, glm::value_ptr(DIFFUSE_COLOR));
			gl::Uniform3fv(m_shader("Ka"), 1, glm::value_ptr(AMBIENT_COLOR));
			gl::Uniform3fv(m_shader("Ks"), 1, glm::value_ptr(SPECULAR_COLOR));
			gl::Uniform1f(m_shader("Gamma"), GAMMA);

			gl::DrawElementsInstanced(gl::TRIANGLES, (GLsizei)CUBE_INDICES.size(), gl::UNSIGNED_BYTE, nullptr, (GLsizei)m_instanceCount);
			GL_CHECK_ERRORS;
		}
		m_shader.unuse();
		gl::BindVertexArray(0);
	}
}
