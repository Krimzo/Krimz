// Vertex shader
struct vOut {
    float4 screen : SV_POSITION;
};

vOut vShader(float3 pos : POS_IN, float2 tex : TEX_IN, float3 norm : NORM_IN) {
    vOut data;
    data.screen = float4(pos, 1.0f);
    return data;
}

// Pixel shader
cbuffer PS_CB : register(b0) {
    float4 higCol;
    float4 selInd;
}

Texture2D indexBuff : register(t0);

float4 pShader(vOut data) : SV_TARGET {
    const float lineThick = 2.0f;

    for (int y = -lineThick; y <= lineThick; y++) {
        for (int x = -lineThick; x <= lineThick; x++) {
            if (indexBuff[data.screen.xy + int2(x, y)].r == selInd.x) {
                return higCol;
            }
        }
    }

    discard;
    return float4(0.0f, 0.0f, 0.0f, 0.0f);
}
