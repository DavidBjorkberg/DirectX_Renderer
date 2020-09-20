struct VS_IN
{
	float4 pos : POSITION;
	float4 tangent : TANGENT;
	float4 normal : NORMAL;
	float2 TexCoord : TEXCOORD;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION;
	float4 tangent : TANGENT;
	float4 normal : NORMAL;
	float2 TexCoord : TEXCOORD;
};

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	output.pos = float4(input.pos.xyz, 1);
	output.worldPos = float4(input.pos.xyz, 1);
	output.TexCoord = input.TexCoord;
	output.normal = input.normal;
	output.tangent = float4(normalize(input.tangent.xyz),0);
	return output;
}