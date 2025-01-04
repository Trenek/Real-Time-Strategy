#ifndef MODEL_BUILDER
#define MODEL_BUILDER
#include <vulkan/vulkan.h>

struct instanceInfo {
    uint32_t textureIndex;
    mat4 modelMatrix;
};

struct instance {
    uint32_t textureIndex;
    vec3 pos;
    vec3 rotation;
    vec3 scale;

    vec3 velocity;
    vec3 acceleration;
    float maxVelocity;
};

struct Vertex;
struct Model;

struct ModelBuilder {
    const char *modelPath;

    uint32_t texturesQuantity;
    const char **texturesPath;

    const char *fragmentShader;

    uint32_t instanceCount;
    uint32_t *instanceTextureIndex;
    struct instance *instance;

    void (*loadModel)(const char *objectPath, struct Model *model, VkDevice device, VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);
};

struct ModelBuilder gltfLoader(struct ModelBuilder a);
struct ModelBuilder objLoader(struct ModelBuilder a);
#endif
