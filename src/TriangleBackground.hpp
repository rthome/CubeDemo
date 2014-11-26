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
        
    public:
        void update(const GameTime& time);
        void render();
    };
}
