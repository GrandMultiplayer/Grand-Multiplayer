// Needs to be first inorder to force winsock2 to be used
#include <WinSock2.h>

#include "DirectXHook.h"

#include <vector>
#include <unordered_map>

#include <WICTextureLoader.h>

#include <Detours.h>

// Vendor.CEF
#include <include/cef_app.h>
#include <include/cef_client.h>

#include "hook/enums.h"
#include "hook/types.h"
#include "Memory.h"
#include "Pattern.h"
#include "Hooking.h"

#include "SharedUtility.h"

#include "CefMainContext.h"
#include "CefTexture.h"

#include "CGui.h"
#include "CLocalPlayer.h"
#include "CStreamer.h"
#include "CPickup.h"
#include "CPlayer.h"
#include "CVehicle.h"
#include "CObject.h"
#include "CNPC.h"
#include "CNetworkManager.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

using namespace Hooking;

DirectXHook g_D3DHook;

//texture declariations
std::unordered_map<int, std::wstring> CreateTextureArray;
struct ScriptTex { DXTEXTURE2D tex; int id; };
struct ReloadTex { int id; std::wstring name; };
std::vector<ScriptTex> DrawTextureArray;
std::vector<ReloadTex> ReloadArray;
int DrawTextureArrayIndex = 0;
static Vector2 windowSize = Vector2();

//====================================================================================================================================================================
// Function hook stubs

// IDXGISwapChain::Present
DetourHook<HRESULT WINAPI(IDXGISwapChain*, UINT, UINT)> Detour_Present;
HRESULT WINAPI Present(IDXGISwapChain* chain, UINT syncInterval, UINT flags)
{
	if (!g_D3DHook.m_IsResizing)
	{
		if (g_D3DHook.m_pSwapchain == nullptr)
		{
			g_D3DHook.m_pSwapchain = chain;
			g_D3DHook.InitializeDevices();
		}
		else
		{
			BOOL fullscreenState;
			if (SUCCEEDED(chain->GetFullscreenState(&fullscreenState, nullptr)))
			{
				if (fullscreenState != g_D3DHook.m_fullscreenState)
				{
					g_D3DHook.m_IsResizing = true;

					g_D3DHook.ReleaseDevices(false);

					g_D3DHook.m_fullscreenState = fullscreenState;

					Detour_Present(chain, syncInterval, flags);

					g_D3DHook.m_IsResizing = false;
				}

			}

			g_D3DHook.Draw();
		}
	}

	return Detour_Present(chain, syncInterval, flags);
}

