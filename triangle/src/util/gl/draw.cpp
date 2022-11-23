
#include "draw.h"

void gl::setup()
{
	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glPushMatrix();
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);
	glViewport(0, 0, viewport[2], viewport[3]);
	gl::window_width = viewport[2];
	gl::window_height = viewport[3];

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, viewport[2], viewport[3], 0, -1, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_DEPTH_TEST);
}

void gl::restore()
{
	glPopMatrix();
	glPopAttrib();
}

void gl::draw_filled_rect(float x, float y, float width, float height, const GLubyte color[3])
{
	glColor3ub(color[0], color[1], color[2]);
	glBegin(GL_QUADS);
	glVertex2f(x, y);
	glVertex2f(x + width, y);
	glVertex2f(x + width, y + height);
	glVertex2f(x, y + height);
	glEnd();
}

void gl::draw_rect(float x, float y, float width, float height, float lineWidth, const GLubyte color[3])
{
	glLineWidth(lineWidth);
	glBegin(GL_LINE_STRIP);
	glColor3ub(color[0], color[1], color[2]);
	glVertex2f(x - 0.5f, y - 0.5f);
	glVertex2f(x + width + 0.5f, y - 0.5f);
	glVertex2f(x + width + 0.5f, y + height + 0.5f);
	glVertex2f(x - 0.5f, y + height + 0.5f);
	glVertex2f(x - 0.5f, y - 0.5f);
	glEnd();
}

bool gl::world_to_screen(vec_t position, screen_t& screen, float vm[16])
{
	vec4_t mvp;
	mvp.x = position.x * vm[0] + position.y * vm[4] + position.z * vm[8] + vm[12];
	mvp.y = position.x * vm[1] + position.y * vm[5] + position.z * vm[9] + vm[13];
	mvp.z = position.x * vm[2] + position.y * vm[6] + position.z * vm[10] + vm[14];
	mvp.w = position.x * vm[3] + position.y * vm[7] + position.z * vm[11] + vm[15];

	if (mvp.w < 0.1f) // not present on screen
		return false;

	
	screen_t ndc;
	ndc.x = mvp.x / mvp.w;
	ndc.y = mvp.y / mvp.w;

	screen.x = (window_width / 2 * ndc.x) + (ndc.x + window_width / 2);
	screen.y = -(window_height / 2 * ndc.y) + (ndc.y + window_height / 2 );
		return true;
}

void gl::draw_line(screen_t from, screen_t to, const GLubyte color[3], float thickness)
{
	glLineWidth(thickness);
	glBegin(GL_LINES);
	glColor3ub(color[0], color[1], color[2]);
	glVertex2f(from.x, from.y);
	glVertex2f(to.x, to.y);
	glEnd();
}