struct VS_IN
{
	float3 pos : POSITION;
	float2 TexCoord : TEXCOORD;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION;
	float2 TexCoord : TEXCOORD;
};
cbuffer dsBuffer : register(b0)
{
	float4x4 viewProj;
};

VS_OUT VS_main(VS_IN input)
{
	float4x4 viewProjTrans = transpose(viewProj);
	VS_OUT output = (VS_OUT)0;
	output.pos = mul(float4(input.pos, 1), viewProjTrans);
	output.worldPos = float4(input.pos, 1);
	output.TexCoord = input.TexCoord;
	return output;
}