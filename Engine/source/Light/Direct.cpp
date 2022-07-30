#include "Light/Direct.h"


Krimz::Light::Direct::Direct(const std::string& name, kl::ref<kl::gpu> gpu, const kl::uint2& size)
	: Entity(name), m_GPU(gpu), m_TextureSize(size)
{
	kl::dx::desc::texture depthDescriptor = {};
	depthDescriptor.Width = uint(size.x);
	depthDescriptor.Height = uint(size.y);
	depthDescriptor.MipLevels = 1;
	depthDescriptor.ArraySize = 1;
	depthDescriptor.Format = DXGI_FORMAT_R32_TYPELESS;
	depthDescriptor.SampleDesc.Count = 1;
	depthDescriptor.Usage = D3D11_USAGE_DEFAULT;
	depthDescriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	kl::dx::texture depthTextures[4] = {};
	for (auto& texture : depthTextures)
	{
		texture = gpu->newTexture(&depthDescriptor);
	}

	kl::dx::view::desc::depth depthViewDescriptor = {};
	depthViewDescriptor.Format = DXGI_FORMAT_D32_FLOAT;
	depthViewDescriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	for (int i = 0; i < 4; i++)
	{
		m_DepthViews[i] = gpu->newDepthView(depthTextures[i], &depthViewDescriptor);
	}

	kl::dx::view::desc::shader shaderViewDescriptor = {};
	shaderViewDescriptor.Format = DXGI_FORMAT_R32_FLOAT;
	shaderViewDescriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderViewDescriptor.Texture2D.MipLevels = 1;
	for (int i = 0; i < 4; i++)
	{
		m_ShaderViews[i] = gpu->newShaderView(depthTextures[i], &shaderViewDescriptor);
	}

	for (auto& texture : depthTextures)
	{
		gpu->destroy(texture);
	}
}
Krimz::Light::Direct::~Direct()
{
	for (auto& depthView : m_DepthViews)
	{
		m_GPU->destroy(depthView);
	}
	for (auto& shaderView : m_ShaderViews)
	{
		m_GPU->destroy(shaderView);
	}
}

kl::uint2 Krimz::Light::Direct::size() const
{
	return m_TextureSize;
}

kl::float3 Krimz::Light::Direct::direction() const
{
	return m_Direction;
}
void Krimz::Light::Direct::direction(const kl::float3& dir)
{
	m_Direction = dir.normalize();
}

kl::mat4 Krimz::Light::Direct::matrix(const kl::camera& cam, uint ind) const
{
	if (kl::console::warning(ind > 3, "Accessing out of bounds frustum"))
	{
		return {};
	}

	kl::camera copyCam = cam;
	copyCam.near = cam.near + (cam.far - cam.near) * (ind > 0 ? farLimits[ind - 1] : 0.0f);
	copyCam.far = cam.near + (cam.far - cam.near) * farLimits[ind];
	const kl::mat4 invCam = copyCam.matrix().inverse();

	std::vector<kl::float4> frustumCorners;
	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			for (int z = 0; z < 2; z++)
			{
				const kl::float4 corner = invCam * kl::float4(x * 2.0f - 1.0f, y * 2.0f - 1.0f, z * 2.0f - 1.0f, 1.0f);
				frustumCorners.push_back(corner / corner.w);
			}
		}
	}

	kl::float3 centroid;
	for (auto& corn : frustumCorners)
	{
		centroid += corn.xyz;
	}
	centroid /= 8.0f;

	const kl::mat4 view = kl::mat4::lookAt(centroid - m_Direction, centroid, { 0.0f, 1.0f, 0.0f });

	kl::float3 minp = { FLT_MAX, FLT_MAX, FLT_MAX };
	kl::float3 maxp = { -FLT_MAX, -FLT_MAX, -FLT_MAX };
	for (auto& corn : frustumCorners)
	{
		const kl::float4 lightCorn = view * corn;
		minp.x = min(minp.x, lightCorn.x);
		maxp.x = max(maxp.x, lightCorn.x);
		minp.y = min(minp.y, lightCorn.y);
		maxp.y = max(maxp.y, lightCorn.y);
		minp.z = min(minp.z, lightCorn.z);
		maxp.z = max(maxp.z, lightCorn.z);
	}
	maxp.z *= 5.0f;

	const kl::mat4 proj = kl::mat4::orthographic(minp.x, maxp.x, minp.y, maxp.y, maxp.z, minp.z);

	return proj * view;
}

kl::dx::view::depth Krimz::Light::Direct::depthView(uint ind) const
{
	if (kl::console::warning(ind > 3, "Accessing out of bounds frustum"))
	{
		return nullptr;
	}
	return m_DepthViews[ind];
}
ID3D11ShaderResourceView* Krimz::Light::Direct::shaderView(uint ind) const
{
	if (kl::console::warning(ind > 3, "Accessing out of bounds frustum"))
	{
		return nullptr;
	}
	return m_ShaderViews[ind];
}

kl::float4 Krimz::Light::Direct::frustumBounds(const kl::camera& cam) const
{
	kl::float4 bounds;
	bounds.x = cam.near + (cam.far - cam.near) * farLimits[0];
	bounds.y = cam.near + (cam.far - cam.near) * farLimits[1];
	bounds.z = cam.near + (cam.far - cam.near) * farLimits[2];
	bounds.w = cam.near + (cam.far - cam.near) * farLimits[3];
	return bounds;
}
