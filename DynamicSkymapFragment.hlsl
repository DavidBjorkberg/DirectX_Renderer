struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION;
	float3 texCoord : TEXCOORD;

};
struct PS_OUT
{
	float4 color : SV_Target;
};
Texture2D colorTex : register(t0);
SamplerState mySampler : register(s0);
PS_OUT PS_main(PS_IN input)
{
	PS_OUT output;

	float3 color = colorTex.Sample(mySampler, input.texCoord).rgb;
	output.color = float4(color, 1);
	return output;
}