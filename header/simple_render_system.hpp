#pragma once

#include "lve_camera.hpp"
#include "lve_pipeline.hpp"
#include "lve_device.hpp"
#include "lve_coordinator.hpp"
#include "lve_transform_2D.hpp"
#include "lve_transform_3D.hpp"
#include "lve_model_render.hpp"



//std
#include <memory>
#include <vector>
namespace lve {
	class SimpleRenderSystem
	{
	public:

		SimpleRenderSystem(LveDevice& device,VkRenderPass renderPass);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void createPipelineLayout();
		void createPipeline(VkRenderPass renderPass);
		void renderEntities(VkCommandBuffer commandBuffer, std::vector<Entity> &entities, const LveCamera &camera);

	private:
		

		LveDevice &lveDevice;
		std::unique_ptr<LvePipeline> lvePipeline;
		VkPipelineLayout pipelineLayout;
	};
}