// this is the constant buffer that holds the different matrices for transforming to clip space
// they are individually passed her for other calculations in the future
cbuffer ConstantBufferType : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
}

// this is where the vertices are passed into the shader
struct VertexInputType
{
    float4 position : POSITION;
};

// this is where data is passed to the pixel shader
struct PixelInputType
{
    float4 position : SV_POSITION;
};

// main entrypoint
PixelInputType main(VertexInputType input)
{
    PixelInputType output;
    // Transform the vertex position from model space to clip space
    float4 worldPosition = mul(input.position, worldMatrix);
    float4 viewPosition = mul(worldPosition, viewMatrix);
    output.position = mul(viewPosition, projectionMatrix);

    return output;
}
