#include <vulkan/vulkan.h>

#include "MY_ASSERT.h"

VkPipelineLayout createPipelineLayout(VkDevice device, VkDescriptorSetLayout descriptorSetLayout) {
    VkPipelineLayout pipelineLayout = NULL;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 1,
        .pSetLayouts = &descriptorSetLayout,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = NULL
    };

    MY_ASSERT(VK_SUCCESS == vkCreatePipelineLayout(device, &pipelineLayoutInfo, NULL, &pipelineLayout));

    return pipelineLayout;
}
