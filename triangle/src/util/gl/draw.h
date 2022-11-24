#pragma once
#pragma comment(lib, "OpenGL32.lib")
#include <numbers>
#include <stdio.h>

#include <Windows.h>
#include <gl\GL.h>

#include "../../sdk/math.h"

namespace rgb
{
	const GLubyte red[3] = { 255,0,0 };
	const GLubyte green[3] = { 0,255,0 };
	const GLubyte gray[3] = { 55,55,55 };
	const GLubyte lightgray[3] = { 192,192,192 };
	const GLubyte white[3] = { 255,255,255 };
	const GLubyte black[3] = { 0,0,0 };
}

namespace gl
{
	inline int window_width;
	inline int window_height;

	void setup();
	void restore();

	void draw_filled_rect(float x, float y, float width, float height, const GLubyte color[3]);
	void draw_rect(float x, float y, float width, float height, float lineWidth, const GLubyte color[3]);

	void draw_line(float x, float y, const GLubyte color[3], float thickness);
	void draw_line(screen_t from, screen_t to, const GLubyte color[3], float thickness);
	void draw_circle(screen_t position, float radius, const GLubyte color[3]);

	bool is_within_radius(screen_t position, screen_t c_origin, float radius);
	bool world_to_screen(vec_t position, screen_t& screen, float view_matrix[16]);
}