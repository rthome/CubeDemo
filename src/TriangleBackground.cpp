#include "TriangleBackground.hpp"

#include <vector>
#include <random>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>

#include "ShaderSources.hpp"
#include "Util.hpp"

namespace cubedemo
{
    std::vector<glm::vec3> generateTriangleMeshPositions(size_t width, size_t height)
	{
		static const float Z = 1.0f;

		std::vector<glm::vec3> positions;
		positions.reserve(width * height);

		for (size_t y = 0; y < height; y++)
		{
			for (size_t x = 0; x < width; x++)
			{
				positions.emplace_back(x, y, Z);
			}
		}

		return positions;
	}

	std::vector<unsigned int> generateTriangleMeshIndices(size_t width, size_t height)
	{
		auto index = [&](size_t column, size_t row) { return column + row * width; };

		std::vector<unsigned int> indices;
		for (size_t y = 0; y < height - 1; y++)
		{
			for (size_t x = 0; x < width - 1; x++)
			{
				indices.push_back((unsigned int)index(x, y)); // upper left
				indices.push_back((unsigned int)index(x, y + 1)); // lower left
				indices.push_back((unsigned int)index(x + 1, y)); // upper right

				indices.push_back((unsigned int)index(x + 1, y)); // upper right
				indices.push_back((unsigned int)index(x, y + 1)); // lower left
				indices.push_back((unsigned int)index(x + 1, y + 1)); // lower right
			}
		}

		return indices;
	}

	TriangleBackground::TriangleBackground(size_t hcount, size_t vcount)
		: m_hcount{ hcount }, m_vcount{ vcount }
	{
		// Generate buffers etc
		gl::GenVertexArrays(1, &m_vao);
		gl::GenBuffers(1, &m_positionsVBO);
		gl::GenBuffers(1, &m_indices);
		gl::GenBuffers(1, &m_brightnessVBO);
		GL_CHECK_ERRORS;

		// Set up shader
		m_shader.attachShaderFromSource(gl::VERTEX_SHADER, shaderSourceBackgroundVert());
		m_shader.attachShaderFromSource(gl::FRAGMENT_SHADER, shaderSourceBackgroundFrag());
		m_shader.link();
		m_shader.addAttributes({ "position", "brightness" });
		m_shader.addUniforms({ "BaseColor", "MVP", "Gamma" });
		GL_CHECK_ERRORS;

		// TODO: Regenerate positions and indices when window size changes
		// Preserve ratio of vertical to horizontal triangles

		// Generate triangle mesh data
		auto positions = generateTriangleMeshPositions(hcount, vcount);
		auto indices = generateTriangleMeshIndices(hcount, vcount);
		m_elementCount = (GLsizei)indices.size();

		// Set up VAO and buffer data
		gl::BindVertexArray(m_vao);
		{
			// Position data
			gl::BindBuffer(gl::ARRAY_BUFFER, m_positionsVBO);
			gl::BufferData(gl::ARRAY_BUFFER, sizeof(glm::vec3) * positions.size(), positions.data(), gl::STATIC_DRAW);
			gl::EnableVertexAttribArray(m_shader["position"]);
			gl::VertexAttribPointer(m_shader["position"], 3, gl::FLOAT, gl::FALSE_, 0, nullptr);
			GL_CHECK_ERRORS;

			// Brightness
			// No data yet, will be copied in update()
			gl::BindBuffer(gl::ARRAY_BUFFER, m_brightnessVBO);
			gl::BufferData(gl::ARRAY_BUFFER, sizeof(float) * m_hcount * m_vcount, nullptr, gl::STREAM_DRAW);
			gl::EnableVertexAttribArray(m_shader["brightness"]);
			gl::VertexAttribPointer(m_shader["brightness"], 1, gl::FLOAT, gl::FALSE_, 0, nullptr);
			GL_CHECK_ERRORS;

			// Indices
			gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, m_indices);
			gl::BufferData(gl::ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), indices.data(), gl::STATIC_DRAW);
			GL_CHECK_ERRORS;
		}
		gl::BindVertexArray(0);
	}

	TriangleBackground::~TriangleBackground()
	{
		gl::DeleteBuffers(1, &m_brightnessVBO);
		gl::DeleteBuffers(1, &m_indices);
		gl::DeleteBuffers(1, &m_positionsVBO);
		gl::DeleteVertexArrays(1, &m_vao);
		GL_CHECK_ERRORS;
	}

	void TriangleBackground::update(const GameTimePoint& time)
	{
		std::vector<float> brightnessData;
		for (size_t y = 0; y < m_vcount; y++)
		{
			for (size_t x = 0; x < m_hcount; x++)
			{
				auto noise = 0.5f * (1 + glm::simplex(glm::vec3{ (float)x, (float)y, time.total() * 0.30f }));
				brightnessData.push_back(noise);
			}
		}

		// Copy data into brightness VBO
		gl::BindBuffer(gl::ARRAY_BUFFER, m_brightnessVBO);
		{
			gl::BufferSubData(gl::ARRAY_BUFFER, 0, sizeof(float) * brightnessData.size(), brightnessData.data());
		}
		gl::BindBuffer(gl::ARRAY_BUFFER, 0);
		GL_CHECK_ERRORS;
	}

	void TriangleBackground::render(const GameTimePoint& time)
	{
		glm::vec4 baseColor{ 1.0f, 0.761f, 0.0f, 1.0f };
		glm::mat4 mvp = glm::ortho(0.0f, (float)m_hcount - 1, 0.0f, (float)m_vcount - 1);

		gl::BindVertexArray(m_vao);
		m_shader.use();
		{
			gl::Uniform1f(m_shader("Gamma"), GAMMA);
			gl::Uniform4fv(m_shader("BaseColor"), 1, glm::value_ptr(baseColor));
			gl::UniformMatrix4fv(m_shader("MVP"), 1, gl::FALSE_, glm::value_ptr(mvp));
			GL_CHECK_ERRORS;

			gl::DrawElements(gl::TRIANGLES, m_elementCount, gl::UNSIGNED_INT, nullptr);
			GL_CHECK_ERRORS;
		}
		m_shader.unuse();
		gl::BindVertexArray(0);
	}
}
