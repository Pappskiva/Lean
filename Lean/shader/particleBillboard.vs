cbuffer perFrame : register( b0 )
{
	float4x4 viewProjection;
	float3 cameraPosition;
};

struct VertexIn
{
	float2 uv : TEXCOORD0;
	
	float3 pos : POSITION;
	float2 scale : SCALE;
	float rotation : ROTATION;
	float alpha : ALPHA;
};

struct PixelIn
{
	float4 pos : SV_Position;
	float2 uv : UV;
	float alpha : ALPHA;
};

PixelIn BillboardVertexShader(VertexIn input)
{
	PixelIn output = (PixelIn) 0;
	float3 eyeVector = normalize(cameraPosition - input.pos);

	float3x3 rot = (float3x3) 0;
	rot._11 = cos(input.rotation);
	rot._12 = -sin(input.rotation);
	rot._21 = -rot._12;
	rot._22 = rot._11;
	rot._33 = 1.0f;
		
	float3 up = mul(float3(0.0f, 1.0f, 0.0f), rot);
	float3 side = normalize(cross(eyeVector, up) );    
	up = normalize(cross(side, eyeVector) );   

	float3 finalPos = input.pos;
	finalPos += (input.uv.x - 0.5f) * side * input.scale[0];
	finalPos += (0.5f - input.uv.y) * up * input.scale[1];

	output.pos = mul(float4(finalPos, 1.0f) , viewProjection);
	output.alpha = input.alpha;
	output.uv = input.uv;

	return output;
}

