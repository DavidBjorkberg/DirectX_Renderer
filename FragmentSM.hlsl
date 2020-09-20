struct PS_IN
{
	float4 pos : SV_POSITION;
	float4 worldPos : POSITION;
	float3 texCoord : TEXCOORD;
};
struct PS_OUT
{
	float4 color : SV_Target0;
	float4 normal : SV_Target1;
	float4 pos : SV_Target2;
};
SamplerState sampAni;
TextureCube SkyMap : register(t0);

PS_OUT PS_main(PS_IN input)
{
	PS_OUT output;

	output.color = SkyMap.Sample(sampAni, input.texCoord);
	output.normal = float4(0, 1, 0, 0);
	output.pos = input.worldPos;
	return output;
}