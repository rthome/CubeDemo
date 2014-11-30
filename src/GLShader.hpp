#pragma once

#include <list>
#include <string>
#include <unordered_map>
#include <initializer_list>

#include "gl_core_4_1.hpp"

namespace cubedemo
{
	class GLShader
	{
	private:
		GLuint m_program;
		std::list<GLuint> m_attachedShaders;
		std::unordered_map<std::string, GLuint> m_attributes;
		std::unordered_map<std::string, GLuint> m_uniforms;

	public:
		GLShader();
		~GLShader();

		inline GLuint program() const { return m_program; }

		void attachShaderFromSource(GLenum shaderType, const std::string& source);

		void link();
		void use() const;
		void unuse() const;

		void addAttribute(const std::string& attribName);
		void addUniform(const std::string& uniformName);
		void addAttributes(std::initializer_list<std::string> attributeNames);
		void addUniforms(std::initializer_list<std::string> uniformNames);

		GLuint operator[](const std::string& attribName) const;
		GLuint operator()(const std::string& uniformName) const;
	};
}
