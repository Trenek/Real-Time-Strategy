#ifndef MODEL_BUILDER
#define MODEL_BUILDER

struct instanceInfo {
    uint32_t textureIndex;
    mat4 modelMatrix;
};

struct instance {
    struct instanceInfo *instanceInfo;
    void (*update)(struct instance *instance);
    vec3 pos;
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
    void (**update)(struct instance *instance);
    vec3 *pos;

    void (*loadModel)(const char *objectPath, struct Model *model);
};

struct ModelBuilder gltfLoader(struct ModelBuilder a);
struct ModelBuilder objLoader(struct ModelBuilder a);
#endif
