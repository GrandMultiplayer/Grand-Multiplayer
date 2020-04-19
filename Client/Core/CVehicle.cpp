// Self
#include "CVehicle.h"

#include <typeinfo>

// RakNet
#include "BitStream.h"
#include "PacketPriority.h"
#include "RakNetTypes.h"
#include "MessageIdentifiers.h"

#include "hook/enums.h"
#include "hook/natives.h"

#include "Functions.h"
#include "APIStructs.h"
#include "SharedUtility.h"
#include "CVector3.h"
#include "CMaths.h"
#include "CNetworkManager.h"
#include "CLocalPlayer.h"
#include "CVisual.h"
#include "NetworkCallbacks.h"

// Logger
#include "Console.h"
#include "General.h"
#include "Log.h"

CVehicle::CVehicle(const int id)
{
	//CEntity
	game.entity = NULL;
	game.created = false;
	game.physicsAssignee = RakNet::UNASSIGNED_RAKNET_GUID;
	game.vehicle = -1;

	identity.entityId = id;
	identity.type = Type::eVehicle;

	world.streamDistance = 1000.0f;
	world.dim = 0;

	forwardSpeed = 0;

	health = SyncInt(100, "EntityHealth");
	dead = false;
	lastAttacker = -1;

	//CVehicle
	for (unsigned int i = 0; i < SizeOfArray(occupants); i++)
	{
		occupants[i] = -1;
	}
}

CVehicle::~CVehicle()
{
	identity.entityId = -1;
	identity.type = Type::eNill;
}

CVehicle::CVehicle(const CVehicle & e)
{
	//CEntity
	identity = e.identity;
	game = e.game;
	world = e.world;
	tag = e.tag;
	tag = e.tag;
	transform = e.transform;
	forwardSpeed = e.forwardSpeed;
	health = e.health;
	dead = e.dead;
	lastAttacker = e.lastAttacker;
	interpolation = e.interpolation;
	sync = e.sync;

	//CVehicle
	data = e.data;
	blip = e.blip;
	vehicleSync = e.vehicleSync;

	for (unsigned int i = 0; i < 12; i++)
	{
		occupants[i] = e.occupants[i];
	}
}

CVehicle & CVehicle::operator=(const CVehicle &e)
{
	//CEntity
	identity = e.identity;
	game = e.game;
	world = e.world;
	tag = e.tag;
	tag = e.tag;
	transform = e.transform;
	forwardSpeed = e.forwardSpeed;
	health = e.health;
	dead = e.dead;
	lastAttacker = e.lastAttacker;
	interpolation = e.interpolation;
	sync = e.sync;

	//CVehicle
	data = e.data;
	blip = e.blip;
	vehicleSync = e.vehicleSync;

	for (unsigned int i = 0; i < 12; i++)
	{
		occupants[i] = e.occupants[i];
	}

	return (*this);
}

// Move constructor
CVehicle::CVehicle(CVehicle&& copy)
	: CEntity(std::move(copy))
	, blip(std::move(copy.blip))
	, data(std::move(copy.data))
	, vehicleSync(std::move(copy.vehicleSync))
{
	for (unsigned int i = 0; i < 12; i++)
	{
		occupants[i] = std::move(copy.occupants[i]);
	}
}

// Move assignment
CVehicle& CVehicle::operator=(CVehicle&& copy)
{
	CEntity::operator=(std::move(copy));
	blip = std::move(copy.blip);
	data = std::move(copy.data);
	vehicleSync = std::move(copy.vehicleSync);
	
	for (unsigned int i = 0; i < 12; i++)
	{
		occupants[i] = std::move(copy.occupants[i]);
	}

	return *this;
}

