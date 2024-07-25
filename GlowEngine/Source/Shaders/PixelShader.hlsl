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

cbuffer ColorRegister : register(b2)
{
    ColorBuffer colorBuffer;
};

cbuffer OutlineRegister : register(b3)
{
    ColorBuffer outlineBuffer;
};

// GPU fog buffer
cbuffer GlobalLightBuffer : register(b1)
{
    float3 lightDirection;
    float3 lightColor;
    float3 cameraPosition;
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
    
    // Get the texture color
    float4 textureColor = shaderTexture.Sample(SampleType, input.texcoord);
    
    // Initialize final color with ambient light
    float4 finalColor = float4(0.3,0.4,0.4,1);

    // Apply the accumulated light color to the pixel's color
    float4 litColor = finalColor;

    // Combine the lit color with the texture color, if a texture is present
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
