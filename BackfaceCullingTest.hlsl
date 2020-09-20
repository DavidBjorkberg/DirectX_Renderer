struct GS_OUT
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION;
	bool cull : CULL;
};
struct GS_IN
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION;

};

cbuffer gsBuffer : register(b0)
{
	float4x4 projection;
};
cbuffer gsBuffer : register(b1)
{
	float4x4 view;
};
[maxvertexcount(3)]
void GS_main(
	triangle GS_IN input[3] : SV_POSITION,
	inout TriangleStream< GS_OUT > output
)
{
	float3 faceEdgeA = input[1].pos.xyz - input[0].pos.xyz;
	float3 faceEdgeB = input[2].pos.xyz - input[0].pos.xyz;
	float4 crossNormal = float4(normalize(cross(faceEdgeA, faceEdgeB)), 1);
	crossNormal = normalize(crossNormal);
	for (uint i = 0; i < 3; i++)
	{
		GS_OUT element;

		element.pos = input[i].pos;
		element.worldPos = input[i].pos;
		if (crossNormal.z > 0.0f)
			element.cull = true;
		else
			element.cull = false;
		output.Append(element);
	}
}