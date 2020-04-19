#include <winsock2.h>
#include <Windows.h>
#include <mutex>
#include <list>

#include "Common.h"
#include "DirectXHook.h"
#include "CGui.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

#include "CefTexture.h"


ID3D11RenderTargetView* CefTexture::g_mainRenderTargetView = nullptr;

ID3DBlob *CefTexture::PixelBlob = nullptr;
ID3D10Blob *CefTexture::VertexBlob = nullptr;

ID3D11Buffer *CefTexture::WebViewGeometryBuffer = nullptr;
ID3D11Texture2D *CefTexture::Texture = nullptr;
ID3D11ShaderResourceView *CefTexture::TextureShaderResourceView = nullptr;
ID3D11InputLayout *CefTexture::InputLayout = nullptr;
ID3D11PixelShader *CefTexture::PixelShader = nullptr;
ID3D11SamplerState *CefTexture::Sampler = nullptr;
ID3D11BlendState *CefTexture::BlendState = nullptr;
ID3D11RasterizerState *CefTexture::RasterizerState = nullptr;
ID3D11DepthStencilState *CefTexture::DepthStencilState = nullptr;
ID3D11VertexShader *CefTexture::VertexShader = nullptr;

unsigned CefTexture::TextureWidth = 1;
unsigned CefTexture::TextureHeight = 1;

std::mutex CefTexture::paintMutex;
std::list<std::unique_ptr<DrawData>> CefTexture::drawData;

const char CefTexture::PixelShaderSource[] = {
	"Texture2D diffuse;\
	sampler sampler0;\
	\
	struct INPUT \
	{\
		float2 uv : TEXCOORD; \
	};\
	\
	float4 main(INPUT input) : SV_TARGET\
	{\
		float4 output = diffuse.Sample(sampler0, input.uv); \
		return output; \
	}"
};

const char CefTexture::VertexShaderSource[] = {
	"struct INPUT\
		{\
			float2 pos : POSITION;\
			float2 uv : TEXCOORD;\
		};\
		\
		struct OUTPUT \
		{ \
			float2 uv : TEXCOORD; \
			float4 pos : SV_POSITION; \
		}; \
		\
		OUTPUT main(INPUT input)\
		{\
			OUTPUT output; \
			output.uv = input.uv; \
			output.pos = float4(input.pos.x, input.pos.y, 0.0f, 1.0f);\
			return output;\
		}"
};

bool CefTexture::CreateShaders()
{
	ID3DBlob *ErrorMsg = nullptr;
	if (FAILED(D3DCompile(PixelShaderSource, strlen(PixelShaderSource), NULL, NULL, NULL, "main", "ps_4_0", 0, 0, &PixelBlob, &ErrorMsg)))
	{
		const char *const ERROR_BASE = "Failed to compile pixel shader.";
		char *const error = new char[strlen(ERROR_BASE) + ErrorMsg->GetBufferSize() + strlen(PixelShaderSource) + 1 + 2 /*new lines */];
		sprintf(error, "%s\n%s\n%s", ERROR_BASE, (const char *)ErrorMsg->GetBufferPointer(), PixelShaderSource);
		MessageBoxA(NULL, error, NULL, NULL);
		delete[]error;
		return false;
	}
	
	if (FAILED(g_D3DHook.GetDevice()->CreatePixelShader(PixelBlob->GetBufferPointer(), PixelBlob->GetBufferSize(), NULL, &PixelShader)))
	{
		MessageBoxA(NULL, "Failed to create pixel shader.", NULL, NULL);
		return false;
	}

	if (FAILED(D3DCompile(VertexShaderSource, strlen(VertexShaderSource), NULL, NULL, NULL, "main", "vs_4_0", 0, 0, &VertexBlob, &ErrorMsg)))
	{
		const char *const ERROR_BASE = "Failed to compile vertex shader.";
		char *const error = new char[strlen(ERROR_BASE) + ErrorMsg->GetBufferSize() + strlen(VertexShaderSource) + 1 + 2 /*new lines */];
		sprintf(error, "%s\n%s\n%s", ERROR_BASE, (const char *)ErrorMsg->GetBufferPointer(), VertexShaderSource);
		MessageBoxA(NULL, error, NULL, NULL);
		delete[]error;
		return false;
	}

	if (FAILED(g_D3DHook.GetDevice()->CreateVertexShader(VertexBlob->GetBufferPointer(), VertexBlob->GetBufferSize(), NULL, &VertexShader)))
	{
		MessageBoxA(NULL, "Failed to create vertex shader.", NULL, NULL);
		return false;
	}

	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.MipLODBias = 0.f;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.MinLOD = 0.f;
	desc.MaxLOD = 0.f;
	g_D3DHook.GetDevice()->CreateSamplerState(&desc, &Sampler);
	return true;
}

