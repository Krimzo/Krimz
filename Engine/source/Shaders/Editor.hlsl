// Vertex shader
cbuffer VS_CB : register(b0)
{
	matrix wMatrix;
	matrix vpCameraMatrix;
	matrix vpSunMatrices[4];
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

	data.world = mul(float4(pos, 1.0f), wMatrix).xyz;
	data.screen = mul(float4(data.world, 1.0f), vpCameraMatrix);
	data.textur = tex;
	data.normal = mul(float4(norm, 0.0f), wMatrix).xyz;

	float3 alterPos = pos + norm * 0.075f;
	for (int i = 0; i < 4; i++)
	{
		data.sunPos[i] = mul(float4(alterPos, 1.0f), mul(wMatrix, vpSunMatrices[i]));
		data.sunPos[i] = data.sunPos[i] * float4(0.5f, -0.5f, 1.0f, 1.0f) + float4(0.5f, 0.5f, 0.0f, 0.0f) * data.sunPos[i].w;
	}

	return data;
}

// Pixel shader
cbuffer PS_CB : register(b0)
{
	float4 multiData;
	float4 ambientColor;
	float4 sunColor;
	float4 sunDirection;
	float4 cameraPosition;
	float4 frustumFarPlanes;
	matrix cameraViewMatrix;
}

SamplerState defaultSampler : register(s0);
SamplerState shadowSampler : register(s1);
Texture2D colorMap : register(t0);
Texture2D normalMap : register(t1);
Texture2D roughnessMap : register(t2);
Texture2D sunDepthMap0 : register(t3);
Texture2D sunDepthMap1 : register(t4);
Texture2D sunDepthMap2 : register(t5);
Texture2D sunDepthMap3 : register(t6);

struct PS_OUT
{
	float4 color : SV_TARGET0;
	float index : SV_TARGET1;
};

float3 GetFragNormal(float3 fragPosition, float3 fragNormal, float2 fragTexture)
{
	float3 newNormal = fragNormal;
	if (multiData.y)
	{
		const float3 Q1 = ddx(fragPosition);
		const float3 Q2 = ddy(fragPosition);
		const float2 st1 = ddx(fragTexture);
		const float2 st2 = ddy(fragTexture);
		const float3 T = normalize(Q1 * st2.x - Q2 * st1.x);
		const float3 B = normalize(-Q1 * st2.y + Q2 * st1.y);
		const float3x3 TBN = float3x3(T, B, fragNormal);
		newNormal = normalize(normalMap.Sample(defaultSampler, fragTexture).xyz * 2.0f - 1.0f);
		newNormal = normalize(mul(newNormal, TBN));
	}
	return newNormal;
}

float GetFragRoughnes(float2 fragTexture)
{
	if (multiData.z)
	{
		return roughnessMap.Sample(defaultSampler, fragTexture).x;
	}
	return multiData.w;
}

float CalcShadow(float3 lightPosition, float lightNormalDot, int frustumLayer)
{
	float shadowFactor = 0.0f;

	uint mapWidth = 0;
	uint mapHeight = 0;
	uint ignore = 0;
	sunDepthMap0.GetDimensions(0, mapWidth, mapHeight, ignore);
	const float2 texelSize = 1.0f / float2(mapWidth, mapHeight);

	if (frustumLayer == 0)
	{
		for (int y = -1; y <= 1; y++)
		{
			for (int x = -1; x <= 1; x++)
			{
				const float depth = sunDepthMap0.Sample(shadowSampler, lightPosition.xy + float2(x, y) * texelSize).r;
				shadowFactor += (depth < lightPosition.z) ? 0.0f : 1.0f;
			}
		}
	}
	else if (frustumLayer == 1)
	{
		for (int y = -1; y <= 1; y++)
		{
			for (int x = -1; x <= 1; x++)
			{
				const float depth = sunDepthMap1.Sample(shadowSampler, lightPosition.xy + float2(x, y) * texelSize).r;
				shadowFactor += (depth < lightPosition.z) ? 0.0f : 1.0f;
			}
		}
	}
	else if (frustumLayer == 2)
	{
		for (int y = -1; y <= 1; y++)
		{
			for (int x = -1; x <= 1; x++)
			{
				const float depth = sunDepthMap2.Sample(shadowSampler, lightPosition.xy + float2(x, y) * texelSize).r;
				shadowFactor += (depth < lightPosition.z) ? 0.0f : 1.0f;
			}
		}
	}
	else if (frustumLayer == 3)
	{
		for (int y = -1; y <= 1; y++)
		{
			for (int x = -1; x <= 1; x++)
			{
				const float depth = sunDepthMap3.Sample(shadowSampler, lightPosition.xy + float2(x, y) * texelSize).r;
				shadowFactor += (depth < lightPosition.z) ? 0.0f : 1.0f;
			}
		}
	}
	return shadowFactor * 0.111f;
}

PS_OUT pShader(VS_OUT data)
{
	data.normal = GetFragNormal(data.world, normalize(data.normal), data.textur);

	const float4 baseColor = colorMap.Sample(defaultSampler, data.textur);

	const float diffuseFactor = max(dot(-sunDirection.xyz, data.normal), 0.0f);
	const float3 diffuseComponent = sunColor.rgb * diffuseFactor;

	const float specularStrength = 1.0f - GetFragRoughnes(data.textur);
	const float3 viewDirection = normalize(cameraPosition.xyz - data.world);
	const float3 reflectionDirection = reflect(sunDirection.xyz, data.normal);
	const float specularFactor = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 64.0f);
	const float3 specularComponent = specularStrength * specularFactor * sunColor.rgb;

	float4 cameraViewPosition = mul(float4(data.world, 1.0f), cameraViewMatrix);
	float distanceFromCamera = abs(cameraViewPosition.z);
	int frustumLayer;
	for (int i = 0; i < 4; i++)
	{
		if (distanceFromCamera < frustumFarPlanes[i])
		{
			frustumLayer = i;
			break;
		}
	}
	float3 sunPosition = data.sunPos[frustumLayer] / data.sunPos[frustumLayer].w;
	sunPosition.z = min(sunPosition.z, 1.0f);
	const float shadowComponent = CalcShadow(sunPosition, diffuseFactor, frustumLayer);

	const float4 fullLight = float4(shadowComponent * (diffuseComponent + specularComponent) + ambientColor.rgb, 1.0f);

	PS_OUT output;
	output.color = baseColor * fullLight;
	output.index = multiData.x;
	return output;
}
