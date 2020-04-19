#include "CGui.h"

// Vendor/Cef
#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>
#include <include/cef_sandbox_win.h>

#include "SharedUtility.h"

#include "CefMainContext.h"

#include "OffscreenClient.h"

#include "CVector3.h"

#include "CLocalPlayer.h"

#include "CPlayer.h"
#include "CVehicle.h"
#include "CObject.h"
#include "CNPC.h"
#include "CPickup.h"

#include "CConfig.h"
#include "CStreamer.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

bool CGui::displayLoadingScreen = true;
bool CGui::displayMainMenu = false;
bool CGui::displayChat = false;
bool CGui::displayGameSettings = false;
bool CGui::displayCursor = false;

float CGui::Debug::inbound = 0;
float CGui::Debug::outbound = 0;
float CGui::Debug::loss = 0;

int CGui::Debug::fps = 0;
int CGui::Debug::ping = 0;
std::chrono::system_clock::time_point CGui::Debug::timeNow = std::chrono::system_clock::now();
std::chrono::system_clock::time_point CGui::Debug::timeLast = std::chrono::system_clock::now();
std::chrono::duration<double, std::milli> CGui::Debug::timeTook = timeNow - timeLast;

std::vector<std::string> CGui::frames;

Vector2 CGui::resolution = Vector2();

void CGui::Init()
{
	// Initally Set Setting Values
	std::string codeValueSetup =
		"$('#s-nickname').val(GrandM.GetOption('name'));"
		""
		"$('#s-ip').val(GrandM.GetOption('ip'));"
		""
		"$('#dc_ip').val(GrandM.GetOption('ip'));"
		""
		"$('#s-port').val(GrandM.GetOption('port'));"
		""
		"$('#dc_port').val(GrandM.GetOption('port'));"
		""
		"$('#s-chat').prop('checked', GrandM.GetOption('chatvisible')).change();"
		""
		"$('#s-debug').prop('checked', GrandM.GetOption('debugui')).change();"
		""
		"$('#s-editor').prop('checked', GrandM.GetOption('editor')).change();"
		""
		"slider.setValue(GrandM.GetOption('chatmessagecount'), true, true);"
		""
		"$('#s-chatsize').selectpicker('val', GrandM.GetOption('chatsize'));"
		;

	// Setup Settings to change config on value changes
	std::string codeValueChangeSetup =
		"$('#s-nickname').change(function () {"
		"GrandM.SetOption('name', $(this).val());"
		"});"
		""
		"$('#s-ip').change(function () {"
		"GrandM.SetOption('ip', $(this).val());"
		"});"
		""
		"$('#s-port').change(function () {"
		"GrandM.SetOption('port', parseInt($(this).val()));"
		"});"
		""
		"$('#s-chat').change(function () {"
		"GrandM.SetOption('chatvisible', $(this).is(':checked'));"
		"});"
		""
		"$('#s-debug').change(function () {"
		"GrandM.SetOption('debugui', $(this).is(':checked'));"
		"});"
		""
		"$('#s-editor').change(function () {"
		"GrandM.SetOption('editor', $(this).is(':checked'));"
		"});"
		""
		"slider.on('slide', function(val) {"
		"GrandM.SetOption('chatmessagecount', val);"
		"});"
		""
		"$('#s-chatsize').change(function () {"
		"GrandM.SetOption('chatsize', $(this).val());"
		"});"
		;

#ifndef _CEFDISABLED
	CefRefPtr<CefFrame> frame = CefMainContext::getBrowser()->GetMainFrame();


	if (frame != NULL && frame->IsValid())
	{
		frame->ExecuteJavaScript(codeValueSetup, frame->GetURL(), 0);
		frame->ExecuteJavaScript(codeValueChangeSetup, frame->GetURL(), 0);
	}
	else
		LOG_ERROR("[CEF] Main Frame is null!");
#endif

	CGui::LoadURL("grandm.chat", "./chat.html", false, false);

	CGui::LoadURL("grandm.debug", "./debug.html", false, false);

#ifndef _CEFDISABLED
	std::string debug = (CConfig::IsDebugUI() ? "$('#debugFrame').show()" : "$('#debugFrame').hide()");

	if (frame != NULL && frame->IsValid())
	{
		frame->ExecuteJavaScript(debug, frame->GetURL(), 0);
	}
	else
		LOG_ERROR("[CEF] Main Frame is null!");
#endif
}

