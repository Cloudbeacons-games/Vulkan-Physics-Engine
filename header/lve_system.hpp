#pragma once

#include "lve_entity.hpp"
#include <set>

namespace lve {
	class System
	{
	public:
		std::set<Entity> mEntities;
	};
}