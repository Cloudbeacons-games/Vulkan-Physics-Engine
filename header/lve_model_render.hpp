#pragma once
#include "lve_model.hpp"

#include<memory>

namespace lve{

	struct Render
	{
		std::shared_ptr<LveModel> model{};
		glm::vec3 color{};
	};
}