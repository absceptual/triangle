#include "mem.h"

void memory::patch(void* target, void* data, const size_t size)
{
	// Override existing protection
	DWORD o_protection{ 0x0 };
	VirtualProtect(target, size, PAGE_EXECUTE_READWRITE, &o_protection);

	// Overwrite bytes and restore old protection
	memcpy(target, data, size);
	VirtualProtect(target, size, o_protection, &o_protection);
}

void memory::nop(void* target, const size_t size)
{
	// Set our array to 0x90 (NOP) and patch our destination
	auto nops{ std::make_unique<uint8_t[]>(size) };
	memset(nops.get(), 0x90, size);

	memory::patch(nops.get(), target, size);
}

bool memory::detour(void* target, void* code, const size_t size)
{
	// Bytecode uses 12 bytes
	if (size < 12)
		return false;

	// Override original protection
	DWORD o_protection{ 0x0 };
	VirtualProtect(target, size, PAGE_EXECUTE_READWRITE, &o_protection);

	// Nop any stray bytes
	memset(target, 0x90, size);

	const uint8_t bytecode[] = {
		0x48, 0xb8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // movabs rax, offset
		0xff, 0xe0 // jmp rax (absolute)
	};
	memcpy(target, bytecode, sizeof(bytecode));

	// Write our offset to the bytecode instead of doing some weird gay calculations
	*reinterpret_cast<uint64_t*>(uintptr_t(target) + 0x2) = uintptr_t(code);

	VirtualProtect(target, size, o_protection, &o_protection);
	return true;
}

memory::trampoline_data* memory::tramphook(void* target, void* code, const size_t size)
{
	auto tramp = new memory::trampoline_data{ };

	// Bytecode uses 12 bytes
	if (size < 12)
		return tramp;

	// Setup our trampoline data that'll be used for unhooking
	tramp->address = target;

	auto gateway = reinterpret_cast<uintptr_t*>(VirtualAlloc(NULL, size + 12, MEM_RESERVE | MEM_COMMIT, PAGE_EXECUTE_READWRITE));
	if (!gateway)
		return tramp;

	// Override original protection (12 bytes needed for bytecode)
	DWORD o_protection{ 0x0 };
	VirtualProtect(target, size + 12, PAGE_EXECUTE_READWRITE, &o_protection);

	// Copy our bytes to our gateway & trampoline data
	tramp->size = size;
	tramp->o_bytes = std::make_unique<uint8_t[]>(size);
	memcpy(gateway, target, size);
	memcpy(tramp->o_bytes.get(), target, size);
	

	const uint8_t bytecode[] = {
		0x48, 0xb8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // movabs rax, offset
		0xff, 0xe0 // jmp rax (absolute)
	};
	memcpy(reinterpret_cast<void*>((uintptr_t(gateway) + size)), bytecode, sizeof(bytecode));

	// Write our offset to the bytecode instead of doing some weird gay calculations
	*reinterpret_cast<uint64_t*>((uintptr_t(gateway) + size) + 0x2) = (uintptr_t(target) + size);
	
	tramp->gateway = gateway;

	memory::detour(target, code, size);
	return tramp;
}

void memory::untramphook(memory::trampoline_data* data)
{
	// Attempt to restore original bytes of the hooked function
	DWORD o_protection{ 0x0 };
	VirtualProtect(data->address, data->size, PAGE_EXECUTE_READWRITE, &o_protection);

	memcpy(data->address, data->o_bytes.get(), data->size);

	// Optionally deallocate our gateway (size of stolen bytes + bytecode) and delete our trampoline_data
	VirtualAlloc(data->gateway, data->size + 12, MEM_RESET, PAGE_NOACCESS);
	delete data;

}
