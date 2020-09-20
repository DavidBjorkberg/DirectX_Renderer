struct VS_IN
{
	float2 Pos : POSITION;
	float2 tex : UV_COORD;
};

struct VS_OUT
{
	float4 position : SV_POSITION;
	float2 tex : UV_COORD;
};

VS_OUT VS_main(VS_IN input)
{
	VS_OUT output = (VS_OUT)0;
	output.position = float4(input.Pos, 0, 1);
	output.tex = input.tex;
	return output;
}	