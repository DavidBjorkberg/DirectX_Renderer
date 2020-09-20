struct PS_IN
{
	float4 pos : SV_POSITION;
	float2 UV : UV;
};
struct PS_OUT
{
	float4 color1 : SV_Target0;
};

Texture2D colorMapTex : register(t0);
SamplerState sampAni : register(s0);
PS_OUT PS_main(PS_IN input)
{
	PS_OUT output;
	output.color1 = colorMapTex.Sample(sampAni, input.UV);
	return output;
}