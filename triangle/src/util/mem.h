#pragma once
#include <cstdint>
#include <memory>
#include <unordered_map>

#include <Windows.h>

namespace memory
{
	void patch(void* target, void* data, const size_t size);
	void nop(void* target, const size_t size);

	struct trampoline_data
	{
		std::unique_ptr<uint8_t[]> o_bytes{ nullptr };
		size_t size{ 0x0 };
		void* gateway{ nullptr };
		void* address{ nullptr };
	};

	bool detour(void* target, void* code, const size_t size);
	trampoline_data* tramphook(void* target, void* code, const size_t size);
	void untramphook(memory::trampoline_data* data);

	inline std::unordered_map<void*, trampoline_data*> trampolines; // Returns a trampoline data structure for whatever gateway is passed

	template <typename Return, typename... Arguments>
	constexpr Return call(void* vmt, const uint32_t index, Arguments... args)
	{
		using func_t = Return(__thiscall*)(void*, decltype(args)...);
		return (*static_cast<func_t**>(vmt))[index](vmt, args...);
	}
}
