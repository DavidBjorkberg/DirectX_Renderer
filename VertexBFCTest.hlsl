struct VS_IN
{
	float3 Pos : POSITION;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION;
};
cbuffer vsBuffer : register(b0)
{
	float4x4 viewProj;
};
cbuffer vsBuffer : register(b1)
{
	float4x4 world;
};
VS_OUT VS_main(VS_IN input)
{
	float4x4 viewProjTrans = transpose(viewProj);
	float4x4 worldTrans = transpose(world);
	VS_OUT output = (VS_OUT)0;
	output.pos = mul(float4(input.Pos, 1), worldTrans);
	output.pos = mul(output.pos,viewProjTrans);
	output.worldPos = mul(float4(input.Pos, 1), worldTrans);
	return output;
}