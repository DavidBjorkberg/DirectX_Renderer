struct PS_IN
{
	float4 position : SV_POSITION;
	float2 tex : UV_COORD;
};
Texture2D colorTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D positionTex : register(t2);
SamplerState mySampler : register(s0);
cbuffer vsBuffer : register(b0)
{
	float4 camPos;
};
float4 PS_main(PS_IN input) : SV_TARGET
{
	float3 textureColor = colorTex.Sample(mySampler, input.tex).rgb;
	float3 normal = normalTex.Sample(mySampler, input.tex).rgb;
	float3 position = positionTex.Sample(mySampler, input.tex).rgb;

	float3 lightPos = float3(0, 0, -2);
	float3 lightColour = float3(1, 1, 1);
	float3 posToLight = normalize(lightPos - position);
	// ambient
	float ambientStrength = 0.5f;
	float3 ambient = ambientStrength * textureColor;
	// diffuse 
	float diffuseStrength = 0.5f;
	float diff = clamp(dot(posToLight, normal),0.0f,1.0f);
	float3 diffuse = diff * textureColor;

	// specular
	float specularStrength = 1;
	float3 viewDir = normalize(camPos.xyz - position);
	float3 reflectDir = reflect(-posToLight, normal);
	float spec = pow(max(dot(viewDir, reflectDir),0.0f), 32);
	float3 specular = lightColour * spec;
	float3 result = (ambient + diffuse + spec);

	return float4(result,1);
}