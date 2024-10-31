#include "VulkanTools.h"

void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex, VkFramebuffer *swapChainFramebuffers, VkExtent2D swapChainExtent, VkRenderPass renderPass, VkPipeline graphicsPipeline) {
    VkCommandBufferBeginInfo beginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = 0,
        .pInheritanceInfo = NULL,
        .pNext = NULL
    };

    VkClearValue clearColor = {
        .color = {
            .float32 = {
                0.0f,
                0.0f,
                0.0f,
                1.0f
            }
        }
    };

    VkRenderPassBeginInfo renderPassInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = renderPass,
        .framebuffer = swapChainFramebuffers[imageIndex],
        .renderArea = {
            .offset = { 
                .x = 0,
                .y = 0
            },
            .extent = swapChainExtent
        },
        .clearValueCount = 1,
        .pClearValues = &clearColor
    };

    VkViewport viewport = {
        .x = 0.0f,
        .y = 0.0f,
        .width = (float)swapChainExtent.width,
        .height = (float)swapChainExtent.height,
        .minDepth = 0.0f,
        .maxDepth = 1.0f
    };

    VkRect2D scissor = {
        .offset = { 0, 0 },
        .extent = swapChainExtent
    };

    MY_ASSERT(VK_SUCCESS == vkBeginCommandBuffer(commandBuffer, &beginInfo));

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);

    vkCmdDraw(commandBuffer, 3, 1, 0, 0);
    vkCmdEndRenderPass(commandBuffer);

    MY_ASSERT(VK_SUCCESS == vkEndCommandBuffer(commandBuffer));
}

void drawFrame(struct VulkanTools *vulkan) {
    uint32_t imageIndex = 0;

    VkSemaphore waitSemaphores[] = {
        vulkan->imageAvailableSemaphore
    };

    VkPipelineStageFlags waitStages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };

    VkSemaphore signalSemaphores[] = {
        vulkan->renderFinishedSemaphore
    };

    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = sizeof(waitSemaphores) / sizeof(VkSemaphore),
        .pWaitSemaphores = waitSemaphores,
        .pWaitDstStageMask = waitStages,
        .commandBufferCount = 1,
        .pCommandBuffers = &vulkan->commandBuffer,
        .signalSemaphoreCount = sizeof(signalSemaphores) / sizeof(VkSemaphore),
        .pSignalSemaphores = signalSemaphores
    };

    VkSwapchainKHR swapChains[] = {
        vulkan->swapChain.this
    };

    VkPresentInfoKHR presentInfo = {
        .sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = sizeof(waitSemaphores) / sizeof(VkSemaphore),
        .pWaitSemaphores = signalSemaphores,
        .swapchainCount = sizeof(swapChains) / sizeof(VkSwapchainKHR),
        .pSwapchains = swapChains,
        .pImageIndices = &imageIndex,
        .pResults = NULL // optional
    };

    vkWaitForFences(vulkan->device, 1, &vulkan->inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(vulkan->device, 1, &vulkan->inFlightFence);

    vkAcquireNextImageKHR(vulkan->device, vulkan->swapChain.this, UINT64_MAX, vulkan->imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
    vkResetCommandBuffer(vulkan->commandBuffer, 0);
    recordCommandBuffer(vulkan->commandBuffer, imageIndex, vulkan->swapChainFramebuffers, vulkan->swapChain.extent, vulkan->renderPass, vulkan->graphicsPipeline);

    MY_ASSERT(VK_SUCCESS == vkQueueSubmit(vulkan->graphicsQueue, 1, &submitInfo, vulkan->inFlightFence));
    vkQueuePresentKHR(vulkan->presentQueue, &presentInfo);
}
