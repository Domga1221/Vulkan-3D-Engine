#include "lve_window.hpp"

#include <stdexcept>

namespace lve {
	// Constructor with member initializer list
	LveWindow::LveWindow(int width, int height, std::string windowName) : width{ width }, height{ height }, windowName{ windowName }{
		initWindow();
	}

	LveWindow::~LveWindow() {
		glfwDestroyWindow(window); // destroy window
		glfwTerminate(); // terminate glfw Library
	}

	void LveWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("failed to create window surface");
		}
	}

	void LveWindow::initWindow() {
		glfwInit(); // initialize glfw library
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // no openGL, because Vulkan
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // no resize allowed

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr); // no fullscreen
	}
}