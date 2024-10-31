#include <malloc.h>

#include "VulkanTools.h"

VkFramebuffer *createFramebuffers(VkDevice device, VkImageView *swapChainImageViews, uint32_t swapChainImagesCount, VkExtent2D swapChainExtent, VkRenderPass renderPass) {
    VkFramebuffer *swapChainFramebuffers = NULL;
    uint32_t i = 0;

    swapChainFramebuffers = malloc(sizeof(VkFramebuffer) * swapChainImagesCount);

    while (i < swapChainImagesCount) {
        VkImageView attachments[] = {
            swapChainImageViews[i]
        };

        VkFramebufferCreateInfo framebufferInfo = {
            .sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
            .renderPass = renderPass,
            .attachmentCount = 1,
            .pAttachments = attachments,
            .width = swapChainExtent.width,
            .height = swapChainExtent.height,
            .layers = 1
        };

        MY_ASSERT(VK_SUCCESS == vkCreateFramebuffer(device, &framebufferInfo, NULL, &swapChainFramebuffers[i]));

        i += 1;
    }

    return swapChainFramebuffers;
}

void destroyFramebuffers(VkDevice device, VkFramebuffer *swapChainFramebuffers, uint32_t swapChainImagesCount) {
    uint32_t i = 0;

    while (i < swapChainImagesCount) {
        vkDestroyFramebuffer(device, swapChainFramebuffers[i], NULL);

        i += 1;
    }

    free(swapChainFramebuffers);
}
