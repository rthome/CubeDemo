#include "FloatingCubes.hpp"

namespace cubedemo
{
	FloatingCubes::FloatingCubes(size_t count)
		: m_cubeCount{ count }, m_cubeStates{ new CubeState[count] }, m_cubePositions{ new glm::vec3[count] }, m_cubeRotations{ new glm::quat[count] }
	{

	}

	FloatingCubes::~FloatingCubes()
	{
		delete[] m_cubeStates;
		delete[] m_cubePositions;
		delete[] m_cubeRotations;
	}

	void FloatingCubes::update(double time)
	{

	}

	void FloatingCubes::render()
	{

	}
}
