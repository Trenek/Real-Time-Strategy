#include <cglm.h>

struct instanceBuffer {
    uint32_t textureIndex;
    mat4 modelMatrix;
    bool shadow;
};

struct instance {
    uint32_t textureIndex;
    vec3 pos;
    vec3 rotation;
    vec3 scale;
    bool shadow;

    vec3 velocity;
    vec3 acceleration;
    float maxVelocity;
};
