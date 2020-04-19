#include <vector>

// Vendor.RakNet
#include <RakPeerInterface.h>
#include <BitStream.h>

// Vendor.CEF
#include <include/cef_app.h>
#include <include/cef_client.h>
#include <include/cef_render_handler.h>
#include <include/cef_sandbox_win.h>

// Hook
#include "hook/enums.h"
#include "hook/types.h"
#include "hook/natives.h"

// Discord
#include "discord_rpc.h"

#include "Functions.h"
#include "World.h"
#include "Scripts.h"
#include "Doors.h"
#include "keyboard.h"
#include "CGui.h"
#include "CPlayer.h"
#include "CVehicle.h"
#include "CObject.h"
#include "CPickup.h"
#include "CNPC.h"
#include "CCheckpoint.h"
#include "CLocalPlayer.h"
#include "CRPCManager.h"
#include "CNetworkManager.h"
#include "CDiscord.h"

#include "CefMainContext.h"
#include "CStreamer.h"
#include "CWorld.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

// Self
#include "CCore.h"

std::vector<CPlayer>				g_Players;
std::vector<CVehicle>				g_Vehicles;
std::vector<CObject>				g_Objects;
std::vector<CNPC>					g_Npcs;
std::vector<CCheckpoint>			g_Checkpoints;
std::vector<CBlip>					g_Blips;
std::vector<CPickup>				g_Pickups;

unsigned long CCore::LastCleanUp;
unsigned long CCore::LastUnlock;

bool CCore::Initialized = false;

static bool showSettings = false;
static int	 SettingsInst = -1;

bool CCore::Initialize()
{
	CGui::Init();

	//CAM::DESTROY_ALL_CAMS(false);

	//Loads multiplayer World
	DLC::_LOAD_MP_DLC_MAPS();
	GAMEPLAY::_USE_FREEMODE_MAP_BEHAVIOR(true);
	SCRIPT::SHUTDOWN_LOADING_SCREEN();

	CAM::DO_SCREEN_FADE_OUT(1);

	CLocalPlayer::Init();
	CNetworkManager::Initialize();

	//ENTITY::FREEZE_ENTITY_POSITION(CLocalPlayer::GetPed(), true);
	STREAMING::STOP_PLAYER_SWITCH();
	STREAMING::SHUTDOWN_CREATOR_BUDGET();
	BRAIN::CLEAR_PED_TASKS_IMMEDIATELY(CLocalPlayer::GetPed());

	TIME::PAUSE_CLOCK(true);
	GAMEPLAY::CLEAR_OVERRIDE_WEATHER();

	WATER::_SET_WAVES_INTENSITY(0.0f);

	PLAYER::SET_PLAYER_INVINCIBLE(PLAYER::PLAYER_ID(), false);
	PLAYER::SET_WANTED_LEVEL_MULTIPLIER(0.0f);

	PLAYER::SET_PLAYER_HEALTH_RECHARGE_MULTIPLIER(CLocalPlayer::GetPlayer(), 0.0f);
	PLAYER::SET_POLICE_IGNORE_PLAYER(CLocalPlayer::GetPlayer(), true);
	PLAYER::SET_AUTO_GIVE_PARACHUTE_WHEN_ENTER_PLANE(CLocalPlayer::GetPlayer(), false);
	PLAYER::_0xD2B315B6689D537D(CLocalPlayer::GetPlayer(), false);

	AUDIO::SET_AUDIO_FLAG("LoadMPData", true);
	AUDIO::SET_AUDIO_FLAG("DisableBarks", true);
	AUDIO::SET_AUDIO_FLAG("ActivateSwitchWheelAudio", false);
	AUDIO::SET_AUDIO_FLAG("PoliceScannerDisabled", true);
	AUDIO::SET_AUDIO_FLAG("DisableFlightMusic", true);
	AUDIO::SET_AUDIO_FLAG("OnlyAllowScriptTriggerPoliceScanner", true);
	AUDIO::SET_AUDIO_FLAG("WantedMusicDisabled", true);
	AUDIO::SET_AUDIO_FLAG("AllowPlayerAIOnMission", false);
	AUDIO::SET_AUDIO_FLAG("AllowPoliceScannerWhenPlayerHasNoControl", false);
	AUDIO::_FORCE_AMBIENT_SIREN(false);
	AUDIO::STOP_ALL_ALARMS(true);
	AUDIO::_DISABLE_POLICE_REPORTS();

	GTAV::World::RevealFullMap();

	//GTAV::World::LiftPlayerWorldLimits();
	PLAYER::_EXPAND_WORLD_LIMITS(FLT_MAX, FLT_MAX, FLT_MAX);

	// Disables dispatch services
	for (unsigned int s = 1; s < 16; s++)
	{
		GAMEPLAY::ENABLE_DISPATCH_SERVICE(s, false);
	}

	//Disable Hospitals
	for (unsigned int i = 0; i <= 5; i++)
	{
		GAMEPLAY::DISABLE_HOSPITAL_RESTART(i, true);
	}

	//Disable Stunt Jumps
	for (unsigned int i = 0; i < 50; i++)
	{
		GAMEPLAY::DISABLE_STUNT_JUMP_SET(i);
		GAMEPLAY::DELETE_STUNT_JUMP(i);
	}

	// Call OnLoad
	if (!CNetworkManager::Start())
	{
		LOG_ERROR("[CCore] NetworkManager could not be started.");
		return false;
	}

	CRPCManager::RegisterRPCMessages();

	Scripts::StopAll();
	CleanUp();

	Initialized = true;

	//GAMEPLAY::CLEAR_BIT();
	int test = (int)*getGlobalPtr(25834 + 23 + 11);
	LOG_DEBUG("[CCore] >> %i.", test);

	test = (int)*getGlobalPtr(106070 + 24951 + 3);
	LOG_DEBUG("[CCore] >> %i.", test);

	LOG_DEBUG("[CCore] Initialized.");

	return true;
}

