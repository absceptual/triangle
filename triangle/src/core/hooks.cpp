#include "hooks.h"

void hooks::init()
{
    void* o_swapbuffers  = reinterpret_cast<swapbuffers_t>(GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers"));
    hooks::swapbuffers = memory::tramphook(o_swapbuffers, hooks::hk_swapbuffers, 15);
}


BOOL __stdcall hooks::hk_swapbuffers(HDC hdc)
{
    /*
    auto dc = wglGetCurrentDC();
    auto ctx = wglGetCurrentContext();

    if (dc != hdc)
    {
        wglMakeCurrent(dc, ctx);
    }
    */

    // Save old context and additionally create a new one if it is not already made
    const auto o_ctx = wglGetCurrentContext();
    const auto o_hdc = wglGetCurrentDC();
    const int index = GetPixelFormat(hdc);

    // Check if a context is present for the current device
    if (!contexts.count(index))
    {
        // Create a new context for the current device if we don't already have one
        auto ctx = wglCreateContext(hdc);
        wglMakeCurrent(hdc, ctx);


        wglMakeCurrent(o_hdc, o_ctx);
        hooks::contexts[index] = ctx;
    }
    wglMakeCurrent(hdc, hooks::contexts[index]);

    gl::setup();
    {
        esp::run();
    }
    gl::restore();
    wglMakeCurrent(o_hdc, o_ctx);

    return reinterpret_cast<swapbuffers_t>(hooks::swapbuffers->gateway)(hdc);
}


