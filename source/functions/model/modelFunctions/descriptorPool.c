#include <vulkan/vulkan.h>

#include "MY_ASSERT.h"
#include "definitions.h"

VkDescriptorPool createDescriptorPool(uint32_t texturesCount, VkDevice device) {
    VkDescriptorPool descriptorPool = NULL;

    VkDescriptorPoolSize poolSize[] = {
        [0] = {
            .type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
            .descriptorCount = MAX_FRAMES_IN_FLIGHT
        },
        [1] = {
            .type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
            .descriptorCount = texturesCount * MAX_FRAMES_IN_FLIGHT
        },
        [2] = {
            .type = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
            .descriptorCount = MAX_FRAMES_IN_FLIGHT
        }
    };

    VkDescriptorPoolCreateInfo poolInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .poolSizeCount = sizeof(poolSize) / sizeof(VkDescriptorPoolSize),
        .pPoolSizes = poolSize,
        .maxSets = MAX_FRAMES_IN_FLIGHT,
        .flags = VK_DESCRIPTOR_POOL_CREATE_UPDATE_AFTER_BIND_BIT
    };

    MY_ASSERT(VK_SUCCESS == vkCreateDescriptorPool(device, &poolInfo, NULL, &descriptorPool));

    return descriptorPool;
}
