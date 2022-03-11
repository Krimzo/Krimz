// Vertex shader
cbuffer VS_CB : register(b0) {
    matrix wvp;
}

struct VS_OUT {
    float4 screen : SV_POSITION;
};

VS_OUT vShader(float3 pos : POS_IN, float2 tex : TEX_IN, float3 norm : NORM_IN) {
    VS_OUT data;
    data.screen = mul(float4(pos, 1.0f), wvp);
    return data;
}

// Pixel shader
cbuffer PS_CB : register(b0) {
    float4 objInd;
}

struct PS_OUT {
    float index : SV_TARGET0;
};

PS_OUT pShader(VS_OUT data) {
    PS_OUT output;
    output.index = objInd.x;
    return output;
}