// IDXGISwapChain::ResizeBuffers
DetourHook<HRESULT WINAPI(IDXGISwapChain*, UINT, UINT, UINT, DXGI_FORMAT, UINT)> Detour_ResizeBuffers;
HRESULT WINAPI ResizeBuffers(IDXGISwapChain* chain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
{
	//if (g_HookState == HookStateRunning)
	{
		g_D3DHook.m_IsResizing = true;

		g_D3DHook.ReleaseDevices(false);

		Detour_ResizeBuffers(chain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

		g_D3DHook.m_IsResizing = false;

		return HRESULT();
	}

	return Detour_ResizeBuffers(chain, BufferCount, Width, Height, NewFormat, SwapChainFlags);
}

//====================================================================================================================================================================
//Ensure SwapChain Vtable and perform function hooks

enum eSwapChainVtableIndices : int
{
	SC_QUERYINTERFACE,
	SC_ADDREF,
	SC_RELEASE,
	SC_SETPRIVATEDATA,
	SC_SETPRIVATEDATAINTERFACE,
	SC_GETPRIVATEDATA,
	SC_GETPARENT,
	SC_GETDEVICE,
	SC_PRESENT,
	SC_GETBUFFER,
	SC_SETFULLSCREENSTATE,
	SC_GETFULLSCREENSTATE,
	SC_GETDESC,
	SC_RESIZEBUFFERS,
	SC_RESIZETARGET,
	SC_GETCONTAININGOUTPUT,
	SC_GETFRAMESTATISTICS,
	SC_GETLASTPRESENTCOUNT,
};

//48 8B 3D ? ? ? ? 48 8D 54 24 ? 48 8B 07 48 8B CF FF 50 60
static int64_t(*GetGtaSwapChain)() = "48 8B 05 ? ? ? ? C3 48 8B C1 8D 4A 0E"_Scan.as<decltype(GetGtaSwapChain)>();
bool DirectXHook::InitializeHooks()
{
	HMODULE hD3D11DLL = 0;
	while (!hD3D11DLL)
	{
		hD3D11DLL = GetModuleHandleA("d3d11.dll");
		Sleep(100);
	}

	if (auto swapchain = GetGtaSwapChain())
	{
		PVOID* pVTable = *(PVOID**)swapchain;

		Detour_Present.Hook(RCast(Present, pVTable[SC_PRESENT]), &Present, "IDXGISwapChainPresent");

		Detour_ResizeBuffers.Hook(RCast(ResizeBuffers, pVTable[SC_RESIZEBUFFERS]), &ResizeBuffers, "IDXGISwapChainResizeBuffers");

		return true;
	}

	return false;
}

//====================================================================================================================================================================
//Device Initialization and drawing functions

void DirectXHook::InitializeDevices()
{
	if (m_pSwapchain)
	{
		//get device and context
		if (SUCCEEDED(m_pSwapchain->GetDevice(__uuidof(ID3D11Device), (void **)&m_pDevice)))
		{
			m_pSwapchain->GetDevice(__uuidof(m_pDevice), (void**)&m_pDevice);
			m_pDevice->GetImmediateContext(&m_pContext);

			m_pCommonState.reset(new CommonStates(m_pDevice));
			m_pSpriteBatch.reset(new SpriteBatch(m_pContext));
			m_pBatch.reset(new PrimitiveBatch<VertexPositionColor>(m_pContext));
			m_stateSaver.reset(new StateSaver());
		}

		// use the back buffer address to create the render target
		if (SUCCEEDED(m_pSwapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&m_pRenderTargetTexture)))
		{
			m_pDevice->CreateRenderTargetView(m_pRenderTargetTexture, NULL, &m_pRenderTargetView);
			m_pRenderTargetTexture->Release();
			ReloadTextures();
		}

		BOOL fullscreenState;
		if (SUCCEEDED(m_pSwapchain->GetFullscreenState(&fullscreenState, nullptr)))
		{
			m_fullscreenState = fullscreenState;
			windowSize = GetResolution();

			//LOG_ERROR("[DirectXHook] Resolution is %i x %i", windowSize.x, windowSize.y);
		}
	}
}

void DirectXHook::ReleaseDevices(bool unhook)
{
	g_D3DHook.m_pRenderTargetTexture->Release();
	Utility::SafeRelease(m_pRenderTargetTexture);
	g_D3DHook.m_pRenderTargetView->Release();
	Utility::SafeRelease(m_pRenderTargetView);
	g_D3DHook.m_pContext->Release();
	Utility::SafeRelease(m_pContext);
	g_D3DHook.m_pDevice->Release();
	Utility::SafeRelease(m_pDevice);
	g_D3DHook.m_pSwapchain->Release();
	Utility::SafeRelease(m_pSwapchain);
	m_stateSaver.release();
	LOG_DEBUG("Unloaded D3DX11 Devices");

	if (unhook)
	{
		Detour_ResizeBuffers.UnHook();
		Detour_Present.UnHook();
	}
}

// ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_impl_dx11.h"
#define IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_PLACEMENT_NEW
#include "imgui/imgui_internal.h"

void DirectXHook::Draw()
{
	if (CefMainContext::Get() == nullptr || !CefMainContext::Get()->IsInitialized())
		return;

	CreateTextures();

	if (!DrawTextureArray.empty())
	{
		m_pSpriteBatch->Begin(SpriteSortMode::SpriteSortMode_BackToFront, m_pCommonState->NonPremultiplied());
		m_restoreState = m_stateSaver->saveCurrentState(m_pContext);
		m_pBatch->Begin();

		for (auto& t : DrawTextureArray)
		{
			if (t.tex.bEnabled) t.tex.Draw(*m_pSpriteBatch);
		}

		m_pBatch->End();
		m_pSpriteBatch->End();

		if (m_restoreState)
			m_stateSaver->restoreSavedState();
	}

	static bool FirstRender = false;
	if (!FirstRender)
	{
		//ImGuiIO& ioinit = ImGui::GetIO();
		//ioinit.Fonts->AddFontDefault();

		//Default (English & Cyrillic)
		/*ImFontConfig font_config;
		font_config.MergeMode = true;
		font_config.OversampleH = font_config.OversampleV = 1;
		font_config.PixelSnapH = true;
		ioinit.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\Roboto-Regular.ttf", 40.0f);

		ioinit.Fonts->Build();*/

		//ImGui_ImplDX11_Init(FindWindowA(NULL, "Grand Theft Auto V"), m_pDevice, m_pContext);
		//ImGui_ImplDX11_CreateDeviceObjects();

#ifndef _CEFDISABLED
		if (!CefTexture::SetupD3D())
			LOG_ERROR("[D3DDraw] Setup of CEF d3d texture failed");
#endif

		FirstRender = true;
	}

		
#ifndef _CEFDISABLED
	CefTexture::UpdateRenderTexture();

	CefTexture::DrawWebView();
#endif

	POINT cursor;
	GetCursorPos(&cursor);
	ScreenToClient(FindWindowA(NULL, "Grand Theft Auto V"), &cursor);
	/*
	ImGuiIO& io = ImGui::GetIO();
	if (CGui::IsCursorVisible())
	{
		io.MousePos.x = cursor.x;
		io.MousePos.y = cursor.y;
	}
		
	io.MouseDrawCursor = CGui::IsCursorVisible();

	ImGui_ImplDX11_NewFrame();
	{
		static float windowScale = 1.0f;
		static float textScale = 1.0f;
		Vector2 res = CGui::GetResolution();

		if (res.x < 1920.0f)
		{
			float diffrence = (1920.0f - res.x) / (1920.0f / 10.0f) * 0.1f;
			windowScale = 1.0f - diffrence;
			textScale = 1.0f - (diffrence - (diffrence / 2));
			if (textScale < 0.75f)
				textScale = 0.75f;
		}

		if (CNetworkManager::g_ConnectionState == CNetworkManager::ConenctionState::Connected && false)
		{
			ImGui::SetNextWindowPos(ImVec2(res.x - (600 * windowScale), res.y - (180 * windowScale)));
			ImGui::SetNextWindowSize(ImVec2((600 * windowScale), (180 * windowScale)));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 1.0f);
			ImGui::Begin("GrandMultiplayer_Debug", NULL, ImVec2(0, 0), 0.5f, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoSavedSettings);
			{
				ImGui::SetWindowFontScale(textScale / 2);
				CVector3 pos = CLocalPlayer::GetPosition();
				CVector3 velo = CLocalPlayer::GetVelocity();
				float SizeH;

				//Position Debug
				std::string text = "Pos { X: " + std::to_string((float)pos.x) + ", Y: " + std::to_string((float)pos.y) + ", Z: " + std::to_string((float)pos.z) + " }";
				SizeH = ImGui::CalcTextSize(text.c_str()).x;

				ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - SizeH);
				ImGui::TextUnformatted(text.c_str());
				ImGui::NewLine();

				//Velocity Debug
				text = "Velo { X: " + std::to_string((float)velo.x) + ", Y: " + std::to_string((float)velo.y) + ", Z: " + std::to_string((float)velo.z) + " }";
				SizeH = ImGui::CalcTextSize(text.c_str()).x;

				ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - SizeH);
				ImGui::TextUnformatted(text.c_str());
				ImGui::NewLine();

				//Heading & Forwardspeed Debug
				text = "Speed: " + std::to_string(CLocalPlayer::GetForwardSpeed()) + ", Heading: " + std::to_string((float)CLocalPlayer::GetRotation().z);
				SizeH = ImGui::CalcTextSize(text.c_str()).x;

				ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - SizeH);
				ImGui::TextUnformatted(text.c_str());
				ImGui::NewLine();

					
				text = "Network: ";

				text += " In: ";
				text += CString::to_string_with_precision(inbound, 2);
				text += "kbs";

				text += " Out: ";
				text += CString::to_string_with_precision(outbound, 2);
				text += "kbs";

				text += " Loss: ";
				text += CString::to_string_with_precision(loss, 2);

				SizeH = ImGui::CalcTextSize(text.c_str()).x;
				ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - SizeH);
				ImGui::TextUnformatted(text.c_str());
				ImGui::NewLine();


				text = "Health: " + std::to_string(CLocalPlayer::GetHealth());
				SizeH = ImGui::CalcTextSize(text.c_str()).x;
				ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - SizeH);
				ImGui::TextUnformatted(text.c_str());
				ImGui::NewLine();

				// Pools Debug
				text = "P: " + std::to_string(CStreamer::GetPlayerCount()) + "/" + std::to_string(g_Players.size()) +
					", V: " + std::to_string(CStreamer::GetVehicleCount()) + "/" + std::to_string(g_Vehicles.size()) +
					", O: " + std::to_string(CStreamer::GetObjectCount()) + "/" + std::to_string(g_Objects.size()) +
					", N: " + std::to_string(CStreamer::GetNpcCount()) + "/" + std::to_string(g_Npcs.size()) +
					", Pi: " + std::to_string(CStreamer::GetPickupCount()) + "/" + std::to_string(g_Pickups.size());
				SizeH = ImGui::CalcTextSize(text.c_str()).x;

				ImGui::SameLine(ImGui::GetWindowContentRegionMax().x - SizeH);
				ImGui::TextUnformatted(text.c_str());
			}
			ImGui::End();
			ImGui::PopStyleVar(1);
		}
	}
	ImGui::Render();
	*/

	for (auto & function : m_PresentCallbacks)
	{
		m_restoreState = m_stateSaver->saveCurrentState(m_pContext);
		function(m_pSwapchain);
		if (m_restoreState)
			m_stateSaver->restoreSavedState();
	}
}

// Doesn't seem to work... just getting 0,0 from RSGetViewports
Vector2 DirectXHook::GetResolution()
{
	D3D11_RECT drect;
	UINT nrects = 1;
	m_pContext->RSGetScissorRects(&nrects, &drect);
	return Vector2(drect.right - drect.left, drect.bottom - drect.top);


	/*D3D11_VIEWPORT vp;
	UINT nvp = 1;
	m_pContext->RSGetViewports(&nvp, &vp);
	return Vector2(vp.Width, vp.Height);*/
}


bool DirectXHook::IsFullscreen()
{
	BOOL state = false;
	if(FAILED(m_pSwapchain->GetFullscreenState(&state, NULL)))
		LOG_WARNING("Failed to get swapchain fullscreen state.");

	return state;
}

//====================================================================================================================================================================
//Custom Texture loading via WIC
void DirectXHook::CreateTextures()
{
	for (auto it = CreateTextureArray.begin(); it != CreateTextureArray.end(); ++it)
	{
		DXTEXTURE2D t;
		if (SUCCEEDED(CreateWICTextureFromFile(m_pDevice, it->second.c_str(), t.resource.GetAddressOf(), t.view.GetAddressOf(), 0)))
		{
			t.resource.As(&t.texture);
			t.texture->GetDesc(&t.desc);
			ScriptTex Temptex;
			Temptex.id = it->first;
			Temptex.tex = t;
			DrawTextureArray.push_back(Temptex);
			ReloadTex tempR;
			tempR.id = it->first;
			tempR.name = it->second;
			ReloadArray.push_back(tempR);
		}
		else
		{
			std::string string = CString::utf16ToUtf8(it->second);
			LOG_ERROR("Unable to Load Texture %s = %d", string.c_str(), it->first);
		}
	}

	CreateTextureArray.clear();
}

void DirectXHook::ReloadTextures()
{
	for (auto it : ReloadArray)
	{
		DXTEXTURE2D t;
		if (SUCCEEDED(CreateWICTextureFromFile(m_pDevice, it.name.c_str(), t.resource.GetAddressOf(), t.view.GetAddressOf(), 0)))
		{
			t.resource.As(&t.texture);
			t.texture->GetDesc(&t.desc);
			ScriptTex Temptex;
			Temptex.id = it.id;
			Temptex.tex = t;
			DrawTextureArray.push_back(Temptex);
		}
		else
		{
			std::string string = CString::utf16ToUtf8(it.name);
			LOG_ERROR("Unable to Load Texture %s = %d", string.c_str(), it.id);
		}
	}
}

int DirectXHook::CreateTexture(const char *texFileName)
{
	// convert the filename from UTF-8...
	std::string fileNameStr = texFileName;
	std::wstring passedFileName = CString::utf8ToUtf16(fileNameStr);
	std::wstring retFileName = passedFileName;

	// then, try finding the requested file.
	bool found = false;

	// absolute path
	if (passedFileName[1] == L':' || passedFileName[0] == '\\')
	{
		LOG_PRINT("File found %s", fileNameStr.c_str());
		found = true;
	}

	// not found at all?
	if (!found)
	{
		LOG_ERROR("File not found %s", fileNameStr.c_str());
		return -1;
	}

	DrawTextureArrayIndex++;
	CreateTextureArray[DrawTextureArrayIndex] = retFileName;
	return DrawTextureArrayIndex;
}

void DirectXHook::DrawTexture(int id, int index, int level, int time, float sizeX, float sizeY, float centerX, float centerY, float posX, float posY, float rotation, float screenHeightScaleFactor, float r, float g, float b, float a)
{
	for (auto& t : DrawTextureArray)
	{
		if (t.id == id)
			t.tex.SetProperties(id, index, level, time, sizeX, sizeY, centerX, centerY, posX, posY, rotation, screenHeightScaleFactor, r, g, b, a);
	}
}

//====================================================================================================================================================================
//State Saver Class

// Construct
StateSaver::StateSaver() :
	m_savedState(false),
	m_featureLevel(D3D_FEATURE_LEVEL_11_0),
	m_pContext(NULL),
	m_primitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED),
	m_pInputLayout(NULL),
	m_pBlendState(NULL),
	m_sampleMask(0xffffffff),
	m_pDepthStencilState(NULL),
	m_stencilRef(0),
	m_pRasterizerState(NULL),
	m_pPSSRV(NULL),
	m_pSamplerState(NULL),
	m_pVS(NULL),
	m_numVSClassInstances(0),
	m_pVSConstantBuffer(NULL),
	m_pGS(NULL),
	m_numGSClassInstances(0),
	m_pGSConstantBuffer(NULL),
	m_pGSSRV(NULL),
	m_pPS(NULL),
	m_numPSClassInstances(0),
	m_pHS(NULL),
	m_numHSClassInstances(0),
	m_pDS(NULL),
	m_numDSClassInstances(0),
	m_pVB(NULL),
	m_vertexStride(0),
	m_vertexOffset(0),
	m_pIndexBuffer(NULL),
	m_indexFormat(DXGI_FORMAT_UNKNOWN),
	m_indexOffset(0)
{
	for (int i = 0; i < 4; ++i)
	{
		m_blendFactor[i] = 0.0f;
	}
	for (int i = 0; i < 256; ++i)
	{
		m_pVSClassInstances[i] = NULL;
		m_pGSClassInstances[i] = NULL;
		m_pPSClassInstances[i] = NULL;
		m_pHSClassInstances[i] = NULL;
		m_pDSClassInstances[i] = NULL;
	}
}


