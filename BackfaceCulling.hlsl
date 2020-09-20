struct GS_OUT
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION;
	float4 tangent : TANGENT;
	float4 normal : NORMAL;
	float2 TexCoord : TEXCOORD;
	bool cull : CULL;
};
struct GS_IN
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION;
	float4 tangent : TANGENT;
	float4 normal : NORMAL;
	float2 TexCoord : TEXCOORD;
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
	float4x4 projTrans = transpose(projection);
	float4x4 viewTrans = transpose(view);
	float4 viewPosA = input[0].pos;
	float4 viewPosB = input[1].pos;
	float4 viewPosC = input[2].pos;
	float3 faceEdgeA = viewPosB.xyz - viewPosA.xyz;
	float3 faceEdgeB = viewPosC.xyz - viewPosA.xyz;
	float4 crossNormal = float4(normalize(cross(faceEdgeA, faceEdgeB)), 1);
	crossNormal = normalize(crossNormal);

	for (uint i = 0; i < 3; i++)
	{
		GS_OUT element;

		element.pos = input[i].pos;
		if (crossNormal.z > 0.2f)
			element.cull = true;
		else
			element.cull = false;
		element.worldPos = input[i].worldPos;
		element.tangent = input[i].tangent;
		element.normal = input[i].normal;
		element.TexCoord = input[i].TexCoord;
		output.Append(element);
	}
}