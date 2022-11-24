// C++ header files
#include <iostream>
#include <chrono>
#include <thread>

// Windows and other related files
#include <Windows.h>

#include "core/globals.h"
#include "core/hooks.h"



auto startup()
{
	AllocConsole();
	FILE* output;
	freopen_s(&output, "CONOUT$", "w", stdout);

	if (!globals::init())
		globals::init();

	return output;
}

DWORD cleanup(HMODULE module, FILE* output)
{
	fclose(output);
	FreeConsole();
	FreeLibraryAndExitThread(module, 0);

	// Unhook any existing trampoline hooks and restore any byte patching (unsafe!)

	// FIX DEHOOKING
	memory::untramphook(hooks::swapbuffers);
	memory::untramphook(hooks::raycast);
	memory::unpatch();

	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	return 0;
}

DWORD WINAPI entry_point(HMODULE module)
{
	auto output{ startup() };
	printf("[+] initalized globals\n");

	if (!hooks::init())
	{
		printf("[-] trampoline hooks could not be placed, exiting in 5 seconds...");
		std::this_thread::sleep_for(std::chrono::milliseconds(5000));

		return cleanup(module, output);
	}
	
	while (!GetAsyncKeyState(VK_END) & 1)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	return cleanup(module, output);
}

DWORD WINAPI DllMain(HINSTANCE module,
	DWORD reason,
	void* reserved
)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		const auto thread = CreateThread(nullptr, 0x0, reinterpret_cast<LPTHREAD_START_ROUTINE>(entry_point), module, 0x0, nullptr);
		if (thread) 
		{
			DisableThreadLibraryCalls(module);
			CloseHandle(thread);
		}
	}
	return TRUE; 
}


