#version 450 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aUV;
layout(location = 2) in uint aColor;

layout(push_constant) uniform uPushConstant {
        vec2 scale;
        vec2 translate;
} pc;

out gl_PerVertex {
        vec4 gl_Position;
};

layout(location = 0) out struct {
    vec4 Color;
    vec2 UV;
} Out;

void main()
{
    Out.Color = unpackUnorm4x8(aColor);
    Out.UV = aUV;
    gl_Position = vec4(aPos.x * pc.scale.x + pc.translate.x,-(aPos.y * pc.scale.y + pc.translate.y) , 0, 1);
}