struct PS_IN
{
	float4 Pos : SV_POSITION;
	float2 UV : UV;

};
struct PS_OUT
{
	float4 color : SV_Target;
};
PS_OUT PS_main(PS_IN input)
{
	PS_OUT output;
	
	output.color = float4(input.UV.x % 0.2, input.UV.y % 0.2, 0, 1);
	return output;
}