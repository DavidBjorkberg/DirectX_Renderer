struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION;
	float4 tangent : TANGENT;
	float4 normal : NORMAL;
	float2 TexCoord : TEXCOORD;
	bool cull : CULL;

};
struct PS_OUT
{
	float4 color : SV_Target0;
	float4 normal : SV_Target1;
	float4 pos : SV_Target2;
};

Texture2D colorMapTex : register(t0);
Texture2D normalMapTex : register(t1);
SamplerState sampAni : register(s0);
PS_OUT PS_main(PS_IN input)
{
	if (input.cull)
	{
		clip(-1);
	}
	PS_OUT output;

	float3 normalMap = normalMapTex.Sample(sampAni, input.TexCoord).rgb;
	normalMap = normalize(normalMap * 2 - 1);
	output.pos = input.worldPos;
	output.color = colorMapTex.Sample(sampAni, input.TexCoord);
	float3 tangent = input.tangent.xyz;
	tangent = normalize(tangent);
	tangent = normalize(tangent - dot(tangent, input.normal.xyz)*input.normal.xyz);
	float3 biTangent = cross(tangent, input.normal.xyz);
	float3x3 TBN = float3x3(tangent, biTangent, input.normal.xyz);
	output.normal = -float4(normalize(mul(normalMap, TBN)), 1);

	return output;
}