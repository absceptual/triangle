#pragma once
#pragma comment(lib, "OpenGL32.lib")

#include <Windows.h>
#include <stdio.h>
#include <gl\GL.h>

#include "../../sdk/math.h"

namespace gl
{
	class Font
	{
	public:
		const char* font_name = "Consolas";
		bool bBuilt = false;
		GLuint base;
		HDC hdc = nullptr;
		int height;
		int width;

	public:
		void Build(int height);
		void Print(float x, float y, const GLubyte color[3], const char* format, ...);

		vec_t center(float x, float y, float width, float height, float textWidth, float textHeight);
		float center(float x, float width, float textWidth);
	};
}