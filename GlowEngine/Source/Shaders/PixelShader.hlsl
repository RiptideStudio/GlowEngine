// pixel shader input struct
struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

// light buffer
cbuffer LightBufferType : register(b1)
{
    float3 lightDirection;
    float3 lightColor;
    float3 cameraPosition;
};

// texture sampler
Texture2D shaderTexture;
SamplerState SampleType;

// main entrypoint
float4 main(PixelInputType input) : SV_TARGET
{
    // get the texture color
    float4 textureColor = shaderTexture.Sample(SampleType, input.texcoord);
    
    // Calculate diffuse lighting
    float diffuseFactor = max(dot(input.normal, lightDirection), 0.5);
    float3 diffuseColor = diffuseFactor * lightColor * float3(2.25,2,2.25);

    // Combine the vertex color with the diffuse lighting
    float4 finalColor = input.color * float4(diffuseColor, 1);

    if (textureColor.x == 0)
    {
        return finalColor;
    }
    return textureColor * finalColor;
}
