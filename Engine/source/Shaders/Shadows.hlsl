// Vertex shader
cbuffer VS_CB : register(b0)
{
    matrix wvpMatrix;
}

float4 vShader(float3 pos : POS_IN) : SV_POSITION
{
    return mul(float4(pos, 1.0f), wvpMatrix);
}

// Pixel shader
float4 pShader(float4 screen : SV_POSITION) : SV_TARGET
{
    return float4(0.0f, 0.0f, 0.0f, 0.0f);
}
