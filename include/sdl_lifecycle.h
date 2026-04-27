#pragma once

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

void initialize_sdl(const char* window_title);
void cleanup_sdl_subsystems();
void quit_sdl();