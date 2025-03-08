struct VertexInput
{
  float3 position : POSITION;
  float4 color : COLOR0;
};

struct PixelInput
{
  float4 position : SV_POSITION;
  float4 color : COLOR0;
};

cbuffer MatrixCollection : register(b0)
{
  float4x4 World;
  float4x4 View;
  float4x4 Projection;
}

PixelInput vertex_main(VertexInput Input) {
  PixelInput Output;
  Output.position = float4(Input.position, 1);
  
  Output.position = mul(Output.position, World);
  Output.position = mul(Output.position, View);
  Output.position = mul(Output.position, Projection);
  
  Output.color = Input.color;
  
  return Output;
}