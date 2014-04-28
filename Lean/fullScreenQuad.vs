
struct VertexInput
{
	float3 position : POSITION0;
	float2 uv : TEXCOORD0;
};

struct PixelInput
{
	float4 position : SV_Position;
	float2 uv : TEXCOORD0;
};

PixelInput VS(VertexInput input)
{
	PixelInput output;

	output.position = float4(input.position, 1.0f);
	output.uv = input.uv;

	return output;
}