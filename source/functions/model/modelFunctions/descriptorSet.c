#include <vulkan/vulkan.h>

#include "MY_ASSERT.h"
#include "model.h"
#include "definitions.h"

#include "uniformBufferObject.h"

void fillArray(int num, VkDescriptorSetLayout layouts[], VkDescriptorSetLayout elem) {
    while (num --> 0) {
        layouts[num] = elem;
    }
}

void createDescriptorSets(VkDescriptorSet descriptorSets[], VkDevice device, VkDescriptorPool descriptorPool, VkBuffer uniformBuffers[], VkDescriptorSetLayout descriptorSetLayout, struct Model model) {
    VkDescriptorSetLayout layouts[MAX_FRAMES_IN_FLIGHT]; {
        fillArray(MAX_FRAMES_IN_FLIGHT, layouts, descriptorSetLayout);
    };
    VkDescriptorSetAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = descriptorPool,
        .descriptorSetCount = MAX_FRAMES_IN_FLIGHT,
        .pSetLayouts = layouts
    };

    MY_ASSERT(VK_SUCCESS == vkAllocateDescriptorSets(device, &allocInfo, descriptorSets));

    VkDescriptorImageInfo imageInfoArray[model.texturesQuantity];
    for (uint32_t i = 0; i < model.texturesQuantity; i += 1) {
        imageInfoArray[i] = (VkDescriptorImageInfo) {
            .imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            .imageView = model.texture[i].imageView,
            .sampler = model.texture[i].sampler
        };
    }

    for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i += 1) {
        VkDescriptorBufferInfo bufferInfo = {
            .buffer = uniformBuffers[i],
            .offset = 0,
            .range = sizeof(struct UniformBufferObject)
        };

        VkDescriptorBufferInfo modelBufferInfo = {
            .buffer = model.uniformModelBuffers[i],
            .offset = 0,
            .range = model.instanceCount * sizeof(struct instanceInfo)
        };

        VkWriteDescriptorSet descriptorWrites[] = {
            [0] = {
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .dstSet = descriptorSets[i],
                .dstBinding = 0,
                .dstArrayElement = 0,
                .descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                .descriptorCount = 1,
                .pBufferInfo = &bufferInfo,
                .pTexelBufferView = NULL
            },
            [1] = {
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .dstSet = descriptorSets[i],
                .dstBinding = 1,
                .dstArrayElement = 0,
                .descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
                .descriptorCount = sizeof(imageInfoArray) / sizeof(VkDescriptorImageInfo),
                .pImageInfo = imageInfoArray,
                .pTexelBufferView = NULL
            },
            [2] = {
                .sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET,
                .dstSet = descriptorSets[i],
                .dstBinding = 2,
                .dstArrayElement = 0,
                .descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                .descriptorCount = 1,
                .pBufferInfo = &modelBufferInfo,
                .pTexelBufferView = NULL
            }
        };

        vkUpdateDescriptorSets(device, sizeof(descriptorWrites) / sizeof(VkWriteDescriptorSet), descriptorWrites, 0, NULL);
    }
}