bool CefTexture::CreateSampler()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.MipLODBias = 0.f;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.MinLOD = 0.f;
	desc.MaxLOD = 0.f;

	if (FAILED(g_D3DHook.GetDevice()->CreateSamplerState(&desc, &Sampler)))
	{
		MessageBoxA(NULL, "Failed to create sampler state.", NULL, NULL);
		return false;
	}
	return true;
}

bool CefTexture::CreateWebViewGeometryBuffer()
{
	// 1/4 of the screen, top right to bottom left
	/*Vertex2d rect[6] = {
	{ 0.0f, 0.0f, 0.0f, 1.0f },
	{ 1.0f, 0.0f, 1.0f, 1.0f },
	{ 0.0f, 1.0f, 0.0f, 0.0f },

	{ 0.0f, 1.0f, 0.0f, 0.0f },
	{ 1.0f, 0.0f, 1.0f, 1.0f },
	{ 1.0f, 1.0f, 1.0f, 0.0f }
	};*/

	// fullscreen
	Vertex2d rect[6] = {
		{ -1.0f, 1.0f, 0.0f, 0.0f },
		{ 1.0f, 1.0f, 1.0f, 0.0f },
		{ -1.0f, -1.0f, 0.0f, 1.0f },

		{ -1.0f, -1.0f, 0.0f, 1.0f },
		{ 1.0f, 1.0f, 1.0f, 0.0f },
		{ 1.0f, -1.0f, 1.0f, 1.0f }
	};

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(bufferDesc));

	bufferDesc.ByteWidth = sizeof(Vertex2d) * 6;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA data;
	memset(&data, 0, sizeof(data));
	data.pSysMem = rect;
	
	if (FAILED(g_D3DHook.GetDevice()->CreateBuffer(&bufferDesc, &data, &WebViewGeometryBuffer)))
	{
		MessageBoxA(NULL, "Failed to create buffer", NULL, NULL);
		return false;
	}

	if (!VertexBlob) return false;

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex2d, x), D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, offsetof(Vertex2d, u), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	if (FAILED(g_D3DHook.GetDevice()->CreateInputLayout(layout, 2, VertexBlob->GetBufferPointer(), VertexBlob->GetBufferSize(), &InputLayout)))
	{
		MessageBoxA(NULL, "Failed to create input layout", NULL, NULL);
		return false;
	}

	VertexBlob->Release();
	VertexBlob = nullptr;

	PixelBlob->Release();
	PixelBlob = nullptr;
	return true;
}

bool CefTexture::CreateTexture(unsigned Width, unsigned Height)
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = Width;
	desc.Height = Height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.SampleDesc.Count = 1;
	desc.MiscFlags = 0;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HRESULT hr = g_D3DHook.GetDevice()->CreateTexture2D(&desc, NULL, &Texture);
	if (hr != S_OK)
	{
		char error[256] = { 0 };
		sprintf(error, "Failed to create texture (%X)", hr);
		MessageBoxA(NULL, error, NULL, NULL);
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = desc.MipLevels;
	srvDesc.Texture2D.MostDetailedMip = 0;

	hr = g_D3DHook.GetDevice()->CreateShaderResourceView(Texture, &srvDesc, &TextureShaderResourceView);
	if (hr != S_OK)
	{
		char error[256] = { 0 };
		sprintf(error, "Failed to create shader resource view for texture (%X)", hr);
		MessageBoxA(NULL, error, NULL, NULL);
		return false;
	}

	TextureWidth = Width;
	TextureHeight = Height;
	return true;
}

