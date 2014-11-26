#include "Spiral.hpp"

#include <cmath>

#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
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
        
        auto rotationQuat = glm::rotation(glm::normalize(curvePos - helix.position), glm::normalize(helix.direction));
        auto rotatedCurvePos = glm::rotate(rotationQuat, curvePos);
        
        return helix.position + rotatedCurvePos;
    }
}
