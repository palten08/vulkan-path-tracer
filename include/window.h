#pragma once

#include <SDL3/SDL.h>
#include "vulkan_instance.h"

typedef struct WindowResolution {
    uint32_t    width; // 4 bytes
    uint32_t    height; // 4 bytes
} WindowResolution;

typedef struct Window {
    SDL_Window* handle; // 8 bytes
    VkSurfaceKHR surface; // 8 bytes
    WindowResolution    window_resolution; // 8 bytes
    bool    resized; // 1 byte
    bool    close_requested; // 1 byte
} Window;

void initialize_window(Window* window, WindowResolution window_resolution, VkInstance vulkan_instance, const char* window_title);
void destroy_window(Window* window, VkInstance vulkan_instance);
void window_poll_events(Window* window);