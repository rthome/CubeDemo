#include "GLTextureBuffer.hpp"

#include "Util.hpp"

namespace cubedemo
{
    GLTextureBuffer::GLTextureBuffer(GLenum textureFormat)
        : m_textureFormat{ textureFormat }
    {
        gl::GenTextures(1, &m_textureID);
        gl::GenBuffers(1, &m_bufferID);
    }

    GLTextureBuffer::~GLTextureBuffer()
    {
        gl::DeleteTextures(1, &m_textureID);
        gl::DeleteBuffers(1, &m_bufferID);
    }

    void GLTextureBuffer::bind(unsigned int texUnitIndex, unsigned int uniformLocation) const
    {
        gl::ActiveTexture(gl::TEXTURE0 + texUnitIndex);
        gl::BindTexture(gl::TEXTURE_BUFFER, m_textureID);
        gl::TexBuffer(gl::TEXTURE_BUFFER, m_textureFormat, m_bufferID);
        gl::Uniform1i(uniformLocation, texUnitIndex);
        GL_CHECK_ERRORS;
    }

    void GLTextureBuffer::updateData(size_t count, const void *data, GLenum usageHint)
    {
        gl::BindBuffer(gl::TEXTURE_BUFFER, m_bufferID);
        {
            gl::BufferData(gl::TEXTURE_BUFFER, count, data, usageHint);
        }
        gl::BindBuffer(gl::TEXTURE_BUFFER, 0);
        GL_CHECK_ERRORS;
    }
}
