#include "first_app.hpp"

namespace lve {

	void lve::FirstApp::run() {
		while (!lveWindow.shouldClose()) {
			glfwPollEvents(); // poll window events, keyboard keys, etc.
		}
	}
}
