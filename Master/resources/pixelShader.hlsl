struct PixelInput
{
  float4 position : SV_POSITION;
  float4 color : COLOR0;
};

float4 pixel_main(PixelInput Input) : SV_TARGET {
  return Input.color;
}