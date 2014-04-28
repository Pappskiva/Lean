cbuffer perFrame : register ( b0 )
{
	float4x4 viewProjInverted;
	float4x4 viewProj;
};

cbuffer perInstance : register ( b1 )
{
	float3 lightPos;
	float3 lightColor;
	float lightRadius;
	float lightIntensity;
};

struct PixelInput
{
	float4 pos : SV_Position;
	float4 screenCoord : ScreenCoord;
};


PixelInput PointLightVertexShader(in float3 inPos : POSITION)
{
	PixelInput output = (PixelInput) 0;


	output.pos = mul(float4((inPos * lightRadius) + lightPos, 1.0f), viewProj);
	//output.pos = mul(float4((inPos * lightRadius) + lightPos, 1.0f), viewProj);
	output.screenCoord = output.pos;

	return output;
}