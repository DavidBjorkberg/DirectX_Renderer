struct PS_IN
{
	float4 Pos : SV_POSITION;
	float4 worldPos : POSITION;
	bool cull : CULL;

};

float4 PS_main(PS_IN input) : SV_Target
{
	if (input.cull)
	{
		clip(-1);
	}
	return float4(0,0,0,1);
}