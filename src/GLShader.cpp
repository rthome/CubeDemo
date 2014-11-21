#include "GLShader.hpp"

#include <memory>
#include <functional>

#include "Util.hpp"

inline void printInfoLog(GLuint handle,
	std::function<void(GLuint, GLenum, GLint*)> getiv,
	std::function<void(GLuint, GLint, GLsizei*, GLchar*)> getinfolog)
{
	GLint infoLogLength;
	getiv(handle, gl::INFO_LOG_LENGTH, &infoLogLength);
	auto infoLog = std::unique_ptr<GLchar[]>(new GLchar[infoLogLength]);
	getinfolog(handle, infoLogLength, nullptr, infoLog.get());
	LOG_ERROR("Error compiling shader " << handle << ". Info log: " << infoLog.get());
}

namespace cubedemo
{
	GLShader::GLShader()
	{
		LOG_INFO("Creating new Shader.");
	}

	GLShader::~GLShader()
	{
		LOG_INFO("Destroying Shader.");

		gl::DeleteProgram(m_program);
		GL_CHECK_ERRORS;
	}

	void GLShader::attachShaderFromSource(GLenum shaderType, const std::string& source)
	{
		LOG_INFO("Attaching shader (" << shaderType << ")");

		auto shader = gl::CreateShader(shaderType);
		auto tmpPtr = source.c_str();
		gl::ShaderSource(shader, 1, &tmpPtr, nullptr);
		gl::CompileShader(shader);
		GL_CHECK_ERRORS;

		GLint compiled = gl::FALSE_;
		gl::GetShaderiv(shader, gl::COMPILE_STATUS, &compiled);
		if (!compiled)
			printInfoLog(shader, gl::GetShaderiv, gl::GetShaderInfoLog);
		GL_CHECK_ERRORS;

		m_attachedShaders.push_back(shader);
	}

	void GLShader::link()
	{
		m_program = gl::CreateProgram();
		for (auto shader : m_attachedShaders)
			gl::AttachShader(m_program, shader);
		gl::LinkProgram(m_program);
		GL_CHECK_ERRORS;

		GLint compiled = gl::FALSE_;
		gl::GetProgramiv(m_program, gl::LINK_STATUS, &compiled);
		if (!compiled)
			printInfoLog(m_program, gl::GetProgramiv, gl::GetProgramInfoLog);
		GL_CHECK_ERRORS;

		for (auto shader : m_attachedShaders)
			gl::DeleteShader(shader);
		GL_CHECK_ERRORS;

		m_attachedShaders.clear();
	}

	void GLShader::use() const
	{
		gl::UseProgram(m_program);
	}

	void GLShader::unuse() const
	{
		gl::UseProgram(0);
	}

	void GLShader::addAttribute(const std::string& attribName)
	{
		m_attributes[attribName] = gl::GetAttribLocation(m_program, attribName.c_str());
	}

	void GLShader::addUniform(const std::string& uniformName)
	{
		m_uniforms[uniformName] = gl::GetUniformLocation(m_program, uniformName.c_str());
	}

	void GLShader::addAttributes(std::initializer_list<std::string> attributeNames)
	{
		for (auto attrib : attributeNames)
			addAttribute(attrib);
	}

	void GLShader::addUniforms(std::initializer_list<std::string> uniformNames)
	{
		for (auto uniform : uniformNames)
			addUniform(uniform);
	}

	GLuint GLShader::operator[](const std::string& attribName) const
	{
		return m_attributes.at(attribName);
	}

	GLuint GLShader::operator()(const std::string& uniformName) const
	{
		return m_uniforms.at(uniformName);
	}
}
