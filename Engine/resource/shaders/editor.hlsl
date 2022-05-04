// Vertex shader
cbuffer VS_CB : register(b0)
{
    matrix w;
    matrix vpCam;
    matrix vpSun[4];
}

struct VS_OUT
{
    float4 screen : SV_POSITION;
    float3 world : WRLD;
    float2 textur : TEX;
    float3 normal : NORM;
    float4 sunPos[4] : SUN;
};

VS_OUT vShader(float3 pos : POS_IN, float2 tex : TEX_IN, float3 norm : NORM_IN)
{
    VS_OUT data;

    // World transform
    data.world = mul(float4(pos, 1.0f), w).xyz;

    // Screen transform
    data.screen = mul(float4(data.world, 1.0f), vpCam);

    // Texture transform
    data.textur = tex;

    // Normal transform
    data.normal = mul(float4(norm, 0.0f), w).xyz;

    // Lightspace transform
    for (int i = 0; i < 4; i++)
    {
        data.sunPos[i] = mul(float4(pos, 1.0f), mul(w, vpSun[i]));
        data.sunPos[i] = data.sunPos[i] * float4(0.5f, -0.5f, 1.0f, 1.0f) + float4(0.5f, 0.5f, 0.0f, 0.0f) * data.sunPos[i].w;
    }
    
    return data;
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
    float4 ambCol;
    float4 dirCol;
    float4 dirDir;
    float4 camPos;
    float4 rghFac;
    float4 objInd;
    float4 frFars;
    matrix camViewM;
}

SamplerState samp0 : register(s0);
SamplerState samp1 : register(s1);
Texture2D tex0 : register(t0);
Texture2D sun0 : register(t1);
Texture2D sun1 : register(t2);
Texture2D sun2 : register(t3);
Texture2D sun3 : register(t4);

struct PS_OUT
{
    float4 color : SV_TARGET0;
    float index : SV_TARGET1;
};

float CalcShadow(float3 lightPos, float lightNormDot, int layer);

PS_OUT pShader(VS_OUT data)
{
    // Base pixel color
    const float4 baseCol = tex0.Sample(samp0, data.textur);

    // Diffuse
    const float3 norm = normalize(data.normal);
    const float diffuseFac = max(dot(-dirDir.xyz, norm), 0.0f);
    const float3 diffuse = dirCol.rgb * diffuseFac;

    // Specular
    const float specStr = 1.0f - rghFac.x;
    const float3 viewDir = normalize(camPos.xyz - data.world);
    const float3 reflDir = reflect(dirDir.xyz, norm);
    const float specFac = pow(max(dot(viewDir, reflDir), 0.0f), 64.0f);
    const float3 specular = specStr * specFac * dirCol.rgb;

    // Shadow
    float4 camViewPos = mul(float4(data.world, 1.0f), camViewM);
    float depthValue = abs(camViewPos.z);
    int layer;
    for (int i = 0; i < 4; i++)
    {
        if (depthValue < frFars[i])
        {
            layer = i;
            break;
        }
    }
    float4 sunPos = data.sunPos[layer];
    sunPos /= sunPos.w;
    sunPos.z = min(sunPos.z, 1.0f);
    const float shadow = CalcShadow(sunPos.xyz, diffuseFac, layer);

    // Full light
    const float4 light = float4(shadow * (diffuse + specular) + ambCol.rgb, 1.0f);
  
    PS_OUT output;
    output.color = baseCol * light;
    output.index = objInd.x;
    return output;
}

float CalcShadow(float3 lightPos, float lightNormDot, int layer)
{
    // Shadow factor
    float shadowFac = 0.0f;

    // Calculating bias
    const float biasMin = 0.0001f;
    const float biasMax = 0.00025f;
    const float bias = max(biasMax * (1.0f - lightNormDot), biasMin);

    // Getting the texture size
    uint mapWidth = 0;
    uint mapHeight = 0;
    uint ignore = 0;
    sun0.GetDimensions(0, mapWidth, mapHeight, ignore);
    const float2 texelSize = 1.0f / float2(mapWidth, mapHeight);
    
    // Smoothing the shadow
    if (layer == 0)
    {
        for (int y = -1; y <= 1; y++)
        {
            for (int x = -1; x <= 1; x++)
            {
                const float depth = sun0.Sample(samp1, lightPos.xy + float2(x, y) * texelSize).r;
                shadowFac += ((depth + bias) < lightPos.z) ? 0.0f : 1.0f;
            }
        }
    }
    else if (layer == 1)
    {
        for (int y = -1; y <= 1; y++)
        {
            for (int x = -1; x <= 1; x++)
            {
                const float depth = sun1.Sample(samp1, lightPos.xy + float2(x, y) * texelSize).r;
                shadowFac += ((depth + bias) < lightPos.z) ? 0.0f : 1.0f;
            }
        }
    }
    else if (layer == 2)
    {
        for (int y = -1; y <= 1; y++)
        {
            for (int x = -1; x <= 1; x++)
            {
                const float depth = sun2.Sample(samp1, lightPos.xy + float2(x, y) * texelSize).r;
                shadowFac += ((depth + bias) < lightPos.z) ? 0.0f : 1.0f;
            }
        }
    }
    else if (layer == 3)
    {
        for (int y = -1; y <= 1; y++)
        {
            for (int x = -1; x <= 1; x++)
            {
                const float depth = sun3.Sample(samp1, lightPos.xy + float2(x, y) * texelSize).r;
                shadowFac += ((depth + bias) < lightPos.z) ? 0.0f : 1.0f;
            }
        }
    }
    return shadowFac * 0.111f;
}