// Destruct
StateSaver::~StateSaver()
{
	releaseSavedState();
}


// Save all states that are changed by the font-wrapper when drawing a string
bool StateSaver::saveCurrentState(ID3D11DeviceContext *pContext)
{
	if (m_savedState) releaseSavedState();
	if (pContext == NULL) return false;

	ID3D11Device *pDevice;
	pContext->GetDevice(&pDevice);
	if (pDevice != NULL)
	{
		m_featureLevel = pDevice->GetFeatureLevel();
		pDevice->Release();
	}

	pContext->AddRef();
	m_pContext = pContext;

	m_pContext->IAGetPrimitiveTopology(&m_primitiveTopology);
	m_pContext->IAGetInputLayout(&m_pInputLayout);

	m_pContext->OMGetBlendState(&m_pBlendState, m_blendFactor, &m_sampleMask);
	m_pContext->OMGetDepthStencilState(&m_pDepthStencilState, &m_stencilRef);

	m_pContext->RSGetState(&m_pRasterizerState);

	m_numVSClassInstances = 256;
	m_pContext->VSGetShader(&m_pVS, m_pVSClassInstances, &m_numVSClassInstances);
	m_pContext->VSGetConstantBuffers(0, 1, &m_pVSConstantBuffer);

	m_numPSClassInstances = 256;
	m_pContext->PSGetShader(&m_pPS, m_pPSClassInstances, &m_numPSClassInstances);
	m_pContext->PSGetShaderResources(0, 1, &m_pPSSRV);
	pContext->PSGetSamplers(0, 1, &m_pSamplerState);

	if (m_featureLevel >= D3D_FEATURE_LEVEL_10_0)
	{
		m_numGSClassInstances = 256;
		m_pContext->GSGetShader(&m_pGS, m_pGSClassInstances, &m_numGSClassInstances);
		m_pContext->GSGetConstantBuffers(0, 1, &m_pGSConstantBuffer);

		m_pContext->GSGetShaderResources(0, 1, &m_pGSSRV);

		if (m_featureLevel >= D3D_FEATURE_LEVEL_11_0)
		{
			m_numHSClassInstances = 256;
			m_pContext->HSGetShader(&m_pHS, m_pHSClassInstances, &m_numHSClassInstances);

			m_numDSClassInstances = 256;
			m_pContext->DSGetShader(&m_pDS, m_pDSClassInstances, &m_numDSClassInstances);
		}
	}

	m_pContext->IAGetVertexBuffers(0, 1, &m_pVB, &m_vertexStride, &m_vertexOffset);

	m_pContext->IAGetIndexBuffer(&m_pIndexBuffer, &m_indexFormat, &m_indexOffset);

	m_savedState = true;

	return true;
}


