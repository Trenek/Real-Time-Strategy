#version 450

#extension GL_EXT_nonuniform_qualifier : enable

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in flat uint fragTexIndex;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler[1];

void main() {
    outColor = vec4(texture(texSampler[nonuniformEXT(fragTexIndex)], fragTexCoord).xyz * fragColor, 1.0);
}
