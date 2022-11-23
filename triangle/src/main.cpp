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

	
	return output;
}
DWORD cleanup(HMODULE module, FILE* output)
{
	fclose(output);
	FreeConsole();
	FreeLibraryAndExitThread(module, 0);

	return 0;
}


DWORD WINAPI entry_point(HMODULE module)
{
	auto output{ startup() };

	if (!globals::init())
		globals::init();

	printf("[+] initalized globals\n");

	hooks::init();
	printf("[+] initalized hooks\n");

	while (!GetAsyncKeyState(VK_END) & 1)
	{
		
		Sleep(50);
	}

	// hope it works..
	memory::untramphook(hooks::swapbuffers);
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));

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
		if (thread) {
			CloseHandle(thread);
		}
	}
	return TRUE; 
}