void CCore::OnGameTick()
{
	// Discord
	Discord_RunCallbacks();
	CDiscord::updateDiscordPresence();

	if (timeGetTime() >= LastCleanUp + 500)
	{
		Scripts::Pulse();	// Script Clean Up
		CleanUp();			// World Clean Up
		PreventCheat();
	}

	if (timeGetTime() >= LastUnlock + 2000)
	{
		Doors::Pulse();		// Unlocks doors
		LastUnlock = timeGetTime();
	}

	CleanUpTick();

	/*
	<Item key="PAUSE_MENU_PAGES_MAP">
	<RequiredMovies>
	<movie>PAUSE_MENU_SHARED_COMPONENTS_03</movie>
	</RequiredMovies>
	</Item>

	MENU_UNIQUE_ID_HEADER_SAVE_GAME

	<MenuScreen>MENU_UNIQUE_ID_MAP</MenuScreen>
	<cGfxFilename>PAUSE_MENU_PAGES_MAP</cGfxFilename>
	<runtime>
	<type>MAP</type>
	<params>
	<data key="path">PauseMenu_Map</data>
	<data key="sp_path">PauseMenu_Map</data>
	<data key="continual">TRUE</data>
	</params>
	</runtime>


	<!--  ACTUAL SCREEN ID: 7 -->
	<Item>
	<MenuScreen>MENU_UNIQUE_ID_MAP</MenuScreen>
	<cGfxFilename>PAUSE_MENU_PAGES_MAP</cGfxFilename>
	<runtime>
	<type>MAP</type>
	<params>
	<data key="path">PauseMenu_Map</data>
	<data key="sp_path">PauseMenu_Map</data>
	<data key="continual">TRUE</data>
	</params>
	</runtime>

	<depth>1</depth>
	<ScrollBarFlags>Width_1 Offset_2Right Arrows_UpDown Align_Right</ScrollBarFlags>


	<ButtonList>
	<ButtonPrompts>

	<CMenuButton> <hButtonHash>IB_MS_WARP</hButtonHash>		<ButtonInput>INPUT_FRONTEND_ACCEPT</ButtonInput> <Contexts>*ALL*,MAP_CanWarp, *NONE*,CORONA_BIGMAP_CLOSE, CORONA_BIGMAP_CLOSE_KM</Contexts></CMenuButton>
	<CMenuButton> <hButtonHash>IB_WAYPOINT</hButtonHash>	<ButtonInput>INPUT_FRONTEND_ACCEPT</ButtonInput> <Contexts>*ALL*,MAP_Waypoint, *NONE*,CORONA_BIGMAP_CLOSE, CORONA_BIGMAP_CLOSE_KM, MAP_Destination</Contexts></CMenuButton>
	<CMenuButton> <hButtonHash>IB_SETDEST</hButtonHash>	<ButtonInput>INPUT_FRONTEND_ACCEPT</ButtonInput> <Contexts>*ALL*,MAP_Destination, *NONE*,CORONA_BIGMAP_CLOSE, CORONA_BIGMAP_CLOSE_KM</Contexts></CMenuButton>
	<CMenuButton> <hButtonHash>IB_ZOOM</hButtonHash>		<ButtonInputGroup>INPUTGROUP_FRONTEND_TRIGGERS</ButtonInputGroup> <Contexts>*ALL*,MAP_CanZoom</Contexts></CMenuButton>
	<CMenuButton> <hButtonHash>IB_POI</hButtonHash>		<ButtonInput>INPUT_FRONTEND_Y</ButtonInput> <Contexts>*ALL*,MAP_CanDropPOI, *NONE*,CORONA_BIGMAP_CLOSE, CORONA_BIGMAP_CLOSE_KM</Contexts></CMenuButton>
	<!--	<CMenuButton> <hButtonHash>IB_CRIMINAL</hButtonHash>	<ButtonInput>INPUT_FRONTEND_CANCEL</ButtonInput> <Contexts>*ALL*,MAP_Criminal</Contexts></CMenuButton> -->
	<CMenuButton> <hButtonHash>IB_INTEXT</hButtonHash>		<ButtonInput>INPUT_FRONTEND_SELECT</ButtonInput> <Contexts>*ALL*,MAP_Interior,*NONE*,MAP_Criminal, *NONE*,CORONA_BIGMAP_CLOSE, CORONA_BIGMAP_CLOSE_KM</Contexts></CMenuButton>
	<CMenuButton> <hButtonHash>IB_GOLF</hButtonHash>		<ButtonInput>INPUT_FRONTEND_SELECT</ButtonInput> <Contexts>*ALL*,MAP_Golf,*NONE*,MAP_Interior,MAP_Criminal, *NONE*,CORONA_BIGMAP_CLOSE, CORONA_BIGMAP_CLOSE_KM</Contexts></CMenuButton>

	<!--<CMenuButton> <hButtonHash>IB_NAVIGATE</hButtonHash>	<ButtonInputGroup>INPUTGROUP_FRONTEND_LSTICK_ALL</ButtonInputGroup></CMenuButton>-->
	<CMenuButton> <hButtonHash>IB_LEGEND</hButtonHash>		<ButtonInputGroup>INPUTGROUP_FRONTEND_DPAD_ALL</ButtonInputGroup> <Contexts>*NONE*,CORONA_BIGMAP_CLOSE, CORONA_BIGMAP_CLOSE_KM</Contexts> </CMenuButton>
	<CMenuButton> <hButtonHash>IB_BLIPVIS1</hButtonHash>		<ButtonInput>INPUT_FRONTEND_RS</ButtonInput> <Contexts>*ALL*,MAP_VisHigh, *NONE*,CORONA_BIGMAP_CLOSE, CORONA_BIGMAP_CLOSE_KM</Contexts></CMenuButton>
	<CMenuButton> <hButtonHash>IB_BLIPVIS0</hButtonHash>		<ButtonInput>INPUT_FRONTEND_RS</ButtonInput> <Contexts>*ALL*,MAP_VisLow, *NONE*,CORONA_BIGMAP_CLOSE, CORONA_BIGMAP_CLOSE_KM</Contexts></CMenuButton>
	<CMenuButton> <hButtonHash>IB_SRTMISS</hButtonHash>		<ButtonInput>INPUT_FRONTEND_X</ButtonInput> <Contexts>*ALL*,MAP_CanStartMiss, *NONE*,CORONA_BIGMAP_CLOSE, CORONA_BIGMAP_CLOSE_KM</Contexts></CMenuButton>
	<CMenuButton> <hButtonHash>IB_BACK</hButtonHash> 		<ButtonInput>INPUT_FRONTEND_CANCEL</ButtonInput> <Contexts>*NONE*,CORONA_BIGMAP_CLOSE, CORONA_BIGMAP_CLOSE_KM</Contexts> </CMenuButton>

	<CMenuButton> <hButtonHash>FM_COR_HIDE_BIGMAP</hButtonHash>		<ButtonInput>INPUT_FRONTEND_Y</ButtonInput>			<Contexts>*ALL*, CORONA_BIGMAP_CLOSE, *NONE*</Contexts> </CMenuButton>
	<CMenuButton> <hButtonHash>FM_COR_HIDE_BIGMAP</hButtonHash>		<ButtonInput>INPUT_FRONTEND_LS</ButtonInput>		<Contexts>*ALL*, CORONA_BIGMAP_CLOSE_KM, *NONE*</Contexts> </CMenuButton>
	</ButtonPrompts>
	</ButtonList>
	<MenuItems>
	<Item> <MenuAction>MENU_OPTION_ACTION_FILL_CONTENT</MenuAction> </Item>
	</MenuItems>
	</Item>

	<!--  ACTUAL SCREEN ID: 0 -->
	<Item>
	<MenuScreen>MENU_UNIQUE_ID_HEADER</MenuScreen>
	<depth>0</depth>
	<MenuItems>
	<Item> <cTextId>PM_SCR_MAP</cTextId> <MenuAction>MENU_OPTION_ACTION_LINK</MenuAction> <MenuUniqueId>MENU_UNIQUE_ID_MAP</MenuUniqueId> </Item>
	<Item> <cTextId>PM_SCR_BRF</cTextId> <MenuAction>MENU_OPTION_ACTION_LINK</MenuAction> <MenuUniqueId>MENU_UNIQUE_ID_INFO</MenuUniqueId> </Item>
	<Item> <cTextId>PM_SCR_STA</cTextId> <MenuAction>MENU_OPTION_ACTION_LINK</MenuAction> <MenuUniqueId>MENU_UNIQUE_ID_STATS</MenuUniqueId> </Item>
	<Item> <cTextId>PM_SCR_SET</cTextId> <MenuAction>MENU_OPTION_ACTION_LINK</MenuAction> <MenuUniqueId>MENU_UNIQUE_ID_SETTINGS</MenuUniqueId> </Item>
	<Item> <cTextId>PM_SCR_GAM</cTextId> <MenuAction>MENU_OPTION_ACTION_LINK</MenuAction> <MenuUniqueId>MENU_UNIQUE_ID_GAME</MenuUniqueId> </Item>
	<Item> <cTextId>PM_SCR_MIS</cTextId> <MenuAction>MENU_OPTION_ACTION_LINK</MenuAction> <MenuUniqueId>MENU_UNIQUE_ID_MISSION_CREATOR</MenuUniqueId> </Item>
	<Item> <cTextId>PM_SCR_FRI</cTextId> <MenuAction>MENU_OPTION_ACTION_LINK</MenuAction> <MenuUniqueId>MENU_UNIQUE_ID_FRIENDS</MenuUniqueId> </Item>
	<!--<Item> <cTextId>PM_SCR_SOC</cTextId> <MenuAction>MENU_OPTION_ACTION_LINK</MenuAction> <MenuUniqueId>MENU_UNIQUE_ID_SOCIALCLUB</MenuUniqueId> </Item>-->
	<Item> <cTextId>PM_SCR_GAL</cTextId> <MenuAction>MENU_OPTION_ACTION_LINK</MenuAction> <MenuUniqueId>MENU_UNIQUE_ID_GALLERY</MenuUniqueId> </Item>
	<Item> <cTextId>CMRC_GOTOSTORE</cTextId> <MenuAction>MENU_OPTION_ACTION_LINK</MenuAction> <MenuUniqueId>MENU_UNIQUE_ID_STORE</MenuUniqueId> </Item>
	<Item> <cTextId>PM_SCR_RPL</cTextId> <MenuAction>MENU_OPTION_ACTION_LINK</MenuAction> <MenuUniqueId>MENU_UNIQUE_ID_REPLAY_EDITOR</MenuUniqueId> <Contexts>*ALL*,ReplayEditor,*NONE*,Creator</Contexts> </Item>
	</MenuItems>
	</Item>



	PAUSE_MENU_PAGES_KEYMAP

	FE_MENU_VERSION_SP_PAUSE

	FE_MENU_VERSION_MP_PAUSE

	FE_MENU_VERSION_CREATOR_PAUSE

	FE_MENU_VERSION_CUTSCENE_PAUSE

	FE_MENU_VERSION_LANDING_MENU
	*/

	/* 
	Notes:
		Need chat open check.
		Idealy we want just to open the map and open game settings via the scaleform for it
	*/
	if(CONTROLS::IS_DISABLED_CONTROL_JUST_PRESSED(2, ControlFrontendPause) && !CGui::IsCursorVisible())
	{
		UI::SET_FRONTEND_ACTIVE(false);
		UI::ACTIVATE_FRONTEND_MENU($("FE_MENU_VERSION_SP_PAUSE"), false, -1);
	}

	// Show game UI
#ifdef _DEV
	if (KeyJustUp(VK_F9))
	{
		CNetworkManager::Disconnect();
	}
#endif
#ifndef _CEFDISABLED
	if (KeyJustUp(0x54))
	{
		CefRefPtr<CefFrame> frame = CefMainContext::getBrowser()->GetFrame(CefString("grandm.chat"));

		if (frame->IsValid())
			frame->ExecuteJavaScript(CefString("ShowInput(true)"), frame->GetURL(), 0);

		CGui::GiveFocus(true);
	}
#endif

	if (KeyJustUp(VK_ESCAPE) && CNetworkManager::g_ConnectionState == CNetworkManager::ConenctionState::Connected)
	{
		if (!CGui::IsMainMenu())
			CGui::ShowMainMenu(true, true);
		else
		{
			CGui::ShowChat(true, true);
			CGui::GiveFocus(false);
		}
	}

	KeyCheck();

	CLocalPlayer::Pulse();

	CNetworkManager::Pulse();

	CStreamer::Pulse();

	PulseEntities();

	if (!CLocalPlayer::IsControllable() || CGui::IsMainMenu() || CGui::IsLoadingScreen())
	{
		//if (CNetworkManager::g_ConnectionState == CNetworkManager::ConenctionState::Connected)
			//LOG_DEBUG("%i, %i, %i,", CLocalPlayer::IsControllable(), CGui::IsMainMenu(), CGui::IsLoadingScreen());

		for (unsigned int i = 0; i < 33; i++)
		{
			CONTROLS::DISABLE_ALL_CONTROL_ACTIONS(i);
		}
	}

	//////
	WAIT(0);
	//////
}

