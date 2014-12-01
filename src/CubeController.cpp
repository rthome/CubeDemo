#include "CubeController.hpp"

#include <random>

#include <glm/geometric.hpp>

namespace cubedemo
{
    // // //
    // CubeStates implementation
    // // //

    CubeStates::CubeStates(size_t size)
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

    inline float deltaOpacity(float seconds, const GameTime& time)
    {
        return (1.0f / seconds) * 0.001f * time.deltaTime.count();
    }

    CubeController::CubeController(size_t count)
        : m_cubeCount{ count }, m_cubeStates{ count }
    {
    }

    void CubeController::update(const GameTime& time)
    {
        // TODO: Change random number generation to be less shitty
        static std::default_random_engine randEngine;
        static std::uniform_real_distribution<float> startRandDistrib{ -1.0f, 1.0f };
        static std::normal_distribution<float> movementRandDistrib{ 10.0f, 2.0f };
        static std::normal_distribution<float> scaleRandDistrib{ 1.0f, 0.20f };

        static const int MAX_SPAWNS_PER_FRAME = 3;

        int spawnedCubes = 0;
        for (size_t i = 0; i < m_cubeCount; i++)
        {
            if (m_cubeStates.states[i] == CubeState::Dead)
            {
                if (spawnedCubes++ < MAX_SPAWNS_PER_FRAME)
                {
                    // When a cube spawns, fill in a bunch of random data
                    m_cubeStates.states[i] = CubeState::FadeIn;
                    m_cubeStates.scales[i] = scaleRandDistrib(randEngine);
                    m_cubeStates.rotationAxes[i] = glm::normalize(glm::vec3{ startRandDistrib(randEngine), startRandDistrib(randEngine), startRandDistrib(randEngine) });
                    m_cubeStates.rotationSpeeds[i] = 0.8f * scaleRandDistrib(randEngine) * (signbit(startRandDistrib(randEngine)) ? 1.0f : -1.0f);
                    m_cubeStates.startTimes[i] = time.totalTime.count() / 1000.0f; // save in seconds
                    m_cubeStates.helices[i].t0 = movementRandDistrib(randEngine);
                    m_cubeStates.helices[i].position = glm::vec3(startRandDistrib(randEngine) * 125, 70, 150 + startRandDistrib(randEngine) * 100);
                    m_cubeStates.helices[i].r = 2 * movementRandDistrib(randEngine) * (signbit(startRandDistrib(randEngine)) ? 1.0f : -1.0f);
                    m_cubeStates.helices[i].h = -7 * movementRandDistrib(randEngine);
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
                }
            }

            if (m_cubeStates.states[i] != CubeState::Dead)
                m_cubeStates.positions[i] = mapOntoHelix(m_cubeStates.helices[i], 0.1f * ((time.totalTime.count() / 1000.0f) - m_cubeStates.startTimes[i]));

            if (m_cubeStates.states[i] == CubeState::Moving && m_cubeStates.positions[i].y < -70.0f)
                m_cubeStates.states[i] = CubeState::FadeOut;
        }
    }
}
