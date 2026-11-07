// IW7 PS bfc071e2 flow/HDR sampling on Replay's native effect-quad interface; with FLOW_FEATHER, IW7 PS d384b91e
// (its zfeather twin without mkhdr: the same flow, plus the soft intersection with the scene).
// The native f9c07ff5 VS supplies squared particle colour and separate emissive intensity.
// FLOW_LIT: IW7 PS a386c911 (eq_effect_zfeather_blend_lin_ndw_flow_nocast_mask, the lit flow: the same flow and
// feather, an alpha mask, the particle lit by the scene) on the bindings of Replay's lit effect set (VS 372e0e1a,
// PS a9864c07): its per-particle lighting map at t0, the albedo at t2, the scene depth at t4, the scope buffer at t5
// and the Particle_MaterialData at t6; the flow maps and the mask follow at t7-t9. The unmasked lit flows (d29f9b4f,
// and d241aa2a without the feather) bind a white mask. FLOW_UVD: IW7's lit UV distortion (c258449e, 5d9b9a5b, and the
// twins without the dissolve 684ca3a1, 69656d4c) on the same bindings, its noise map in the flow map's slot.
// FLOW_ATEST: IW7's alpha-tested lit effects (c4e82cfb, c4b7c455; with FLOW_UVD 5e767f2f) on the same bindings, cut at
// 0.501961 and drawn opaque.
cbuffer Stable : register(b2) { float4 stable[9]; }
#ifdef FLOW_LIT
cbuffer Material : register(b3) { float4 material[5]; }
#else
cbuffer Material : register(b3) { float4 material[4]; }
#endif
cbuffer Scene : register(b7) { float4 scene[95]; }
Texture2DArray<float4> effectFog : register(t1);
Texture2D<float4> colorMap : register(t2);
Texture3D<float4> volumetricVisibility : register(t111);
Texture2D<float4> volumetricDepth : register(t112);
SamplerState colorSampler : register(s3);
SamplerState screenSampler : register(s8);
#ifdef FLOW_LIT
Texture2D<float4> particleLighting : register(t0);
Texture2D<float4> scopeBuffer : register(t5);
Texture2D<float4> flowMap : register(t7);
Texture2D<float4> flowSurface : register(t8);
Texture2D<float4> flowMask : register(t9);
Texture3D<float4> volumetricScatter : register(t110);
// Pixel rows 1, 2 and 9 are the lit set's (layerData1, absorptionScale, atlas): the flow constants follow.
#define FLOW_SCALE material[3].x
#define FLOW_PERIOD material[4]
#else
Texture2D<float4> scopeBuffer : register(t4);
Texture2D<float4> flowMap : register(t6);
Texture2D<float4> flowSurface : register(t7);
#define FLOW_SCALE material[1].x
#define FLOW_PERIOD material[2]
#endif
#ifdef FLOW_FEATHER
// The template's own bindings (stock effect PSs b0ec5d88 / a9864c07): the scene depth and each element's
// Particle_MaterialData (192 bytes, indexed by the particle's COLOR3.x).
struct ParticleMaterialData
{
    float4 rows[12];
};
#ifdef FLOW_LIT
Texture2D<float4> sceneDepth : register(t4);
StructuredBuffer<ParticleMaterialData> particleMaterialData : register(t6);
#else
Texture2D<float4> sceneDepth : register(t3);
StructuredBuffer<ParticleMaterialData> particleMaterialData : register(t5);
#endif
#endif

// The native VS's whole output signature, in its register order and with the interpolation the stock PS declares:
// a pixel shader that skips an element reads the next one's register (TEXCOORD3 is register 3).
struct Input
{
    float4 position : SV_POSITION;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
    float4 viewVector : TEXCOORD3;
    centroid float2 lightingUV : TEXCOORD5;
    float4 emissive : TEXCOORD8;
    nointerpolation float4 center : POSITION0;
    nointerpolation uint2 particle : COLOR3;
};
struct Output
{
    float4 color : SV_TARGET0;
    float4 opacity : SV_TARGET1;
};

