#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "vulkan_device.h"

static const char* vulkan_physical_device_type_as_string(VkPhysicalDeviceType vulkan_physical_device_type) {
    switch (vulkan_physical_device_type) {
        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
            return "Other";
        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            return "Discrete GPU";
        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            return "Integrated GPU";
        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            return "Virtual GPU";
        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            return "CPU";
        default:
            return "Unknown";
    }
}

void vulkan_device_create(VulkanDevice* output_device, VkInstance vulkan_instance, VkSurfaceKHR vulkan_surface) {
    uint32_t vulkan_device_count = 0;
    vkEnumeratePhysicalDevices(vulkan_instance, &vulkan_device_count, NULL);
    VkPhysicalDevice* vulkan_physical_devices = malloc(vulkan_device_count * sizeof(VkPhysicalDevice));
    VkResult physical_devices_enumeration_result = vkEnumeratePhysicalDevices(vulkan_instance, &vulkan_device_count, vulkan_physical_devices);
    assert(physical_devices_enumeration_result == VK_SUCCESS);

    for (uint32_t i = 0; i < vulkan_device_count; i++) {
        VkPhysicalDeviceProperties vulkan_device_properties = {0};
        vkGetPhysicalDeviceProperties(vulkan_physical_devices[i], &vulkan_device_properties);

        uint32_t vulkan_version_major = VK_VERSION_MAJOR(vulkan_device_properties.apiVersion);
        uint32_t vulkan_version_minor = VK_VERSION_MINOR(vulkan_device_properties.apiVersion);
        uint32_t vulkan_version_patch = VK_VERSION_PATCH(vulkan_device_properties.apiVersion);

        uint32_t driver_version_major = VK_VERSION_MAJOR(vulkan_device_properties.driverVersion);
        uint32_t driver_version_minor = VK_VERSION_MINOR(vulkan_device_properties.driverVersion);
        uint32_t driver_version_patch = VK_VERSION_PATCH(vulkan_device_properties.driverVersion);

        printf("Device ID:  \t%d\n", vulkan_device_properties.deviceID);
        printf("Device Name:    \t%s\n", vulkan_device_properties.deviceName);
        printf("Device Type:    \t%s\n", vulkan_physical_device_type_as_string(vulkan_device_properties.deviceType));
        printf("Vendor ID:  \t%d\n", vulkan_device_properties.vendorID);
        printf("API Version:    \tv%d.%d.%d\n", vulkan_version_major, vulkan_version_minor, vulkan_version_patch);
        printf("Driver Version: \tv%d.%d.%d\n", driver_version_major, driver_version_minor, driver_version_patch);
    }
}

void vulkan_device_destroy(VulkanDevice* vulkan_device) {}