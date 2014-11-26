#include "TriangleBackground.hpp"

namespace cubedemo
{
    TriangleBackground::TriangleBackground(size_t hcount, size_t vcount)
        : m_hcount{ hcount }, m_vcount{ vcount }
    {
        gl::GenVertexArrays(1, &m_vao);
    }
    
    TriangleBackground::~TriangleBackground()
    {
        gl::DeleteVertexArrays(1, &m_vao);
    }
    
    void TriangleBackground::update(const GameTime& time)
    {
        
    }
    
    void TriangleBackground::render()
    {
        
    }
}
