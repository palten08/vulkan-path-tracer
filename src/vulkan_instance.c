#include <stdio.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <assert.h>

#include "vulkan_instance.h"

static void vulkan_instance_load_functions(VulkanInstance* vulkan_instance) {
    #define LOAD(name)                                                      \
        do {                                                                \
            vulkan_instance->name = (PFN_##name)                            \
                vkGetInstanceProcAddr(vulkan_instance->handle, #name);      \
            assert(vulkan_instance->name != NULL);                          \
        } while (0)
    LOAD(vkCreateDebugUtilsMessengerEXT);
    LOAD(vkDestroyDebugUtilsMessengerEXT);
    #undef LOAD
}

static VKAPI_ATTR VkBool32 VKAPI_CALL debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT        type,
    const VkDebugUtilsMessengerCallbackDataEXT* data,
    void* user_data)
{
    fprintf(stderr, "[Vulkan] %s\n", data->pMessage);
    return VK_FALSE;
}

VkResult vulkan_instance_create(VulkanInstance* output_instance, const char* application_name) {
    const char* vulkan_layer_names[] = { "VK_LAYER_KHRONOS_validation" };
    const char* vulkan_extension_names[] = { VK_EXT_DEBUG_UTILS_EXTENSION_NAME };

    uint32_t sdl_extension_count = 0;
    const char* const* sdl_extensions = SDL_Vulkan_GetInstanceExtensions(&sdl_extension_count);

    uint32_t vulkan_extension_count = sizeof(vulkan_extension_names) / sizeof(vulkan_extension_names[0]);

    const char** combined_extension_names = malloc(sizeof(char*) * (sdl_extension_count + vulkan_extension_count));

    memcpy(combined_extension_names, sdl_extensions, sdl_extension_count * sizeof(*combined_extension_names));
    memcpy(combined_extension_names + sdl_extension_count, vulkan_extension_names, vulkan_extension_count * sizeof(*combined_extension_names));

    uint32_t total_extension_count = sdl_extension_count + vulkan_extension_count;

    VkApplicationInfo vulkan_app_info = {0};
    vulkan_app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    vulkan_app_info.pApplicationName = application_name;
    vulkan_app_info.applicationVersion = VK_MAKE_VERSION(0, 0, 1);
    vulkan_app_info.engineVersion = VK_MAKE_VERSION(0, 0, 1);
    vulkan_app_info.pEngineName = "No Engine";
    vulkan_app_info.apiVersion = VK_API_VERSION_1_4;

    VkInstanceCreateInfo vulkan_create_info = {0};
    vulkan_create_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    vulkan_create_info.pApplicationInfo = &vulkan_app_info;
    vulkan_create_info.enabledExtensionCount = total_extension_count;
    vulkan_create_info.ppEnabledExtensionNames = combined_extension_names;
    vulkan_create_info.enabledLayerCount = sizeof(vulkan_layer_names) / sizeof(vulkan_layer_names[0]);
    vulkan_create_info.ppEnabledLayerNames = vulkan_layer_names;

    VkDebugUtilsMessengerCreateInfoEXT vulkan_debug_callback_extension_create_info = {0};
    vulkan_debug_callback_extension_create_info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    vulkan_debug_callback_extension_create_info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    vulkan_debug_callback_extension_create_info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    vulkan_debug_callback_extension_create_info.pfnUserCallback = debug_callback;
    vulkan_debug_callback_extension_create_info.pUserData = NULL;

    vulkan_create_info.pNext = &vulkan_debug_callback_extension_create_info;


    VkResult vulkan_instance_creation_result = vkCreateInstance(&vulkan_create_info, NULL, &output_instance->handle);
    assert(vulkan_instance_creation_result == VK_SUCCESS);

    vulkan_instance_load_functions(output_instance);

    output_instance->vkCreateDebugUtilsMessengerEXT(output_instance->handle, &vulkan_debug_callback_extension_create_info, NULL, &output_instance->debug_messenger);
    assert(output_instance->debug_messenger != NULL);

    free(combined_extension_names);
    combined_extension_names = NULL;

    return vulkan_instance_creation_result;
}

void vulkan_instance_destroy(VulkanInstance* vulkan_instance) {
    vulkan_instance->vkDestroyDebugUtilsMessengerEXT(vulkan_instance->handle, vulkan_instance->debug_messenger, NULL);

    vkDestroyInstance(vulkan_instance->handle, NULL);
}