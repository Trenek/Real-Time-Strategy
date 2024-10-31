#include <malloc.h>

#include "VulkanTools.h"

VkImageView *createImageViews(VkDevice device, struct swapChain swapChain) {
    VkImageView *swapChainImageViews = malloc(sizeof(VkImageView) * swapChain.imagesCount);
    uint32_t i = 0;

    while (i < swapChain.imagesCount) {
        VkImageViewCreateInfo createInfo = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = swapChain.images[i],
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = swapChain.imageFormat,
            .components = {
                .r = VK_COMPONENT_SWIZZLE_IDENTITY,
                .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                .a = VK_COMPONENT_SWIZZLE_IDENTITY
            },
            .subresourceRange = {
                .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                .baseMipLevel = 0,
                .levelCount = 1,
                .baseArrayLayer = 0,
                .layerCount = 1
            }
        };

        MY_ASSERT(VK_SUCCESS == vkCreateImageView(device, &createInfo, NULL, &swapChainImageViews[i]));

        i += 1;
    }

    return swapChainImageViews;
}

void destroyImageViews(VkImageView *swapChainImageViews, uint32_t swapChainImagesCount, VkDevice device) {
    uint32_t i = 0;

    while (i < swapChainImagesCount) {
        vkDestroyImageView(device, swapChainImageViews[i], NULL);

        i += 1;
    }

    free(swapChainImageViews);
}
