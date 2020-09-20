struct VS_IN
{
	float3 Pos : POSITION;
	float2 UV : UV;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 UV : UV;
};
cbuffer vsBuffer : register(b0)
{
	float4x4 viewProj;
};
cbuffer rotationBuffer : register(b1)
{
	float4 rotation;
};
cbuffer vsBuffer : register(b2)
{
	float4x4 world;
};
VS_OUT VS_main(VS_IN input)
{
	float4x4 viewProjTrans = transpose(viewProj);
	float4x4 worldTrans = transpose(world);
	VS_OUT output = (VS_OUT)0;
	output.position = float4(-7,-1, -1, 1);
	output.position = mul(output.position, worldTrans);
	output.position = output.position + float4(input.Pos, 0) + float4(1,1,1,0);
	output.position = mul(output.position, viewProjTrans);
	output.UV = input.UV + rotation.x;
	return output;
}