void CefTexture::UpdateRenderTexture()
{
	std::unique_ptr<DrawData> data;
	{
		std::lock_guard<std::mutex> lock(paintMutex);
		if (drawData.empty())
			return;
		
		data = std::move(drawData.front());
		drawData.pop_front();
	}

	// In case texture size changed recreate it (also handles creation)
	//LOG_DEBUG("%i : %i - %i : %i", TextureWidth, data->width, TextureHeight, data->height);
	if (TextureWidth != data->width || TextureHeight != data->height)
	{
		LOG_WARNING("[CefTexture] Texture size does not match currect resolution %i x %i >> %i x %i.", TextureWidth, TextureHeight, data->width, data->height);
		DestroyTexture();
		CreateTexture(data->width, data->height);
	}

	if (!Texture)
		return;


	D3D11_MAPPED_SUBRESOURCE subRes;
	if (FAILED(g_D3DHook.GetContext()->Map(Texture, 0, D3D11_MAP_WRITE_DISCARD, 0, &subRes)))
	{
		LOG_ERROR("[CefTexture] Device Context map failed.");
		return;
	}

	unsigned *pixels = (unsigned *)subRes.pData;
	const unsigned *cefPixels = (const unsigned *)data->pixels;

	memcpy(pixels, cefPixels, 4 * TextureWidth * TextureHeight);

	g_D3DHook.GetContext()->Unmap(Texture, 0);
	return;
}

void CefTexture::DestroyTexture()
{
	SAFE_RELEASE(Texture);
}

void CefTexture::CreateRenderTarget()
{
	DXGI_SWAP_CHAIN_DESC sd;
	g_D3DHook.GetSwapChain()->GetDesc(&sd);

	/*ID3D11Texture2D *pTexture = NULL;
	D3D11_SUBRESOURCE_DATA subResource;
	subResource.pSysMem = pixels;
	subResource.SysMemPitch = desc.Width * 4;
	subResource.SysMemSlicePitch = 0;
	g_pd3dDevice->CreateTexture2D(&desc, &subResource, &pTexture);*/

	// Create the render target
	ID3D11Texture2D* pBackBuffer = NULL;
	D3D11_RENDER_TARGET_VIEW_DESC render_target_view_desc;
	ZeroMemory(&render_target_view_desc, sizeof(render_target_view_desc));
	render_target_view_desc.Format = sd.BufferDesc.Format;
	render_target_view_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	g_D3DHook.GetSwapChain()->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	g_D3DHook.GetDevice()->CreateRenderTargetView(pBackBuffer, &render_target_view_desc, &g_mainRenderTargetView);
	g_D3DHook.GetContext()->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
	pBackBuffer->Release();
}

void CefTexture::CleanupRenderTarget()
{
	if (g_mainRenderTargetView)
	{
		g_mainRenderTargetView->Release();
		g_mainRenderTargetView = NULL;
	}
}

