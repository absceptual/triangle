#pragma once

#include "../sdk/entities.h"
#include "../sdk/math.h"

#include "../core/globals.h"

#include "../util/gl/draw.h"
#include "../util/gl/text.h"

#include "../hacks/aimbot.h"

namespace esp
{
	inline gl::Font font;

	void run();

	bool is_teammate(fpsent* entity);
	const GLubyte* get_color(fpsent* entity);

	void draw_menu();
	void draw_box(fpsent* entity);
	void draw_line(fpsent* entity);
	void draw_3d_box(fpsent* entity);
	void draw_fov(float radius);
	void draw_name(fpsent* entity);
}