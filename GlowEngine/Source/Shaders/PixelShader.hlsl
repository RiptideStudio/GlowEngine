// pixel shader input struct
struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 worldpos : WORLDPOS;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

// point light buffer
#define MAXLIGHTS 8
struct PointLightBuffer
{
    float3 lightPosition;
    float size;
    float4 color;
};

// GPU fog buffer
cbuffer LightBufferType : register(b1)
{
    float3 lightDirection;
    float3 lightColor;
    float3 cameraPosition;
};

// define an array of point lights
cbuffer PointLight : register(b0)
{
    PointLightBuffer pointLights[8];
};

// texture sampler
Texture2D shaderTexture;
SamplerState SampleType;

// main entrypoint
// main entry point
float4 main(PixelInputType input) : SV_TARGET
{
    // Get the texture color
    float4 textureColor = shaderTexture.Sample(SampleType, input.texcoord);
    
    // Test constants for lighting calculations
    float shininess = 0.25f; // Shininess factor for specular reflection
    float constantAttenuation = 1.f; // Constant attenuation factor

    // float3 lowResCoords = round(input.worldpos / 3)*3; // this creates the pixelated effect
    float3 lowResCoords = input.worldpos; // this creates the pixelated effect
    
    // Initialize final color
    float3 finalColor = float3(0.3,0.2,0.25);

    // Calculate lighting for each point light
    for (int i = 0; i < MAXLIGHTS; ++i)
    {
        // calculate size
        float linearAttenuation = 0.0045f / pointLights[i].size; // Linear attenuation factor
        float quadraticAttenuation = 0.05f / pointLights[i].size; // Quadratic attenuation factor
        
        // Calculate vector from pixel to light source
        float3 pixelToLight = normalize(pointLights[i].lightPosition - input.worldpos.xyz);
        
        // Calculate the diffuse factor
        float diffuseIntensity = max(dot(input.normal, pixelToLight), 0.0);
        float3 diffuseLight = diffuseIntensity * pointLights[i].color.rgb;

        // Calculate the view vector
        float3 viewVector = normalize(cameraPosition - input.worldpos.xyz);
        
        // Calculate the halfway vector for Blinn-Phong specular highlights
        float3 halfwayVector = normalize(pixelToLight + viewVector);

        // Calculate the specular factor
        float specularIntensity = pow(max(dot(input.normal, halfwayVector), shininess), shininess);
        float3 specularLight = specularIntensity * pointLights[i].color.rgb; // Specular color

        // Combine the diffuse and specular components
        float3 lightColor = diffuseLight + specularLight;

        // Calculate distance to the light source
        float distance = length(pointLights[i].lightPosition - lowResCoords);
        
        // Calculate distance-based attenuation
        float attenuation = 1.0 / (constantAttenuation + linearAttenuation * distance + quadraticAttenuation * distance * distance);

        // Accumulate the light's contribution
        finalColor += attenuation * lightColor;
    }

    // Apply the accumulated light color to the pixel's color
    float4 litColor = float4(finalColor, 1.0) * input.color;

    // Combine the lit color with the texture color, if a texture is present
    if (textureColor.x != 0)
    {
        litColor *= textureColor;
    }

    return litColor;
}
