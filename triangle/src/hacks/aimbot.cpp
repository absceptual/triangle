#include "aimbot.h"

fpsent* aimbot::get_closest_screen_entity(bool fov_check = false)
{
	std::unordered_map<void*, screen_t> s_entities{ };
	for (int i = 1; i < *globals::player_count; ++i) 
	{
		auto entity = globals::entitylist->get_entity(i);
		if (!entity->is_valid())
			continue;

		screen_t screen;
		if (!gl::world_to_screen(entity->position, screen, globals::view_matrix))
			continue;

		// Bind our entity address to a screen position
		s_entities[entity] = screen;
	}

	// Loop through all the screen positions and get the closest one

	fpsent* b_entity = nullptr;
	float b_dist = LLONG_MAX;

	auto center = screen_t(gl::window_width / 2, gl::window_height / 2);
	for (const auto& [entity, position] : s_entities)
	{	
		auto delta = position - center;
		auto magnitude = sqrt(pow(delta.x, 2) + pow(delta.y, 2));

		if (magnitude < b_dist)
		{
			b_entity = static_cast<fpsent*>(entity);
			b_dist = magnitude;
		}
	}

	if (!b_entity)
		return nullptr;

	// We aren't checking if the entity is within our FOV
	if (!fov_check)
		return b_entity;

	auto visible = gl::is_within_radius(s_entities[b_entity], center, aimbot::radius);
	return visible ? b_entity : nullptr;

}