void CGui::ShowLoadingScreen(const bool state, const bool tOM)
{
	displayLoadingScreen = state;
	
	if (tOM)
	{
		ShowMainMenu(false);
		ShowChat(false);
	}

#ifndef _CEFDISABLED
	CefRefPtr<CefFrame> frame = CefMainContext::getBrowser()->GetMainFrame();

	if (frame != nullptr && frame->IsValid())
	{
		if (state)
			frame->ExecuteJavaScript(CefString("$('#loadingFrame').show();"), frame->GetURL(), 0);
		else
			frame->ExecuteJavaScript(CefString("$('#loadingFrame').hide();"), frame->GetURL(), 0);
	}
#endif
}

void CGui::ShowMainMenu(const bool state, const bool tOM)
{
	displayMainMenu = state;
	
	if (tOM)
	{
		ShowLoadingScreen(false);
		ShowChat(false);
	}

	displayCursor = state;

#ifndef _CEFDISABLED
	CefRefPtr<CefFrame> frame = CefMainContext::getBrowser()->GetMainFrame();

	if (frame != nullptr && frame->IsValid())
	{
		if (state)
			frame->ExecuteJavaScript(CefString("$('#MainMenu').show();"), frame->GetURL(), 0);
		else
			frame->ExecuteJavaScript(CefString("$('#MainMenu').hide();"), frame->GetURL(), 0);
	}
#endif
	CGui::GiveFocus(true);
}

void CGui::ShowChat(const bool state, const bool tOM)
{
	displayChat = state;

	if (tOM)
	{
		ShowMainMenu(false);
		ShowLoadingScreen(false);
	}

#ifndef _CEFDISABLED
	CefRefPtr<CefFrame> frame = CefMainContext::getBrowser()->GetMainFrame();

	if (frame != nullptr && frame->IsValid())
	{
		if (state)
			frame->ExecuteJavaScript(CefString("$('#chatFrame').show();"), frame->GetURL(), 0);
		else
			frame->ExecuteJavaScript(CefString("$('#chatFrame').hide();"), frame->GetURL(), 0);
	}
#endif
}

void CGui::SetVersion(std::string version)
{
#ifndef _CEFDISABLED
	CefRefPtr<CefFrame> frame = CefMainContext::getBrowser()->GetMainFrame();

	if (frame != nullptr && frame->IsValid())
		frame->ExecuteJavaScript(CefString(std::string("$('#text_Version').html('" + version + "');")), frame->GetURL(), 0);
#endif
}

void CGui::Refresh()
{
#ifndef _CEFDISABLED
	wchar_t  buffer[MAX_PATH];
	GetModuleFileName(GetModuleHandle(L"Client.dll"), buffer, sizeof(buffer));

	std::wstring filePath = buffer;
	filePath.erase(filePath.find(L"Client.dll"), std::wstring(L"Client.dll").size());
	filePath = std::wstring(filePath + L"ui\\index.html");

	CefRefPtr<CefFrame> frame = CefMainContext::getBrowser()->GetMainFrame();

	if (frame != nullptr && frame->IsValid())
		frame->LoadURL(filePath);

	if (displayMainMenu)
	{
		ShowMainMenu(true);
	}
	else if (displayLoadingScreen)
	{
		ShowLoadingScreen(true);
	}
	else if (displayChat)
	{
		ShowChat(true);
	}
#endif
}

void CGui::ClearChat()
{
#ifndef _CEFDISABLED
	CefRefPtr<CefFrame> frame = CefMainContext::getBrowser()->GetFrame(CefString("grandm.chat"));

	if (frame != nullptr && frame->IsValid())
		frame->ExecuteJavaScript(CefString("ClearChatMessages();"), frame->GetURL(), 0);
#endif
}

void CGui::ClearFrame()
{
	//CGui::LoadURL("grandm.server.ui", "about:blank");
}

void CGui::SetConnected(const bool state)
{
#ifndef _CEFDISABLED
	CefRefPtr<CefFrame> frame = CefMainContext::getBrowser()->GetMainFrame();

	if (frame != nullptr && frame->IsValid())
	{
		if (state)
			frame->ExecuteJavaScript(CefString("$('#button_Disconnect').show();"), frame->GetURL(), 0);
		else
			frame->ExecuteJavaScript(CefString("$('#button_Disconnect').hide();"), frame->GetURL(), 0);
	}
#endif
}

