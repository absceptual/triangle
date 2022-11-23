#pragma once
#include <Windows.h>

#include "../sdk/entities.h"

namespace globals
{
	inline uintptr_t base;
	
	inline fpsentlist* entitylist;
	inline fpsent* lplayer;
	
	inline int* player_count;
	inline uint8_t* gamemode;
	inline float* view_matrix;
	bool init();
}

namespace offsets
{
	inline size_t player_count = 0x346C9C;
	inline size_t entitylist   = 0x346C90;
	inline size_t localplayer  = 0x2A2560;
	inline size_t viewmatrix   = 0x32D040;
	inline size_t gamemode     = 0x26f6c0;
}

