#pragma once

#include "../util/gl/draw.h"
#include "../core/globals.h"
#include "../sdk/entities.h"

#include <unordered_map>

namespace aimbot
{
	inline float radius = 300.0f;

	fpsent* get_closest_screen_entity(bool fov_check);
}