void CGui::GiveFocus(const bool dCursor)
{
	displayCursor = dCursor;
}

void CGui::ShowConnectionError(std::string error, std::string message)
{
#ifndef _CEFDISABLED
	CefRefPtr<CefFrame> frame = CefMainContext::getBrowser()->GetMainFrame();

	if (frame != nullptr && frame->IsValid())
	{
		frame->ExecuteJavaScript(CefString(std::string("$('#connectionErrorModalhead').html('" + error + "');")), frame->GetURL(), 0);
		frame->ExecuteJavaScript(CefString(std::string("$('#connectionErrorModalbody').html('" + message + "');")), frame->GetURL(), 0);
		frame->ExecuteJavaScript(CefString("$('#connectionErrorModal').modal({ show: true });"), frame->GetURL(), 0);
	}
#endif
}

void CGui::SetLoadingText(std::string text)
{
#ifndef _CEFDISABLED
	CefRefPtr<CefFrame> frame = CefMainContext::getBrowser()->GetFrame(CefString("grandm.loading"));

	if (frame != nullptr && frame->IsValid())
		frame->ExecuteJavaScript(CefString("document.getElementById('loadingText').innerHTML = '" + text + "'"), frame->GetURL(), 0);
	else
		LOG_ERROR("[CGui::SetLoadingText] grandm.loading frame has magicly vanished....");
#endif
}

void CGui::ShowChatMessage(std::string message)
{
#ifndef _CEFDISABLED
	CefRefPtr<CefFrame> frame = CefMainContext::getBrowser()->GetFrame(CefString("grandm.chat"));
	
	if (frame != nullptr && frame->IsValid())
		frame->ExecuteJavaScript(CefString("ShowChatMessage(\"" + message + "\");"), frame->GetURL(), 0);
	else
		LOG_ERROR("[CGui::ShowChatMessage] grandm.chat frame has magicly vanished....");
#endif
}

const bool CGui::DoesFrameExist(std::string frame)
{
	for each (std::string f in frames)
	{
		if (f.compare(frame) == 0)
			return true;
	}
	return false;
}

void CGui::CreateFrame(std::string frame)
{
	if (DoesFrameExist(frame))
		return;

#ifndef _CEFDISABLED
	CefRefPtr<CefFrame> mainFrame = CefMainContext::getBrowser()->GetMainFrame();

	if (mainFrame != nullptr && mainFrame->IsValid())
		mainFrame->ExecuteJavaScript(
			CefString("$('#dynFrames').append('<iframe id='" + frame + "' class='grandframe' name='" + frame + "' src='about:blank'></iframe>');")
			, mainFrame->GetURL(), 0);
#endif
	
	frames.push_back(frame);
}

void CGui::RemoveFrame(std::string frame)
{
	if (!DoesFrameExist(frame))
		return;

	for (unsigned int i = 0; i < frames.size(); i++)
	{
		if (frames[i].compare(frame) == 0)
		{
			frames.erase(frames.begin() + i);
			frames.shrink_to_fit();

#ifndef _CEFDISABLED
			CefRefPtr<CefFrame> mainFrame = CefMainContext::getBrowser()->GetMainFrame();

			if (mainFrame != nullptr && mainFrame->IsValid())
				mainFrame->ExecuteJavaScript(CefString("$('#'" + frame + "').remove();"), mainFrame->GetURL(), 0);
#endif
		}
	}
}

void CGui::LoadFrame(std::string frame, std::string url, const bool remote)
{
	if (!DoesFrameExist(frame) && frame.compare("grandm.chat") != 0 && frame.compare("grandm.loading") != 0 && frame.compare("grandm.debug") != 0)
		CreateFrame(frame);

	CGui::LoadURL(frame, url, remote, true);
}