bool CVehicle::CreateVehicle()
{
	if (!game.created)
	{
		if (!STREAMING::IS_MODEL_IN_CDIMAGE(data.model.hash) || !STREAMING::IS_MODEL_VALID(data.model.hash))
		{
			LOG_WARNING("[CVehicle] Invalid vehicle model '%i' for entity '%i' reverting to dilettante.", data.model.hash, identity.entityId);
			data.model.hash = 0xbc993509;
		}

		STREAMING::REQUEST_MODEL(data.model.hash);
		WAIT(100);

		int time = 0;
		while (!STREAMING::HAS_MODEL_LOADED(data.model.hash))
		{
			WAIT(100);

			if (time == 1000 && !STREAMING::HAS_MODEL_LOADED(data.model.hash))
				return false;
			else
				time += 100;
		}

		game.entity = VEHICLE::CREATE_VEHICLE(data.model.hash, transform.Position.x, transform.Position.y, transform.Position.z, 0, false, true);

		ENTITY::FREEZE_ENTITY_POSITION(game.entity, TRUE);

		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(data.model.hash);

		STREAMING::REQUEST_COLLISION_AT_COORD(transform.Position.x, transform.Position.y, transform.Position.z);

		ENTITY::SET_ENTITY_LOAD_COLLISION_FLAG(game.entity, TRUE, 0);
		ENTITY::SET_ENTITY_COLLISION(game.entity, TRUE, FALSE);

		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(game.entity, transform.Position.x, transform.Position.y, transform.Position.z, FALSE, FALSE, FALSE);
		ENTITY::SET_ENTITY_ROTATION(game.entity, transform.Rotation.x, transform.Rotation.y, transform.Rotation.z, 2, true);

		data.hasRocketboost = VEHICLE::_HAS_VEHICLE_ROCKET_BOOST(game.entity);
		data.Class = VEHICLE::GET_VEHICLE_CLASS(game.entity);

		for (unsigned int i = 1; i < SizeOfArray(data.model.Extras); i++)
		{
			if (data.model.Extras[i].edited)
				VEHICLE::SET_VEHICLE_EXTRA(game.entity, i, data.model.Extras[i].enabled);
		}

		if (data.needsInit)
		{
			RakNet::BitStream bitstream;

			for (unsigned int i = 1; i < SizeOfArray(data.model.Extras); i++)
			{
				if (!data.model.Extras[i].edited)
				{
					data.model.Extras[i].edited = true;
					data.model.Extras[i].enabled = VEHICLE::IS_VEHICLE_EXTRA_TURNED_ON(game.entity, i);

					bitstream.Reset();
					bitstream.Write(identity.entityId);
					bitstream.Write(i);
					bitstream.Write(data.model.Extras[i].enabled);
					CNetworkManager::GetRPC().Signal("SetExtra", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
				}
			}

			bitstream.Reset();
			bitstream.Write(identity.entityId);
			CNetworkManager::GetRPC().Signal("VehicleInited", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
		}

		// Vehicle Number Plate
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(game.entity, (char *)data.plate.text.c_str());
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(game.entity, data.plate.style);

		if (data.Class == 18 || data.Class == 17 || data.Class == 15 || data.Class == 16 || data.Class == 20 || data.Class == 14)
		{
			VEHICLE::SET_VEHICLE_MOD(game.entity, 48, 0, 0);
			VEHICLE::SET_VEHICLE_LIVERY(game.entity, 0);
		}

		// Vehicle Mods
		for (unsigned int i = 0; i < SizeOfArray(data.model.Mods); i++)
		{
			if (data.model.Mods[i].Edited)
			{
				VEHICLE::SET_VEHICLE_MOD(game.entity, i, data.model.Mods[i].index, false);
				if (i = 48)
					VEHICLE::SET_VEHICLE_LIVERY(game.entity, data.model.Mods[i].index);
			}
		}

		// Vehicle Colors
		if (!data.model.Colors[0].custom)
		{
			VEHICLE::SET_VEHICLE_MOD_COLOR_1(game.entity, data.model.Colors[0].type, data.model.Colors[0].color, 0);
		}
		else
		{
			VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(game.entity, data.model.Colors[0].customCol.Red, data.model.Colors[0].customCol.Green, data.model.Colors[0].customCol.Blue);
		}

		if (!data.model.Colors[1].custom)
		{
			VEHICLE::SET_VEHICLE_MOD_COLOR_2(game.entity, data.model.Colors[1].type, data.model.Colors[1].color);
		}
		else
		{
			VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(game.entity, data.model.Colors[1].customCol.Red, data.model.Colors[1].customCol.Green, data.model.Colors[1].customCol.Blue);
		}

		// Engine State (Forces Non-Vanilla engine state)
		if (data.engine.forcedEngineState > -1)
		{
			VEHICLE::SET_VEHICLE_ENGINE_ON(game.entity, data.engine.forcedEngineState, true, true);
			VEHICLE::SET_VEHICLE_UNDRIVEABLE(game.entity, !data.engine.forcedEngineState);
		}

		VEHICLE::SET_TAXI_LIGHTS(game.entity, TRUE);

		VEHICLE::SET_VEHICLE_DOORS_LOCKED(game.entity, data.doorsLockState);

		ENTITY::SET_ENTITY_HEALTH(game.entity, health.Get());

		// Engine & Fuel Health
		VEHICLE::SET_VEHICLE_ENGINE_HEALTH(game.entity, data.engine.health.Get());
		VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(game.entity, data.engine.fuelHealth.Get());

		bool tl;
		VEHICLE::SET_TAXI_LIGHTS(game.entity, tl);
		data.taxiLight.Set(tl, RakNet::UNASSIGNED_RAKNET_GUID, identity.entityId, identity.type);

		VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(game.entity, 1, data.leftIndicator.Get());
		VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(game.entity, 0, data.rightIndicator.Get());

		VEHICLE::SET_VEHICLE_INTERIORLIGHT(game.entity, data.interiorLight.Get());

		VEHICLE::START_VEHICLE_HORN(game.entity, 0, $("HELDDOWN"), data.horn.Get());

		VEHICLE::_SET_VEHICLE_SIREN_SOUND(game.entity, data.sirenSound.Get());

		ENTITY::FREEZE_ENTITY_POSITION(game.entity, FALSE);
		ENTITY::SET_ENTITY_DYNAMIC(game.entity, TRUE);

		if (!ENTITY::DOES_ENTITY_EXIST(game.entity))
			return false;

		blip.CreateBlip(game.entity);

		LOG_DEBUG("[CVehicle] Created Vehicle %i.", identity.entityId);
		game.created = true;

		return true;
	}

	LOG_DEBUG("[CVehicle] Vehicle %i already created.", identity.entityId);
	return false;
}

void CVehicle::Destroy()
{
	if (game.created)
	{
		LOG_DEBUG("Removing Vehicle %i", identity.entityId);

		blip.Remove();

		if (ENTITY::DOES_ENTITY_EXIST(game.entity))
			VEHICLE::DELETE_VEHICLE(&game.entity);

		game.created = false;
	}
}

void CVehicle::Delete()
{
	if (game.created)
	{
		blip.Remove();

		if (ENTITY::DOES_ENTITY_EXIST(game.entity))
			VEHICLE::DELETE_VEHICLE(&game.entity);

		game.created = false;
	}

	game.entity = NULL;
}

void CVehicle::PulseVehicleData()
{
	if (game.created && identity.entityId != -1 && Exists())
	{
#ifdef _VERBOSE_LOGING
		LOG_DEBUG("Pulsing Vehicle: %i", identity.entityId);
#endif
		Interpolate();

		const int t_CurrentVehicle = CLocalPlayer::GetVehicleID();

		Assignment();

		if (CNetworkManager::GetInterface()->GetMyGUID() == game.physicsAssignee //&&
			/*(t_CurrentVehicle == identity.entityId && CLocalPlayer::GetSeat() == 0)*/)
		{
			if (vehicleSync.lastSyncSent + (1000.0f / 10) <= getticktime())
			{
				const bool engineState = VEHICLE::GET_IS_VEHICLE_ENGINE_RUNNING(game.entity);
				if (data.engine.forcedEngineState != -1)
				{
					if (engineState != data.engine.forcedEngineState)
					{
						VEHICLE::SET_VEHICLE_ENGINE_ON(game.entity, data.engine.forcedEngineState, true, true);
						VEHICLE::SET_VEHICLE_UNDRIVEABLE(game.entity, !data.engine.forcedEngineState);
					}
				}
				else if (data.engine.engineState.Get() != engineState)
				{
					data.engine.engineState.Set(engineState, RakNet::UNASSIGNED_RAKNET_GUID, identity.entityId, identity.type);
				}

#pragma region HeadLights
				bool HeadlightState, HighbeamState;
				VEHICLE::GET_VEHICLE_LIGHTS_STATE(game.entity, &HeadlightState, &HighbeamState);

				data.headlightState.Set(HeadlightState, RakNet::UNASSIGNED_RAKNET_GUID, identity.entityId, identity.type);
				data.highbeamState.Set(HighbeamState, RakNet::UNASSIGNED_RAKNET_GUID, identity.entityId, identity.type);
#pragma endregion

				if (VEHICLE::IS_VEHICLE_A_CONVERTIBLE(game.entity, false))
				{
					const int convertibleroofstate = VEHICLE::GET_CONVERTIBLE_ROOF_STATE(game.entity);
					if (convertibleroofstate != data.roofState.Get())
					{
						data.roofState.Set(convertibleroofstate, RakNet::UNASSIGNED_RAKNET_GUID, identity.entityId, identity.type);
					}
				}

				if (data.hasRocketboost)
				{
					const bool boostState = VEHICLE::_IS_VEHICLE_ROCKET_BOOST_ACTIVE(game.entity);
					if (boostState != data.boostActive.Get())
					{
						data.boostActive.Set(boostState, RakNet::UNASSIGNED_RAKNET_GUID, identity.entityId, identity.type);
					}
				}

				if (data.Class == 11)
				{
					Vehicle veh = VEHICLE::GET_ENTITY_ATTACHED_TO_TOW_TRUCK(game.entity);
					if (veh > 0)
					{
						for (unsigned int i = 0; i < g_Vehicles.size(); i++)
						{
							if (g_Vehicles[i].GetEntity() == veh)
							{
								if (data.towedVehicle != g_Vehicles[i].GetID())
								{
									data.towedVehicle = g_Vehicles[i].GetID();

									RakNet::BitStream bitstream;
									bitstream.Write(identity.entityId);
									bitstream.Write(g_Vehicles[i].GetID());
									CNetworkManager::GetRPC().Signal("TowVehicle", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);

									if (g_Vehicles[i].GetAssignee() != CNetworkManager::GetInterface()->GetMyGUID())
									{
										g_Vehicles[i].SetAssignee(CNetworkManager::GetInterface()->GetMyGUID());

										bitstream.Reset();
										bitstream.Write(identity.entityId);
										bitstream.Write(identity.type);
										bitstream.Write(CNetworkManager::GetInterface()->GetMyGUID());
										CNetworkManager::GetRPC().Signal("SetPhysicsAssignment", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
									}
								}

								break;
							}
						}
					}
					else if (data.towedVehicle != -1)
					{
						RakNet::BitStream bitstream;
						bitstream.Write(identity.entityId);
						bitstream.Write(-1);
						CNetworkManager::GetRPC().Signal("TowVehicle", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);

						data.towedVehicle = -1;
					}
				}
				else if (data.Class == 15)
				{
					if (VEHICLE::DOES_CARGOBOB_HAVE_PICK_UP_ROPE(game.entity))
					{
						if (data.cargobobRopeType != 0)
						{
							data.cargobobRopeType = 0;

							RakNet::BitStream bitstream;
							bitstream.Write(identity.entityId);
							bitstream.Write(0);
							CNetworkManager::GetRPC().Signal("DeployCargobobRope", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
						}
					}
					else if (VEHICLE::DOES_CARGOBOB_HAVE_PICKUP_MAGNET(game.entity))
					{
						if (data.cargobobRopeType != 1)
						{
							data.cargobobRopeType = 1;

							RakNet::BitStream bitstream;
							bitstream.Write(identity.entityId);
							bitstream.Write(1);
							CNetworkManager::GetRPC().Signal("DeployCargobobRope", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
						}
					}
					else if (data.cargobobRopeType != -1)
					{
						data.cargobobRopeType = -1;

						RakNet::BitStream bitstream;
						bitstream.Write(identity.entityId);
						bitstream.Write(-1);
						CNetworkManager::GetRPC().Signal("DeployCargobobRope", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
					}


					if (data.cargobobRopeType != -1)
					{
						Vehicle veh = VEHICLE::GET_VEHICLE_ATTACHED_TO_CARGOBOB(game.entity);
						if (veh > 0)
						{
							for (unsigned int i = 0; i < g_Vehicles.size(); i++)
							{
								if (g_Vehicles[i].GetEntity() == veh)
								{
									if (data.towedVehicle != g_Vehicles[i].GetID())
									{
										data.towedVehicle = g_Vehicles[i].GetID();

										RakNet::BitStream bitstream;
										bitstream.Write(identity.entityId);
										bitstream.Write(g_Vehicles[i].GetID());
										CNetworkManager::GetRPC().Signal("TowVehicle", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);

										if (g_Vehicles[i].GetAssignee() != CNetworkManager::GetInterface()->GetMyGUID())
										{
											g_Vehicles[i].SetAssignee(CNetworkManager::GetInterface()->GetMyGUID());

											bitstream.Reset();
											bitstream.Write(identity.entityId);
											bitstream.Write(identity.type);
											bitstream.Write(CNetworkManager::GetInterface()->GetMyGUID());
											CNetworkManager::GetRPC().Signal("SetPhysicsAssignment", &bitstream, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
										}
									}

									break;
								}
							}
						}
					}
				}

				data.gear = vehExt.GetGearCurr(game.entity);
				data.rpm = vehExt.GetCurrentRPM(game.entity);

				data.clutch = vehExt.GetClutch(game.entity);
				data.turbo = vehExt.GetTurbo(game.entity);
				data.acceleration = vehExt.GetThrottle(game.entity);
				data.brake = vehExt.GetBrakeP(game.entity);

				data.dashSpeed = vehExt.GetDashSpeed(game.entity);
				data.steeringAngle = vehExt.GetSteeringAngle(game.entity);
				data.forwardWheelAngle = /*vehExt.GetSteeringInputAngle(game.entity)*/vehExt.GetThrottleP(game.entity);

				data.engine.health.Set(VEHICLE::GET_VEHICLE_ENGINE_HEALTH(game.entity), RakNet::UNASSIGNED_RAKNET_GUID, identity.entityId, identity.type);

				/* Health Check for 'world' damage */
				const int curHealth = ENTITY::GET_ENTITY_HEALTH(game.entity);
				if (curHealth < health.Get())
				{
					GrandM::Network::Callback::OnEntityDamage(identity.entityId, (health.Get() - curHealth), -1, "World");
					health.Set(curHealth);
				}

				// BitStream
				RakNet::BitStream bitstream;
				bitstream.Write((unsigned char)ID_PACKET_VEHICLE);

				bitstream.Write(identity.entityId);

				bitstream.Write(data.gear);
				bitstream.Write(data.rpm);

				bitstream.Write(data.clutch);
				bitstream.Write(data.turbo);
				bitstream.Write(data.acceleration);
				bitstream.Write(data.brake);

				bitstream.Write(data.dashSpeed);
				bitstream.Write(data.steeringAngle);
				bitstream.Write(data.forwardWheelAngle);

				CNetworkManager::GetInterface()->Send(&bitstream, MEDIUM_PRIORITY, UNRELIABLE_SEQUENCED, 0, CNetworkManager::GetSystemAddress(), false);

				vehicleSync.lastSyncSent = getticktime();
			}
		}

#ifdef _VERBOSE_LOGING
		LOG_DEBUG("Finished Pulsing Vehicle: %i", identity.entityId);
#endif
	}
}

void CVehicle::UpdateVehicleData(RakNet::Packet * packet)
{
	RakNet::BitStream bitstream(packet->data + 1, packet->length + 1, false);

	bitstream.Read(identity.entityId);

	bitstream.Read(data.gear);
	bitstream.Read(data.rpm);

	bitstream.Read(data.clutch);
	bitstream.Read(data.turbo);
	bitstream.Read(data.acceleration);
	bitstream.Read(data.brake);

	bitstream.Read(data.dashSpeed);
	bitstream.Read(data.steeringAngle);
	bitstream.Read(data.forwardWheelAngle);

	for (unsigned int i = 0; i < SizeOfArray(occupants); i++)
	{
		bitstream.Read(occupants[i]);
	}


	SetData();

	//#ifndef LOCALTESTING
	//	if ( (Network.Assigned != CNetworkManager::GetInterface()->GetMyGUID()) || (identity.entityId == CLocalPlayer::GetVehicleId() && CLocalPlayer::GetSeat() != 0) || identity.entityId != CLocalPlayer::GetVehicleId())
	//	if (network.physicsAssignee != CNetworkManager::GetInterface()->GetMyGUID() || (identity.entityId == CLocalPlayer::GetVehicleID() && CLocalPlayer::GetSeat() != 0))
	//#else
	//	if ((identity.entityId == CLocalPlayer::GetVehicleId() && CLocalPlayer::GetSeat() != 0) || identity.entityId != CLocalPlayer::GetVehicleId())
	//#endif
	//	{
	//		SetTargetPosition();
	//		SetTargetRotation();
	//	}

	vehicleSync.lastSyncReceived = getticktime();
}

//void CVehicle::Interpolate()
//{
//#ifndef LOCALTESTING
//	//if ((Network.Assigned != CNetworkManager::GetInterface()->GetMyGUID()) || (identity.entityId == CLocalPlayer::GetVehicleId() && CLocalPlayer::GetSeat() != 0) || identity.entityId != CLocalPlayer::GetVehicleId())
//	if (game.physicsAssignee != CNetworkManager::GetInterface()->GetMyGUID() || (identity.entityId == CLocalPlayer::GetVehicleID() && CLocalPlayer::GetSeat() != 0))
//#else
//	if ((identity.entityId == CLocalPlayer::GetVehicleId() && CLocalPlayer::GetSeat() != 0) || identity.entityId != CLocalPlayer::GetVehicleId())
//#endif
//	{
//		SetTargetData();
//		DebugUI();
//	}
//}

//void CVehicle::UpdateTargetPosition()
//{
//	// Grab the current game position
//	Vector3 localCoords = ENTITY::GET_ENTITY_COORDS(game.entity, ENTITY::IS_ENTITY_DEAD(game.entity, 0));
//	CVector3 vecCurrentPosition = { localCoords.x, localCoords.y, localCoords.z };
//
//	// Get the factor of time spent from the interpolation start
//	// to the current time.
//	unsigned long ulCurrentTime = timeGetTime();
//	float fAlpha = Math::Unlerp(interpolation.position.startTime,
//		ulCurrentTime,
//		interpolation.position.finishTime);
//
//	// Don't let it overcompensate the error too much
//	fAlpha = Math::Clamp(0.0f, fAlpha, 1.5f);
//
//	// Get the current error portion to compensate
//	float fCurrentAlpha = fAlpha - interpolation.position.lastAlpha;
//	interpolation.position.lastAlpha = fAlpha;
//
//	// Apply the error compensation
//	CVector3 vecCompensation = Math::Lerp(CVector3(), fCurrentAlpha, interpolation.position.error);
//
//	// If we finished compensating the error, finish it for the next pulse
//	if (fAlpha == 1.5f)
//	{
//		interpolation.position.finishTime = 0;
//	}
//
//	CVector3 vecNewPosition = vecCurrentPosition + vecCompensation;
//
//	// Don't bother interpolating if the distance is greater than x (should prevent static vehicles bouncing)
//	if (CVector3::Distance(vecNewPosition, vecCurrentPosition) <= 0.05f)
//		return;
//
//	// Check if the distance to interpolate is too far.
//	float fThreshold = (15 + 10 * transform.Velocity.Magnitude()) /** g_pGame->GetGameSpeed()*/ * 100 / 100;
//
//	// To prevent NaNs generating new NaNs after interpolating (Comparing with NaNs always results in false).
//	if (!((vecCurrentPosition - interpolation.position.target).Magnitude() <= fThreshold))
//	{
//		// Abort all interpolation
//		interpolation.position.finishTime = 0;
//		vecNewPosition = interpolation.position.target;
//
//		/*if (HasTargetRotation())
//			SetRotationDegrees(m_interp.rot.vecTarget);
//		m_interp.rot.ulFinishTime = 0;*/
//	}
//
//	ENTITY::SET_ENTITY_COORDS_NO_OFFSET(game.entity, vecNewPosition.x, vecNewPosition.y, vecNewPosition.z, false, false, false);
//	ENTITY::SET_ENTITY_VELOCITY(game.entity, transform.Velocity.x, transform.Velocity.y, transform.Velocity.z);
//}

void CVehicle::SetTargetPosition()
{
	if (game.created && Exists())
	{
		UpdateTargetPosition();

		unsigned long curTime = timeGetTime();
		Vector3 localCoords = ENTITY::GET_ENTITY_COORDS(game.entity, ENTITY::IS_ENTITY_DEAD(game.entity, 0));
		CVector3 vecLocalPosition = { localCoords.x, localCoords.y, localCoords.z };

		interpolation.position.target = transform.Position;
		// Calculate the relative error
		interpolation.position.error = transform.Position - vecLocalPosition;

		// Extrapolation
		///needs doing
		//const SVehExtrapolateSettings& vehExtrapolate = g_pClientGame->GetVehExtrapolateSettings();
		//if (vehExtrapolate.bEnabled)
		//{
		//	// Base amount to account for something
		//	int iExtrapolateMs = vehExtrapolate.iBaseMs;

		//	if (CClientPlayer* pPlayerDriver = DynamicCast < CClientPlayer >((CClientEntity*)m_pDriver))
		//		iExtrapolateMs += pPlayerDriver->GetLatency() * vehExtrapolate.iScalePercent / 110;

		//	// Limit amount
		//	iExtrapolateMs = Clamp(0, iExtrapolateMs, vehExtrapolate.iMaxMs);

		//	CVector vecVelocity;
		//	GetMoveSpeed(vecVelocity);
		//	vecVelocity *= 50.f * iExtrapolateMs * (1 / 1000.f);
		//	m_interp.pos.vecError += vecVelocity;
		//}

		// Apply the error over 400ms (i.e. 1/4 per 100ms )
		interpolation.position.error *= Math::Lerp< const float >(0.25f, Math::UnlerpClamped(100, 100, 400), 1.0f);

		// Get the interpolation interval
		interpolation.position.startTime = curTime;
		interpolation.position.finishTime = curTime + 100;

		// Initialize the interpolation
		interpolation.position.lastAlpha = 0.0f;
	}
}

//void CVehicle::UpdateTargetRotation()
//{
//	// Grab the current game rotation
//	Vector3 CurrentRotationVec = ENTITY::GET_ENTITY_ROTATION(game.entity, 2);
//	CVector3 vecCurrentRotation(CurrentRotationVec.x, CurrentRotationVec.y, CurrentRotationVec.z);
//
//	// Get the factor of time spent from the interpolation start
//	// to the current time.
//	unsigned long CurrentTime = timeGetTime();
//	float fAlpha = Math::Unlerp(interpolation.rotation.startTime, CurrentTime, interpolation.rotation.finishTime);
//
//	// Don't let it to overcompensate the error
//	fAlpha = Math::Clamp(0.0f, fAlpha, 1.0f);
//
//	// Get the current error portion to compensate
//	float fCurrentAlpha = (fAlpha - interpolation.rotation.lastAlpha);
//	interpolation.rotation.lastAlpha = fAlpha;
//
//	CVector3 vecCompensation = Math::Lerp(CVector3(), fCurrentAlpha, interpolation.rotation.error);
//
//	// If we finished compensating the error, finish it for the next pulse
//	if (fAlpha == 1.0f)
//		interpolation.rotation.finishTime = 0;
//
//	// Calculate the new position
//	CVector3 vecNewRotation = vecCurrentRotation + vecCompensation;
//
//	ENTITY::SET_ENTITY_ROTATION(game.entity, vecNewRotation.x, vecNewRotation.y, vecNewRotation.z, 2, true);
//}

void CVehicle::SetTargetRotation()
{
	if (game.created && Exists())
	{
		UpdateTargetRotation();

		unsigned long CurrentTime = timeGetTime();
		unsigned int Delay = CurrentTime - sync.lastSyncReceived;

		Vector3 CurrentRotationVec = ENTITY::GET_ENTITY_ROTATION(game.entity, 2);
		CVector3 CurrentRotation(CurrentRotationVec.x, CurrentRotationVec.y, CurrentRotationVec.z);

		interpolation.rotation.target = transform.Rotation;

		// Get the error
		interpolation.rotation.error.x = Math::GetOffsetDegrees(CurrentRotation.x, transform.Rotation.x);
		interpolation.rotation.error.y = Math::GetOffsetDegrees(CurrentRotation.y, transform.Rotation.y);
		interpolation.rotation.error.z = Math::GetOffsetDegrees(CurrentRotation.z, transform.Rotation.z);

		// Apply the error over 250ms (i.e. 2/5 per 100ms )
		interpolation.rotation.error *= Math::Lerp < const float >(0.40f, Math::UnlerpClamped(100, Delay, 400), 1.0f);

		// Get the interpolation interval
		interpolation.rotation.startTime = CurrentTime;
		interpolation.rotation.finishTime = CurrentTime + Delay;

		// Initialize the interpolation
		interpolation.rotation.lastAlpha = 0.0f;
	}
}

void CVehicle::SetData()
{
	if (game.created && Exists())
	{
		const bool engineState = VEHICLE::GET_IS_VEHICLE_ENGINE_RUNNING(game.entity);
		if (data.engine.forcedEngineState != -1)
		{
			if (engineState != data.engine.forcedEngineState)
			{
				VEHICLE::SET_VEHICLE_ENGINE_ON(game.entity, data.engine.forcedEngineState, true, true);
				VEHICLE::SET_VEHICLE_UNDRIVEABLE(game.entity, !data.engine.forcedEngineState);
			}
		}
		else
		{
			if (engineState != data.engine.forcedEngineState)
			{
				VEHICLE::SET_VEHICLE_ENGINE_ON(game.entity, data.engine.forcedEngineState, true, true);
				VEHICLE::SET_VEHICLE_UNDRIVEABLE(game.entity, false);
			}
		}

		// Engine & Fuel Health
		VEHICLE::SET_VEHICLE_ENGINE_HEALTH(game.entity, data.engine.health.Get());
		VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(game.entity, data.engine.fuelHealth.Get());

		if (data.Class == 11)
		{
			if (data.towedVehicle != -1)
			{
				if (VEHICLE::GET_ENTITY_ATTACHED_TO_TOW_TRUCK(game.entity) == 0 && transform.Velocity.IsNull())
				{
					for (unsigned int i = 0; i < g_Vehicles.size(); i++)
					{
						if (g_Vehicles[i].GetID() == data.towedVehicle)
						{
							if (g_Vehicles[i].IsCreated() && g_Vehicles[i].Exists())
								if (!VEHICLE::IS_VEHICLE_ATTACHED_TO_TOW_TRUCK(game.entity, g_Vehicles[i].GetEntity()))
									VEHICLE::ATTACH_VEHICLE_TO_TOW_TRUCK(game.entity, g_Vehicles[i].GetEntity(), false, 0.0f, 0.0f, 0.0f); // The rear option seems to be ignored

							break;
						}
					}
				}
			}
			else if (VEHICLE::GET_ENTITY_ATTACHED_TO_TOW_TRUCK(game.entity) > 0)
			{
				VEHICLE::DETACH_VEHICLE_FROM_ANY_TOW_TRUCK(VEHICLE::GET_ENTITY_ATTACHED_TO_TOW_TRUCK(game.entity));
			}
		}
		else if (data.Class == 15)
		{
			if (data.cargobobRopeType != -1)
			{
				if (data.cargobobRopeType == 0 && !VEHICLE::DOES_CARGOBOB_HAVE_PICK_UP_ROPE(game.entity))
				{
					VEHICLE::CREATE_PICK_UP_ROPE_FOR_CARGOBOB(game.entity, 0);
				}
				else if (data.cargobobRopeType == 1 && !VEHICLE::DOES_CARGOBOB_HAVE_PICK_UP_ROPE(game.entity))
				{
					VEHICLE::CREATE_PICK_UP_ROPE_FOR_CARGOBOB(game.entity, 1);
				}
				else if (data.cargobobRopeType == -1 && VEHICLE::DOES_CARGOBOB_HAVE_PICK_UP_ROPE(game.entity))
				{
					VEHICLE::REMOVE_PICK_UP_ROPE_FOR_CARGOBOB(game.entity);
				}

				if (data.towedVehicle != -1)
				{
					if (!VEHICLE::GET_VEHICLE_ATTACHED_TO_CARGOBOB(game.entity) && transform.Velocity.IsNull())
					{
						for (unsigned int i = 0; i < g_Vehicles.size(); i++)
						{
							if (g_Vehicles[i].GetID() == data.towedVehicle)
							{
								if (g_Vehicles[i].IsCreated() && g_Vehicles[i].Exists())
									if (!VEHICLE::IS_VEHICLE_ATTACHED_TO_CARGOBOB(game.entity, g_Vehicles[i].GetEntity()))
										VEHICLE::ATTACH_VEHICLE_TO_CARGOBOB(game.entity, g_Vehicles[i].GetEntity(), 0, 0.0f, 0.0f, 0.0f);

								break;
							}
						}
					}
				}
				else if (VEHICLE::GET_VEHICLE_ATTACHED_TO_CARGOBOB(game.entity))
				{
					VEHICLE::DETACH_VEHICLE_FROM_CARGOBOB(game.entity, VEHICLE::GET_VEHICLE_ATTACHED_TO_CARGOBOB(game.entity));
				}
			}
		}

		vehExt.SetGearCurr(game.entity, data.gear);
		vehExt.SetCurrentRPM(game.entity, data.rpm);

		vehExt.SetClutch(game.entity, data.clutch);
		vehExt.SetTurbo(game.entity, data.turbo);
		vehExt.SetThrottle(game.entity, data.acceleration);
		vehExt.SetBrakeP(game.entity, data.brake);

		vehExt.SetSteeringAngle(game.entity, data.steeringAngle);
		vehExt.SetThrottleP(game.entity, data.forwardWheelAngle);
		vehExt.SetDashSpeed(game.entity, data.dashSpeed);


		sync.lastUpdateData = getticktime();
	}
}


Vehicle CVehicle::getClosestVehicleFromPedPos(Ped ped, int maxDistance)
{
	Vector3 ppos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(ped, 0.0, 0.0, -1.0f);
	CVector3 playerPos = CVector3(ppos.x, ppos.y, ppos.z);

	Vehicle vehicle = 0;
	float lastMaxDistance = maxDistance;

	if (!g_Vehicles.empty())
	{
		for (unsigned int i = 0; i < g_Vehicles.size(); i++)
		{
			if (g_Vehicles[i].GetID() != -1)
			{
				Vector3 vpos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(g_Vehicles[i].GetEntity(), 0.0, 0.0, 0.0f);
				CVector3 vehiclePos = CVector3(vpos.x, vpos.y, vpos.z);

				float distance = Math::GetDistanceBetweenPoints3D(ppos.x, ppos.y, ppos.z, vpos.x, vpos.y, vpos.z);
				if (distance < lastMaxDistance)
				{
					lastMaxDistance = distance;
					vehicle = g_Vehicles[i].GetEntity();
				}

			}
		}
	}

	return vehicle;
}

void CVehicle::SetColor(const int layer, const int painttype, const int color)
{
	if (layer == 1)
	{
		data.model.Colors[0].type = painttype;
		data.model.Colors[0].color = color;
		data.model.Colors[0].custom = false;
	}
	else if (layer == 2)
	{
		data.model.Colors[1].type = painttype;
		data.model.Colors[1].color = color;
		data.model.Colors[1].custom = false;
	}

	if (game.created && Exists())
	{
		if (layer == 1)
			VEHICLE::SET_VEHICLE_MOD_COLOR_1(game.entity, painttype, color, 0);
		else if (layer == 2)
			VEHICLE::SET_VEHICLE_MOD_COLOR_2(game.entity, painttype, color);
	}
}

void CVehicle::SetColor(const int layer, const Color color)
{
	if (layer == 1)
	{
		data.model.Colors[0].custom = true;
		data.model.Colors[0].customCol = color;
	}
	else if (layer == 2)
	{
		data.model.Colors[1].custom = true;
		data.model.Colors[1].customCol = color;
	}

	if (game.created && Exists())
	{
		if (layer == 1)
			VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(game.entity, color.Red, color.Green, color.Blue);
		else if (layer == 2)
			VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(game.entity, color.Red, color.Green, color.Blue);
	}
}

void CVehicle::SetNumberPlate(const std::string plate)
{
	data.plate.text = plate;

	if (game.created && Exists())
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT(game.entity, (char *)data.plate.text.c_str());
}

void CVehicle::SetMod(const int modType, const int modIndex)
{
	data.model.Mods[modType].index = modIndex;

	if (game.created && Exists())
		VEHICLE::SET_VEHICLE_MOD(game.entity, modType, modIndex, false);
}

void CVehicle::SetEngineState(const bool state, const bool instant, const bool forced)
{
	if (forced)
		data.engine.forcedEngineState = state;


	data.engine.engineState.Set(state, RakNet::UNASSIGNED_RAKNET_GUID, identity.entityId, identity.type);

	if (game.created && Exists())
	{
		VEHICLE::SET_VEHICLE_ENGINE_ON(game.entity, state, instant, true);
		VEHICLE::SET_VEHICLE_UNDRIVEABLE(game.entity, !state);
	}
}

void CVehicle::SetEngineState(const bool state, RakNet::RakNetGUID sender)
{
	data.engine.engineState.Set(state, sender, identity.entityId, identity.type);

	if (game.created && Exists())
	{
		VEHICLE::SET_VEHICLE_ENGINE_ON(game.entity, state, false, true);
		VEHICLE::SET_VEHICLE_UNDRIVEABLE(game.entity, !state);
	}
}

void CVehicle::SetDoorsLockState(const int state)
{
	data.doorsLockState = state;

	if (game.created && Exists())
	{
		VEHICLE::SET_VEHICLE_DOORS_LOCKED(game.entity, state);
	}
}

void CVehicle::SetNumberPlateStyle(const int style)
{
	data.plate.style = style;

	if (game.created && Exists())
	{
		VEHICLE::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX(game.entity, style);
	}
}

void CVehicle::SetExtra(const int extra, const bool toggle)
{
	data.model.Extras[extra].enabled = !toggle;

	if (game.created && Exists())
	{
		VEHICLE::SET_VEHICLE_EXTRA(game.entity, extra, !toggle);
	}
}

void CVehicle::SetEngineHealth(const float health, RakNet::RakNetGUID sender)
{
	data.engine.health.Set(health, sender, identity.entityId, identity.type);

	if (game.created && Exists())
	{
		VEHICLE::SET_VEHICLE_ENGINE_HEALTH(game.entity, health);
	}
}

void CVehicle::SetFuelTankHealth(const float health, RakNet::RakNetGUID sender)
{
	data.engine.health.Set(health, sender, identity.entityId, identity.type);

	if (game.created && Exists())
	{
		VEHICLE::SET_VEHICLE_PETROL_TANK_HEALTH(game.entity, health);
	}
}

void CVehicle::FixDeformation()
{
	if (game.created && Exists())
	{
		VEHICLE::SET_VEHICLE_DEFORMATION_FIXED(game.entity);
	}
}

void CVehicle::SetTaxiLightState(const bool state, RakNet::RakNetGUID sender)
{
	data.taxiLight.Set(state, sender, identity.entityId, identity.type);

	if (game.created && Exists())
	{
		VEHICLE::SET_TAXI_LIGHTS(game.entity, state);
	}
}

void CVehicle::SetIndicatorState(const int turnlight, const bool state, RakNet::RakNetGUID sender)
{
	if (turnlight == 0)
		data.rightIndicator.Set(state, sender, identity.entityId, identity.type);
	else if (turnlight == 1)
		data.leftIndicator.Set(state, sender, identity.entityId, identity.type);

	if (game.created && Exists())
	{
		VEHICLE::SET_VEHICLE_INDICATOR_LIGHTS(game.entity, turnlight, state);
	}
}

void CVehicle::SetInteriorLightState(const bool state, RakNet::RakNetGUID sender)
{
	data.interiorLight.Set(state, sender, identity.entityId, identity.type);

	if (game.created && Exists())
	{
		VEHICLE::SET_VEHICLE_INTERIORLIGHT(game.entity, state);
	}
}

void CVehicle::SetSirenSoundState(const bool state, RakNet::RakNetGUID sender)
{
	data.sirenSound.Set(state, sender, identity.entityId, identity.type);

	if (game.created && Exists())
	{
		VEHICLE::_SET_VEHICLE_SIREN_SOUND(game.entity, state);
	}
}

void CVehicle::SetHornState(const bool state, RakNet::RakNetGUID sender)
{
	data.horn.Set(state, sender, identity.entityId, identity.type);

	if (game.created && Exists())
	{
		VEHICLE::START_VEHICLE_HORN(game.entity, 0, $("HELDDOWN"), state);
	}
}

void CVehicle::Assignment()
{
	if (identity.entityId == -1)
	{
		LOG_ERROR("CVehicle::Assignment | One of the vehicles is of id -1...");
		return;
	}

	Vector3 Coords = ENTITY::GET_ENTITY_COORDS(game.entity, ENTITY::IS_ENTITY_DEAD(game.entity, 0));
	CVector3 Position = { Coords.x, Coords.y, Coords.z };
	RakNet::BitStream bs;

	// If the vehicle is within 50 meters of the local player...
	if (CVector3::Distance(CLocalPlayer::GetPosition(), Position) <= 50.0f)
	{
		// and its not assigned to anyone else & has no driver take physics assignment
		if (game.physicsAssignee == RakNet::UNASSIGNED_RAKNET_GUID && occupants[0] == -1)
		{
			game.physicsAssignee = CNetworkManager::GetInterface()->GetMyGUID();

			bs.Write(identity.entityId);
			bs.Write((int)identity.type);
			bs.Write(CNetworkManager::GetInterface()->GetMyGUID());
			CNetworkManager::GetRPC().Signal("SetPhysicsAssignment", &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
		}
	}
	// Else the vehicle is outside the 50 meter radius...
	else
	{
		// and its assigned to us
		if (CNetworkManager::GetInterface()->GetMyGUID() == game.physicsAssignee)
		{
			// drop physics assignment
			game.physicsAssignee = RakNet::UNASSIGNED_RAKNET_GUID;

			bs.Write(identity.entityId);
			bs.Write((int)identity.type);
			bs.Write(RakNet::UNASSIGNED_RAKNET_GUID);
			CNetworkManager::GetRPC().Signal("SetPhysicsAssignment", &bs, HIGH_PRIORITY, RELIABLE_ORDERED, 0, CNetworkManager::GetSystemAddress(), false, false);
		}
	}
}

void CVehicle::SetHeadlightState(const bool state, RakNet::RakNetGUID sender)
{
	data.headlightState.Set(state, sender, identity.entityId, identity.type);

	if (game.created && Exists())
	{
		if (state)
			VEHICLE::SET_VEHICLE_LIGHTS(game.entity, 0);
		else
			VEHICLE::SET_VEHICLE_LIGHTS(game.entity, 1);
	}
}

void CVehicle::SetHighbeamState(const bool state, RakNet::RakNetGUID sender)
{
	data.highbeamState.Set(state, sender, identity.entityId, identity.type);

	VEHICLE::SET_VEHICLE_FULLBEAM(game.entity, state);
}

/*0 -> up
1 -> lowering down
2 -> down
3 -> raising up*/
void CVehicle::SetRoofState(const int state, RakNet::RakNetGUID sender)
{
	data.roofState.Set(state, sender, identity.entityId, identity.type);

	if (game.created && Exists())
	{
		if (state == 0)
		{
			VEHICLE::RAISE_CONVERTIBLE_ROOF(game.entity, true);
		}
		else if (state == 1)
		{
			VEHICLE::LOWER_CONVERTIBLE_ROOF(game.entity, false);
		}
		else if (state == 2)
		{
			VEHICLE::LOWER_CONVERTIBLE_ROOF(game.entity, true);
		}
		else if (state == 3)
		{
			VEHICLE::RAISE_CONVERTIBLE_ROOF(game.entity, false);
		}
	}
}

const bool CVehicle::Exists()
{
	return ENTITY::DOES_ENTITY_EXIST(game.entity);
}

void CVehicle::SetBoostState(const bool state, RakNet::RakNetGUID sender)
{
	data.boostActive.Set(state, sender, identity.entityId, identity.type);

	if (game.created && Exists())
	{
		VEHICLE::_SET_VEHICLE_ROCKET_BOOST_ACTIVE(game.entity, state);
	}
}

void CVehicle::SetTowedVehicle(const int entity)
{
	data.towedVehicle = entity;

	if (game.created && Exists())
	{
		if (data.towedVehicle != -1)
		{
			if (data.Class == 11)
			{
				for (unsigned int i = 0; i < g_Vehicles.size(); i++)
				{
					if (g_Vehicles[i].GetID() == data.towedVehicle)
					{
						if (g_Vehicles[i].IsCreated() && g_Vehicles[i].Exists() && !VEHICLE::IS_VEHICLE_ATTACHED_TO_TOW_TRUCK(game.entity, g_Vehicles[i].GetEntity()))
						{
							VEHICLE::ATTACH_VEHICLE_TO_TOW_TRUCK(game.entity, g_Vehicles[i].GetEntity(), false, 0.0f, 0.0f, 0.0f);
						}

						return;
					}
				}
			}
			else if (data.Class == 15)
			{
				if (!VEHICLE::GET_VEHICLE_ATTACHED_TO_CARGOBOB(game.entity))
				{
					for (unsigned int i = 0; i < g_Vehicles.size(); i++)
					{
						if (g_Vehicles[i].GetID() == data.towedVehicle)
						{
							if (g_Vehicles[i].IsCreated() && g_Vehicles[i].Exists())
								if (!VEHICLE::IS_VEHICLE_ATTACHED_TO_CARGOBOB(game.entity, g_Vehicles[i].GetEntity()))
									VEHICLE::ATTACH_VEHICLE_TO_CARGOBOB(game.entity, g_Vehicles[i].GetEntity(), 0, 0.0f, 0.0f, 0.0f);

							break;
						}
					}
				}
			}
		}
		else
		{
			if (data.Class == 11)
				VEHICLE::DETACH_VEHICLE_FROM_ANY_TOW_TRUCK(VEHICLE::GET_ENTITY_ATTACHED_TO_TOW_TRUCK(game.entity));
			else if (data.Class == 15)
				VEHICLE::DETACH_VEHICLE_FROM_CARGOBOB(game.entity, VEHICLE::GET_VEHICLE_ATTACHED_TO_CARGOBOB(game.entity));
		}
	}
}

void CVehicle::SetCargobobRopeType(const int type)
{
	data.cargobobRopeType = type;

	if (game.created && Exists())
	{
		if (data.cargobobRopeType == 0 && !VEHICLE::DOES_CARGOBOB_HAVE_PICK_UP_ROPE(game.entity))
		{
			VEHICLE::CREATE_PICK_UP_ROPE_FOR_CARGOBOB(game.entity, 0);
		}
		else if (data.cargobobRopeType == 1 && !VEHICLE::DOES_CARGOBOB_HAVE_PICK_UP_ROPE(game.entity))
		{
			VEHICLE::CREATE_PICK_UP_ROPE_FOR_CARGOBOB(game.entity, 1);
		}
		else if (data.cargobobRopeType == -1 && VEHICLE::DOES_CARGOBOB_HAVE_PICK_UP_ROPE(game.entity))
		{
			VEHICLE::REMOVE_PICK_UP_ROPE_FOR_CARGOBOB(game.entity);
		}
	}
}