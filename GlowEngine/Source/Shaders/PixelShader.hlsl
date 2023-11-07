// pixel shader input struct
struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 worldpos : WORLDPOS;
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

    // Calculate the distance from the camera to the pixel
    float distance = length(cameraPosition - input.worldpos.xyz);

    // Compute the fog factor based on the distance
    float fogFactor = clamp((100 - distance) / (100 - 25), 0.0, 1.0);
    
    // Interpolate between the object's color and the fog color
    finalColor = lerp(float4(0.8,0.8,0.8,1), finalColor, fogFactor);
    
    if (textureColor.x == 0)
    {
        return finalColor;
    }
    return textureColor * finalColor;
}
