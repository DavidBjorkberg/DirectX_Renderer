struct VS_IN
{
	float3 pos : POSITION;
	float2 TexCoord : TEXCOORD;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION;
	float3 texCoord : TEXCOORD;

};
cbuffer vsBuffer : register(b0)
{
	float4x4 WVP;
};
VS_OUT VS_main(VS_IN input)
{
	float4x4 WVPTrans = transpose(WVP);
	VS_OUT output = (VS_OUT)0;
	output.pos = mul(float4(input.pos, 1.0f), WVPTrans).xyww;
	output.worldPos = float4(input.pos, 1);
	output.texCoord = input.pos;

	return output;
}