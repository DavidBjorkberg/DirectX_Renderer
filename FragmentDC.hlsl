struct PS_IN
{
	float4 Pos : SV_POSITION;
	float2 UV : UV;

};

Texture2D colorTex : register(t0);
SamplerState mySampler : register(s0);
float4 PS_main(PS_IN input) : SV_Target
{
	float2 newUV = float2((input.UV.x * -1) + 1,input.UV.y);
	float3 color = colorTex.Sample(mySampler, newUV).rgb;
	return float4(color, 1);
}