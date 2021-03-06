// Vertex shader
float4 vShader(float3 pos : POS_IN) : SV_POSITION
{
    return float4(pos, 1.0f);
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
    float4 highlightColor;
}

Texture2D indexBuff : register(t0);

float4 pShader(float4 screen : SV_POSITION) : SV_TARGET
{
    const int lineThickness = 1;
    for (int y = -lineThickness; y <= lineThickness; y++)
    {
        for (int x = -lineThickness; x <= lineThickness; x++)
        {
            if (indexBuff[screen.xy + int2(x, y)].r == 1.0f)
            {
                return highlightColor;
            }
        }
    }

    discard;
    return float4(0.0f, 0.0f, 0.0f, 0.0f);
}
