struct HS_IN
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION;
	float4 tangent : TANGENT;
	float4 normal : NORMAL;
	float2 TexCoord : TEXCOORD;
	bool cull : CULL;

};
struct HS_OUT
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION;
	float4 tangent : TANGENT;
	float4 normal : NORMAL;
	float2 TexCoord : TEXCOORD;
	bool cull : CULL;

};
struct HS_PATCH_OUTPUT
{
	float Edges[3] : SV_TessFactor;
	float Inside : SV_InsideTessFactor;
};
HS_PATCH_OUTPUT HSPATCH(InputPatch<HS_IN, 3> ip, uint PatchID :SV_PrimitiveID)
{
	HS_PATCH_OUTPUT output;

	const float factor = 16.0f;
	output.Edges[0] = factor;
	output.Edges[1] = factor;
	output.Edges[2] = factor;
	output.Inside = factor;

	return output;
}

[domain("tri")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("HSPATCH")]
HS_OUT HS_main(InputPatch<HS_IN, 3> ip, uint i : SV_OutputControlPointID, uint patchId : SV_PrimitiveID)
{
	HS_OUT output;

	output.pos = ip[i].pos;
	output.worldPos = ip[i].worldPos;
	output.TexCoord = ip[i].TexCoord;
	output.tangent = ip[i].tangent;
	output.normal = ip[i].normal;
	output.cull = ip[i].cull;

	return output;
}