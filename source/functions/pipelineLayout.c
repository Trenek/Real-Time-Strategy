#include "VulkanTools.h"

VkPipelineLayout createPipelineLayout(VkDevice device) {
    VkPipelineLayout pipelineLayout = NULL;

    VkPipelineLayoutCreateInfo pipelineLayoutInfo = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = 0,
        .pSetLayouts = NULL,
        .pushConstantRangeCount = 0,
        .pPushConstantRanges = NULL
    };

    MY_ASSERT(VK_SUCCESS == vkCreatePipelineLayout(device, &pipelineLayoutInfo, NULL, &pipelineLayout));

    return pipelineLayout;
}
