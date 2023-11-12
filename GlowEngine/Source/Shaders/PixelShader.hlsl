// pixel shader input struct
struct PixelInputType
{
    float4 position : SV_POSITION;
    float4 worldpos : WORLDPOS;
    float4 color : COLOR;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
};

// GPU fog buffer
cbuffer LightBufferType : register(b1)
{
    float3 lightDirection;
    float3 lightColor;
    float3 cameraPosition;
};

// point light buffer
cbuffer PointLight : register(b0)
{
    float3 lightPosition;
    float padding;
    float4 color;
};

// texture sampler
Texture2D shaderTexture;
SamplerState SampleType;

// main entrypoint
float4 main(PixelInputType input) : SV_TARGET
{
    // get the texture color
    float4 textureColor = shaderTexture.Sample(SampleType, input.texcoord);
    
    // test contants
    float shininess = 0.1f;
    float constantAttenuation = 1.5f;
    float linearAttenuation = 0.0045f;
    float quadraticAttenuation = 0.0075f;
    
    // Calculate vector from pixel to light source
    float3 pixelToLight = normalize(lightPosition - input.worldpos.xyz);
    
    // Calculate the diffuse factor
    float diffuseIntensity = max(dot(input.normal, pixelToLight), 0.0);
    float3 diffuseLight = diffuseIntensity * color.rgb;

    // Calculate the view vector
    float3 viewVector = normalize(cameraPosition - input.worldpos.xyz);
    
    // Calculate the halfway vector for Blinn-Phong specular highlights
    float3 halfwayVector = normalize(pixelToLight + viewVector);

    // Calculate the specular factor
    float specularIntensity = pow(max(dot(input.normal, halfwayVector), 0.1), shininess);
    float3 specularLight = specularIntensity * color.rgb; // Assuming specular color is white for simplicity
    
    // Calculate the final color by combining the diffuse and specular components
    float3 lightColor = diffuseLight + specularLight;
    float4 litColor = float4(lightColor, 1) * input.color;

    // Combine the lit color with the texture color, if a texture is present
    if (textureColor.x != 0)
    {
        litColor *= textureColor;
    }
    float distance = length(lightPosition - input.worldpos.xyz);
    
    // Calculate distance-based attenuation
    float attenuation = 1.0 / (constantAttenuation + linearAttenuation * distance + quadraticAttenuation * distance * distance);
    litColor.rgb *= attenuation;
    
    // Apply fog based on distance
    float wideness = 75;
    float fogFactor = clamp((wideness - distance) / (wideness - wideness / 6), 0.2, 1);
    litColor = lerp(float4(0.025, 0, 0.075, 1), litColor, fogFactor);
    
    // Return the final color
    return litColor;
}

