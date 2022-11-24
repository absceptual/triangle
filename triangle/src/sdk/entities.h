#pragma once
#include <array>

#include "../util/mem.h"
#include "../sdk/math.h"


constexpr int PLAYER_HEIGHT     = 15;
constexpr int PLAYER_WIDTH      = 4;
constexpr int PLAYER_RATIO		= PLAYER_HEIGHT / PLAYER_WIDTH;

constexpr int MAX_WEAPON_COUNT  = 6;
constexpr int MAX_PLAYER_COUNT  = 33;
constexpr int MAX_NAME_LENGTH   = 16;
constexpr int MAX_TEAM_LENGTH	= 5;

#define STR_MERGE_IMPL(a, b) a##b
#define STR_MERGE(a, b) STR_MERGE_IMPL(a, b)
#define MAKE_PAD(size) STR_MERGE(_pad, __COUNTER__)[size]
#define DEFINE_MEMBER_N(type, name, offset) struct {unsigned char MAKE_PAD(offset); type name;}
#define DEFINE_FIRST_MEMBER(type, name) struct {type name;}

inline auto teammodes = std::to_array<uint8_t>({ 2, 4, 6, 8, 10, 11, 12, 17, 13, 14, 18, 15, 16, 19, 20, 21, 22 });

class fpsent
{
public:
	union 
	{
		DEFINE_MEMBER_N(velocity_t, left, 0xC);
		DEFINE_MEMBER_N(velocity_t, forward, 0x10);
		DEFINE_MEMBER_N(velocity_t, up, 0x14);
		DEFINE_MEMBER_N(vec_t, manual_velocity, 0x18);
		DEFINE_MEMBER_N(vec_t, position, 0x30);
		DEFINE_MEMBER_N(float, yaw, 0x3C);
		DEFINE_MEMBER_N(float, pitch, 0x40);
		DEFINE_MEMBER_N(float, height, 0x50);
		DEFINE_MEMBER_N(float, nametag_offset, 0x54);
		DEFINE_MEMBER_N(float, width, 0x58);
		DEFINE_MEMBER_N(bool, dead, 0x77);
		DEFINE_MEMBER_N(int32_t, health, 0x178);
		DEFINE_MEMBER_N(int32_t, max_health, 0x17C);
		DEFINE_MEMBER_N(int32_t, armor, 0x180);
		DEFINE_MEMBER_N(milli_t, quad_damage_time, 0x188); // time is in milliseconds (1000ms, 2000ms)
		DEFINE_MEMBER_N(int32_t, weapon_id, 0x18C);
		DEFINE_MEMBER_N(milli_t, firerate, 0x190);
		DEFINE_MEMBER_N(int, ammo[MAX_WEAPON_COUNT], 0x198); 
		DEFINE_MEMBER_N(char, name[MAX_NAME_LENGTH], 0x274); 
		DEFINE_MEMBER_N(char, team[MAX_TEAM_LENGTH], 0x378);
	};

public:
	vec_t origin() { auto origin = this->position; origin.z -= PLAYER_HEIGHT; return origin; }
	bool is_valid() { return this != nullptr && this->health && !this->dead; }

};

class fpsentlist
{
private:
	fpsent* entities[MAX_PLAYER_COUNT];

public:
	fpsentlist() = default;

	fpsent* get_entity(int index)
	{
		return entities[index];
	}
};

