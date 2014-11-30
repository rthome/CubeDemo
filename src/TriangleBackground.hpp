#pragma once

#include "gl_core_4_1.hpp"
#include "GLShader.hpp"
#include "GameTime.hpp"
#include "NonCopyable.hpp"

namespace cubedemo
{
    class TriangleBackground : private NonCopyable
    {
    public:
        const float GAMMA = 2.2f;

    private:
        GLuint m_vao;
        GLuint m_positionsVBO;
        GLuint m_indices;
        GLuint m_brightnessVBO;
        GLShader m_shader;
        
        GLsizei m_elementCount; // Count of indices to draw
        size_t m_hcount, m_vcount; // Amount of triangles in horizontal and vertical directions
        
    public:
        TriangleBackground(size_t hcount, size_t vcount);
        ~TriangleBackground();
        
        void update(const GameTime& time);
        void render(const GameTime& time);
    };
}
