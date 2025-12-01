#include "Common.hlsli"

Texture2D textures[] : register(t0);
cbuffer TexID : register(b3) { uint textureID; };
sampler textureSampler : register(s0);

float4 main(VertexOut vIn) : SV_Target
{
    return textures[textureID].Sample(textureSampler, vIn.uv);
}