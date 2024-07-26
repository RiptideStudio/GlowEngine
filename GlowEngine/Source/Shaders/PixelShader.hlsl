// Pixel shader input struct
struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 worldpos : WORLDPOS;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float4 shadowCoord : TEXCOORD1;
};

struct ColorBuffer
{
    float4 objectColor;
};

cbuffer UvScale : register(b0)
{
  float2 uvScale;
  float padding;
  float padding2;
};

cbuffer GlobalLightBuffer : register(b1)
{
  float3 lightDirection;
  float3 lightColor;
  float3 cameraPosition;
};

cbuffer ColorRegister : register(b2)
{
    ColorBuffer colorBuffer;
};

cbuffer OutlineRegister : register(b3)
{
    ColorBuffer outlineBuffer;
};


// Texture sampler
Texture2D shaderTexture;
SamplerState SampleType;

// Shadow map texture sampler
Texture2D shadowTexture;
SamplerComparisonState shadowSampler;

// Main entry point
float4 main(PixelInputType input) : SV_TARGET
{
    // Draw the object an entire color
    if (colorBuffer.objectColor.a != 0)
    {
        return colorBuffer.objectColor;
    }
    // Outlined object are slightly scaled up
    if (outlineBuffer.objectColor.a != 0)
    {
        return outlineBuffer.objectColor;
    }
    
    // dynamically scale UV coordinates
    input.texcoord *= uvScale;
  
    // Get the texture color
    float4 textureColor = shaderTexture.Sample(SampleType, input.texcoord);
  
    // Normalize the normal vector
    float3 normal = normalize(input.normal);

    // Calculate the diffuse light factor
    float diffuseFactor = max(dot(normal, -lightDirection), 0.2);

    // Apply the light color to the diffuse factor
    float3 diffuseColor = diffuseFactor * lightColor;

    // Combine the lit color with the texture color, if a texture is present
    float4 litColor = float4(diffuseColor, 1.0);
    
    litColor *= textureColor;

    // Calculate fog factor
    float fogStart = 125.0f; // Start distance for fog
    float fogEnd = 250.0f; // End distance for fog
    float fogDistance = length(input.worldpos.xyz - cameraPosition);
    float fogFactor = saturate((fogEnd - fogDistance) / (fogEnd - fogStart));

    // Fog color
    float3 fogColor = float3(0.5, 0.5, 0.5);

    // Interpolate between the fog color and the lit color
    float4 finalColorWithFog = lerp(float4(fogColor, 0.0), litColor, fogFactor);
    return finalColorWithFog;
}
