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

set(CD_SOURCES
	src/Main.cpp)

set(CD_HEADERS
    )

# Add GLM and GLFW
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glfw)
add_subdirectory(${PROJECT_SOURCE_DIR}/lib/glm)

# Fix up additional include directories
include_directories(${PROJECT_SOURCE_DIR}/lib/glfw/include)
include_directories(${PROJECT_SOURCE_DIR}/lib/glm)

add_executable(CubeDemo ${CD_HEADERS} ${CD_SOURCES})
target_link_libraries(CubeDemo glfw ${GLFW_LIBRARIES})
