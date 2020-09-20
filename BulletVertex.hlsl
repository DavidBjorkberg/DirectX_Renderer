struct VS_IN
{
	float3 Pos : POSITION;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
};
cbuffer vsBuffer : register(b0)
{
	float4x4 world;
};
cbuffer vsBuffer : register(b1)
{
	float4x4 viewProj;
};

VS_OUT VS_main(VS_IN input)
{

	float4x4 worldTrans = transpose(world);
	float4x4 viewProjTrans = transpose(viewProj);
	VS_OUT output = (VS_OUT)0;
	output.position = mul(float4(input.Pos, 1), worldTrans);
	output.position = mul(output.position, viewProjTrans);
	return output;
}