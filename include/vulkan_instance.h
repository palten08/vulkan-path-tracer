#pragma once

#include <vulkan/vulkan.h>

typedef struct VulkanInstance {
    VkInstance  handle;
    VkDebugUtilsMessengerEXT    debug_messenger;
    PFN_vkCreateDebugUtilsMessengerEXT  vkCreateDebugUtilsMessengerEXT;
    PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
} VulkanInstance;

VkResult vulkan_instance_create(VulkanInstance* output_instance, const char* application_name);
void vulkan_instance_destroy(VulkanInstance* vulkan_instance);