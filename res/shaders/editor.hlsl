// Vertex shader
cbuffer VS_CB : register(b0) {
    matrix w;
    matrix vpCam;
    matrix vpSun;
}

struct VS_OUT {
    float4 world : SV_POSITION;
    float2 textur : TEX;
    float3 normal : NORM;
    float4 sunPos : SUN;
};

VS_OUT vShader(float3 pos : POS_IN, float2 tex : TEX_IN, float3 norm : NORM_IN) {
    VS_OUT data;

    // World transform
    data.world = mul(float4(pos, 1.0f), mul(w, vpCam));

    // Texture transform
    data.textur = tex;

    // Normal transform
    data.normal = mul(float4(norm, 0.0f), w).xyz;

    // Lightspace transform
    data.sunPos = mul(float4(pos, 1.0f), mul(w, vpSun));
    data.sunPos = data.sunPos * float4(0.5f, -0.5f, 1.0f, 1.0f) + (float4(0.5f, 0.5f, 0.0f, 0.0f) * data.sunPos.w);

    return data;
}

// Pixel shader
cbuffer PS_CB : register(b0) {
    float4 ambCol;
    float4 dirCol;
    float4 dirDir;
    float4 objInd;
}

SamplerState samp0 : register(s0);
SamplerState samp1 : register(s1);
Texture2D tex0 : register(t0);
Texture2D tex1 : register(t1);

struct PS_OUT {
    float4 color : SV_TARGET0;
    float index : SV_TARGET1;
};

float CalcShadow(float3 lightPos, float lightNormDot);

PS_OUT pShader(VS_OUT data) {
    PS_OUT output;

    // Texture color
    float3 textureColor = tex0.Sample(samp0, data.textur).xyz;

    // Ambient light color
    float3 ambientColor = ambCol.xyz;

    // Directional light color
    float3 directColor = float3(0.0f, 0.0f, 0.0f);

    // Calculating the directional light intensity
    float diffuseFactor = dot(-dirDir.xyz, normalize(data.normal));

    // Checking the diffuse factor
    if (diffuseFactor > 0.0f) {
        directColor = (dirCol * diffuseFactor).xyz;
    }

    // Dividing the sun coords
    data.sunPos /= data.sunPos.w;

    // Clamping the light z
    data.sunPos.z = min(data.sunPos.z, 1.0f);

    // Computing the shadow factor
    float shadowFac = CalcShadow(data.sunPos.xyz, diffuseFactor);

    // Setting the pixel color
    output.color = float4(textureColor * (shadowFac * directColor + ambientColor), 1);

    // Setting the index
    output.index = objInd.x;

    // Returning
    return output;
}

float CalcShadow(float3 lightPos, float lightNormDot) {
    // Shadow factor
    float shadow = 0.0f;

    // Calculating bias
    const float biasMin = 0.0001f;
    const float biasMax = 0.0005f;
    const float bias = max(biasMax * (1.0f - lightNormDot), biasMin);

    // Getting the texture size
    uint mapWidth = 0;
    uint mapHeight = 0;
    uint ignore = 0;
    tex1.GetDimensions(0, mapWidth, mapHeight, ignore);

    // Smoothing the shadow
    const float2 texelSize = 1.0f / float2(mapWidth, mapHeight);
    for (int y = -1; y <= 1; y++) {
        for (int x = -1; x <= 1; x++) {
            const float depth = tex1.Sample(samp1, lightPos.xy + float2(x, y) * texelSize).r;
            shadow += ((depth + bias) < lightPos.z) ? 0.0f : 1.0f;
        }
    }

    // Returing the shadow
    return shadow * 0.111f;
}
