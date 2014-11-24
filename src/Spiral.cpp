#include "Spiral.hpp"

#include <cmath>

#include <glm/gtc/constants.hpp>

namespace cubedemo
{
    static const float TWO_PI = glm::pi<float>() * 2.0f;
    
    glm::vec3 mapOntoHelix(const HelixData& helix, float t)
    {
        auto x = helix.r * cos(t * TWO_PI + helix.t0);
        auto z = helix.r * sin(t * TWO_PI + helix.t0);
        auto y = helix.h * t;
        auto curvePos = glm::vec3 { x, y, z };
		return helix.position + curvePos;
    }
}
