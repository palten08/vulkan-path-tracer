#include <stdio.h>

#include "app.h"
#include "window.h"
#include "vulkan_instance.h"
#include "sdl_lifecycle.h"

int main(int argc, char** argv) {
    Window app_window = {0};
    VulkanInstance vulkan_instance = {0};
    VkDebugUtilsMessengerEXT vulkan_debug_messenger = VK_NULL_HANDLE;

    const char* application_name = "Kool Path Tracer";

    initialize_sdl(application_name);
    vulkan_instance_create(&vulkan_instance, application_name);
    initialize_window(&app_window, (WindowResolution){800,600}, vulkan_instance.handle, application_name);

    puts("Vulkan instance created");

    while (!app_window.close_requested) {
        window_poll_events(&app_window);
    }

    destroy_window(&app_window, vulkan_instance.handle);
    vulkan_instance_destroy(&vulkan_instance);
    cleanup_sdl_subsystems();
    quit_sdl();
}