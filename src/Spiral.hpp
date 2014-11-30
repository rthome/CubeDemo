#pragma once

#include <glm/vec3.hpp>

namespace cubedemo
{
    // Contains the parameters of a helix curve
    struct HelixData final
    {
        float r; // Radius
        float h; // Distance the helix "travels" over the course of a revolution
        float t0; // Initial Offset
        glm::vec3 position; // The origin (position at t=0) of the helix curve
    };
    
    // Maps a given position onto a helix
    //   pos: A three-dimensional coordinate
    //   helix: A HelixData object
    //   t: Elapsed time in seconds
    glm::vec3 mapOntoHelix(const HelixData& helix, float t);
}