Output main(Input input)
{
#ifdef FLOW_UVD
    // IW7 c258449e / 5d9b9a5b (eq_effect_blend_lin[_ct]_ndw_shl_uvd_ad_nocast_norm_spec): the UVs move by a scrolling noise
    // map (the flow map's slot), uv + (noise.xy * 2 - 1) * strength / atlas, the noise at uv * scale + time * scroll (row
    // 3: scroll xy, scale zw; row 4: strength, atlas yz, dissolve K w); the vertex alpha dissolves the alpha instead of
    // scaling it, alpha = saturate((texel.a + vertex alpha) * K - K). Their twins without the dissolve (K 0: 684ca3a1,
    // eq_effect_blend_lin_ndw_shl_uvd_nocast_norm_spec; 69656d4c, eq_effect_blend_lin_ndw_uvd_nocast) scale it,
    // alpha = texel.a * vertex alpha.
    float2 noiseUV = input.uv * material[3].zw + stable[8].w * material[3].xy;
    float2 offset = (flowMap.Sample(colorSampler, noiseUV).xy * 2 - 1) * material[4].x / material[4].yz;
    float4 texel = colorMap.SampleBias(colorSampler, input.uv + offset, stable[7].y);
    float alpha = material[4].w > 0 ? saturate((texel.a + input.color.a) * material[4].w - material[4].w)
                                    : texel.a * input.color.a;
#elif defined(FLOW_ATEST)
    // IW7 c4e82cfb / c4b7c455 (eq_effect_atest_lin_nocast, eq/el_effect_atest_lin_shl_nocast_norm_spec): the plain
    // texel, no flow; its alpha times the particle's is cut below.
    float4 texel = colorMap.SampleBias(colorSampler, input.uv, stable[7].y);
    float alpha = texel.a * input.color.a;
#else
    float4 flow = flowMap.SampleLevel(colorSampler, input.uv, 0);
    float4 surface = flowSurface.SampleLevel(colorSampler, input.uv, 0);
    float2 velocity = (flow.xy * 2 - 1) * flow.z * FLOW_SCALE;
    float period = FLOW_PERIOD.x;
    float halfPeriod = FLOW_PERIOD.z;
    // Source DXBC uses signed frac, equivalent to fmod, in both wrapping stages.
    float phaseA = fmod(fmod(stable[8].w, period) + surface.a, 2 * halfPeriod);
    float phaseB = fmod(fmod(stable[8].w + halfPeriod, period) + surface.a, 2 * halfPeriod);
    float weight = saturate(abs(halfPeriod - phaseA) / halfPeriod);
    float4 a = colorMap.SampleBias(colorSampler, input.uv - velocity * phaseA, stable[7].y);
    float4 b = colorMap.SampleBias(colorSampler, input.uv - velocity * phaseB, stable[7].y);
    float4 texel = lerp(a, b, weight);
    float alpha = texel.a * input.color.a * surface.r;
#endif
#if defined(FLOW_ATEST)
    // IW7's alpha test (c4e82cfb, c4b7c455, 5e767f2f): the particle is cut below 0.501961 and draws opaque (o0.w = 1).
    if (alpha < 0.501961)
        discard;
    alpha = 1;
#elif defined(FLOW_UVD)
    // Its dissolve above; it has no mask.
#elif defined(FLOW_LIT)
    // a386c911 multiplies the alpha by its mask's alpha at the particle's UV (its VS passes the UV unchanged).
    alpha *= flowMask.Sample(colorSampler, input.uv).a;
#else
    // IW7 39eb46cf / d2bc120f (the additive flow with alpha dissolve): the vertex alpha moves a threshold instead,
    // alpha = saturate((texel.a * mask + vertex alpha) * K - K), K = alphaDissolveParms.x (material row 3 .y).
    if (material[3].z != 0)
        alpha = saturate((texel.a * surface.r + input.color.a) * material[3].y - material[3].y);
#endif
#ifdef FLOW_FEATHER
    // b0ec5d88 with feature flag 16 and its plain fade (+116 = 0, flags 32/64/128/1024 clear, as the converter writes
    // them): alpha *= saturate((scene depth - particle depth) * +96.x), +96.x = IW7 featherParms.x. IW7 d384b91e takes
    // min(|...|, 1), which is the same wherever the particle lies in front of the scene.
    ParticleMaterialData data = particleMaterialData[input.particle.x];
    if ((asuint(data.rows[11].w) & 16) != 0)
    {
        const uint level = asuint(stable[7].z);
        const uint2 pixel = (uint2)(input.position.xy * stable[7].x) >> level;
        const float depth = abs(sceneDepth.Load(int3(pixel, level)).r * 6);
        alpha *= saturate((depth - input.position.w) * data.rows[6].x);
    }
#endif
#ifndef FLOW_ADDITIVE
    if (alpha <= 0.000061)
        discard;
#endif
    // Replay supplies the particle RGB squared. IW7 squares (texel * particleColour) before mkhdr.
    float3 linearColor = texel.rgb * texel.rgb * input.color.rgb;
#ifdef FLOW_LIT
    // a386c911 lights the squared colour by its per-particle light; Replay's lit effects read theirs at the
    // particle's lighting UV, times the effect lighting scale (a9864c07: t0 * scene[42].y).
    float3 color = linearColor * particleLighting.SampleLevel(screenSampler, input.lightingUV, 0).rgb * scene[42].y;
#ifdef FLOW_UVD
    // 5d9b9a5b tints the squared albedo (colorTint, row 0; white for c258449e).
    color *= material[0].rgb;
#endif
#else
    float3 hdr = material[3].x != 0 ? -log2(1 - min(linearColor, 0.99)) : linearColor;
    float3 color = hdr * material[0].rgb * input.emissive.rgb * input.emissive.w;
#endif

    // Replay b0ec5d88: the native effect fog map and volumetric-visibility lookup, with the same bindings.
    color *= effectFog.Sample(screenSampler, float3(input.lightingUV, 1)).rgb;
#ifdef FLOW_LIT
    // a386c911 blends toward the fog colour over the particle's coverage; Replay's lit effects add the fog's
    // in-scatter (fog layer 0) weighted by the coverage (a9864c07). The colour is premultiplied from here.
    color = color * alpha + effectFog.Sample(screenSampler, float3(input.lightingUV, 0)).rgb * alpha;
#endif
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
#ifdef FLOW_LIT
    // a386c911 adds the volumetric in-scatter; a9864c07 adds Replay's (t110, times the effect lighting scale) over
    // the coverage.
    color += volumetricScatter.SampleLevel(screenSampler, float3(screenUV, min(slice, limit)), 0).rgb * scene[42].y *
        alpha;
#endif
#ifdef FLOW_ADDITIVE
    // IW7 40967ff1 / 6a577499 / 8ce146ac (and their zfeather twins 4622c9c5 / a6efe634): no alpha cutoff; the fogged
    // colour is cut at 0.000061 and drawn times alpha with no coverage, as Replay's stock additive effect PS b0ec5d88
    // ends (o0.w = o1 = 0).
    if (max(color.r, max(color.g, color.b)) <= 0.000061)
        discard;
#endif
    color *= scene[42].x;
    bool thermal = asint(scene[13].x) == 1 ||
        (asint(scene[13].x) == 2 && scopeBuffer.SampleLevel(screenSampler, screenUV, 0).r > 0);
    if (thermal)
        color = float3(dot(color, float3(0.2126, 0.7152, 0.0722)), 0, 0);
    Output output;
#ifdef FLOW_ADDITIVE
    output.color = float4(min(color * max(alpha, 0), 32255), 0);
    output.opacity = 0;
#elif defined(FLOW_LIT)
    output.color = min(float4(color, alpha), 32255);
    output.opacity = alpha.xxxx;
#else
    output.color = min(float4(color * alpha, alpha), 32255);
    output.opacity = alpha.xxxx;
#endif
    return output;
}