void CCore::PulseEntities() 
{
	if (!g_Players.empty())
	{
		for (unsigned int i = 0; i < g_Players.size(); i++)
		{
			g_Players[i].Pulse();
		}
	}

	if (!g_Vehicles.empty())
	{
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			g_Vehicles[i].PulseMovement();
			g_Vehicles[i].PulseVehicleData();
		}
	}

	if (!g_Objects.empty())
	{
		for (unsigned int i = 0; i < g_Objects.size(); i++)
		{
			g_Objects[i].Pulse();
		}
	}

	if (!g_Checkpoints.empty())
	{
		for (unsigned int i = 0; i < g_Checkpoints.size(); i++)
		{
			g_Checkpoints[i].Pulse();
		}
	}

	if (!g_Npcs.empty())
	{
		for (unsigned int i = 0; i < g_Npcs.size(); i++)
		{
			g_Npcs[i].Pulse();
		}
	}

	if (!g_Pickups.empty())
	{
		for (unsigned int i = 0; i < g_Pickups.size(); i++)
		{
			g_Pickups[i].Pulse();
		}
	}
}

void CCore::CleanUp()
{
	Vector3 Position = CLocalPlayer::GetPositionEx();

	PLAYER::DISABLE_PLAYER_VEHICLE_REWARDS(PLAYER::PLAYER_ID());
	PLAYER::SET_MAX_WANTED_LEVEL(0);

	VEHICLE::SET_GARBAGE_TRUCKS(false);
	VEHICLE::SET_RANDOM_BOATS(false);
	VEHICLE::SET_RANDOM_TRAINS(false);
	VEHICLE::SET_FAR_DRAW_VEHICLES(false);
	VEHICLE::_DISPLAY_DISTANT_VEHICLES(false);
	VEHICLE::DELETE_ALL_TRAINS();
	VEHICLE::SET_NUMBER_OF_PARKED_VEHICLES(-1);
	VEHICLE::SET_ALL_LOW_PRIORITY_VEHICLE_GENERATORS_ACTIVE(false);
	VEHICLE::REMOVE_VEHICLES_FROM_GENERATORS_IN_AREA(-9999.0f, -9999.0f, -9999.0f, 9999.0f, 9999.0f, 9999.0f, false);

	PED::SET_CREATE_RANDOM_COPS_NOT_ON_SCENARIOS(false);
	PED::SET_CREATE_RANDOM_COPS(false);
	PED::SET_CREATE_RANDOM_COPS_ON_SCENARIOS(false);

	GAMEPLAY::SET_TIME_SCALE(1.0f);
	GAMEPLAY::CLEAR_AREA_OF_PEDS(Position.x, Position.y, Position.z, 2000.0f, 1);
	GAMEPLAY::CLEAR_AREA_OF_VEHICLES(Position.x, Position.y, Position.z, 2000.0f, false, false, false, false, false, 0);
	GAMEPLAY::CLEAR_AREA_OF_COPS(Position.x, Position.y, Position.z, 2000.0f, 0);
	GAMEPLAY::_DISABLE_AUTOMATIC_RESPAWN(true);
	GAMEPLAY::IGNORE_NEXT_RESTART(true);
	GAMEPLAY::SET_MISSION_FLAG(true);
	GAMEPLAY::SET_FADE_OUT_AFTER_DEATH(false);
	GAMEPLAY::USING_MISSION_CREATOR(true);

	LastCleanUp = timeGetTime();
}

