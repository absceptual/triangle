#include "esp.h"

#define GET_MENU_COLOR(flag) flag ? rgb::green : rgb::red
const char* title = "<triangle>";

void print_option(screen_t origin, const char* option, int& offset, const GLubyte color[3] = rgb::white)
{
	float position = esp::font.center((gl::window_width / 2) - 50, strlen(title) * esp::font.width, strlen(option) * esp::font.width);
	esp::font.Print(position, origin.y + offset, color, option);

	offset += 20;
}

void esp::draw_menu()
{
	auto top = screen_t((gl::window_width / 2) - 50, 30);
	auto offset = 20;


	esp::font.Print(top.x, top.y, rgb::white, title);

	print_option(top, "[F10] Toggle Menu", offset);
	print_option(top, "[F2] Toggle ESP", offset, GET_MENU_COLOR(settings::esp::enabled));
	print_option(top, "[F3] Toggle Bounding Boxes", offset, GET_MENU_COLOR(settings::esp::bounding_boxes));
	print_option(top, "[F4] Toggle 3D Box", offset, GET_MENU_COLOR(settings::esp::ingame_boxes));
	print_option(top, "[F5] Toggle Snaplines", offset, GET_MENU_COLOR(settings::esp::snaplines));

	offset += 30;
	print_option(top, "[F6] No Recoil", offset, GET_MENU_COLOR(settings::exploits::recoil));
	print_option(top, "[F7] Silent Aim", offset, GET_MENU_COLOR(settings::exploits::silent_aim));
	print_option(top, "[F8] Infinite Jump", offset, GET_MENU_COLOR(settings::exploits::infinite_jump));
}


void esp::run()
{
	GLint viewport[4];
	glViewport(0, 0, viewport[2], viewport[3]);
	glGetIntegerv(GL_VIEWPORT, viewport);
	
	gl::window_width = viewport[2];
	gl::window_height = viewport[3];

    for (int i = 0; i < *globals::player_count; ++i) {
        auto entity = globals::entitylist->get_entity(i);

        if (entity != globals::lplayer && entity->is_valid())
        {
			if (settings::esp::ingame_boxes)
				draw_3d_box(entity);

			if (settings::esp::bounding_boxes)
				draw_box(entity);

			if (settings::esp::snaplines)
				draw_line(entity);

			esp::draw_name(entity);
        }
    }
}

bool esp::is_teammate(fpsent* entity)
{
	auto result = std::find(teammodes.begin(), teammodes.end(), *globals::gamemode);
	if (result == teammodes.end()) // Current gamemode isn't a teammode (ffa or some shit like that)
		return false;

	return !strcmp(entity->team, globals::lplayer->team);
}

const GLubyte* esp::get_color(fpsent* entity)
{
	return esp::is_teammate(entity) ? rgb::green : rgb::red;
}

void esp::draw_3d_box(fpsent* entity)
{
	// Length and width will be the size (obviously) of the bottom square. You can change height to whatever is necessary for you.
	static int length = 10;
	static int width  = 10;
	static int height = PLAYER_HEIGHT;

	Vector3 bottom[4]{ };
	Vector3 top[4]{ };

	Vector3 bl, br, btl, btr;
	Vector3 tl, tr, ttl, ttr;

	// All other positions will be offsets of the top (bottom) left
	bottom[0] = entity->origin();

	// Magic numbers (yucky!) to offset from our origin to the bottom left, sets up everything else nicely though
	bottom[0].x -= 5; 
	bottom[0].y -= 5;

	bottom[1] = bottom[0];
	bottom[1].y += length; // To

	bottom[2] = bottom[1];
	bottom[2].x += width;

	bottom[3] = bottom[2];
	bottom[3].y -= length;

	// We are using the same positions but just adding height

	top[0] = bottom[0];
	top[1] = bottom[1];
	top[2] = bottom[2];
	top[3] = bottom[3];

	top[0].z = top[1].z = top[2].z = top[3].z += height;

	screen_t s_bottom[4]{ };
	screen_t s_top[4]{ };

	for (int i = 0; i < 4; ++i) {
		if (!gl::world_to_screen(bottom[i], s_bottom[i], globals::view_matrix))
			return;
	}

	for (int i = 0; i < 4; ++i) {
		if (!gl::world_to_screen(top[i], s_top[i], globals::view_matrix))
			return;
	}

	auto color = get_color(entity);
	// Bottom box
	for (int i = 0; i < 4; ++i)
	{
		// Connect final line to original point
		if (i == 3)
			gl::draw_line(s_bottom[3], s_bottom[0], color, 3.0f);
		else
			gl::draw_line(s_bottom[i], s_bottom[i + 1], color, 3.0f);
	}

	// Top box
	for (int i = 0; i < 4; ++i)
	{

		if (i == 3)
			gl::draw_line(s_top[3], s_top[0], color, 3.0f);
		else
			gl::draw_line(s_top[i], s_top[i + 1], color, 3.0f);
	}

	// Connect top to bottom
	for (int i = 0; i < 4; ++i)
	{
		gl::draw_line(s_bottom[i], s_top[i], color, 3.0f);
	}
}

void esp::draw_fov(float radius)
{
	const GLubyte* color;
	if (aimbot::get_closest_screen_entity(true))
		color = rgb::red;
	else
		color = rgb::white;

	gl::draw_circle(screen_t(gl::window_width / 2, gl::window_height / 2), radius, color);
}

void esp::draw_name(fpsent* entity)
{
	vec_t head = entity->position;
	head.x += 2;
	head.z += 7;

	screen_t s_head;
	if (!gl::world_to_screen(head, s_head, globals::view_matrix))
		return;

	esp::font.Print(s_head.x, s_head.y + 10, rgb::white, "%s", entity->name);

}

void esp::draw_box(fpsent* entity)
{
	vec_t origin = entity->origin();
	vec_t head = entity->position;

	screen_t s_origin, s_head;
	if (!gl::world_to_screen(origin, s_origin, globals::view_matrix) || !gl::world_to_screen(head, s_head, globals::view_matrix))
		return;

	float height = s_origin.y - s_head.y;
	float width = height * (45.f / 80.f);

	auto color = get_color(entity);
	gl::draw_rect(s_head.x - width / 2, s_head.y, width, height, 2.0f, color);
}

void esp::draw_line(fpsent* entity)
{
	auto origin = entity->origin();

	screen_t s_origin;
	if (!gl::world_to_screen(origin, s_origin, globals::view_matrix))
		return;

	auto color = get_color(entity);
	gl::draw_line(screen_t( gl::window_width / 2, gl::window_height ), s_origin, color, 2.0f);
}
