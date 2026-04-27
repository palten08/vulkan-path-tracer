#include <assert.h>
#include <SDL3/SDL_vulkan.h>
#include "window.h"
#include "vulkan_instance.h"

void initialize_window(Window* window, WindowResolution window_resolution, VkInstance vulkan_instance, const char* window_title) {
    window->window_resolution = window_resolution;

    window->handle = SDL_CreateWindow(window_title, window->window_resolution.width, window->window_resolution.height, SDL_WINDOW_VULKAN);
    assert(window->handle != NULL);

    window->close_requested = false;

    bool vulkan_surface_creation_result = SDL_Vulkan_CreateSurface(window->handle, vulkan_instance, NULL, &window->surface);
    assert(vulkan_surface_creation_result);
}

void destroy_window(Window* window, VkInstance vulkan_instance) {
    if (window->surface) {
        SDL_Vulkan_DestroySurface(vulkan_instance, window->surface, NULL);
    }
    if (window->handle) {
        SDL_DestroyWindow(window->handle);
    }
}

void window_poll_events(Window* window) {
    SDL_Event sdl_event;
    while (SDL_PollEvent(&sdl_event)) {
        switch (sdl_event.type) {
            case SDL_EVENT_QUIT:
                window->close_requested = true;
                break;
            case SDL_EVENT_MOUSE_WHEEL:
                break;
        }
    }
}