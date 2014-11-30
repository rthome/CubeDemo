cmake_minimum_required(VERSION 3.0)

project(CubeDemo)

# Warnings for MSVC
if(MSVC)
  # Force to always compile with W4
  if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
    string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
  else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  endif()
  # Suppress dump "conditional expression is constant" warning in VS
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4127")
endif()

# Warnings and C++11 for GCC and Clang
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -std=c++11")
endif()

# XCode specific stuff
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    set(CMAKE_XCODE_ATTRIBUTE_GCC_VERSION "com.apple.compilers.llvm.clang.1_0")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
    set(CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
endif()

# Tell GLM to always use radians
add_definitions(-DGLM_FORCE_RADIANS)

file(READ "${PROJECT_SOURCE_DIR}/src/shaders/cubes_vert.glsl" CD_SHADERSRC_CUBES_VERTEX)
file(READ "${PROJECT_SOURCE_DIR}/src/shaders/cubes_frag.glsl" CD_SHADERSRC_CUBES_FRAGMENT)
file(READ "${PROJECT_SOURCE_DIR}/src/shaders/background_vert.glsl" CD_SHADERSRC_BACKGROUND_VERTEX)
file(READ "${PROJECT_SOURCE_DIR}/src/shaders/background_frag.glsl" CD_SHADERSRC_BACKGROUND_FRAGMENT)

# Create shader header file
set(CD_SOURCES_TEMPLATE_PATH "${PROJECT_SOURCE_DIR}/src/Shaders.hpp.in")
set(CD_SOURCES_HEADER_PATH "${PROJECT_BINARY_DIR}/Shaders.hpp")
configure_file(${CD_SOURCES_TEMPLATE_PATH} ${CD_SOURCES_HEADER_PATH})

set(CD_SOURCES
    src/gl_core_3_3.cpp
    src/GLShader.cpp
    src/GLTextureBuffer.cpp
    src/FloatingCubes.cpp
    src/TriangleBackground.cpp
    src/GameTime.cpp
    src/Spiral.cpp
    src/Main.cpp)

set(CD_HEADERS
    src/gl_core_3_3.hpp
    src/GLShader.hpp
    src/GLTextureBuffer.hpp
    src/FloatingCubes.hpp
    src/TriangleBackground.hpp
    src/GameTime.hpp
    src/Spiral.hpp
    src/NonCopyable.hpp
	src/MeshData.hpp
    src/Util.hpp)

set(CD_SHADERFILES
    src/shaders/background_vert.glsl
    src/shaders/background_frag.glsl
    src/shaders/cubes_vert.glsl
    src/shaders/cubes_frag.glsl)

set(CD_PROCESSEDFILES
    ${CD_SOURCES_TEMPLATE_PATH}
    ${CD_SOURCES_HEADER_PATH})

# Add GLM and GLFW
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glm)

# Fix up additional include directories
include_directories("${PROJECT_BINARY_DIR}") # For the generated shader source header file
include_directories(${PROJECT_SOURCE_DIR}/lib/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/glm)

add_executable(CubeDemo ${CD_HEADERS} ${CD_SOURCES} ${CD_SHADERFILES} ${CD_PROCESSEDFILES})
target_link_libraries(CubeDemo glfw ${GLFW_LIBRARIES})

source_group("GLSL Files" FILES ${CD_SHADERFILES})
source_group("Generated Files" FILES ${CD_PROCESSEDFILES})
