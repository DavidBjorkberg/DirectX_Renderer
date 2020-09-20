struct DS_OUT
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION;
	float4 tangent : TANGENT;
	float4 normal : NORMAL;
	float2 TexCoord : TEXCOORD;
	bool cull : CULL;
};
// Output control point
struct DS_IN
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION;
	float4 tangent : TANGENT;
	float4 normal : NORMAL;
	float2 TexCoord : TEXCOORD;
	bool cull : CULL;
};
// Output patch constant data.
struct HS_PATCH_OUTPUT
{
	float EdgeTessFactor[3]	: SV_TessFactor;
	float InsideTessFactor : SV_InsideTessFactor;

};
cbuffer dsBuffer : register(b0)
{
	float4x4 viewProj;
	float4 camPos;
};

Texture2D HeightTexture : register(t0);
SamplerState LinearSampler;
[domain("tri")]
DS_OUT DS_main(const OutputPatch<DS_IN, 3> TriPatch,
	float3 Coords : SV_DomainLocation,
	HS_PATCH_OUTPUT input)
{
	DS_OUT output = (DS_OUT)0;
	float4x4 viewProjTrans = transpose(viewProj);
	float4 normal = Coords.x * TriPatch[0].normal
		+ Coords.y * TriPatch[1].normal
		+ Coords.z * TriPatch[2].normal;
	float4 worldPos = Coords.x * TriPatch[0].worldPos
		+ Coords.y * TriPatch[1].worldPos
		+ Coords.z * TriPatch[2].worldPos;

	output.TexCoord = Coords.x * TriPatch[0].TexCoord
		+ Coords.y * TriPatch[1].TexCoord
		+ Coords.z * TriPatch[2].TexCoord;

	output.tangent = Coords.x * TriPatch[0].tangent
		+ Coords.y * TriPatch[1].tangent
		+ Coords.z * TriPatch[2].tangent;

	output.normal = normal;
	output.cull = Coords.x * TriPatch[0].cull
		+ Coords.y * TriPatch[1].cull
		+ Coords.z * TriPatch[2].cull;

	float MipInterval = 20.f;
	float heightlevel = clamp((distance(worldPos.xyz, camPos.xyz) - MipInterval) / MipInterval, 0.f, 6.f);
	float4 texHeight = HeightTexture.SampleLevel(LinearSampler, output.TexCoord, heightlevel);
	const float fScale = 0.3f;
	worldPos.xyz += (texHeight.r - 1)* fScale * normalize(normal);
	output.pos = mul(worldPos, viewProjTrans);
	output.worldPos = worldPos;
	return output;

}
