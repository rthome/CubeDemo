#include <cstdlib>
#include <thread>

#include "gl_core_3_3.hpp"
#include <GLFW/glfw3.h>

#include "Util.hpp"
#include "FloatingCubes.hpp"
#include "TriangleBackground.hpp"
#include "GameTime.hpp"

// Whether to limit rendering to 60 fps
// #define ENABLE_FRAMELIMITING

// Constants for initial window size
static const size_t WINDOW_WIDTH = 1280;
static const size_t WINDOW_HEIGHT = 720;

cubedemo::FloatingCubesRenderer *globalRenderer = nullptr;

void errorCallback(int error, const char *description)
{
	LOG_ERROR("GLFW ERROR: " << error << " " << description)
}

void windowResizeCallback(GLFWwindow *window, int width, int height)
{
	int fbw, fbh;
	glfwGetFramebufferSize(window, &fbw, &fbh);
	gl::Viewport(0, 0, fbw, fbh);
	if (globalRenderer != nullptr)
		globalRenderer->onWindowSizeChanged(width, height);
	LOG_INFO("Window resized. FB is now " << fbw << "x" << fbh);
}

void logRendererInfo()
{
	auto vendor = gl::GetString(gl::VENDOR);
	auto renderer = gl::GetString(gl::RENDERER);
	auto version = gl::GetString(gl::VERSION);
	auto slversion = gl::GetString(gl::SHADING_LANGUAGE_VERSION);

	LOG_INFO("OpenGL information:");
	LOG_INFO("\tVendor: " << vendor);
	LOG_INFO("\tRenderer: " << renderer);
	LOG_INFO("\tContext Version: " << version);
	LOG_INFO("\tGLSL Version: " << slversion);
}

int main(int argc, char const *argv[])
{
	LOG_INFO("Arguments (" << argc << "):");
	for (auto i = 0; i < argc; i++)
		LOG_INFO("\t" << i << ": " << argv[i]);

	LOG_INFO("Initializing GLFW...");
	if (!glfwInit())
	{
		LOG_ERROR("Error initializing GLFW. Exiting.")
			exit(EXIT_FAILURE);
	}
	glfwSetErrorCallback(errorCallback);

	// Request OpenGL 3.3 context
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	// Create window
	LOG_INFO("Creating window...")
		auto window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "CubeDemo", nullptr, nullptr);
	if (window == nullptr)
	{
		LOG_ERROR("Error creating window. Exiting")
			glfwTerminate();
		exit(EXIT_FAILURE);
	}

	// Make context current on calling thread
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, windowResizeCallback);

	// Load OpenGL functions
	LOG_INFO("Loading OpenGL functions...");
	if (!gl::sys::LoadFunctions())
	{
		glfwDestroyWindow(window);
		glfwTerminate();
		LOG_ERROR("Error loading OpenGL functions. Exiting.")
			exit(EXIT_FAILURE);
	}

	logRendererInfo();

	gl::ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	gl::Enable(gl::DEPTH_TEST);

	// Check for any errors so far
	GL_CHECK_ERRORS;

	// Set up cubes
	cubedemo::FloatingCubes floatingCubes{ 4000 };
	cubedemo::FloatingCubesRenderer *renderer = new cubedemo::FloatingCubesRenderer();
	renderer->onWindowSizeChanged(WINDOW_WIDTH, WINDOW_HEIGHT);
	globalRenderer = renderer;
    
    // set up background
    cubedemo::TriangleBackground *background = new cubedemo::TriangleBackground(10, 10);

	cubedemo::GameTime time;
    
	LOG_INFO("Entering main loop...");
	while (!glfwWindowShouldClose(window))
	{
		time = cubedemo::GameTime::next(time);

		GL_CHECK_ERRORS;

		gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

        background->update(time); // Update background animations
        floatingCubes.update(time); // Update cube states

        background->render(); // Render background first
        renderer->update(floatingCubes); // Update renderer with new cube states
        renderer->render(); // Render cubes

		GL_CHECK_ERRORS;

#ifdef ENABLE_FRAMELIMITING
        // TODO: Find out why this causes stuttering on Windows.
		const float TARGET_TIME = 16.667f; // 60 fps
		while (time.timeSince().count() < TARGET_TIME)
        {
            std::this_thread::sleep_for(chrono::microseconds(600));
        }
#endif

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	LOG_INFO("Exiting main loop...");

	globalRenderer = nullptr;
	delete renderer;
    delete background;

	glfwDestroyWindow(window);
	glfwTerminate();

	return EXIT_SUCCESS;
}