// Restore state
bool StateSaver::restoreSavedState()
{
	if (!m_savedState) return false;

	m_pContext->IASetPrimitiveTopology(m_primitiveTopology);
	m_pContext->IASetInputLayout(m_pInputLayout);

	m_pContext->OMSetBlendState(m_pBlendState, m_blendFactor, m_sampleMask);
	m_pContext->OMSetDepthStencilState(m_pDepthStencilState, m_stencilRef);

	m_pContext->RSSetState(m_pRasterizerState);

	m_pContext->VSSetShader(m_pVS, m_pVSClassInstances, m_numVSClassInstances);
	m_pContext->VSSetConstantBuffers(0, 1, &m_pVSConstantBuffer);

	m_pContext->PSSetShader(m_pPS, m_pPSClassInstances, m_numPSClassInstances);
	m_pContext->PSSetShaderResources(0, 1, &m_pPSSRV);
	m_pContext->PSSetSamplers(0, 1, &m_pSamplerState);

	if (m_featureLevel >= D3D_FEATURE_LEVEL_10_0)
	{
		m_pContext->GSSetShader(m_pGS, m_pGSClassInstances, m_numGSClassInstances);
		m_pContext->GSSetConstantBuffers(0, 1, &m_pGSConstantBuffer);

		m_pContext->GSSetShaderResources(0, 1, &m_pGSSRV);

		if (m_featureLevel >= D3D_FEATURE_LEVEL_11_0)
		{
			m_pContext->HSSetShader(m_pHS, m_pHSClassInstances, m_numHSClassInstances);

			m_pContext->DSSetShader(m_pDS, m_pDSClassInstances, m_numDSClassInstances);
		}
	}

	m_pContext->IASetVertexBuffers(0, 1, &m_pVB, &m_vertexStride, &m_vertexOffset);

	m_pContext->IASetIndexBuffer(m_pIndexBuffer, m_indexFormat, m_indexOffset);

	return true;
}


