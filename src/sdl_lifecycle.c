#include <assert.h>

#include "sdl_lifecycle.h"

void initialize_sdl(const char* window_title) {
    bool sdl_metadata_initialized = SDL_SetAppMetadata(window_title, "dev", "zone.pete.vpt");
    assert(sdl_metadata_initialized);

    bool sdl_video_init_result = SDL_InitSubSystem(SDL_INIT_VIDEO);
    assert(sdl_video_init_result);

    bool sdl_events_init_result = SDL_InitSubSystem(SDL_INIT_EVENTS);
    assert(sdl_events_init_result);
}

void cleanup_sdl_subsystems() {
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_QuitSubSystem(SDL_INIT_EVENTS);
}

void quit_sdl() {
    SDL_Quit();
}