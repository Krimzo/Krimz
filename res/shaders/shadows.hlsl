// Vertex shader
cbuffer VS_CB : register(b0) {
    matrix wvp;
}

struct VS_OUT {
    float4 world : SV_POSITION;
};

VS_OUT vShader(float3 pos : POS_IN, float2 tex : TEX_IN, float3 norm : NORM_IN) {
    VS_OUT data;

    data.world = mul(float4(pos, 1), wvp);

    return data;
}

// Pixel shader
float4 pShader(VS_OUT data) : SV_TARGET {
    return float4(0, 0, 0, 0);
}
