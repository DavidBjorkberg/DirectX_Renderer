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

VS_OUT VS_main(VS_IN input)
{
	float4x4 viewProjTrans = transpose(viewProj);
	VS_OUT output = (VS_OUT)0;
	output.position = mul(float4(input.Pos, 1), viewProjTrans);
	output.UV = input.UV;
	return output;
}