#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out flat uint fragTexIndex;
layout(location = 3) out flat uint shadow;

struct ObjectData {
    uint index;
    mat4 model;
    bool shadow;
};

layout(std140, binding = 2) readonly buffer ObjectBuffer{
	ObjectData objects[];
} instance;

layout(std140, binding = 3) readonly buffer MeshBuffer{
	mat4 localModel[];
} mesh;

layout(push_constant) uniform constants {
	int meshID;
} PushConstants;

void main() {
    gl_Position = vec4(vec4(instance.objects[gl_InstanceIndex].model * vec4(inPosition, 1.0)).xy, 0.0, 1.0);
    fragColor = vec3(0.0, 0.0, 1.0);
    fragTexCoord = inTexCoord;
    fragTexIndex = instance.objects[gl_InstanceIndex].index;
    shadow = instance.objects[gl_InstanceIndex].shadow ? 1 : 0;
}
