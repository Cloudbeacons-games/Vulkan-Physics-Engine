#include "../header/simple_render_system.hpp"
//#include "../header/firstWindow.hpp"

#include<stdexcept>
#include <array>
#include <iostream>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
namespace lve {

	extern Coordinator lveCoordinator;
	struct SimplePushConstantData {
		glm::mat4 transform{ 1.f };
		alignas(16)glm::vec3 color;
	};
	SimpleRenderSystem::SimpleRenderSystem(LveDevice& device, VkRenderPass renderPass):lveDevice{device}
	{
		createPipelineLayout();
		createPipeline(renderPass);
	}

	SimpleRenderSystem::~SimpleRenderSystem()
	{
		vkDestroyPipelineLayout(lveDevice.device(), pipelineLayout, nullptr);
	}

	void SimpleRenderSystem::createPipelineLayout()
	{

		VkPushConstantRange pushConstantRange{};
		pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
		pushConstantRange.offset = 0;
		pushConstantRange.size = sizeof(SimplePushConstantData);

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0;
		pipelineLayoutInfo.pSetLayouts = nullptr;
		pipelineLayoutInfo.pushConstantRangeCount = 1;
		pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;

		if (vkCreatePipelineLayout(lveDevice.device(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("Failed to create pipeline Layout");
		}
	}

	void SimpleRenderSystem::createPipeline(VkRenderPass renderPass)
	{
		assert(pipelineLayout != nullptr && "Cannot create pipeline before pipeline layout");
		PipelineConfigInfo pipelineConfig{};
		LvePipeline::defaultPipelineConfigInfo(pipelineConfig);
		pipelineConfig.renderPass = renderPass;
		pipelineConfig.pipelineLayout = pipelineLayout;
		lvePipeline = std::make_unique<LvePipeline>(lveDevice, "shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv", pipelineConfig);
	}


	void SimpleRenderSystem::renderEntities(VkCommandBuffer commandBuffer, std::vector<Entity> &entities, const LveCamera &camera)
	{
		lvePipeline->bind(commandBuffer);

		auto projectionView = camera.getProjection() * camera.getView();
		for (auto& entity : entities)
		{
			SimplePushConstantData push{};
			push.color = lveCoordinator.GetComponent<Render>(entity).color;
			push.transform = projectionView * lveCoordinator.GetComponent<Transform3dComponent>(entity).mat4();

			vkCmdPushConstants(commandBuffer, pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(SimplePushConstantData), &push);
			lveCoordinator.GetComponent<Render>(entity).model->bind(commandBuffer);
			lveCoordinator.GetComponent<Render>(entity).model->draw(commandBuffer);
		}
	}


}