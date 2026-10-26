// IW7 PS bfc071e2 flow/HDR sampling on Replay's native effect-quad interface.
// The native f9c07ff5 VS supplies squared particle colour and separate emissive intensity.
cbuffer Stable : register(b2) { float4 stable[9]; }
cbuffer Material : register(b3) { float4 material[4]; }
cbuffer Scene : register(b7) { float4 scene[95]; }
Texture2DArray<float4> effectFog : register(t1);
Texture2D<float4> colorMap : register(t2);
Texture2D<float4> scopeBuffer : register(t4);
Texture2D<float4> flowMap : register(t6);
Texture2D<float4> flowSurface : register(t7);
Texture3D<float4> volumetricVisibility : register(t111);
Texture2D<float4> volumetricDepth : register(t112);
SamplerState colorSampler : register(s3);
SamplerState screenSampler : register(s8);

struct Input
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
    centroid float2 lightingUV : TEXCOORD5;
    float4 emissive : TEXCOORD8;
};
struct Output
{
    float4 color : SV_TARGET0;
    float4 opacity : SV_TARGET1;
};

Output main(Input input)
{
    float4 flow = flowMap.SampleLevel(colorSampler, input.uv, 0);
    float4 surface = flowSurface.SampleLevel(colorSampler, input.uv, 0);
    float2 velocity = (flow.xy * 2 - 1) * flow.z * material[1].x;
    float period = material[2].x;
    float halfPeriod = material[2].z;
    // Source DXBC uses signed frac, equivalent to fmod, in both wrapping stages.
    float phaseA = fmod(fmod(stable[8].w, period) + surface.a, 2 * halfPeriod);
    float phaseB = fmod(fmod(stable[8].w + halfPeriod, period) + surface.a, 2 * halfPeriod);
    float weight = saturate(abs(halfPeriod - phaseA) / halfPeriod);
    float4 a = colorMap.SampleBias(colorSampler, input.uv - velocity * phaseA, stable[7].y);
    float4 b = colorMap.SampleBias(colorSampler, input.uv - velocity * phaseB, stable[7].y);
    float4 texel = lerp(a, b, weight);
    float alpha = texel.a * input.color.a * surface.r;
    if (alpha <= 0.000061)
        discard;
    // Replay supplies the particle RGB squared. IW7 squares (texel * particleColour) before mkhdr.
    float3 linearColor = texel.rgb * texel.rgb * input.color.rgb;
    float3 hdr = material[3].x != 0 ? -log2(1 - min(linearColor, 0.99)) : linearColor;
    float3 color = hdr * material[0].rgb * input.emissive.rgb * input.emissive.w;

    // Replay b0ec5d88: the native effect fog map and volumetric-visibility lookup, with the same bindings.
    color *= effectFog.Sample(screenSampler, float3(input.lightingUV, 1)).rgb;
    float2 screenUV = input.position.xy * stable[7].x * stable[6].zw;
    float depth = input.position.z > 0.984375 ? -4 : input.position.w - 4;
    float4 slices;
    slices.x = depth;
    slices.y = slices.x - scene[48].x;
    slices.z = slices.y - scene[48].y;
    slices.w = slices.z - scene[48].z;
    float slice = dot(saturate(slices * scene[49]), float4(0.25, 0.25, 0.25, 0.25));
    float4 depths = volumetricDepth.GatherRed(screenSampler, screenUV);
    float packed = min(min(depths.x, depths.y), min(depths.z, depths.w));
    uint packedSlice = (uint)(packed * 255 + 0.5);
    float limit = ((float)packedSlice - 0.500061) / 128;
    color *= volumetricVisibility.SampleLevel(screenSampler, float3(screenUV, min(slice, limit)), 0).r;
    color *= scene[42].x;
    bool thermal = asint(scene[13].x) == 1 ||
        (asint(scene[13].x) == 2 && scopeBuffer.SampleLevel(screenSampler, screenUV, 0).r > 0);
    if (thermal)
        color = float3(dot(color, float3(0.2126, 0.7152, 0.0722)), 0, 0);
    Output output;
    output.color = min(float4(color * alpha, alpha), 32255);
    output.opacity = alpha.xxxx;
    return output;
}