void CCore::CleanUpTick()
{
	PED::SET_PED_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
	PED::SET_SCENARIO_PED_DENSITY_MULTIPLIER_THIS_FRAME(0.0f, 0.0f);

	VEHICLE::SET_RANDOM_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
	VEHICLE::SET_PARKED_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
	VEHICLE::SET_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);
	VEHICLE::_SET_SOME_VEHICLE_DENSITY_MULTIPLIER_THIS_FRAME(0.0f);

	EVENT::SUPPRESS_SHOCKING_EVENTS_NEXT_FRAME();
	EVENT::SUPPRESS_AGITATION_EVENTS_NEXT_FRAME();
}

void CCore::PreventCheat()
{
	Ped ped = CLocalPlayer::GetPed();

	// Stops the client changing their max health
	if (PED::GET_PED_MAX_HEALTH(ped) != CLocalPlayer::GetMaxHealth())
		PED::SET_PED_MAX_HEALTH(ped, CLocalPlayer::GetMaxHealth());

	/* ! This may work incorrrectly with health but we will see ! */
	// Stops the client changing their current health
	if (ENTITY::GET_ENTITY_HEALTH(ped) != CLocalPlayer::GetHealth())
		ENTITY::SET_ENTITY_HEALTH(ped, CLocalPlayer::GetHealth());

	if (!PED::CAN_KNOCK_PED_OFF_VEHICLE(ped))
		PED::SET_PED_CAN_BE_KNOCKED_OFF_VEHICLE(ped, true);

	if (PLAYER::GET_PLAYER_WANTED_LEVEL(CLocalPlayer::GetPlayer()) > 0)
		PLAYER::CLEAR_PLAYER_WANTED_LEVEL(CLocalPlayer::GetPlayer());

	if (PLAYER::GET_PLAYER_INVINCIBLE(CLocalPlayer::GetPlayer()))
		PLAYER::SET_PLAYER_INVINCIBLE(CLocalPlayer::GetPlayer(), false);


	if (TIME::GET_CLOCK_HOURS() != CWorld::GetHour() || TIME::GET_CLOCK_MINUTES() != CWorld::GetMinute() || TIME::GET_CLOCK_SECONDS() != CWorld::GetSecond())
		TIME::SET_CLOCK_TIME(CWorld::GetHour(), CWorld::GetMinute(), CWorld::GetSecond());
}

