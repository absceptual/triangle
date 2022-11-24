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
	inline void* pushback;

	inline bool silent_enabled{ true };
	bool init();
}

namespace offsets
{
	inline size_t player_count    = 0x346C9C;
	inline size_t entitylist      = 0x346C90;
	inline size_t localplayer     = 0x2A2560;
	inline size_t viewmatrix      = 0x32D040;
	inline size_t gamemode        = 0x26f6c0;
	inline size_t pushback_offset = 0x1DB76F;
}

namespace settings
{
	namespace esp
	{
		inline bool enabled        = true;
		inline bool bounding_boxes = false;
		inline bool ingame_boxes   = true;
		inline bool snaplines      = true;
	}

	namespace aimbot
	{
		inline bool enabled        = true;
		inline float radius        = 100.0f;
		inline bool friendly_fire  = false;
	}

	namespace exploits
	{
		inline bool silent_aim     = true;
		inline bool infinite_ammo  = true;
		inline bool recoil		   = true;
		inline bool infinite_jump  = true;
	}
}