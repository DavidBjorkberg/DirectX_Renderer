struct PS_IN
{
	float4 Pos : SV_POSITION;
	float2 UV : UV;

};
SamplerState mySampler;
Texture2D colorTex : register(t0);
float4 PS_main(PS_IN input) : SV_Target
{
	float3 textureColor = colorTex.Sample(mySampler, input.UV).rgb;

	return float4(textureColor,1);
}