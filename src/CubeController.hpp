#pragma once

#include <vector>

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Spiral.hpp"
#include "GameTime.hpp"

namespace cubedemo
{
    // The states a cube can be in:
    // FadeIn - Just spawned, fading it in
    // Moving - Moving around
    // FadeOut - Despawning, but still fading out
    // Dead - Ready to be destroyed
    enum class CubeState
    {
        Dead,
        FadeIn,
        Moving,
        FadeOut,
    };

    // Contains per-cube state for a collection of cubes
    // This is saved as a structure-of-arrays instead of the usual array-of-structs
    // to ease bulk data copying into GL buffers and for a slight (?) speed boost
    struct CubeStates
    {
        std::vector<CubeState> states; // The state each vector is in
        std::vector<HelixData> helices; // Helix data for each cube
        std::vector<glm::vec3> positions; // The center of each cube after applying any mapping and movement
        std::vector<glm::quat> rotations; // The rotation of each cube
        std::vector<float> opacities; // The opacity of each cube, used for fade in and fade out
        std::vector<float> scales; // Adjusts the size of each cube
        std::vector<float> startTimes; // Start time for helix curve mapping (in seconds)

        CubeStates(size_t size);
    };

    // Collects the state of a bunch of cubes, floating in space
    class CubeController
    {
    private:
        size_t m_cubeCount; // Amount of managed cubes
        CubeStates m_cubeStates; // Per-cube state

    public:
        CubeController(size_t count);

        inline size_t count() const { return m_cubeCount; }
        inline const CubeState* cubeStates() const { return m_cubeStates.states.data(); }
        inline const glm::vec3* cubePositions() const { return m_cubeStates.positions.data(); }
        inline const glm::quat* cubeRotations() const { return m_cubeStates.rotations.data(); }
        inline const float* cubeOpacities() const { return m_cubeStates.opacities.data(); }
        inline const float* cubeScales() const { return m_cubeStates.scales.data(); }

        void update(const GameTime& time); // Update the state of each cube
    };
}
