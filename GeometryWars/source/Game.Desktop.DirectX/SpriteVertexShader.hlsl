cbuffer CBufferPerObject
{
	float4x4 ModelViewProjection : WORLDVIEWPROJECTION;
}

struct VS_INPUT
{
	float2 ObjectPosition: POSITION;
	float2 TextureCoordinate : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 Position: SV_Position;
	float2 TextureCoordinate : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT IN)
{
	VS_OUTPUT OUT = (VS_OUTPUT)0;

	OUT.Position = mul(float4(IN.ObjectPosition.xy, 0, 1), ModelViewProjection);
	OUT.Position.z = 0;
	OUT.TextureCoordinate = IN.TextureCoordinate;
	OUT.TextureCoordinate.y *= -1;

	return OUT;
}