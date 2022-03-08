#include "first_app.hpp"
#include "simple_render_system.hpp"

#include <stdexcept>
#include <array>
#include <vector>

#define GLM_FORCE_RADIANS // glm functions will expect angles in radians not degrees
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // depth buffer values will range from 0 to 1
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace lve {

	FirstApp::FirstApp() {
		loadGameObjects();
	}

	FirstApp::~FirstApp() {}

	void lve::FirstApp::run() {
		SimpleRenderSystem simpleRenderSystem{ lveDevice, lveRenderer.getSwapChainRenderPass() };

		while (!lveWindow.shouldClose()) {
			glfwPollEvents(); // poll window events, keyboard keys, etc.
			
			if (auto commandBuffer = lveRenderer.beginFrame()) {
				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects);
				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lveDevice.device()); // CPU blocks until all GPU operations have completed
	}

	void FirstApp::loadGameObjects() {
		std::vector<LveModel::Vertex> vertices{
			{{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
			{{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
			{{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
		};

		auto lveModel = std::make_shared<LveModel>(lveDevice, vertices);

		auto triangle = LveGameObject::createGameObject();
		triangle.model = lveModel;
		triangle.color = { 0.1f,0.8f,0.1f };
		triangle.transform2d.translation.x = 0.2f;
		triangle.transform2d.scale = { 2.0f, 0.5f };
		triangle.transform2d.rotation = 0.25f * glm::two_pi<float>(); // 90 degree rotation

		gameObjects.push_back(std::move(triangle));
	}

}
