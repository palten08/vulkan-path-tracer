#pragma once

#include <vulkan/vulkan.h>

typedef struct VulkanDevice {
    VkPhysicalDevice    physical;
    VkDevice    logical;

    uint32_t    graphics_queue_family;
    VkQueue     graphics_queue;

    VkPhysicalDeviceRayTracingPipelinePropertiesKHR ray_tracing_pipeline_properties;
    VkPhysicalDeviceAccelerationStructurePropertiesKHR  acceleration_structure_properties;

    PFN_vkCreateAccelerationStructureKHR    vkCreateAccelerationStructureKHR;
    PFN_vkDestroyAccelerationStructureKHR   vkDestroyAccelerationStructureKHR;
    PFN_vkCmdBuildAccelerationStructuresKHR vkCmdBuildAccelerationStructuresKHR;
    PFN_vkGetAccelerationStructureBuildSizesKHR vkGetAccelerationStructureBuildSizesKHR;
    PFN_vkGetAccelerationStructureDeviceAddressKHR  vkGetAccelerationStructureDeviceAddressKHR;
    PFN_vkCreateRayTracingPipelinesKHR  vkCreateRayTracingPipelinesKHR;
    PFN_vkGetRayTracingShaderGroupHandlesKHR    vkGetRayTracingShaderGroupHandlesKHR;
    PFN_vkCmdTraceRaysKHR   vkCmdTraceRaysKHR;
} VulkanDevice;

void vulkan_device_create(VulkanDevice* output_device, VkInstance vulkan_instance, VkSurfaceKHR vulkan_surface);
void vulkan_device_destroy(VulkanDevice* vulkan_device);