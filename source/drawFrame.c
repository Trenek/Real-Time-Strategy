#include <stdio.h>
#include <string.h>
#include <time.h>

#include "VulkanTools.h"
#include "uniformBufferObject.h"

#include "MY_ASSERT.h"

void updateUniformBuffer(void *uniformBuffersMapped[], uint32_t currentImage, VkExtent2D swapChainExtent, vec3 cameraPos, [[maybe_unused]] vec3 center) {
    struct UniformBufferObject ubo;

    glm_look_rh_no(cameraPos, center, (vec3) { 0.0f, 0.0f, 1.0f }, ubo.view);

    glm_perspective(glm_rad(45.0f), swapChainExtent.width / (float) swapChainExtent.height, 0.1f, 10000.0f, ubo.proj);

    ubo.proj[1][1] *= -1;

    memcpy(uniformBuffersMapped[currentImage], &ubo, sizeof(ubo));
}

void updateModelUniformBuffer(uint32_t instanceCount, struct instance instance[instanceCount]) {
    for (uint32_t i = 0; i < instanceCount; i += 1) {
        instance[i].update(&instance[i]);
    }
}

void recordCommandBuffer(VkCommandBuffer commandBuffer, VkFramebuffer swapChainFramebuffer, VkExtent2D swapChainExtent, struct VulkanTools *vulkan, uint32_t currentFrame) {
    VkCommandBufferBeginInfo beginInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,
        .flags = 0,
        .pInheritanceInfo = NULL,
        .pNext = NULL
    };

    VkClearValue clearValues[] = {
        [0].color.float32 = {
            0.0f,
            0.0f,
            0.0f,
            1.0f
        },
        [1].depthStencil = {
            .depth = 1.0f,
            .stencil = 0
        }
    };

    VkRenderPassBeginInfo renderPassInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = vulkan->renderPass,
        .framebuffer = swapChainFramebuffer,
        .renderArea = {
            .offset = {
                .x = 0,
                .y = 0
            },
            .extent = swapChainExtent
        },
        .clearValueCount = sizeof(clearValues) / sizeof(VkClearValue),
        .pClearValues = clearValues
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

    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    for (uint32_t i = 0; i < vulkan->modelQuantity; i += 1) {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->model[i].graphicsPipeline);

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, vulkan->model[i].pipelineLayout, 0, 1, &vulkan->model[i].descriptorSets[currentFrame], 0, NULL);
        for (uint32_t j = 0; j < vulkan->model[i].meshQuantity; j += 1) {
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, &vulkan->model[i].mesh[j].vertexBuffer, (VkDeviceSize[]){ 0 });
            vkCmdBindIndexBuffer(commandBuffer, vulkan->model[i].mesh[j].indexBuffer, 0, VK_INDEX_TYPE_UINT16);

            vkCmdDrawIndexed(commandBuffer, vulkan->model[i].mesh[j].indicesQuantity, vulkan->model[i].instanceCount, 0, 0, 0);
        }
    }
    vkCmdEndRenderPass(commandBuffer);
    MY_ASSERT(VK_SUCCESS == vkEndCommandBuffer(commandBuffer));
}

VkResult drawFrame(struct VulkanTools *vulkan) {
    VkResult result = VK_TRUE;

    uint32_t imageIndex = 0;
    static uint32_t currentFrame = 0;

    VkSemaphore waitSemaphores[] = {
        vulkan->imageAvailableSemaphore[currentFrame]
    };

    VkPipelineStageFlags waitStages[] = {
        VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
    };

    VkSemaphore signalSemaphores[] = {
        vulkan->renderFinishedSemaphore[currentFrame]
    };

    VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = sizeof(waitSemaphores) / sizeof(VkSemaphore),
        .pWaitSemaphores = waitSemaphores,
        .pWaitDstStageMask = waitStages,
        .commandBufferCount = 1,
        .pCommandBuffers = &vulkan->commandBuffer[currentFrame],
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

    vkWaitForFences(vulkan->device, 1, &vulkan->inFlightFence[currentFrame], VK_TRUE, UINT64_MAX);

    result = vkAcquireNextImageKHR(vulkan->device, vulkan->swapChain.this, UINT64_MAX, vulkan->imageAvailableSemaphore[currentFrame], VK_NULL_HANDLE, &imageIndex);
    if (VK_SUCCESS == result) {
        updateUniformBuffer(vulkan->uniformBuffersMapped, currentFrame, vulkan->swapChain.extent, vulkan->cameraPos, vulkan->center);
        for (uint32_t i = 0; i < vulkan->modelQuantity; i += 1) {
            updateModelUniformBuffer(vulkan->model[i].instanceCount, vulkan->model[i].instance);
            memcpy(vulkan->model[i].uniformModelBuffersMapped[currentFrame], vulkan->model[i].localUniformModelBuffersMapped, sizeof(struct instanceInfo) * vulkan->model[i].instanceCount);
        }

        vkResetFences(vulkan->device, 1, &vulkan->inFlightFence[currentFrame]);

        vkResetCommandBuffer(vulkan->commandBuffer[currentFrame], 0);
        recordCommandBuffer(vulkan->commandBuffer[currentFrame], vulkan->swapChainFramebuffers[imageIndex], vulkan->swapChain.extent, vulkan, currentFrame);

        MY_ASSERT(VK_SUCCESS == vkQueueSubmit(vulkan->graphicsQueue, 1, &submitInfo, vulkan->inFlightFence[currentFrame]));
        result = vkQueuePresentKHR(vulkan->presentQueue, &presentInfo);

        if (VK_SUCCESS == result) {
            currentFrame += 1;
            currentFrame %= MAX_FRAMES_IN_FLIGHT;
        }
    }

    return result;
}
