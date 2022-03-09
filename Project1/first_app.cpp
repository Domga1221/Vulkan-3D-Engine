#include "first_app.hpp"
#include "lve_camera.hpp"
#include "simple_render_system.hpp"
#include "keyboard_movement_controller.hpp"

#include <stdexcept>
#include <array>
#include <vector>
#include <chrono>

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
        LveCamera camera{};
        camera.setViewTarget(glm::vec3(-1.0f, -2.0f, -2.0f), glm::vec3(0.0f, 0.0f, 2.5f));

        auto viewerObject = LveGameObject::createGameObject(); // store camera state
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now(); // high precision value representing current time


		while (!lveWindow.shouldClose()) {
			glfwPollEvents(); // poll window events, keyboard keys, etc.

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            currentTime = newTime;

            cameraController.moveInPlaneXZ(lveWindow.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = lveRenderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 10.0f);
			
			if (auto commandBuffer = lveRenderer.beginFrame()) {
				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(commandBuffer, gameObjects, camera);
				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lveDevice.device()); // CPU blocks until all GPU operations have completed
	}


    void FirstApp::loadGameObjects() {
        std::shared_ptr<LveModel> lveModel = LveModel::createModelFromFile(lveDevice, "../models/flat_vase.obj");
        auto gameObject = LveGameObject::createGameObject();
        gameObject.model = lveModel;
        gameObject.transform.translation = { -0.5f, 0.5f, 2.5f };
        //gameObject.transform.scale = glm::vec3(3.0f);
        gameObject.transform.scale = { 3.0f, 1.5f, 3.0f };
        gameObjects.push_back(std::move(gameObject));


        lveModel = LveModel::createModelFromFile(lveDevice, "../models/smooth_vase.obj");
        auto smoothVase = LveGameObject::createGameObject();
        smoothVase.model = lveModel;
        smoothVase.transform.translation = { 0.5f, 0.5f, 2.5f };
        smoothVase.transform.scale = { 3.f, 1.5f, 3.f };
        gameObjects.push_back(std::move(smoothVase));
    }


}
