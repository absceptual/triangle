#include "hooks.h"




bool hooks::init()
{
    void* o_swapbuffers{ reinterpret_cast<swapbuffers_t>(GetProcAddress(GetModuleHandle(L"opengl32.dll"), "wglSwapBuffers")) };
    hooks::swapbuffers = memory::tramphook(o_swapbuffers, hooks::hk_swapbuffers, 15);
    if (!hooks::swapbuffers)
        return false;

    void* o_raycast = reinterpret_cast<raycast_t>(globals::base + 0x1DB2A0);
    hooks::raycast = memory::tramphook(o_raycast, hooks::hk_raycast, 15);
    if (!hooks::raycast)
        return false;

    return true;
}

fpsent* __fastcall hooks::hk_raycast(const vec_t* from, const vec_t* to, fpsent* entity, float* b_dist)
{
    // Check if silent aim is enabled and our local player is calling the function
    if (globals::silent_enabled && entity == globals::lplayer)
    {
        auto target = aimbot::get_closest_screen_entity(true);
        if (target)
        {
            const auto origin = entity->origin();
            return reinterpret_cast<raycast_t>(raycast->gateway)(
                   &origin,
                   &target->position,
                   entity,
                   b_dist
            );
            return target;
        } 
    }
    
    return reinterpret_cast<raycast_t>(raycast->gateway)(from, to, entity, b_dist);
}

BOOL __stdcall hooks::hk_swapbuffers(HDC hdc)
{
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
    

    if (esp::font.hdc != hdc || !esp::font.bBuilt)
        esp::font.Build(esp::font.height);


    gl::setup();
    hooks::run();
    gl::restore();

    wglMakeCurrent(o_hdc, o_ctx);
    return reinterpret_cast<swapbuffers_t>(hooks::swapbuffers->gateway)(hdc);
}

void hooks::run()
{
    if (GetAsyncKeyState(VK_F5) & 1)
    {
        settings::exploits::recoil = !settings::exploits::recoil;
        exploits::push_back();
    }

    if (GetAsyncKeyState(VK_F6) & 1)
        settings::exploits::infinite_ammo = !settings::exploits::infinite_ammo;

    if (GetAsyncKeyState(VK_F7) & 1)
        settings::exploits::infinite_jump = !settings::exploits::infinite_jump;

    if (GetAsyncKeyState(VK_DOWN) & 1)
        aimbot::radius -= 50;
    else if (GetAsyncKeyState(VK_UP) & 1)
        aimbot::radius += 50;

    if (GetAsyncKeyState(VK_SPACE) & 1)
        exploits::double_jump();

    exploits::infinite_ammo();

    esp::run();
    esp::draw_fov(aimbot::radius);

  
   
}