void CGui::LoadURL(std::string frame, std::string url, const bool remote, const bool resource)
{
	LOG_DEBUG("[CGui::LoadURL] frame->%s , url->%s", frame.c_str(), url.c_str());

#ifndef _CEFDISABLED
	//CefRefPtr<CefFrame> f = CefRenderer::getBrowser()->GetFrame(CefString(frame.c_str()));
	CefRefPtr<CefFrame> main = CefMainContext::getBrowser()->GetMainFrame();

	if (main != NULL && main->IsValid())
	{
		std::wstring filePath;

		if(url.compare("about:blank") != 0 && !remote)
		{
			wchar_t  buffer[MAX_PATH];
			GetModuleFileName(GetModuleHandle(L"Client.dll"), buffer, sizeof(buffer));

			filePath = buffer;
			filePath.erase(filePath.find(L"Client.dll"), std::wstring(L"Client.dll").size());

			if (!resource)
				filePath = CString::utf8ToUtf16(url);
			else
				filePath += L"/resources/" + CString::utf8ToUtf16(url);

			std::replace(filePath.begin(), filePath.end(), '\\', '/');
		}
		else
		{
			filePath = CString::utf8ToUtf16(url);
		}

		std::string js =
			//"$('#"
			"$(\"[name='" 
			+ frame +
			"']\").attr('src',\""
			+ CString::utf16ToUtf8(filePath) +
			"\");";

		LOG_DEBUG("[CGui::LoadURL] js->%s", js.c_str());

		main->ExecuteJavaScript(CefString(js), main->GetURL(), 0);
		//f->LoadURL(CefString(filePath.c_str()));
	}
	else
		LOG_WARNING("[CGui] %s is not a valid frame.", frame.c_str());
#endif
}

Vector2 CGui::GetResolution() 
{ 
	return resolution; 
}

void CGui::SetResolution(Vector2 res) 
{ 
	resolution = res;
}

void CGui::SetResolution(const int x, const int y)
{ 
	resolution.x = x; resolution.y = y;
}

void CGui::Debug::UpdateDebug()
{
	CVector3 pos = CLocalPlayer::GetPosition();
	CVector3 rot = CLocalPlayer::GetRotation();
	CVector3 velo = CLocalPlayer::GetVelocity();

	std::string debugClient = "$('#grandm-debug-position').html('Position - [ X : " + std::to_string(pos.x) + " ][ Y : " + std::to_string(pos.y) + " ][ Z : " + std::to_string(pos.z) + " ]'); ";
	debugClient += "$('#grandm-debug-velocity').html('Velocity - [ X : " + std::to_string(velo.x) + " ][ Y : " + std::to_string(velo.y) + " ][ Z : " + std::to_string(velo.z) + " ]'); ";
	debugClient += "$('#grandm-debug-rotation').html('Rotation - [ X : " + std::to_string(rot.x) + " ][ Y : " + std::to_string(rot.y) + " ][ Z : " + std::to_string(rot.z) + " ]'); ";
	debugClient += "$('#grandm-debug-speedheading').html('Speed - [ X " + std::to_string(CLocalPlayer::GetForwardSpeed()) + " ] | Heading - [ " + std::to_string((float)rot.z) + " ]'); ";
	debugClient += "$('#grandm-debug-streamer').html('Streamer - [ Player : " + std::to_string(CStreamer::GetPlayerCount()) + "/" + std::to_string(g_Players.size()) +
		" ][ Vehicles : " + std::to_string(CStreamer::GetVehicleCount()) + "/" + std::to_string(g_Vehicles.size()) +
		" ][ Objects : " + std::to_string(CStreamer::GetObjectCount()) + "/" + std::to_string(g_Objects.size()) + 
		" ][ NPCs : " + std::to_string(CStreamer::GetNpcCount()) + "/" + std::to_string(g_Npcs.size()) +
		" ][ Pickups : " + std::to_string(CStreamer::GetPickupCount()) + "/" + std::to_string(g_Pickups.size()) +
		" ]'); ";

	std::string debugFPS = "$('#grandm-debug-fpsping').html('" + std::to_string(fps) + " TPS (" +  + ") | " + std::to_string(ping) + "ms'); ";

	std::string debugNetwork = "$('#grandm-debug-network').html('In - " + std::to_string(inbound) + "Kbs | Out - " + std::to_string(outbound) + "Kbs | Loss - " + std::to_string(loss) + " '); ";

	CefRefPtr<CefFrame> cefFrame = CefMainContext::getBrowser()->GetFrame(CefString("grandm.debug"));

	cefFrame->ExecuteJavaScript(CefString(debugClient.c_str()), cefFrame->GetURL(), 0);
	cefFrame->ExecuteJavaScript(CefString(debugFPS.c_str()), cefFrame->GetURL(), 0);
	cefFrame->ExecuteJavaScript(CefString(debugNetwork.c_str()), cefFrame->GetURL(), 0);
}