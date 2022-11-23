#include "esp.h"


void esp::run()
{
    for (int i = 0; i < *globals::player_count; ++i) {
        auto entity = globals::entitylist->get_entity(i);
        if (entity && entity != globals::lplayer)
        {
         
           // auto head = entity->position;
            //auto origin = entity->origin();
            vec_t p1 = entity->origin();
            vec_t p2 = entity->position;


            screen_t _p1, _p2;
            if (!gl::world_to_screen(p1, _p1, globals::view_matrix) || !gl::world_to_screen(p2, _p2, globals::view_matrix))
                continue;

            float height = _p1.y - _p2.y;
            float width = height * (45.f / 80.f); 
            // gl::draw_rect(_p2.x - width / 2, _p2.y, width, height, 2.0f, rgb::green);
			esp::draw_3d_box(entity);
			esp::draw_line(entity);
        }
    }
}

bool esp::is_valid_entity(fpsent* entity)
{
	return entity && entity->health && entity->alive;
}

bool esp::is_teammate(fpsent* entity)
{
	auto result = std::find(teammodes.begin(), teammodes.end(), *globals::gamemode);
	if (result == teammodes.end()) // Current gamemode isn't a teammode (ffa or some shit like that)
		return false;

	return !strcmp(entity->team, globals::lplayer->team);
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

	// Bottom box
	for (int i = 0; i < 4; ++i)
	{
		// Connect final line to original point
		if (i == 3)
			gl::draw_line(s_bottom[3], s_bottom[0], rgb::green, 3.0f);
		else
			gl::draw_line(s_bottom[i], s_bottom[i + 1], rgb::green, 3.0f);
	}

	// Top box
	for (int i = 0; i < 4; ++i)
	{

		if (i == 3)
			gl::draw_line(s_top[3], s_top[0], rgb::green, 3.0f);
		else
			gl::draw_line(s_top[i], s_top[i + 1], rgb::green, 3.0f);
	}

	// Connect top to bottom
	for (int i = 0; i < 4; ++i)
	{
		gl::draw_line(s_bottom[i], s_top[i], rgb::green, 3.0f);
	}
}

void esp::draw_line(fpsent* entity)
{
	auto origin = entity->origin();

	screen_t s_origin;
	if (!gl::world_to_screen(origin, s_origin, globals::view_matrix))
		return;

	const GLubyte* color;
	if (esp::is_teammate(entity))
		color = rgb::green;
	else
		color = rgb::red;

	gl::draw_line(screen_t( gl::window_width / 2, gl::window_height ), s_origin, color, 2.0f);
}
