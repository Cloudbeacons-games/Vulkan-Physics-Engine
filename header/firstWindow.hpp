#pragma once

#include "lve_window.hpp"
#include "lve_device.hpp"
#include "lve_coordinator.hpp"
#include "lve_renderer.hpp"
#include "lve_transform_2D.hpp"
#include "lve_transform_3D.hpp"
#include "lve_model_render.hpp"


//std
#include <memory>
#include <vector>
namespace lve {
	//Coordinator lveCoordinator;
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 600;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp&operator=(const FirstApp&) = delete;

		void run();


	private:
		void createCoordinator();
		void loadEntities();

		LveWindow lveWindow{ WIDTH,HEIGHT,"HELLO VULKAN" };
		LveDevice lveDevice{ lveWindow };
		LveRenderer lveRenderer{ lveWindow,lveDevice };
	};
}