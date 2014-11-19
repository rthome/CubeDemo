#pragma once

#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>

namespace cubedemo
{
	// The states a cube can be in:
	// Spawned - Just spawned, not yet fading in
	// FadeIn - Just spawned, fading it in
	// Moving - Moving around
	// FadeOut - Despawning, but still fading out
	// Dead - Ready to be destroyed
	enum class CubeState
	{
		Spawned,
		FadeIn,
		Moving,
		FadeOut,
		Dead,
	};

	// Collects the state of a bunch of cubes, floating in space
	class FloatingCubes final
	{
	private:
		size_t m_cubeCount; // Amount of managed cubes
		CubeState *m_cubeStates; // The state each cube is in
		glm::vec3 *m_cubePositions; // Position of each cube
		glm::quat *m_cubeRotations; // Rotation of each cube

	public:
		FloatingCubes(size_t count);
		~FloatingCubes();

		void update(double time); // Update the state of each cube and 
		void render();
	};
}
