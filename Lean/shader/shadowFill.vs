cbuffer perFrame
{
	float4x4 shadowView;
	float4x4 shadowProjection;
	float4x4 worldMatrix;
};


float4 ShadowVS(float3 inPos : POSITION) : SV_Position
{
	float4 pos = mul(float4(inPos, 1.0f), worldMatrix);
	//pos.x += 3.0f;
	float4x4 viewProj = mul(shadowView, shadowProjection);

	return mul(pos, viewProj);
}