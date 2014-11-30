#pragma once

#include "gl_core_4_1.hpp"
#include "NonCopyable.hpp"

namespace cubedemo
{
    class GLTextureBuffer : private NonCopyable
    {
    private:
        GLuint m_textureID;
        GLuint m_bufferID;
        GLenum m_textureFormat;

    public:
        GLTextureBuffer(GLenum textureFormat); // textureFormat: GL_R32F, GL_RGB8I, etc
        ~GLTextureBuffer();
        
        inline GLuint texture() const { return m_textureID; }
        inline GLuint buffer() const { return m_bufferID; }

        // Bind the texture to the given texture unit and update the corresponding uniform
        void bind(GLuint texUnitIndex, GLuint uniformLocation) const;

        // Copy a given number of bytes into the texture buffer, with an optional gl usage hint
        void updateData(size_t count, const void *data, GLenum usageHint = gl::DYNAMIC_DRAW);
    };
}
