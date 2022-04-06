// Vertex shader
cbuffer VS_CB : register(b0)
{
    matrix wvp;
}

float4 vShader(float3 pos : POS_IN) : SV_POSITION
{
    return mul(float4(pos, 1), wvp);
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
    float4 objCol;
    float4 objInd;
}

struct PS_OUT
{
    float4 color : SV_TARGET0;
    float index : SV_TARGET1;
};

PS_OUT pShader(float4 screen : SV_POSITION)
{
    PS_OUT output;
    output.color = objCol;
    output.index = objInd.x;
    return output;
}
