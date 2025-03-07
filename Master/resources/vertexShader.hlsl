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


PixelInput vertex_main(VertexInput Input) {
  PixelInput Output;
  Output.position = float4(Input.position, 1);
  Output.color = Input.color;
  
  return Output;
}