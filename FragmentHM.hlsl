struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION;
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
SamplerState sampAni : register(s0);
PS_OUT PS_main(PS_IN input)
{
	if (input.cull)
	{
		clip(-1);
	}
	PS_OUT output;

	output.color = colorMapTex.Sample(sampAni, input.TexCoord);
	output.pos = input.worldPos;
	output.normal = float4(0, -1, 0, 0);

	return output;
}