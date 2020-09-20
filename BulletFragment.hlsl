struct PS_IN
{
	float4 Pos : SV_POSITION;
};
struct PS_OUT
{
	float4 color : SV_Target;
};
PS_OUT PS_main(PS_IN input)
{
	PS_OUT output;
	output.color = float4(1, 1, 0, 1);
	return output;
}