#pragma once

#include <ctime>
#include <iostream>
#include <cassert>

// Basic logging macros
#define LOG_INFO(msg) do { std::cout << "[INFO] [" << clock() << "] " << msg << std::endl; } while(0);
#define LOG_WARN(msg) do { std::cout << "[WARN] [" << clock() << "] " << msg << std::endl; } while(0);
#define LOG_ERROR(msg) do { std::cout << "[ERROR] [" << clock() << "] ["  << __FILE__ << ":" << __LINE__ << "] " << msg << std::endl; } while(0);

// Logging macros for InstanceID-derived classes
#define LOG_INFO_ID(msg) LOG_INFO("(ID=" << uniqueObjectID() << ") " << msg)
#define LOG_WARN_ID(msg) LOG_WARN("(ID=" << uniqueObjectID() << ") " << msg)
#define LOG_ERROR_ID(msg) LOG_ERROR("(ID=" << uniqueObjectID() << ") " << msg)

// Assertion macros
#define CC_ASSERT(cond) assert(cond);
#define CC_STATIC_ASSERT(cond) static_assert(cond);

// OpenGL error checking helper macro
#define GL_CHECK_ERRORS \
    do { \
        auto _err = gl::GetError(); \
        if (_err != gl::NO_ERROR_) \
            LOG_WARN("OpenGL error " << _err << " encountered at " << __FILE__ << ":" << __LINE__); \
        CC_ASSERT(_err == gl::NO_ERROR_); \
    } while(0)
