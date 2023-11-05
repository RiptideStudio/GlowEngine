// pixel shader input struct
struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float3 normal : NORMAL;
};

// light buffer
cbuffer LightBufferType : register(b1)
{
    float3 lightDirection;
    float3 lightColor;
    float3 cameraPosition;
};

// main entrypoint
float4 main(PixelInputType input) : SV_TARGET
{
    // Normalize the normal vector
    float3 normalizedNormal = normalize(input.normal);
    
    // Calculate diffuse lighting
    float diffuseFactor = max(dot(normalizedNormal, -lightDirection), 0.2);
    float3 diffuseColor = diffuseFactor * lightColor * float3(2.25, 2, 2);

    // Combine the vertex color with the diffuse lighting
    float4 finalColor = input.color * float4(diffuseColor, 1);

    // return input.color;
    return finalColor;
}
