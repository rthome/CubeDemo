#include "CubeController.hpp"

#include <cmath>
#include <random>

#include <glm/geometric.hpp>
#include "Util.hpp"

namespace cubedemo
{
    // // //
    // CubeStates implementation
    // // //

    CubeStates::CubeStates(int size)
    {
        states.resize(size);
        helices.resize(size);
        positions.resize(size);
        rotationAxes.resize(size);
        rotationSpeeds.resize(size);
        opacities.resize(size);
        scales.resize(size);
        startTimes.resize(size);
    }

    // // //
    // CubeRenderer implementation
    // // //

    static float deltaOpacity(float seconds, const GameTimePoint& time)
    {
        return (1.0f / seconds) * 0.001f * time.delta();
    }

    static int aliveCubesForTime(const GameTimePoint& time, int maxCubes)
    {
	    auto cubes = 2.0f * time.total();
        if (time.total() > 8.0f)
            cubes *= 1 + (time.total() - 8.0f);
        return int(fmin(maxCubes, cubes));
    }

    CubeController::CubeController(int count)
        : m_cubeCount{ count }, m_aliveCubes{ 0 }, m_cubeStates{ count }
    {
        CC_ASSERT(count > 0)
    }

    void CubeController::update(const GameTimePoint& time)
    {
        // TODO: Change random number generation to be less shitty
        static std::default_random_engine randEngine;
        static std::uniform_real_distribution<float> startRandDistrib{ -1.0f, 1.0f };
        static std::normal_distribution<float> movementRandDistrib{ 10.0f, 2.0f };
        static std::normal_distribution<float> scaleRandDistrib{ 1.0f, 0.20f };

	    auto aliveCubesThisFrame = aliveCubesForTime(time, m_cubeCount);

        for (size_t i = 0; i < m_cubeCount; i++)
        {
            if (m_cubeStates.states[i] == CubeState::Dead)
            {
                if (m_aliveCubes < aliveCubesThisFrame)
                {
                    // When a cube spawns, fill in a bunch of random data
                    m_cubeStates.states[i] = CubeState::FadeIn;
                    m_cubeStates.scales[i] = scaleRandDistrib(randEngine);
                    m_cubeStates.rotationAxes[i] = glm::normalize(glm::vec3{ startRandDistrib(randEngine), startRandDistrib(randEngine), startRandDistrib(randEngine) });
                    m_cubeStates.rotationSpeeds[i] = 0.8f * scaleRandDistrib(randEngine) * (std::signbit(startRandDistrib(randEngine)) ? 1.0f : -1.0f);
                    m_cubeStates.startTimes[i] = time.total(); // save in seconds
                    m_cubeStates.helices[i].t0 = movementRandDistrib(randEngine);
                    m_cubeStates.helices[i].position = glm::vec3(startRandDistrib(randEngine) * 125, 70, 150 + startRandDistrib(randEngine) * 100);
                    m_cubeStates.helices[i].r = 2 * movementRandDistrib(randEngine) * (std::signbit(startRandDistrib(randEngine)) ? 1.0f : -1.0f);
                    m_cubeStates.helices[i].h = -7 * movementRandDistrib(randEngine);
                    m_aliveCubes++;
                }
            }

            if (m_cubeStates.states[i] == CubeState::FadeIn)
            {
                // Fade in over 3 seconds
                m_cubeStates.opacities[i] += deltaOpacity(3.0f, time);
                // Once the opacity reaches 1, stop fading in
                if (m_cubeStates.opacities[i] >= 1.0f)
                {
                    m_cubeStates.opacities[i] = 1.0f;
                    m_cubeStates.states[i] = CubeState::Moving;
                }
            }

            if (m_cubeStates.states[i] == CubeState::FadeOut)
            {
                // Fade out over 3 seconds
                m_cubeStates.opacities[i] -= deltaOpacity(3.0f, time);
                // If opacity reaches 0, kill the cube
                if (m_cubeStates.opacities[i] <= 0.0f)
                {
                    m_cubeStates.opacities[i] = 0.0f;
                    m_cubeStates.states[i] = CubeState::Dead;
                    m_aliveCubes--;
                }
            }

            if (m_cubeStates.states[i] != CubeState::Dead)
                m_cubeStates.positions[i] = mapOntoHelix(m_cubeStates.helices[i], 0.1f * (time.total() - m_cubeStates.startTimes[i]));

            if (m_cubeStates.states[i] == CubeState::Moving && m_cubeStates.positions[i].y < -70.0f)
                m_cubeStates.states[i] = CubeState::FadeOut;
        }
    }
}