void CefTexture::DrawWebView()
{
	if (!WebViewGeometryBuffer)
	{
		LOG_WARNING("[CefTexture] No Geometry Buffer...");
		return;
	}

	// Backup Old
	struct BACKUP_DX11_STATE
	{
		UINT                        ScissorRectsCount, ViewportsCount;
		D3D11_RECT                  ScissorRects[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
		D3D11_VIEWPORT              Viewports[D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE];
		ID3D11RasterizerState*      RS;
		ID3D11BlendState*           BlendState;
		FLOAT                       BlendFactor[4];
		UINT                        SampleMask;
		UINT                        StencilRef;
		ID3D11DepthStencilState*    DepthStencilState;
		ID3D11ShaderResourceView*   PSShaderResource;
		ID3D11SamplerState*         PSSampler;
		ID3D11PixelShader*          PS;
		ID3D11VertexShader*         VS;
		UINT                        PSInstancesCount, VSInstancesCount;
		ID3D11ClassInstance*        PSInstances[256], *VSInstances[256];   // 256 is max according to PSSetShader documentation 
		D3D11_PRIMITIVE_TOPOLOGY    PrimitiveTopology;
		ID3D11Buffer*               IndexBuffer, *VertexBuffer, *VSConstantBuffer;
		UINT                        IndexBufferOffset, VertexBufferStride, VertexBufferOffset;
		DXGI_FORMAT                 IndexBufferFormat;
		ID3D11InputLayout*          InputLayout;
	};
	BACKUP_DX11_STATE old;
	old.ScissorRectsCount = old.ViewportsCount = D3D11_VIEWPORT_AND_SCISSORRECT_OBJECT_COUNT_PER_PIPELINE;
	g_D3DHook.GetContext()->RSGetScissorRects(&old.ScissorRectsCount, old.ScissorRects);
	g_D3DHook.GetContext()->RSGetViewports(&old.ViewportsCount, old.Viewports);
	g_D3DHook.GetContext()->RSGetState(&old.RS);
	g_D3DHook.GetContext()->OMGetBlendState(&old.BlendState, old.BlendFactor, &old.SampleMask);
	g_D3DHook.GetContext()->OMGetDepthStencilState(&old.DepthStencilState, &old.StencilRef);
	g_D3DHook.GetContext()->PSGetShaderResources(0, 1, &old.PSShaderResource);
	g_D3DHook.GetContext()->PSGetSamplers(0, 1, &old.PSSampler);
	old.PSInstancesCount = old.VSInstancesCount = 256;
	g_D3DHook.GetContext()->PSGetShader(&old.PS, old.PSInstances, &old.PSInstancesCount);
	g_D3DHook.GetContext()->VSGetShader(&old.VS, old.VSInstances, &old.VSInstancesCount);
	g_D3DHook.GetContext()->VSGetConstantBuffers(0, 1, &old.VSConstantBuffer);
	g_D3DHook.GetContext()->IAGetPrimitiveTopology(&old.PrimitiveTopology);
	g_D3DHook.GetContext()->IAGetIndexBuffer(&old.IndexBuffer, &old.IndexBufferFormat, &old.IndexBufferOffset);
	g_D3DHook.GetContext()->IAGetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset);
	g_D3DHook.GetContext()->IAGetInputLayout(&old.InputLayout);

	/*RECT rect;
	GetClientRect(FindWindowA(NULL, "Grand Theft Auto V"), &rect);
	LOG_DEBUG("CefTexture::DrawWebView %i - %i x %i - %i", rect.left, rect.right, rect.top, rect.bottom);
	
	// Start CEF Set state
	const D3D11_VIEWPORT viewport = {
		0.0f, 0.0f,
		rect.left - rect.right, rect.top - rect.bottom,
		0.0f, 1.0f
	};*/
	
	Vector2 resolution = CGui::GetResolution();

	// Start CEF Set state
	const D3D11_VIEWPORT viewport = {
		0.0f, 0.0f,
		resolution.x, resolution.y,
		0.0f, 1.0f
	};

	g_D3DHook.GetContext()->RSSetViewports(1, &viewport);
	{
		D3D11_BLEND_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AlphaToCoverageEnable = false;
		desc.RenderTarget[0].BlendEnable = true;
		desc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		if (FAILED(g_D3DHook.GetDevice()->CreateBlendState(&desc, &BlendState)))
			LOG_ERROR("[CefTexture] Failed to create Blend State.");
	}


	UINT Stride = sizeof(Vertex2d);
	UINT Offset = 0;

	g_D3DHook.GetContext()->IASetInputLayout(InputLayout);
	g_D3DHook.GetContext()->IASetVertexBuffers(0, 1, &WebViewGeometryBuffer, &Stride, &Offset);
	g_D3DHook.GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	g_D3DHook.GetContext()->VSSetShader(VertexShader, NULL, 0);

	g_D3DHook.GetContext()->PSSetShaderResources(0, 1, &TextureShaderResourceView);
	g_D3DHook.GetContext()->PSSetSamplers(0, 1, &Sampler);
	g_D3DHook.GetContext()->PSSetShader(PixelShader, NULL, 0);

	const float blend_factor[4] = { 0.f, 0.f, 0.f, 0.f };
	g_D3DHook.GetContext()->OMSetBlendState(BlendState, blend_factor, 0xffffffff);
	{
		D3D11_RASTERIZER_DESC desc;
		memset(&desc, 0, sizeof(desc));
		desc.FillMode = /*D3D11_FILL_WIREFRAME*/D3D11_FILL_SOLID;
		desc.CullMode = D3D11_CULL_NONE;
		desc.ScissorEnable = true;
		desc.DepthClipEnable = true;
		if (FAILED(g_D3DHook.GetDevice()->CreateRasterizerState(&desc, &RasterizerState)))
			LOG_ERROR("[CefTexture] Failed to create Rasterizer State.");

	}

	g_D3DHook.GetContext()->RSSetState(RasterizerState);
	{
		D3D11_DEPTH_STENCIL_DESC desc;
		memset(&desc, 0, sizeof(desc));
		desc.DepthEnable = false;
		desc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		desc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		desc.StencilEnable = false;
		desc.FrontFace.StencilFailOp = desc.FrontFace.StencilDepthFailOp = desc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		desc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		desc.BackFace = desc.FrontFace;
		if (FAILED(g_D3DHook.GetDevice()->CreateDepthStencilState(&desc, &DepthStencilState)))
			LOG_ERROR("[CefTexture] Failed to create Depth Stencil State.");
	}

	g_D3DHook.GetContext()->OMSetDepthStencilState(DepthStencilState, 0);

	g_D3DHook.GetContext()->Draw(6, 0);


	// Restore modified DX state 
	g_D3DHook.GetContext()->RSSetScissorRects(old.ScissorRectsCount, old.ScissorRects);
	g_D3DHook.GetContext()->RSSetViewports(old.ViewportsCount, old.Viewports);
	g_D3DHook.GetContext()->RSSetState(old.RS); if (old.RS) old.RS->Release();
	g_D3DHook.GetContext()->OMSetBlendState(old.BlendState, old.BlendFactor, old.SampleMask); if (old.BlendState) old.BlendState->Release();
	g_D3DHook.GetContext()->OMSetDepthStencilState(old.DepthStencilState, old.StencilRef); if (old.DepthStencilState) old.DepthStencilState->Release();
	g_D3DHook.GetContext()->PSSetShaderResources(0, 1, &old.PSShaderResource); if (old.PSShaderResource) old.PSShaderResource->Release();
	g_D3DHook.GetContext()->PSSetSamplers(0, 1, &old.PSSampler); if (old.PSSampler) old.PSSampler->Release();
	g_D3DHook.GetContext()->PSSetShader(old.PS, old.PSInstances, old.PSInstancesCount); if (old.PS) old.PS->Release();
	for (UINT i = 0; i < old.PSInstancesCount; i++) if (old.PSInstances[i]) old.PSInstances[i]->Release();
	g_D3DHook.GetContext()->VSSetShader(old.VS, old.VSInstances, old.VSInstancesCount); if (old.VS) old.VS->Release();
	g_D3DHook.GetContext()->VSSetConstantBuffers(0, 1, &old.VSConstantBuffer); if (old.VSConstantBuffer) old.VSConstantBuffer->Release();
	for (UINT i = 0; i < old.VSInstancesCount; i++) if (old.VSInstances[i]) old.VSInstances[i]->Release();
	g_D3DHook.GetContext()->IASetPrimitiveTopology(old.PrimitiveTopology);
	g_D3DHook.GetContext()->IASetIndexBuffer(old.IndexBuffer, old.IndexBufferFormat, old.IndexBufferOffset); if (old.IndexBuffer) old.IndexBuffer->Release();
	g_D3DHook.GetContext()->IASetVertexBuffers(0, 1, &old.VertexBuffer, &old.VertexBufferStride, &old.VertexBufferOffset); if (old.VertexBuffer) old.VertexBuffer->Release();
	g_D3DHook.GetContext()->IASetInputLayout(old.InputLayout); if (old.InputLayout) old.InputLayout->Release();
}

bool CefTexture::SetupD3D()
{
	if (!CreateShaders())
	{
		LOG_ERROR("[CefTexture] Failed to create Shaders.");
		return false;
	}

	if (!CreateSampler())
	{
		LOG_ERROR("[CefTexture] Failed to create Sampler.");
		return false;
	}

	if (!CreateWebViewGeometryBuffer())
	{
		LOG_ERROR("[CefTexture] Failed to create Web View Geometry Buffer.");
		return false;
	}

	LOG_DEBUG("[CefTexture] D3D Setup finished.");
	return true;
}
