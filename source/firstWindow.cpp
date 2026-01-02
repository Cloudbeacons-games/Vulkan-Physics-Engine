#include "../header/firstWindow.hpp"
#include "../header/simple_render_system.hpp"
#include "../header/lve_camera.hpp"
#include "../header/keyboard_movement.hpp"

#include<stdexcept>
#include <array>
#include <iostream>
#include <chrono>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
namespace lve {
	
	Coordinator lveCoordinator{};
	
	std::vector<Entity> entities(1);
	FirstApp::FirstApp()
	{
		createCoordinator();
		loadEntities();
	}

	FirstApp::~FirstApp() {}

	void FirstApp::run()
	{
		SimpleRenderSystem simpeRenderSystem{ lveDevice,lveRenderer.getSwapChainRenderPass() };
		LveCamera camera{};
		
		auto viewerObject = lveCoordinator.CreateEntity();
		lveCoordinator.AddComponent(viewerObject, Transform3dComponent{});
		
		auto cameraSystem = lveCoordinator.RegisterSystem<KeyboardMovement_System>();
		{
			Signature signature;
			signature.set(lveCoordinator.GetComponentType<Transform3dComponent>());
			lveCoordinator.SetSystemSignature<KeyboardMovement_System>(signature);
		}

		auto currentTime = std::chrono::high_resolution_clock::now();
		while (!lveWindow.shouldClose())
		{
			glfwPollEvents();

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			
			cameraSystem->Update(lveWindow.getGLFWwindow(), frameTime, viewerObject);
			camera.setViewYXZ(lveCoordinator.GetComponent<Transform3dComponent>(viewerObject).translation, lveCoordinator.GetComponent<Transform3dComponent>(viewerObject).rotation);

			float aspectRatio = lveRenderer.getAspectRatio();
			//camera.setOrthographicsProjection(-aspectRatio, aspectRatio, -1, 1, -1, 1);
			camera.setPrespectiveProjection(glm::radians(50.0f), aspectRatio, .1f, 10.f);

			if (auto commandBuffer = lveRenderer.beginFrame())
			{
				lveRenderer.beginSwapChainRenderPass(commandBuffer);
				simpeRenderSystem.renderEntities(commandBuffer, entities, camera);
				lveRenderer.endSwapChainRenderPass(commandBuffer);
				lveRenderer.endFrame();
			}
		}

		vkDeviceWaitIdle(lveDevice.device());
	}

	void FirstApp::createCoordinator()
	{
		lveCoordinator.Init();

		lveCoordinator.RegisterComponent<Transform2dComponent>();
		lveCoordinator.RegisterComponent<Transform3dComponent>();
		lveCoordinator.RegisterComponent<Render>();	

	}

	std::unique_ptr<LveModel> createCubeModel(LveDevice& device, glm::vec3 offset) {
		LveModel::Builder modelBuilder{};
		modelBuilder.vertices = {

			// left face (white)
			{{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
			{{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

			// right face (yellow)
			{{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

			// top face (orange, remember y axis points down)
			{{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
			{{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

			// bottom face (red)
			{{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
			{{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

			 // nose face (blue)
			{{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
			{{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

			// tail face (green)
			{{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
			{{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			{{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
			 {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
		};
		for (auto& v : modelBuilder.vertices) {
			v.position += offset;
		}

		modelBuilder.indices = { 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
								12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };

		return std::make_unique<LveModel>(device, modelBuilder);
	}


	void FirstApp::loadEntities()
	{

		std::shared_ptr<LveModel> lveModel = createCubeModel(lveDevice,{.0f,.0f,.0f});
		for (auto& entity : entities)
		{

			entity = lveCoordinator.CreateEntity();

			lveCoordinator.AddComponent(entity, Transform3dComponent{
				{.0f,.0f,2.5f},//translation
				{.4f,.4f,.4f}//scale
				});
			lveCoordinator.AddComponent(entity, Render{ {lveModel} });
		}
		
	}


}