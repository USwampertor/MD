Texture2D txColor : register(t0);
Texture2D txReflect : register(t1);

SamplerState samPoint : register(s0);
SamplerState samLinear : register(s1);
SamplerState samAniso : register(s2);

struct VertexInput
{
  float3 position : POSITION;
  float3 normal : NORMAL0;
  float4 color : COLOR0;
  float2 texCoord : TEXCOORD0;
};

struct PixelInput
{
  float4 position : SV_POSITION;
  float4 color : TEXCOORD0;
  float3 normal : TEXCOORD2;
  float2 texCoord : TEXCOORD1;
  float3 posW : TEXCOORD3;
};


float4 pixel_main(PixelInput Input) : SV_TARGET {
  
  float4 color = txColor.Sample(samLinear, Input.texCoord);
  
  float2 screenPos = Input.position.xy;
  
  float2 texSize;
  txReflect.GetDimensions(texSize.x, texSize.y);
  screenPos = screenPos / texSize;
  
  
  float4 colorReflect = txReflect.Sample(samLinear, screenPos);
  return (color + colorReflect * 0.25f);
  return colorReflect;
  
  // return float4(Input.texCoord, 0.0f, 1.0f);
}


