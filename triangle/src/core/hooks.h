#pragma once
#include <map>

#include "../util/mem.h"
#include "../hacks/esp.h"

#include "globals.h"

namespace hooks
{
	using swapbuffers_t	= BOOL(__stdcall*)(HDC);
	inline memory::trampoline_data* swapbuffers{ };

	inline std::map<int, HGLRC> contexts;
	BOOL __stdcall hk_swapbuffers(HDC hdc);

	void init();
}