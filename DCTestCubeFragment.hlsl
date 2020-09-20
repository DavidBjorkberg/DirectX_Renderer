struct PS_IN
{
	float4 Pos : SV_POSITION;
	float2 UV : UV;

};
struct PS_OUT
{
	float4 color : SV_Target0;
};
Texture2D colorTex : register(t0);
SamplerState mySampler : register(s0);
PS_OUT PS_main(PS_IN input)
{
	PS_OUT output;
	float3 color = colorTex.Sample(mySampler, input.UV).rgb;
	output.color = float4(color, 1);
	return output;
}