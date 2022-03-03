#include "first_app.hpp"
#include <stdexcept>
#include <array>
#include <vector>

namespace lve {

	FirstApp::FirstApp() {
		loadModels();
		createPipelineLayout();
		createPipeline();
		createCommandBuffers();
	}

	FirstApp::~FirstApp() {
		vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
	}

	void lve::FirstApp::run() {
		while (!lveWindow.shouldClose()) {
			glfwPollEvents(); // poll window events, keyboard keys, etc.
			drawFrame();
		}

		vkDeviceWaitIdle(lveDevice.device()); // CPU blocks until all GPU operations have completed
	}

	void FirstApp::loadModels() {
		std::vector<LveModel::Vertex> vertices{
			{{0.0f, -0.5f}},
			{{0.5f, 0.5f}},
			{{-0.5f, 0.5f}}
		};

		lveModel = std::make_unique<LveModel>(lveDevice, vertices);
	}

	void FirstApp::createPipelineLayout() {
		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 0;
		pipelineLayoutInfo.pPushConstantRanges = nullptr;

		if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout");
		}
	}

	void FirstApp::createPipeline() {
		PipelineConfigInfo pipelineConfig{};
		LvePipeline::defaultPipelineConfigInfo(
			pipelineConfig,
			lveSwapChain.width(),
			lveSwapChain.height());
		pipelineConfig.renderPass = lveSwapChain.getRenderPass(); // describes structure and format of render buffer 
		pipelineConfig.pipelineLayout = pipelineLayout;
		lvePipeline = std::make_unique<LvePipeline>(
			lveDevice,
			"../simple_shader.vert.spv",
			"../simple_shader.frag.spv",
			pipelineConfig);
	}

	void FirstApp::createCommandBuffers() {
		commandBuffers.resize(lveSwapChain.imageCount()); // 2 or 3, double or triple buffering

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandPool = lveDevice.getCommandPool();
		allocInfo.commandBufferCount = static_cast<uint32_t>(commandBuffers.size());

		if (vkAllocateCommandBuffers(lveDevice.device(), &allocInfo, commandBuffers.data()) != VK_SUCCESS) {
			throw std::runtime_error("failed to allocate command buffers!");
		}

		for (int i = 0; i < commandBuffers.size(); i++) {
			VkCommandBufferBeginInfo beginInfo{};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			
			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS) {
				throw std::runtime_error("failed to begin recording command buffer!");
			}

			VkRenderPassBeginInfo renderPassInfo{};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = lveSwapChain.getRenderPass();
			renderPassInfo.framebuffer = lveSwapChain.getFrameBuffer(i);

			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = lveSwapChain.getSwapChainExtent();

			std::array<VkClearValue, 2> clearValues{};
			clearValues[0].color = { 0.1f, 0.1f, 0.1f, 1.0f }; // rgba
			clearValues[1].depthStencil = { 1.0f, 0 }; // index 0 is color attachment of the frame buffer, index 1 is depth
			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE); // no secondary command buffers

			lvePipeline->bind(commandBuffers[i]); // bind graphics pipeline
			lveModel->bind(commandBuffers[i]); // bind model with vertix data
			lveModel->draw(commandBuffers[i]); // record command buffer to draw every vertex contained in buffer

			vkCmdEndRenderPass(commandBuffers[i]);
			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS) {
				throw std::runtime_error("failed to record command buffer!");
			}
		}
	};

	void FirstApp::drawFrame() {
		uint32_t imageIndex;
		auto result = lveSwapChain.acquireNextImage(&imageIndex); // determine if nextImage could be acquired

		if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		result = lveSwapChain.submitCommandBuffers(&commandBuffers[imageIndex], &imageIndex);
		if (result != VK_SUCCESS) {
			throw std::runtime_error("failed to present swap chain image");
		}
	};
}