// Release state
void StateSaver::releaseSavedState()
{
	m_primitiveTopology = D3D11_PRIMITIVE_TOPOLOGY_UNDEFINED;
	Utility::SafeRelease(m_pInputLayout);
	Utility::SafeRelease(m_pBlendState);
	for (int i = 0; i < 4; ++i)
		m_blendFactor[i] = 0.0f;
	m_sampleMask = 0xffffffff;
	Utility::SafeRelease(m_pDepthStencilState);
	m_stencilRef = 0;
	Utility::SafeRelease(m_pRasterizerState);
	Utility::SafeRelease(m_pPSSRV);
	Utility::SafeRelease(m_pSamplerState);
	Utility::SafeRelease(m_pVS);
	for (UINT i = 0; i < m_numVSClassInstances; ++i)
		Utility::SafeRelease(m_pVSClassInstances[i]);
	m_numVSClassInstances = 0;
	Utility::SafeRelease(m_pVSConstantBuffer);
	Utility::SafeRelease(m_pGS);
	for (UINT i = 0; i < m_numGSClassInstances; ++i)
		Utility::SafeRelease(m_pGSClassInstances[i]);
	m_numGSClassInstances = 0;
	Utility::SafeRelease(m_pGSConstantBuffer);
	Utility::SafeRelease(m_pGSSRV);
	Utility::SafeRelease(m_pPS);
	for (UINT i = 0; i < m_numPSClassInstances; ++i)
		Utility::SafeRelease(m_pPSClassInstances[i]);
	m_numPSClassInstances = 0;
	Utility::SafeRelease(m_pHS);
	for (UINT i = 0; i < m_numHSClassInstances; ++i)
		Utility::SafeRelease(m_pHSClassInstances[i]);
	m_numHSClassInstances = 0;
	Utility::SafeRelease(m_pDS);
	for (UINT i = 0; i < m_numDSClassInstances; ++i)
		Utility::SafeRelease(m_pDSClassInstances[i]);
	m_numDSClassInstances = 0;
	Utility::SafeRelease(m_pVB);
	m_vertexStride = 0;
	m_vertexOffset = 0;
	Utility::SafeRelease(m_pIndexBuffer);
	m_indexFormat = DXGI_FORMAT_UNKNOWN;
	m_indexOffset = 0;

	Utility::SafeRelease(m_pContext);
	m_featureLevel = D3D_FEATURE_LEVEL_11_0;

	m_savedState = false;
}
