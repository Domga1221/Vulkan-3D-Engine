#pragma once // only include once to avoid compiler errors

#define GLFW_INCLUDE_VULKAN // include GLFW and load Vulkan header
#include <GLFW/glfw3.h>

#include <string>

namespace lve {

	// little vulkan engine window
	class LveWindow {
	public: 
		LveWindow(int width, int height, std::string name);
		~LveWindow();

		// deletes copy constructor
		LveWindow(const LveWindow&) = delete; // delete operator destroys objects which are created by new
		// deletes copy assignment operator
		LveWindow& operator=(const LveWindow&) = delete;

		bool shouldClose() { return glfwWindowShouldClose(window); }; // returns if user wants to close window

		VkExtent2D getExtent() { return { static_cast<uint32_t>(width), static_cast<uint32_t>(height) }; };

		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

	private: 

		void initWindow();

		const int width;
		const int height;
		std::string windowName;
		GLFWwindow* window;
	};
}

