#include "globals.h"

bool globals::init()
{
	base = reinterpret_cast<uintptr_t>(GetModuleHandle(L"sauerbraten.exe"));
	if (!base)
		return false;

	lplayer = *reinterpret_cast<fpsent**>(base + offsets::localplayer);
	if (!lplayer)
		return false;

	entitylist = *reinterpret_cast<fpsentlist**>(base + offsets::entitylist);
	if (!entitylist)
		return false;

	view_matrix  = reinterpret_cast<float*>(globals::base + offsets::viewmatrix);
	player_count = reinterpret_cast<int*>(globals::base + offsets::player_count);
	gamemode     = reinterpret_cast<uint8_t*>(globals::base + offsets::gamemode);
	pushback     = reinterpret_cast<void*>(globals::base + offsets::pushback_offset);
	return true;
}
