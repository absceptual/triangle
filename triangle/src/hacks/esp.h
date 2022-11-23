#pragma once

#include "../sdk/entities.h"
#include "../sdk/math.h"

#include "../core/globals.h"
#include "../util/gl/draw.h"

namespace esp
{
	void run();


	bool is_valid_entity(fpsent* entity);
	bool is_teammate(fpsent* entity);

	void draw_box(fpsent* entity);
	void draw_line(fpsent* entity);
	void draw_3d_box(fpsent* entity);
}