void CCore::KeyCheck()
{
	CONTROLS::DISABLE_CONTROL_ACTION(2, ControlFrontendPause, 1);
	CONTROLS::DISABLE_CONTROL_ACTION(2, ControlFrontendPauseAlternate, 1);
	CONTROLS::DISABLE_CONTROL_ACTION(2, ControlEnterCheatCode, 1);
	CONTROLS::DISABLE_CONTROL_ACTION(2, ControlEnter, 1);
	CONTROLS::DISABLE_CONTROL_ACTION(2, ControlCharacterWheel, 1);
	CONTROLS::DISABLE_CONTROL_ACTION(2, ControlFrontendSocialClub, 1);
	CONTROLS::DISABLE_CONTROL_ACTION(2, ControlFrontendSocialClubSecondary, 1);
	CONTROLS::DISABLE_CONTROL_ACTION(2, ControlDropWeapon, 1);
	CONTROLS::DISABLE_CONTROL_ACTION(2, ControlDropAmmo, 1);
	CONTROLS::DISABLE_CONTROL_ACTION(2, ControlSelectCharacterMichael, 1);
	CONTROLS::DISABLE_CONTROL_ACTION(2, ControlSelectCharacterFranklin, 1);
	CONTROLS::DISABLE_CONTROL_ACTION(2, ControlSelectCharacterTrevor, 1);
	CONTROLS::DISABLE_CONTROL_ACTION(2, ControlSelectCharacterMultiplayer, 1);


	if (CLocalPlayer::IsControllable())
	{
		if (KeyJustUp(0x46) && !PED::IS_PED_IN_ANY_VEHICLE(CLocalPlayer::GetPed(), true))
		{
			Vehicle vehicle = CVehicle::getClosestVehicleFromPedPos(CLocalPlayer::GetPed(), 10.0f);
			if (vehicle)
			{
				for (unsigned int v = 0; v < g_Vehicles.size(); v++)
				{
					if (g_Vehicles[v].GetEntity() == vehicle)
					{
						if (g_Vehicles[v].GetOccupant(0) == -1)
						{
							BRAIN::TASK_ENTER_VEHICLE(CLocalPlayer::GetPed(), vehicle, 5000, -1, 2.0, 1, 0);

							g_Vehicles[v].SetAssignee(CNetworkManager::GetInterface()->GetMyGUID());

							RakNet::BitStream bitstream;
							bitstream.Write(g_Vehicles[v].GetID());
							bitstream.Write(g_Vehicles[v].GetType());
							bitstream.Write(CNetworkManager::GetInterface()->GetMyGUID());
							CNetworkManager::GetRPC().Signal("SetPhysicsAssignment", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
						}
						break;
					}
				}
			}
			else
			{
				BRAIN::CLEAR_PED_TASKS(CLocalPlayer::GetPed());
			}

			ResetKeyState(0x46);
		}

		if (KeyJustUp(0x47) && !PED::IS_PED_IN_ANY_VEHICLE(CLocalPlayer::GetPed(), true))
		{
			Vehicle vehicle = CVehicle::getClosestVehicleFromPedPos(CLocalPlayer::GetPed(), 10.0f);
			if (vehicle)
			{
				if (!VEHICLE::ARE_ANY_VEHICLE_SEATS_FREE(vehicle))
					return;

				int vehicleIndex = -1;

				for (unsigned int v = 0; v < g_Vehicles.size(); v++)
				{
					if (g_Vehicles[v].GetEntity() == vehicle)
					{
						vehicleIndex = v;
						break;
					}
				}

				if (vehicleIndex != -1)
				{
					int seat = 0;
					int multiplyer = 0;

					if (ENTITY::GET_ENTITY_BONE_INDEX_BY_NAME(vehicle, "seat_pside_r") == -1)
					{
						multiplyer = -2;
					}

					float distance = Vehicles::GetSeatDistance(vehicle, CLocalPlayer::GetPosition(), "seat_pside_r");
					float BoneDistance = 10000;

					if (g_Vehicles[vehicleIndex].GetOccupant(3) == -1)
						seat = 2;

					BoneDistance = Vehicles::GetSeatDistance(vehicle, CLocalPlayer::GetPosition(), "seat_pside_f");
					if (BoneDistance < distance)
					{
						if (/*g_Vehicles[vehicleIndex].GetOccupant(1) == -1*/VEHICLE::IS_VEHICLE_SEAT_FREE(vehicle, 0, 0))
						{
							distance = BoneDistance;
							seat = 0;
						}
					}

					BoneDistance = Vehicles::GetSeatDistance(vehicle, CLocalPlayer::GetPosition(), "seat_dside_r");
					if (BoneDistance < distance)
					{
						if (/*g_Vehicles[vehicleIndex].GetOccupant(2) == -1*/VEHICLE::IS_VEHICLE_SEAT_FREE(vehicle, 1, 0))
						{
							distance = BoneDistance;
							seat = 1;
						}
					}

					/*if (Vehicles::GetSeatDistance(vehicle, CLocalPlayer::GetPosition(), "seat_r") < distance)
					{
						if (VEHICLE::IS_VEHICLE_SEAT_FREE(vehicle, -2))
						{
							distance = Vehicles::GetSeatDistance(vehicle, CLocalPlayer::GetPosition(), "seat_r");
							seat = -2;
						}
					}*/
					BoneDistance = Vehicles::GetSeatDistance(vehicle, CLocalPlayer::GetPosition(), "seat_dside_r1");
					if (BoneDistance < distance)
					{
						if (VEHICLE::IS_VEHICLE_SEAT_FREE(vehicle, 3, 0))
						{
							distance = BoneDistance;
							seat = 3;
						}
					}

					BoneDistance = Vehicles::GetSeatDistance(vehicle, CLocalPlayer::GetPosition(), "seat_dside_r2");
					if (BoneDistance < distance)
					{
						if (VEHICLE::IS_VEHICLE_SEAT_FREE(vehicle, 5, 0))
						{
							distance = BoneDistance;
							seat = 5;
						}
					}

					BoneDistance = Vehicles::GetSeatDistance(vehicle, CLocalPlayer::GetPosition(), "seat_dside_r3");
					if (BoneDistance < distance)
					{
						if (VEHICLE::IS_VEHICLE_SEAT_FREE(vehicle, 3, 0))
						{
							distance = BoneDistance;
							seat = 3;
						}
					}

					BoneDistance = Vehicles::GetSeatDistance(vehicle, CLocalPlayer::GetPosition(), "seat_dside_r4");
					if (BoneDistance < distance)
					{
						if (VEHICLE::IS_VEHICLE_SEAT_FREE(vehicle, 5, 0))
						{
							distance = BoneDistance;
							seat = 5;
						}
					}

					BoneDistance = Vehicles::GetSeatDistance(vehicle, CLocalPlayer::GetPosition(), "seat_dside_r5");
					if (BoneDistance < distance)
					{
						if (VEHICLE::IS_VEHICLE_SEAT_FREE(vehicle, 7, 0))
						{
							distance = BoneDistance;
							seat = 7;
						}
					}


					BoneDistance = Vehicles::GetSeatDistance(vehicle, CLocalPlayer::GetPosition(), "seat_dside_r6");
					if (BoneDistance < distance)
					{
						if (VEHICLE::IS_VEHICLE_SEAT_FREE(vehicle, 9, 0))
						{
							distance = BoneDistance;
							seat = 9;
						}
					}

					BoneDistance = Vehicles::GetSeatDistance(vehicle, CLocalPlayer::GetPosition(), "seat_dside_r7");
					if (BoneDistance < distance)
					{
						if (VEHICLE::IS_VEHICLE_SEAT_FREE(vehicle, 11, 0))
						{
							distance = BoneDistance;
							seat = 11;
						}
					}

					BoneDistance = Vehicles::GetSeatDistance(vehicle, CLocalPlayer::GetPosition(), "seat_pside_r1");
					if (BoneDistance < distance)
					{
						if (VEHICLE::IS_VEHICLE_SEAT_FREE(vehicle, 4, 0))
						{
							distance = BoneDistance;
							seat = 4;
						}
					}

					BoneDistance = Vehicles::GetSeatDistance(vehicle, CLocalPlayer::GetPosition(), "seat_pside_r2");
					if (BoneDistance < distance)
					{
						if (VEHICLE::IS_VEHICLE_SEAT_FREE(vehicle, 6, 0))
						{
							distance = BoneDistance;
							seat = 6;
						}
					}

					BoneDistance = Vehicles::GetSeatDistance(vehicle, CLocalPlayer::GetPosition(), "seat_pside_r3");
					if (BoneDistance < distance)
					{
						if (VEHICLE::IS_VEHICLE_SEAT_FREE(vehicle, 4, 0))
						{
							distance = BoneDistance;
							seat = 4;
						}
					}

					BoneDistance = Vehicles::GetSeatDistance(vehicle, CLocalPlayer::GetPosition(), "seat_pside_r4");
					if (BoneDistance < distance)
					{
						if (VEHICLE::IS_VEHICLE_SEAT_FREE(vehicle, 6, 0))
						{
							distance = BoneDistance;
							seat = 6;
						}
					}

					BoneDistance = Vehicles::GetSeatDistance(vehicle, CLocalPlayer::GetPosition(), "seat_pside_r5");
					if (BoneDistance < distance)
					{
						if (VEHICLE::IS_VEHICLE_SEAT_FREE(vehicle, 8, 0))
						{
							distance = BoneDistance;
							seat = 8;
						}
					}

					BoneDistance = Vehicles::GetSeatDistance(vehicle, CLocalPlayer::GetPosition(), "seat_pside_r6");
					if (BoneDistance < distance)
					{
						if (VEHICLE::IS_VEHICLE_SEAT_FREE(vehicle, 10, 0))
						{
							distance = BoneDistance;
							seat = 10;
						}
					}

					BoneDistance = Vehicles::GetSeatDistance(vehicle, CLocalPlayer::GetPosition(), "seat_pside_r7");
					if (BoneDistance < distance)
					{
						if (VEHICLE::IS_VEHICLE_SEAT_FREE(vehicle, 12, 0))
						{
							distance = BoneDistance;
							seat = 12;
						}
					}

					BoneDistance = Vehicles::GetSeatDistance(vehicle, CLocalPlayer::GetPosition(), "seat_dside_f");
					if (BoneDistance < distance)
					{
						if (/*g_Vehicles[vehicleIndex].GetOccupant(0) == -1*/VEHICLE::IS_VEHICLE_SEAT_FREE(vehicle, -1, 0))
						{
							distance = BoneDistance;
							seat = -1;

							g_Vehicles[vehicleIndex].SetAssignee(CNetworkManager::GetInterface()->GetMyGUID());

							RakNet::BitStream bitstream;
							bitstream.Write(g_Vehicles[vehicleIndex].GetID());
							bitstream.Write(g_Vehicles[vehicleIndex].GetType());
							bitstream.Write(CNetworkManager::GetInterface()->GetMyGUID());
							CNetworkManager::GetRPC().Signal("SetPhysicsAssignment", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
						}
					}

					Hash mHash = g_Vehicles[vehicleIndex].GetModel();
					if (mHash == 1356124575 || mHash == 1180875963 || mHash == -2096818938 || mHash == -32236122 && seat == 1)
					{
						BRAIN::TASK_ENTER_VEHICLE(CLocalPlayer::GetPed(), vehicle, 5000, seat, 2.0, 16, 0);
					}
					else if (mHash == -1860900134 || mHash == -1924433270 && seat == 7)
					{
						BRAIN::TASK_ENTER_VEHICLE(CLocalPlayer::GetPed(), vehicle, 5000, seat, 2.0, 16, 0);
					}
					else if (mHash == -562680400 && seat == 0)
					{
						BRAIN::TASK_ENTER_VEHICLE(CLocalPlayer::GetPed(), vehicle, 5000, seat, 2.0, 16, 0);
					}
					else
					{
						BRAIN::TASK_ENTER_VEHICLE(CLocalPlayer::GetPed(), vehicle, 5000, seat + multiplyer, 2.0, 1, 0);
					}
				}
				else
				{
					BRAIN::CLEAR_PED_TASKS(CLocalPlayer::GetPed());
				}
			}
			else
			{
				BRAIN::CLEAR_PED_TASKS(CLocalPlayer::GetPed());
			}

			ResetKeyState(0x47);
		}

		if (CONTROLS::IS_CONTROL_PRESSED(0, ControlMoveUp) || CONTROLS::IS_CONTROL_PRESSED(0, ControlMoveDown) || CONTROLS::IS_CONTROL_PRESSED(0, ControlMoveLeft) || CONTROLS::IS_CONTROL_PRESSED(0, ControlMoveRight))
		{
			if (BRAIN::GET_IS_TASK_ACTIVE(CLocalPlayer::GetPed(), 160))
			{
				BRAIN::CLEAR_PED_TASKS(CLocalPlayer::GetPed());
			}
		}
	}
}