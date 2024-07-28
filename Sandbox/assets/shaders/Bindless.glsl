#type vertex
#version 450 core

layout(location = 0) in vec3  a_Position;
layout(location = 1) in vec4  a_Color;
layout(location = 2) in vec2  a_TexCoord;
layout(location = 3) in float a_TexIndex;

out vec4 v_Color;
out vec2 v_TexCoord;
out float v_TexIndex;

uniform mat4 u_ViewProjection;

void main() {
    v_Color = a_Color;
    v_TexCoord = a_TexCoord;
    v_TexIndex = a_TexIndex;
    gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
}

#type fragment
#version 450 core

#extension GL_ARB_bindless_texture : require

layout (location = 0) out vec4 color;

layout(binding = 0, std430) readonly buffer ssbo1 {
    sampler2D textures[];
};

in vec4 v_Color;
in vec2 v_TexCoord;
in float v_TexIndex;

void main() {
    color = texture(textures[int(v_TexIndex)], v_TexCoord) * v_Color;
    if (color.a == 0.0)
        discard;
}