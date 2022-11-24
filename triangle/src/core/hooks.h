#pragma once
#include <map>


#include "../util/mem.h"

#include "../hacks/esp.h"
#include "../hacks/aimbot.h"
#include "../hacks/exploits.h"


#include "globals.h"

namespace hooks
{
	using swapbuffers_t	= BOOL(__stdcall*)(HDC);
	inline memory::trampoline_data* swapbuffers{ };

	inline std::map<int, HGLRC> contexts;
	BOOL __stdcall hk_swapbuffers(HDC hdc);

	using raycast_t = fpsent * (__fastcall*)(const vec_t* from, const vec_t* to, fpsent* entity, float* b_dist);
	inline memory::trampoline_data* raycast{ };
	fpsent* __fastcall hk_raycast(const vec_t* from, const vec_t* to, fpsent* entity, float* b_dist);

	// Ran every frame inside the swap buffers loop
	void run();

	// Hook initalization
	bool init();
}