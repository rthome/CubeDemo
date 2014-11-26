#pragma once

#include "gl_core_3_3.hpp"
#include "GLShader.hpp"
#include "GameTime.hpp"

namespace cubedemo
{
    class TriangleBackground final
    {
    private:
        GLuint m_vao;
        GLShader m_shader;
        
        size_t m_hcount, m_vcount;
        
    public:
        TriangleBackground(size_t hcount, size_t vcount);
        ~TriangleBackground();
        
        void update(const GameTime& time);
        void render();
    };
}
