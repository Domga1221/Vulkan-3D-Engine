#include "first_app.hpp"
#include "lve_camera.hpp"
#include "simple_render_system.hpp"
#include "keyboard_movement_controller.hpp"
#include "lve_buffer.hpp"

#include <stdexcept>
#include <array>
#include <vector>
#include <chrono>

#define GLM_FORCE_RADIANS // glm functions will expect angles in radians not degrees
#define GLM_FORCE_DEPTH_ZERO_TO_ONE // depth buffer values will range from 0 to 1
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

namespace lve {

    struct GlobalUbo { // uniform buffer readonly data for the shader
        glm::mat4 projectionView{ 1.0f };
        glm::vec3 lightDirection = glm::normalize(glm::vec3{ 1.0f, -3.0f, -1.0f });
    };

	FirstApp::FirstApp() {
		loadGameObjects();
	}

	FirstApp::~FirstApp() {}

	void lve::FirstApp::run() {

        std::vector<std::unique_ptr<LveBuffer>> uboBuffers(LveSwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); i++) {
            uboBuffers[i] = std::make_unique<LveBuffer>(
                lveDevice,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uboBuffers[i]->map();
        }

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
                int frameIndex = lveRenderer.getFrameIndex();
                FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera
                };

                //update objects in memory
                GlobalUbo ubo{};
                ubo.projectionView = camera.getProjection() * camera.getView();
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush(); // write manually because HOST COHERENT BIT not used

                // render
				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(frameInfo, gameObjects);
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
