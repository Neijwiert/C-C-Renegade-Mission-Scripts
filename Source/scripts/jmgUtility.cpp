
#include "general.h"

#include "JmgUtility.h"
#include "GameObjManager.h"
#include "VehicleGameObj.h"
#include "WeaponClass.h"
#include "VehicleGameObjDef.h"
#include "OffenseObjectClass.h"
#include "SoldierGameObj.h"
#include "BuildingGameObj.h"
#include "engine_player.h"
bool JmgUtility::hasStatedDeathMessage[128] = {false};

void JMG_Utility_Check_If_Script_Is_In_Library::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Check_If_Script_Is_In_Library::Timer_Expired(GameObject *obj,int number)
{
	if (1 == number)
	{
		if (!ScriptRegistrar::GetScriptFactory(Get_Parameter("ScriptName")))
		{
			char errMsg[220],pErrMsg[220];
			sprintf(errMsg,"ERROR: The Script %s was not found in scripts.dll. Please check your workspace and make sure you have included %s.cpp and its includes!",Get_Parameter("ScriptName"),Get_Parameter("CppName"));
			Console_Input(errMsg);
			sprintf(pErrMsg,"msg %s",errMsg);
			Console_Input(pErrMsg);
		}
	}
}
void JMG_Send_Custom_When_Custom_Sequence_Matched::Created(GameObject *obj)
{
	depth = 0;
	failCount = 0;
	enabled = Get_Int_Parameter("Starts_Enabled") == 0 ? false : true;
	if (!Get_Int_Parameter("Custom_0"))
	{
		Console_Input("msg JMG_Send_Custom_When_Custom_Sequence_Matched ERROR: Custom_0 must not be 0!");
		enabled = false;
	}
}
void JMG_Send_Custom_When_Custom_Sequence_Matched::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom_0") || message == Get_Int_Parameter("Custom_1") || message == Get_Int_Parameter("Custom_2") || message == Get_Int_Parameter("Custom_3") || message == Get_Int_Parameter("Custom_4") || message == Get_Int_Parameter("Custom_5") || message == Get_Int_Parameter("Custom_6") || message == Get_Int_Parameter("Custom_7") || message == Get_Int_Parameter("Custom_8") || message == Get_Int_Parameter("Custom_9"))
	{
		if (!enabled)
			return;
		char custom[32];
		sprintf(custom,"Custom_%0d",depth);
		bool correctStep = false;
		if (message == Get_Int_Parameter(custom))
		{
			correctStep = true;
			depth++;
		}
		else
		{
			if (!Get_Int_Parameter("Failure_Saftey") || depth)
			{
				failCount++;
				if (failCount < Get_Int_Parameter("Max_Failures"))
				{
					Send_Custom(sender,Get_Int_Parameter("Partial_Failure_Custom"),depth);
					return;
				}
				Send_Custom(sender,Get_Int_Parameter("Failure_Custom"),depth);
				if (Get_Int_Parameter("Disable_On_Failure"))
					enabled = false;
			}
			depth = 0;
			failCount = 0;
		}
		sprintf(custom,"Custom_%0d",depth);
		if (depth >= 10 || !Get_Int_Parameter(custom))
		{
			if (Get_Int_Parameter("Disable_On_Success"))
				enabled = false;
			depth = 0;
			failCount = 0;
			Send_Custom(sender,Get_Int_Parameter("Success_Custom"),param);
		}
		else if (correctStep && (!Get_Int_Parameter("Correct_Step_Saftey") || depth))
			Send_Custom(sender,Get_Int_Parameter("Correct_Step_Custom"),depth);
	}
	if (message == Get_Int_Parameter("Enable_Custom"))
	{
		if (enabled)
			return;
		depth = 0;
		failCount = 0;
		enabled = true;
		if (!Get_Int_Parameter("Custom_0"))
		{
			Console_Input("msg JMG_Send_Custom_When_Custom_Sequence_Matched ERROR: Custom_0 must not be 0!");
			enabled = false;
		}
	}
}
void JMG_Send_Custom_When_Custom_Sequence_Matched::Send_Custom(GameObject *obj,int custom,int param)
{
	GameObject *sendId = Commands->Find_Object(Get_Int_Parameter("Send_To_ID"));
	if (sendId)
		Commands->Send_Custom_Event(obj,sendId,custom,param,0);
}
void JMG_Utility_Change_Model_On_Timer::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Time"),1);
}
void JMG_Utility_Change_Model_On_Timer::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Set_Model(obj,Get_Parameter("Model"));
	}
}
void JMG_Utility_Emulate_DamageableStaticPhys::Created(GameObject *obj)
{
	team = Commands->Get_Player_Type(obj);
	alive = true;
	playingTwitch = false;
	playingTransition = false;
	Set_Death_Points(obj,0);
	Set_Damage_Points(obj,Get_Float_Parameter("Damage_Points"));
	Play_Animation(obj,true,Get_Float_Parameter("Live_Loop_Start"),Get_Float_Parameter("Live_Loop_End"));
}
void JMG_Utility_Emulate_DamageableStaticPhys::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damage)
		return;
	if (alive && !Commands->Get_Shield_Strength(obj))
	{
		alive = false;
		if (Get_Int_Parameter("Neutral_When_Dead"))
			Commands->Set_Player_Type(obj,-2);
		Set_Damage_Points(obj,Get_Float_Parameter("Damage_Points"));
		Play_Animation(obj,false,Get_Float_Parameter("Death_Transition_Start"),Get_Float_Parameter("Death_Transition_End"));
		playingTransition = true;
		GameObject *sendId = Commands->Find_Object(Get_Int_Parameter("Send_To_ID"));
		if (sendId)
			Commands->Send_Custom_Event(obj,sendId,Get_Int_Parameter("Death_Custom"),Commands->Get_ID(damager),0);
		if (_stricmp(Get_Parameter("Killed_Explosion"),"null"))
			Commands->Create_Explosion(Get_Parameter("Killed_Explosion"),Commands->Get_Position(obj),damager);
		if (damager)
			Commands->Give_Points(damager,(Commands->Get_Player_Type(obj) == Commands->Get_Player_Type(damager) ? -1 : 1) * Get_Float_Parameter("Death_Points"),0);
		if (Get_Int_Parameter("Disable_Collision_When_Dead"))
			Commands->Disable_Physical_Collisions(obj);
	}
	if (!alive && Commands->Get_Shield_Strength(obj) >= Commands->Get_Max_Shield_Strength(obj))
	{
		alive = true;
		Commands->Set_Player_Type(obj,team);
		Set_Damage_Points(obj,Get_Float_Parameter("Heal_Points"));
		Play_Animation(obj,false,Get_Float_Parameter("Live_Transition_Start"),Get_Float_Parameter("Live_Transition_End"));
		playingTransition = true;
		GameObject *sendId = Commands->Find_Object(Get_Int_Parameter("Send_To_ID"));
		if (sendId)
			Commands->Send_Custom_Event(obj,sendId,Get_Int_Parameter("Repaired_Custom"),Commands->Get_ID(damager),0);
		if (_stricmp(Get_Parameter("Repaired_Explosion"),"null"))
			Commands->Create_Explosion(Get_Parameter("Repaired_Explosion"),Commands->Get_Position(obj),damager);
		if (damager)
			Commands->Give_Points(damager,(Commands->Get_Player_Type(obj) == Commands->Get_Player_Type(damager) ? 1 : -1) * Get_Float_Parameter("Repair_Points"),0);
		if (Get_Int_Parameter("Disable_Collision_When_Dead"))
			Commands->Enable_Collisions(obj);
	}
	if (!playingTransition && !playingTwitch)
	{
		if (alive)
			Play_Animation(obj,false,Get_Float_Parameter("Live_Twitch_Start"),Get_Float_Parameter("Live_Twitch_End"));
		else
			Play_Animation(obj,false,Get_Float_Parameter("Death_Twitch_Start"),Get_Float_Parameter("Death_Twitch_End"));
		playingTwitch = Get_Int_Parameter("Play_Twitches_To_Completion") ? true : false;
	}
}
void JMG_Utility_Emulate_DamageableStaticPhys::Animation_Complete(GameObject *obj,const char *anim)
{
	if (alive)
		Play_Animation(obj,true,Get_Float_Parameter("Live_Loop_Start"),Get_Float_Parameter("Live_Loop_End"));
	else
		Play_Animation(obj,true,Get_Float_Parameter("Death_Loop_Start"),Get_Float_Parameter("Death_Loop_End"));
	if (playingTransition)
		playingTransition = false;
	if (playingTwitch)
		playingTwitch = false;
}
void JMG_Utility_Emulate_DamageableStaticPhys::Play_Animation(GameObject *obj,bool loop,float start,float end)
{
	if (start == -1 || end == -1)
		return;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),loop,0,start,end,false);
}
void JMG_Utility_Display_HUD_Info_Text_All_Players_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
		JmgUtility::SetHUDHelpText(Get_Int_Parameter("StringId"),Get_Vector3_Parameter("ColorRGB"));
	if (!Get_Int_Parameter("Repeatable"))
		Destroy_Script();
}
void JMG_Utility_Display_HUD_Info_Text_To_Sender_On_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		GameObject *player = sender;
		GameObject *tempPlayer = Get_GameObj(param);
		if (tempPlayer)
			player = tempPlayer;
		Set_HUD_Help_Text_Player(player,Get_Int_Parameter("StringId"),Get_Vector3_Parameter("ColorRGB"));
	if (!Get_Int_Parameter("Repeatable"))
		Destroy_Script();
	}
}
void JMG_Utility_Soldier_Transition_On_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		if (Get_Vehicle(obj))
			Soldier_Transition_Vehicle(obj);
		else
		{
			Vector3 pos = Commands->Get_Position(obj);
			float dist = 0;
			GameObject *nearest = NULL;
			for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
			{
				SmartGameObj* o = current->Data();
				if (o->As_VehicleGameObj() && Get_Vehicle_Occupant_Count(o) < Get_Vehicle_Seat_Count(o))
				{
					float tempDistance = JmgUtility::SimpleDistance(pos,Commands->Get_Position(o));
					if (!nearest || tempDistance < dist)
					{
						nearest = o;
						dist = tempDistance;
					}
				}
			}
			if (nearest)
				Force_Vehicle_Entry(obj,nearest);
		}
	}
}
void JMG_Utility_Poke_Send_Self_Custom::Created(GameObject *obj)
{
	poked = false;
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void JMG_Utility_Poke_Send_Self_Custom::Poked(GameObject *obj, GameObject *poker)
{
	if (!poked && Commands->Is_A_Star(poker))
	{
		poked= true;
		Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
		Commands->Enable_HUD_Pokable_Indicator(obj,false);
		if (Get_Int_Parameter("LockoutTime") >= 0.0f)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("LockoutTime"),1);
	}
}
void JMG_Utility_Poke_Send_Self_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		poked = false;
		Commands->Enable_HUD_Pokable_Indicator(obj,true);
	}
}
void JMG_Turret_Spawn::Created(GameObject *obj)
{
	GameObject *turret = Commands->Create_Object(Get_Parameter("Turret_Preset"),Vector3());
	if(!turret)
	{
		Console_Output("[%d:%s:%s] JMG_Turret_Spawn Critical Error: Failed to create an instance of the preset %s. Destroying script...\n", Commands->Get_ID(obj), Commands->Get_Preset_Name(obj), this->Get_Name(), Get_Parameter("Turret_Preset"));
		Destroy_Script();
		return;
	}
	Commands->Attach_To_Object_Bone(turret,obj,Get_Parameter("Bone_Name"));
	turretId = Commands->Get_ID(turret);
	if (turret->As_VehicleGameObj())
		turret->As_VehicleGameObj()->Set_Is_Scripts_Visible(false);
	hasDriver = false;
}
void JMG_Turret_Spawn::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (!hasDriver)
		{
			hasDriver = true;
			GameObject *turret = Commands->Find_Object(turretId);
			if (turret)
			{
				Commands->Set_Player_Type(turret,Commands->Get_Player_Type(sender));
				Commands->Action_Reset(turret,100);
			}
		}
	}
	if (message == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (hasDriver && obj->As_VehicleGameObj() && !Get_Vehicle_Occupant_Count(obj))
		{
			hasDriver = false;
			GameObject *turret = Commands->Find_Object(turretId);
			if (turret)
			{
				Commands->Set_Player_Type(turret,Commands->Get_Player_Type(obj));
				Commands->Action_Reset(turret,100);
			}
		}
	}
}
void JMG_Turret_Spawn::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *turret = Commands->Find_Object(turretId);
	if (turret)
		Commands->Destroy_Object(turret);
}
void JMG_Turret_Spawn::Destroyed(GameObject *obj)
{
	GameObject *turret = Commands->Find_Object(turretId);
	if (turret)
		Commands->Destroy_Object(turret);
}
ClientNetworkObjectPositionSync *clientNetworkObjectPositionSyncControl = NULL;
void JMG_Utility_Sync_System_Object::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.25f,1);
	Commands->Start_Timer(obj,this,0.0f,2);
}
void JMG_Utility_Sync_System_Object::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!clientNetworkObjectPositionSyncControl)
		{
			Console_Input("msg JMG_Utility_Sync_System_Object ERROR: Make sure there is a JMG_Utility_Sync_System_Controller on the map!");
			return;
		}
		syncNode = clientNetworkObjectPositionSyncControl->addNode(obj);
	}
	if (number == 2)
	{
		Force_Position_Update(obj);
	}
}
void JMG_Utility_Sync_System_Object::Killed(GameObject *obj,GameObject *killer)
{
	if (syncNode)
		syncNode->id = 0;
}
void JMG_Utility_Sync_System_Object::Destroyed(GameObject *obj)
{
	if (The_Game()->Is_Game_Over())
		return;
	if (syncNode)
		syncNode->id = 0;
}
void JMG_Utility_Sync_System_Object::Detach(GameObject *obj)
{
	if (Exe == 4)
		return;
	Destroyed(obj);
}
JMG_Utility_Sync_System_Controller::JMG_Utility_Sync_System_Controller()
{
	clientNetworkObjectPositionSyncControl = new ClientNetworkObjectPositionSync();
}
void JMG_Utility_Sync_System_Controller::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,2.5f,1);
	Commands->Start_Timer(obj,this,2.5f,2);
}
void JMG_Utility_Sync_System_Controller::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (The_Game()->Is_Game_Over())
		{
			Destroyed(obj);
			return;
		}
		else
			clientNetworkObjectPositionSyncControl->checkForPlayersThatLeftTheGame();
		Commands->Start_Timer(obj,this,0.25f,1);
	}
	if (number == 2)
	{
		if (The_Game()->Is_Game_Over())
			return;
		clientNetworkObjectPositionSyncControl->triggerSingleNetworkSync();
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Sync_Rate"),2);
	}
}
void JMG_Utility_Sync_System_Controller::Destroyed(GameObject *obj)
{
	if (!clientNetworkObjectPositionSyncControl)
		return;
	clientNetworkObjectPositionSyncControl->Empty_List();
	clientNetworkObjectPositionSyncControl = NULL;
}
void JMG_Utility_Sync_Object_Periodically::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.25f,1);
}
void JMG_Utility_Sync_Object_Periodically::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Force_Position_Update(obj);
		Commands->Start_Timer(obj,this,Get_Float_Parameter("Sync_Rate"),1);
	}
}
Vector3 JMG_Utility_Basic_Spawner::preSpawnLocation = Vector3();
void JMG_Utility_Basic_Spawner::Created(GameObject *obj)
{
	spawnedId = 0;
	spawnLimit = Get_Int_Parameter("SpawnLimit");
	enabled = Get_Int_Parameter("StartsEnabled") ? true : false;
	CalculateRespawnTime();
	Commands->Disable_All_Collisions(obj);
	Commands->Set_Is_Rendered(obj,false);
	Commands->Set_Is_Visible(obj,false);
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Basic_Spawner::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!spawnedId && respawnTime > 0 && spawnLimit && enabled)
		{
			respawnTime -= 0.1f;
			if (respawnTime <= 0)
			{
				Vector3 createPos = Commands->Get_Position(obj);
				GameObject *spawnedObject = Commands->Create_Object(Get_Parameter("SpawnPreset"),preSpawnLocation);
				Commands->Set_Facing(spawnedObject,Commands->Get_Facing(obj));
				MoveablePhysClass *mphys = spawnedObject->As_PhysicalGameObj() ? spawnedObject->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
				if (!mphys || mphys->Can_Teleport(Matrix3D(createPos)))
				{
					if (spawnLimit > 0)
						spawnLimit--;
					Commands->Set_Position(spawnedObject,createPos);
					spawnedId = Commands->Get_ID(spawnedObject);
					char params[256];
					sprintf(params,"%d",Commands->Get_ID(obj));
					Commands->Attach_Script(spawnedObject,"JMG_Utility_Basic_Spawner_Spawned_Object",params);
					Commands->Send_Custom_Event(spawnedObject,obj,6873522,6873522,0.0f);
				}
				else
				{
					Commands->Destroy_Object(spawnedObject);
					if (spawnedObject->As_SoldierGameObj())
						ChangeTeamDeaths(Commands->Get_Player_Type(spawnedObject),-1);
					respawnTime = 1.0f;
				}
			}
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Basic_Spawner::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (Get_Int_Parameter("EnableOrDisableCustom") && message == Get_Int_Parameter("EnableOrDisableCustom"))
	{
		if (param == 1)
		{
			enabled = true;
			CalculateRespawnTime();
		}
		else if (!param)
			enabled = false;
		else if (param == -1)
		{
			enabled = false;
			GameObject *spawnedObject = Commands->Find_Object(spawnedId);
			if (spawnedObject)
				Commands->Apply_Damage(spawnedObject,9999.9f,"BlamoKiller",NULL);
		}
		else if (param == -2)
		{
			enabled = false;
			GameObject *spawnedObject = Commands->Find_Object(spawnedId);
			if (spawnedObject)
				Commands->Destroy_Object(spawnedObject);
		}
	}
	if (message == 6873521)
	{
		respawnTime = Get_Float_Parameter("RespawnTime")+(Get_Float_Parameter("RespawnTimeRandom") ? Commands->Get_Random(-Get_Float_Parameter("RespawnTimeRandom"),Get_Float_Parameter("RespawnTimeRandom")) : 0.0f);
		if (respawnTime <= 0.0f)
			respawnTime = 0.1f;
		spawnedId = 0;
	}
}
void JMG_Utility_Basic_Spawner::CalculateRespawnTime()
{
	respawnTime = min((Get_Int_Parameter("DelayOnStartup") ? (Get_Float_Parameter("RespawnTime")+(Get_Float_Parameter("RespawnTimeRandom") ? Commands->Get_Random(-Get_Float_Parameter("RespawnTimeRandom"),Get_Float_Parameter("RespawnTimeRandom")) : 0.0f)) : 0),0.1f);
	if (respawnTime <= 0.0f)
		respawnTime = 0.1f;
}
void JMG_Utility_Basic_Spawner_Spawned_Object::Destroyed(GameObject *obj)
{
	GameObject *spawner = Commands->Find_Object(Get_Int_Parameter("ControllerId"));
	if (spawner)
		Commands->Send_Custom_Event(obj,spawner,6873521,0,0.0f);
}
void JMG_Utility_AI_Engineer::Created(GameObject *obj)
{
	centerLocation = (JmgUtility::SimpleDistance(Get_Vector3_Parameter("BaseCenterPoint"),Vector3()) <= 0.0f ? Commands->Get_Position(obj) : Get_Vector3_Parameter("BaseCenterPoint"));
	maxRange = Get_Float_Parameter("PatrolRange")*Get_Float_Parameter("PatrolRange");
	if (!Has_Weapon(obj,Get_Parameter("RepaiarGun_Preset")))
		Grant_Weapon(obj,Get_Parameter("RepaiarGun_Preset"),true,-1,true);
	if (Has_Weapon(obj,Get_Parameter("RepaiarGun_Preset")))
		Commands->Select_Weapon(obj,Get_Parameter("RepaiarGun_Preset"));
	if (_stricmp(Get_Parameter("Weapon_Preset"),"null"))
	{
		if (!Has_Weapon(obj,Get_Parameter("Weapon_Preset")))
			Grant_Weapon(obj,Get_Parameter("Weapon_Preset"),true,-1,true);
		canFight = true;
	}
	else
		canFight = false;
	const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(Get_Parameter("RepaiarGun_Preset"),true);
	if (ammo)
	{
		repairGunRange = ammo->Range;
		repairGunEffectiveRange = ammo->Range < ammo->EffectiveRange ? ammo->Range : ammo->EffectiveRange;
	}
	ammo = Get_Weapon_Ammo_Definition(Get_Parameter("Weapon_Preset"),true);
	if (ammo)
	{
		weaponRange = ammo->Range;
		weaponEffectiveRange = ammo->Range < ammo->EffectiveRange ? ammo->Range : ammo->EffectiveRange;
	}
	targetId = 0;
	actionUpdate = 0;
	repairGun = true;
	lastPos = Commands->Get_Position(obj);
	samePosition = 0;
	repairTargetId = 0;
	lastTarget = NULL;
	lastSecondaryTarget = NULL;
	lastRepairTarget = false;
	lastUseRepairGun = false;
	for (int x = 0;x < 128;x++)
		aiIgnorePlayers[x] = 0;
	randomResetAmount = Commands->Get_Random_Int(0,4);
	wanderPointGroup = Get_Int_Parameter("WanderGroupID");
	Commands->Start_Timer(obj,this,0.25f,1);
	Commands->Start_Timer(obj,this,1.0f,5);
	Commands->Start_Timer(obj,this,1.0f,6);
	Commands->Enable_Enemy_Seen(obj,true);
}
void JMG_Utility_AI_Engineer::Enemy_Seen(GameObject *obj,GameObject *seen)	
{
	if (!canFight)
		return;
	if (seen->As_SmartGameObj() && seen->As_SmartGameObj()->Is_Stealthed() && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 900)
		return;
	if (targetId && targetUpdate)
		return;
	if (targetId && JmgUtility::SimpleDistance(Commands->Get_Position(seen),Commands->Get_Position(obj)) > targetDistance)
		return;
	if (aiIgnorePlayers[JmgUtility::JMG_Get_Player_ID(seen)])
		return;
	targetId = Commands->Get_ID(seen);
	targetUpdate = 20;
}
void JMG_Utility_AI_Engineer::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (Commands->Get_Health(obj) <= 0)
			return;
		int playerType = Commands->Get_Player_Type(obj);
		GameObject *target = targetId ? Commands->Find_Object(targetId) : NULL;
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *lastRep = Commands->Find_Object(repairTargetId);
		if (repairTargetId && (!lastRep || Get_Hitpoints(lastRep) >= Get_Max_Hitpoints(lastRep) || !Commands->Get_Health(lastRep)))
		{
			if (!lastRep || !Commands->Get_Health(lastRep) || Commands->Get_Random(0.0f,1.0f) < 0.33f)
				samePosition = 100;
			else
				samePosition = randomResetAmount;
			lastRep = NULL;
			repairTargetId = 0;
		}
		float nearestObject = lastRep ? JmgUtility::SimpleDistance(pos,Commands->Get_Position(lastRep)) : 0.0f;
		GameObject *repairTarget = lastRep;
		for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
		{
			SmartGameObj* o = current->Data();
			if (Valid_Repair_Target(obj,o,playerType))
			{
				if (!inRange(o))
					continue;
				float tempDistance = JmgUtility::SimpleDistance(pos,Commands->Get_Position(o))*Get_Vector3_Parameter("Priority[RepairTargets|C4|Beacons]").X;
				if (!repairTarget || tempDistance < nearestObject)
				{
					repairTarget = o;
					nearestObject = tempDistance;
				}
			}
		}
		if (Get_Vector3_Parameter("MinHP[EngineerTarget|C4|Beacon]").Y < 1.0)
			for (SLNode<C4GameObj> *current = GameObjManager::C4GameObjList.Head();current;current = current->Next())
			{
				GameObject *o = (GameObject *)current->Data();
				if (Valid_Repair_Target_C4(obj,o,playerType))
				{
					if (!inRange(o))
						continue;
					float tempDistance = JmgUtility::SimpleDistance(pos,Commands->Get_Position(o))*Get_Vector3_Parameter("Priority[RepairTargets|C4|Beacons]").Y;
					if (!repairTarget || tempDistance < nearestObject)
					{
						repairTarget = o;
						nearestObject = tempDistance;
					}
				}
			}
		if (Get_Vector3_Parameter("MinHP[EngineerTarget|C4|Beacon]").Z < 1.0)
			for (SLNode<BeaconGameObj> *current = GameObjManager::BeaconGameObjList.Head();current;current = current->Next())
			{
				GameObject *o = (GameObject *)current->Data();
				if (Valid_Repair_Target_C4(obj,o,playerType))
				{
					if (!inRange(o))
						continue;
					float tempDistance = JmgUtility::SimpleDistance(pos,Commands->Get_Position(o))*Get_Vector3_Parameter("Priority[RepairTargets|C4|Beacons]").Z;
					if (!repairTarget || tempDistance < nearestObject)
					{
						repairTarget = o;
						nearestObject = tempDistance;
					}
				}
			}
		if (repairTarget)
			repairTargetId = Commands->Get_ID(repairTarget);
		if (targetId && (!target || !Commands->Get_Health(target) || !inRange(target)))
		{
			target = NULL;
			targetUpdate = 0;
			targetId = 0;
		}
		if (target)
		{
			float repairTargetDistance = JmgUtility::SimpleDistance(Commands->Get_Position(repairTarget),pos);
			if (!repairTarget || repairTargetDistance > repairGunRange*repairGunRange || repairTargetDistance > JmgUtility::SimpleDistance(Commands->Get_Position(target),pos))
				AttackTarget(obj,repairTarget,target,true,false);
			else
				AttackTarget(obj,repairTarget,target,true,true);
		}
		else if (repairTarget)
			AttackTarget(obj,repairTarget,target,true,true);
		if (targetUpdate)
			targetUpdate--;
		if (actionUpdate)
			actionUpdate--;
		if (!targetId && !repairTargetId && samePosition >= 10 && (wanderPointGroup != -1 || (Get_Float_Parameter("PatrolRange") > 0 ? true : false)))
		{
			randomResetAmount = Commands->Get_Random_Int(0,4);
			samePosition = randomResetAmount;
			Vector3 wanderPos = Vector3();
			if (wanderPointGroup != -1 && Get_Random_Wander_Point(&wanderPos))
			{
				Commands->Action_Reset(obj,100);
				ActionParamsStruct params;
				params.Set_Movement(wanderPos,1.0f,2.5f,false);
				params.MovePathfind = true;
				Commands->Action_Goto(obj,params);
			}
			else
			{
				pos = centerLocation;
				Vector3 returnPos;
				for (int x = 0;x < 10;x++)
				{
					if (Get_Random_Pathfind_Spot(pos,Get_Float_Parameter("PatrolRange"),&returnPos))
					{
						Commands->Action_Reset(obj,100);
						ActionParamsStruct params;
						params.Set_Movement(returnPos,1.0f,2.5f,false);
						params.MovePathfind = true;
						Commands->Action_Goto(obj,params);
						break;
					}
					pos.Z = centerLocation.Z+Commands->Get_Random(-Get_Float_Parameter("PatrolRange"),Get_Float_Parameter("PatrolRange"));
				}
			}
		}
		if (JmgUtility::SimpleDistance(pos,lastPos) < 0.1)
			samePosition++;
		else
			samePosition = randomResetAmount;
		lastPos = pos;
		Commands->Start_Timer(obj,this,0.25f,1);
	}
	if (number == 5)
	{
		for (int x = 1;x < 128;x++)
			if (aiIgnorePlayers[x])
				aiIgnorePlayers[x]--;
		if (!targetId && !repairTargetId && Commands->Get_Random(0.0f,1.0f) <= 0.35f)
		{
			ActionParamsStruct params;
			Commands->Get_Action_Params(obj,params);
			Vector3 pos = Commands->Get_Position(obj);
			float rotation = Commands->Get_Random(0,9)*45;
			pos.X += cos(rotation)*100.0f;
			pos.Y += sin(rotation)*100.0f;
			params.Set_Look(pos,Commands->Get_Random(1.0f,5.0f));
			Commands->Action_Goto(obj,params);
		}
		Commands->Start_Timer(obj,this,1.0f,5);
	}
	if (number == 6)
	{
		Vector3 speed = Vector3();
		if (obj->As_SoldierGameObj())
			obj->As_SoldierGameObj()->Get_Velocity(speed);
		float repeatTime = Commands->Get_Random(5.0f,7.5f);
		if (!targetId && !repairTargetId && JmgUtility::SimpleDistance(Vector3(),speed) <= 0.0f)
		{
			ActionParamsStruct params;
			Commands->Get_Action_Params(obj,params);
			Vector3 pos = Commands->Get_Position(obj);
			float rotation = Commands->Get_Random(0,9)*45;
			pos.X += cos(rotation)*100.0f;
			pos.Y += sin(rotation)*100.0f;
			params.Set_Face_Location(pos,repeatTime);
			Commands->Action_Face_Location(obj,params);
		}
		Commands->Start_Timer(obj,this,repeatTime,6);
	}
}
void JMG_Utility_AI_Engineer::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!canFight)
		return;
	if (damage <= 0 || !damager || obj == damager || (targetId && targetUpdate) || targetId == Commands->Get_ID(damager) || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	if (aiIgnorePlayers[JmgUtility::JMG_Get_Player_ID(damager)])
		return;
	if (targetId && JmgUtility::SimpleDistance(Commands->Get_Position(damager),Commands->Get_Position(obj)) > targetDistance)
		return;
	targetId = Commands->Get_ID(damager);
	targetUpdate = 40;
}
void JMG_Utility_AI_Engineer::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason)
{
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{
		if (Commands->Get_Health(obj) <= 0)
			return;
		GameObject *target = Commands->Find_Object(targetId);
		int playerId = JmgUtility::JMG_Get_Player_ID(target);
		if (playerId && Commands->Get_Health(target))
			aiIgnorePlayers[playerId] = 30;
	}
}
void JMG_Utility_AI_Engineer::AttackTarget(GameObject *obj,GameObject *target,GameObject *secondaryTarget,bool repairTarget,bool useRepairGun)
{
	if (useRepairGun != repairGun)
	{
		actionUpdate = 0;
		repairGun = useRepairGun;
		Commands->Select_Weapon(obj,useRepairGun ? Get_Parameter("RepaiarGun_Preset") : Get_Parameter("Weapon_Preset"));
	}
	if (target != lastTarget || secondaryTarget != lastSecondaryTarget || repairTarget != lastRepairTarget || lastUseRepairGun != useRepairGun)
		actionUpdate = 0;
	if (actionUpdate)
		return;
	lastTarget = target;
	lastSecondaryTarget = secondaryTarget;
	lastRepairTarget = repairTarget;
	lastUseRepairGun = useRepairGun;
	actionUpdate = 15;
	ActionParamsStruct params;
	if (repairTarget)
		params.Set_Movement(target,1.0f,repairGun ? repairGunEffectiveRange*0.9f : weaponEffectiveRange*0.9f,false);
	else if (secondaryTarget)
		params.Set_Movement(secondaryTarget,1.0f,repairGun ? repairGunEffectiveRange*0.9f : weaponEffectiveRange*0.9f,false);
	else
		params.Set_Movement(moveLocation,1.0f,repairGun ? repairGunEffectiveRange*0.9f : weaponEffectiveRange*0.9f,false);
	params.MovePathfind = true;
	Commands->Action_Goto(obj,params);
	params.Set_Basic(this,100,100,AI_STATE_COMBAT);
	params.AttackCheckBlocked = false;
	if (repairTarget && !useRepairGun && secondaryTarget)
	{
		params.MoveObject = secondaryTarget;
		params.Set_Attack(secondaryTarget,repairGun ? repairGunRange : weaponRange,0.0f,true);
		Commands->Action_Attack(obj,params);
	}
	else if (target && (repairTarget && useRepairGun) || (!repairTarget && !useRepairGun))
	{
		params.MoveObject = target;
		params.Set_Attack(target,repairGun ? repairGunRange : weaponRange,0.0f,true);
		Commands->Action_Attack(obj,params);
	}
}
bool JMG_Utility_AI_Engineer::inRange(GameObject *obj)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (JmgUtility::SimpleFlatDistance(pos,centerLocation) < maxRange)
		return true;
	return false;
}
bool JMG_Utility_AI_Engineer::Valid_Repair_Target(GameObject *obj,GameObject *target,int playerType)
{
	if (!target || target == obj || Commands->Get_Player_Type(target) != playerType || Get_Hitpoints(target) >= Get_Max_Hitpoints(target) || Is_Script_Attached(target,"JMG_Utility_AI_Engineer_Ignore_Target"))
		return false;
	if (Get_Vector3_Parameter("MinHP[EngineerTarget|C4|Beacon]").X && Is_Script_Attached(target,"JMG_Utility_AI_Engineer_Repair_Target") && Get_Hitpoints(target) < Get_Max_Hitpoints(target)*Get_Vector3_Parameter("MinHP[EngineerTarget|C4|Beacon]").X)
		return true;
	if (Get_Vector3_Parameter("MinHP[Soldiers|Vehicles|Turrets]").X && target->As_SoldierGameObj() && Get_Hitpoints(target) < Get_Max_Hitpoints(target)*Get_Vector3_Parameter("MinHP[Soldiers|Vehicles|Turrets]").X)
		return true;
	if (Get_Vector3_Parameter("MinHP[Soldiers|Vehicles|Turrets]").Y && target->As_VehicleGameObj() && Get_Vehicle_Mode(target) != VEHICLE_TYPE_TURRET && Get_Hitpoints(target) < Get_Max_Hitpoints(target)*Get_Vector3_Parameter("MinHP[Soldiers|Vehicles|Turrets]").Y)
		return true;
	if (Get_Vector3_Parameter("MinHP[Soldiers|Vehicles|Turrets]").Z && target->As_VehicleGameObj() && Get_Vehicle_Mode(target) == VEHICLE_TYPE_TURRET && Get_Hitpoints(target) < Get_Max_Hitpoints(target)*Get_Vector3_Parameter("MinHP[Soldiers|Vehicles|Turrets]").Z)
		return true;
	return false;
}
bool JMG_Utility_AI_Engineer::Valid_Repair_Target_C4(GameObject *obj,GameObject *target,int playerType)
{
	if (!target || target == obj || Commands->Get_Player_Type(target) == playerType)
		return false;
	if (Get_Vector3_Parameter("MinHP[EngineerTarget|C4|Beacon]").Y < 1.0 && target->As_PhysicalGameObj() && target->As_PhysicalGameObj()->As_C4GameObj() && Get_Hitpoints(target) > Get_Max_Hitpoints(target)*Get_Vector3_Parameter("MinHP[EngineerTarget|C4|Beacon]").Y)
		if (Get_C4_Attached(target) == obj)
			return false;
		else
			return true;
	if (Get_Vector3_Parameter("MinHP[EngineerTarget|C4|Beacon]").Z < 1.0 && target->As_PhysicalGameObj() && target->As_PhysicalGameObj()->As_BeaconGameObj() && Get_Hitpoints(target) > Get_Max_Hitpoints(target)*Get_Vector3_Parameter("MinHP[EngineerTarget|C4|Beacon]").Z)
		return true;
	return false;
}
void JMG_Utility_Set_Object_Visibility_For_Player_On_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		Set_Object_Visibility_For_Player(Get_GameObj(Get_Int_Parameter("PlayerID")),Commands->Get_ID(obj),Get_Int_Parameter("Visible") ? true : false);
	}
}
void JMG_Utility_AI_Vehicle::Created(GameObject *obj)
{
	const char *weap = Get_Current_Weapon(obj);
	if (weap)
	{
		primary = DefineAmmo(Get_Weapon_Ammo_Definition(weap,true));
		secondary = DefineAmmo(Get_Weapon_Ammo_Definition(weap,true));
	}
	minDistanceSquared = Get_Float_Parameter("MinAttackRange")*Get_Float_Parameter("MinAttackRange");
	retreatTime = 0;
	lastHealth = 0;
	moving = false;
	attacking = false;
	reverseTime = 0;
	stuckCount = 0;
	drivingBackward = false;
	badDestAttempt = 0;
	doNotUsePathfind = 0;
	maxHuntRangeSquared = Get_Float_Parameter("MaxHuntRange")*Get_Float_Parameter("MaxHuntRange");
	lastWanderPointSpot = currentAction.position = retreatPos = homepos = Commands->Get_Position(obj);
	myteam = Commands->Get_Player_Type(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	if (obj->As_VehicleGameObj()) 
	{
		Commands->Innate_Enable(obj);
		if (Get_Int_Parameter("EnableTransitions")) 
			Commands->Enable_Vehicle_Transitions(obj,true);
		else 
			Commands->Enable_Vehicle_Transitions(obj,false);
		Commands->Enable_Engine(obj,true);
		Commands->Enable_Hibernation(obj,false);
	} 
	else 
	{
		Commands->Innate_Enable(obj);
		Commands->Enable_Enemy_Seen(obj,true);
		Commands->Enable_Hibernation(obj,false);
	}
	Commands->Start_Timer(obj,this,1.0,1);
	grabNextPointDistance = Get_Float_Parameter("NextWanderPointWhenLastReached")*Get_Float_Parameter("NextWanderPointWhenLastReached");
	if (Get_Int_Parameter("WanderingAIGroupID") != -1)
		if (grabNextPointDistance)
		{
			lastWanderPointSpot = Commands->Get_Position(obj);
			Commands->Start_Timer(obj,this,0.1f,9);
		}
		else
			Commands->Start_Timer(obj,this,Get_Float_Parameter("MinRandWander") == Get_Float_Parameter("MaxRandWander") ? Get_Float_Parameter("MaxRandWander") : Commands->Get_Random(Get_Float_Parameter("MinRandWander"),Get_Float_Parameter("MaxRandWander")),2);
	Commands->Start_Timer(obj,this,5.0,3);
	Commands->Start_Timer(obj,this,0.1f,8);
}
void JMG_Utility_AI_Vehicle::Custom(GameObject *obj,int message,int param,GameObject *sender) 
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		attacking = false;
		moving = false;
		int testTeam = Commands->Get_Player_Type(sender);
		if(testTeam != Commands->Get_Player_Type(obj))
			Commands->Start_Timer(obj,this,0.5,4);
	}
	if (message == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		Commands->Set_Player_Type(obj,myteam);
	}
}
void JMG_Utility_AI_Vehicle::Action_Complete(GameObject *obj,int action,ActionCompleteReason reason)
{
	if (reason == ACTION_COMPLETE_PATH_BAD_START)
	{
		bool refreshAction = false;
		if (!doNotUsePathfind)
			refreshAction = true;
		doNotUsePathfind = 3;
		if (refreshAction)
			Commands->Start_Timer(obj,this,0.1f,7);
	}
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{
		if (!badDestAttempt)
			Commands->Start_Timer(obj,this,0.25f,5);
		else
			Commands->Start_Timer(obj,this,0.25f,6);
	}
	if (action == 10)
		moving = false;
	Commands->Action_Reset(obj,action*1.0f);
	Commands->Action_Reset(obj,100*1.0f);
}
void JMG_Utility_AI_Vehicle::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0 || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	if (Is_Script_Attached(damager,"JMG_Utility_AI_Vehicle_Ignored"))
		return;
	if (!_stricmp(Get_Skin(damager),"Blamo") || !_stricmp(Get_Shield_Type(damager),"Blamo"))
		return;
	if(!attacking)
		RunAttack(obj,SetTarget(damager));
	else 
		RunAttack(obj,SelectTarget(obj,damager));
	lastSeenCount = Commands->Get_Random_Int(6,15);
}
void JMG_Utility_AI_Vehicle::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (Is_Script_Attached(seen,"JMG_Utility_AI_Vehicle_Ignored"))
		return;
	Vector3 enemyPos = Commands->Get_Position(seen);
	float dist = JmgUtility::SimpleDistance(Commands->Get_Position(obj),enemyPos);
	if (!_stricmp(Get_Skin(seen),"Blamo") || !_stricmp(Get_Shield_Type(seen),"Blamo"))
		return;
	if (Get_Float_Parameter("MaxHuntRange") > 0.0f && JmgUtility::SimpleDistance(lastWanderPointSpot,enemyPos) > maxHuntRangeSquared)
		return;
	if (Get_Float_Parameter("MinAttackRange") && dist < minDistanceSquared)
		return;
	if (seen->As_SmartGameObj() && seen->As_SmartGameObj()->Is_Stealthed())
		if (seen->As_SoldierGameObj() && dist > seen->As_SoldierGameObj()->Get_Stealth_Fade_Distance()*seen->As_SoldierGameObj()->Get_Stealth_Fade_Distance())
			return;
		else if (seen->As_VehicleGameObj() && dist > seen->As_VehicleGameObj()->Get_Stealth_Fade_Distance()*seen->As_VehicleGameObj()->Get_Stealth_Fade_Distance())
			return;	
		else if (dist > seen->As_SmartGameObj()->Get_Stealth_Fade_Distance()*seen->As_SmartGameObj()->Get_Stealth_Fade_Distance())
			return;
	if (Commands->Get_ID(seen) == currentAction.targetId)
		lastSeenCount = Commands->Get_Random_Int(3,16);
	if(!attacking)
		RunAttack(obj,SetTarget(seen));
	else
		RunAttack(obj,SelectTarget(obj,seen));
}
void JMG_Utility_AI_Vehicle::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1) 
	{
		StuckCheck(obj);
		if (lastSeenCount)
			lastSeenCount--;
		if (retreatTime)
			retreatTime--;
		if (doNotUsePathfind)
			doNotUsePathfind--;
		Commands->Start_Timer(obj,this,1.0,1);
	}
	if (number == 2)
	{
		Vector3 wanderPos = Vector3();
		if (GetRandomPosition(&wanderPos) && (!Get_Int_Parameter("FollowTarget") || !currentAction.targetId))
		{
			moving = true;
			lastWanderPointSpot = wanderPos;
			AttackMove(obj,Commands->Find_Object(currentAction.targetId),false,wanderPos,currentAction.useAmmo,0.0f,false,2.5f);
		}
		Commands->Start_Timer(obj,this,Get_Float_Parameter("MinRandWander") == Get_Float_Parameter("MaxRandWander") ? Get_Float_Parameter("MaxRandWander") : Commands->Get_Random(Get_Float_Parameter("MinRandWander"),Get_Float_Parameter("MaxRandWander")),2);
	}
	if (number == 3) 
	{
		if (retreatTime)
		{
			GameObject *target = Commands->Find_Object(currentAction.targetId);
			if(target)
			{
				AttackMove(obj,Commands->Find_Object(currentAction.targetId),false,currentAction.position,currentAction.useAmmo,0.0f,false,2.5f);
				Commands->Start_Timer(obj,this,1.0f,3);
			}
			return;
		}
		if(attacking)
		{
			attacking = false;
			if (!lastSeenCount && currentAction.targetId)
				currentAction.targetId = 0;
			GameObject *target = Commands->Find_Object(currentAction.targetId);
			if(target)
				RunAttack(obj,SetTarget(target));
			else 
			{
				currentAction.useAmmo = 1;
				currentAction.targetId = 0;
				if (Get_Int_Parameter("ReturnHome") && !retreatTime) 
					ReturnHome(obj);
				else
					AttackMove(obj,NULL,false,currentAction.position,currentAction.useAmmo,0.0f,false,2.5f);
			}
		} 
		else
			if (Get_Int_Parameter("ReturnHome") && !retreatTime)
				ReturnHome(obj);
			else
				AttackMove(obj,Commands->Find_Object(currentAction.targetId),false,currentAction.position,currentAction.useAmmo,0.0f,false,2.5f);
	}
	if (number == 4)
	{
		if (Get_Vehicle_Occupant_Count(obj))
			Force_Occupants_Exit(obj);
	}
	if (number == 5) 
	{
		badDestAttempt = 1;
		AttackMove(obj,Commands->Find_Object(currentAction.targetId),false,Commands->Get_Position(obj),currentAction.useAmmo,0.0f,false,2.5f);
	}
	if (number == 6) 
	{
		badDestAttempt = 0;
		AttackMove(obj,Commands->Find_Object(currentAction.targetId),false,homepos,currentAction.useAmmo,0.0f,false,2.5f);
	}
	if (number == 7) 
	{
		AttackMove(obj,Commands->Find_Object(currentAction.targetId),false,currentAction.position,currentAction.useAmmo,0.0f,true,2.5f);
	}
	if (number == 8) 
	{
		if (attacking || currentAction.targetId)
		{
			GameObject *target = Commands->Find_Object(currentAction.targetId);
			if ((attacking && !target) || (target && !Commands->Get_Health(target)))
			{
				lastSeenCount = 0;
				attacking = false;
				currentAction.targetId = 0;
				Vector3 wanderPos = Vector3();
				if (Get_Int_Parameter("WanderingAIGroupID") != -1 && GetRandomPosition(&wanderPos) && (!Get_Int_Parameter("FollowTarget") || !currentAction.targetId))
				{
					moving = true;
					lastWanderPointSpot = wanderPos;
					AttackMove(obj,Commands->Find_Object(currentAction.targetId),false,wanderPos,currentAction.useAmmo,0.0f,false,2.5f);
				}
				else
				{
					lastWanderPointSpot = homepos;
					AttackMove(obj,Commands->Find_Object(currentAction.targetId),false,homepos,currentAction.useAmmo,0.0f,false,2.5f);
				}
			}
		}
		Commands->Start_Timer(obj,this,0.1f,8);
	}
	if (number == 9)
	{
		if (!Get_Int_Parameter("FollowTarget") || !currentAction.targetId)
		{
			if (JmgUtility::SimpleDistance(Commands->Get_Position(obj),lastWanderPointSpot) <= grabNextPointDistance)
			{
				GetRandomPosition(&lastWanderPointSpot);
				moving = true;
			}
			AttackMove(obj,Commands->Find_Object(currentAction.targetId),false,lastWanderPointSpot,currentAction.useAmmo,0.0f,false,2.5f);
		}
		Commands->Start_Timer(obj,this,1.0f,9);
	}
}
void JMG_Utility_AI_Vehicle::ReturnHome(GameObject * obj)
{
	currentAction = JMGVehicleAction();
	lastHealth = 0;
	attacking = false;
	moving = true;
	Vector3 pos;
	pos = mypos = Commands->Get_Position(obj);
	if (retreatTime && Commands->Find_Object(lastAction.targetId) && JmgUtility::SimpleDistance(Commands->Get_Position(Commands->Find_Object(lastAction.targetId)),retreatPos) > minDistanceSquared) 
		AttackMove(obj,Commands->Find_Object(lastAction.targetId),false,retreatPos,lastAction.useAmmo,0.0f,true,2.5f);
	else if (GetRandomPositionOutsideOfRange(&pos))
		AttackMove(obj,Commands->Find_Object(lastAction.targetId),false,pos,lastAction.useAmmo,0.0f,false,2.5f);
	else
		AttackMove(obj,Commands->Find_Object(lastAction.targetId),false,homepos,lastAction.useAmmo,0.0f,false,2.5f);
	lastWanderPointSpot = homepos;
}
void JMG_Utility_AI_Vehicle::RunAttack(GameObject *obj,GameObject *target) 
{
	if(target) 	
	{
		if (retreatTime)
		{
			AttackMove(obj,Commands->Find_Object(currentAction.targetId),false,currentAction.position,currentAction.useAmmo,0.0f,false,2.5f);
			return;
		}
		int ammo = SelectAmmo(target);
		if (ammo == -1) 
		{
			attacking = false;
			useAmmo = 1;
			currentAction.targetId = 0;
			return;
		}
		if (ammo == 2) 
		{
			if (lastHealth == Commands->Get_Health(target))
				useAmmo = useAmmo == 1 ? 0 : 1;
			ammo = useAmmo;
		}
		useAmmo = ammo;
		lastHealth = Commands->Get_Health(target);
		attacking = true;
		Vector3 pos = Commands->Get_Position(target);
		mypos = Commands->Get_Position(obj);
		if (Get_Int_Parameter("FollowTarget")) 
		{
			moving = true;
			ActionParamsStruct params;
			AttackMove(obj,target,true,Vector3(),useAmmo,0.0f,false,Get_Float_Parameter("MaxRange"));
		} 
		else 
		{
			moving = false;
			ActionParamsStruct params;
			AttackMove(obj,target,false,currentAction.position,useAmmo,0.0f,false,Get_Float_Parameter("MaxRange"));
		}
		Commands->Start_Timer(obj,this,1.0f,3);
	}
	else 
	{
		moving = false;
		attacking = false;
		useAmmo = 1;
		AttackMove(obj,NULL,false,currentAction.position,useAmmo,0.0f,false,currentAction.arriveDistance);
		return;
	}
}
int JMG_Utility_AI_Vehicle::GetThreatRating(GameObject * obj) 
{
	if (obj == NULL)
		return 0;
	int rating=0;
	if(Commands->Is_A_Star(obj)) 
	{ 
		rating += 1;
		GameObject *vehicle = Get_Vehicle(obj);
		if (vehicle) 
			rating += 3; 
	}
	if (obj->As_SoldierGameObj()) 
		rating += 1; 
	if (obj->As_VehicleGameObj()) 
		rating += 3; 
	return rating;
}
GameObject * JMG_Utility_AI_Vehicle::GetAttackObject(GameObject * obj) 
{
	if (obj == NULL)
		return NULL;
	if(Commands->Is_A_Star(obj)) 
	{
		GameObject *vehicle = Get_Vehicle(obj); 
		if(vehicle)
			return vehicle;
		else 
			return obj;
	}
	return obj;
}
GameObject *JMG_Utility_AI_Vehicle::SelectTarget(GameObject *obj,GameObject *target)
{
	GameObject *old = currentAction.targetId ? Commands->Find_Object(currentAction.targetId) : NULL;
	if (!target || !Commands->Get_ID(target))
		return GetAttackObject(old);
	if (!old) 
	{
		currentAction.targetId = Commands->Get_ID(target);
		return GetAttackObject(target);
	}
	int trOld = GetThreatRating(old);
	int trNew = GetThreatRating(target);
	if (trOld > trNew)
		return GetAttackObject(old);
	if (trNew > trOld) 
	{
		currentAction.targetId = Commands->Get_ID(target);
		return GetAttackObject(target);
	}
	GameObject * closest = GetClosest(obj,target,old);
	currentAction.targetId = Commands->Get_ID(closest);
	return GetAttackObject(closest);
}
GameObject *JMG_Utility_AI_Vehicle::SetTarget(GameObject *target)
{
	currentAction.targetId = Commands->Get_ID(target);
	if(!attacking)
		return GetAttackObject(target);
	return 0;
}
GameObject *JMG_Utility_AI_Vehicle::GetClosest(GameObject *obj,GameObject *new_target,GameObject *old_target)
{
	Vector3 obj_pos = Commands->Get_Position(obj);
	Vector3 target1_pos = Commands->Get_Position(new_target);
	Vector3 target2_pos = Commands->Get_Position(old_target);
	float dist1 = JmgUtility::SimpleDistance(obj_pos,target1_pos);
	float dist2 = JmgUtility::SimpleDistance(obj_pos,target2_pos);
	if(dist1 < dist2)
		return new_target;
	else
		return old_target;
}
int JMG_Utility_AI_Vehicle::SelectAmmo(GameObject *target)
{
	if(target)
	{
		if(target->As_SoldierGameObj())
			return Get_Int_Parameter("vsSoldier");
		if(target->As_VehicleGameObj()) 
		{
			if(Is_VTOL(target))
				return Get_Int_Parameter("vsAircraft");
			return Get_Int_Parameter("vsVehicle");
		}
	}
	return -1;
}
void JMG_Utility_AI_Vehicle::StuckCheck(GameObject *obj)
{
	Vector3 current = Commands->Get_Position(obj);
	if (reverseTime)
	{
		reverseTime--;
		stuckCount = 0;
		mypos = current;
		drivingBackward = false;
		AttackMove(obj,Commands->Find_Object(lastAction.targetId),lastAction.following,lastAction.position,lastAction.useAmmo,0.0f,true,0.0f);
		return;
	}
	if (!moving)
	{
		drivingBackward = false;
		return;
	}
	if(JmgUtility::SimpleDistance(mypos,current) < 1.0f)
	{
		if(stuckCount > 2)
		{
			reverseTime = Commands->Get_Random_Int(2,6);
			stuckCount = 0;
			drivingBackward = true;
			AttackMove(obj,Commands->Find_Object(lastAction.targetId),lastAction.following,lastAction.position,lastAction.useAmmo,0.0f,true,currentAction.arriveDistance);
		}
		stuckCount++;
	}
	else
	{
		stuckCount = 0;
		mypos = current;
	}
}
void JMG_Utility_AI_Vehicle::AttackMove(GameObject *obj,GameObject *target,bool followTarget,Vector3 targetLocation,int fireMode,float weaponError,bool forceUpdate,float arriveDistance)
{
	mypos = Commands->Get_Position(obj);
	float dist = 9999.9f;
	if (target)
		if (followTarget)
			dist = JmgUtility::SimpleDistance(Commands->Get_Position(target),mypos);
		else
			dist = JmgUtility::SimpleDistance(currentAction.position,mypos);
	if (minDistanceSquared && dist > minDistanceSquared+1089.0f)
		retreatPos = mypos;
	if (minDistanceSquared && dist < minDistanceSquared && !(Get_Int_Parameter("SquishInfantry") && target->As_SoldierGameObj()))
	{
		if (retreatTime)
			return;
		retreatTime = 5;
		lastSeenCount = 7;
		ReturnHome(obj);
		return;
	}
	currentAction.useAmmo = fireMode;
	currentAction.following = followTarget;
	currentAction.backward = drivingBackward;
	currentAction.arriveDistance = arriveDistance;
	if (!followTarget)
		currentAction.position = targetLocation;
	if (lastAction == currentAction && !forceUpdate)
		return;
	lastAction = currentAction;
	bool canSquish = false;
	bool isMoving = moving;
	Commands->Action_Reset(obj,10.0f);
	moving = isMoving;
	ActionParamsStruct params;
	if (target)
	{
		JMGVehicleAmmo ammo = (fireMode ? primary : secondary);
		weaponError = 0.0f;
		if (Get_Float_Parameter("WeaponError") != -1.00f)
			weaponError = Get_Float_Parameter("WeaponError");
		else if (ammo.allowError)
			weaponError = JmgUtility::SimpleDistance(Commands->Get_Position(target),mypos)/ammo.speed*5.0f;
		params.Set_Attack(target,ammo.range,weaponError,fireMode ? true : false);
		if (Get_Int_Parameter("ForceFire"))	
		{
			params.AttackCheckBlocked = false;
			params.AttackForceFire = true;
		}
		if (Get_Int_Parameter("SquishInfantry") && target->As_SoldierGameObj() && !Is_Unsquishable(target))
			canSquish = true;
	}
	if (followTarget)
		if (Is_VTOL(obj) && Get_Float_Parameter("VTOLHover") && !canSquish)
		{
			if (target)
			{
				targetLocation = Commands->Get_Position(target);
				targetLocation.Z += Get_Float_Parameter("VTOLHover");
				params.AttackFaceTarget = false;
			}
			params.Set_Movement(targetLocation,JmgUtility::MathClamp(dist,1.0f,100.0f),arriveDistance,false);
			params.MoveFollow = false;
		}
		else
			params.Set_Movement(target,JmgUtility::MathClamp(dist,1.0f,100.0f),canSquish ? 1.0f : arriveDistance,false);
	else
	{
		if (Is_VTOL(obj) && Get_Float_Parameter("VTOLHover") && !canSquish)
			targetLocation.Z += Get_Float_Parameter("VTOLHover");
		params.Set_Movement(targetLocation,JmgUtility::MathClamp(dist,1.0f,100.0f),canSquish ? 1.0f : arriveDistance,false);
	}
	params.MoveBackup = currentAction.backward;
	if (Is_VTOL(obj))
		params.MovePathfind = false;
	else
	{
		if (Get_Int_Parameter("DisableEngineOnArrival"))
			params.ShutdownEngineOnArrival = true;
		params.MovePathfind = !doNotUsePathfind && Get_Int_Parameter("UsePathfind") ? true : false;
	}
	if (!target)
		params.Set_Basic(this,10,10);
	Commands->Action_Goto(obj,params);
	if (target)
	{
		params.Set_Basic(this,10,10);
		Commands->Action_Attack(obj,params);
	}
}
JMG_Utility_AI_Vehicle::JMGVehicleAmmo JMG_Utility_AI_Vehicle::DefineAmmo(const AmmoDefinitionClass *ammo)
{
	JMGVehicleAmmo theammo;
	if (ammo)
	{
		theammo.range = ammo->Range;
		if (ammo->Velocity >= 400.0f || (ammo->IsTracking && ammo->TurnRate))
			theammo.allowError = false;
		else
			theammo.allowError = true;
		theammo.speed = ammo->Velocity*ammo->Velocity;
	}
	return theammo;
}
DynamicClockSystem DynamicClockSystemControl = DynamicClockSystem();
void JMG_Utility_Dynamic_Clock_Control::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,1.0,1);
}
void JMG_Utility_Dynamic_Clock_Control::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1) 
	{
		if (lastMinute != getMinute())
		{
			lastMinute = getMinute();
			DynamicClockSystemControl.updateClocks();
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}
void JMG_Utility_Dynamic_Clock_Control::Destroyed(GameObject *obj)
{
	DynamicClockSystemControl.emptyList();
}
int JMG_Utility_Dynamic_Clock_Control::getMinute()
{
	time_t rawtime;
	struct tm * ptm;
	time (&rawtime);
	ptm = gmtime (&rawtime);
	return ptm->tm_min;
}
void JMG_Utility_Dynamic_Clock_Object::Created(GameObject *obj)
{
	DynamicClockSystemControl += obj;
}
void JMG_Utility_Dynamic_Clock_Object::Destroyed(GameObject *obj)
{
	DynamicClockSystemControl -= obj;
}
void JMG_Utility_Change_Screen_Color_While_In_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (Commands->Is_A_Star(enterer))
	{
		Set_Screen_Fade_Color_Player(enterer,Get_Vector3_Parameter("Color[Red|Green|Blue]").X,Get_Vector3_Parameter("Color[Red|Green|Blue]").Y,Get_Vector3_Parameter("Color[Red|Green|Blue]").Z,Get_Float_Parameter("EnterFadeInTime"));
		Set_Screen_Fade_Opacity_Player(enterer,Get_Float_Parameter("Opacity"),Get_Float_Parameter("EnterFadeInTime"));
		Attach_Script_Once(enterer,"JMG_Utility_Reset_Screen_Color_When_Destroyed","");
	}
}
void JMG_Utility_Change_Screen_Color_While_In_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	if (Commands->Is_A_Star(exiter))
	{
		Set_Screen_Fade_Color_Player(exiter,0.0f,0.0f,0.0f,Get_Float_Parameter("ExitFadeOutTime"));
		Set_Screen_Fade_Opacity_Player(exiter,0.0f,Get_Float_Parameter("ExitFadeOutTime"));
		if (Is_Script_Attached(exiter,"JMG_Utility_Reset_Screen_Color_When_Destroyed"))
			Remove_Script(exiter,"JMG_Utility_Reset_Screen_Color_When_Destroyed");
	}
}
void JMG_Utility_Reset_Screen_Color_When_Destroyed::Destroyed(GameObject *obj)
{
	if (!Commands->Is_A_Star(obj))
		return;
	Set_Screen_Fade_Color_Player(obj,0.0f,0.0f,0.0f,0.0f);
	Set_Screen_Fade_Opacity_Player(obj,0.0f,0.0f);
}
char JMG_Utility_Play_Music_On_Join_Controller::musicFileName[256] = {"null.mp3"};
bool JMG_Utility_Play_Music_On_Join_Controller::controllerPlaced = false;
void JMG_Utility_Play_Music_On_Join_Controller::Created(GameObject *obj)
{
	if (controllerPlaced)
	{
		Console_Input("msg JMG_Utility_Play_Music_On_Join_Controller ERROR: Only place one controller on each map!");
		return;
	}
	controllerPlaced = true;
	sprintf(JMG_Utility_Play_Music_On_Join_Controller::musicFileName,"%s",Get_Parameter("Music"));
	for (int x = 1;x < 128;x++)
		playingMusic[x] = false;
	Commands->Start_Timer(obj,this,1.0,1);
}
void JMG_Utility_Play_Music_On_Join_Controller::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1) 
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (player && !playingMusic[x])
			{
				playingMusic[x] = true;
				Set_Background_Music_Player(player,JMG_Utility_Play_Music_On_Join_Controller::musicFileName);
			}
			if (!player && playingMusic[x])
				playingMusic[x] = false;
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}
void JMG_Utility_Play_Music_On_Join_Controller::Destroyed(GameObject *obj)
{
	controllerPlaced = false;
}
void JMG_Utility_Play_Music_On_Join_Controller::Set_Music(const char *musicFilName,int fadeOut,int fadeIn)
{
	sprintf(JMG_Utility_Play_Music_On_Join_Controller::musicFileName,"%s",musicFilName);
	if (fadeOut || fadeIn)
		Commands->Fade_Background_Music(musicFilName,fadeOut,fadeIn);
	else
		Commands->Set_Background_Music(musicFilName);
}
void JMG_Utility_Play_Music_On_Join_Change_Music::Created(GameObject *obj)
{
	if (!JMG_Utility_Play_Music_On_Join_Controller::controllerPlaced)
	{
		Console_Input("msg JMG_Utility_Play_Music_On_Join_Change_Music ERROR: You need to place JMG_Utility_Play_Music_On_Join_Controller on an object in order to use this script!");
		return;
	}
	JMG_Utility_Play_Music_On_Join_Controller::Set_Music(Get_Parameter("Music"),Get_Int_Parameter("FadeOut"),Get_Int_Parameter("FadeIn"));
	Destroy_Script();
}
void JMG_Utility_Toggle_Door::Created(GameObject *obj)
{
	preDamagedFrame = 0.0f;
	sprintf(originalModel,"%s",Get_Model(obj));
	enabled = true;
	open = false;
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
	if (_stricmp(Get_Parameter("SwitchAnimation"),"null"))
		Commands->Set_Animation(obj,Get_Parameter("SwitchAnimation"),false,0,0.0f,0.0f,false);
}
void JMG_Utility_Toggle_Door::Poked(GameObject *obj,GameObject *poker)
{
	GameObject *theDoor = Commands->Find_Object(Get_Int_Parameter("DoorID"));
	if (poker != obj)
	{
		if (!theDoor)
		{
			if (_stricmp(Get_Parameter("NoDoorMessage"),"null"))
					JmgUtility::DisplayChatMessage(poker,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").X,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Y,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Z,Get_Parameter("NoDoorMessage"));
			if (_stricmp(Get_Parameter("SwitchBlockedSound"),"null"))
					Commands->Create_Sound(Get_Parameter("SwitchBlockedSound"),Commands->Get_Position(obj),obj);
			SendCustom(obj,6);
			return;
		}
		if (!enabled)
		{
			if (_stricmp(Get_Parameter("SwitchDamagedPokeMessage"),"null"))
					JmgUtility::DisplayChatMessage(poker,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").X,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Y,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Z,Get_Parameter("SwitchDamagedPokeMessage"));
			if (_stricmp(Get_Parameter("SwitchDamagedPokeSound"),"null"))
					Commands->Create_Sound(Get_Parameter("SwitchDamagedPokeSound"),Commands->Get_Position(obj),obj);
			SendCustom(obj,5);
			return;
		}
		if ((Get_Int_Parameter("Key") && !Commands->Has_Key(poker,Get_Int_Parameter("Key"))))
		{
			if (_stricmp(Get_Parameter("WrongKeyMessage"),"null"))
				JmgUtility::DisplayChatMessage(poker,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").X,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Y,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Z,Get_Parameter("WrongKeyMessage"));
			if (_stricmp(Get_Parameter("WrongKeySound"),"null"))
				Commands->Create_Sound(Get_Parameter("WrongKeySound"),Commands->Get_Position(obj),obj);
			SendCustom(obj,4);
			return;
		}
		if (CheckPlayerType(poker,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(poker) == -4)
		{
			if (_stricmp(Get_Parameter("WrongPlayerTypeMessage"),"null"))
				JmgUtility::DisplayChatMessage(poker,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").X,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Y,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Z,Get_Parameter("WrongPlayerTypeMessage"));
			if (_stricmp(Get_Parameter("WrongPlayerTypeSound"),"null"))
				Commands->Create_Sound(Get_Parameter("WrongPlayerTypeSound"),Commands->Get_Position(obj),obj);
			SendCustom(obj,3);
			return;
		}
	}
	if (!enabled)
		return;
	if (!open)
	{
		open = true;
		if (Get_Animation_Frame(theDoor) == 0.0f)
		{
			Commands->Set_Animation(theDoor,Get_Parameter("Animation"),false,0,0.0f,Get_Float_Parameter("LastFrame"),false);
			if (_stricmp(Get_Parameter("GotoLastFrameMessage"),"null"))
				JmgUtility::DisplayChatMessage(poker,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").X,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Y,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Z,Get_Parameter("GotoLastFrameMessage"));
			if (_stricmp(Get_Parameter("SwitchAnimation"),"null"))
				Commands->Set_Animation(obj,Get_Parameter("SwitchAnimation"),false,0,0.0f,Get_Float_Parameter("SwitchLastFrame"),false);
			if (_stricmp(Get_Parameter("DoorSound"),"null"))
				Commands->Create_Sound(Get_Parameter("DoorSound"),Commands->Get_Position(theDoor),theDoor);
			if (_stricmp(Get_Parameter("SwitchSound"),"null"))
				Commands->Create_Sound(Get_Parameter("SwitchSound"),Commands->Get_Position(obj),obj);
			SendCustom(obj,1);

		}
		else
		{
			if (_stricmp(Get_Parameter("ActionBlockedMessage"),"null"))
				JmgUtility::DisplayChatMessage(poker,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").X,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Y,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Z,Get_Parameter("ActionBlockedMessage"));
			if (_stricmp(Get_Parameter("SwitchBlockedSound"),"null"))
				Commands->Create_Sound(Get_Parameter("SwitchBlockedSound"),Commands->Get_Position(obj),obj);
			SendCustom(obj,2);
		}

	}
	else
	{
		open = false;
		if (Get_Animation_Frame(theDoor) == Get_Float_Parameter("LastFrame"))
		{
			Commands->Set_Animation(theDoor,Get_Parameter("Animation"),false,0,Get_Float_Parameter("LastFrame"),0.0f,false);
			if (_stricmp(Get_Parameter("GotoFirstFrameMessage"),"null"))
				JmgUtility::DisplayChatMessage(poker,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").X,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Y,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Z,Get_Parameter("GotoFirstFrameMessage"));
			if (_stricmp(Get_Parameter("SwitchAnimation"),"null"))
				Commands->Set_Animation(obj,Get_Parameter("SwitchAnimation"),false,0,Get_Float_Parameter("SwitchLastFrame"),0.0f,false);
			if (_stricmp(Get_Parameter("DoorSound"),"null"))
				Commands->Create_Sound(Get_Parameter("DoorSound"),Commands->Get_Position(theDoor),theDoor);
			if (_stricmp(Get_Parameter("SwitchSound"),"null"))
				Commands->Create_Sound(Get_Parameter("SwitchSound"),Commands->Get_Position(obj),obj);
			SendCustom(obj,0);
		}
		else
		{
			if (_stricmp(Get_Parameter("ActionBlockedMessage"),"null"))
				JmgUtility::DisplayChatMessage(poker,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").X,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Y,(int)Get_Vector3_Parameter("TextColor[Red|Green|Blue]").Z,Get_Parameter("ActionBlockedMessage"));
			if (_stricmp(Get_Parameter("SwitchBlockedSound"),"null"))
				Commands->Create_Sound(Get_Parameter("SwitchBlockedSound"),Commands->Get_Position(obj),obj);
			SendCustom(obj,2);
		}
	}
}
void JMG_Utility_Toggle_Door::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damage || !Get_Int_Parameter("DisableSwitchWhenNoShield"))
		return;
	if (enabled && Commands->Get_Shield_Strength(obj) <= 0)
	{
		enabled = false;
		if (_stricmp(Get_Parameter("SwitchAnimation"),"null"))
			preDamagedFrame = Get_Animation_Frame(obj);
		if (_stricmp(Get_Parameter("SwitchDisabledModel"),"null"))
			Commands->Set_Model(obj,Get_Parameter("SwitchDisabledModel"));
		char animation[32];
		sprintf(animation,"%s.%s",Get_Parameter("SwitchDisabledModel"),Get_Parameter("SwitchDisabledModel"));
		Commands->Set_Animation(obj,animation,false,0,0,0,false);
		
	}
	if (!enabled && Commands->Get_Shield_Strength(obj) >= Commands->Get_Max_Shield_Strength(obj))
	{
		enabled = true;
		if (_stricmp(Get_Parameter("SwitchDisabledModel"),"null"))
			Commands->Set_Model(obj,originalModel);
		if (_stricmp(Get_Parameter("SwitchAnimation"),"null"))
			Commands->Set_Animation(obj,Get_Parameter("SwitchAnimation"),false,0,preDamagedFrame,preDamagedFrame,false);
	}
}
void JMG_Utility_Toggle_Door::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("DoorControlCustom") && Get_Int_Parameter("DoorControlCustom"))
	{
		if (param == 1 && !open)
			Poked(obj,obj);
		if (param == 0 && open)
			Poked(obj,obj);
	}
}
void JMG_Utility_Toggle_Door::SendCustom(GameObject *obj,int param)
{
	if (!Get_Int_Parameter("SendCustom") || !Get_Int_Parameter("SendCustomId"))
		return;
	GameObject *sendObj = Commands->Find_Object(Get_Int_Parameter("SendCustomId"));
	if (sendObj)
		Commands->Send_Custom_Event(obj,sendObj,Get_Int_Parameter("SendCustom"),param,0);
}
void JMG_Utility_Set_Animation_Frame_On_Creation::Created(GameObject *obj)
{
	Commands->Set_Animation(obj,Get_Parameter("Animation"),false,0,Get_Float_Parameter("Frame"),Get_Float_Parameter("Frame"),false);
}
void JMG_Utility_Set_Model_On_Damage_Percent_3_States::Created(GameObject *obj)
{
	healthThresholds[0] = 0.0f;
	healthThresholds[1] = Commands->Get_Max_Shield_Strength(obj)*0.5f;
	healthThresholds[2] = Commands->Get_Max_Shield_Strength(obj);
	damageState =  2;
	if (Commands->Get_Shield_Strength(obj) != Commands->Get_Max_Shield_Strength(obj))
	{
		damageState =  1;
		Commands->Set_Model(obj,Get_Parameter("HalfShieldModel"));
		SetModel(obj);
	}
}
void JMG_Utility_Set_Model_On_Damage_Percent_3_States::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damage)
		return;
	SetModel(obj);
}
void JMG_Utility_Set_Model_On_Damage_Percent_3_States::SetModel(GameObject *obj)
{
	if (Commands->Get_Shield_Strength(obj) >= healthThresholds[2] && damageState != 2)
	{
		damageState = 2;
		Commands->Set_Model(obj,Get_Parameter("FullShieldModel"));
	}
	if (Commands->Get_Shield_Strength(obj) <= healthThresholds[0] && damageState != 0)
	{
		damageState = 0;
		Commands->Set_Model(obj,Get_Parameter("NoShieldModel"));
	}
	if ((!damageState && Commands->Get_Shield_Strength(obj) >= healthThresholds[1]) || (damageState == 2 && Commands->Get_Shield_Strength(obj) <= healthThresholds[1]))
	{
		damageState = 1;
		Commands->Set_Model(obj,Get_Parameter("HalfShieldModel"));
	}
}
void JMG_Utility_Set_Model_On_Damage_Percent_4_States::Created(GameObject *obj)
{
	healthThresholds[0] = 0.0f;
	healthThresholds[1] = Commands->Get_Max_Shield_Strength(obj)*0.33f;
	healthThresholds[2] = Commands->Get_Max_Shield_Strength(obj)*0.66f;
	healthThresholds[3] = Commands->Get_Max_Shield_Strength(obj);
	damageState =  3;
	if (Commands->Get_Shield_Strength(obj) != Commands->Get_Max_Shield_Strength(obj))
	{
		damageState =  1;
		Commands->Set_Model(obj,Get_Parameter("HalfShieldModel"));
		SetModel(obj);
	}
}
void JMG_Utility_Set_Model_On_Damage_Percent_4_States::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damage)
		return;
	SetModel(obj);
}
void JMG_Utility_Set_Model_On_Damage_Percent_4_States::SetModel(GameObject *obj)
{
	if (Commands->Get_Shield_Strength(obj) >= healthThresholds[3] && damageState != 3)
	{
		damageState = 3;
		Commands->Set_Model(obj,Get_Parameter("FullShieldModel"));
	}
	if (Commands->Get_Shield_Strength(obj) <= healthThresholds[0] && damageState != 0)
	{
		damageState = 0;
		Commands->Set_Model(obj,Get_Parameter("NoShieldModel"));
	}
	if ((!damageState && Commands->Get_Shield_Strength(obj) >= healthThresholds[1]) || (damageState == 2 && Commands->Get_Shield_Strength(obj) <= healthThresholds[1]))
	{
		damageState = 1;
		Commands->Set_Model(obj,Get_Parameter("ThirdShieldModel"));
	}
	if ((damageState == 1 && Commands->Get_Shield_Strength(obj) >= healthThresholds[2]) || (damageState == 3 && Commands->Get_Shield_Strength(obj) <= healthThresholds[2]))
	{
		damageState = 2;
		Commands->Set_Model(obj,Get_Parameter("TwoThirdsShieldModel"));
	}
}
int JMG_Utility_PCT::pctInaccessible[128] = {0};
void JMG_Utility_PCT::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void JMG_Utility_PCT::Poked(GameObject *obj,GameObject *poker)
{
	if (Commands->Get_Player_Type(poker) != Commands->Get_Player_Type(obj))
	{
		char displayMsg[220];
		sprintf(displayMsg,"pamsg %d %s",JmgUtility::JMG_Get_Player_ID(poker),Get_Translated_String(8780));
		Console_Input(displayMsg);
		return;
	}
	if (JMG_Utility_PCT::pctInaccessible[JmgUtility::JMG_Get_Player_ID(poker)])
	{
		char displayMsg[220];
		sprintf(displayMsg,"pamsg %d You cannot access this terminal from your current location.",JmgUtility::JMG_Get_Player_ID(poker));
		Console_Input(displayMsg);
		return;
	}
	if (!Commands->Get_Player_Type(poker))
		Display_NOD_Player_Terminal_Player(poker);
	else
		Display_GDI_Player_Terminal_Player(poker);
}
void JMG_Utility_PCT_Inaccessible_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (Commands->Is_A_Star(enterer))
	{
		JMG_Utility_PCT::pctInaccessible[JmgUtility::JMG_Get_Player_ID(enterer)]++;
		Attach_Script_Once(enterer,"JMG_Utility_PCT_Inaccessible_Zone_Attach","");
	}
}
void JMG_Utility_PCT_Inaccessible_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	if (Commands->Is_A_Star(exiter) && JMG_Utility_PCT::pctInaccessible[JmgUtility::JMG_Get_Player_ID(exiter)])
		JMG_Utility_PCT::pctInaccessible[JmgUtility::JMG_Get_Player_ID(exiter)]--;
}
void JMG_Utility_PCT_Inaccessible_Zone_Attach::Destroyed(GameObject *obj)
{
		JMG_Utility_PCT::pctInaccessible[JmgUtility::JMG_Get_Player_ID(obj)] = 0;
}
void JMG_Utility_Display_HUD_Info_Text_All_Players_Custom_Replace_String::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
		JmgUtility::SetHUDHelpText(Get_Int_Parameter("StringId"),Get_Parameter("ReplaceString"),Get_Vector3_Parameter("ColorRGB"));
	if (!Get_Int_Parameter("Repeatable"))
		Destroy_Script();
}
void JMG_Utility_Display_Text_Message_To_All_Players_On_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		JmgUtility::MessageAllPlayers((int)Get_Vector3_Parameter("ColorRGB").X,(int)Get_Vector3_Parameter("ColorRGB").Y,(int)Get_Vector3_Parameter("ColorRGB").Z,Get_Parameter("Message"));
		if (!Get_Int_Parameter("Repeatable"))
			Destroy_Script();
	}
}
void JMG_Utility_Set_Team_On_Create::Created(GameObject *obj)
{
	if (!Get_Float_Parameter("Delay"))
	{
		Commands->Set_Player_Type(obj,Get_Int_Parameter("PlayerType"));
		Destroy_Script();
		return;
	}
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
}
void JMG_Utility_Set_Team_On_Create::Timer_Expired(GameObject *obj,int number)
{
	if (1 == number)
	{
		Commands->Set_Player_Type(obj,Get_Int_Parameter("PlayerType"));
		Destroy_Script();
	}
}
void JMG_Utility_AI_Aggressive_Melee::Created(GameObject *obj)
{
	LastSeen = 0;
	currentTargetID = 0;
	secondaryTargetId = 0;
	lastSeenSecondary = 0;
	huntorattack = 0;
	waitcount = 0;
	speed = 2.0f;
	stuckTime = 0;
	lastPos = homelocation = Commands->Get_Position(obj);
	noPathfindRange = Get_Float_Parameter("NoPathfindDistance")*Get_Float_Parameter("NoPathfindDistance");
	minVisibilityTime = Get_Int_Parameter("MinLooseVisibilityTime")*10;
	maxVisibilityTime = Get_Int_Parameter("MaxLooseVisibilityTime")*10;
	maxHuntDistance = Get_Float_Parameter("MaxHuntDistance")*Get_Float_Parameter("MaxHuntDistance");
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,0.1f,92562343);
}
void JMG_Utility_AI_Aggressive_Melee::Enemy_Seen(GameObject *obj,GameObject *seen)	
{
	int SeenID = Commands->Get_ID(seen);
	if (!currentTargetID || !LastSeen)
	{
		LastSeen = minVisibilityTime == maxVisibilityTime ? maxVisibilityTime : Commands->Get_Random_Int(minVisibilityTime,maxVisibilityTime);
		huntorattack = 0;
		currentTargetID = SeenID;
	}
	else if (currentTargetID == SeenID)
		LastSeen = minVisibilityTime == maxVisibilityTime ? maxVisibilityTime : Commands->Get_Random_Int(minVisibilityTime,maxVisibilityTime);
	else if (!secondaryTargetId || !lastSeenSecondary)
	{
		lastSeenSecondary = maxVisibilityTime;
		secondaryTargetId = SeenID;
	}
	else if (secondaryTargetId == SeenID)
		lastSeenSecondary = maxVisibilityTime;
}
void JMG_Utility_AI_Aggressive_Melee::Timer_Expired(GameObject *obj,int number)
{
	if (92562343 == number)
	{
		if (LastSeen)
		{
			LastSeen--;
			if (!LastSeen)
				currentTargetID = 0;
		}
		if (lastSeenSecondary)
		{
			lastSeenSecondary--;
			if (!lastSeenSecondary)
				secondaryTargetId = 0;
		}
		if (currentTargetID)
		{
			GameObject *target = Commands->Find_Object(currentTargetID);
			GameObject *target2 = Commands->Find_Object(secondaryTargetId);
			if (!target2 || Commands->Get_Health(target2) == 0.0f)
				secondaryTargetId = 0;
			if (!target || Commands->Get_Health(target) == 0.0f)
			{
				if (target2)
				{
					currentTargetID = secondaryTargetId;
					LastSeen = maxVisibilityTime;
				}
				else
				{
					waitcount = 0;
					currentTargetID = 0;
					Commands->Action_Reset(obj,10);
					ActionParamsStruct params;
					params.Set_Basic(this,999,10);
					params.Set_Movement(homelocation,Get_Float_Parameter("ReturnHomeSpeed"),Get_Float_Parameter("ReturnHomeArriveDistance"),false);
					Commands->Action_Goto(obj,params);
				}
			}
			else if (Get_Float_Parameter("MaxHuntDistance") && JmgUtility::SimpleDistance(homelocation,Commands->Get_Position(target)) > maxHuntDistance)
			{
				currentTargetID = 0;
				waitcount = 0;
				if (huntorattack == 1)
					Commands->Action_Reset(obj,10);
			}
			else
			{
				waitcount++;
				if (waitcount > 2)
				{
					waitcount = 0;
					ActionParamsStruct params;
					params.Set_Movement(target,Get_Float_Parameter("ChaseSpeed"),Get_Float_Parameter("AttackArriveDistance"),false);
					params.MovePathfind = JmgUtility::SimpleDistance(Commands->Get_Position(target),Commands->Get_Position(obj)) <= noPathfindRange ? false : true;
					Commands->Action_Goto(obj,params);
					params.Set_Basic(this,999,10);
					params.Set_Attack(target,Get_Float_Parameter("StartAttackDistance"),1,true);
					Commands->Action_Attack(obj,params);
				}
			}
		}
		Commands->Start_Timer(obj,this,0.1f,92562343);
	}
}
void JMG_Utility_AI_Aggressive_Melee::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damager || Commands->Get_Player_Type(damager) != 1 || currentTargetID || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	if (!chooseTarget(obj,damager,&currentTargetID,&LastSeen))
		chooseTarget(obj,damager,&secondaryTargetId,&lastSeenSecondary);
	else
	{
		huntorattack = 0;
		waitcount = 0;
	}
}
bool JMG_Utility_AI_Aggressive_Melee::chooseTarget(GameObject *obj,GameObject *damager,int *compareId,int *seenTimer)
{
	GameObject *target = Commands->Find_Object(*compareId);
	if (target)
	{
		float targetDistance = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(target));
		float damagerDistance = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(damager));
		if (targetDistance < damagerDistance)
			return false;
	}
	*seenTimer = minVisibilityTime == maxVisibilityTime ? maxVisibilityTime : Commands->Get_Random_Int(minVisibilityTime,maxVisibilityTime);
	*compareId = Commands->Get_ID(damager);
	return true;
}
void JMG_Utility_Infantry_Placed_Buildable_Object::Created(GameObject *obj)
{
	canRegen = false;
	previewObjectId = 0;
	reloadTime = 0;
	if (!Has_Weapon(obj,Get_Parameter("WeaponPreset")) && Get_Int_Parameter("GrantWeapon"))
		Grant_Weapon(obj,Get_Parameter("WeaponPreset"),true,1,0);
	Commands->Start_Timer(obj,this,0.25f,1);
	Commands->Start_Timer(obj,this,1.0f,2);
}
void JMG_Utility_Infantry_Placed_Buildable_Object::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!Commands->Get_Health(obj))
			return;
		const char *weap = Get_Current_Weapon(obj);
		if (weap && !_stricmp(weap,Get_Parameter("WeaponPreset")) && !Get_Vehicle(obj) && !reloadTime)
		{
			if (!previewObjectId)
			{
				GameObject *preview = Commands->Create_Object(Get_Parameter("PreviewPreset"),Commands->Get_Position(obj));
				previewObjectId = Commands->Get_ID(preview);
				Update_Network_Object(preview);
				Set_Object_Visibility(previewObjectId,false);
				if ((int)Get_Vector3_Parameter("MatchTeam[Preview|Deployed|Repaired]").X)
					Commands->Set_Player_Type(preview,Commands->Get_Player_Type(obj));
				Commands->Attach_To_Object_Bone(preview,obj,"origin");
				if (!placementBlocked)
					Commands->Set_Model(preview,Get_Parameter("PlaceablePreview"));
				else
					Commands->Set_Model(preview,Get_Parameter("UnPlaceablePreview"));
				Set_Object_Visibility_For_Player(obj,previewObjectId,true);
			}
		}
		else if (previewObjectId)
			DestroyPreview();
		Commands->Start_Timer(obj,this,0.25f,1);
	}
	if (number == 2)
	{
		if (reloadTime && canRegen)
		{
			reloadTime--;
			if (!reloadTime)
			{
				Set_Bullets(obj,Get_Parameter("WeaponPreset"),1);
				JmgUtility::DisplayChatMessage(obj,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").X,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Y,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Z,Get_Parameter("PlacementMessage")); 
			}
		}
		Commands->Start_Timer(obj,this,1.0f,2);
	}
}
void JMG_Utility_Infantry_Placed_Buildable_Object::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("DeployCustom"))
	{
		if (Get_Vehicle(obj) || !Commands->Get_Health(obj))
			return;
		const char *weap = Get_Current_Weapon(obj);
		if (!weap || _stricmp(weap,Get_Parameter("WeaponPreset")))
			return;
		if (obj->As_SoldierGameObj() && obj->As_SoldierGameObj()->Is_Airborne())
		{
			JmgUtility::DisplayChatMessage(obj,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").X,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Y,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Z,"You must be on the ground to place this!");
			return;
		}
		if (obj->As_SoldierGameObj() && obj->As_SoldierGameObj()->Is_On_Ladder())
		{
			JmgUtility::DisplayChatMessage(obj,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").X,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Y,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Z,"You cannot place this while climbing ladders!");
			return;
		}
		if (placementBlocked)
		{
			JmgUtility::DisplayChatMessage(obj,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").X,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Y,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Z,Get_Parameter("DeploymentBlockedMessage"));
			return;
		}
		if (!canRegen && reloadTime)
		{
			JmgUtility::DisplayChatMessage(obj,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").X,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Y,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Z,"You must repair the last one you placed before you can place another.");
			return;
		}
		if (reloadTime)
		{
			char reloadTimeMessage[220];
			sprintf(reloadTimeMessage,"You must wait another %d seconds before you can place this.",reloadTime);
			JmgUtility::DisplayChatMessage(obj,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").X,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Y,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Z,reloadTimeMessage);
			return;
		}
		if (Get_Float_Parameter("Cost") && Commands->Get_Money(obj) < Get_Float_Parameter("Cost"))
		{
			char costMessage[220];
			sprintf(costMessage,"You need $%s to place this.",JmgUtility::formatDigitGrouping(Get_Float_Parameter("Cost")));
			JmgUtility::DisplayChatMessage(obj,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").X,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Y,(int)Get_Vector3_Parameter("MessageColor[R|G|B]").Z,costMessage);
			return;
		}
		if (Get_Float_Parameter("Cost"))
			Commands->Give_Money(obj,-Get_Float_Parameter("Cost"),false);
		canRegen = Get_Int_Parameter("RegenStartsAfterRepair") ? false : true;
		reloadTime = Get_Int_Parameter("RegainTimer");
		Set_Bullets(obj,Get_Parameter("WeaponPreset"),0);
		GameObject *preview = Commands->Find_Object(previewObjectId);
		if (preview)
			Commands->Destroy_Object(preview);
		previewObjectId = 0;
		GameObject *placed = Commands->Create_Object(Get_Parameter("DeployPreset"),Commands->Get_Position(obj));
		if ((int)Get_Vector3_Parameter("MatchTeam[Preview|Deployed|Repaired]").Y)
			Commands->Set_Player_Type(placed,Commands->Get_Player_Type(obj));
		Commands->Set_Facing(placed,Commands->Get_Facing(obj));
		char params[220];
		sprintf(params,"%d,%d,%s,%.0f,%d",this->Get_ID(),Commands->Get_ID(obj),Get_Parameter("RepairedPreset"),Get_Vector3_Parameter("MatchTeam[Preview|Deployed|Repaired]").Z,Commands->Get_Player_Type(obj));
		Commands->Attach_Script(placed,"JMG_Utility_Infantry_Placed_Buildable_Object_Attached",params);
	}
	if (message == Get_Int_Parameter("EnableCustom"))
	{
		if (placementBlocked)
			placementBlocked--;
		if (!placementBlocked)
		{
			GameObject *preview = Commands->Find_Object(previewObjectId);
			if (preview)
				Commands->Set_Model(preview,Get_Parameter("PlaceablePreview"));
		}
	}
	if (message == Get_Int_Parameter("DisableCustom"))
	{
		if (!placementBlocked)
		{
			GameObject *preview = Commands->Find_Object(previewObjectId);
			if (preview)
				Commands->Set_Model(preview,Get_Parameter("UnPlaceablePreview"));
		}
		placementBlocked++;
	}
	if (message == 9117115 && param == this->Get_ID())
	{
		reloadTime = Get_Int_Parameter("DestroyedRegenTime");
		canRegen = true;
	}
	if (message == 9117116 && param == this->Get_ID())
	{
		reloadTime = 0;
		Set_Bullets(obj,Get_Parameter("WeaponPreset"),1);
	}
	if (message == 9117117 && param == this->Get_ID())
	{
		canRegen = true;
	}
	if (message == 9117118 && param == this->Get_ID())
	{
		if (Get_Float_Parameter("Cost"))
			Commands->Give_Money(obj,Get_Float_Parameter("Cost"),false);
	}
}
void JMG_Utility_Infantry_Placed_Buildable_Object::Killed(GameObject *obj,GameObject *killer)
{
	DestroyPreview();
}
void JMG_Utility_Infantry_Placed_Buildable_Object::Destroyed(GameObject *obj)
{
	DestroyPreview();
}
void JMG_Utility_Infantry_Placed_Buildable_Object::DestroyPreview()
{
	GameObject *preview = Commands->Find_Object(previewObjectId);
	if (preview)
		Commands->Destroy_Object(preview);
	previewObjectId = 0;
}
void JMG_Utility_Infantry_Placed_Buildable_Object_Attached::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage < 0 && Get_Hitpoints(obj) == Get_Max_Hitpoints(obj))
	{
		GameObject *placed = Commands->Create_Object(Get_Parameter("RepairedPreset"),Commands->Get_Position(obj));
		if (Get_Int_Parameter("MatchTeam"))
			Commands->Set_Player_Type(placed,Get_Int_Parameter("Team"));
		Commands->Set_Facing(placed,Commands->Get_Facing(obj));
		Commands->Destroy_Object(obj);
		GameObject *placer = Commands->Find_Object(Get_Int_Parameter("PlacerId"));
		if (placer)
			Commands->Send_Custom_Event(obj,placer,9117117,Get_Int_Parameter("ScriptId"),0);
	}
}
void JMG_Utility_Infantry_Placed_Buildable_Object_Attached::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *placer = Commands->Find_Object(Get_Int_Parameter("PlacerId"));
	if (Commands->Get_Player_Type(placer) == Commands->Get_Player_Type(killer))
		Commands->Send_Custom_Event(obj,placer,9117116,Get_Int_Parameter("ScriptId"),0);
	else if (placer)
		Commands->Send_Custom_Event(obj,placer,9117115,Get_Int_Parameter("ScriptId"),0);
	Commands->Send_Custom_Event(obj,placer,9117118,Get_Int_Parameter("ScriptId"),0);
}
void JMG_Utility_Lock_Weapon_Selection_While_Script_Attached::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Lock_Weapon_Selection_While_Script_Attached::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		const char *weap = Get_Current_Weapon(obj);
		if (Has_Weapon(obj,Get_Parameter("WeaponPreset")) && (!weap || _stricmp(weap,Get_Parameter("WeaponPreset"))))
			Commands->Select_Weapon(obj,Get_Parameter("WeaponPreset"));
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
float JMG_Utility_Swimming_Zone::fogMinDistance = 200.0f;
float JMG_Utility_Swimming_Zone::fogMaxDistance = 300.0f;
void JMG_Utility_Swimming_zDefault_Map_Fog_Values::Created(GameObject *obj)
{
	Get_Fog_Range(JMG_Utility_Swimming_Zone::fogMinDistance,JMG_Utility_Swimming_Zone::fogMaxDistance);
}
void JMG_Utility_Swimming_Zone::Created(GameObject *obj)
{
	Get_Fog_Range(fogMinDistance,fogMaxDistance);
}
void JMG_Utility_Swimming_Zone::Entered(GameObject *obj,GameObject *enter)
{
	if (!enter->As_SoldierGameObj())
		return;
	if (!Is_Script_Attached(enter,"JMG_Utility_Swimming_Infantry"))
	{
		Commands->Apply_Damage(enter,9999.9f,"Death",obj);
		if (Commands->Is_A_Star(enter) && !Commands->Get_Health(enter))
		{
			Commands->Attach_Script(enter,"JMG_Utility_Reset_Screen_Fade_And_Fog_On_Destroy","");
			Set_Screen_Fade_Color_Player(enter,Get_Vector3_Parameter("WaterColor[R|G|B]").X,Get_Vector3_Parameter("WaterColor[R|G|B]").Y,Get_Vector3_Parameter("WaterColor[R|G|B]").Z,0.0f);
			Set_Screen_Fade_Opacity_Player(enter,Get_Float_Parameter("WaterOpacity"),0.1f);
			Set_Fog_Range_Player(obj,Get_Float_Parameter("WaterMinViewDistance"),JMG_Utility_Swimming_Zone::Get_Float_Parameter("WaterMaxViewDistance"),0.1f);
		}
		return;
	}
	waterNode[JmgUtility::JMG_Get_Player_ID(enter)] = PlayerWaterNode(Get_Vector3_Parameter("WaterColor[R|G|B]"),Get_Float_Parameter("WaterOpacity"),Get_Float_Parameter("WaterMinViewDistance"),Get_Float_Parameter("WaterMaxViewDistance"));
	Commands->Send_Custom_Event(obj,enter,347341,(int)(Get_Float_Parameter("SwimmingSpeedMultiplier")*100),0);
}
void JMG_Utility_Swimming_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	if (!exiter->As_SoldierGameObj())
		return;
	Commands->Send_Custom_Event(obj,exiter,347340,0,0);
}
void JMG_Utility_Swimming_Zone::Update_Fog_Settings(float minFog,float maxFog)
{
	fogMinDistance = minFog;
	fogMaxDistance = maxFog;
}
JMG_Utility_Swimming_Zone::PlayerWaterNode JMG_Utility_Swimming_Zone::waterNode[128] = {JMG_Utility_Swimming_Zone::PlayerWaterNode()};
bool JMG_Utility_Swimming_Infantry::isUnderwater[128] = {false};
bool JMG_Utility_Swimming_Infantry::isInWater[128] = {false};
void JMG_Utility_Swimming_Infantry::Created(GameObject *obj)
{
	lastDisplayTime = NULL;
	lastWaterZoneId = 0;
	heartBeatSoundId = 0;
	pantSoundId = 0;
	playerId = JmgUtility::JMG_Get_Player_ID(obj);
	underwater = false;
	waterZoneCount = 0;
	startedFadeRed = false;
	drownTime = 0.0f;
	defaultSpeed = obj->As_SoldierGameObj()->Get_Max_Speed();
	for (SLNode<ScriptZoneGameObj>* node = GameObjManager::ScriptZoneGameObjList.Head(); node; node = node->Next())
	{
		GameObject *zone = (GameObject *)node->Data();
		if (Is_Script_Attached(zone,"JMG_Utility_Swimming_Zone") && IsInsideZone(zone,obj))
			Commands->Send_Custom_Event(obj,obj,347341,347341,0.25f);
	}
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Swimming_Infantry::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1 && Commands->Get_Health(obj))
	{
		if (obj->As_SoldierGameObj()->Is_Crouched() && !obj->As_SoldierGameObj()->Is_Airborne() && waterZoneCount)
		{
			if (!underwater)
			{
				underwater = true;
				isUnderwater[playerId] = true;
				DestroySoundEmitter(&pantSoundId);
				if (startedFadeRed)
				{
					Set_Screen_Fade_Color_Player(obj,1.0f,0.0f,0.0f,JmgUtility::MathClamp(Get_Float_Parameter("DrownTime")-drownTime,0,Get_Float_Parameter("StarDrownSequence")));
					CreateSoundEmitter(obj,Get_Parameter("HeartBeatSoundEmitterModel"),&heartBeatSoundId);
				}
				else
					Set_Screen_Fade_Color_Player(obj,JMG_Utility_Swimming_Zone::waterNode[playerId].waterColor.X,JMG_Utility_Swimming_Zone::waterNode[playerId].waterColor.Y,JMG_Utility_Swimming_Zone::waterNode[playerId].waterColor.Z,0.0f);
				Set_Screen_Fade_Opacity_Player(obj,JMG_Utility_Swimming_Zone::waterNode[playerId].waterColorOpacity,0.1f);
				Set_Fog_Range_Player(obj,JMG_Utility_Swimming_Zone::waterNode[playerId].waterMinViewDistance,JMG_Utility_Swimming_Zone::waterNode[playerId].waterMaxViewDistance,0.1f);
			}
			drownTime += 0.1f;
			if (!startedFadeRed && drownTime >= Get_Float_Parameter("DrownTime")-Get_Float_Parameter("StarDrownSequence"))
			{
				startedFadeRed = true;
				Set_Screen_Fade_Color_Player(obj,1.0f,0.0f,0.0f,Get_Float_Parameter("StarDrownSequence"));
				CreateSoundEmitter(obj,Get_Parameter("HeartBeatSoundEmitterModel"),&heartBeatSoundId);
			}
			if (drownTime >= Get_Float_Parameter("DrownTime"))
				Commands->Apply_Damage(obj,Get_Float_Parameter("DrownDamageRate"),"None",obj);
		}
		else
		{
			if (underwater)
			{
				underwater = false;
				isUnderwater[playerId] = false;
				JMG_Utility_Set_Screen_Color_Fade_Controller::Update_Player(obj,0.1f);
				Set_Fog_Range_Player(obj,JMG_Utility_Swimming_Zone::fogMinDistance,JMG_Utility_Swimming_Zone::fogMaxDistance,0.1f);
				DestroySoundEmitter(&heartBeatSoundId);
				if (drownTime > 1.0f)
					CreateSoundEmitter(obj,Get_Parameter("PantingSoundEmitterModel"),&pantSoundId);
				if (drownTime > Get_Float_Parameter("DrownTime"))
					Commands->Create_3D_Sound_At_Bone(Get_Parameter("GaspForBreath"),obj,"c head");
			}
			if (drownTime)
			{
				drownTime -= Get_Float_Parameter("CatchBreathRate");
				if (startedFadeRed && drownTime < Get_Float_Parameter("DrownTime")-Get_Float_Parameter("StarDrownSequence"))
					startedFadeRed = false;
				if (drownTime <= 0)
				{
					drownTime = 0.0f;
					DestroySoundEmitter(&pantSoundId);
				}
			}
		}
		if (waterZoneCount)
		{
			const char *weap = Get_Current_Weapon(obj);
			if (Has_Weapon(obj,Get_Parameter("WeaponPreset")) && (!weap || _stricmp(weap,Get_Parameter("WeaponPreset"))))
				Commands->Select_Weapon(obj,Get_Parameter("WeaponPreset"));
			if (Get_Float_Parameter("WaterDamageAmount"))
				Commands->Apply_Damage(obj,Get_Float_Parameter("WaterDamageAmount"),Get_Parameter("WaterDamageWarhead"),Commands->Find_Object(lastWaterZoneId));
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Swimming_Infantry::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 347340)
	{
		Commands->Send_Custom_Event(sender,obj,347342,param,0.25f);
	}
	if (message == 347342)
	{
		if (obj->As_SoldierGameObj()->Is_Airborne())
		{
			Commands->Send_Custom_Event(sender,obj,message,param,0.25f);
			return;
		}
		waterZoneCount--;
		if (!waterZoneCount)
		{
			isInWater[playerId] = false;
			obj->As_SoldierGameObj()->Set_Can_Play_Damage_Animations(true);
			obj->As_SoldierGameObj()->Set_Movement_Loiters_Allowed(true);
			if (Has_Weapon(obj,enterWeapon))
				Commands->Select_Weapon(obj,enterWeapon);
			if (Has_Weapon(obj,Get_Parameter("WeaponPreset")))
				Remove_Weapon(obj,Get_Parameter("WeaponPreset"));
			obj->As_SoldierGameObj()->Set_Max_Speed(defaultSpeed);
		}
	}
	if (message == 347341)
	{
		lastWaterZoneId = Commands->Get_ID(sender);
		if (!waterZoneCount)
		{
			if (Get_Int_Parameter("EnterWaterMessageStringId"))
			{
				time_t currentTime = clock();
				if (difftime(currentTime,lastDisplayTime) > 10000.0f)
				{
					Set_HUD_Help_Text_Player(obj,Get_Int_Parameter("EnterWaterMessageStringId"),Get_Vector3_Parameter("WaterEnterMessageColor[R|G|B]"));
					lastDisplayTime = currentTime;
				}
				obj->As_SoldierGameObj()->Set_Max_Speed(defaultSpeed*(param/100.0f));
			}
			isInWater[playerId] = true;
			sprintf(enterWeapon,"%s",Get_Current_Weapon(obj) ? Get_Current_Weapon(obj) : "");
			Grant_Weapon(obj,Get_Parameter("WeaponPreset"),true,-1,1);
			obj->As_SoldierGameObj()->Set_Can_Play_Damage_Animations(false);
			obj->As_SoldierGameObj()->Set_Movement_Loiters_Allowed(false);
			Commands->Set_Loiters_Allowed(obj,false);
		}
		waterZoneCount++;
	}
}
void JMG_Utility_Swimming_Infantry::Killed(GameObject *obj,GameObject *killer)
{
	DestroySoundEmitter(&heartBeatSoundId);
	DestroySoundEmitter(&pantSoundId);
}
void JMG_Utility_Swimming_Infantry::Destroyed(GameObject *obj)
{
	JMG_Utility_Set_Screen_Color_Fade_Controller::Update_Player(obj,0.0f);
	Set_Fog_Range_Player(obj,JMG_Utility_Swimming_Zone::fogMinDistance,JMG_Utility_Swimming_Zone::fogMaxDistance,0.0f);
	DestroySoundEmitter(&heartBeatSoundId);
	DestroySoundEmitter(&pantSoundId);
}
void JMG_Utility_Swimming_Infantry::Detach(GameObject *obj)
{
	if (Exe == 4)
		return;
	Destroyed(obj);
}
void JMG_Utility_Swimming_Infantry::CreateSoundEmitter(GameObject *obj,const char *model,int *soundId)
{
	GameObject *soundEmitter = Commands->Find_Object(*soundId);
	if (!soundEmitter)
	{
		soundEmitter = Commands->Create_Object("Daves Arrow",Commands->Get_Position(obj));
		*soundId = Commands->Get_ID(soundEmitter);
		Commands->Attach_To_Object_Bone(soundEmitter,obj,"c head");
	}
	Commands->Set_Model(soundEmitter,model);
}
void JMG_Utility_Swimming_Infantry::DestroySoundEmitter(int *soundId)
{
	GameObject *soundEmitter = Commands->Find_Object(*soundId);
	if (soundEmitter)
		Commands->Destroy_Object(soundEmitter);
	*soundId = 0;
}
void JMG_Utility_Zone_Enable_Spawners_In_Range::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(enter) == -4)
		return;
	int end = Get_Int_Parameter("EndID");
	bool enable = Get_Int_Parameter("Enable") ? true : false;
	for (int x = Get_Int_Parameter("StartID");x <= end;x++)
		Commands->Enable_Spawner(x,enable);
	if (Get_Int_Parameter("TriggerOnce"))
		Destroy_Script();
}
void JMG_Utility_Display_Message_On_Vehicle_Enter::Created(GameObject *obj)
{
	for (int x = 0;x < 128;x++)
		hasShownMessage[x] = false;
}
void JMG_Utility_Display_Message_On_Vehicle_Enter::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		int playerId = JmgUtility::JMG_Get_Player_ID(sender);
		if (!playerId)
			return;
		if (CheckPlayerType(sender,Get_Int_Parameter("PlayerType")))
			return;
		if (Get_Int_Parameter("DriverOnly") && Get_Vehicle_Driver(obj) != sender)
			return;
		if (!Get_Int_Parameter("ShowOnce") || !hasShownMessage[playerId])
		{
			hasShownMessage[playerId] = true;
			if (_stricmp(Get_Parameter("MessageOverride"),"null"))
				Set_HUD_Help_Text_Player_Text(sender,Get_Int_Parameter("StringId"),Get_Parameter("MessageOverride"),Get_Vector3_Parameter("Color[R|G|B]"));
			else
				Set_HUD_Help_Text_Player(sender,Get_Int_Parameter("StringId"),Get_Vector3_Parameter("Color[R|G|B]"));
		}
	}
}
void JMG_Utility_Zone_Apply_Damage_On_Enter::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(enter) == -4)
		return;
	GameObject *target = Get_Int_Parameter("ID") ? Commands->Find_Object(Get_Int_Parameter("ID")) : enter;
	if (target)
	{
		VehicleGameObj *vehicleGameObj = target->As_VehicleGameObj();
		if (vehicleGameObj && Get_Int_Parameter("DamageOccupants"))
		{
			int x = vehicleGameObj->Get_Definition().Get_Seat_Count();
			PhysicalGameObj *physicalGameObj = obj->As_PhysicalGameObj();
			OffenseObjectClass offenseObjectClass = OffenseObjectClass(Get_Float_Parameter("DamageAmount"),ArmorWarheadManager::Get_Warhead_Type(Get_Parameter("Warhead")),physicalGameObj ? physicalGameObj->As_ArmedGameObj() : NULL);
			for (int i = 0;i < x;i++)
			{
				SoldierGameObj *soldierGameObj = vehicleGameObj->Get_Occupant(i);
				if (soldierGameObj)
				{
					soldierGameObj->Apply_Damage_IgnoreVehicleCheck(offenseObjectClass,1,-1);
					if (!Commands->Get_Health(soldierGameObj))
					{
						if (Get_Vehicle_Driver(target) == soldierGameObj)
							Commands->Enable_Engine(target,false);
						Toggle_Fly_Mode(soldierGameObj);
						Commands->Apply_Damage(soldierGameObj,1.0,"BlamoKiller",obj);
					}
				}
			}
		}
		Commands->Apply_Damage(target,Get_Float_Parameter("DamageAmount"),Get_Parameter("Warhead"),obj);
	}
	if (Get_Int_Parameter("OnlyOnce"))
		this->Destroy_Script();
}
void JMG_Utility_AI_Guardian_Aircraft::Created(GameObject *obj)
{
	dpPosition = Commands->Get_Position(obj);
	EnemyID = 0;
	EnemyTimeOutTime = 0;
	LastPos = Commands->Get_Position(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Enable_Engine(obj,true);
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_AI_Guardian_Aircraft::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (EnemyTimeOutTime)
			EnemyTimeOutTime--;
		if (!EnemyTimeOutTime)
		{
			EnemyID = 0;
			Goto_Location(obj);
		}
		if (EnemyID)
		{
			GameObject *Target = Commands->Find_Object(EnemyID);
			if (!Target || !Commands->Get_Health(Target) || Commands->Get_Player_Type(Target) == Commands->Get_Player_Type(obj))
			{
				EnemyID = 0;
				Goto_Location(obj);
			}
		}
		Vector3 Pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(Pos,LastPos) < 25.0)
		{
			if (Get_A_Defense_Point(&dpPosition))
				Goto_Location(obj);
		}
		LastPos = Pos;
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Utility_AI_Guardian_Aircraft::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (Is_Script_Attached(seen,"JMG_Utility_AI_Guardian_Ignored"))
		return;
	if (!EnemyID || !EnemyTimeOutTime)
	{
		EnemyID = Commands->Get_ID(seen);
		EnemyTimeOutTime = 2;
		Goto_Location(obj);
	}
	else if (EnemyID == Commands->Get_ID(seen))
		EnemyTimeOutTime = 2;
}
void JMG_Utility_AI_Guardian_Aircraft::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0 || (EnemyID && EnemyTimeOutTime) || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	EnemyID = Commands->Get_ID(damager);
	EnemyTimeOutTime = 2;
	Goto_Location(obj);
}
void JMG_Utility_AI_Guardian_Aircraft::Goto_Location(GameObject *obj)
{
	ActionParamsStruct params;
	Commands->Action_Reset(obj,100);
	Vector3 GoToPos = dpPosition;
	GoToPos.Z += Get_Float_Parameter("FlightHeight");
	params.Set_Movement(GoToPos,1.0f,1.0f,false);
	params.MovePathfind = false;
	params.Set_Basic(this,100,10);
	GameObject *Target = Commands->Find_Object(EnemyID);
	if (Target)
	{
		if (Get_Int_Parameter("FaceTarget"))
			params.Set_Face_Location(Commands->Get_Position(Target),5.0f);
		else
			params.AttackFaceTarget = false;
		params.Set_Attack(Target,Get_Float_Parameter("FireRange"),0,true);
		Commands->Action_Attack(obj,params);
	}
	else
	{
		if (Get_Int_Parameter("FaceTarget"))
			params.Set_Face_Location(GoToPos,5.0f);
		else
			params.AttackFaceTarget = false;
		Commands->Action_Goto(obj,params);
	}
}
void JMG_Utility_Switch_Weapon_While_Primary_Empty::Created(GameObject *obj)
{
	if (Get_Float_Parameter("SecondaryToPrimaryTime") != -1.0)
		Commands->Set_Animation(obj,Get_Parameter("IdlePrimaryAnim"),true,0,0.0,-1,false);
	Commands->Start_Timer(obj,this,0.25f,1);
}
void JMG_Utility_Switch_Weapon_While_Primary_Empty::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		const char *weap = Get_Current_Weapon(obj);
		if (!Get_Bullets(obj,Get_Parameter("PrimaryWeapon")) && !_stricmp(weap,Get_Parameter("PrimaryWeapon")))
		{
			Commands->Select_Weapon(obj,Get_Parameter("SecondaryWeapon"));
			Commands->Start_Timer(obj,this,Get_Float_Parameter("ReloadTime"),2);
			if (Get_Float_Parameter("PrimaryToSecondaryTime") != -1.0)
				Commands->Start_Timer(obj,this,Get_Float_Parameter("PrimaryToSecondaryTime"),3);
			else
				Commands->Start_Timer(obj,this,0.0f,3);
			if (Get_Float_Parameter("SecondaryToPrimaryTime") != -1.0)
				Commands->Start_Timer(obj,this,Get_Float_Parameter("ReloadTime")-Get_Float_Parameter("SecondaryToPrimaryTime"),5);
			else
				Commands->Start_Timer(obj,this,Get_Float_Parameter("ReloadTime"),5);
			if (_stricmp(Get_Parameter("PrimaryToSecondaryAnim"),"null"))
				Commands->Set_Animation(obj,Get_Parameter("PrimaryToSecondaryAnim"),false,0,0.0,-1,false);
			if (_stricmp(Get_Parameter("PrimaryToSecondarySound"),"null"))
				Commands->Create_Sound(Get_Parameter("PrimaryToSecondarySound"),Commands->Get_Position(obj),obj);
		}
		Commands->Start_Timer(obj,this,0.25f,1);
	}
	if (number == 2)
	{
		Set_Bullets(obj,Get_Parameter("PrimaryWeapon"),Get_Max_Bullets(obj,Get_Parameter("PrimaryWeapon")));
		Commands->Select_Weapon(obj,Get_Parameter("PrimaryWeapon"));
	}
	if (number == 3 && _stricmp(Get_Parameter("IdleSecondaryAnim"),"null"))
	{
		Commands->Set_Animation(obj,Get_Parameter("IdleSecondaryAnim"),true,0,0.0,-1,false);
	}
	if (number == 4 && _stricmp(Get_Parameter("IdlePrimaryAnim"),"null"))
	{
		Commands->Set_Animation(obj,Get_Parameter("IdlePrimaryAnim"),true,0,0.0,-1,false);
	}
	if (number == 5)
	{
		if (_stricmp(Get_Parameter("SecondaryToPrimaryAnim"),"null"))
			Commands->Set_Animation(obj,Get_Parameter("SecondaryToPrimaryAnim"),false,0,0.0,-1,false);
			if (_stricmp(Get_Parameter("SecondaryToPrimarySound"),"null"))
				Commands->Create_Sound(Get_Parameter("SecondaryToPrimarySound"),Commands->Get_Position(obj),obj);
		if (Get_Float_Parameter("SecondaryToPrimaryTime") > -1.0f)
			Commands->Start_Timer(obj,this,Get_Float_Parameter("SecondaryToPrimaryTime"),4);
		else
			Commands->Start_Timer(obj,this,0.0f,4);
	}
}
void JMG_Utility_Send_Custom_When_Near_Building::Created(GameObject *obj)
{
	nearBuilding = false;
	Commands->Start_Timer(obj,this,Get_Float_Parameter("CheckRate"),1);
}
void JMG_Utility_Send_Custom_When_Near_Building::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1 && Commands->Get_Health(obj))
	{
		Vector3 myPos = Commands->Get_Position(obj);
		float ClosestDist = FLT_MAX;
		for (SLNode<BuildingGameObj> *z = GameObjManager::BuildingGameObjList.Head();z;z = z->Next())
		{
			if ((Get_Int_Parameter("BuildingPlayerType") == 2 || z->Data()->Get_Player_Type() == Get_Int_Parameter("BuildingPlayerType")) && (Get_Int_Parameter("CheckDeadBuildings") || !z->Data()->Is_Destroyed()))
			{
				float Dist = z->Data()->Find_Closest_Poly(myPos);
				if (Dist < ClosestDist)
					ClosestDist = Dist;
			}
		}
		if (ClosestDist < Get_Float_Parameter("CloseToBuildingDistance") && !nearBuilding)
		{
			nearBuilding = true;
			if (Get_Int_Parameter("SendCustomObjectID"))
				Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("SendCustomObjectID")),Get_Int_Parameter("NearToBuildingCustom"),0,0);
			else
				Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("NearToBuildingCustom"),0,0);
		}
		if (ClosestDist > Get_Float_Parameter("CloseToBuildingDistance") && nearBuilding)
		{
			nearBuilding = false;
			if (Get_Int_Parameter("SendCustomObjectID"))
				Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("SendCustomObjectID")),Get_Int_Parameter("FarFromBuildingCustom"),0,0);
			else
				Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("FarFromBuildingCustom"),0,0);
		}
		Commands->Start_Timer(obj,this,Get_Float_Parameter("CheckRate"),1);
	}
}
void JMG_Utility_AI_Engineer_Repair_Target::Created(GameObject *obj)
{
}
void JMG_Utility_Reset_Screen_Fade_And_Fog_On_Destroy::Destroyed(GameObject *obj)
{
	JMG_Utility_Set_Screen_Color_Fade_Controller::Update_Player(obj,0.0f);
	Set_Fog_Range_Player(obj,JMG_Utility_Swimming_Zone::fogMinDistance,JMG_Utility_Swimming_Zone::fogMaxDistance,0.0f);
}
void JMG_Utility_Reset_Screen_Fade_And_Fog_On_Destroy::Detach(GameObject *obj)
{
	if (Exe == 4)
		return;
	Destroyed(obj);
}
void JMG_Utility_AI_Goto_Player::Created(GameObject *obj)
{
	state = IDLE;
	moveBackward = false;
	lastPosition = homeLocation = Commands->Get_Position(obj);
	targetId = 0;
	lastSeenTime = 0;
	huntingStarId = 0;
	stuckTime = 0;
	reverseTime = 0;
	huntStealth = Get_Int_Parameter("HuntStealth") ? true : false;
	huntSearchDistance = Get_Float_Parameter("HuntSearchDistance") < 0.0f ? Get_Float_Parameter("HuntSearchDistance") : Get_Float_Parameter("HuntSearchDistance")*Get_Float_Parameter("HuntSearchDistance");
	for (int x = 0;x < 128;x++)
		ignoreStarsTime[x] = 0;
	const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(Get_Current_Weapon(obj),true);
	if (ammo)
	{
		weaponRange = ammo->Range;
		weaponEffectiveRange = ammo->Range < ammo->EffectiveRange ? ammo->Range : ammo->EffectiveRange;
	}
	attackArriveDistance = Get_Float_Parameter("AttackDistance") >= 0 ? Get_Float_Parameter("AttackDistance") : weaponEffectiveRange;
	huntArriveDistance = Get_Float_Parameter("HuntArriveDistance");
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,1.0f,1);
	Commands->Start_Timer(obj,this,1.0f,2);
	if (Get_Float_Parameter("RandomAttackDistance"))
		Commands->Start_Timer(obj,this,0.0f,3);
	if (Get_Float_Parameter("RandomHuntArriveDistance"))
		Commands->Start_Timer(obj,this,0.0f,4);
}
void JMG_Utility_AI_Goto_Player::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (Is_Script_Attached(seen,"JMG_Utility_AI_Goto_Player_Ignore_Object"))
		return;
	if (!_stricmp(Get_Skin(seen),"Blamo") || !_stricmp(Get_Shield_Type(seen),"Blamo"))
		return;
	if (Get_Int_Parameter("CanSeeStealth") != 2 && seen->As_SmartGameObj() && seen->As_SmartGameObj()->Is_Stealthed())
	{
		if (!Get_Int_Parameter("CanSeeStealth"))
			return;
		float dist = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen));
		if (seen->As_SoldierGameObj() && dist > seen->As_SoldierGameObj()->Get_Stealth_Fade_Distance()*seen->As_SoldierGameObj()->Get_Stealth_Fade_Distance())
			return;
		else if (seen->As_VehicleGameObj() && dist > seen->As_VehicleGameObj()->Get_Stealth_Fade_Distance()*seen->As_VehicleGameObj()->Get_Stealth_Fade_Distance())
			return;	
		else if (dist > seen->As_SmartGameObj()->Get_Stealth_Fade_Distance()*seen->As_SmartGameObj()->Get_Stealth_Fade_Distance())
			return;
	}
	if (!targetId || !lastSeenTime || Choose_Target(obj,seen))
	{
		targetId = Commands->Get_ID(seen);
		lastSeenTime = Commands->Get_Random_Int(30,60);
		state = ATTACKING_TARGET;
		Attack_Move(obj,seen,Vector3(),Get_Float_Parameter("AttackSpeed"),attackArriveDistance,true,false);
	}
	if (targetId == Commands->Get_ID(seen))
		lastSeenTime = Commands->Get_Random_Int(30,60);
}
void JMG_Utility_AI_Goto_Player::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 targetPos = Commands->Get_Position(obj);
		if (lastSeenTime)
		{
			lastSeenTime--;
			if (!lastSeenTime)
			{
				targetId = 0;
				Return_Home(obj);
			}
		}
		if (targetId)
		{
			GameObject *target = Commands->Find_Object(targetId);
			targetPos = Commands->Get_Position(target);
			if (!target || !Commands->Get_Health(target) || Commands->Get_Player_Type(target) == -2 || (Commands->Get_Player_Type(obj) != -1 && Commands->Get_Player_Type(target) == Commands->Get_Player_Type(obj)))
			{
				targetId = 0;
				Return_Home(obj);
			}
		}
		if (state == HUNTING_STAR)
		{
			GameObject *star = Get_GameObj(huntingStarId);
			if (!star || !Commands->Get_Health(star))
				Return_Home(obj);
		}
		if (state == IDLE || state == RETURNING_HOME || state == WANDERING_GROUP)
		{
			GameObject *star = findClosestStar(obj);
			if (star)
			{
				state = HUNTING_STAR;
				Attack_Move(obj,star,Vector3(),Get_Float_Parameter("HuntSpeed"),huntArriveDistance,false,false);
			}
		}
		if (state == HUNTING_STAR)
		{
			GameObject *star = Get_GameObj(huntingStarId);
			if (!star || (huntSearchDistance >= 0.0f && JmgUtility::SimpleDistance(Commands->Get_Position(star),Commands->Get_Position(obj)) > huntSearchDistance))
				Return_Home(obj);
		}
		if (state == IDLE && Get_Int_Parameter("WanderingAIGroupID") != -1)
		{
			Vector3 wanderPos = Vector3();
			if (GetRandomPosition(&wanderPos))
			{
				state = WANDERING_GROUP;
				Attack_Move(obj,NULL,wanderPos,Get_Float_Parameter("WanderSpeed"),1.0f,false,false);
			}
		}
		if (state == RETURNING_HOME || state == WANDERING_GROUP)
			if (JmgUtility::SimpleDistance(lastAction.location,Commands->Get_Position(obj)) <= (obj->As_VehicleGameObj() ? 25.0f : 1.0f))
				state = IDLE;
		if (state == ACTION_BADPATH)
			Cant_Get_To_target(obj);
		if (state != IDLE)
			Stuck_Check(obj,targetPos);
		Commands->Start_Timer(obj,this,0.1f,1);
	}
	if (number == 2)
	{
		for (int x = 1;x < 128;x++)
		{
			if (ignoreStarsTime[x])
				ignoreStarsTime[x]--;
		}
		Commands->Start_Timer(obj,this,1.0f,2);
	}
	if (number == 3)
	{
		attackArriveDistance = Get_Float_Parameter("AttackDistance") >= 0 ? Get_Float_Parameter("AttackDistance") : weaponEffectiveRange;
		attackArriveDistance += Commands->Get_Random(Get_Float_Parameter("RandomAttackDistance"),Get_Float_Parameter("RandomAttackDistance"));
	}
	if (number == 4)
	{
		huntArriveDistance = Get_Float_Parameter("HuntArriveDistance");
		huntArriveDistance += Commands->Get_Random(Get_Float_Parameter("RandomHuntArriveDistance"),Get_Float_Parameter("RandomHuntArriveDistance"));
	}
}
void JMG_Utility_AI_Goto_Player::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason)
{
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{
		if (state == HUNTING_STAR)
		{
			ignoreStarsTime[huntingStarId] = 10;
			state = ACTION_BADPATH;
		}
		else if (state == ATTACKING_TARGET)
			state = ACTION_BADPATH;
	}
}
void JMG_Utility_AI_Goto_Player::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0.0f || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	if (!_stricmp(Get_Skin(damager),"Blamo") || !_stricmp(Get_Shield_Type(damager),"Blamo"))
		return;
	if (!targetId || !lastSeenTime || Choose_Target(obj,damager))
	{
		targetId = Commands->Get_ID(damager);
		lastSeenTime = Commands->Get_Random_Int(30,60);
		state = ATTACKING_TARGET;
		Attack_Move(obj,damager,Vector3(),Get_Float_Parameter("AttackSpeed"),attackArriveDistance,true,false);
	}
}
void JMG_Utility_AI_Goto_Player::Attack_Move(GameObject *obj,GameObject *target,Vector3 location,float speed,float distance,bool attack,bool overrideLocation)
{
	lastAction.location = location;
	Commands->Action_Reset(obj,100);
	lastAction = LastAction(targetId,location,speed,distance,attack,overrideLocation);
	ActionParamsStruct params;
	params.ObserverID = this->Get_ID();
	if (target && !overrideLocation)
	{
		params.Set_Movement(target,speed,distance,false);
		params.MoveFollow = true;
	}
	else
	{
		params.Set_Movement(location,speed,distance,false);
		params.MoveFollow = false;
	}
	params.MoveBackup = moveBackward;
	params.MovePathfind = true;
	if (Get_Int_Parameter("ShutdownEngineOnArrival"))
		params.ShutdownEngineOnArrival = true;
	Commands->Action_Goto(obj,params);
	params.Set_Basic(this,100,100,AI_STATE_COMBAT);
	params.AttackCheckBlocked = Get_Int_Parameter("AttackCheckBlocked") ? true : false;
	if (target && attack)
	{
		params.Set_Attack(target,weaponRange,0.0f,true);
		Commands->Action_Attack(obj,params);
	}
}
GameObject *JMG_Utility_AI_Goto_Player::findClosestStar(GameObject *obj)
{
	GameObject *nearest = NULL;
	float nearDist = -1.0f;
	Vector3 pos = Commands->Get_Position(obj);
	for (int x = 1;x < 128;x++)
	{
		GameObject *player = Get_GameObj(x);
		if (!player || ignoreStarsTime[x] || !Commands->Get_Health(player) || Commands->Get_Player_Type(player) == -4)
			continue;
		if (Is_Script_Attached(player,"JMG_Utility_AI_Goto_Player_Ignore_Object"))
			continue;
		if (!huntStealth)
		{
			GameObject *vehicle = Get_Vehicle(player);
			if (vehicle && vehicle->As_SmartGameObj() && vehicle->As_SmartGameObj()->Is_Stealthed())
				continue;
			else if (player->As_SmartGameObj() && player->As_SmartGameObj()->Is_Stealthed())
				continue;
		}
		float tempDist = JmgUtility::SimpleDistance(pos,Commands->Get_Position(player));
		if (huntSearchDistance >= 0.0f && tempDist > huntSearchDistance)
			continue;
		if (!nearest || (tempDist < nearDist))
		{
			nearest = player;
			nearDist = tempDist;
			huntingStarId = x;
		}
	}
	return nearest;
}
void JMG_Utility_AI_Goto_Player::Return_Home(GameObject *obj)
{
	Vector3 wanderPos = Vector3();
	if (Get_Int_Parameter("WanderingAIGroupID") != -1 && GetRandomPosition(&wanderPos))
	{
		state = WANDERING_GROUP;
		Attack_Move(obj,NULL,wanderPos,Get_Float_Parameter("WanderSpeed"),1.0f,false,false);
	}
	else if (Get_Int_Parameter("ReturnHome"))
	{
		state = RETURNING_HOME;
		Attack_Move(obj,NULL,homeLocation,Get_Float_Parameter("ReturnHomeSpeed"),1.0f,false,false);
	}
	else
	{
		state = IDLE;
		Attack_Move(obj,NULL,Commands->Get_Position(obj),1.0f,1.0f,false,false);
	}
}
void JMG_Utility_AI_Goto_Player::Stuck_Check(GameObject *obj,Vector3 targetPos)
{
	if (!obj->As_VehicleGameObj())
		return;
	Vector3 pos = Commands->Get_Position(obj);
	if (JmgUtility::SimpleDistance(pos,lastPosition) < 1.0f)
	{
		if (targetId && JmgUtility::SimpleDistance(pos,targetPos) <= attackArriveDistance*attackArriveDistance)
		{
			stuckTime = 0;
			return;
		}
		if (!reverseTime)
		{
			stuckTime++;
			if (stuckTime > 30)
			{
				moveBackward = true;
				Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
				reverseTime = Commands->Get_Random_Int(10,60);
				stuckTime = 0;
			}
		}
	}
	else
	{
		stuckTime = 0;
		lastPosition = pos;
	}
	if (reverseTime)
	{
		reverseTime--;
		if (!reverseTime)
		{
			moveBackward = false;
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
		}
	}
}
void JMG_Utility_AI_Goto_Player::Cant_Get_To_target(GameObject *obj)
{
	Vector3 wanderPos = Vector3();
	GameObject *star = findClosestStar(obj);
	if (star)
	{
		state = HUNTING_STAR;
		Attack_Move(obj,Commands->Find_Object(lastAction.targetId),Commands->Get_Position(star),Get_Float_Parameter("HuntSpeed"),huntArriveDistance,true,true);
	}
	else if (Get_Int_Parameter("WanderingAIGroupID") != -1 && GetRandomPosition(&wanderPos))
	{
		state = WANDERING_GROUP;
		Attack_Move(obj,Commands->Find_Object(lastAction.targetId),wanderPos,Get_Float_Parameter("WanderSpeed"),1.0f,true,true);
	}
	else
	{
		state = RETURNING_HOME;
		Attack_Move(obj,Commands->Find_Object(lastAction.targetId),homeLocation,Get_Float_Parameter("ReturnHomeSpeed"),1.0f,true,true);
	}
}
bool JMG_Utility_AI_Goto_Player::Choose_Target(GameObject *obj,GameObject *target)
{
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *currentTarget = Commands->Find_Object(targetId);
	if (!currentTarget)
		return true;
	if (JmgUtility::SimpleDistance(pos,Commands->Get_Position(target)) < JmgUtility::SimpleDistance(pos,Commands->Get_Position(currentTarget)))
		return true;
	return false;
}
AggressiveAttackSpotSystem *aggressiveAttackSpotSystemControl = NULL;
void JMG_Utility_AI_Aggressive_Attack_Spot_Control::Created(GameObject *obj)
{
	aggressiveAttackSpotSystemControl = new AggressiveAttackSpotSystem();
}
void JMG_Utility_AI_Aggressive_Attack_Spot_Control::Destroyed(GameObject *obj)
{
	if (aggressiveAttackSpotSystemControl)
	{
		AggressiveAttackSpotSystem *temp = aggressiveAttackSpotSystemControl;
		aggressiveAttackSpotSystemControl = NULL;
		temp->Empty_List();
		delete(temp);
	}
}
void JMG_Utility_AI_Aggressive_Attack_Spot_Point::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_AI_Aggressive_Attack_Spot_Point::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!aggressiveAttackSpotSystemControl)
		{
			Console_Input("msg JMG_Utility_AI_Aggressive_Attack_Spot_Point ERROR: Make sure JMG_Utility_AI_Aggressive_Attack_Spot_Control is placed on the map!");
			return;
		}
		node = aggressiveAttackSpotSystemControl->addNode(obj,Get_Int_Parameter("GroupId"),Get_Vector3_Parameter("AttackOffset"));
	}
}
void JMG_Utility_AI_Aggressive_Attack_Spot_Point::Destroyed(GameObject *obj)
{
	if (!The_Game()->Is_Game_Over() && node)
		node->alive = false;
}
void JMG_Utility_AI_Aggressive_Attack_Spot_Point2::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_AI_Aggressive_Attack_Spot_Point2::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!aggressiveAttackSpotSystemControl)
		{
			Console_Input("msg JMG_Utility_AI_Aggressive_Attack_Spot_Point2 ERROR: Make sure JMG_Utility_AI_Aggressive_Attack_Spot_Control is placed on the map!");
			return;
		}
		Vector3 mySpot = Commands->Get_Position(obj);
		Vector3 attackSpot = Get_Vector3_Parameter("AbsolutePosition");
		int objectId = Get_Int_Parameter("ObjectID");
		if (objectId)
		{
			GameObject *point = Commands->Find_Object(objectId);
			if (!point)
			{
				char errorDebug[220];
				sprintf(errorDebug,"msg JMG_Utility_AI_Aggressive_Attack_Spot_Point2 ERROR: %d Could not find the ObjectID %d!",Commands->Get_ID(obj),objectId);
				Console_Input(errorDebug);
			}
			attackSpot = Commands->Get_Position(point);
			Commands->Destroy_Object(point);
		}
		attackSpot.X -= mySpot.X;
		attackSpot.Y -= mySpot.Y;
		attackSpot.Z -= mySpot.Z;
		node = aggressiveAttackSpotSystemControl->addNode(obj,Get_Int_Parameter("GroupId"),attackSpot);
	}
}
void JMG_Utility_AI_Aggressive_Attack_Spot_Point2::Destroyed(GameObject *obj)
{
	if (!The_Game()->Is_Game_Over() && node)
		node->alive = false;
}
void JMG_Utility_AI_Aggressive_Attack_Spot::Created(GameObject *obj)
{
	state = IDLE;
	moveBackward = false;
	lastPosition = homeLocation = Commands->Get_Position(obj);
	targetId = 0;
	lastSeenTime = 0;
	stuckTime = 0;
	reverseTime = 0;
	usePrimaryFire = Get_Int_Parameter("UseSecondaryFire") ? false : true;
	const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(Get_Current_Weapon(obj),usePrimaryFire);
	if (ammo)
	{
		weaponRange = ammo->Range;
		weaponEffectiveRange = ammo->Range < ammo->EffectiveRange ? ammo->Range : ammo->EffectiveRange;
	}
	attackArriveDistance = Get_Float_Parameter("AttackDistance") >= 0 ? Get_Float_Parameter("AttackDistance") : weaponEffectiveRange;
	attackPointArriveDistance = Get_Float_Parameter("AttackPointDistance") >= 0 ? Get_Float_Parameter("AttackPointDistance") : weaponEffectiveRange;
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,1.0f,1);
	if (Get_Float_Parameter("RandomAttackPointDistance"))
		Commands->Start_Timer(obj,this,0.0f,2);
	if (Get_Float_Parameter("RandomAttackDistance"))
		Commands->Start_Timer(obj,this,0.0f,3);
	if (Get_Float_Parameter("ReactToAttackChance") == 0)
		reactToAttackChance = false;
	else if (Get_Float_Parameter("ReactToAttackChance") == 1.0f)
		reactToAttackChance = true;
	else
		Commands->Start_Timer(obj,this,0.0f,4);
}
void JMG_Utility_AI_Aggressive_Attack_Spot::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if ((state == IDLE || state == RETURNING_HOME) && (!targetId || !lastSeenTime || Choose_Target(obj,seen)))
	{
		targetId = Commands->Get_ID(seen);
		lastSeenTime = Commands->Get_Random_Int(30,60);
		state = ATTACKING_ATTACKER;
		Attack_Move(obj,seen,Vector3(),Get_Float_Parameter("AttackSpeed"),attackArriveDistance,true,false,false);
	}
	if (targetId == Commands->Get_ID(seen))
		lastSeenTime = Commands->Get_Random_Int(30,60);
}
void JMG_Utility_AI_Aggressive_Attack_Spot::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 targetPos = Commands->Get_Position(obj);
		if (lastSeenTime)
		{
			lastSeenTime--;
			if (!lastSeenTime)
			{
				targetId = 0;
				IdleChoice(obj,true);
			}
		}
		if (targetId)
		{
			GameObject *target = Commands->Find_Object(targetId);
			targetPos = Commands->Get_Position(target);
			if (!target || !Commands->Get_Health(target) || Commands->Get_Player_Type(target) == -2 || Commands->Get_Player_Type(target) == Commands->Get_Player_Type(obj))
			{
				targetId = 0;
				IdleChoice(obj,true);
			}
		}
		if (state == ATTACKING_POINT && !aggressiveAttackSpotSystemControl)
		{
			IdleChoice(obj,true);
		}
		if (aggressiveAttackSpotSystemControl && (state == IDLE || (state == ATTACKING_POINT && (!attackNode || !attackNode->alive))))
		{
			attackNode = aggressiveAttackSpotSystemControl->GetRandomNode(Get_Int_Parameter("GroupId"));
			if (!attackNode && Get_Int_Parameter("SecondaryGroupId"))
				attackNode = aggressiveAttackSpotSystemControl->GetRandomNode(Get_Int_Parameter("SecondaryGroupId"));
			if (attackNode && attackNode->alive)
			{
				state = ATTACKING_POINT;
				Attack_Move(obj,NULL,attackNode->position,Get_Float_Parameter("AttackPointSpeed"),attackPointArriveDistance,false,false,true);
			}
			else
				IdleChoice(obj,false);
		}
		if (state == RETURNING_HOME)
			if (JmgUtility::SimpleDistance(lastAction.location,Commands->Get_Position(obj)) <= (obj->As_VehicleGameObj() ? 25.0f : 1.0f))
				state = IDLE;
		if (state == ACTION_BADPATH)
			IdleChoice(obj,false);
		if (state != IDLE)
			Stuck_Check(obj,targetPos);
		Commands->Start_Timer(obj,this,0.1f,1);
	}
	if (number == 2)
	{
		attackPointArriveDistance = Get_Float_Parameter("AttackPointDistance") >= 0 ? Get_Float_Parameter("AttackPointDistance") : weaponEffectiveRange;
		attackPointArriveDistance += Commands->Get_Random(Get_Float_Parameter("RandomAttackPointDistance"),Get_Float_Parameter("RandomAttackPointDistance"));
	}
	if (number == 3)
	{
		attackArriveDistance = Get_Float_Parameter("AttackDistance") >= 0 ? Get_Float_Parameter("AttackDistance") : weaponEffectiveRange;
		attackArriveDistance += Commands->Get_Random(Get_Float_Parameter("RandomAttackDistance"),Get_Float_Parameter("RandomAttackDistance"));
	}
	if (number == 4)
	{
		if (Commands->Get_Random(0.0f,1.0f) <= Get_Float_Parameter("ReactToAttackChance"))
			reactToAttackChance = true;
		else
			reactToAttackChance = false;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("ReactToAttackRate"),4);
	}
}
void JMG_Utility_AI_Aggressive_Attack_Spot::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason)
{
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{
		if (state == ATTACKING_POINT)
		{
			if (Get_Int_Parameter("EnableDebug"))
			{
				char errorMsg[220];
				sprintf(errorMsg,"msg ERROR: Point %d is not reachable by the pathfind!",(attackNode ? attackNode->id : 0));
				Console_Input(errorMsg);
			}
			attackNode = NULL;
		}
		state = ACTION_BADPATH;
	}
}
void JMG_Utility_AI_Aggressive_Attack_Spot::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!reactToAttackChance && state == ATTACKING_POINT)
		return;
	if (damage <= 0.0f)
		return;
	if (!targetId || !lastSeenTime || Choose_Target(obj,damager))
	{
		targetId = Commands->Get_ID(damager);
		lastSeenTime = Commands->Get_Random_Int(30,60);
		state = ATTACKING_ATTACKER;
		Attack_Move(obj,damager,Vector3(),Get_Float_Parameter("AttackSpeed"),attackArriveDistance,true,false,false);
	}
}
void JMG_Utility_AI_Aggressive_Attack_Spot::Attack_Move(GameObject *obj,GameObject *target,Vector3 location,float speed,float distance,bool attack,bool overrideLocation,bool attackingPoint)
{
	lastAction.location = location;
	Commands->Action_Reset(obj,100);
	lastAction = LastAction(targetId,location,speed,distance,attack,overrideLocation,attackingPoint);
	ActionParamsStruct params;
	if (!attackingPoint && target && !overrideLocation)
	{
		params.Set_Movement(target,speed,distance,false);
		params.MoveFollow = true;
	}
	else
	{
		params.Set_Movement(location,speed,distance,false);
		params.MoveFollow = false;
	}
	params.MoveBackup = moveBackward;
	params.MovePathfind = true;
	if (Get_Int_Parameter("ShutdownEngineOnArrival"))
		params.ShutdownEngineOnArrival = true;
	Commands->Action_Goto(obj,params);
	params.Set_Basic(this,100,100,AI_STATE_COMBAT);
	if (attackingPoint)
	{
		location.X += attackNode->attackOffset.X;
		location.Y += attackNode->attackOffset.Y;
		location.Z += attackNode->attackOffset.Z;
		params.Set_Attack(location,weaponRange,0.0f,usePrimaryFire);
		params.AttackLocation = location;
		params.AttackCheckBlocked = false;
		Commands->Action_Attack(obj,params);
	}
	else if (target && attack)
	{
		params.Set_Attack(target,weaponRange,0.0f,usePrimaryFire);
		Commands->Action_Attack(obj,params);
	}
}
void JMG_Utility_AI_Aggressive_Attack_Spot::Stuck_Check(GameObject *obj,Vector3 targetPos)
{
	if (!obj->As_VehicleGameObj())
		return;
	Vector3 pos = Commands->Get_Position(obj);
	if (JmgUtility::SimpleDistance(pos,lastPosition) < 1.0f)
	{
		if (state == ATTACKING_ATTACKER && targetId && JmgUtility::SimpleDistance(pos,targetPos) <= attackArriveDistance*attackArriveDistance)
		{
			stuckTime = 0;
			return;
		}
		if (state == ATTACKING_POINT && attackNode && JmgUtility::SimpleDistance(pos,attackNode->position) <= attackPointArriveDistance*attackPointArriveDistance)
		{
			stuckTime = 0;
			return;
		}
		if (!reverseTime)
		{
			stuckTime++;
			if (stuckTime > 30)
			{
				moveBackward = true;
				Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation,lastAction.attackingPoint);
				reverseTime = Commands->Get_Random_Int(10,60);
				stuckTime = 0;
			}
		}
	}
	else
	{
		stuckTime = 0;
		lastPosition = pos;
	}
	if (reverseTime)
	{
		reverseTime--;
		if (!reverseTime)
		{
			moveBackward = false;
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation,lastAction.attackingPoint);
		}
	}
}
void JMG_Utility_AI_Aggressive_Attack_Spot::IdleChoice(GameObject *obj,bool allowAttackPoint)
{
	attackNode = aggressiveAttackSpotSystemControl ? ((!attackNode || !attackNode->alive) ? aggressiveAttackSpotSystemControl->GetRandomNode(Get_Int_Parameter("GroupId")) : attackNode) : NULL;
	if (!attackNode && Get_Int_Parameter("SecondaryGroupId"))
		attackNode = aggressiveAttackSpotSystemControl ? ((!attackNode || !attackNode->alive) ? aggressiveAttackSpotSystemControl->GetRandomNode(Get_Int_Parameter("SecondaryGroupId")) : attackNode) : NULL;
	if (allowAttackPoint && attackNode && attackNode->alive)
	{
		state = ATTACKING_POINT;
		Attack_Move(obj,NULL,attackNode->position,Get_Float_Parameter("AttackPointSpeed"),attackPointArriveDistance,false,false,true);
	}
	else if (Get_Int_Parameter("ReturnHome"))
	{
		state = RETURNING_HOME;
		Attack_Move(obj,Commands->Find_Object(lastAction.targetId),homeLocation,Get_Float_Parameter("ReturnHomeSpeed"),1.0f,true,true,false);
	}
	else
	{
		state = IDLE;
		Attack_Move(obj,NULL,Commands->Get_Position(obj),1.0f,1.0f,false,false,false);
	}
}
bool JMG_Utility_AI_Aggressive_Attack_Spot::Choose_Target(GameObject *obj,GameObject *target)
{
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *currentTarget = Commands->Find_Object(targetId);
	if (!currentTarget)
		return true;
	if (JmgUtility::SimpleDistance(pos,Commands->Get_Position(target)) < JmgUtility::SimpleDistance(pos,Commands->Get_Position(currentTarget)))
		return true;
	return false;
}
void JMG_Utility_Destroy_Objects_In_ID_Range_On_Death::Destroyed(GameObject *obj)
{
	int end = Get_Int_Parameter("EndID");
	for (int x = Get_Int_Parameter("StartID");x <= end;x++)
	{
		GameObject *destroy = Commands->Find_Object(x);
		if (destroy)
			Commands->Destroy_Object(destroy);
	}
}
void JMG_Utility_Custom_Enable_Spawners_In_Range::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		int end = Get_Int_Parameter("EndID");
		bool enable = Get_Int_Parameter("Enable") == -1 ? (param ? true : false) : (Get_Int_Parameter("Enable") ? true : false);
		for (int x = Get_Int_Parameter("StartID");x <= end;x++)
			Commands->Enable_Spawner(x,enable);
	}
}
void JMG_Utility_Send_Custom_On_Player_Count::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	id = Get_Int_Parameter("ID");
	supressMatchSpam = Get_Int_Parameter("SupressMatchSpam") ? true : false;
	playerCountParam = Get_Int_Parameter("PlayerCount");
	triggerEq = Get_Vector3_Parameter("TriggerOn[eq|<|>]");
	destroyAfterTrigger = Get_Int_Parameter("DestroyAfterTrigger") ? true : false;
	playerCount = -1;
	conditionMatching = false;
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_Send_Custom_On_Player_Count::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		int tempCount = Get_Player_Count();
		if (playerCount != tempCount)
		{
			if (triggerEq.X && tempCount == playerCountParam)
				Send_The_Message(obj,tempCount);
			else if (triggerEq.Y && tempCount < playerCountParam)
				Send_The_Message(obj,tempCount);
			else if (triggerEq.Z && tempCount > playerCountParam)
				Send_The_Message(obj,tempCount);
			else
				conditionMatching = false;
			playerCount = tempCount;
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Utility_Send_Custom_On_Player_Count::Send_The_Message(GameObject *obj,int tempCount)
{
	if (conditionMatching && supressMatchSpam)
		return;
	conditionMatching = true;
	GameObject *target = id ? Commands->Find_Object(id) : obj;
	if (target)
		Commands->Send_Custom_Event(obj,target,custom,param,delay);
	if (destroyAfterTrigger)
		this->Destroy_Script();
}
void JMG_Utility_Send_Custom_To_Preset_On_Message::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("TriggerCustom"))
	{
		Send_Custom_Event_To_Object(obj,Get_Parameter("Preset"),Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	}
}
void JMG_Utility_Basic_Spawner_Attach_Script::Created(GameObject *obj)
{
	const char *parameters = Get_Parameter("Params");
	params = newstr(parameters);
	char delim = Get_Parameter("Delim")[0];
	unsigned int length = strlen(parameters);
	for (unsigned int x = 0;x < length;x++)
		if (params[x] == delim)
			params[x] = ',';
}
void JMG_Utility_Basic_Spawner_Attach_Script::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 6873522)
	{
		Commands->Attach_Script(sender,Get_Parameter("Script"),params);
	}
}
void JMG_Utility_Send_Custom_On_Preset_Enter::Entered(GameObject *obj,GameObject *enter)
{
	if (_stricmp(Commands->Get_Preset_Name(enter),Get_Parameter("PresetName")))
		return;
	GameObject *target = NULL;
	switch (Get_Int_Parameter("ID"))
	{
	case -1:target = enter;break;
	case 0:target = obj;break;
	default:target = Commands->Find_Object(Get_Int_Parameter("ID"));break;
	}
	if (target)
		Commands->Send_Custom_Event(obj,target,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
}
void JMG_Utility_Basic_Spawner_Set_Prespawn_Location::Created(GameObject *obj)
{
	Vector3 newLoaction = Get_Vector3_Parameter("OverrideLocation");
	if (JmgUtility::SimpleDistance(newLoaction,Vector3()) <= 0.0f)
		newLoaction = Commands->Get_Position(obj);
	JMG_Utility_Basic_Spawner::preSpawnLocation = newLoaction;
}
void JMG_Utility_Basic_Spawner_Set_Prespawn_Location::Destroyed(GameObject *obj)
{
	JMG_Utility_Basic_Spawner::preSpawnLocation = Vector3();
}
void JMG_Utility_Teleport_On_Pickup::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		if (Get_Vehicle(sender))
			return;
		if (!JmgUtility::IsTruePhysicalObject(sender))
			return;
		GameObject *Object = Commands->Find_Object(Get_Int_Parameter("ObjectID"));
		Vector3 TargetPos = Get_Vector3_Parameter("Position");
		if (Object)
			TargetPos = Commands->Get_Position(Object);
		MoveablePhysClass *mphys = sender->As_PhysicalGameObj() ? sender->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
		if (mphys && mphys->Find_Teleport_Location(TargetPos,Get_Float_Parameter("MaxDistance"),&TargetPos))
		{
			Commands->Set_Position(sender,TargetPos);
			Force_Position_Update(sender);
			float facing = Get_Float_Parameter("Facing");
			if (Is_Script_Attached(sender,"JMG_Utility_Delay_Then_Rotate_Camera"))
				Remove_Script(sender,"JMG_Utility_Delay_Then_Rotate_Camera");
			if (facing != -999.0f)
			{
				char params[220];
				sprintf(params,"0.1,%.2f",facing);
				Commands->Attach_Script(sender,"JMG_Utility_Delay_Then_Rotate_Camera",params);
			}
		}
	}
}
void JMG_Utility_Zone_Set_Animation::Entered(GameObject *obj,GameObject *enter)
{
	GameObject *target = Commands->Find_Object(Get_Int_Parameter("ObjectID"));
	if (target)
		Commands->Set_Animation(target,Get_Parameter("Animation"),Get_Int_Parameter("Looping") ? true : false,0,Get_Float_Parameter("StartFrame"),Get_Float_Parameter("EndFrame"),Get_Int_Parameter("Blended") ? true : false);
	if (Get_Int_Parameter("TriggerOnce"))
		Destroy_Script();
}
void JMG_Utility_Scale_Infantry::Created(GameObject *obj)
{
	if (!obj->As_SoldierGameObj())
	{
		Destroy_Script();
		return;
	}
	obj->As_SoldierGameObj()->Set_Scale_Across_Network(Get_Float_Parameter("Scale"));
}
void JMG_Utility_Set_Innate_On_Create::Created(GameObject *obj)
{
	if (!obj->As_SoldierGameObj())
	{
		Destroy_Script();
		return;
	}
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_Set_Innate_On_Create::Timer_Expired(GameObject *obj,int number)
{
	if (Get_Int_Parameter("Enable"))
		Commands->Innate_Enable(obj);
	else
		Commands->Innate_Disable(obj);
}
void JMG_Utility_Set_Soldier_Damage_Animations::Created(GameObject *obj)
{
	if (!obj->As_SoldierGameObj())
	{
		Destroy_Script();
		return;
	}
	obj->As_SoldierGameObj()->Set_Can_Play_Damage_Animations(Get_Int_Parameter("Enable") ? true : false);
}
void JMG_Utility_Play_Music_On_Join_Enter_Change_Music::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(enter) == -4)
		return;
	if (!JMG_Utility_Play_Music_On_Join_Controller::controllerPlaced)
	{
		Console_Input("msg JMG_Utility_Play_Music_On_Join_Enter_Change_Music ERROR: You need to place JMG_Utility_Play_Music_On_Join_Controller on an object in order to use this script!");
		return;
	}
	JMG_Utility_Play_Music_On_Join_Controller::Set_Music(Get_Parameter("Music"),Get_Int_Parameter("FadeOut"),Get_Int_Parameter("FadeIn"));
	if (Get_Int_Parameter("TriggerOnce"))
		Destroy_Script();
}
void JMG_Utility_Play_Music_On_Join_Killed_Change_Music::Killed(GameObject *obj,GameObject *killer)
{
	if (!JMG_Utility_Play_Music_On_Join_Controller::controllerPlaced)
	{
		Console_Input("msg JMG_Utility_Play_Music_On_Join_Killed_Change_Music ERROR: You need to place JMG_Utility_Play_Music_On_Join_Controller on an object in order to use this script!");
		return;
	}
	JMG_Utility_Play_Music_On_Join_Controller::Set_Music(Get_Parameter("Music"),Get_Int_Parameter("FadeOut"),Get_Int_Parameter("FadeIn"));
	Destroy_Script();
}
NewObjectiveSystem BasicObjectiveSystem = NewObjectiveSystem(2,true,"mObjective","null","null");
bool JMG_Utility_Objective_System_Controller::controllerPlaced = false;
void JMG_Utility_Objective_System_Controller::Created(GameObject *obj)
{
	controllerPlaced = true;
	for (int x = 0;x < 128;x++)
		playerNames[x][0] = '\0';
	BasicObjectiveSystem = NewObjectiveSystem(Get_Int_Parameter("Team"),Get_Int_Parameter("ShowMarkersOnRadar") ? true : false,Get_Parameter("PrimaryObjectiveModel"),Get_Parameter("SecondaryObjectiveModel"),Get_Parameter("TertiaryObjectiveModel"));
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_Objective_System_Controller::Timer_Expired(GameObject *obj,int number)
{
	if (1 == number)
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (playerNames[x][0] != '\0')
			{
				if (!player)
					playerNames[x][0] = '\0';
				else if (_stricmp(playerNames[x],Get_Player_Name(player)))
				{
					BasicObjectiveSystem.Display_All_Objectives(player);
					sprintf(playerNames[x],"%s",Get_Player_Name(player));
				}
			}
			else if (player)
			{
				BasicObjectiveSystem.Display_All_Objectives(player);
				BasicObjectiveSystem.Dispaly_First_Pending_Primary_Objective_On_Hud(player);
				sprintf(playerNames[x],"%s",Get_Player_Name(player));
			}
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Utility_Objective_System_Controller::Destroyed(GameObject *obj)
{
	BasicObjectiveSystem.~NewObjectiveSystem();
	controllerPlaced = false;
}
void JMG_Utility_Objective_System_Objective_Update_Enter::Entered(GameObject *obj,GameObject *enter)
{
	if (!JMG_Utility_Objective_System_Controller::controllerPlaced)
	{
		Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Enter ERROR: Make sure you have a JMG_Utility_Objective_System_Controller placed on the map!");
		return;
	}
	if (!Commands->Is_A_Star(enter) || triggered)
		return;
	triggered = true;
	if (Get_Int_Parameter("CompleteObjectiveID"))
	{
		BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Primary,NewObjectiveSystem::Hidden,0,"",0);
		BasicObjectiveSystem.Set_Objective_Status(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Accomplished);
	}
	Commands->Send_Custom_Event(enter,obj,7531456,4562843,Get_Float_Parameter("Delay"));
}
void JMG_Utility_Objective_System_Objective_Update_Enter::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 7531456 && param == 4562843)
	{
		if (Get_Int_Parameter("NewObjectiveID"))
		{
			int objectiveMarkerId = Get_Int_Parameter("ObjectiveMarkerObjectID");
			GameObject *ObjectiveMarker = objectiveMarkerId == -1 ? sender : Commands->Find_Object(objectiveMarkerId);
			if (!ObjectiveMarker && objectiveMarkerId)
			{
				Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Enter ERROR: Could not find objective marker object!");
				return;
			}
			if (ObjectiveMarker)
				BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"),ObjectiveMarker);
			else
				BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"));
			JmgUtility::SetHUDHelpText(Get_Int_Parameter("NewObjectiveStringID"),BasicObjectiveSystem.Get_Hud_Help_Text_Color((NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority")));
		}
	}
}
void JMG_Utility_Objective_System_Objective_Update_Pickup::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		if (!JMG_Utility_Objective_System_Controller::controllerPlaced)
		{
			Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Pickup ERROR: Make sure you have a JMG_Utility_Objective_System_Controller placed on the map!");
			return;
		}
		if (!Commands->Is_A_Star(sender))
			return;
		if (Get_Int_Parameter("CompleteObjectiveID"))
		{
			BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Primary,NewObjectiveSystem::Hidden,0,"",0);
			BasicObjectiveSystem.Set_Objective_Status(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Accomplished);
		}
		int objectiveMarkerId = Get_Int_Parameter("ObjectiveMarkerObjectID");
		GameObject *ObjectiveMarker = objectiveMarkerId == -1 ? sender : Commands->Find_Object(objectiveMarkerId);
		if (!ObjectiveMarker && objectiveMarkerId)
		{
			Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Enter ERROR: Could not find objective marker object!");
			return;
		}
		if (ObjectiveMarker)
			BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"),ObjectiveMarker);
		else
			BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"));
		JmgUtility::SetHUDHelpText(Get_Int_Parameter("NewObjectiveStringID"),BasicObjectiveSystem.Get_Hud_Help_Text_Color((NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority")));
	}
}
void JMG_Utility_Objective_System_Objective_Update_Killed::Killed(GameObject *obj, GameObject *damager)
{
	if (The_Cnc_Game()->Get_Game_Duration_S() < 10)
		return;
	if (!JMG_Utility_Objective_System_Controller::controllerPlaced)
	{
		Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Killed ERROR: Make sure you have a JMG_Utility_Objective_System_Controller placed on the map!");
		return;
	}
	if (Get_Int_Parameter("CompleteObjectiveID"))
	{
		BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Primary,NewObjectiveSystem::Hidden,0,"",0);
		BasicObjectiveSystem.Set_Objective_Status(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Accomplished);
	}
	int objectiveMarkerId = Get_Int_Parameter("ObjectiveMarkerObjectID");
	GameObject *ObjectiveMarker = objectiveMarkerId == -1 ? damager : Commands->Find_Object(objectiveMarkerId);
	if (!ObjectiveMarker && objectiveMarkerId)
	{
		Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Enter ERROR: Could not find objective marker object!");
		return;
	}
	if (ObjectiveMarker)
		BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"),ObjectiveMarker);
	else
		BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"));
	JmgUtility::SetHUDHelpText(Get_Int_Parameter("NewObjectiveStringID"),BasicObjectiveSystem.Get_Hud_Help_Text_Color((NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority")));
}
void JMG_Utility_Objective_System_Objective_Update_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		if (!JMG_Utility_Objective_System_Controller::controllerPlaced)
		{
			Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Custom ERROR: Make sure you have a JMG_Utility_Objective_System_Controller placed on the map!");
			return;
		}
		if (Get_Int_Parameter("CompleteObjectiveID"))
		{
			BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Primary,NewObjectiveSystem::Hidden,0,"",0);
			BasicObjectiveSystem.Set_Objective_Status(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Accomplished);
		}
		int objectiveMarkerId = Get_Int_Parameter("ObjectiveMarkerObjectID");
		GameObject *ObjectiveMarker = objectiveMarkerId == -1 ? sender : Commands->Find_Object(objectiveMarkerId);
		if (!ObjectiveMarker && objectiveMarkerId)
		{
			Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Enter ERROR: Could not find objective marker object!");
			return;
		}
		if (ObjectiveMarker)
			BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"),ObjectiveMarker);
		else
			BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"));
		JmgUtility::SetHUDHelpText(Get_Int_Parameter("NewObjectiveStringID"),BasicObjectiveSystem.Get_Hud_Help_Text_Color((NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority")));
	}
}
void JMG_Utility_Destroyed_Apply_Damage::Destroyed(GameObject *obj)
{
	GameObject *target = Commands->Find_Object(Get_Int_Parameter("ID"));
	if (target)
		Commands->Apply_Damage(target,Get_Float_Parameter("Damage"),Get_Parameter("Warhead"),0);
}
void JMG_Utility_Scale_Damage_By_Player_Count::Created(GameObject *obj)
{
	damageRefund = 0;
	resurrectCount = 0;
	maxPlayersToScaleFor = Get_Int_Parameter("MaxPlayersToScaleFor");
	maxScaleFactor = Get_Float_Parameter("MaxScaleFactor");
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_Scale_Damage_By_Player_Count::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		damageRefund = min((float)max(Get_Player_Count()-1,0)/(float)maxPlayersToScaleFor,maxScaleFactor);
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Utility_Scale_Damage_By_Player_Count::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0.0f)
		return;
	damage *= damageRefund;
	float health = Commands->Get_Health(obj);
	if (!health)
	{
		resurrectCount++;
		if (Get_Player_Count()/4 < resurrectCount)
			return;
	}
	float maxHealth = Commands->Get_Max_Health(obj);
	health += damage;
	if (health > maxHealth)
	{
		damage = health-maxHealth;
		health = maxHealth;
		if (damage > 0)
		{
			float armor = Commands->Get_Shield_Strength(obj);
			float maxArmor = Commands->Get_Max_Shield_Strength(obj);
			armor += damage;
			if (armor > maxArmor)
				armor = maxArmor;
			Commands->Set_Shield_Strength(obj,armor);
		}
	}
	Commands->Set_Health(obj,health);
}
void JMG_Utility_Scale_Damage_Square_By_Player_Count::Created(GameObject *obj)
{
	resurrectCount = 0;
	damageRefund = 0;
	maxPlayersToScaleFor = Get_Int_Parameter("MaxPlayersToScaleFor")*Get_Int_Parameter("MaxPlayersToScaleFor");
	maxScaleFactor = Get_Float_Parameter("MaxScaleFactor");
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_Scale_Damage_Square_By_Player_Count::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		damageRefund = min((float)max(Get_Player_Count()*Get_Player_Count()-1,0)/(float)maxPlayersToScaleFor,maxScaleFactor);
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Utility_Scale_Damage_Square_By_Player_Count::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0.0f)
		return;
	damage *= damageRefund;
	float health = Commands->Get_Health(obj);
	if (!health)
	{
		resurrectCount++;
		if (Get_Player_Count()/4 < resurrectCount)
			return;
	}
	float maxHealth = Commands->Get_Max_Health(obj);
	health += damage;
	if (health > maxHealth)
	{
		damage = health-maxHealth;
		health = maxHealth;
		if (damage > 0)
		{
			float armor = Commands->Get_Shield_Strength(obj);
			float maxArmor = Commands->Get_Max_Shield_Strength(obj);
			armor += damage;
			if (armor > maxArmor)
				armor = maxArmor;
			Commands->Set_Shield_Strength(obj,armor);
		}
	}
	Commands->Set_Health(obj,health);
}
void JMG_Utility_Regen_HitPoints::Created(GameObject *obj)
{
	regenHealth = Get_Int_Parameter("RegenHealth") ? true : false;
	regenArmor = Get_Int_Parameter("RegenArmor");
	healthAmount = Get_Float_Parameter("HealthAmount");
	armorAmount = Get_Float_Parameter("ArmorAmount");
	healthPerPlayer = Get_Float_Parameter("HealthPerPlayer");
	armorPerPlayer = Get_Float_Parameter("ArmorPerPlayer");
	damageDelay = Get_Float_Parameter("DamageDelay");
	rate = Get_Float_Parameter("Rate");
	enabled = true;
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Regen_HitPoints::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (enabled)
		{
			float health = Commands->Get_Health(obj);
			float maxHealth = Commands->Get_Max_Health(obj);
			float armor = Commands->Get_Shield_Strength(obj);
			float maxArmor = Commands->Get_Max_Shield_Strength(obj);
			if (regenArmor == -1 && armor < maxArmor)
			{
				Commands->Set_Shield_Strength(obj,armor+armorAmount+armorPerPlayer*Get_Player_Count());
				if (Commands->Get_Shield_Strength(obj) > maxArmor)
					Commands->Set_Shield_Strength(obj,maxArmor);
			}
			else if (regenHealth && health < maxHealth)
			{
				Commands->Set_Health(obj,health+healthAmount+healthPerPlayer*Get_Player_Count());
				if (Commands->Get_Health(obj) > maxHealth)
					Commands->Set_Health(obj,maxHealth);
			}
			else if (regenArmor && armor < maxArmor)
			{
				Commands->Set_Shield_Strength(obj,armor+armorAmount+armorPerPlayer*Get_Player_Count());
				if (Commands->Get_Shield_Strength(obj) > maxArmor)
					Commands->Set_Shield_Strength(obj,maxArmor);
			}
		}
		Commands->Start_Timer(obj,this,rate,1);
	}
	if (number == 2)
	{
		enabled = true;
	}
}
void JMG_Utility_Regen_HitPoints::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0.0f || damageDelay <= 0.0f)
		return;
	enabled = false;
	Stop_Timer2(obj,this,2);
	Commands->Start_Timer(obj,this,damageDelay,2);
}
void JMG_Utility_Toggle_Flight_On_Delay::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
}
void JMG_Utility_Toggle_Flight_On_Delay::Timer_Expired(GameObject *obj,int number)
{
	Toggle_Fly_Mode(obj);
}
void JMG_Utility_Fainting_Soldier::Created(GameObject *obj)
{
	if (!obj->As_SoldierGameObj())
	{
		Console_Input("msg JMG_Utility_Fainting_Soldier ERROR: JMG_Utility_Fainting_Soldier can only be attached to soldier game objects!");
		Destroy_Script();
		return;
	}
	changeArmorTypeWhenKnockedOut = Get_Int_Parameter("ChangeArmorTypeWhenKnockedOut") ? true : false;
	posLockId = 0;
	fainted = false;
	sprintf(faintAnimation,"%s",Get_Parameter("FaintAnimation"));
	sprintf(layAnimation,"%s",Get_Parameter("LayAnimation"));
	sprintf(standAnimation,"%s",Get_Parameter("StandAnimation"));
	sprintf(faintSound,"%s",Get_Parameter("FaintSound"));
	sprintf(standSound,"%s",Get_Parameter("StandSound"));
	sprintf(armorTypeWhileKnockedOut,"%s",Get_Parameter("ArmorTypeWhileKnockedOut"));
	teamWhileKnockedOut = Get_Int_Parameter("TeamWhileKnockedOut");
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_Fainting_Soldier::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (fainted && Commands->Get_Shield_Strength(obj) >= Commands->Get_Max_Shield_Strength(obj))
		{
			if (changeArmorTypeWhenKnockedOut)
				Commands->Set_Shield_Type(obj,armorType);
			fainted = false;
			Commands->Set_Is_Visible(obj,true);
			if (teamWhileKnockedOut != -99)
				Commands->Set_Player_Type(obj,originalTeam);
			if (Get_Fly_Mode(obj))
				Toggle_Fly_Mode(obj);
			Unlock_Soldier_Collision_Group(obj);
			GameObject *locker = Commands->Find_Object(posLockId);
			if (locker)
				Commands->Destroy_Object(locker);
			Commands->Create_Sound(standSound,Commands->Get_Position(obj),obj);
			Commands->Set_Animation(obj,standAnimation,false,0,0.0f,-1,false);
		}
		if (!fainted && !Commands->Get_Shield_Strength(obj) && !obj->As_SoldierGameObj()->Is_Airborne())
		{
			if (changeArmorTypeWhenKnockedOut)
			{
				sprintf(armorType,"%s",Get_Shield_Type(obj));
				Commands->Set_Shield_Type(obj,armorTypeWhileKnockedOut);
			}
			Lock_Soldier_Collision_Group(obj,SOLDIER_GHOST_COLLISION_GROUP);
			GameObject *locker = Commands->Create_Object("Daves Arrow",Commands->Get_Position(obj));
			Commands->Set_Model(locker,"null");
			Commands->Set_Facing(locker,Commands->Get_Facing(obj));
			Commands->Attach_To_Object_Bone(obj,locker,"origin");
			posLockId = Commands->Get_ID(locker);
			fainted = true;
			Commands->Set_Is_Visible(obj,false);
			if (teamWhileKnockedOut != -99)
			{
				originalTeam = Commands->Get_Player_Type(obj);
				Commands->Set_Player_Type(obj,teamWhileKnockedOut);
			}
			if (!Get_Fly_Mode(obj))
				Toggle_Fly_Mode(obj);
			Commands->Start_Timer(obj,this,0.01f,2);
		}
		Commands->Start_Timer(obj,this,0.25f,1);
	}
	if (number == 2)
	{
		Commands->Create_Sound(faintSound,Commands->Get_Position(obj),obj);
		Commands->Set_Animation(obj,faintAnimation,false,0,0.0f,-1,false);
	}
}
void JMG_Utility_Fainting_Soldier::Animation_Complete(GameObject *obj,const char *animation)
{
	if (!_stricmp(animation,faintAnimation))
	{
		Commands->Set_Animation(obj,layAnimation,true,0,0.0f,-1,true);
	}
}
void JMG_Utility_Fainting_Soldier::Destroyed(GameObject *obj)
{
	GameObject *locker = Commands->Find_Object(posLockId);
	if (locker)
		Commands->Destroy_Object(locker);
}
void JMG_Utility_AI_Guardian_Infantry::Created(GameObject *obj)
{
	dpPosition = Commands->Get_Position(obj);
	EnemyID = 0;
	EnemyTimeOutTime = 0;
	LastPos = Commands->Get_Position(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Enable_Engine(obj,true);
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_AI_Guardian_Infantry::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (EnemyTimeOutTime)
		{
			EnemyTimeOutTime--;
			if (!EnemyTimeOutTime)
			{
				EnemyID = 0;
				Goto_Location(obj);
			}
		}
		if (EnemyID)
		{
			GameObject *Target = Commands->Find_Object(EnemyID);
			if (!Target || !Commands->Get_Health(Target) || Commands->Get_Player_Type(Target) == Commands->Get_Player_Type(obj))
			{
				EnemyID = 0;
				Goto_Location(obj);
			}
		}
		Vector3 Pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(Pos,LastPos) < 1.0f)
		{
			if (Get_A_Defense_Point(&dpPosition))
				Goto_Location(obj);
		}
		LastPos = Pos;
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Utility_AI_Guardian_Infantry::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (Is_Script_Attached(seen,"JMG_Utility_AI_Guardian_Ignored"))
		return;
	if (!EnemyID || !EnemyTimeOutTime)
	{
		EnemyID = Commands->Get_ID(seen);
		EnemyTimeOutTime = Commands->Get_Random_Int(2,5);
		Goto_Location(obj);
	}
	else if (EnemyID == Commands->Get_ID(seen))
		EnemyTimeOutTime = Commands->Get_Random_Int(2,5);
}
void JMG_Utility_AI_Guardian_Infantry::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0 || (EnemyID && EnemyTimeOutTime) || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	EnemyID = Commands->Get_ID(damager);
	EnemyTimeOutTime = Commands->Get_Random_Int(2,5);
	Goto_Location(obj);
}
void JMG_Utility_AI_Guardian_Infantry::Goto_Location(GameObject *obj)
{
	ActionParamsStruct params;
	Commands->Action_Reset(obj,100);
	Vector3 GoToPos = dpPosition;
	params.Set_Movement(GoToPos,Get_Float_Parameter("WanderSpeed"),1.0f,false);
	params.MovePathfind = true;
	params.Set_Basic(this,100,10);
	GameObject *Target = Commands->Find_Object(EnemyID);
	if (Target)
	{
		if (Get_Int_Parameter("FaceTarget"))
			params.Set_Face_Location(Commands->Get_Position(Target),5.0f);
		else
			params.AttackFaceTarget = false;
		params.Set_Attack(Target,Get_Float_Parameter("FireRange"),0,true);
		params.AttackCheckBlocked = Get_Int_Parameter("CheckBlocked") ? true : false;
		Commands->Action_Attack(obj,params);
	}
	else
	{
		if (Get_Int_Parameter("FaceTarget"))
			params.Set_Face_Location(GoToPos,5.0f);
		else
			params.AttackFaceTarget = false;
		Commands->Action_Goto(obj,params);
	}
}
void JMG_Utility_Set_Innate_Max_Wander_Distance::Created(GameObject *obj)
{
	Commands->Set_Innate_Soldier_Home_Location(obj,Commands->Get_Position(obj),Get_Float_Parameter("Distance"));
}
void JMG_Utility_Switch_Weapon_To_Empty_Hands_Until_Custom::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	sprintf(weaponName,"%s",Get_Current_Weapon(obj));
	Commands->Select_Weapon(obj,"");
}
void JMG_Utility_Switch_Weapon_To_Empty_Hands_Until_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Select_Weapon(obj,weaponName);
	}
}
void JMG_Utility_Set_Skin_And_Shield_Type_On_Custom::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Set_Skin_And_Shield_Type_On_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Set_Skin(obj,Get_Parameter("SkinType"));
		Commands->Set_Shield_Type(obj,Get_Parameter("ShieldType"));
	}
}
void JMG_Utility_Zone_Damage_All_In_ID_Range_On_Enter::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(enter) == -4)
		return;
	int end = Get_Int_Parameter("EndID");
	float damage = Get_Float_Parameter("Damage");
	const char *warhead = Get_Parameter("Warhead");
	int damager = Get_Int_Parameter("Damager");
	GameObject *theDamager = NULL;
	if (damager == 1)
		theDamager = enter;
	if (damager == 2)
		theDamager = obj;
	for (int x = Get_Int_Parameter("StartID");x <= end;x++)
	{
		GameObject *object = Commands->Find_Object(x);
		if (!object)
			continue;
		Commands->Apply_Damage(object,damage,warhead,theDamager);
	}
	if (Get_Int_Parameter("TriggerOnce"))
		Destroy_Script();
}
void JMG_Utility_Zone_Send_Custom_To_All_With_Script::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(enter) == -4)
		return;
	int custom = Get_Int_Parameter("Custom");
	int param = Get_Int_Parameter("Param");
	float delay = Get_Float_Parameter("Delay");
	const char *theScript = Get_Parameter("AttachedScript");
	for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
	{
		SmartGameObj* o = current->Data();
		if (o && Is_Script_Attached(o,theScript))
			Commands->Send_Custom_Event(enter,o,custom,param,delay);
	}
	if (Get_Int_Parameter("TriggerOnce"))
		Destroy_Script();
}
void JMG_Utility_HitPoints_In_Range_Change_Model::Created(GameObject *obj)
{
	modelSet = false;
	upperHP = Get_Float_Parameter("UpperHitPointsPercent")*0.01f;
	lowerHP = Get_Float_Parameter("LowerHitPointsPercent")*0.01f;
	Commands->Start_Timer(obj,this,0.25f,1);
}
void JMG_Utility_HitPoints_In_Range_Change_Model::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float hp = Get_Hitpoints(obj)/Get_Max_Hitpoints(obj);
		if (hp <= upperHP && hp > lowerHP)
		{
			if (!modelSet)
			{
				modelSet = true;
				Commands->Set_Model(obj,Get_Parameter("Model"));
			}
		}
		else if (modelSet)
			modelSet = false;
		Commands->Start_Timer(obj,this,0.25f,1);
	}
}
void JMG_Utility_HitPoints_In_Range_Attach_Preset::Created(GameObject *obj)
{
	presetId = 0;
	upperHP = Get_Float_Parameter("UpperHitPointsPercent")*0.01f;
	lowerHP = Get_Float_Parameter("LowerHitPointsPercent")*0.01f;
	Commands->Start_Timer(obj,this,0.25f,1);
}
void JMG_Utility_HitPoints_In_Range_Attach_Preset::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1 && Commands->Get_Health(obj))
	{
		float hp = Get_Hitpoints(obj)/Get_Max_Hitpoints(obj);
		if (hp <= upperHP && hp > lowerHP)
		{
			if (!presetId)
			{
				GameObject *turret = Commands->Create_Object(Get_Parameter("Preset"),Commands->Get_Position(obj));
				Commands->Attach_To_Object_Bone(turret,obj,Get_Parameter("BoneName"));
				presetId = Commands->Get_ID(turret);
			}
		}
		else if (presetId)
		{
			GameObject *turret = Commands->Find_Object(presetId);
			Commands->Destroy_Object(turret);
			presetId = 0;
		}
		Commands->Start_Timer(obj,this,0.25f,1);
	}
}
void JMG_Utility_HitPoints_In_Range_Attach_Preset::Killed(GameObject *obj,GameObject *killer)
{
	Destroyed(obj);
}
void JMG_Utility_HitPoints_In_Range_Attach_Preset::Destroyed(GameObject *obj)
{
	GameObject *turret = Commands->Find_Object(presetId);
	Commands->Destroy_Object(turret);
}
void JMG_Utility_HitPoints_In_Range_Enable_Spawner::Created(GameObject *obj)
{
	enabled = false;
	upperHP = Get_Float_Parameter("UpperHitPointsPercent")*0.01f;
	lowerHP = Get_Float_Parameter("LowerHitPointsPercent")*0.01f;
	Commands->Start_Timer(obj,this,0.25f,1);
}
void JMG_Utility_HitPoints_In_Range_Enable_Spawner::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1 && Commands->Get_Health(obj))
	{
		float hp = Get_Hitpoints(obj)/Get_Max_Hitpoints(obj);
		if (hp <= upperHP && hp > lowerHP)
		{
			if (!enabled)
			{
				enabled = true;
				Commands->Enable_Spawner(Get_Int_Parameter("SpawnerID"),true);
			}
		}
		else if (enabled)
		{
			enabled = false;
			Commands->Enable_Spawner(Get_Int_Parameter("SpawnerID"),false);
		}
		Commands->Start_Timer(obj,this,0.25f,1);
	}
}
void JMG_Utility_HitPoints_In_Range_Create_Preset::Created(GameObject *obj)
{
	presetId = 0;
	upperHP = Get_Float_Parameter("UpperHitPointsPercent")*0.01f;
	lowerHP = Get_Float_Parameter("LowerHitPointsPercent")*0.01f;
	Commands->Start_Timer(obj,this,0.25f,1);
}
void JMG_Utility_HitPoints_In_Range_Create_Preset::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1 && Commands->Get_Health(obj))
	{
		float hp = Get_Hitpoints(obj)/Get_Max_Hitpoints(obj);
		if (hp <= upperHP && hp > lowerHP)
		{
			if (!presetId)
			{
				GameObject *turret = Commands->Create_Object(Get_Parameter("Preset"),Get_Vector3_Parameter("Location"));
				Commands->Set_Facing(turret,Get_Float_Parameter("Facing"));
				presetId = Commands->Get_ID(turret);
			}
		}
		else if (presetId)
		{
			GameObject *turret = Commands->Find_Object(presetId);
			Commands->Destroy_Object(turret);
			presetId = 0;
		}
		Commands->Start_Timer(obj,this,0.25f,1);
	}
}
void JMG_Utility_HitPoints_In_Range_Create_Preset::Killed(GameObject *obj,GameObject *killer)
{
	Destroyed(obj);
}
void JMG_Utility_HitPoints_In_Range_Create_Preset::Destroyed(GameObject *obj)
{
	GameObject *turret = Commands->Find_Object(presetId);
	Commands->Destroy_Object(turret);
}
void JMG_Utility_HitPoints_In_Range_Send_Custom::Created(GameObject *obj)
{
	inRange = false;
	upperHP = Get_Float_Parameter("UpperHitPointsPercent")*0.01f;
	lowerHP = Get_Float_Parameter("LowerHitPointsPercent")*0.01f;
	Commands->Start_Timer(obj,this,0.25f,1);
}
void JMG_Utility_HitPoints_In_Range_Send_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1 && Commands->Get_Health(obj))
	{
		float hp = Get_Hitpoints(obj)/Get_Max_Hitpoints(obj);
		if (hp <= upperHP && hp > lowerHP)
		{
			if (!inRange)
			{
				int id = Get_Int_Parameter("ID");
				GameObject *object = id ? Commands->Find_Object(id) : obj;
				if (object)
					Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
				inRange = true;
			}
		}
		else if (inRange)
			inRange = false;
		Commands->Start_Timer(obj,this,0.25f,1);
	}
}
void JMG_Utility_Custom_Set_Weather_Precipitation::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Set_Weather_Precipitation::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		float transitionTime = Get_Float_Parameter("TransitionTime");
		Vector3 changeRSA = Get_Vector3_Parameter("Change[Rain|Snow|Ash|]");
		Vector3 setRSA = Get_Vector3_Parameter("Set[Rain|Snow|Ash|]");
		if (changeRSA.X)
			Commands->Set_Rain(setRSA.X,transitionTime,true);
		if (changeRSA.Y)
			Commands->Set_Snow(setRSA.Y,transitionTime,true);
		if (changeRSA.Z)
			Commands->Set_Ash(setRSA.Z,transitionTime,true);
	}
}
void JMG_Utility_Custom_Set_Weather_Fog::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Set_Weather_Fog::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Set_Fog_Range(Get_Float_Parameter("StartDistance"),Get_Float_Parameter("EndDistance"),Get_Float_Parameter("TransitionTime"));
	}
}
void JMG_Utility_Custom_Set_Weather_Wind::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Set_Weather_Wind::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Set_Wind(Get_Float_Parameter("Heading"),Get_Float_Parameter("Speed"),Get_Float_Parameter("Variability"),Get_Float_Parameter("TransitionTime"));
	}
}
void JMG_Utility_Custom_Set_Weather_Lightning::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Set_Weather_Lightning::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Set_Lightning(Get_Float_Parameter("Intensity"),Get_Float_Parameter("StartDistance"),Get_Float_Parameter("EndDistance"),Get_Float_Parameter("Heading"),Get_Float_Parameter("Distribution"),Get_Float_Parameter("TransitionTime"));
	}
}
void JMG_Utility_Custom_Set_Weather_War_Blitz::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Set_Weather_War_Blitz::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Set_War_Blitz(Get_Float_Parameter("Intensity"),Get_Float_Parameter("StartDistance"),Get_Float_Parameter("EndDistance"),Get_Float_Parameter("Heading"),Get_Float_Parameter("Distribution"),Get_Float_Parameter("TransitionTime"));
	}
}
void JMG_Utility_Custom_Set_Weather_Clouds::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Set_Weather_Clouds::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Set_Clouds(Get_Float_Parameter("Cover"),Get_Float_Parameter("Gloominess"),Get_Float_Parameter("TransitionTime"));
	}
}
void JMG_Utility_Play_Music_On_Join_Custom_Change_Music::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,1.0,1);
}
void JMG_Utility_Play_Music_On_Join_Custom_Change_Music::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		custom = Get_Int_Parameter("Custom");
		if (!JMG_Utility_Play_Music_On_Join_Controller::controllerPlaced)
		{
			Console_Input("msg JMG_Utility_Play_Music_On_Join_Custom_Change_Music ERROR: You need to place JMG_Utility_Play_Music_On_Join_Controller on an object in order to use this script!");
			Destroy_Script();
			return;
		}
	}
}
void JMG_Utility_Play_Music_On_Join_Custom_Change_Music::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		JMG_Utility_Play_Music_On_Join_Controller::Set_Music(Get_Parameter("Music"),Get_Int_Parameter("FadeOut"),Get_Int_Parameter("FadeIn"));
		if (!Get_Int_Parameter("Repeat"))
			Destroy_Script();
	}
}
void JMG_Utility_Zone_Send_Custom_If_Has_Weapon::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(enter) == -4)
		return;
	if (!Has_Weapon(enter,Get_Parameter("WeaponName")))
		return;
	int id = Get_Int_Parameter("ID");
	GameObject *object = id ? (id == -1 ? enter : Commands->Find_Object(id)) : obj;
	if (object)
		Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	if (Get_Int_Parameter("RemoveWeapon"))
		Remove_Weapon(enter,Get_Parameter("WeaponName"));
	if (Get_Int_Parameter("TriggerOnce"))
		Destroy_Script();
}
void JMG_Utility_Receive_All_Customs_To_Send_Custom::Created(GameObject *obj)
{
	resetCustom = Get_Int_Parameter("ResetCustom");
	for (int x = 0;x < 10;x++)
	{
		char param[20];
		sprintf(param,"Custom%0d",x);
		customs[x] = Get_Int_Parameter(param);
		receivedCustoms[x] = customs[x] ? false : true;
	}
}
void JMG_Utility_Receive_All_Customs_To_Send_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == resetCustom)
	{
		for (int x = 0;x < 10;x++)
			receivedCustoms[x] = customs[x] ? false : true;
	}
	for (int x = 0;x < 10;x++)
		if (message == customs[x])
		{
			receivedCustoms[x] = true;
			for (int y = 0;y < 10;y++)
				if (!receivedCustoms[y])
					return;
			int id = Get_Int_Parameter("ID");
			GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
			if (object)
				Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
			if (Get_Int_Parameter("TriggerOnce"))
				Destroy_Script();
			break;
		}
}
void JMG_Utility_Zone_Teleport_To_Random_Wander_Point::Created(GameObject *obj)
{
	retryOnFailure = Get_Int_Parameter("RetryOnFailure") ? true : false;
	changeGroupIDCustom = Get_Int_Parameter("ChangeGroupIDCustom");
	playerType = Get_Int_Parameter("PlayerType");
	safeTeleportDistance = Get_Float_Parameter("SafeTeleportDistance");
	wanderPointGroup = Get_Int_Parameter("WanderingAIGroupID");
}
void JMG_Utility_Zone_Teleport_To_Random_Wander_Point::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == changeGroupIDCustom)
	{
		wanderPointGroup = param;
	}
}
void JMG_Utility_Zone_Teleport_To_Random_Wander_Point::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,playerType) || Commands->Get_Player_Type(enter) == -4)
		return;
	Grab_Teleport_Spot(enter,5);
}
bool JMG_Utility_Zone_Teleport_To_Random_Wander_Point::Grab_Teleport_Spot(GameObject *enter,int attempts)
{
	if (The_Game()->Get_Game_Duration_S() < 1.0f)
	{
		{
			char params[220];
			sprintf(params,"%d,%.2f",wanderPointGroup,safeTeleportDistance);
			Commands->Attach_Script(enter,"JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Attach",params);
		}
		return false;
	}
	if (!attempts)
	{
		if (retryOnFailure)
		{
			char params[220];
			sprintf(params,"%d,%.2f",wanderPointGroup,safeTeleportDistance);
			Commands->Attach_Script(enter,"JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Attach",params);
		}
		return false;
	}
	attempts--;
	Vector3 targetPos;
	float facing;
	if (!Get_A_Defense_Point(&targetPos,&facing))
	{
		char errormsg[220];
		sprintf(errormsg,"msg JMG_Utility_Zone_Teleport_To_Random_Wander_Point ERROR: No wander points found for group %d!",wanderPointGroup);
		Console_Input(errormsg);
		return false;
	}
	MoveablePhysClass *mphys = enter->As_PhysicalGameObj() ? enter->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
	if (mphys && mphys->Find_Teleport_Location(targetPos,safeTeleportDistance,&targetPos))
	{
		Commands->Set_Position(enter,targetPos);
		Force_Position_Update(enter);
		char params[220];
		sprintf(params,"0.1,%.2f",facing);
		Commands->Attach_Script(enter,"JMG_Utility_Delay_Then_Rotate_Camera",params);
		return true;
	}
	else
	{
		Commands->Set_Position(enter,targetPos);
		return Grab_Teleport_Spot(enter,attempts);
	}
}
void JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Attach::Created(GameObject *obj)
{
	safeTeleportDistance = Get_Float_Parameter("SafeTeleportDistance");
	wanderPointGroup = Get_Int_Parameter("WanderingAIGroupID");
	Commands->Start_Timer(obj,this,0.25f,1);
}
void JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Attach::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 targetPos;
		float facing;
		if (!Get_A_Defense_Point(&targetPos,&facing))
			return;
		MoveablePhysClass *mphys = obj->As_PhysicalGameObj() ? obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
		if (mphys && mphys->Find_Teleport_Location(targetPos,safeTeleportDistance,&targetPos))
		{
			Commands->Set_Position(obj,targetPos);
			Force_Position_Update(obj);
			char params[220];
			sprintf(params,"0.1,%.2f",facing);
			Commands->Attach_Script(obj,"JMG_Utility_Delay_Then_Rotate_Camera",params);
		}
		else
		{
			Commands->Start_Timer(obj,this,0.25f,1);
			return;
		}
		Destroy_Script();
	}
}
void JMG_Utility_Zone_Set_Player_Type::Created(GameObject *obj)
{
	requiredPlayerType = Get_Int_Parameter("RequiredPlayerType");
	setPlayerType = Get_Int_Parameter("SetPlayerType");
}
void JMG_Utility_Zone_Set_Player_Type::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,requiredPlayerType) || Commands->Get_Player_Type(enter) == -4)
		return;
	Commands->Set_Player_Type(enter,setPlayerType);
}
void JMG_Utility_Zone_Send_Custom_Enter::Created(GameObject *obj)
{
	playerType = Get_Int_Parameter("PlayerType");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("Custom");
	param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	triggerOnce = Get_Int_Parameter("TriggerOnce") ? true : false;
}
void JMG_Utility_Zone_Send_Custom_Enter::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,playerType) || Commands->Get_Player_Type(enter) == -4)
		return;
	GameObject *object = id ? (id == -1 ? enter : Commands->Find_Object(id)) : obj;
	if (object)
		Commands->Send_Custom_Event(obj,object,custom,param,delay);
	if (triggerOnce)
		Destroy_Script();
}
void JMG_Utility_Custom_Destroy_Closest_Object_To_Position::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Destroy_Closest_Object_To_Position::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		GameObject *nearest = NULL;
		float dist = 0.0f;
		Vector3 pos = Get_Vector3_Parameter("Position");
		const char *presetName = Get_Parameter("PresetName");
		float maxDist = Get_Float_Parameter("MaxDistance");
		maxDist *= maxDist;
		for (SLNode<BaseGameObj> *current = GameObjManager::GameObjList.Head();current;current = current->Next())
		{
			GameObject* o = current->Data()->As_ScriptableGameObj();
			if (!_stricmp(presetName,Commands->Get_Preset_Name(o)))
			{
				float tempDistance = JmgUtility::SimpleDistance(pos,Commands->Get_Position(o));
				if (!nearest || (tempDistance < dist && (maxDist == 0.0 || tempDistance <= maxDist)))
				{
					nearest = o;
					dist = tempDistance;
				}
			}
		}
		if (nearest)
			Commands->Destroy_Object(nearest);
	}
}
void JMG_Utility_Zone_Set_Spawner::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(enter) == -4)
		return;
	Commands->Enable_Spawner(Get_Int_Parameter("SpawnerID"),Get_Int_Parameter("Enable") ? true : false);
	if (Get_Int_Parameter("TriggerOnce"))
		Destroy_Script();
}
void JMG_Utility_Persistant_Weapon_Powerup::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		const char *powerupNameOverride = Get_Parameter("PowerupNameOverride");
		if (!_stricmp(powerupNameOverride,"null"))
			powerupNameOverride = Commands->Get_Preset_Name(obj);
		char params[256];
		sprintf(params,"%s,%s,%d",powerupNameOverride,Get_Parameter("WeaponName"),Get_Int_Parameter("DisarmCustom"));
		Commands->Attach_Script(sender,"JMG_Utility_Persistant_Weapon_zAttached",params);
	}
}
void JMG_Utility_Persistant_Weapon_zStandin::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_Persistant_Weapon_zStandin::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 myPos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(Commands->Get_Position(Commands->Get_A_Star(myPos)),myPos) >= 9.0f)
		{
			myPos.Z += 0.25f;
			GameObject *statue = Commands->Create_Object(Get_Parameter("PowerupName"),myPos);
			Commands->Set_Facing(statue,Commands->Get_Facing(obj));
			Commands->Destroy_Object(obj);
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Utility_Persistant_Weapon_zAttached::Created(GameObject *obj)
{
	disarmCustom = Get_Int_Parameter("DisarmCustom");
	sprintf(weaponName,"%s",Get_Parameter("WeaponName"));
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Persistant_Weapon_zAttached::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!Has_Weapon(obj,weaponName))
		{
			Destroyed(obj);
			return;
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Persistant_Weapon_zAttached::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == disarmCustom)
	{
		if (Has_Weapon(obj,weaponName))
			Remove_Weapon(obj,weaponName);
		Destroy_Script();
	}
}
void JMG_Utility_Persistant_Weapon_zAttached::Destroyed(GameObject *obj)
{
	GameObject *standin = Commands->Create_Object("Daves Arrow",Commands->Get_Position(obj));
	Commands->Set_Model(standin,"null");
	Commands->Set_Facing(standin,Commands->Get_Facing(obj));
	char params[256];
	sprintf(params,"%s",Get_Parameter("PowerupName"));
	Commands->Attach_Script(standin,"JMG_Utility_Persistant_Weapon_zStandin",params);
	Destroy_Script();
}
void JMG_Utility_Custom_Teleport_Players_Outside_Range_To_Wanderpoints::Created(GameObject *obj)
{
	scanTeleportSpot = Get_Vector3_Parameter("ScanTeleportSpot");
	teleportIgnoreDistance = Get_Float_Parameter("TeleportIgnoreDistance");
	teleportIgnoreDistance *= teleportIgnoreDistance;
	safeTeleportDistance = Get_Float_Parameter("SafeTeleportDistance");
	wanderPointGroup = Get_Int_Parameter("WanderingAIGroupID");
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Teleport_Players_Outside_Range_To_Wanderpoints::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player)
				continue;
			if (JmgUtility::SimpleDistance(scanTeleportSpot,Commands->Get_Position(player)) <= teleportIgnoreDistance)
				continue;
			Grab_Teleport_Spot(player,5);
		}
	}
}
bool JMG_Utility_Custom_Teleport_Players_Outside_Range_To_Wanderpoints::Grab_Teleport_Spot(GameObject *player,int attempts)
{
	if (!attempts)
	{
		char params[220];
		sprintf(params,"%d,%.2f",wanderPointGroup,safeTeleportDistance);
		Commands->Attach_Script(player,"JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Attach",params);
		return false;
	}
	attempts--;
	Vector3 targetPos;
	float facing;
	if (!Get_A_Defense_Point(&targetPos,&facing))
	{
		char errormsg[220];
		sprintf(errormsg,"msg JMG_Utility_Custom_Teleport_Players_Outside_Range_To_Wanderpoints ERROR: No wander points found for group %d!",wanderPointGroup);
		Console_Input(errormsg);
		return false;
	}
	MoveablePhysClass *mphys = player->As_PhysicalGameObj() ? player->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
	if (mphys && mphys->Find_Teleport_Location(targetPos,safeTeleportDistance,&targetPos))
	{
		Commands->Set_Position(player,targetPos);
		Force_Position_Update(player);
		char params[220];
		sprintf(params,"0.1,%.2f",facing);
		Commands->Attach_Script(player,"JMG_Utility_Delay_Then_Rotate_Camera",params);
		return true;
	}
	else
	{
		Commands->Set_Position(player,targetPos);
		return Grab_Teleport_Spot(player,attempts);
	}
}
void JMG_Utility_Custom_Set_Tile_Frame::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Set_Tile_Frame::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Static_Anim_Phys_Goto_Frame(Get_Int_Parameter("ID"),Get_Float_Parameter("Frame"),Get_Parameter("Animation"));
	}
}
void JMG_Utility_Zone_Send_Custom_No_Weapon::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(enter) == -4)
		return;
	if (Has_Weapon(enter,Get_Parameter("WeaponName")))
		return;
	int id = Get_Int_Parameter("ID");
	GameObject *object = id ? (id == -1 ? enter : Commands->Find_Object(id)) : obj;
	if (object)
		Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	if (Get_Int_Parameter("TriggerOnce"))
		Destroy_Script();
}
void JMG_Utility_Custom_Display_Briefing_Message::Created(GameObject *obj)
{
	triggered = false;
	BriefingText = NULL;
	CurrentNode = NULL;
	int File = Commands->Text_File_Open(Get_Parameter("FileName"));
	if (!File)
	{
		Console_Input("msg JMG_RP2_Mutant_Assault_Briefing_Custom_Message ERROR: Text File not found!");
		return;
	}
	bool NextLineIsTarget = false;
	char TextString[16384];
	while (Commands->Text_File_Get_String(File,TextString,16383))
	{
		if (!_stricmp(TextString,""))
			continue;
		if (!NextLineIsTarget)
			for (int x = 0;x < 16384;x++)
				if (TextString[x] == '\n' || TextString[x] == 13 || TextString[x] == '\0')
					TextString[x] = '\0';
		if (NextLineIsTarget)
		{
			int Count = 0;
			for (int x = 0;x < 16384;x++)
			{
				if ((Count > 150 && TextString[x] == ' ') || Count == 210)
				{// String getting too long, start a new one.
					CurrentNode->Text[Count] = '\0';
					AddNewTextNode();
					Count = 0;
				}
				else if (TextString[x] == '\n' || TextString[x] == '\0')
				{
					CurrentNode->Text[Count] = '\0';
					break;// end of line your done
				}
				else if (TextString[x] == '\f')
				{
					CurrentNode->Text[Count] = '\0';
					AddNewTextNode();
					Count = 0;
				}
				else
				{
					CurrentNode->Text[Count] = TextString[x];
					Count++;
				}
			}
			break;
		}
		else if (!_stricmp(TextString,Get_Parameter("TextTag")))
		{
			NextLineIsTarget = true;
			AddNewTextNode();
		}
		TextString[0] = '\0';
	}
	Commands->Text_File_Close(File);
	CurrentNode = BriefingText;
	Commands->Start_Timer(obj,this,0.1f,56345343);
}
void JMG_Utility_Custom_Display_Briefing_Message::Timer_Expired(GameObject *obj,int number)
{
	if (number == 56345344)
	{
		if (!CurrentNode)
		{
			RemoveTextNodes();
			Destroy_Script();
			return;
		}
		if (CurrentNode->Text[0] != '\0')
			for (int x = 1;x < 128;x++)
			{
				GameObject *Player = Get_GameObj(x);
				if (Player)
					JmgUtility::DisplayChatMessage(Player,(int)Get_Vector3_Parameter("MessageColor").X,(int)Get_Vector3_Parameter("MessageColor").Y,(int)Get_Vector3_Parameter("MessageColor").Z,CurrentNode->Text);
			}
		CurrentNode = CurrentNode->next;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("LineDelayTime"),56345344);
	}
}
void JMG_Utility_Custom_Display_Briefing_Message::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		if (triggered)
			return;
		triggered = true;
		Commands->Start_Timer(obj,this,0.0f,56345344);
		Commands->Create_2D_Sound(Get_Parameter("UISound"));
	}
}
void JMG_Utility_Custom_Display_Briefing_Message::Destroyed(GameObject *obj)
{
	RemoveTextNodes();
}
void JMG_Utility_Custom_Display_Briefing_Message::AddNewTextNode()
{
	if (!BriefingText)
	{
		BriefingText = new BriefingTextNode;
		sprintf(BriefingText->Text,"\0");
		BriefingText->next = NULL;
		CurrentNode = BriefingText;
		return;
	}
	BriefingTextNode *Current = BriefingText;
	while (Current)
	{
		if (!Current->next)
		{
			Current->next = new BriefingTextNode;
			Current = Current->next;
			sprintf(Current->Text,"\0");
			Current->next = NULL;
			CurrentNode = Current;
			return;
		}
		Current = Current->next;
	}
}
void JMG_Utility_Custom_Display_Briefing_Message::RemoveTextNodes()
{
	BriefingTextNode *Current = BriefingText,*Temp;
	while (Current)
	{
		Temp = Current;
		Current = Current->next;
		delete Temp;
	}
	BriefingText = NULL;
	CurrentNode = NULL;
}
void JMG_Utility_Zone_Set_Player_Team::Created(GameObject *obj)
{
	requiredPlayerTeam = Get_Int_Parameter("RequiredPlayerTeam");
	setPlayerTeam = Get_Int_Parameter("SetPlayerTeam");
}
void JMG_Utility_Zone_Set_Player_Team::Entered(GameObject *obj,GameObject *enter)
{
	int playerId = JmgUtility::JMG_Get_Player_ID(enter);
	if (!playerId || (requiredPlayerTeam != 2 && requiredPlayerTeam != Get_Player_Team(playerId)))
		return;
	Change_Team_2(enter,setPlayerTeam);
}
void JMG_Utility_Objective_System_Objective_Update_Create::Created(GameObject *obj)
{
	if (!JMG_Utility_Objective_System_Controller::controllerPlaced)
	{
		Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Create ERROR: Make sure you have a JMG_Utility_Objective_System_Controller placed on the map!");
		return;
	}
	if (triggered)
		return;
	triggered = true;
	if (Get_Int_Parameter("CompleteObjectiveID"))
	{
		BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Primary,NewObjectiveSystem::Hidden,0,"",0);
		BasicObjectiveSystem.Set_Objective_Status(Get_Int_Parameter("CompleteObjectiveID"),NewObjectiveSystem::Accomplished);
	}
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
}
void JMG_Utility_Objective_System_Objective_Update_Create::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (Get_Int_Parameter("NewObjectiveID"))
		{
			int objectiveMarkerId = Get_Int_Parameter("ObjectiveMarkerObjectID");
			GameObject *ObjectiveMarker = objectiveMarkerId == -1 ? obj : Commands->Find_Object(objectiveMarkerId);
			if (!ObjectiveMarker && objectiveMarkerId)
			{
				Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Enter ERROR: Could not find objective marker object!");
				return;
			}
			if (ObjectiveMarker)
				BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"),ObjectiveMarker);
			else
				BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("NewObjectiveID"),(NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority"),NewObjectiveSystem::Pending,Get_Int_Parameter("NewObjectiveStringID"),"",Get_Int_Parameter("NewObjectiveStringID"));
			JmgUtility::SetHUDHelpText(Get_Int_Parameter("NewObjectiveStringID"),BasicObjectiveSystem.Get_Hud_Help_Text_Color((NewObjectiveSystem::Priority)Get_Int_Parameter("NewObjectivePriority")));
		}
	}
}
void JMG_Utility_Objective_System_Objective_Remove_Custom::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Objective_System_Objective_Remove_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		BasicObjectiveSystem.Remove_Objective(Get_Int_Parameter("ObjectiveID"));
	}
}
void JMG_Utility_Delay_Then_Rotate_Camera::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
}
void JMG_Utility_Delay_Then_Rotate_Camera::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 pos = Commands->Get_Bone_Position(obj,"c head");
		float facing = Get_Float_Parameter("Facing");
		pos.X += 100.0f*(cos(facing*PI180));
		pos.Y += 100.0f*(sin(facing*PI180));
		Force_Camera_Look_Player(obj,pos);
	}
	Destroy_Script();
}
void JMG_Utility_Damage_SmartGameObjects_In_Range::Created(GameObject *obj)
{
	range = Get_Float_Parameter("Range");
	range *= range;
	damage = Get_Float_Parameter("Damage");
	sprintf(warhead,"%s",Get_Parameter("Warhead"));
	rate = Get_Float_Parameter("Rate");
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Damage_SmartGameObjects_In_Range::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 mypos = Commands->Get_Position(obj);
		for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
		{
			SmartGameObj* o = current->Data();
			if (o != obj && JmgUtility::SimpleDistance(Commands->Get_Position(o),mypos) <= range)
				Commands->Apply_Damage(o,damage,warhead,obj);
		}
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Unstick_Infantry_If_Stuck::Created(GameObject *obj)
{
	distance = Get_Float_Parameter("Distance");
	rate = Get_Float_Parameter("Rate");
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Unstick_Infantry_If_Stuck::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 myPos = Commands->Get_Position(obj);
		MoveablePhysClass *mphys = obj->As_PhysicalGameObj() ? obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
		if (mphys && !mphys->Can_Teleport(Matrix3D(myPos)))
		{
			if (mphys->Find_Teleport_Location(myPos,distance,&myPos))
				Commands->Set_Position(obj,myPos);
		}
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Custom_Send_Custom_On_Count::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	resetCustom = Get_Int_Parameter("ResetCustom");
	count = Get_Int_Parameter("Count");
}
void JMG_Utility_Custom_Send_Custom_On_Count::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == resetCustom)
	{
		count = Get_Int_Parameter("Count");
	}
	if (message == custom && count)
	{
		count--;
		if (count)
			return;
		int id = Get_Int_Parameter("ID");
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
		if (object)
			Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("SendCustom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	}
}
void JMG_Utility_Custom_Destroy_Self::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Destroy_Self::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Destroy_Object(obj);
	}
}
void JMG_Utility_Zone_Set_Player_Team2::Created(GameObject *obj)
{
	requiredPlayerTeam = Get_Int_Parameter("RequiredPlayerTeam");
	setPlayerTeam = Get_Int_Parameter("SetPlayerTeam");
}
void JMG_Utility_Zone_Set_Player_Team2::Entered(GameObject *obj,GameObject *enter)
{
	int playerId = JmgUtility::JMG_Get_Player_ID(enter);
	if (!playerId || (requiredPlayerTeam != 2 && requiredPlayerTeam != Get_Player_Team(playerId)))
		return;
	Change_Team_2(enter,setPlayerTeam);
	cPlayer *thePlayer = Find_Player(playerId);
	thePlayer->Set_Deaths(thePlayer->Get_Deaths()-1);
	Commands->Destroy_Object(enter);
}
void JMG_Utility_Poke_Send_Custom::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void JMG_Utility_Poke_Send_Custom::Poked(GameObject *obj, GameObject *poker)
{
	if (CheckPlayerType(poker,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(poker) == -4)
		return;
	int id = Get_Int_Parameter("ID");
	GameObject *object = id ? (id == -1 ? poker : Commands->Find_Object(id)) : obj;
	if (object)
		Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	if (Get_Int_Parameter("TriggerOnce"))
	{
		Commands->Enable_HUD_Pokable_Indicator(obj,false);
		Destroy_Script();
	}
}
void JMG_Utility_Set_Collision_Group::Created(GameObject *obj)
{
	if (!obj->As_SoldierGameObj())
		obj->As_PhysicalGameObj()->Peek_Physical_Object()->Set_Collision_Group((Collision_Group_Type)Get_Int_Parameter("CollisionGroupID"));
	else
		Lock_Soldier_Collision_Group(obj,(Collision_Group_Type)Get_Int_Parameter("CollisionGroupID"));
}
void JMG_Utility_Cap_Credits::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	team = Get_Int_Parameter("Team");
	credits = Get_Float_Parameter("Credits");
	Commands->Start_Timer(obj,this,0.01f,1);
}
void JMG_Utility_Cap_Credits::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player || CheckPlayerType(player,team))
				continue;
			if (Get_Money(x) > credits)
				Set_Money(x,credits);
		}
		Commands->Start_Timer(obj,this,0.01f,1);
	}
}
void JMG_Utility_Cap_Credits::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		credits = (float)param;
	}
}
void JMG_Utility_Custom_Apply_Damage::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Apply_Damage::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		int id = Get_Int_Parameter("ID");
		GameObject *object = id ? Commands->Find_Object(id) : obj;
		int damagerId = Get_Int_Parameter("DamagerID");
		GameObject *damager = damagerId > 0 ? Commands->Find_Object(damagerId) : (damagerId == -1 ? sender : (damagerId == -2 ? obj : object));
		if (object)
			Commands->Apply_Damage(object,Get_Float_Parameter("Damage"),Get_Parameter("Warhead"),damager);
	}
}
void JMG_Utility_AI_Goto_Enemy::Created(GameObject *obj)
{
	state = IDLE;
	moveBackward = false;
	lastPosition = homeLocation = Commands->Get_Position(obj);
	targetId = 0;
	lastSeenTime = 0;
	huntingEnemyId = 0;
	stuckTime = 0;
	reverseTime = 0;
	huntStealth = Get_Int_Parameter("HuntStealth") ? true : false;
	removeIgnoreTime = 0;
	ignoreEnemyId = 0;
	huntSearchDistance = Get_Float_Parameter("HuntSearchDistance") < 0.0f ? Get_Float_Parameter("HuntSearchDistance") : Get_Float_Parameter("HuntSearchDistance")*Get_Float_Parameter("HuntSearchDistance");
	const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(Get_Current_Weapon(obj),true);
	if (ammo)
	{
		weaponRange = ammo->Range;
		weaponEffectiveRange = ammo->Range < ammo->EffectiveRange ? ammo->Range : ammo->EffectiveRange;
	}
	attackArriveDistance = Get_Float_Parameter("AttackDistance") >= 0 ? Get_Float_Parameter("AttackDistance") : weaponEffectiveRange;
	huntArriveDistance = Get_Float_Parameter("HuntArriveDistance");
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,1.0f,1);
	Commands->Start_Timer(obj,this,1.0f,2);
	if (Get_Float_Parameter("RandomAttackDistance"))
		Commands->Start_Timer(obj,this,0.0f,3);
	if (Get_Float_Parameter("RandomHuntArriveDistance"))
		Commands->Start_Timer(obj,this,0.0f,4);
}
void JMG_Utility_AI_Goto_Enemy::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (Is_Script_Attached(seen,"JMG_Utility_AI_Goto_Enemy_Ignore_Object"))
		return;
	if (!_stricmp(Get_Skin(seen),"Blamo") || !_stricmp(Get_Shield_Type(seen),"Blamo"))
		return;
	if (Get_Int_Parameter("CanSeeStealth") != 2 && seen->As_SmartGameObj() && seen->As_SmartGameObj()->Is_Stealthed())
	{
		if (!Get_Int_Parameter("CanSeeStealth"))
			return;
		float dist = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen));
		if (seen->As_SoldierGameObj() && dist > seen->As_SoldierGameObj()->Get_Stealth_Fade_Distance()*seen->As_SoldierGameObj()->Get_Stealth_Fade_Distance())
			return;
		else if (seen->As_VehicleGameObj() && dist > seen->As_VehicleGameObj()->Get_Stealth_Fade_Distance()*seen->As_VehicleGameObj()->Get_Stealth_Fade_Distance())
			return;	
		else if (dist > seen->As_SmartGameObj()->Get_Stealth_Fade_Distance()*seen->As_SmartGameObj()->Get_Stealth_Fade_Distance())
			return;
	}
	if (!targetId || !lastSeenTime || Choose_Target(obj,seen))
	{
		targetId = Commands->Get_ID(seen);
		lastSeenTime = Commands->Get_Random_Int(30,60);
		state = ATTACKING_TARGET;
		Attack_Move(obj,seen,Vector3(),Get_Float_Parameter("AttackSpeed"),attackArriveDistance,true,false);
	}
	if (targetId == Commands->Get_ID(seen))
		lastSeenTime = Commands->Get_Random_Int(30,60);
}
void JMG_Utility_AI_Goto_Enemy::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 targetPos = Commands->Get_Position(obj);
		if (lastSeenTime)
		{
			lastSeenTime--;
			if (!lastSeenTime)
			{
				targetId = 0;
				Return_Home(obj);
			}
		}
		if (targetId)
		{
			GameObject *target = Commands->Find_Object(targetId);
			targetPos = Commands->Get_Position(target);
			if (!target || !Commands->Get_Health(target) || Commands->Get_Player_Type(target) == -2 || (Commands->Get_Player_Type(obj) != -1 && Commands->Get_Player_Type(target) == Commands->Get_Player_Type(obj)))
			{
				targetId = 0;
				Return_Home(obj);
			}
		}
		if (state == HUNTING_STAR)
		{
			GameObject *star = Commands->Find_Object(huntingEnemyId);
			if (!star || !Commands->Get_Health(star))
				Return_Home(obj);
		}
		if (state == IDLE || state == RETURNING_HOME || state == WANDERING_GROUP)
		{
			GameObject *star = findClosestStar(obj);
			if (star)
			{
				state = HUNTING_STAR;
				Attack_Move(obj,star,Vector3(),Get_Float_Parameter("HuntSpeed"),huntArriveDistance,false,false);
			}
		}
		if (state == HUNTING_STAR)
		{
			GameObject *star = Commands->Find_Object(huntingEnemyId);
			if (!star || (huntSearchDistance >= 0.0f && JmgUtility::SimpleDistance(Commands->Get_Position(star),Commands->Get_Position(obj)) > huntSearchDistance))
				Return_Home(obj);
		}
		if (state == IDLE && Get_Int_Parameter("WanderingAIGroupID") != -1)
		{
			Vector3 wanderPos = Vector3();
			if (GetRandomPosition(&wanderPos))
			{
				state = WANDERING_GROUP;
				Attack_Move(obj,NULL,wanderPos,Get_Float_Parameter("WanderSpeed"),1.0f,false,false);
			}
		}
		if (state == RETURNING_HOME || state == WANDERING_GROUP)
			if (JmgUtility::SimpleDistance(lastAction.location,Commands->Get_Position(obj)) <= (obj->As_VehicleGameObj() ? 25.0f : 1.0f))
				state = IDLE;
		if (state == ACTION_BADPATH)
			Cant_Get_To_target(obj);
		if (state != IDLE)
			Stuck_Check(obj,targetPos);
		Commands->Start_Timer(obj,this,0.1f,1);
	}
	if (number == 2)
	{
		if (removeIgnoreTime)
		{
			removeIgnoreTime--;
			if (!removeIgnoreTime)
				ignoreEnemyId = 0;
		}
		Commands->Start_Timer(obj,this,1.0f,2);
	}
	if (number == 3)
	{
		attackArriveDistance = Get_Float_Parameter("AttackDistance") >= 0 ? Get_Float_Parameter("AttackDistance") : weaponEffectiveRange;
		attackArriveDistance += Commands->Get_Random(Get_Float_Parameter("RandomAttackDistance"),Get_Float_Parameter("RandomAttackDistance"));
	}
	if (number == 4)
	{
		huntArriveDistance = Get_Float_Parameter("HuntArriveDistance");
		huntArriveDistance += Commands->Get_Random(Get_Float_Parameter("RandomHuntArriveDistance"),Get_Float_Parameter("RandomHuntArriveDistance"));
	}
}
void JMG_Utility_AI_Goto_Enemy::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason)
{
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{
		if (state == HUNTING_STAR)
		{
			ignoreEnemyId = huntingEnemyId;
			removeIgnoreTime = 60;
			state = ACTION_BADPATH;
		}
		else if (state == ATTACKING_TARGET)
			state = ACTION_BADPATH;
	}
}
void JMG_Utility_AI_Goto_Enemy::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0.0f || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	if (!_stricmp(Get_Skin(damager),"Blamo") || !_stricmp(Get_Shield_Type(damager),"Blamo"))
		return;
	if (!targetId || !lastSeenTime || Choose_Target(obj,damager))
	{
		targetId = Commands->Get_ID(damager);
		lastSeenTime = Commands->Get_Random_Int(30,60);
		state = ATTACKING_TARGET;
		Attack_Move(obj,damager,Vector3(),Get_Float_Parameter("AttackSpeed"),attackArriveDistance,true,false);
	}
}
void JMG_Utility_AI_Goto_Enemy::Attack_Move(GameObject *obj,GameObject *target,Vector3 location,float speed,float distance,bool attack,bool overrideLocation)
{
	lastAction.location = location;
	Commands->Action_Reset(obj,100);
	lastAction = LastAction(targetId,location,speed,distance,attack,overrideLocation);
	ActionParamsStruct params;
	params.ObserverID = this->Get_ID();
	if (target && !overrideLocation)
	{
		params.Set_Movement(target,speed,distance,false);
		params.MoveFollow = true;
	}
	else
	{
		params.Set_Movement(location,speed,distance,false);
		params.MoveFollow = false;
	}
	params.MoveBackup = moveBackward;
	params.MovePathfind = true;
	if (Get_Int_Parameter("ShutdownEngineOnArrival"))
		params.ShutdownEngineOnArrival = true;
	Commands->Action_Goto(obj,params);
	params.Set_Basic(this,100,100,AI_STATE_COMBAT);
	params.AttackCheckBlocked = Get_Int_Parameter("AttackCheckBlocked") ? true : false;
	if (target && attack)
	{
		params.Set_Attack(target,weaponRange,0.0f,true);
		Commands->Action_Attack(obj,params);
	}
}
GameObject *JMG_Utility_AI_Goto_Enemy::findClosestStar(GameObject *obj)
{
	if (Commands->Get_Player_Type(obj) == -2)
		return NULL;
	GameObject *nearest = NULL;
	float nearDist = -1.0f;
	Vector3 pos = Commands->Get_Position(obj);
	int myPlayerType = Commands->Get_Player_Type(obj);
	for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
	{
		SmartGameObj* o = current->Data();
		int oPlayerType = Commands->Get_Player_Type(o);
		int enemyId = Commands->Get_ID(o);
		if (Is_Script_Attached(o,"JMG_Utility_AI_Goto_Enemy_Ignore_Object"))
			continue;
		if (!(o->As_SoldierGameObj() || o->As_VehicleGameObj()))
			continue;
		if (!Commands->Get_Health(o) || oPlayerType == -4 || oPlayerType == -2 || oPlayerType == myPlayerType || enemyId == ignoreEnemyId)
			continue;
		if (!_stricmp(Get_Skin(o),"Blamo") || !_stricmp(Get_Shield_Type(o),"Blamo"))
			continue;
		if (!huntStealth)
		{
			GameObject *vehicle = Get_Vehicle(o);
			if (vehicle && vehicle->As_SmartGameObj() && vehicle->As_SmartGameObj()->Is_Stealthed())
				continue;
			else if (o->As_SmartGameObj() && o->As_SmartGameObj()->Is_Stealthed())
				continue;
		}
		float tempDist = JmgUtility::SimpleDistance(pos,Commands->Get_Position(o));
		if (huntSearchDistance >= 0.0f && tempDist > huntSearchDistance)
			continue;
		if (!nearest || (tempDist < nearDist))
		{
			nearest = o;
			nearDist = tempDist;
			huntingEnemyId = enemyId;
		}
	}
	return nearest;
}
void JMG_Utility_AI_Goto_Enemy::Return_Home(GameObject *obj)
{
	Vector3 wanderPos = Vector3();
	if (Get_Int_Parameter("WanderingAIGroupID") != -1 && GetRandomPosition(&wanderPos))
	{
		state = WANDERING_GROUP;
		Attack_Move(obj,NULL,wanderPos,Get_Float_Parameter("WanderSpeed"),1.0f,false,false);
	}
	else if (Get_Int_Parameter("ReturnHome"))
	{
		state = RETURNING_HOME;
		Attack_Move(obj,NULL,homeLocation,Get_Float_Parameter("ReturnHomeSpeed"),1.0f,false,false);
	}
	else
	{
		state = IDLE;
		Attack_Move(obj,NULL,Commands->Get_Position(obj),1.0f,1.0f,false,false);
	}
}
void JMG_Utility_AI_Goto_Enemy::Stuck_Check(GameObject *obj,Vector3 targetPos)
{
	if (!obj->As_VehicleGameObj())
		return;
	Vector3 pos = Commands->Get_Position(obj);
	if (JmgUtility::SimpleDistance(pos,lastPosition) < 1.0f)
	{
		if (targetId && JmgUtility::SimpleDistance(pos,targetPos) <= attackArriveDistance*attackArriveDistance)
		{
			stuckTime = 0;
			return;
		}
		if (!reverseTime)
		{
			stuckTime++;
			if (stuckTime > 30)
			{
				moveBackward = true;
				Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
				reverseTime = Commands->Get_Random_Int(10,60);
				stuckTime = 0;
			}
		}
	}
	else
	{
		stuckTime = 0;
		lastPosition = pos;
	}
	if (reverseTime)
	{
		reverseTime--;
		if (!reverseTime)
		{
			moveBackward = false;
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
		}
	}
}
void JMG_Utility_AI_Goto_Enemy::Cant_Get_To_target(GameObject *obj)
{
	Vector3 wanderPos = Vector3();
	GameObject *star = findClosestStar(obj);
	if (star)
	{
		state = HUNTING_STAR;
		Attack_Move(obj,Commands->Find_Object(lastAction.targetId),Commands->Get_Position(star),Get_Float_Parameter("HuntSpeed"),huntArriveDistance,true,true);
	}
	else if (Get_Int_Parameter("WanderingAIGroupID") != -1 && GetRandomPosition(&wanderPos))
	{
		state = WANDERING_GROUP;
		Attack_Move(obj,Commands->Find_Object(lastAction.targetId),wanderPos,Get_Float_Parameter("WanderSpeed"),1.0f,true,true);
	}
	else
	{
		state = RETURNING_HOME;
		Attack_Move(obj,Commands->Find_Object(lastAction.targetId),homeLocation,Get_Float_Parameter("ReturnHomeSpeed"),1.0f,true,true);
	}
}
bool JMG_Utility_AI_Goto_Enemy::Choose_Target(GameObject *obj,GameObject *target)
{
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *currentTarget = Commands->Find_Object(targetId);
	if (!currentTarget)
		return true;
	if (JmgUtility::SimpleDistance(pos,Commands->Get_Position(target)) < JmgUtility::SimpleDistance(pos,Commands->Get_Position(currentTarget)))
		return true;
	return false;
}
void JMG_Utility_AI_Goto_Enemy_Not_Star::Created(GameObject *obj)
{
	state = IDLE;
	moveBackward = false;
	lastPosition = homeLocation = Commands->Get_Position(obj);
	targetId = 0;
	lastSeenTime = 0;
	huntingEnemyId = 0;
	stuckTime = 0;
	reverseTime = 0;
	huntStealth = Get_Int_Parameter("HuntStealth") ? true : false;
	removeIgnoreTime = 0;
	ignoreEnemyId = 0;
	huntSearchDistance = Get_Float_Parameter("HuntSearchDistance") < 0.0f ? Get_Float_Parameter("HuntSearchDistance") : Get_Float_Parameter("HuntSearchDistance")*Get_Float_Parameter("HuntSearchDistance");
	const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(Get_Current_Weapon(obj),true);
	if (ammo)
	{
		weaponRange = ammo->Range;
		weaponEffectiveRange = ammo->Range < ammo->EffectiveRange ? ammo->Range : ammo->EffectiveRange;
	}
	attackArriveDistance = Get_Float_Parameter("AttackDistance") >= 0 ? Get_Float_Parameter("AttackDistance") : weaponEffectiveRange;
	huntArriveDistance = Get_Float_Parameter("HuntArriveDistance");
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,1.0f,1);
	Commands->Start_Timer(obj,this,1.0f,2);
	if (Get_Float_Parameter("RandomAttackDistance"))
		Commands->Start_Timer(obj,this,0.0f,3);
	if (Get_Float_Parameter("RandomHuntArriveDistance"))
		Commands->Start_Timer(obj,this,0.0f,4);
}
void JMG_Utility_AI_Goto_Enemy_Not_Star::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (Is_Script_Attached(seen,"JMG_Utility_AI_Goto_Enemy_Not_Star_Ignore_Object"))
		return;
	if (!_stricmp(Get_Skin(seen),"Blamo") || !_stricmp(Get_Shield_Type(seen),"Blamo"))
		return;
	if (Get_Int_Parameter("CanSeeStealth") != 2 && seen->As_SmartGameObj() && seen->As_SmartGameObj()->Is_Stealthed())
	{
		if (!Get_Int_Parameter("CanSeeStealth"))
			return;
		float dist = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen));
		if (seen->As_SoldierGameObj() && dist > seen->As_SoldierGameObj()->Get_Stealth_Fade_Distance()*seen->As_SoldierGameObj()->Get_Stealth_Fade_Distance())
			return;
		else if (seen->As_VehicleGameObj() && dist > seen->As_VehicleGameObj()->Get_Stealth_Fade_Distance()*seen->As_VehicleGameObj()->Get_Stealth_Fade_Distance())
			return;	
		else if (dist > seen->As_SmartGameObj()->Get_Stealth_Fade_Distance()*seen->As_SmartGameObj()->Get_Stealth_Fade_Distance())
			return;
	}
	if (!targetId || !lastSeenTime || Choose_Target(obj,seen))
	{
		targetId = Commands->Get_ID(seen);
		lastSeenTime = Commands->Get_Random_Int(30,60);
		state = ATTACKING_TARGET;
		Attack_Move(obj,seen,Vector3(),Get_Float_Parameter("AttackSpeed"),attackArriveDistance,true,false);
	}
	if (targetId == Commands->Get_ID(seen))
		lastSeenTime = Commands->Get_Random_Int(30,60);
}
void JMG_Utility_AI_Goto_Enemy_Not_Star::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 targetPos = Commands->Get_Position(obj);
		if (lastSeenTime)
		{
			lastSeenTime--;
			if (!lastSeenTime)
			{
				targetId = 0;
				Return_Home(obj);
			}
		}
		if (targetId)
		{
			GameObject *target = Commands->Find_Object(targetId);
			targetPos = Commands->Get_Position(target);
			if (!target || !Commands->Get_Health(target) || Commands->Get_Player_Type(target) == -2 || (Commands->Get_Player_Type(obj) != -1 && Commands->Get_Player_Type(target) == Commands->Get_Player_Type(obj)))
			{
				targetId = 0;
				Return_Home(obj);
			}
		}
		if (state == HUNTING_STAR)
		{
			GameObject *star = Commands->Find_Object(huntingEnemyId);
			if (!star || !Commands->Get_Health(star))
				Return_Home(obj);
		}
		if (state == IDLE || state == RETURNING_HOME || state == WANDERING_GROUP)
		{
			GameObject *star = findClosestStar(obj);
			if (star)
			{
				state = HUNTING_STAR;
				Attack_Move(obj,star,Vector3(),Get_Float_Parameter("HuntSpeed"),huntArriveDistance,false,false);
			}
		}
		if (state == HUNTING_STAR)
		{
			GameObject *star = Commands->Find_Object(huntingEnemyId);
			if (!star || (huntSearchDistance >= 0.0f && JmgUtility::SimpleDistance(Commands->Get_Position(star),Commands->Get_Position(obj)) > huntSearchDistance))
				Return_Home(obj);
		}
		if (state == IDLE && Get_Int_Parameter("WanderingAIGroupID") != -1)
		{
			Vector3 wanderPos = Vector3();
			if (GetRandomPosition(&wanderPos))
			{
				state = WANDERING_GROUP;
				Attack_Move(obj,NULL,wanderPos,Get_Float_Parameter("WanderSpeed"),1.0f,false,false);
			}
		}
		if (state == RETURNING_HOME || state == WANDERING_GROUP)
			if (JmgUtility::SimpleDistance(lastAction.location,Commands->Get_Position(obj)) <= (obj->As_VehicleGameObj() ? 25.0f : 1.0f))
				state = IDLE;
		if (state == ACTION_BADPATH)
			Cant_Get_To_target(obj);
		if (state != IDLE)
			Stuck_Check(obj,targetPos);
		Commands->Start_Timer(obj,this,0.1f,1);
	}
	if (number == 2)
	{
		if (removeIgnoreTime)
		{
			removeIgnoreTime--;
			if (!removeIgnoreTime)
				ignoreEnemyId = 0;
		}
		Commands->Start_Timer(obj,this,1.0f,2);
	}
	if (number == 3)
	{
		attackArriveDistance = Get_Float_Parameter("AttackDistance") >= 0 ? Get_Float_Parameter("AttackDistance") : weaponEffectiveRange;
		attackArriveDistance += Commands->Get_Random(Get_Float_Parameter("RandomAttackDistance"),Get_Float_Parameter("RandomAttackDistance"));
	}
	if (number == 4)
	{
		huntArriveDistance = Get_Float_Parameter("HuntArriveDistance");
		huntArriveDistance += Commands->Get_Random(Get_Float_Parameter("RandomHuntArriveDistance"),Get_Float_Parameter("RandomHuntArriveDistance"));
	}
}
void JMG_Utility_AI_Goto_Enemy_Not_Star::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason)
{
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{
		if (state == HUNTING_STAR)
		{
			ignoreEnemyId = huntingEnemyId;
			removeIgnoreTime = 60;
			state = ACTION_BADPATH;
		}
		else if (state == ATTACKING_TARGET)
			state = ACTION_BADPATH;
	}
}
void JMG_Utility_AI_Goto_Enemy_Not_Star::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0.0f || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	if (!_stricmp(Get_Skin(damager),"Blamo") || !_stricmp(Get_Shield_Type(damager),"Blamo"))
		return;
	if (!targetId || !lastSeenTime || Choose_Target(obj,damager))
	{
		targetId = Commands->Get_ID(damager);
		lastSeenTime = Commands->Get_Random_Int(30,60);
		state = ATTACKING_TARGET;
		Attack_Move(obj,damager,Vector3(),Get_Float_Parameter("AttackSpeed"),attackArriveDistance,true,false);
	}
}
void JMG_Utility_AI_Goto_Enemy_Not_Star::Attack_Move(GameObject *obj,GameObject *target,Vector3 location,float speed,float distance,bool attack,bool overrideLocation)
{
	lastAction.location = location;
	Commands->Action_Reset(obj,100);
	lastAction = LastAction(targetId,location,speed,distance,attack,overrideLocation);
	ActionParamsStruct params;
	params.ObserverID = this->Get_ID();
	if (target && !overrideLocation)
	{
		params.Set_Movement(target,speed,distance,false);
		params.MoveFollow = true;
	}
	else
	{
		params.Set_Movement(location,speed,distance,false);
		params.MoveFollow = false;
	}
	params.MoveBackup = moveBackward;
	params.MovePathfind = true;
	if (Get_Int_Parameter("ShutdownEngineOnArrival"))
		params.ShutdownEngineOnArrival = true;
	Commands->Action_Goto(obj,params);
	params.Set_Basic(this,100,100,AI_STATE_COMBAT);
	params.AttackCheckBlocked = Get_Int_Parameter("AttackCheckBlocked") ? true : false;
	if (target && attack)
	{
		params.Set_Attack(target,weaponRange,0.0f,true);
		Commands->Action_Attack(obj,params);
	}
}
GameObject *JMG_Utility_AI_Goto_Enemy_Not_Star::findClosestStar(GameObject *obj)
{
	if (Commands->Get_Player_Type(obj) == -2)
		return NULL;
	GameObject *nearest = NULL;
	float nearDist = -1.0f;
	Vector3 pos = Commands->Get_Position(obj);
	int myPlayerType = Commands->Get_Player_Type(obj);
	for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
	{
		SmartGameObj* o = current->Data();
		int oPlayerType = Commands->Get_Player_Type(o);
		int enemyId = Commands->Get_ID(o);
		if (Is_Script_Attached(o,"JMG_Utility_AI_Goto_Enemy_Not_Star_Ignore_Object"))
			continue;
		if (!(o->As_SoldierGameObj() || o->As_VehicleGameObj()))
			continue;
		if (!Commands->Get_Health(o) || Commands->Is_A_Star(o) || oPlayerType == -4 || oPlayerType == -2 || oPlayerType == myPlayerType || enemyId == ignoreEnemyId)
			continue;
		if (!_stricmp(Get_Skin(o),"Blamo") || !_stricmp(Get_Shield_Type(o),"Blamo"))
			continue;
		if (!huntStealth)
		{
			GameObject *vehicle = Get_Vehicle(o);
			if (vehicle && vehicle->As_SmartGameObj() && vehicle->As_SmartGameObj()->Is_Stealthed())
				continue;
			else if (o->As_SmartGameObj() && o->As_SmartGameObj()->Is_Stealthed())
				continue;
		}
		float tempDist = JmgUtility::SimpleDistance(pos,Commands->Get_Position(o));
		if (huntSearchDistance >= 0.0f && tempDist > huntSearchDistance)
			continue;
		if (!nearest || (tempDist < nearDist))
		{
			nearest = o;
			nearDist = tempDist;
			huntingEnemyId = enemyId;
		}
	}
	return nearest;
}
void JMG_Utility_AI_Goto_Enemy_Not_Star::Return_Home(GameObject *obj)
{
	Vector3 wanderPos = Vector3();
	if (Get_Int_Parameter("WanderingAIGroupID") != -1 && GetRandomPosition(&wanderPos))
	{
		state = WANDERING_GROUP;
		Attack_Move(obj,NULL,wanderPos,Get_Float_Parameter("WanderSpeed"),1.0f,false,false);
	}
	else if (Get_Int_Parameter("ReturnHome"))
	{
		state = RETURNING_HOME;
		Attack_Move(obj,NULL,homeLocation,Get_Float_Parameter("ReturnHomeSpeed"),1.0f,false,false);
	}
	else
	{
		state = IDLE;
		Attack_Move(obj,NULL,Commands->Get_Position(obj),1.0f,1.0f,false,false);
	}
}
void JMG_Utility_AI_Goto_Enemy_Not_Star::Stuck_Check(GameObject *obj,Vector3 targetPos)
{
	if (!obj->As_VehicleGameObj())
		return;
	Vector3 pos = Commands->Get_Position(obj);
	if (JmgUtility::SimpleDistance(pos,lastPosition) < 1.0f)
	{
		if (targetId && JmgUtility::SimpleDistance(pos,targetPos) <= attackArriveDistance*attackArriveDistance)
		{
			stuckTime = 0;
			return;
		}
		if (!reverseTime)
		{
			stuckTime++;
			if (stuckTime > 30)
			{
				moveBackward = true;
				Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
				reverseTime = Commands->Get_Random_Int(10,60);
				stuckTime = 0;
			}
		}
	}
	else
	{
		stuckTime = 0;
		lastPosition = pos;
	}
	if (reverseTime)
	{
		reverseTime--;
		if (!reverseTime)
		{
			moveBackward = false;
			Attack_Move(obj,Commands->Find_Object(lastAction.targetId),lastAction.location,lastAction.speed,lastAction.distance,lastAction.attack,lastAction.overrideLocation);
		}
	}
}
void JMG_Utility_AI_Goto_Enemy_Not_Star::Cant_Get_To_target(GameObject *obj)
{
	Vector3 wanderPos = Vector3();
	GameObject *star = findClosestStar(obj);
	if (star)
	{
		state = HUNTING_STAR;
		Attack_Move(obj,Commands->Find_Object(lastAction.targetId),Commands->Get_Position(star),Get_Float_Parameter("HuntSpeed"),huntArriveDistance,true,true);
	}
	else if (Get_Int_Parameter("WanderingAIGroupID") != -1 && GetRandomPosition(&wanderPos))
	{
		state = WANDERING_GROUP;
		Attack_Move(obj,Commands->Find_Object(lastAction.targetId),wanderPos,Get_Float_Parameter("WanderSpeed"),1.0f,true,true);
	}
	else
	{
		state = RETURNING_HOME;
		Attack_Move(obj,Commands->Find_Object(lastAction.targetId),homeLocation,Get_Float_Parameter("ReturnHomeSpeed"),1.0f,true,true);
	}
}
bool JMG_Utility_AI_Goto_Enemy_Not_Star::Choose_Target(GameObject *obj,GameObject *target)
{
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *currentTarget = Commands->Find_Object(targetId);
	if (!currentTarget)
		return true;
	if (JmgUtility::SimpleDistance(pos,Commands->Get_Position(target)) < JmgUtility::SimpleDistance(pos,Commands->Get_Position(currentTarget)))
		return true;
	return false;
}
void JMG_Utility_Grant_Key_On_Create::Created(GameObject *obj)
{
	Commands->Grant_Key(obj,Get_Int_Parameter("Key"),Get_Int_Parameter("Grant") ? true : false);
}
void JMG_Utility_Custom_Send_Custom::Created(GameObject *obj)
{
	recieveMessage = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Send_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == recieveMessage)
	{
		int id = Get_Int_Parameter("ID");
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
		Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("SendCustom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	}
}
void JMG_Utility_Damage_Unoccupied_Vehicle::Created(GameObject *obj)
{
	hasBeenOccupied = false;
	rate = Get_Float_Parameter("Rate");
	damage = Get_Float_Parameter("Damage");
	sprintf(warhead,"%s",Get_Parameter("Warhead"));
	maxDelay = delay = Get_Float_Parameter("Delay");
	decreaseTick = Get_Float_Parameter("DecreaseTick");
	increaseTick = Get_Float_Parameter("IncreaseTick");
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Damage_Unoccupied_Vehicle::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (hasBeenOccupied)
		{
			if (Get_Vehicle_Occupant_Count(obj) && delay < maxDelay)
			{
				delay += increaseTick;
				if (delay > maxDelay)
					delay = maxDelay;
			}
			else
			{
				if (delay > 0)
				{
					delay -=decreaseTick;
					if (delay <= 0)
						delay = 0;
				}
				else
					Commands->Apply_Damage(obj,damage,warhead,obj);
			}

		}
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Damage_Unoccupied_Vehicle::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		hasBeenOccupied = true;
	}
}
void JMG_Utility_Custom_Damage_All_Soldiers_On_Team::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	team = Get_Int_Parameter("Team");
	damage = Get_Float_Parameter("Damage");
	sprintf(warhead,"%s",Get_Parameter("Warhead"));
}
void JMG_Utility_Custom_Damage_All_Soldiers_On_Team::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
		{
			SmartGameObj* o = current->Data();
			if (o->As_SoldierGameObj() && !CheckPlayerType(o,team))
				Commands->Apply_Damage(o,damage,warhead,o);
		}
	}
}
void JMG_Utility_AI_Guardian_Vehicle::Created(GameObject *obj)
{
	aimAtFeet = Get_Int_Parameter("AimAtFeet") ? true : false;
	dpPosition = Commands->Get_Position(obj);
	EnemyID = 0;
	EnemyTimeOutTime = 0;
	LastPos = Commands->Get_Position(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Enable_Engine(obj,true);
	Commands->Start_Timer(obj,this,1.0f,1);
	if (aimAtFeet)
		Commands->Start_Timer(obj,this,0.25f,2);
}
void JMG_Utility_AI_Guardian_Vehicle::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (EnemyTimeOutTime)
		{
			EnemyTimeOutTime--;
			if (!EnemyTimeOutTime)
			{
				EnemyID = 0;
				Goto_Location(obj);
			}
		}
		if (EnemyID)
		{
			GameObject *Target = Commands->Find_Object(EnemyID);
			if (!Target || !Commands->Get_Health(Target) || Commands->Get_Player_Type(Target) == Commands->Get_Player_Type(obj))
			{
				EnemyID = 0;
				Goto_Location(obj);
			}
		}
		Vector3 Pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(Pos,LastPos) < 1.0f)
		{
			if (Get_A_Defense_Point(&dpPosition))
				Goto_Location(obj);
		}
		LastPos = Pos;
		Commands->Start_Timer(obj,this,1.0f,1);
	}
	if (number == 2)
	{
		GameObject *Target = Commands->Find_Object(EnemyID);
		if (Target && Commands->Get_Health(Target) && Target->As_SoldierGameObj())
		{
			ActionParamsStruct params;
			Commands->Get_Action_Params(obj,params);
			params.AttackCheckBlocked = false;
			params.Set_Attack(Commands->Get_Position(Target),Get_Float_Parameter("FireRange"),0,true);
			Commands->Action_Attack(obj,params);
		}
		Commands->Start_Timer(obj,this,0.25f,2);
	}
}
void JMG_Utility_AI_Guardian_Vehicle::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (Is_Script_Attached(seen,"JMG_Utility_AI_Guardian_Ignored"))
		return;
	if (!EnemyID || !EnemyTimeOutTime)
	{
		EnemyID = Commands->Get_ID(seen);
		EnemyTimeOutTime = Commands->Get_Random_Int(2,5);
		Goto_Location(obj);
	}
	else if (EnemyID == Commands->Get_ID(seen))
		EnemyTimeOutTime = Commands->Get_Random_Int(2,5);
}
void JMG_Utility_AI_Guardian_Vehicle::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0 || (EnemyID && EnemyTimeOutTime) || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	EnemyID = Commands->Get_ID(damager);
	EnemyTimeOutTime = Commands->Get_Random_Int(2,5);
	Goto_Location(obj);
}
void JMG_Utility_AI_Guardian_Vehicle::Goto_Location(GameObject *obj)
{
	ActionParamsStruct params;
	Commands->Action_Reset(obj,100);
	Vector3 GoToPos = dpPosition;
	params.Set_Movement(GoToPos,Get_Float_Parameter("WanderSpeed"),1.0f,false);
	params.MovePathfind = true;
	params.Set_Basic(this,100,10);
	GameObject *Target = Commands->Find_Object(EnemyID);
	if (Target)
	{
		if (Get_Int_Parameter("FaceTarget"))
			params.Set_Face_Location(Commands->Get_Position(Target),5.0f);
		else
			params.AttackFaceTarget = false;
		params.AttackCheckBlocked = Get_Int_Parameter("CheckBlocked") ? true : false;
		if (aimAtFeet && Target->As_SoldierGameObj())
		{
			params.AttackCheckBlocked = false;
			params.Set_Attack(Commands->Get_Position(Target),Get_Float_Parameter("FireRange"),0,true);
		}
		else
			params.Set_Attack(Target,Get_Float_Parameter("FireRange"),0,true);
		Commands->Action_Attack(obj,params);
	}
	else
	{
		if (Get_Int_Parameter("FaceTarget"))
			params.Set_Face_Location(GoToPos,5.0f);
		else
			params.AttackFaceTarget = false;
		params.ShutdownEngineOnArrival = Get_Int_Parameter("TurnOffEngineOnArrival") ? true : false;
		Commands->Action_Goto(obj,params);
	}
}
void JMG_Utility_Custom_Destroy_Closest_Model_To_Position::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Destroy_Closest_Model_To_Position::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		GameObject *nearest = NULL;
		float dist = 0.0f;
		Vector3 pos = Get_Vector3_Parameter("Position");
		const char *modelName = Get_Parameter("Model");
		float maxDist = Get_Float_Parameter("MaxDistance");
		maxDist *= maxDist;
		for (SLNode<BaseGameObj> *current = GameObjManager::GameObjList.Head();current;current = current->Next())
		{
			GameObject* o = current->Data()->As_ScriptableGameObj();
			if (!_stricmp(modelName,Get_Model(o)))
			{
				float tempDistance = JmgUtility::SimpleDistance(pos,Commands->Get_Position(o));
				if (!nearest || (tempDistance < dist && (maxDist == 0.0 || tempDistance <= maxDist)))
				{
					nearest = o;
					dist = tempDistance;
				}
			}
		}
		if (nearest)
			Commands->Destroy_Object(nearest);
	}
}
int JMG_Utility_Send_Custom_On_Deaths_Controller::deathCount = 0;
int JMG_Utility_Send_Custom_On_Deaths_Controller::maxDeaths = 0;
int JMG_Utility_Send_Custom_On_Deaths_Controller::deathReminder = 0;
int JMG_Utility_Send_Custom_On_Deaths_Controller::urgentDeathReminder = 0;
int JMG_Utility_Send_Custom_On_Deaths_Controller::stringId = 0;
Vector3 JMG_Utility_Send_Custom_On_Deaths_Controller::reminderColor = Vector3();
char JMG_Utility_Send_Custom_On_Deaths_Controller::reminderMessage[220] = "";
char JMG_Utility_Send_Custom_On_Deaths_Controller::deathSingular[220] = "";
char JMG_Utility_Send_Custom_On_Deaths_Controller::deathPlural[220] = "";
char JMG_Utility_Send_Custom_On_Deaths_Controller::remainingSingular[220] = "";
char JMG_Utility_Send_Custom_On_Deaths_Controller::remainingPlural[220] = "";
int JMG_Utility_Send_Custom_On_Deaths_Controller::reminderMessageOrder = 0;
int JMG_Utility_Send_Custom_On_Deaths_Controller::myId = 0;
bool JMG_Utility_Send_Custom_On_Deaths_Controller::controllerPlaced = false;
void JMG_Utility_Send_Custom_On_Deaths_Controller::Created(GameObject *obj)
{
	if (controllerPlaced)
	{
		Console_Input("msg JMG_Utility_Send_Custom_On_Deaths_Controller ERROR: Only place one controller on the map at a time!");
		return;
	}
	deathCount = 0;
	maxDeaths = Get_Int_Parameter("MaxDeaths");
	deathReminder = Get_Int_Parameter("DeathReminder");
	urgentDeathReminder = Get_Int_Parameter("UrgentDeathReminder");
	stringId = Get_Int_Parameter("StringId");
	reminderColor = Get_Vector3_Parameter("ReminderColor");
	const char *parameters = Get_Parameter("ReminderMessage");
	sprintf(reminderMessage,"%s",parameters);
	char delim = Get_Parameter("Delim")[0];
	unsigned int length = strlen(parameters);
	for (unsigned int x = 0;x < length;x++)
		if (reminderMessage[x] == delim)
			reminderMessage[x] = ',';
	reminderMessageOrder = Get_Int_Parameter("ReminderMessageOrder");
	sprintf(deathSingular,"%s",Get_Parameter("DeathSingular"));
	sprintf(deathPlural,"%s",Get_Parameter("DeathPlural"));
	sprintf(remainingSingular,"%s",Get_Parameter("RemainingSingular"));
	sprintf(remainingPlural,"%s",Get_Parameter("RemainingPlural"));
	addDeathsWhenNoPlayers = Get_Float_Parameter("AddDeathsWhenNoPlayers");
	currentNoPlayersAddDeathSaftyTime = noPlayersAddDeathSaftyTime = Get_Int_Parameter("NoPlayersAddDeathSaftyTime");
	if (addDeathsWhenNoPlayers > 0)
		Commands->Start_Timer(obj,this,addDeathsWhenNoPlayers,1);
	controllerPlaced = true;
	myId = Commands->Get_ID(obj);
}
void JMG_Utility_Send_Custom_On_Deaths_Controller::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!Get_Player_Count())
		{
			if (currentNoPlayersAddDeathSaftyTime)
				currentNoPlayersAddDeathSaftyTime--;
			if (!currentNoPlayersAddDeathSaftyTime)
			{
				JMG_Utility_Send_Custom_On_Deaths_Controller::deathCount++;
				int remainingLives = JMG_Utility_Send_Custom_On_Deaths_Controller::maxDeaths-JMG_Utility_Send_Custom_On_Deaths_Controller::deathCount;
				if (!remainingLives)
					Commands->Send_Custom_Event(obj,obj,8765432,8765432,0);
			}
		}
		else
			currentNoPlayersAddDeathSaftyTime = noPlayersAddDeathSaftyTime;
		Commands->Start_Timer(obj,this,addDeathsWhenNoPlayers,1);
	}
}
void JMG_Utility_Send_Custom_On_Deaths_Controller::Destroyed(GameObject *obj)
{
	controllerPlaced = false;
}
void JMG_Utility_Send_Custom_On_Deaths_Controller::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 8765432 && param == 8765432)
	{
		int id = Get_Int_Parameter("ID");
		GameObject *object = id ? Commands->Find_Object(id) : obj;
		if (object)
			Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	}
	if (message == 8765433)
	{
		int id = Get_Int_Parameter("ID");
		GameObject *object = id ? Commands->Find_Object(id) : obj;
		if (object)
			Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("ReminderCustom"),param,0);
	}
}
void JMG_Utility_Send_Custom_On_Deaths_Reporter::Killed(GameObject *obj,GameObject *killer)
{
	if (!JMG_Utility_Send_Custom_On_Deaths_Controller::controllerPlaced)
		return;
	JMG_Utility_Send_Custom_On_Deaths_Controller::deathCount++;
	int deathCount = JMG_Utility_Send_Custom_On_Deaths_Controller::deathCount;
	int remainingLives = JMG_Utility_Send_Custom_On_Deaths_Controller::maxDeaths-JMG_Utility_Send_Custom_On_Deaths_Controller::deathCount;
	if ((!(remainingLives%JMG_Utility_Send_Custom_On_Deaths_Controller::deathReminder) || remainingLives <= JMG_Utility_Send_Custom_On_Deaths_Controller::urgentDeathReminder))
	{
		if (remainingLives > 0)
		{
			char remainingLivesMessage[220];
			switch (JMG_Utility_Send_Custom_On_Deaths_Controller::reminderMessageOrder)
			{
			case 0:sprintf(remainingLivesMessage,"%s",formatReminderString(JMG_Utility_Send_Custom_On_Deaths_Controller::reminderMessage,deathCount,deathCount == 1 ? JMG_Utility_Send_Custom_On_Deaths_Controller::deathSingular : JMG_Utility_Send_Custom_On_Deaths_Controller::deathPlural,remainingLives,remainingLives == 1 ? JMG_Utility_Send_Custom_On_Deaths_Controller::remainingSingular : JMG_Utility_Send_Custom_On_Deaths_Controller::remainingPlural));break;
			case 1:sprintf(remainingLivesMessage,"%s",formatReminderString(JMG_Utility_Send_Custom_On_Deaths_Controller::reminderMessage,remainingLives,remainingLives == 1 ? JMG_Utility_Send_Custom_On_Deaths_Controller::remainingSingular : JMG_Utility_Send_Custom_On_Deaths_Controller::remainingPlural,deathCount,deathCount == 1 ? JMG_Utility_Send_Custom_On_Deaths_Controller::deathSingular : JMG_Utility_Send_Custom_On_Deaths_Controller::deathPlural));break;
			case 2:sprintf(remainingLivesMessage,"%s",formatReminderString(JMG_Utility_Send_Custom_On_Deaths_Controller::reminderMessage,remainingLives,remainingLives == 1 ? JMG_Utility_Send_Custom_On_Deaths_Controller::remainingSingular : JMG_Utility_Send_Custom_On_Deaths_Controller::remainingPlural));break;
			default:sprintf(remainingLivesMessage,"JMG_Utility_Send_Custom_On_Deaths_Controller ERROR: Invalid reminderMessageOrder!");break;
			}
			JmgUtility::SetHUDHelpText(JMG_Utility_Send_Custom_On_Deaths_Controller::stringId,remainingLivesMessage,JMG_Utility_Send_Custom_On_Deaths_Controller::reminderColor);
		}
		GameObject *controller = Commands->Find_Object(JMG_Utility_Send_Custom_On_Deaths_Controller::myId);
		Commands->Send_Custom_Event(obj,controller,8765433,remainingLives,0);
	}
	if (!remainingLives)
	{
		GameObject *controller = Commands->Find_Object(JMG_Utility_Send_Custom_On_Deaths_Controller::myId);
		Commands->Send_Custom_Event(obj,controller,8765432,8765432,0);
	}
}
char *JMG_Utility_Send_Custom_On_Deaths_Reporter::formatReminderString(const char *format,...)
{
	static char displayMsg[256];
	va_list args;
	va_start(args,format);
	vsprintf(displayMsg,format,args);
	va_end(args);
	return displayMsg;
}
void JMG_Utility_AI_Goto_Player_Ignore_Object::Created(GameObject *obj)
{
}
void JMG_Utility_AI_Goto_Enemy_Ignore_Object::Created(GameObject *obj)
{
}
void JMG_Utility_AI_Goto_Enemy_Not_Star_Ignore_Object::Created(GameObject *obj)
{
}
void JMG_Utility_Custom_Set_Team_And_Model::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Set_Team_And_Model::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Set_Model(obj,Get_Parameter("Model"));
		Commands->Set_Player_Type(obj,Get_Int_Parameter("Team"));
		Commands->Action_Reset(obj,Get_Float_Parameter("ResetActionPriority"));
	}
}
void JMG_Utility_Death_Warhead_Create_Object::Created(GameObject *obj)
{
	lastDamageWarhead = 9999;
}
void JMG_Utility_Death_Warhead_Create_Object::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	lastDamageWarhead = Get_Damage_Warhead();
}
void JMG_Utility_Death_Warhead_Create_Object::Killed(GameObject *obj,GameObject *killer)
{
	if (lastDamageWarhead != ArmorWarheadManager::Get_Warhead_Type(Get_Parameter("WarheadName")))
		return;
	if (Commands->Get_Random(0.0f,1.0f) > Get_Float_Parameter("Chance"))
		return;
	GameObject *spawned = Commands->Create_Object(Get_Parameter("Preset"),Commands->Get_Position(obj));
	Commands->Set_Facing(spawned,Commands->Get_Facing(obj));
	Commands->Set_Model(obj,"null");
}
void JMG_Utility_Zone_Change_Character_On_Preset_Enter::Created(GameObject *obj)
{
	playerType = Get_Int_Parameter("PlayerType");
	sprintf(entererPreset,"%s",Get_Parameter("EntererPreset"));
}
void JMG_Utility_Zone_Change_Character_On_Preset_Enter::Entered(GameObject *obj,GameObject *enterer)
{
	if (CheckPlayerType(enterer,playerType) || _stricmp(Commands->Get_Preset_Name(enterer),entererPreset) || Commands->Get_Player_Type(enterer) == -4)
		return;
	Change_Character(enterer,Get_Parameter("Preset"));
}
void JMG_Utility_Sync_HP_With_Object::Created(GameObject *obj)
{
	id = Get_Int_Parameter("ID");
	rate = Get_Float_Parameter("Rate");
	GameObject *object = Commands->Find_Object(id);
	if (object)
	{
		Set_Max_Health(obj,Commands->Get_Max_Health(object));
		Set_Max_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(object));
		Commands->Set_Health(obj,Commands->Get_Health(object));
		Commands->Set_Shield_Strength(obj,Commands->Get_Shield_Strength(object));
		Commands->Start_Timer(obj,this,rate,1);
	}
	else
		Commands->Destroy_Object(obj);
}
void JMG_Utility_Sync_HP_With_Object::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		GameObject *object = Commands->Find_Object(id);
		if (object)
		{
			Commands->Set_Health(obj,Commands->Get_Health(object));
			Commands->Set_Shield_Strength(obj,Commands->Get_Shield_Strength(object));
		}
		else
			Commands->Destroy_Object(obj);
		Commands->Start_Timer(obj,this,rate,1);
	}
}
Vector3 JMG_Utility_Set_Screen_Color_Fade_Controller::color[128] = {Vector3(0.0f,0.0f,0.0f)};
float JMG_Utility_Set_Screen_Color_Fade_Controller::opacity[128] = {0.0f};
bool JMG_Utility_Set_Screen_Color_Fade_Controller::controllerPlaced = false;
Vector3 JMG_Utility_Set_Screen_Color_Fade_Controller::controllerDefaultColor = Vector3(0.0f,0.0f,0.0f);
float JMG_Utility_Set_Screen_Color_Fade_Controller::controllerDefaultOpacity = 0.0f;
void JMG_Utility_Set_Screen_Color_Fade_Controller::Created(GameObject *obj)
{
	controllerDefaultColor = Get_Vector3_Parameter("Color");
	controllerDefaultOpacity = Get_Float_Parameter("Opacity");
	for (int x = 1;x < 128;x++)
	{
		color[x] = Get_Vector3_Parameter("Color");
		opacity[x] = Get_Float_Parameter("Opacity");
		syncedScreen[x] = false;
	}
	Commands->Start_Timer(obj,this,1.0,1);
}
void JMG_Utility_Set_Screen_Color_Fade_Controller::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1) 
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (player && !syncedScreen[x])
			{
				syncedScreen[x] = true;
				Set_Screen_Fade_Color_Player(player,color[x].X,color[x].Y,color[x].Z,0.0f);
				Set_Screen_Fade_Opacity_Player(player,opacity[x],0.0f);
			}
			if (!player && syncedScreen[x])
				syncedScreen[x] = false;
		}
		Commands->Start_Timer(obj,this,1.0,1);
	}
}
void JMG_Utility_Set_Screen_Color_Fade_Controller::Destroyed(GameObject *obj)
{
	Update_Colors(Vector3(),0.0f);
	controllerPlaced = false;
}
void JMG_Utility_Set_Screen_Color_Fade_Controller::Update_Colors(Vector3 Color,float Opacity)
{
	controllerDefaultColor = Color;
	controllerDefaultOpacity = Opacity;
	for (int x = 0;x < 128;x++)
	{
		color[x] = Color;
		opacity[x] = Opacity;
	}
}
void JMG_Utility_Set_Screen_Color_Fade_Controller::Update_Player_Colors(GameObject *player,Vector3 Color,float Opacity)
{
	int playerId = JmgUtility::JMG_Get_Player_ID(player);
	if (Color.X >= 0.0f)
		color[playerId] = Color;
	if (Opacity >= 0.0f)
		opacity[playerId] = Opacity;
}
void JMG_Utility_Set_Screen_Color_Fade_Controller::Reset_To_Default(GameObject *player)
{
	int playerId = JmgUtility::JMG_Get_Player_ID(player);
	color[playerId] = controllerDefaultColor;
	opacity[playerId] = controllerDefaultOpacity;
}
void JMG_Utility_Set_Screen_Color_Fade_Controller::Update_Player(GameObject *player,float transition)
{
	int playerId = JmgUtility::JMG_Get_Player_ID(player);
	Set_Screen_Fade_Color_Player(player,color[playerId].X,color[playerId].Y,color[playerId].Z,transition);
	Set_Screen_Fade_Opacity_Player(player,opacity[playerId],transition);
}
void JMG_Utility_Set_Screen_Color_Fade_Controller::Update_All_Players(float transition)
{
	for (int x = 1;x < 128;x++)
	{
		GameObject *player = Get_GameObj(x);
		if (!player)
			continue;
		Set_Screen_Fade_Color_Player(player,color[x].X,color[x].Y,color[x].Z,transition);
		Set_Screen_Fade_Opacity_Player(player,opacity[x],transition);
	}
}
void JMG_Utility_Set_Screen_Color_Fade_On_Custom::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Set_Screen_Color_Fade_On_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		if (!JMG_Utility_Set_Screen_Color_Fade_Controller::controllerPlaced)
		{
			Console_Input("msg JMG_Utility_Set_Screen_Color_Fade_On_Enter ERROR: You need to place JMG_Utility_Set_Screen_Color_Fade_Controller on an object in order to use this script!");
			return;
		}
		JMG_Utility_Set_Screen_Color_Fade_Controller::Update_Colors(Get_Vector3_Parameter("Color"),Get_Float_Parameter("Opacity"));
		JMG_Utility_Set_Screen_Color_Fade_Controller::Update_All_Players(Get_Float_Parameter("Transition"));
		if (Get_Int_Parameter("TriggerOnce"))
			Destroy_Script();
	}
}
void JMG_Utility_Set_Screen_Color_Fade_Reset_Player_Create::Created(GameObject *obj)
{
	if (!Commands->Is_A_Star(obj))
	{
		Destroy_Script();
		return;
	}
	JMG_Utility_Set_Screen_Color_Fade_Controller::Reset_To_Default(obj);
	JMG_Utility_Set_Screen_Color_Fade_Controller::Update_Player(obj,Get_Float_Parameter("Transition"));
}
void JMG_Utility_Set_Screen_Color_Fade_Reset_Player_Enter::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")) || !Commands->Is_A_Star(enter))
		return;
	JMG_Utility_Set_Screen_Color_Fade_Controller::Reset_To_Default(obj);
	JMG_Utility_Set_Screen_Color_Fade_Controller::Update_Player(enter,Get_Float_Parameter("Transition"));
}
void JMG_Utility_Simple_Mech::Created(GameObject *obj)
{
	idleCooldown = 0;
	releaseDelay = Get_Int_Parameter("ReleaseDelay");
	currentDirection = mvmtDir::IDLE;
	lastMovementDirection = mvmtDir::FORWARD;
	sprintf(idleAnimation,"%s",Get_Parameter("IdleAnimation"));
	hasIdleAnimation = true;
	if (!_stricmp(idleAnimation,""))
		hasIdleAnimation = false;
	sprintf(forwardAnimation,"%s",Get_Parameter("ForwardAnimation"));
	sprintf(backwardAnimation,"%s",Get_Parameter("BackwardAnimation"));
	sprintf(turnLeftAnimation,"%s",Get_Parameter("TurnLeftAnimation"));
	sprintf(turnRightAnimation,"%s",Get_Parameter("TurnRightAnimation"));
	if (!hasIdleAnimation)
		Commands->Set_Animation(obj,idleAnimation,true,0,0.0f,-1,true);
	else
		Commands->Set_Animation(obj,forwardAnimation,false,0,0.0f,0.0f,true);
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Simple_Mech::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (idleCooldown)
			idleCooldown--;
		PhysControllerClass *controller = obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass()->Get_Controller();
		mvmtDir newDirection = mvmtDir::IDLE;
        if(controller->Get_Move_Forward() > 0)
            newDirection = mvmtDir::FORWARD;
        else if(controller->Get_Move_Forward() < 0)
            newDirection = mvmtDir::BACKWARD;
        if (newDirection == mvmtDir::IDLE)
            if(controller->Get_Turn_Left() > 0)
                newDirection = mvmtDir::LEFT;
            else if(controller->Get_Turn_Left() < 0)
                newDirection = mvmtDir::RIGHT;
		if (obj->As_VehicleGameObj()->Is_Immovable())
			newDirection = mvmtDir::IDLE;
		if (!idleCooldown && currentDirection != newDirection)
		{
			float currentAnimationFrame = Get_Animation_Frame(obj);
			if (newDirection == mvmtDir::IDLE)
			{
				if (hasIdleAnimation)
					Commands->Set_Animation(obj,idleAnimation,true,0,currentAnimationFrame,-1,true);
				else
					PlayAnimation(obj,currentDirection,currentAnimationFrame,currentAnimationFrame,false);
			}
			else
				if (currentAnimationFrame != 0.0f && lastMovementDirection != newDirection)
					PlayAnimation(obj,lastMovementDirection,currentAnimationFrame,0.0f,true);
				else
					PlayAnimation(obj,newDirection,currentAnimationFrame,-1,true);
			if (newDirection != mvmtDir::IDLE)
				lastMovementDirection = newDirection;
			currentDirection = newDirection;
		}
		if (currentDirection == newDirection && (controller->Get_Move_Forward() || controller->Get_Turn_Left()))
			idleCooldown = releaseDelay;
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Simple_Mech::Animation_Complete(GameObject *obj,const char *animation_name)
{
	PlayAnimation(obj,currentDirection,0.0f,-1,true);
}
void JMG_Utility_Simple_Mech::PlayAnimation(GameObject *obj,mvmtDir direction,float startFrame,float endFrame,bool looped)
{
	if (direction == mvmtDir::FORWARD)
		Commands->Set_Animation(obj,forwardAnimation,looped,0,startFrame,endFrame,true);
	else if (direction == mvmtDir::BACKWARD)
		Commands->Set_Animation(obj,backwardAnimation,looped,0,startFrame,endFrame,true);
	else if (direction == mvmtDir::LEFT)
		Commands->Set_Animation(obj,turnLeftAnimation,looped,0,startFrame,endFrame,true);
	else if (direction == mvmtDir::RIGHT)
		Commands->Set_Animation(obj,turnRightAnimation,looped,0,startFrame,endFrame,true);
}
void JMG_Utility_Send_Custom_When_Preset_In_Range::Created(GameObject *obj)
{
	range = Get_Float_Parameter("Range");
	range *= range;
	sprintf(preset,"%s",Get_Parameter("Preset"));
	rate = Get_Float_Parameter("Rate");
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Send_Custom_When_Preset_In_Range::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 mypos = Commands->Get_Position(obj);
		for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
		{
			SmartGameObj* o = current->Data();
			if (o != obj && JmgUtility::SimpleDistance(Commands->Get_Position(o),mypos) <= range && !_stricmp(Commands->Get_Preset_Name(o),preset))
			{
				int id = Get_Int_Parameter("ID");
				GameObject *object = id ? (id == -1 ? o : Commands->Find_Object(id)) : obj;
				if (object)
					Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
				if (Get_Int_Parameter("TriggerOnce"))
				{
					Destroy_Script();
					return;
				}
				break;
			}
		}
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Death_Create_Object_At_Bone_Position::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *object = Commands->Create_Object(Get_Parameter("Preset"),Commands->Get_Position(obj));
	Commands->Attach_To_Object_Bone(object,obj,Get_Parameter("Bone"));
	Vector3 targetPos = Commands->Get_Position(object);
	MoveablePhysClass *mphys = object->As_PhysicalGameObj() ? object->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
	Commands->Attach_To_Object_Bone(object,NULL,"");
	if (mphys && mphys->Find_Teleport_Location(targetPos,Get_Float_Parameter("MaxDistance"),&targetPos))
	{
		Commands->Set_Position(object,targetPos);
		Force_Position_Update(object);
	}
}
void JMG_Utility_Enable_Loiter::Created(GameObject *obj)
{
	Commands->Set_Loiters_Allowed(obj,true);
}
void JMG_Utility_Custom_Switch_Weapon::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Switch_Weapon::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Select_Weapon(obj,Get_Parameter("Weapon"));
	}
}
JMG_Utility_HUD_Count_Down::SendCustomOnSecondNode *JMG_Utility_HUD_Count_Down::sendCustomOnSecondController = NULL;
bool JMG_Utility_HUD_Count_Down::controllerPlaced = false;
void JMG_Utility_HUD_Count_Down::Created(GameObject *obj)
{
	sendCustomOnSecondController = NULL;
	seconds = Get_Int_Parameter("TimeInSeconds");
	stringId = Get_Int_Parameter("StringID");
	color = Get_Vector3_Parameter("Color");
	sprintf(hourSingular,"%s",Get_Parameter("HourSingular"));
	sprintf(hourPlural,"%s",Get_Parameter("HourPlural"));
	sprintf(minuteSingular,"%s",Get_Parameter("MinuteSingular"));
	sprintf(minutePlural,"%s",Get_Parameter("MinutePlural"));
	sprintf(secondSingular,"%s",Get_Parameter("SecondSingular"));
	sprintf(secondPlural,"%s",Get_Parameter("SecondPlural"));
	const char *parameters = Get_Parameter("WarningMessage");
	sprintf(warningMessage,"%s",parameters);
	char delim = Get_Parameter("Delim")[0];
	unsigned int length = strlen(parameters);
	for (unsigned int x = 0;x < length;x++)
		if (warningMessage[x] == delim)
			warningMessage[x] = ',';
	controllerPlaced = true;
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_HUD_Count_Down::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (seconds)
		{
			seconds--;
			if (seconds >= 3600 && !(seconds % 3600))
				JmgUtility::SetHUDHelpText(stringId,formatReminderString(warningMessage,seconds/3600,seconds/3600 == 1 ? hourSingular : hourPlural),color);
			else if (seconds >= 60 && (seconds == 1800 || seconds == 1200 || seconds == 600 || (seconds <= 300 && !(seconds % 60))))
				JmgUtility::SetHUDHelpText(stringId,formatReminderString(warningMessage,seconds/60,seconds/60 == 1 ? minuteSingular : minutePlural),color);
			else if (seconds && (seconds <= 10 || seconds == 30 || seconds == 20))
				JmgUtility::SetHUDHelpText(stringId,formatReminderString(warningMessage,seconds,seconds == 1 ? secondSingular : secondPlural),color);
			SendCustomOnSecondNode *node = FindSecondNode(seconds);
			if (node)
				Commands->Send_Custom_Event(obj,Commands->Find_Object(node->id),node->custom,node->param,node->delay);
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Utility_HUD_Count_Down::Destroyed(GameObject *obj)
{
	controllerPlaced = false;
	CleanupSecondNodes();
}
char *JMG_Utility_HUD_Count_Down::formatReminderString(const char *format,...)
{
	static char displayMsg[256];
	va_list args;
	va_start(args,format);
	vsprintf(displayMsg,format,args);
	va_end(args);
	return displayMsg;
}
void JMG_Utility_HUD_Count_Down_Send_Custom::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_HUD_Count_Down_Send_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		int id = Get_Int_Parameter("ID");
		id = id ? id : Commands->Get_ID(obj);
		JMG_Utility_HUD_Count_Down::AddSecondNode(Get_Int_Parameter("TriggerTime"),id,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	}
}
void JMG_Utility_Zone_Screen_Fade::Entered(GameObject *obj,GameObject *enterer)
{
	if (CheckPlayerType(enterer,Get_Int_Parameter("PlayerType")) || !Commands->Is_A_Star(enterer))
		return;
	Vector3 color = Get_Vector3_Parameter("Color");
	float opacity = Get_Float_Parameter("Opacity");
	JMG_Utility_Set_Screen_Color_Fade_Controller::Update_Player_Colors(enterer,color,opacity);
	float transition = Get_Float_Parameter("Transition");
	if (opacity >= 0.0f)
		Set_Screen_Fade_Opacity_Player(enterer,opacity,transition);
	if (color.X >= 0.0f)
		Set_Screen_Fade_Color_Player(enterer,color.X,color.Y,color.Z,transition);
}
void JMG_Utility_Custom_Triggers_Enemy_Seen::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Triggers_Enemy_Seen::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Set_Is_Visible(obj,Get_Int_Parameter("Visible") ? true : false);
	}
}
void JMG_Utility_Screen_Fade_Red_On_Damage::Created(GameObject *obj)
{
	if (!Commands->Is_A_Star(obj))
	{
		Destroy_Script();
		return;
	}
	fadeColor = Get_Vector3_Parameter("FadeColor");
	fadeOpacity = Get_Float_Parameter("FadeOpacity");
	playerId = JmgUtility::JMG_Get_Player_ID(obj);
	injuryRatio = 0.0f;
	screenOpacity = 0.0f;
	lastScreenOpacity = 0.0f;
	lastInjuryRatio = 0.0f;
	ScreenFade(obj);
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Screen_Fade_Red_On_Damage::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1 && Commands->Get_Health(obj) > 0)
	{
		if (injuryRatio > 0)
			injuryRatio -= 0.01f;
		else
			injuryRatio = 0.0f;
		if (screenOpacity > 0)
			screenOpacity -= 0.01f;
		else
			screenOpacity = 0.0f;
		ScreenFade(obj);
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Screen_Fade_Red_On_Damage::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage > 1.0f)
	{
		float totalHP = Commands->Get_Max_Health(obj) + Commands->Get_Max_Shield_Strength(obj);
		float temp = damage/totalHP;
		if (screenOpacity + temp < fadeOpacity)
			screenOpacity += temp;
		else
			screenOpacity = fadeOpacity;
		if (injuryRatio + temp < 1.0f)
			injuryRatio += temp;
		else
			injuryRatio = 1.0f;
		if (!Commands->Get_Health(obj))
			ScreenFade(obj);
	}
}
void JMG_Utility_Screen_Fade_Red_On_Damage::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 596911)
	{
		float tempParam = (float)param / 100.0f;
		if (tempParam > 1.0f)
		{
			float totalHP = Commands->Get_Max_Health(obj) + Commands->Get_Max_Shield_Strength(obj);
			float temp = tempParam/totalHP;
			if (screenOpacity + temp < fadeOpacity)
				screenOpacity += temp;
			else
				screenOpacity = fadeOpacity;
			if (injuryRatio + temp < 1.0f)
				injuryRatio += temp;
			else
				injuryRatio = 1.0f;
		}
	}
}
void JMG_Utility_Screen_Fade_Red_On_Damage::ScreenFade(GameObject *obj)
{
	if (lastScreenOpacity == screenOpacity && lastInjuryRatio == injuryRatio)
		return;
	if ((JMG_Utility_Swimming_Infantry::isUnderwater[playerId] && JMG_Utility_Swimming_Zone::waterNode[playerId].waterColorOpacity) || JMG_Utility_Set_Screen_Color_Fade_Controller::opacity[playerId])
	{
		float temp = 1.0f-injuryRatio;
		if (JMG_Utility_Swimming_Infantry::isUnderwater[playerId])
		{
			Set_Screen_Fade_Color_Player(obj,(JMG_Utility_Swimming_Zone::waterNode[playerId].waterColor.X*temp+injuryRatio*fadeColor.X),JMG_Utility_Swimming_Zone::waterNode[playerId].waterColor.Y*temp+injuryRatio*fadeColor.Y,JMG_Utility_Swimming_Zone::waterNode[playerId].waterColor.Z*temp+injuryRatio*fadeColor.Z,0.1f);
			Set_Screen_Fade_Opacity_Player(obj,JmgUtility::MathClamp(screenOpacity+JMG_Utility_Swimming_Zone::waterNode[playerId].waterColorOpacity*temp,0.0f,fadeOpacity),0.1f);
		}
		else
		{
			Set_Screen_Fade_Color_Player(obj,(JMG_Utility_Set_Screen_Color_Fade_Controller::color[playerId].X*temp+injuryRatio*fadeColor.X),JMG_Utility_Set_Screen_Color_Fade_Controller::color[playerId].Y*temp+injuryRatio*fadeColor.Y,JMG_Utility_Set_Screen_Color_Fade_Controller::color[playerId].Z*temp+injuryRatio*fadeColor.Z,0.1f);
			Set_Screen_Fade_Opacity_Player(obj,JmgUtility::MathClamp(screenOpacity+JMG_Utility_Set_Screen_Color_Fade_Controller::opacity[playerId]*temp,0.0f,fadeOpacity),0.1f);
		}
	}
	else
	{
		Set_Screen_Fade_Color_Player(obj,fadeColor.X,fadeColor.Y,fadeColor.Z,0.1f);
		Set_Screen_Fade_Opacity_Player(obj,screenOpacity,0.1f);
	}
	lastScreenOpacity = screenOpacity;
	lastInjuryRatio = injuryRatio;
}
void JMG_Utility_Player_Count_Enable_Spawners::Created(GameObject *obj)
{
	enabled = Get_Int_Parameter("StartsEnabled") ? true : false;
	enableCustom = Get_Int_Parameter("EnableOrDisableCustom");
	playerCount = Get_Vector3_Parameter("PlayerCount[eq|<|>]");
	playerCount2 = Get_Vector3_Parameter("PlayerCount[neq|between(|)]");
	conditionMatching = false;
	EnableSpawners(false);
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_Player_Count_Enable_Spawners::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (enabled)
		{
			int tempCount = Get_Player_Count();
			if (playerCount.X >= 0.0 && tempCount == playerCount.X)// equal
				EnableSpawners(true);
			else if (playerCount.Y >= 0.0 && tempCount < playerCount.Y)// less than
				EnableSpawners(true);
			else if (playerCount.Z >= 0.0 && tempCount > playerCount.Z)// greater than
				EnableSpawners(true);
			else if (playerCount2.X >= 0.0 && tempCount != playerCount2.X)// not equal
				EnableSpawners(true);
			else if (playerCount2.Y >= 0.0 && playerCount2.Z >= 0.0 && tempCount > playerCount2.Y && tempCount < playerCount2.Z)// between
				EnableSpawners(true);
			else
				EnableSpawners(false);
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Utility_Player_Count_Enable_Spawners::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == enableCustom)
	{
		enabled = param ? true : false;
	}
}
void JMG_Utility_Player_Count_Enable_Spawners::EnableSpawners(bool enable)
{
	if (conditionMatching == enable)
		return;
	conditionMatching = enable;
	int end = Get_Int_Parameter("EndID");
	for (int x = Get_Int_Parameter("StartID");x <= end;x++)
		Commands->Enable_Spawner(x,enable);
}
void JMG_Utility_AI_Engineer_Ignore_Target::Created(GameObject *obj)
{
}
void JMG_Utility_Give_Weapon_In_Range::Created(GameObject *obj)
{
	sprintf(weaponName,"%s",Get_Parameter("WeaponName"));
	location = Get_Vector3_Parameter("Location");
	if (JmgUtility::SimpleDistance(location,Vector3()) <= 0.0f)
		location = Commands->Get_Position(obj);
	range = Get_Float_Parameter("Range");
	range *= range;
	enableCustom = Get_Int_Parameter("EnableOrDisableCustom");
	enabled = Get_Int_Parameter("StartsEnabled") ? true : false;
	selectWeapon = Get_Int_Parameter("SelectWeapon") ? true : false;
	weaponAmmo = Get_Int_Parameter("WeaponAmmo");
	rate = Get_Float_Parameter("Rate");
	if (rate)
		Commands->Start_Timer(obj,this,rate,1);
	refillAmount = Get_Int_Parameter("RefillAmount");
	refillRate = Get_Float_Parameter("RefillRate");
	if (refillRate && refillAmount)
		Commands->Start_Timer(obj,this,refillRate,2);
}
void JMG_Utility_Give_Weapon_In_Range::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (enabled)
			GrantWeapon();
		Commands->Start_Timer(obj,this,rate,1);
	}
	if (number == 2)
	{
		if (enabled)
			for (int x = 1;x < 128;x++)
			{
				GameObject *player = Get_GameObj(x);
				if (!player || !Has_Weapon(player,weaponName) || JmgUtility::SimpleDistance(location,Commands->Get_Position(player)) > range)
					continue;
				int maxTotalBullets = Get_Max_Total_Bullets(player,weaponName);
				int totalBullets = Get_Total_Bullets(player,weaponName);
				if (totalBullets < maxTotalBullets)
				{
					int addBullets = min(maxTotalBullets-totalBullets,refillAmount);
					Set_Clip_Bullets(player,weaponName,Get_Clip_Bullets(player,weaponName)+addBullets);
				}
			}
		Commands->Start_Timer(obj,this,refillRate,2);
	}
}
void JMG_Utility_Give_Weapon_In_Range::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == enableCustom)
	{
		enabled = param ? true : false;
		if (enabled)
			GrantWeapon();
	}
}
void JMG_Utility_Give_Weapon_In_Range::GrantWeapon()
{
	for (int x = 1;x < 128;x++)
	{
		GameObject *player = Get_GameObj(x);
		if (!player || Has_Weapon(player,weaponName) || JmgUtility::SimpleDistance(location,Commands->Get_Position(player)) > range)
			continue;
		Grant_Weapon(player,weaponName,true,weaponAmmo,true);
		if (selectWeapon)
			Commands->Select_Weapon(player,weaponName);
	}
}
int JMG_Utility_AI_Follow_Player_On_Poke_Controller::followingPlayer[128] = {0};
int JMG_Utility_AI_Follow_Player_On_Poke_Controller::maxFollowers = 0;
bool JMG_Utility_AI_Follow_Player_On_Poke_Controller::controllerPlaced = false;
void JMG_Utility_AI_Follow_Player_On_Poke_Controller::Created(GameObject *obj)
{
	maxFollowers = Get_Int_Parameter("MaxFollowers");
	for (int x = 0;x < 128;x++)
		followingPlayer[x] = 0;
}
void JMG_Utility_AI_Follow_Player_On_Poke_Controller::Destroyed(GameObject *obj)
{
	JMG_Utility_AI_Follow_Player_On_Poke_Controller::controllerPlaced = false;
}
void JMG_Utility_AI_Follow_Player_On_Poke::Created(GameObject *obj)
{
	if (!JMG_Utility_AI_Follow_Player_On_Poke_Controller::controllerPlaced)
	{
		Console_Input("msg JMG_Utility_AI_Follow_Player_On_Poke ERROR: JMG_Utility_AI_Follow_Player_On_Poke_Controller must be placed on the map!");
		this->Destroy_Script();
		return;
	}
	const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(Get_Current_Weapon(obj),true);
	if (ammo)
		weaponRange = ammo->Range;
	enemyId = 0;
	actionUpdate = 0;
	enemySeen = 0;
	hurt = false;
	lastTarget = NULL;
	lastFollow = NULL;
	lastLocation = homeLocation = Commands->Get_Position(obj);
	Commands->Set_Player_Type(obj,-2);
	followDistance = Get_Float_Parameter("FollowDistance");
	followNearSpeed = Get_Float_Parameter("FollowNearSpeed");
	followFarSpeed = Get_Float_Parameter("FollowFarSpeed");
	followFarDistance = Get_Float_Parameter("FollowFarDistance");
	followFarDistance *= followFarDistance;
	followVeryFarSpeed = Get_Float_Parameter("FollowVeryFarSpeed");
	followVeryFarDistance = Get_Float_Parameter("FollowVeryFarDistance");
	followVeryFarDistance *= followVeryFarDistance;
	runHomeSpeed = Get_Float_Parameter("RunHomeSpeed");
	fallbackWithoutArmor = Get_Int_Parameter("FallBackWhenOutOfArmor") ? true : false;
	healWhileAtHomeLocation = Get_Float_Parameter("HealWhileAtHomeLocation");
	sendCustomId = Get_Int_Parameter("SendCustomID");
	pokedCustom = Get_Int_Parameter("PokedCustom");
	lostCustom = Get_Int_Parameter("LostCustom");
	killedCustom = Get_Int_Parameter("KilledCustom");
	injuredCustom = Get_Int_Parameter("InjuredCustom");
	healedCustom = Get_Int_Parameter("HealedCustom");
	messageColor = Get_Vector3_Parameter("HUDMessagesColor");
	messagePokeFollowerId = Get_Int_Parameter("MessagePokeFollowerID");
	if (messagePokeFollowerId)
		sprintf(messagePokeFollower,"%s",Get_Translated_String(messagePokeFollowerId));
	messageFollowingYouId = Get_Int_Parameter("MessageFollowingYouID");
	messageFollowingPlayerId = Get_Int_Parameter("MessageFollowingPlayerID");
	messageMaxFollowersId = Get_Int_Parameter("MessageMaxFollowersID");
	if (messageMaxFollowersId)
		sprintf(messageMaxFollowers,"%s",Get_Translated_String(messageMaxFollowersId));
	messageHealingRequiredId = Get_Int_Parameter("MessageHealingRequiredID");
	messageFollowerLostId = Get_Int_Parameter("MessageFollowerLostID");
	messageFollowerInjuredId = Get_Int_Parameter("MessageFollowerInjuredID");
	messageFollowerKilledId = Get_Int_Parameter("MessageFollowerKilledID");
	lastPos = Commands->Get_Position(obj);
	maxFollowTime = Get_Int_Parameter("MaxFollowTime");
	maxIdleTime = Get_Int_Parameter("MaxIdleTime");
	Commands->Start_Timer(obj,this,0.1f,1);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
}
void JMG_Utility_AI_Follow_Player_On_Poke::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (seen->As_SmartGameObj() && seen->As_SmartGameObj()->Is_Stealthed() && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 900)
		return;
	if (enemyId == Commands->Get_ID(seen))
	{
		enemySeen = 30;
		return;
	}
	if (enemyId && JmgUtility::SimpleDistance(Commands->Get_Position(seen),Commands->Get_Position(obj)) > targetDistance)
		return;
	enemyId = Commands->Get_ID(seen);
	enemySeen = 30;
}
void JMG_Utility_AI_Follow_Player_On_Poke::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float speed = runHomeSpeed;
		float distance = followDistance;
		Vector3 myPos = Commands->Get_Position(obj);
		if (enemySeen)
		{
			enemySeen--;
			if (!enemySeen)
				ForgetEnemy(obj);
		}
		if (actionUpdate)
			actionUpdate--;
		GameObject *enemy = Commands->Find_Object(enemyId);
		if (enemy)
			targetDistance = JmgUtility::SimpleDistance(Commands->Get_Position(enemy),myPos);
		else if (enemyId)
			ForgetEnemy(obj);
		GameObject *player = Commands->Find_Object(playerObjectId);
		if (player)
		{
			bool quitFollowing = false;
			if (maxFollowTime)
			{
				currentFollowTime++;
				if (currentFollowTime == maxFollowTime)
					quitFollowing = true;
			}
			if (maxIdleTime && JmgUtility::SimpleDistance(myPos,lastPos) <= 0.0f)
			{
				currentIdleTime++;
				if (currentIdleTime == maxIdleTime)
					quitFollowing = true;
			}
			else
				currentIdleTime = 0;
			if (quitFollowing)
			{
				ShowPlayerMessage(player,messageFollowerLostId,messageColor);
				SendCustom(obj,lostCustom);
				GiveUpOnPlayer(obj,true);
				player = NULL;
			}
			lastPos = myPos;
		}
		if (pokerPlayerId && !player)
		{
			SendCustom(obj,lostCustom);
			GiveUpOnPlayer(obj,true);
		}
		else if (player)
		{
			float tempDist = JmgUtility::SimpleDistance(Commands->Get_Position(player),myPos);
			if (tempDist >= followVeryFarDistance)
				speed = followVeryFarSpeed;
			else if (tempDist >= followFarDistance)
				speed = followFarSpeed;
			else
				speed = followNearSpeed;
		}
		if (hurt || !player)
		{
			distance = 1.0f;
			speed = runHomeSpeed;
		}
		if (healWhileAtHomeLocation && Commands->Get_Shield_Strength(obj) < Commands->Get_Max_Shield_Strength(obj) && JmgUtility::SimpleDistance(myPos,homeLocation) <= 2.0f)
		{
			Commands->Apply_Damage(obj,-healWhileAtHomeLocation,"None",0);
			if (hurt && !pokerPlayerId && fallbackWithoutArmor && Commands->Get_Shield_Strength(obj) >= Commands->Get_Max_Shield_Strength(obj))
			{
				Commands->Enable_HUD_Pokable_Indicator(obj,true);
				SendCustom(obj,healedCustom);
				hurt = false;
			}
		}
		AttackTarget(obj,player,enemy,homeLocation,speed,distance);
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_AI_Follow_Player_On_Poke::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!hurt && fallbackWithoutArmor && Commands->Get_Shield_Strength(obj) <= 0.0f)
	{
		GameObject *player = Commands->Find_Object(playerObjectId);
		if (player)
			ShowPlayerMessage(player,messageFollowerInjuredId,messageColor);
		if (pokerPlayerId)
			SendCustom(obj,injuredCustom);
		GiveUpOnPlayer(obj,false);
		hurt = true;
	}
	if (hurt && Commands->Get_Shield_Strength(obj) >= Commands->Get_Max_Shield_Strength(obj))
	{
		Commands->Enable_HUD_Pokable_Indicator(obj,true);
		SendCustom(obj,healedCustom);
		hurt = false;
	}
	if (damage <= 0 || !damager || obj == damager || enemyId == Commands->Get_ID(damager) || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	if (enemyId && JmgUtility::SimpleDistance(Commands->Get_Position(damager),Commands->Get_Position(obj)) > targetDistance)
		return;
	enemyId = Commands->Get_ID(damager);
	enemySeen = 30;
}
void JMG_Utility_AI_Follow_Player_On_Poke::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason)
{
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{
		if (Commands->Get_Health(obj) <= 0)
			return;
		GameObject *player = Commands->Find_Object(playerObjectId);
		if (player)
			ShowPlayerMessage(player,messageFollowerLostId,messageColor);
		SendCustom(obj,lostCustom);
		GiveUpOnPlayer(obj,true);
	}
}
void JMG_Utility_AI_Follow_Player_On_Poke::Poked(GameObject *obj, GameObject *poker)
{
	int playerId = JmgUtility::JMG_Get_Player_ID(poker);
	if (pokerPlayerId)
	{
		if (pokerPlayerId == playerId)
			ShowPlayerMessage(poker,messageFollowingYouId,messageColor);
		else
			ShowPlayerMessage(poker,messageFollowingPlayerId,messageColor);
		return;
	}
	if (JMG_Utility_AI_Follow_Player_On_Poke_Controller::maxFollowers > 0 && JMG_Utility_AI_Follow_Player_On_Poke_Controller::followingPlayer[playerId] >= JMG_Utility_AI_Follow_Player_On_Poke_Controller::maxFollowers)
	{
		if (messageMaxFollowersId)
			Set_HUD_Help_Text_Player_Text(poker,7234,formatMaxFollowingString(messageMaxFollowers,JMG_Utility_AI_Follow_Player_On_Poke_Controller::maxFollowers,JMG_Utility_AI_Follow_Player_On_Poke_Controller::maxFollowers != 1 ? "s " : " "),messageColor);
		return;
	}
	if (hurt)
	{
		ShowPlayerMessage(poker,messageHealingRequiredId,messageColor);
		return;
	}
	currentFollowTime = 0;
	currentIdleTime = 0;
	SendCustom(obj,pokedCustom);
	pokerPlayerId = playerId;
	playerObjectId = Commands->Get_ID(poker);
	JMG_Utility_AI_Follow_Player_On_Poke_Controller::followingPlayer[playerId]++;
	if (messagePokeFollowerId)
		Set_HUD_Help_Text_Player_Text(poker,7234,formatMaxFollowingString(messagePokeFollower,JMG_Utility_AI_Follow_Player_On_Poke_Controller::followingPlayer[playerId],JMG_Utility_AI_Follow_Player_On_Poke_Controller::maxFollowers),messageColor);
	Commands->Set_Player_Type(obj,Commands->Get_Player_Type(poker));
	Commands->Enable_HUD_Pokable_Indicator(obj,false);
}
void JMG_Utility_AI_Follow_Player_On_Poke::Destroyed(GameObject *obj)
{
	if (detached)
		return;
	detached = true;
	GameObject *player = Commands->Find_Object(playerObjectId);
	if (player)
		ShowPlayerMessage(player,messageFollowerKilledId,messageColor);
	JMG_Utility_AI_Follow_Player_On_Poke_Controller::followingPlayer[pokerPlayerId]--;
	pokerPlayerId = 0;
	SendCustom(obj,killedCustom);
}
void JMG_Utility_AI_Follow_Player_On_Poke::Detach(GameObject *obj)
{
	if (detached)
		return;
	detached = true;
	JMG_Utility_AI_Follow_Player_On_Poke_Controller::followingPlayer[pokerPlayerId]--;
	pokerPlayerId = 0;
}
void JMG_Utility_AI_Follow_Player_On_Poke::GiveUpOnPlayer(GameObject *obj,bool pokeable)
{
	JMG_Utility_AI_Follow_Player_On_Poke_Controller::followingPlayer[pokerPlayerId]--;
	pokerPlayerId = 0;
	playerObjectId = 0;
	Commands->Enable_HUD_Pokable_Indicator(obj,pokeable);
	Commands->Set_Player_Type(obj,-2);
}
void JMG_Utility_AI_Follow_Player_On_Poke::ForgetEnemy(GameObject *obj)
{
	enemyId = 0;
	Commands->Action_Reset(obj,100.0f);
}
void JMG_Utility_AI_Follow_Player_On_Poke::SendCustom(GameObject *obj,int custom)
{
	if (!custom)
		return;
	GameObject *controller = sendCustomId ? Commands->Find_Object(sendCustomId) : obj;
	if (controller)
		Commands->Send_Custom_Event(obj,controller,custom,0,0);
}
void JMG_Utility_AI_Follow_Player_On_Poke::AttackTarget(GameObject *obj,GameObject *follow,GameObject *target,Vector3 location,float speed,float distance)
{
	if (follow != lastFollow || lastTarget != target || JmgUtility::SimpleDistance(location,lastLocation) > 0.0f)
		actionUpdate = 0;
	if (actionUpdate)
		return;
	lastTarget = target;
	lastFollow = follow;
	lastLocation = location;
	actionUpdate = 15;
	ActionParamsStruct params;
	if (follow)
		params.Set_Movement(follow,speed,distance,false);
	else
		params.Set_Movement(location,speed,distance,false);
	params.MovePathfind = true;
	Commands->Action_Goto(obj,params);
	params.Set_Basic(this,100,100,AI_STATE_COMBAT);
	params.AttackCheckBlocked = true;
	if (target)
	{
		params.Set_Attack(target,weaponRange*0.95f,0.0f,true);
		Commands->Action_Attack(obj,params);
	}
}
char *JMG_Utility_AI_Follow_Player_On_Poke::formatMaxFollowingString(const char *format,...)
{
	static char displayMsg[256];
	va_list args;
	va_start(args,format);
	vsprintf(displayMsg,format,args);
	va_end(args);
	return displayMsg;
}
void JMG_Utility_AI_Follow_Player_On_Poke::ShowPlayerMessage(GameObject *player,int stringId,Vector3 color)
{
	if (!stringId)
		return;
	Set_HUD_Help_Text_Player(player,stringId,color);
}
void JMG_Utility_Timer_Damage_And_Teleport::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,Get_Float_Parameter("Delay"),1);
}
void JMG_Utility_Timer_Damage_And_Teleport::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		GameObject *target = Commands->Find_Object(Get_Int_Parameter("TeleportToObjectID"));
		if (target)
		{
			Commands->Set_Position(obj,Commands->Get_Position(target));
			Commands->Set_Facing(obj,Commands->Get_Facing(target));
		}
		else
			Commands->Set_Position(obj,Get_Vector3_Parameter("Location"));
		Commands->Apply_Damage(obj,Get_Float_Parameter("DamageAmount"),Get_Parameter("DamageWarhead"),Get_Int_Parameter("DamageSelf") ? obj : NULL);
	}
}
void JMG_Utility_Zone_Send_Custom_If_Has_Script::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(enter) == -4)
		return;
	if (!Is_Script_Attached(enter,Get_Parameter("ScriptName")))
		return;
	int id = Get_Int_Parameter("ID");
	GameObject *object = id ? (id == -1 ? enter : Commands->Find_Object(id)) : obj;
	if (object)
		Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	if (Get_Int_Parameter("TriggerOnce"))
		Destroy_Script();
}
void JMG_Utility_Damage_When_Outside_Of_Range::Created(GameObject *obj)
{
	for (int x = 0;x < 128;x++)
	{
		warnTime[x] = 0;
		screenEffectOn[x] = false;
	}
	warnDistance = Get_Float_Parameter("Warn_Distance");
	warnDistance *= warnDistance;
	damageDistance = Get_Float_Parameter("Damage_Distance");
	damageDistance *= damageDistance;
	vehicleWarnDistance = Get_Float_Parameter("Vehicle_Warn_Distance");
	vehicleWarnDistance *= vehicleWarnDistance;
	vehicleDamageDistance = Get_Float_Parameter("Vehicle_Damage_Distance");
	vehicleDamageDistance *= vehicleDamageDistance;
	aircraftWarnDistance = Get_Float_Parameter("Aircraft_Warn_Distance");
	aircraftWarnDistance *= aircraftWarnDistance;
	aircraftDamageDistance = Get_Float_Parameter("Aircraft_Damage_Distance");
	aircraftDamageDistance *= aircraftDamageDistance;
	maxSurviveDistance = Get_Float_Parameter("Max_Survive_Distance");
	maxSurviveDistance *= maxSurviveDistance;
	leavingFieldStringId = Get_String_ID_By_Desc(Get_Parameter("Warning_StringID"));
	errorMessageColor = Get_Vector3_Parameter("Warning_Color");
	screenFadeColor = Get_Vector3_Parameter("Screen_Fade_Color");
	sprintf(damageWarhead,"%s",Get_Parameter("DamageWarhead"));
	sprintf(instantKillWarhead,"%s",Get_Parameter("InstantKillWarhead"));
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Damage_When_Outside_Of_Range::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1)
	{
		Vector3 myPos = Commands->Get_Position(obj);
		for (int x = 1;x < 128;x++)
		{
			if (warnTime[x])
				warnTime[x]--;
			GameObject *player = Get_GameObj(x);
			if (!player)
				continue;
			float dist = JmgUtility::SimpleDistance(Commands->Get_Position(player),myPos);
			GameObject *playerVehicle = Get_Vehicle(player);
			if (!playerVehicle)
			{
				if (dist > warnDistance)
				{
					screenEffectOn[x] = true;
					float TempDistCalc = 1-(dist-maxSurviveDistance)/(warnDistance-maxSurviveDistance);
					float inverted = 1-TempDistCalc;
					Set_Screen_Fade_Color_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].X*inverted+screenFadeColor.X,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Y*inverted+screenFadeColor.Y,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Z*inverted+screenFadeColor.Z,0.1f);
					Set_Screen_Fade_Opacity_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::opacity[x]*inverted+TempDistCalc,0.1f);
				}
				else if (screenEffectOn[x])
				{
					screenEffectOn[x] = false;
					Set_Screen_Fade_Color_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].X,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Y,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Z,0.1f);
					Set_Screen_Fade_Opacity_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::opacity[x],0.1f);
				}
				if (dist >= warnDistance && !warnTime[x])
				{
					Set_HUD_Help_Text_Player(player,leavingFieldStringId,errorMessageColor);
					warnTime[x] = 30;
				}
				if (dist >= damageDistance)
					Commands->Apply_Damage(player,0.25f,damageWarhead,player);
			}
			else
			{
				if (Is_VTOL(playerVehicle))
				{
					if (dist > aircraftWarnDistance)
					{
						screenEffectOn[x] = true;
						float TempDistCalc = 1-(dist-maxSurviveDistance)/(aircraftWarnDistance-maxSurviveDistance);
						float inverted = 1-TempDistCalc;
						Set_Screen_Fade_Color_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].X*inverted+screenFadeColor.X,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Y*inverted+screenFadeColor.Y,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Z*inverted+screenFadeColor.Z,0.1f);
						Set_Screen_Fade_Opacity_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::opacity[x]*inverted+TempDistCalc,0.1f);
					}
					else if (screenEffectOn[x])
					{
						screenEffectOn[x] = false;
						Set_Screen_Fade_Color_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].X,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Y,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Z,0.1f);
						Set_Screen_Fade_Opacity_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::opacity[x],0.1f);
					}
					if (dist >= aircraftWarnDistance && !warnTime[x])
					{
						Set_HUD_Help_Text_Player(player,leavingFieldStringId,errorMessageColor);
						warnTime[x] = 30;
					}
				}
				else
				{
					if (dist > vehicleWarnDistance)
					{
						screenEffectOn[x] = true;
						float TempDistCalc = 1-(dist-maxSurviveDistance)/(vehicleWarnDistance-maxSurviveDistance);
						float inverted = 1-TempDistCalc;
						Set_Screen_Fade_Color_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].X*inverted+screenFadeColor.X,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Y*inverted+screenFadeColor.Y,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Z*inverted+screenFadeColor.Z,0.1f);
						Set_Screen_Fade_Opacity_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::opacity[x]*inverted+TempDistCalc,0.1f);
					}
					else if (screenEffectOn[x])
					{
						screenEffectOn[x] = false;
						Set_Screen_Fade_Color_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].X,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Y,JMG_Utility_Set_Screen_Color_Fade_Controller::color[x].Z,0.1f);
						Set_Screen_Fade_Opacity_Player(player,JMG_Utility_Set_Screen_Color_Fade_Controller::opacity[x],0.1f);
					}
					if (dist >= vehicleWarnDistance && !warnTime[x])
					{
						Set_HUD_Help_Text_Player(player,leavingFieldStringId,errorMessageColor);
						warnTime[x] = 30;
					}
				}
				if (Is_VTOL(playerVehicle))
				{
					if (dist >= aircraftDamageDistance && Get_Vehicle_Driver(playerVehicle) == player)
						Commands->Apply_Damage(playerVehicle,1.0f,damageWarhead,player);
				}
				else if (dist >= vehicleDamageDistance && Get_Vehicle_Driver(playerVehicle) == player)
					Commands->Apply_Damage(playerVehicle,1.0f,damageWarhead,player);
			}
			if (dist >= maxSurviveDistance)
				if (playerVehicle)
					Commands->Apply_Damage(playerVehicle,99999.0f,instantKillWarhead,player);
				else
					Commands->Apply_Damage(player,99999.0f,instantKillWarhead,player);
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Send_Custom_When_No_More_Presets_Of_Type_Exist::Created(GameObject *obj)
{
	sprintf(presetName,"%s",Get_Parameter("PresetName"));
	enableCustom = Get_Int_Parameter("EnableOrDisableCustom");
	enabled = Get_Int_Parameter("StartsEnabled") ? true : false;
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Send_Custom_When_No_More_Presets_Of_Type_Exist::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == enableCustom)
	{
		enabled = param ? true : false;
	}
}
void JMG_Utility_Send_Custom_When_No_More_Presets_Of_Type_Exist::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (enabled)
		{
			bool found = false;
			for (SLNode<BaseGameObj> *current = GameObjManager::GameObjList.Head();current;current = current->Next())
			{
				GameObject* o = current->Data()->As_ScriptableGameObj();
				if (!_stricmp(Commands->Get_Preset_Name(o),presetName))
					found = true;
			}
			if (!found)
			{
				int id = Get_Int_Parameter("ID");
				GameObject *object = id ? Commands->Find_Object(id) : obj;
				if (object)
					Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
				this->Destroy_Script();
				return;
			}
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Send_Custom_When_Player_Outside_Of_Range::Created(GameObject *obj)
{
	sleeping = 0;
	distance = Get_Float_Parameter("Distance");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("Custom");
	param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	distance *= distance;
	triggerOnce = Get_Int_Parameter("TriggerOnce") ? true : false;
	sleepTime = Get_Int_Parameter("SleepTime");
	maxRange = Get_Float_Parameter("MaxRange");
	maxRange *= maxRange;
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Send_Custom_When_Player_Outside_Of_Range::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1)
	{
		if (sleeping)
			sleeping--;
		else
			for (int x = 1;x < 128;x++)
			{
				GameObject *player = Get_GameObj(x);
				if (!player)
					continue;
				float playerDist = JmgUtility::SimpleDistance(Commands->Get_Position(player),Commands->Get_Position(obj));
				if (playerDist > distance && (!maxRange || playerDist <= maxRange))
				{
					GameObject *object = id ? Commands->Find_Object(id) : obj;
					if (object)
						Commands->Send_Custom_Event(obj,object,custom,param,delay);
					if (triggerOnce)
					{
						this->Destroy_Script();
						return;
					}
				}
			}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Set_Team_To_Neutral_When_No_Armor::Created(GameObject *obj)
{
	hurt = false;
	originalPlayerType = Commands->Get_Player_Type(obj);
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Set_Team_To_Neutral_When_No_Armor::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (hurt && Commands->Get_Shield_Strength(obj) >= Commands->Get_Max_Shield_Strength(obj))
		{
			hurt = false;
			Commands->Set_Player_Type(obj,originalPlayerType);
		}
		if (!hurt && !Commands->Get_Shield_Strength(obj))
		{
			hurt = true;
			Commands->Set_Player_Type(obj,-2);
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist::Created(GameObject *obj)
{
	team = Get_Int_Parameter("Team");
	enableCustom = Get_Int_Parameter("EnableOrDisableCustom");
	enabled = Get_Int_Parameter("StartsEnabled") ? true : false;
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == enableCustom)
	{
		enabled = param ? true : false;
	}
}
void JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (enabled)
		{
			bool found = false;
			for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
			{
				GameObject* o = current->Data()->As_ScriptableGameObj();
				if (Commands->Get_Player_Type(o) == team && !Is_Script_Attached(o,"JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist_Ignore"))
					found = true;
			}
			if (!found)
			{
				int id = Get_Int_Parameter("ID");
				GameObject *object = id ? Commands->Find_Object(id) : obj;
				if (object)
					Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
				this->Destroy_Script();
				return;
			}
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Custom_Create_Sound_At_Object_Bone::Created(GameObject *obj)
{
	sprintf(sound,"%s",Get_Parameter("Sound"));
	sprintf(bone,"%s",Get_Parameter("Bone"));
	custom = Get_Int_Parameter("Custom");
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Custom_Create_Sound_At_Object_Bone::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Create_3D_Sound_At_Bone(sound,obj,bone);
	}
}
void JMG_Utility_Custom_Send_Custom_On_Player_Count::Created(GameObject *obj)
{
	runningCount = 0;
	custom = Get_Int_Parameter("Custom");
	resetCustom = Get_Int_Parameter("ResetCustom");
	baseCount = Get_Int_Parameter("BaseCount");
	playerCount = Get_Int_Parameter("PlayerCount");
}
void JMG_Utility_Custom_Send_Custom_On_Player_Count::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == resetCustom)
	{
		runningCount = 0;
	}
	if (message == custom)
	{
		runningCount++;
		if (runningCount < baseCount+playerCount*Get_Player_Count())
			return;
		int id = Get_Int_Parameter("ID");
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
		if (object)
			Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("SendCustom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
		this->Destroy_Script();
	}
}
void JMG_Utility_Killed_By_Player_Send_Custom::Killed(GameObject *obj,GameObject *killer)
{
	if (!Commands->Is_A_Star(killer))
		return;
	int id = Get_Int_Parameter("ID");
	GameObject *object = id ? (id == -1 ? killer : Commands->Find_Object(id)) : obj;
	Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	Destroy_Script();
}
void JMG_Utility_AI_Guardian_Ignored::Created(GameObject *obj)
{
}
void JMG_Utility_AI_Vehicle_Ignored::Created(GameObject *obj)
{
}
void JMG_Utility_Killed_By_PresetID_Send_Custom::Killed(GameObject *obj,GameObject *killer)
{
	if (Commands->Get_Preset_ID(killer) != Get_Int_Parameter("PresetID"))
		return;
	int id = Get_Int_Parameter("ID");
	GameObject *object = id ? (id == -1 ? killer : Commands->Find_Object(id)) : obj;
	Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	Destroy_Script();
}
void JMG_Utility_Created_Override_AI_Soldier_Muzzle_Direction::Created(GameObject *obj)
{
	if (obj->As_SoldierGameObj())
		obj->As_SoldierGameObj()->Set_Override_Muzzle_Direction(true);
	this->Destroy_Script();
}
void JMG_Utility_Custom_Send_Custom_On_Player_Count_HUD::Created(GameObject *obj)
{
	hudStringId = Get_Int_Parameter("HudStringId");
	sprintf(hudMessage,"%s",Get_Translated_String(hudStringId));
	hudStringColor = Get_Vector3_Parameter("HudStringColor");
	runningCount = 0;
	custom = Get_Int_Parameter("Custom");
	resetCustom = Get_Int_Parameter("ResetCustom");
	baseCount = Get_Int_Parameter("BaseCount");
	playerCount = Get_Int_Parameter("PlayerCount");
}
void JMG_Utility_Custom_Send_Custom_On_Player_Count_HUD::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == resetCustom)
	{
		runningCount = 0;
	}
	if (message == custom)
	{
		runningCount++;
		int requiredCount = baseCount+playerCount*Get_Player_Count();
		JmgUtility::SetHUDHelpText(hudStringId,formatHUDMessage(hudMessage,runningCount,requiredCount),hudStringColor);
		if (runningCount < requiredCount)
			return;
		int id = Get_Int_Parameter("ID");
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
		if (object)
			Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("SendCustom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
		this->Destroy_Script();
	}
}
char *JMG_Utility_Custom_Send_Custom_On_Player_Count_HUD::formatHUDMessage(const char *format,...)
{
	static char displayMsg[256];
	va_list args;
	va_start(args,format);
	vsprintf(displayMsg,format,args);
	va_end(args);
	return displayMsg;
}
void JMG_Utility_Zone_Send_Custom_If_Has_Weapon_Ammo::Entered(GameObject *obj,GameObject *enter)
{
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")) || Commands->Get_Player_Type(enter) == -4)
		return;
	char weaponName[256];
	sprintf(weaponName,"%s",Get_Parameter("WeaponName"));
	if (!Has_Weapon(enter,weaponName))
		return;
	int totalBullets = Get_Total_Bullets(enter,weaponName);
	int id = Get_Int_Parameter("ID");
	GameObject *object = id ? (id == -1 ? enter : Commands->Find_Object(id)) : obj;
	if (object && totalBullets)
		for (int x = 0;x < totalBullets;x++)
			Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Custom"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	if (Get_Int_Parameter("RemoveWeapon"))
		Remove_Weapon(enter,weaponName);
	if (Get_Int_Parameter("TriggerOnce"))
		Destroy_Script();
}
void JMG_Utility_Custom_Remove_Weapon_From_All_Players::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	sprintf(weaponName,"%s",Get_Parameter("WeaponName"));
}
void JMG_Utility_Custom_Remove_Weapon_From_All_Players::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player || !Has_Weapon(player,weaponName))
				continue;
			Remove_Weapon(player,weaponName);
		}
	}
}
void JMG_Utility_Custom_Destroy_All_Presets_By_Name::Created(GameObject *obj)
{
	sprintf(presetName,"%s",Get_Parameter("PresetName"));
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Destroy_All_Presets_By_Name::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		for (SLNode<BaseGameObj> *current = GameObjManager::GameObjList.Head();current;current = current->Next())
		{
			GameObject* o = current->Data()->As_ScriptableGameObj();
			if (!_stricmp(presetName,Commands->Get_Preset_Name(o)))
				Commands->Destroy_Object(o);
		}
	}
}
void JMG_Utility_Send_Custom_When_Damage_Exceedes_Threshold::Created(GameObject *obj)
{
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("Custom");
	param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	damageThreshold = Get_Float_Parameter("DamageThreshold");
	lockoutTime = Get_Float_Parameter("LockoutTime");
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Send_Custom_When_Damage_Exceedes_Threshold::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (timeRemaining)
		{
			timeRemaining -= 0.1f;
			if (timeRemaining < 0)
				timeRemaining = 0.0f;
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Send_Custom_When_Damage_Exceedes_Threshold::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage < damageThreshold)
		return;
	if (timeRemaining)
		return;
	timeRemaining = lockoutTime;
	GameObject *object = id ? (id == -1 ? damager : Commands->Find_Object(id)) : obj;
	if (object)
		Commands->Send_Custom_Event(obj,object,custom,param,delay);
}
void JMG_Utility_Custom_Send_Translated_String_To_All_Players::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	stringId = Get_Int_Parameter("StringID");
	color = Get_Vector3_Parameter("ColorRGB");
	repeatable = Get_Int_Parameter("Repeatable") ? true : false;
}
void JMG_Utility_Custom_Send_Translated_String_To_All_Players::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		JmgUtility::MessageAllPlayers((int)color.X,(int)color.Y,(int)color.Z,Get_Translated_String(stringId));
		if (!repeatable)
			Destroy_Script();
	}
}
void JMG_Utility_Destroyed_Drop_Powerup::Destroyed(GameObject *obj)
{
	if (!Has_Weapon(obj,Get_Parameter("WeaponPreset")))
		return;
	Vector3 pos = Commands->Get_Position(obj);
	pos.Z += Get_Float_Parameter("HeightAdjust");
	GameObject *powerup = Commands->Create_Object(Get_Parameter("PowerupName"),pos);
	Commands->Set_Facing(powerup,Commands->Get_Facing(obj));
}
void JMG_Utility_Pickup_Attach_Script::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		const char *parameters = Get_Parameter("Params");
		char *params = newstr(parameters);
		char delim = Get_Parameter("Delim")[0];
		unsigned int length = strlen(parameters);
		for (unsigned int x = 0;x < length;x++)
			if (params[x] == delim)
				params[x] = ',';
		Commands->Attach_Script(sender,Get_Parameter("Script"),params);
	}
}
void JMG_Utility_Objective_System_Objective_Status_Update_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		if (!JMG_Utility_Objective_System_Controller::controllerPlaced)
		{
			Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Custom ERROR: Make sure you have a JMG_Utility_Objective_System_Controller placed on the map!");
			return;
		}
		int objectiveId = Get_Int_Parameter("ObjectiveID");
		int objectiveMarkerId = Get_Int_Parameter("NewObjectiveMarkerObjectID");
		GameObject *ObjectiveMarker = objectiveMarkerId == -1 ? sender : Commands->Find_Object(objectiveMarkerId);
		if (!ObjectiveMarker && objectiveMarkerId)
		{
			Console_Input("msg JMG_Utility_Objective_System_Objective_Update_Enter ERROR: Could not find objective marker object!");
			return;
		}
		int stringId = Get_Int_Parameter("NewObjectiveStringID");
		BasicObjectiveSystem.Add_Objective(objectiveId,NewObjectiveSystem::Primary,NewObjectiveSystem::Hidden,0,"",0);
		BasicObjectiveSystem.Set_Objective_Mission(objectiveId,stringId,stringId);
		if (ObjectiveMarker)
			BasicObjectiveSystem.Set_Radar_Blip(objectiveId,ObjectiveMarker,NULL);
		JmgUtility::SetHUDHelpText(stringId,BasicObjectiveSystem.Get_Hud_Help_Text_Color((NewObjectiveSystem::Priority)BasicObjectiveSystem.Get_Objective_Priority(objectiveId)));
	}
}
void JMG_Utility_Objective_System_Objective_Failed_Custom::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Objective_System_Objective_Failed_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		BasicObjectiveSystem.Add_Objective(Get_Int_Parameter("ObjectiveID"),NewObjectiveSystem::Primary,NewObjectiveSystem::Hidden,0,"",0);
		BasicObjectiveSystem.Set_Objective_Status(Get_Int_Parameter("ObjectiveID"),NewObjectiveSystem::Failed);
	}
}
void JMG_Utility_Force_Player_Team_At_Gameover::Created(GameObject *obj)
{
	team = Get_Int_Parameter("Team");
	Commands->Start_Timer(obj,this,1.0,1);
}
void JMG_Utility_Force_Player_Team_At_Gameover::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (The_Game()->Is_Game_Over())
			for (int x = 1;x < 128;x++)
			{
				cPlayer *cplayer = Find_Player(x);
				if (!cplayer)
					continue;
				cplayer->Set_Player_Type(team);
			}
	}
	Commands->Start_Timer(obj,this,1.0,1);
}
void JMG_Utility_AI_Guardian_Generic::Created(GameObject *obj)
{
	flightHeight = Get_Float_Parameter("FlightHeight");
	primaryFire = Get_Int_Parameter("UseSecondaryAttack") ? false : true;
	arriveDistance = Get_Float_Parameter("ArriveDistance");
	arriveDistanceSq = (arriveDistance+1.0f)*(arriveDistance+1.0f);
	dpPosition = Commands->Get_Position(obj);
	EnemyID = 0;
	EnemyTimeOutTime = 0;
	LastPos = Commands->Get_Position(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Enable_Engine(obj,true);
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Utility_AI_Guardian_Generic::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (EnemyTimeOutTime)
		{
			EnemyTimeOutTime--;
			if (!EnemyTimeOutTime)
			{
				EnemyID = 0;
				Goto_Location(obj);
			}
		}
		if (EnemyID)
		{
			GameObject *Target = Commands->Find_Object(EnemyID);
			if (!Target || !Commands->Get_Health(Target) || Commands->Get_Player_Type(Target) == Commands->Get_Player_Type(obj))
			{
				EnemyID = 0;
				Goto_Location(obj);
			}
		}
		Vector3 Pos = Commands->Get_Position(obj);
		Pos.Z -= flightHeight;
		if (JmgUtility::SimpleDistance(Pos,LastPos) < arriveDistanceSq)
		{
			if (Get_A_Defense_Point(&dpPosition))
				Goto_Location(obj);
		}
		LastPos = Pos;
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Utility_AI_Guardian_Generic::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (Is_Script_Attached(seen,"JMG_Utility_AI_Guardian_Ignored"))
		return;
	if (!EnemyID || !EnemyTimeOutTime)
	{
		EnemyID = Commands->Get_ID(seen);
		EnemyTimeOutTime = Commands->Get_Random_Int(2,5);
		Goto_Location(obj);
	}
	else if (EnemyID == Commands->Get_ID(seen))
		EnemyTimeOutTime = Commands->Get_Random_Int(2,5);
}
void JMG_Utility_AI_Guardian_Generic::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0 || (EnemyID && EnemyTimeOutTime) || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	EnemyID = Commands->Get_ID(damager);
	EnemyTimeOutTime = Commands->Get_Random_Int(2,5);
	Goto_Location(obj);
}
void JMG_Utility_AI_Guardian_Generic::Goto_Location(GameObject *obj)
{
	ActionParamsStruct params;
	Commands->Action_Reset(obj,100);
	Vector3 GoToPos = dpPosition;
	if (flightHeight)
		GoToPos.Z += flightHeight;
	params.MovePathfind = flightHeight ? false : true;
	params.Set_Movement(GoToPos,Get_Float_Parameter("WanderSpeed"),arriveDistance,false);
	params.ShutdownEngineOnArrival = Get_Int_Parameter("TurnOffEngineOnArrival") ? true : false;
	params.Set_Basic(this,100,10);
	GameObject *Target = Commands->Find_Object(EnemyID);
	if (Target)
	{
		if (Get_Int_Parameter("FaceTarget"))
			params.Set_Face_Location(Commands->Get_Position(Target),5.0f);
		else
			params.AttackFaceTarget = false;
		params.Set_Attack(Target,Get_Float_Parameter("FireRange"),0,primaryFire);
		params.AttackCheckBlocked = Get_Int_Parameter("CheckBlocked") ? true : false;
		Commands->Action_Attack(obj,params);
	}
	else
	{
		if (Get_Int_Parameter("FaceTarget"))
			params.Set_Face_Location(GoToPos,5.0f);
		else
			params.AttackFaceTarget = false;
		Commands->Action_Goto(obj,params);
	}
}
void JMG_Utility_Custom_Create_Object_In_Front_Of_Self::Created(GameObject *obj)
{
	sprintf(presetName,"%s",Get_Parameter("PresetName"));
	custom = Get_Int_Parameter("Custom");
	distance = Get_Float_Parameter("Distance");
	height = Get_Float_Parameter("Height");
	rotation = Get_Float_Parameter("Rotation");
}
void JMG_Utility_Custom_Create_Object_In_Front_Of_Self::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Vector3 pos = Commands->Get_Position(obj);
		float facing = Commands->Get_Facing(obj);
		pos.X += cos(facing*PI180)*distance;
		pos.Y += sin(facing*PI180)*distance;
		facing += rotation;
		pos.Z += height;
		GameObject *object = Commands->Create_Object(presetName,pos);
		Commands->Set_Facing(object,facing);
	}
}
void JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist_Ignore::Created(GameObject *obj)
{
}
void JMG_Utility_Custom_Damage_All_Vehicles_On_Team::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	team = Get_Int_Parameter("Team");
	damage = Get_Float_Parameter("Damage");
	sprintf(warhead,"%s",Get_Parameter("Warhead"));
}
void JMG_Utility_Custom_Damage_All_Vehicles_On_Team::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
		{
			SmartGameObj* o = current->Data();
			if (o->As_VehicleGameObj() && !CheckPlayerType(o,team))
				Commands->Apply_Damage(o,damage,warhead,o);
		}
	}
}
void JMG_Utility_Kill_Unit_If_Not_Moving_Enough::Created(GameObject *obj)
{
	resetTime = time = (int)(Get_Float_Parameter("Time")*10);
	distance = Get_Float_Parameter("Distance");
	distance *= distance;
	lastPos = Commands->Get_Position(obj);
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Kill_Unit_If_Not_Moving_Enough::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (JmgUtility::SimpleDistance(Commands->Get_Position(obj),lastPos) < distance)
		{
			time--;
			if (!time)
				Commands->Apply_Damage(obj,999999.9f,"BlamoKiller",obj);
		}
		else
		{
			lastPos = Commands->Get_Position(obj);
			time = resetTime;
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Custom_Display_Briefing_Message_StringID::Created(GameObject *obj)
{
	triggered = false;
	BriefingText = NULL;
	CurrentNode = NULL;
	AddNewTextNode();
	char TextString[16384];
	sprintf(TextString,"%s",Get_Translated_String(Get_String_ID_By_Desc(Get_Parameter("StringID"))));
	int Count = 0;
	for (int x = 0;x < 16384;x++)
	{
		if ((Count > 150 && TextString[x] == ' ') || Count == 210)
		{// String getting too long, start a new one.
			CurrentNode->Text[Count] = '\0';
			AddNewTextNode();
			Count = 0;
		}
		else if (TextString[x] == '\n' || TextString[x] == '\0')
		{
			CurrentNode->Text[Count] = '\0';
			break;// end of line your done
		}
		else if (TextString[x] == '\f')
		{
			CurrentNode->Text[Count] = '\0';
			AddNewTextNode();
			Count = 0;
		}
		else
		{
			CurrentNode->Text[Count] = TextString[x];
			Count++;
		}
	}
	CurrentNode = BriefingText;
}
void JMG_Utility_Custom_Display_Briefing_Message_StringID::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!CurrentNode)
		{
			RemoveTextNodes();
			Destroy_Script();
			return;
		}
		if (CurrentNode->Text[0] != '\0')
			for (int x = 1;x < 128;x++)
			{
				GameObject *Player = Get_GameObj(x);
				if (Player)
					JmgUtility::DisplayChatMessage(Player,(int)Get_Vector3_Parameter("MessageColor").X,(int)Get_Vector3_Parameter("MessageColor").Y,(int)Get_Vector3_Parameter("MessageColor").Z,CurrentNode->Text);
			}
		CurrentNode = CurrentNode->next;
		Commands->Start_Timer(obj,this,Get_Float_Parameter("LineDelayTime"),1);
	}
}
void JMG_Utility_Custom_Display_Briefing_Message_StringID::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		if (triggered)
			return;
		triggered = true;
		Commands->Start_Timer(obj,this,0.0f,1);
		Commands->Create_2D_Sound(Get_Parameter("UISound"));
	}
}
void JMG_Utility_Custom_Display_Briefing_Message_StringID::Destroyed(GameObject *obj)
{
	RemoveTextNodes();
}
void JMG_Utility_Custom_Display_Briefing_Message_StringID::AddNewTextNode()
{
	if (!BriefingText)
	{
		BriefingText = new BriefingTextNode;
		sprintf(BriefingText->Text,"\0");
		BriefingText->next = NULL;
		CurrentNode = BriefingText;
		return;
	}
	BriefingTextNode *Current = BriefingText;
	while (Current)
	{
		if (!Current->next)
		{
			Current->next = new BriefingTextNode;
			Current = Current->next;
			sprintf(Current->Text,"\0");
			Current->next = NULL;
			CurrentNode = Current;
			return;
		}
		Current = Current->next;
	}
}
void JMG_Utility_Custom_Display_Briefing_Message_StringID::RemoveTextNodes()
{
	BriefingTextNode *Current = BriefingText,*Temp;
	while (Current)
	{
		Temp = Current;
		Current = Current->next;
		delete Temp;
	}
	BriefingText = NULL;
	CurrentNode = NULL;
}
void JMG_Utility_Destroy_Objects_In_ID_Range_On_Killed::Killed(GameObject *obj,GameObject *killer)
{
	int end = Get_Int_Parameter("EndID");
	for (int x = Get_Int_Parameter("StartID");x <= end;x++)
	{
		GameObject *destroy = Commands->Find_Object(x);
		if (destroy)
			Commands->Destroy_Object(destroy);
	}
}
void JMG_Utility_Timer_Custom::Created(GameObject *obj)
{
	time = Get_Float_Parameter("Time");
	repeat = Get_Int_Parameter("Repeat") ? true : false;
	id = Get_Int_Parameter("ID");
	message = Get_Int_Parameter("Message");
	param = Get_Int_Parameter("Param");
	Commands->Start_Timer(obj,this,time,1);
}
void JMG_Utility_Timer_Custom::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		GameObject *object = !id ? obj : Commands->Find_Object(id);
		if (object)
			Commands->Send_Custom_Event(obj,object,message,param,0);
		if (repeat)
			Commands->Start_Timer(obj,this,time,1);
	}
}
void JMG_Utility_Zone_Change_Character_Model::Created(GameObject *obj)
{
	playerType = Get_Int_Parameter("PlayerType");
	sprintf(newModel,"%s",Get_Parameter("NewModel"));
}
void JMG_Utility_Zone_Change_Character_Model::Entered(GameObject *obj,GameObject *enterer)
{
	if (CheckPlayerType(enterer,playerType) || Commands->Get_Player_Type(enterer) == -4)
		return;
	Commands->Set_Model(enterer,newModel);
}
void JMG_Utility_Timer_Change_Character_Models_On_Team::Created(GameObject *obj)
{
	time = Get_Float_Parameter("Time");
	repeat = Get_Int_Parameter("Repeat") ? true : false;
	playerType = Get_Int_Parameter("PlayerType");
	sprintf(newModel,"%s",Get_Parameter("NewModel"));
	enableCustom = Get_Int_Parameter("EnableCustom");
	enabled = Get_Int_Parameter("StartsEnabled") ? true : false;
	if (enabled)
		Commands->Start_Timer(obj,this,time,1);
}
void JMG_Utility_Timer_Change_Character_Models_On_Team::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player || CheckPlayerType(player,playerType) || Commands->Get_Player_Type(player) == -4 || !_stricmp(Get_Model(player),newModel))
				continue;
			Commands->Set_Model(player,newModel);
		}
		if (repeat)
			Commands->Start_Timer(obj,this,time,1);
	}
}
void JMG_Utility_Timer_Change_Character_Models_On_Team::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (enableCustom == message)
	{
		bool tEnabled = param ? true : false;
		if (enabled == tEnabled)
			return;
		enabled = tEnabled;
		if (!enabled)
			Stop_Timer(obj,1);
		else
			Commands->Start_Timer(obj,this,time,1);
	}
}
void JMG_Utility_Timer_Change_Character_Models_On_HP::Created(GameObject *obj)
{
	time = Get_Float_Parameter("Time");
	repeat = Get_Int_Parameter("Repeat") ? true : false;
	playerType = Get_Int_Parameter("PlayerType");
	sprintf(newModel,"%s",Get_Parameter("NewModel"));
	enableCustom = Get_Int_Parameter("EnableCustom");
	minHp = Get_Float_Parameter("MinHP");
	maxHp = Get_Float_Parameter("MaxHP");
	enabled = Get_Int_Parameter("StartsEnabled") ? true : false;
	if (enabled)
		Commands->Start_Timer(obj,this,time,1);
}
void JMG_Utility_Timer_Change_Character_Models_On_HP::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			float hp = Get_Max_Hitpoints(player);
			if (!player || CheckPlayerType(player,playerType) || Commands->Get_Player_Type(player) == -4 || hp < minHp || hp > maxHp || !_stricmp(Get_Model(player),newModel))
				continue;
			Commands->Set_Model(player,newModel);
		}
		if (repeat)
			Commands->Start_Timer(obj,this,time,1);
	}
}
void JMG_Utility_Timer_Change_Character_Models_On_HP::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (enableCustom == message)
	{
		bool tEnabled = param ? true : false;
		if (enabled == tEnabled)
			return;
		enabled = tEnabled;
		if (!enabled)
			Stop_Timer(obj,1);
		else
			Commands->Start_Timer(obj,this,time,1);
	}
}
void JMG_Utility_Destroy_Send_Custom::Destroyed(GameObject *obj)
{
	int id = Get_Int_Parameter("ID");
	GameObject *object = id ? Commands->Find_Object(id) : obj;
	if (object)
		Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Message"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
}
void JMG_Utility_Set_Vehicle_Collisions_Ghost_When_Unoccupied::Created(GameObject *obj)
{
	if (!obj->As_PhysicalGameObj() || !obj->As_VehicleGameObj())
	{
		Console_Input("msg JMG_Utility_Set_Vehicle_Collisions_Ghost_When_Unoccupied ERROR: This script is only for vehicles!");
		Destroy_Script();
		return;
	}
	ghost = false;
	attemptingCollidable = false;
	myCollisionGroup = obj->As_PhysicalGameObj()->Peek_Physical_Object()->Get_Collision_Group();
	Commands->Start_Timer(obj,this,0.25f,1);
}
void JMG_Utility_Set_Vehicle_Collisions_Ghost_When_Unoccupied::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (ghost && attemptingCollidable)
		{
			MoveablePhysClass *mphys = obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass();
			obj->As_PhysicalGameObj()->Peek_Physical_Object()->Set_Collision_Group(myCollisionGroup);
			if (mphys->Can_Teleport(Matrix3D(Commands->Get_Position(obj))))
			{
				ghost = false;
				attemptingCollidable = false;
			}
			else
				obj->As_PhysicalGameObj()->Peek_Physical_Object()->Set_Collision_Group(Collision_Group_Type::SOLDIER_GHOST_COLLISION_GROUP);
		}
		Commands->Start_Timer(obj,this,0.25f,1);
	}
}
void JMG_Utility_Set_Vehicle_Collisions_Ghost_When_Unoccupied::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (ghost && !attemptingCollidable)
			attemptingCollidable = true;
	}
	if (message == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (!Get_Vehicle_Occupant_Count(obj) && !ghost)
		{
			ghost = true;
			attemptingCollidable = false;
			myCollisionGroup = obj->As_PhysicalGameObj()->Peek_Physical_Object()->Get_Collision_Group();
			obj->As_PhysicalGameObj()->Peek_Physical_Object()->Set_Collision_Group(Collision_Group_Type::SOLDIER_GHOST_COLLISION_GROUP);
		}
	}
}
void JMG_Utility_Attach_Script_To_All_Players::Created(GameObject *obj)
{
	sprintf(scriptName,"%s",Get_Parameter("Script"));
	sprintf(params,"%s",Get_Parameter("Params"));
	char delim = Get_Parameter("Delim")[0];
	unsigned int length = strlen(params);
	for (unsigned int x = 0;x < length;x++)
		if (params[x] == delim)
			params[x] = ',';
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Attach_Script_To_All_Players::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player || Is_Script_Attached(player,scriptName))
				continue;
			Commands->Attach_Script(player,scriptName,params);
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Send_Custom_On_Powerup_Pickup::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		int id = Get_Int_Parameter("ID");
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
		if (object)
			Commands->Send_Custom_Event(obj,object,Get_Int_Parameter("Message"),Get_Int_Parameter("Param"),Get_Float_Parameter("Delay"));
	}
}
void JMG_Utility_Set_Bullets_On_Custom_Or_Damage::Created(GameObject *obj)
{
	sprintf(setWeapon,"%s",Get_Parameter("WeaponName"));
	if (!_stricmp(setWeapon,"null"))
		sprintf(setWeapon,"%s",Get_Current_Weapon(obj));
	repeat = Get_Int_Parameter("Repeat") ? true : false;
	custom = Get_Int_Parameter("Custom");
	triggerOnDamage = Get_Int_Parameter("TriggerOnDamage") ? true : false;
	fullClip = Get_Int_Parameter("FullClip") ? true : false;
	setBackpackBullets = Get_Int_Parameter("SetBackpackBullets");
	if (Get_Int_Parameter("StartWithNoBullets"))
	{
		Set_Bullets(obj,setWeapon,0);
		Set_Clip_Bullets(obj,setWeapon,0);
	}
}
void JMG_Utility_Set_Bullets_On_Custom_Or_Damage::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0.0f || !triggerOnDamage)
		return;
	UpdateBullets(obj);
}
void JMG_Utility_Set_Bullets_On_Custom_Or_Damage::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		UpdateBullets(obj);
	}
}
void JMG_Utility_Set_Bullets_On_Custom_Or_Damage::UpdateBullets(GameObject *obj)
{
	if (Has_Weapon(obj,setWeapon))
	{
		if (fullClip)
			Set_Bullets(obj,setWeapon,Get_Max_Bullets(obj,setWeapon));
		Set_Clip_Bullets(obj,setWeapon,setBackpackBullets);
	}
	if (!repeat)
		Destroy_Script();
}
void JMG_Utility_Custom_Damage_All_Presets::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	sprintf(preset,"%s",Get_Parameter("PresetName"));
	damage = Get_Float_Parameter("Damage");
	sprintf(warhead,"%s",Get_Parameter("Warhead"));
}
void JMG_Utility_Custom_Damage_All_Presets::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
		{
			SmartGameObj* o = current->Data();
			if (!_stricmp(Commands->Get_Preset_Name(o),preset))
				Commands->Apply_Damage(o,damage,warhead,o);
		}
	}
}
void JMG_Utility_Death_Weapon_Create_Object::Killed(GameObject *obj,GameObject *killer)
{
	const char *weap = Get_Current_Weapon(killer);
	if (!weap || _stricmp(weap,Get_Parameter("WeaponPreset")))
		return;
	if (Commands->Get_Random(0.0f,1.0f) > Get_Float_Parameter("Chance"))
		return;
	GameObject *spawned = Commands->Create_Object(Get_Parameter("Preset"),Commands->Get_Position(obj));
	Commands->Set_Facing(spawned,Commands->Get_Facing(obj));
	Commands->Set_Model(obj,"null");
}
void JMG_Utility_Send_Custom_When_Speed_Exceeds_Amount::Created(GameObject *obj)
{
	speed = Get_Float_Parameter("Speed");
	speed *= speed;
	rate = Get_Float_Parameter("Rate");
	repeat = Get_Int_Parameter("Repeat") ? true : false;
	id = Get_Int_Parameter("ID");
	message = Get_Int_Parameter("Message");
	param = Get_Int_Parameter("Param");
	enabled = Get_Int_Parameter("StartsEnabled") ? true : false;
	enableCustom = Get_Int_Parameter("EnableCustom");
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Send_Custom_When_Speed_Exceeds_Amount::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == enableCustom)
	{
		enabled = param ? true : false;
	}
}
void JMG_Utility_Send_Custom_When_Speed_Exceeds_Amount::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (enabled && !obj->As_PhysicalGameObj()->Is_Attached_To_An_Object())
		{
			Vector3 tempSpeed;
			if (obj->As_SoldierGameObj())
				obj->As_SoldierGameObj()->Get_Velocity(tempSpeed);
			else if (obj->As_VehicleGameObj())
				obj->As_VehicleGameObj()->Get_Velocity(tempSpeed);
			if (tempSpeed.Length2() >= speed)
			{
				GameObject *object = !id ? obj : Commands->Find_Object(id);
				if (object)
					Commands->Send_Custom_Event(obj,object,message,param,0);
				if (!repeat)
				{
					Destroy_Script();
					return;
				}
			}
		}
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Send_Custom_When_Speed_Below_Amount::Created(GameObject *obj)
{
	speed = Get_Float_Parameter("Speed");
	speed *= speed;
	rate = Get_Float_Parameter("Rate");
	repeat = Get_Int_Parameter("Repeat") ? true : false;
	id = Get_Int_Parameter("ID");
	message = Get_Int_Parameter("Message");
	param = Get_Int_Parameter("Param");
	enabled = Get_Int_Parameter("StartsEnabled") ? true : false;
	enableCustom = Get_Int_Parameter("EnableCustom");
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Send_Custom_When_Speed_Below_Amount::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == enableCustom)
	{
		enabled = param ? true : false;
	}
}
void JMG_Utility_Send_Custom_When_Speed_Below_Amount::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (enabled && !obj->As_PhysicalGameObj()->Is_Attached_To_An_Object())
		{
			Vector3 tempSpeed;
			if (obj->As_SoldierGameObj())
				obj->As_SoldierGameObj()->Get_Velocity(tempSpeed);
			else if (obj->As_VehicleGameObj())
				obj->As_VehicleGameObj()->Get_Velocity(tempSpeed);
			if (tempSpeed.Length2() < speed)
			{
				GameObject *object = !id ? obj : Commands->Find_Object(id);
				if (object)
					Commands->Send_Custom_Event(obj,object,message,param,0);
				if (!repeat)
				{
					Destroy_Script();
					return;
				}
			}
		}
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Send_Custom_When_Velocity_Exceeds_Amount::Created(GameObject *obj)
{
	velocityFBL = Get_Vector3_Parameter("Velocity[Forward|Backward|Left]");
	velocityFBL = SquareVectorSpecial(velocityFBL);
	velocityRUD = Get_Vector3_Parameter("Velocity[Right|Up|Down]");
	velocityRUD = SquareVectorSpecial(velocityRUD);
	onlyTriggerOn = (SpecialCondition)Get_Int_Parameter("OnlyTriggerOn");
	onlyTriggerOnMinHV = Get_Vector3_Parameter("OnlyTriggerOnMin[Horizontal|Vertical]");
	onlyTriggerOnMinHV = SquareVectorSpecial(onlyTriggerOnMinHV);
	onlyTriggerOnMaxHV = Get_Vector3_Parameter("OnlyTriggerOnMax[Horizontal|Vertical]");
	onlyTriggerOnMaxHV = SquareVectorSpecial(onlyTriggerOnMaxHV);
	rate = Get_Float_Parameter("Rate");
	repeat = Get_Int_Parameter("Repeat") ? true : false;
	id = Get_Int_Parameter("ID");
	message = Get_Int_Parameter("Message");
	param = Get_Int_Parameter("Param");
	enabled = Get_Int_Parameter("StartsEnabled") ? true : false;
	enableCustom = Get_Int_Parameter("EnableCustom");
	Commands->Start_Timer(obj,this,rate,1);
}
void JMG_Utility_Send_Custom_When_Velocity_Exceeds_Amount::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == enableCustom)
	{
		enabled = param ? true : false;
	}
}
void JMG_Utility_Send_Custom_When_Velocity_Exceeds_Amount::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (enabled && !obj->As_PhysicalGameObj()->Is_Attached_To_An_Object())
		{
			Vector3 tempSpeed;
			if (obj->As_SoldierGameObj())
				obj->As_SoldierGameObj()->Get_Velocity(tempSpeed);
			else if (obj->As_VehicleGameObj())
				obj->As_VehicleGameObj()->Get_Velocity(tempSpeed);
			float speedHorizontal,speedVertical = tempSpeed.Z*tempSpeed.Z;
			tempSpeed.Z = 0;
			speedHorizontal = tempSpeed.Length2();

			SpecialCondition specialCondition = DOES_NOT_MEET_CONDITION;
			if (onlyTriggerOn && OnlyTriggerOnTest(speedHorizontal,speedVertical))
				if (onlyTriggerOn == HORIZANTAL_IS_GREATER && speedHorizontal > speedVertical)
					specialCondition = HORIZANTAL_IS_GREATER;
				else if (onlyTriggerOn == VERTICAL_IS_GREATER && speedHorizontal < speedVertical)
					specialCondition = VERTICAL_IS_GREATER;
			PhysControllerClass *controller = obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass()->Get_Controller();
			if (!onlyTriggerOn || (onlyTriggerOn == HORIZANTAL_IS_GREATER &&  specialCondition == onlyTriggerOn))
				if (controller->Get_Move_Forward() > 0 && velocityFBL.X >= 0 && tempSpeed.Length2() > velocityFBL.X)
					SendCustom(obj,1);
				else if (controller->Get_Move_Forward() < 0 && velocityFBL.Y >= 0 && tempSpeed.Length2() > velocityFBL.Y)
					SendCustom(obj,2);
				else if (controller->Get_Move_Left() > 0 && velocityFBL.Z >= 0 && tempSpeed.Length2() > velocityFBL.Z)
					SendCustom(obj,3);
				else if (controller->Get_Move_Left() < 0 && velocityRUD.X >= 0 && tempSpeed.Length2() > velocityRUD.X)
					SendCustom(obj,4);
			if (!onlyTriggerOn || (onlyTriggerOn == VERTICAL_IS_GREATER &&  specialCondition == onlyTriggerOn))
				if (controller->Get_Move_Up() > 0 && velocityRUD.Y >= 0 && speedVertical > velocityRUD.Y)
					SendCustom(obj,5);
				else if (controller->Get_Move_Up() < 0 && velocityRUD.Z >= 0 && speedVertical > velocityRUD.Z)
					SendCustom(obj,6);
		}
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Send_Custom_When_Velocity_Exceeds_Amount::SendCustom(GameObject *obj,int paramOverride)
{
	GameObject *object = !id ? obj : Commands->Find_Object(id);
	if (object)
		Commands->Send_Custom_Event(obj,object,message,param == -1 ? paramOverride : param,0);
	if (!repeat)
		Destroy_Script();
}
Vector3 JMG_Utility_Send_Custom_When_Velocity_Exceeds_Amount::SquareVectorSpecial(Vector3 in)
{
	if (in.X > 0)
		in.X *= in.X;
	if (in.Y > 0)
		in.Y *= in.Y;
	if (in.Z > 0)
		in.Z *= in.Z;
	return in;
}
bool JMG_Utility_Send_Custom_When_Velocity_Exceeds_Amount::OnlyTriggerOnTest(float horizontalSpeed,float verticalSpeed)
{
	if (onlyTriggerOnMinHV.X >= 0.0 && horizontalSpeed < onlyTriggerOnMinHV.X)
		return false;
	if (onlyTriggerOnMinHV.Y >= 0.0 && verticalSpeed < onlyTriggerOnMinHV.Y)
		return false;
	if (onlyTriggerOnMaxHV.X >= 0.0 && horizontalSpeed > onlyTriggerOnMaxHV.X)
		return false;
	if (onlyTriggerOnMaxHV.Y >= 0.0 && verticalSpeed > onlyTriggerOnMaxHV.Y)
		return false;
	return true;
}
void JMG_Utility_Custom_Change_Character::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Change_Character::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (custom == message)
	{
		if (!sender->As_SoldierGameObj())
			return;
		const char *preset = Get_Parameter("Preset");
		if (!Get_Int_Parameter("AllowRepurchase") && !_stricmp(Commands->Get_Preset_Name(sender),preset))
		{
			Commands->Create_Sound(Get_Parameter("FailSound"),Commands->Get_Position(sender),sender);
			return;
		}
		float cost = Get_Float_Parameter("Cost");
		if (cost <= Commands->Get_Money(sender))
		{
			Commands->Give_Money(sender,-cost,0);
			Change_Character(sender,preset);
			Commands->Create_Sound(Get_Parameter("SuccessSound"),Commands->Get_Position(sender),sender);
		}
		else
			Commands->Create_Sound(Get_Parameter("FailSound"),Commands->Get_Position(sender),sender);
	}
}
void JMG_Utility_Apply_Damage_While_In_Zone::Created(GameObject *obj)
{
	int damagerId = Get_Int_Parameter("DamagerId");
	sprintf(params,"%.2f,%.2f,%s,%d",Get_Float_Parameter("Rate"),Get_Float_Parameter("DamageAmount"),Get_Parameter("Warhead"),damagerId == -2 ? Commands->Get_ID(obj) : damagerId);
}
void JMG_Utility_Apply_Damage_While_In_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	Attach_Script_Once(enterer,"JMG_Utility_Apply_Damage_While_In_Zone_Attached",params);
}
void JMG_Utility_Apply_Damage_While_In_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	Remove_Script(exiter,"JMG_Utility_Apply_Damage_While_In_Zone_Attached");
}
void JMG_Utility_Apply_Damage_On_Timer_Base::Created(GameObject *obj)
{
	rate = Get_Float_Parameter("Rate");
	damageAmount = Get_Float_Parameter("DamageAmount");
	sprintf(warhead,"%s",Get_Parameter("Warhead"));
	damagerId = Get_Int_Parameter("DamagerId");
	Commands->Start_Timer(obj,this,0.0f,1);
}
void JMG_Utility_Apply_Damage_On_Timer_Base::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Apply_Damage(obj,damageAmount,warhead,damagerId ? (damagerId == -1 ? obj : Commands->Find_Object(damagerId)) : 0);
		Commands->Start_Timer(obj,this,rate,1);
	}
}
void JMG_Utility_Zone_Send_Custom_Exit::Created(GameObject *obj)
{
	playerType = Get_Int_Parameter("PlayerType");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("Custom");
	param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	triggerOnce = Get_Int_Parameter("TriggerOnce") ? true : false;
}
void JMG_Utility_Zone_Send_Custom_Exit::Exited(GameObject *obj,GameObject *exiter)
{
	if (CheckPlayerType(exiter,playerType) || Commands->Get_Player_Type(exiter) == -4)
		return;
	GameObject *object = id ? (id == -1 ? exiter : Commands->Find_Object(id)) : obj;
	if (object)
		Commands->Send_Custom_Event(obj,object,custom,param,delay);
	if (triggerOnce)
		Destroy_Script();
}
void JMG_Utility_Custom_Enable_Spawners_In_Range_Modular::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		int mod = Get_Int_Parameter("Mod");
		if (mod == -1)
			mod = param;
		int end = Get_Int_Parameter("EndID");
		bool enable = Get_Int_Parameter("Enable") == -1 ? (param ? true : false) : (Get_Int_Parameter("Enable") ? true : false);
		for (int x = Get_Int_Parameter("StartID");x <= end;x++)
			if (!(x % mod))
				Commands->Enable_Spawner(x,enable);
	}
}
void JMG_Utility_Custom_Create_Explosion_At_Bone::Created(GameObject *obj)
{
	sprintf(explosion,"%s",Get_Parameter("Explosion"));
	sprintf(bone,"%s",Get_Parameter("Bone"));
	owner = Get_Int_Parameter("Owner");
	custom = Get_Int_Parameter("Custom");
}
void JMG_Utility_Custom_Create_Explosion_At_Bone::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom)
	{
		Commands->Create_Explosion_At_Bone(explosion,obj,bone,!owner ? obj : (owner == -1 ? sender : Commands->Find_Object(owner))); 
	}
}
void JMG_Utility_Custom_Send_Custom_Repeat_Ignore_Time::Created(GameObject *obj)
{
	custom = Get_Int_Parameter("Custom");
	sendCustom = Get_Int_Parameter("SendCustom");
	params = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	id = Get_Int_Parameter("ID");
	ignoreTime = (int)(Get_Float_Parameter("IgnoreTime")*1000.0f);
	lastTriggerTime = clock()-ignoreTime;
	enable = Get_Int_Parameter("StartsEnabled") ? true : false;
	enableCustom = Get_Int_Parameter("EnableCustom");
}
void JMG_Utility_Custom_Send_Custom_Repeat_Ignore_Time::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == custom && enable)
	{
		time_t currentTime = clock();
		if (difftime(currentTime,lastTriggerTime) < ignoreTime)
			return;
		lastTriggerTime = currentTime;
		GameObject *object = id ? (id == -1 ? sender : Commands->Find_Object(id)) : obj;
		if (object)
			Commands->Send_Custom_Event(obj,object,sendCustom,params,delay);
	}
	if (message == enableCustom)
	{
		enable = param ? true : false;
		if (enable)
			lastTriggerTime = clock()-ignoreTime;
	}
}
void JMG_Utility_Damage_Send_Custom::Created(GameObject *obj)
{
	minDamage = Get_Float_Parameter("MinDamage");
	custom = Get_Int_Parameter("Custom");
	params = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	id = Get_Int_Parameter("ID");
	senderId = Get_Int_Parameter("SenderID");
	enable = Get_Int_Parameter("StartsEnabled") ? true : false;
	enableCustom = Get_Int_Parameter("EnableCustom");
	repeat = Get_Int_Parameter("Repeat") ? true : false;
}
void JMG_Utility_Damage_Send_Custom::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= minDamage || !enable)
		return;
	GameObject *object = id ? (id == -1 ? damager : Commands->Find_Object(id)) : obj;
	GameObject *sender = senderId ? (senderId == -1 ? damager : Commands->Find_Object(senderId)) : obj;
	if (object)
		Commands->Send_Custom_Event(sender,object,custom,params,delay);
	if (!repeat)
		Destroy_Script();
}
void JMG_Utility_Damage_Send_Custom::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == enableCustom)
	{
		enable = param ? true : false;
	}
}
void JMG_Utility_Send_Custom_When_Player_Inside_Of_Range::Created(GameObject *obj)
{
	sleeping = 0;
	distance = Get_Float_Parameter("Distance");
	id = Get_Int_Parameter("ID");
	custom = Get_Int_Parameter("Custom");
	param = Get_Int_Parameter("Param");
	delay = Get_Float_Parameter("Delay");
	distance *= distance;
	triggerOnce = Get_Int_Parameter("TriggerOnce") ? true : false;
	sleepTime = Get_Int_Parameter("SleepTime");
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Utility_Send_Custom_When_Player_Inside_Of_Range::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1)
	{
		if (sleeping)
			sleeping--;
		else
			for (int x = 1;x < 128;x++)
			{
				GameObject *player = Get_GameObj(x);
				if (!player)
					continue;
				float playerDist = JmgUtility::SimpleDistance(Commands->Get_Position(player),Commands->Get_Position(obj));
				if (playerDist <= distance)
				{
					GameObject *object = id ? Commands->Find_Object(id) : obj;
					if (object)
						Commands->Send_Custom_Event(obj,object,custom,param,delay);
					if (triggerOnce)
					{
						this->Destroy_Script();
						return;
					}
				}
			}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Custom_Enable_Spawners_In_Range_ModPlayer::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("Custom"))
	{
		int mod = Get_Int_Parameter("Mod");
		if (mod == -1)
			mod = param;
		int playerMulti = Get_Int_Parameter("PlayerMulti");
		int newMod = JmgUtility::MathClampInt(mod-Get_Player_Count()*playerMulti,1,mod);
		bool invert = Get_Int_Parameter("Invert") ? true : false;
		int end = Get_Int_Parameter("EndID");
		bool enable = Get_Int_Parameter("Enable") == -1 ? (param ? true : false) : (Get_Int_Parameter("Enable") ? true : false);
		for (int x = Get_Int_Parameter("StartID");x <= end;x++)
			if (!(x % newMod))
				Commands->Enable_Spawner(x,enable);
			else if (invert)
				Commands->Enable_Spawner(x,!enable);
	}
}
bool JMG_Utility_Spawn_With_Last_Selected_Gun_Control::controllerPlaced = false;
char JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerWeapons[128][256];
char JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerNames[128][256];
void JMG_Utility_Spawn_With_Last_Selected_Gun_Control::Created(GameObject *obj)
{
	for (int x = 0;x < 128;x++)
	{
		sprintf(playerWeapons[x],"");
		sprintf(playerNames[x],"");
	}
	controllerPlaced = true;
}
void JMG_Utility_Spawn_With_Last_Selected_Gun_Control::Destroyed(GameObject *obj)
{
	controllerPlaced = false;
}
void JMG_Utility_Spawn_With_Last_Selected_Gun_Player::Created(GameObject *obj)
{
	if (!Commands->Is_A_Star(obj))
		Destroy_Script();
	if (!JMG_Utility_Spawn_With_Last_Selected_Gun_Control::controllerPlaced)
	{
		Commands->Start_Timer(obj,this,0.1f,2);
		return;
	}
	InitialSetup(obj);
}
void JMG_Utility_Spawn_With_Last_Selected_Gun_Player::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1)
	{
		GrantPlayersWeapon(obj);
	}
	if (number == 2)
	{
		if (!JMG_Utility_Spawn_With_Last_Selected_Gun_Control::controllerPlaced)
		{
			Console_Input("msg JMG_Utility_Spawn_With_Last_Selected_Gun_Player ERROR: JMG_Utility_Spawn_With_Last_Selected_Gun_Control must be placed on the map!");
			Destroy_Script();
			return;
		}
		InitialSetup(obj);
	}
}
void JMG_Utility_Spawn_With_Last_Selected_Gun_Player::Destroyed(GameObject *obj)
{
	const char *weapon = Get_Current_Weapon(obj);
	if (weapon)
		sprintf(JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerWeapons[playerId],"%s",weapon);
}
void JMG_Utility_Spawn_With_Last_Selected_Gun_Player::GrantPlayersWeapon(GameObject *obj)
{
	if (!_stricmp(JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerWeapons[playerId],""))
		return;
	if (!Has_Weapon(obj,JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerWeapons[playerId]) && Get_Int_Parameter("GrantWeapon"))
		Grant_Weapon(obj,JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerWeapons[playerId],true,99999,true);
	const char *weap = Get_Current_Weapon(obj);
	if ((!weap || _stricmp(weap,JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerWeapons[playerId])) && Has_Weapon(obj,JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerWeapons[playerId]))
		Commands->Select_Weapon(obj,JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerWeapons[playerId]);
}
void JMG_Utility_Spawn_With_Last_Selected_Gun_Player::InitialSetup(GameObject *obj)
{
	playerId = JmgUtility::JMG_Get_Player_ID(obj);
	const char *playerName = Get_Player_Name(obj);
	if (_stricmp(JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerNames[playerId],playerName))
	{
		sprintf(JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerNames[playerId],"%s",playerName);
		sprintf(JMG_Utility_Spawn_With_Last_Selected_Gun_Control::playerWeapons[playerId],"");
	}
	float delay = Get_Float_Parameter("GrantDelay");
	if (!delay)
		GrantPlayersWeapon(obj);
	else
		Commands->Start_Timer(obj,this,delay,1);
}
void JMG_Utility_Created_Give_Weapon::Created(GameObject *obj)
{
	const char *weaponName = Get_Parameter("WeaponName");
	if (weaponName && _stricmp(weaponName,""))
		Grant_Weapon(obj,Get_Parameter("WeaponName"),Get_Int_Parameter("GrantWeapon") ? true : false,Get_Int_Parameter("GrantRounds"),Get_Int_Parameter("GrantClips") ? true : false);
}
ScriptRegistrant<JMG_Utility_Check_If_Script_Is_In_Library> JMG_Utility_Check_If_Script_Is_In_Library_Registrant("JMG_Utility_Check_If_Script_Is_In_Library","ScriptName:string,CppName:string");
ScriptRegistrant<JMG_Send_Custom_When_Custom_Sequence_Matched> JMG_Send_Custom_When_Custom_Sequence_Matched_Registrant("JMG_Send_Custom_When_Custom_Sequence_Matched","Success_Custom=0:int,Correct_Step_Custom=0:int,Partial_Failure_Custom=0:int,Failure_Custom=0:int,Send_To_ID=0:int,Custom_0=0:int,Custom_1=0:int,Custom_2=0:int,Custom_3=0:int,Custom_4=0:int,Custom_5=0:int,Custom_6=0:int,Custom_7=0:int,Custom_8=0:int,Custom_9=0:int,Disable_On_Success=1:int,Disable_On_Failure=0:int,Starts_Enabled=1:int,Enable_Custom=0:int,Correct_Step_Saftey=0:int,Failure_Saftey=1:int,Max_Failures=1:int");
ScriptRegistrant<JMG_Utility_Change_Model_On_Timer> JMG_Utility_Change_Model_On_Timer_Registrant("JMG_Utility_Change_Model_On_Timer","Model=null:string,Time=0:float");
ScriptRegistrant<JMG_Utility_Emulate_DamageableStaticPhys> JMG_Utility_Emulate_DamageableStaticPhys_Registrant("JMG_Utility_Emulate_DamageableStaticPhys","Animation=null:string,Killed_Explosion=null:string,Live_Loop_Start=-1.0:float,Live_Loop_End=-1.0:float,Live_Twitch_Start=-1.0:float,Live_Twitch_End=-1.0:float,Death_Transition_Start=-1.0:float,Death_Transition_End=-1.0:float,Death_Loop_Start=-1.0:float,Death_Loop_End=-1.0:float,Death_Twitch_Start=-1.0:float,Death_Twitch_End=-1.0:float,Live_Transition_Start=-1.0:float,Live_Transition_End=-1.0:float,Repaired_Explosion=null:string,Play_Twitches_To_Completion:int,Send_To_ID:int,Death_Custom:int,Repaired_Custom:int,Damage_Points:float,Death_Points:float,Heal_Points:float,Repair_Points:float,Neutral_When_Dead:int,Disable_Collision_When_Dead:int");
ScriptRegistrant<JMG_Utility_Display_HUD_Info_Text_All_Players_Custom> JMG_Utility_Display_HUD_Info_Text_To_All_Players_On_Custom_Registrant("JMG_Utility_Display_HUD_Info_Text_All_Players_Custom","Custom:int,StringId:int,ColorRGB:Vector3,Repeatable=1:int");
ScriptRegistrant<JMG_Utility_Display_HUD_Info_Text_To_Sender_On_Custom> JMG_Utility_Display_HUD_Info_Text_To_Sender_On_Custom_Registrant("JMG_Utility_Display_HUD_Info_Text_To_Sender_On_Custom","Custom:int,StringId:int,ColorRGB:Vector3,Repeatable=1:int");
ScriptRegistrant<JMG_Utility_Soldier_Transition_On_Custom> JMG_Utility_Soldier_Transition_On_Custom_Registrant("JMG_Utility_Soldier_Transition_On_Custom","Custom:int");
ScriptRegistrant<JMG_Utility_Poke_Send_Self_Custom> JMG_Utility_Poke_Send_Self_Custom_Registrant("JMG_Utility_Poke_Send_Self_Custom","Custom:int,Param:int,Delay:float,LockoutTime=-1.0:float");
ScriptRegistrant<JMG_Turret_Spawn> JMG_Turret_Spawn_Registrant("JMG_Turret_Spawn","Turret_Preset:string,Bone_Name=Tur_Mount:string");
ScriptRegistrant<JMG_Utility_Sync_System_Object> JMG_Utility_Sync_System_Object_Registrant("JMG_Utility_Sync_System_Object","");
ScriptRegistrant<JMG_Utility_Sync_System_Controller> JMG_Utility_Sync_System_Controller_Registrant("JMG_Utility_Sync_System_Controller","Sync_Rate=1.0:float");
ScriptRegistrant<JMG_Utility_Sync_Object_Periodically> JMG_Utility_Sync_Object_Periodically_Registrant("JMG_Utility_Sync_Object_Periodically","Sync_Rate=1.0:float");
ScriptRegistrant<JMG_Utility_Basic_Spawner> JMG_Utility_Basic_Spawner_Registrant("JMG_Utility_Basic_Spawner","SpawnPreset:string,RespawnTime=0.0:float,RespawnTimeRandom=0:float,StartsEnabled=1:int,EnableOrDisableCustom=0:int,SpawnLimit=-1:int,DelayOnStartup=0:int");
ScriptRegistrant<JMG_Utility_Basic_Spawner_Spawned_Object> JMG_Utility_Basic_Spawner_Spawned_Object_Registrant("JMG_Utility_Basic_Spawner_Spawned_Object","ControllerId:int");
ScriptRegistrant<JMG_Utility_AI_Engineer> JMG_Utility_AI_Engineer_Registrant("JMG_Utility_AI_Engineer","RepaiarGun_Preset=Weapon_RepairGun_Player:string,Weapon_Preset=Weapon_Pistol_Player:string,PatrolRange=50.0:float,BaseCenterPoint=0.0 0.0 0.0:vector3,MinHP[Soldiers|Vehicles|Turrets]=0.5 0.75 0.9:vector3,MinHP[EngineerTarget|C4|Beacon]=0.25 0.0 0.0:vector3,Priority[RepairTargets|C4|Beacons]=1.0 1.0 1.0:vector3,WanderGroupID=-1:int");
ScriptRegistrant<JMG_Utility_Set_Object_Visibility_For_Player_On_Custom> JMG_Utility_Set_Object_Visibility_For_Player_On_Custom_Registrant("JMG_Utility_Set_Object_Visibility_For_Player_On_Custom","Custom:int,PlayerID:int,Visible:int");
ScriptRegistrant<JMG_Utility_AI_Vehicle> JMG_Utility_AI_Vehicle_Registrant("JMG_Utility_AI_Vehicle","MaxRange=25.0:float,MinAttackRange=0.0:float,FollowTarget=1:int,ReturnHome=1:int,ForceFire=0:int,VTOLHover=0.0:float,vsSoldier=1:int,vsVehicle=1:int,vsAircraft=1:int,WanderingAIGroupID=-1:int,MinRandWander=60.0:float,MaxRandWander=90.0:float,SquishInfantry=1:int,EnableTransitions=0:int,UsePathfind=1:int,DisableEngineOnArrival=1:int,WeaponError=-1.0:float,MaxHuntRange=0.0:float,NextWanderPointWhenLastReached=0.0:float");
ScriptRegistrant<JMG_Utility_Dynamic_Clock_Control> JMG_Utility_Dynamic_Clock_Control_Registrant("JMG_Utility_Dynamic_Clock_Control","");
ScriptRegistrant<JMG_Utility_Dynamic_Clock_Object> JMG_Utility_Dynamic_Clock_Object_Registrant("JMG_Utility_Dynamic_Clock_Object","");
ScriptRegistrant<JMG_Utility_Change_Screen_Color_While_In_Zone> JMG_Utility_Change_Screen_Color_While_In_Zone_Registrant("JMG_Utility_Change_Screen_Color_While_In_Zone","Color[Red|Green|Blue]:vector3,Opacity:float,EnterFadeInTime:float,ExitFadeOutTime:float");
ScriptRegistrant<JMG_Utility_Reset_Screen_Color_When_Destroyed> JMG_Utility_Reset_Screen_Color_When_Destroyed_Registrant("JMG_Utility_Reset_Screen_Color_When_Destroyed","");
ScriptRegistrant<JMG_Utility_Play_Music_On_Join_Controller> JMG_Utility_Play_Music_On_Join_Controller_Registrant("JMG_Utility_Play_Music_On_Join_Controller","Music=null:string");
ScriptRegistrant<JMG_Utility_Play_Music_On_Join_Change_Music> JMG_Utility_Play_Music_On_Join_Change_Music_Registrant("JMG_Utility_Play_Music_On_Join_Change_Music","Music=null:string,FadeOut:int,FadeIn:int");
ScriptRegistrant<JMG_Utility_Toggle_Door> JMG_Utility_Toggle_Door_Registrant("JMG_Utility_Toggle_Door","DoorID=0:int,Animation=null:string,SwitchAnimation=null:string,LastFrame=0.0:float,SwitchLastFrame=0.0:float,Key=0:int,PlayerType=2:int,DisableSwitchWhenNoShield=0:int,SwitchDisabledModel=null:string,DoorSound=null:string,SwitchSound=null:string,SwitchBlockedSound=null:string,SwitchDamagedPokeSound=null:string,WrongKeySound=null:string,WrongPlayerTypeSound=null:string,TextColor[Red|Green|Blue]=0.0 0.0 0.0:vector3,GotoLastFrameMessage=null:string,GotoFirstFrameMessage=null:string,ActionBlockedMessage=null:string,NoDoorMessage=null:string,SwitchDamagedPokeMessage=null:string,WrongKeyMessage=null:string,WrongPlayerTypeMessage=null:string,DoorControlCustom=0:int,SendCustomId=0:int,SendCustom=0:int");
ScriptRegistrant<JMG_Utility_Set_Animation_Frame_On_Creation> JMG_Utility_Set_Animation_Frame_On_Creation_Registrant("JMG_Utility_Set_Animation_Frame_On_Creation","Animation:string,Frame:float");
ScriptRegistrant<JMG_Utility_Set_Model_On_Damage_Percent_3_States> JMG_Utility_Set_Model_On_Damage_Percent_3_States_Registrant("JMG_Utility_Set_Model_On_Damage_Percent_3_States","FullShieldModel:string,HalfShieldModel:string,NoShieldModel:string");
ScriptRegistrant<JMG_Utility_Set_Model_On_Damage_Percent_4_States> JMG_Utility_Set_Model_On_Damage_Percent_4_States_Registrant("JMG_Utility_Set_Model_On_Damage_Percent_4_States","FullShieldModel:string,TwoThirdsShieldModel:string,ThirdShieldModel:string,NoShieldModel:string");
ScriptRegistrant<JMG_Utility_PCT> JMG_Utility_PCT_Registrant("JMG_Utility_PCT","");
ScriptRegistrant<JMG_Utility_PCT_Inaccessible_Zone> JMG_Utility_PCT_Inaccessible_Zone_Registrant("JMG_Utility_PCT_Inaccessible_Zone","");
ScriptRegistrant<JMG_Utility_PCT_Inaccessible_Zone_Attach> JMG_Utility_PCT_Inaccessible_Zone_Attach_Registrant("JMG_Utility_PCT_Inaccessible_Zone_Attach","");
ScriptRegistrant<JMG_Utility_Display_HUD_Info_Text_All_Players_Custom_Replace_String> JMG_Utility_Display_HUD_Info_Text_All_Players_Custom_2_Registrant("JMG_Utility_Display_HUD_Info_Text_All_Players_Custom_Replace_String","Custom:int,StringId=7233:int,ReplaceString:string,ColorRGB:Vector3,Repeatable=1:int");
ScriptRegistrant<JMG_Utility_Display_Text_Message_To_All_Players_On_Custom> JMG_Utility_Display_Text_Message_To_All_Players_On_Custom_Registrant("JMG_Utility_Display_Text_Message_To_All_Players_On_Custom","Custom:int,Message:string,ColorRGB:Vector3,Repeatable=1:int");
ScriptRegistrant<JMG_Utility_Set_Team_On_Create> JMG_Utility_Set_Team_On_Create_Registrant("JMG_Utility_Set_Team_On_Create","PlayerType:int,Delay=0.0:float");
ScriptRegistrant<JMG_Utility_AI_Aggressive_Melee> JMG_Utility_AI_Aggressive_Melee_Registrant("JMG_Utility_AI_Aggressive_Melee","ReturnHomeSpeed=1.0:float,ReturnHomeArriveDistance=1.0:float,ChaseSpeed=1.0:float,AttackArriveDistance=0.0:float,StartAttackDistance=3.0:float,MaxHuntDistance=0.0:float,MinLooseVisibilityTime=3:int,MaxLooseVisibilityTime=6:int,NoPathfindDistance=5.0:float");
ScriptRegistrant<JMG_Utility_Infantry_Placed_Buildable_Object> JMG_Utility_Infantry_Placed_Buildable_Object_Registrant("JMG_Utility_Infantry_Placed_Buildable_Object","WeaponPreset=null:string,PreviewPreset=Daves Arrow:string,DeployPreset=null:string,RepairedPreset=null:string,PlaceablePreview=null:string,UnPlaceablePreview=null:string,EnableCustom:int,DisableCustom:int,DeployCustom:int,RegainTimer:int,Cost:float,MessageColor[R|G|B]=255.0 255.0 255.0:vector3,DeploymentBlockedMessage=Cannot deploy when near buildings.:string,PlacementMessage=You can now place another :string,GrantWeapon=1:int,RegenStartsAfterRepair=1:int,MatchTeam[Preview|Deployed|Repaired]=0.0 0.0 1.0:vector3,DestroyedRegenTime=1:int");
ScriptRegistrant<JMG_Utility_Infantry_Placed_Buildable_Object_Attached> JMG_Utility_Infantry_Placed_Buildable_Object_Attached_Registrant("JMG_Utility_Infantry_Placed_Buildable_Object_Attached","ScriptId:int,PlacerId:int,RepairedPreset:string,MatchTeam:int,Team:int");
ScriptRegistrant<JMG_Utility_Lock_Weapon_Selection_While_Script_Attached> JMG_Utility_Lock_Weapon_Selection_While_Script_Attached_Registrant("JMG_Utility_Lock_Weapon_Selection_While_Script_Attached","WeaponPreset:string");
ScriptRegistrant<JMG_Utility_Swimming_zDefault_Map_Fog_Values> JMG_Utility_Swimming_zDefault_Map_Fog_Values_Registrant("JMG_Utility_Swimming_zDefault_Map_Fog_Values","");
ScriptRegistrant<JMG_Utility_Swimming_Zone> JMG_Utility_Swim_While_In_Zone_Registrant("JMG_Utility_Swimming_Zone","WaterColor[R|G|B]=0.28 0.43 0.55:vector3,WaterOpacity=0.5:float,WaterMinViewDistance=5.0:float,WaterMaxViewDistance=15.0:float,SwimmingSpeedMultiplier=0.75:float");
ScriptRegistrant<JMG_Utility_Swimming_Infantry> JMG_Utility_Swimming_Infantry_Registrant("JMG_Utility_Swimming_Infantry","WeaponPreset=Weapon_Swimming_Animations:string,DrownTime=10.0:float,StarDrownSequence=3.0:float,GaspForBreath=SFX.SwimmingGaspForBreath:string,PantingSoundEmitterModel=s_panting:string,HeartBeatSoundEmitterModel=s_heartBeat:string,DrownDamageRate=2.5:float,CatchBreathRate=0.33:float,WaterDamageAmount=0.0:float,WaterDamageWarhead=None:string,EnterWaterMessageStringId=0:int,WaterEnterMessageColor[R|G|B]=0.25 0.25 1.0:vector3");
ScriptRegistrant<JMG_Utility_Zone_Enable_Spawners_In_Range> JMG_Utility_Zone_Enable_Spawners_In_Range_Registrant("JMG_Utility_Zone_Enable_Spawners_In_Range","StartID:int,EndID:int,PlayerType=2:int,Enable=1:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Display_Message_On_Vehicle_Enter> JMG_Utility_Display_Message_On_Vehicle_Enter_Registrant("JMG_Utility_Display_Message_On_Vehicle_Enter","StringId:int,MessageOverride=null:string,Color[R|G|B]=0.0 1.0 0.0:vector3,DriverOnly=1:int,ShowOnce=1:int,PlayerType=2:int");
ScriptRegistrant<JMG_Utility_Zone_Apply_Damage_On_Enter> JMG_Utility_Zone_Apply_Damage_On_Enter_Registrant("JMG_Utility_Zone_Apply_Damage_On_Enter","ID:int,DamageAmount:float,Warhead=None:string,DamageOccupants=1:int,PlayerType=2:int,OnlyOnce=0:int");
ScriptRegistrant<JMG_Utility_AI_Guardian_Aircraft> JMG_Utility_AI_Guardian_Aircraft_Registrant("JMG_Utility_AI_Guardian_Aircraft","WanderingAIGroupID:int,FlightHeight=25.0:float,FireRange=100.0:float,FaceTarget=1:int");
ScriptRegistrant<JMG_Utility_Switch_Weapon_While_Primary_Empty> JMG_Utility_Switch_Weapon_While_Primary_Empty_Registrant("JMG_Utility_Switch_Weapon_While_Primary_Empty","PrimaryWeapon=null:string,SecondaryWeapon=null:string,ReloadTime=0.0:float,PrimaryToSecondaryTime=-1.0:float,SecondaryToPrimaryTime=-1.0:float,IdlePrimaryAnim=null:string,IdleSecondaryAnim=null:string,PrimaryToSecondaryAnim=null:string,SecondaryToPrimaryAnim=null:string,PrimaryToSecondarySound=null:string,SecondaryToPrimarySound=null:string");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Near_Building> JMG_Utility_Send_Custom_When_Near_Building_Registrant("JMG_Utility_Send_Custom_When_Near_Building","SendCustomObjectID=0:int,NearToBuildingCustom:int,FarFromBuildingCustom:int,CloseToBuildingDistance=1.0:float,BuildingPlayerType=2:int,CheckDeadBuildings=1:int,CheckRate=0.25:float");
ScriptRegistrant<JMG_Utility_AI_Engineer_Repair_Target> JMG_Utility_AI_Engineer_Repair_Target_Registrant("JMG_Utility_AI_Engineer_Repair_Target","");
ScriptRegistrant<JMG_Utility_Reset_Screen_Fade_And_Fog_On_Destroy> JMG_Utility_Reset_Screen_Fade_And_Fog_On_Destroy_Registrant("JMG_Utility_Reset_Screen_Fade_And_Fog_On_Destroy","");
ScriptRegistrant<JMG_Utility_AI_Goto_Player> JMG_Utility_AI_Goto_Player_Registrant("JMG_Utility_AI_Goto_Player","HuntSearchDistance=-1.0:float,HuntSpeed=1.0:float,HuntArriveDistance=1.0:float,RandomHuntArriveDistance=0.0:float,HuntStealth=0:int,AttackSpeed=1.0:float,AttackDistance=-1.0:float,RandomAttackDistance=0.0:float,ReturnHome=1:int,ReturnHomeSpeed=1.0:float,WanderingAIGroupID=-1:int,WanderSpeed=1.0:float,CanSeeStealth=1:int,ShutdownEngineOnArrival=0:int,AttackCheckBlocked=1:int");
ScriptRegistrant<JMG_Utility_AI_Aggressive_Attack_Spot_Control> JMG_Utility_AI_Aggressive_Attack_Spot_Control_Registrant("JMG_Utility_AI_Aggressive_Attack_Spot_Control","");
ScriptRegistrant<JMG_Utility_AI_Aggressive_Attack_Spot_Point> JMG_Utility_AI_Aggressive_Attack_Spot_Point_Registrant("JMG_Utility_AI_Aggressive_Attack_Spot_Point","GroupId=0:int,AttackOffset=0.0 0.0 0.0:Vector3");
ScriptRegistrant<JMG_Utility_AI_Aggressive_Attack_Spot_Point2> JMG_Utility_AI_Aggressive_Attack_Spot_Point2_Registrant("JMG_Utility_AI_Aggressive_Attack_Spot_Point2","GroupId=0:int,AbsolutePosition=0.0 0.0 0.0:Vector3,ObjectID=0:int");
ScriptRegistrant<JMG_Utility_AI_Aggressive_Attack_Spot> JMG_Utility_AI_Aggressive_Attack_Spot_Registrant("JMG_Utility_AI_Aggressive_Attack_Spot","GroupId=-1:int,SecondaryGroupId=-1:int,AttackPointSpeed=1.0:float,AttackPointDistance=-1.0:float,RandomAttackPointDistance=0.0:float,AttackSpeed=1.0:float,AttackDistance=-1.0:float,RandomAttackDistance=0.0:float,ReturnHome=0:int,ReturnHomeSpeed=1.0:float,ReactToAttackChance=0.5:float,ReactToAttackRate=10.0:float,ShutdownEngineOnArrival=0:int,EnableDebug=0:int,UseSecondaryFire=0:int");
ScriptRegistrant<JMG_Utility_Destroy_Objects_In_ID_Range_On_Death> JMG_Utility_Destroy_Objects_In_ID_Range_On_Death_Registrant("JMG_Utility_Destroy_Objects_In_ID_Range_On_Death","StartID:int,EndID:int");
ScriptRegistrant<JMG_Utility_Custom_Enable_Spawners_In_Range> JMG_Utility_Custom_Enable_Spawners_In_Range_Registrant("JMG_Utility_Custom_Enable_Spawners_In_Range","StartID:int,EndID:int,Custom:int,Enable=1:int");
ScriptRegistrant<JMG_Utility_Send_Custom_On_Player_Count> JMG_Utility_Send_Custom_On_Player_Count_Registrant("JMG_Utility_Send_Custom_On_Player_Count","PlayerCount=0:int,ID=0:int,Custom:int,Param:int,Delay:float,TriggerOn[eq|<|>]=1.0 0.0 0.0:vector3,SupressMatchSpam=1:int,DestroyAfterTrigger=0:int");
ScriptRegistrant<JMG_Utility_Send_Custom_To_Preset_On_Message> JMG_Utility_Send_Custom_To_Preset_On_Message_Registrant("JMG_Utility_Send_Custom_To_Preset_On_Message","TriggerCustom:int,Preset:string,Custom:int,Param:int,Delay=0.0:float");
ScriptRegistrant<JMG_Utility_Basic_Spawner_Attach_Script> JMG_Utility_Basic_Spawner_Attach_Script_Registrant("JMG_Utility_Basic_Spawner_Attach_Script","Script:string,Params:string,Delim=@:string");
ScriptRegistrant<JMG_Utility_Send_Custom_On_Preset_Enter> JMG_Utility_Send_Custom_On_Preset_Enter_Registrant("JMG_Utility_Send_Custom_On_Preset_Enter","PresetName:string,ID:int,Custom:int,Param:int,Delay=0.0:float");
ScriptRegistrant<JMG_Utility_Basic_Spawner_Set_Prespawn_Location> JMG_Utility_Basic_Spawner_Set_Prespawn_Location_Registrant("JMG_Utility_Basic_Spawner_Set_Prespawn_Location","OverrideLocation:Vector3");
ScriptRegistrant<JMG_Utility_Teleport_On_Pickup> JMG_Utility_Teleport_On_Pickup_Registrant("JMG_Utility_Teleport_On_Pickup","ObjectID:int,Position=0.0 0.0 0.0:Vector3,MaxDistance=1.5:float,Facing=-999.0:float");
ScriptRegistrant<JMG_Utility_Zone_Set_Animation> JMG_Utility_Zone_Set_Animation_Registrant("JMG_Utility_Zone_Set_Animation","ObjectID:int,Animation:string,Looping=0:int,StartFrame=0.0:float,EndFrame=-1.0:float,Blended=1:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Scale_Infantry> JMG_Utility_Scale_Infantry_Registrant("JMG_Utility_Scale_Infantry","Scale=1.0:float");
ScriptRegistrant<JMG_Utility_Set_Innate_On_Create> JMG_Utility_Set_Innate_On_Create_Registrant("JMG_Utility_Set_Innate_On_Create","Enable=1:int");
ScriptRegistrant<JMG_Utility_Set_Soldier_Damage_Animations> JMG_Utility_Set_Soldier_Damage_Animations_Registrant("JMG_Utility_Set_Soldier_Damage_Animations","Enable=1:int");
ScriptRegistrant<JMG_Utility_Play_Music_On_Join_Enter_Change_Music> JMG_Utility_Play_Music_On_Join_Enter_Change_Music_Registrant("JMG_Utility_Play_Music_On_Join_Enter_Change_Music","Music=null:string,FadeOut:int,FadeIn:int,PlayerType=2:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Play_Music_On_Join_Killed_Change_Music> JMG_Utility_Play_Music_On_Join_Killed_Change_Music_Registrant("JMG_Utility_Play_Music_On_Join_Killed_Change_Music","Music=null:string,FadeOut:int,FadeIn:int");
ScriptRegistrant<JMG_Utility_Objective_System_Controller> JMG_Utility_Objective_System_Controller_Registrant("JMG_Utility_Objective_System_Controller","ShowMarkersOnRadar=1:int,PrimaryObjectiveModel=null:string,SecondaryObjectiveModel=null:string,TertiaryObjectiveModel=null:string,Team=2:int");
ScriptRegistrant<JMG_Utility_Objective_System_Objective_Update_Enter> JMG_Utility_Objective_System_Objective_Update_Enter_Registrant("JMG_Utility_Objective_System_Objective_Update_Enter","NewObjectiveID:int,NewObjectiveStringID:int,ObjectiveMarkerObjectID:int,CompleteObjectiveID:int,Delay:float,NewObjectivePriority=1:int");
ScriptRegistrant<JMG_Utility_Objective_System_Objective_Update_Pickup> JMG_Utility_Objective_System_Objective_Update_Pickup_Registrant("JMG_Utility_Objective_System_Objective_Update_Pickup","NewObjectiveID:int,NewObjectiveStringID:int,ObjectiveMarkerObjectID:int,CompleteObjectiveID:int,Delay:float,NewObjectivePriority=1:int");
ScriptRegistrant<JMG_Utility_Objective_System_Objective_Update_Killed> JMG_Utility_Objective_System_Objective_Update_Killed_Registrant("JMG_Utility_Objective_System_Objective_Update_Killed","NewObjectiveID:int,NewObjectiveStringID:int,ObjectiveMarkerObjectID:int,CompleteObjectiveID:int,Delay:float,NewObjectivePriority=1:int");
ScriptRegistrant<JMG_Utility_Objective_System_Objective_Update_Custom> JMG_Utility_Objective_System_Objective_Update_Custom_Registrant("JMG_Utility_Objective_System_Objective_Update_Custom","Custom:int,NewObjectiveID:int,NewObjectiveStringID:int,ObjectiveMarkerObjectID:int,CompleteObjectiveID:int,Delay:float,NewObjectivePriority=1:int");
ScriptRegistrant<JMG_Utility_Destroyed_Apply_Damage> JMG_Utility_Destroyed_Apply_Damage_Registrant("JMG_Utility_Destroyed_Apply_Damage","ID:int,Damage:float,Warhead:string");
ScriptRegistrant<JMG_Utility_Scale_Damage_By_Player_Count> JMG_Utility_Scale_Damage_By_Player_Count_Registrant("JMG_Utility_Scale_Damage_By_Player_Count","MaxPlayersToScaleFor=50:int,MaxScaleFactor=0.95:float");
ScriptRegistrant<JMG_Utility_Scale_Damage_Square_By_Player_Count> JMG_Utility_Scale_Damage_Square_By_Player_Count_Registrant("JMG_Utility_Scale_Damage_Square_By_Player_Count","MaxPlayersToScaleFor=50:int,MaxScaleFactor=0.95:float");
ScriptRegistrant<JMG_Utility_Regen_HitPoints> JMG_Utility_Regen_HitPoints_Registrant("JMG_Utility_Regen_HitPoints","RegenHealth=1:int,HealthAmount=1.0:float,HealthPerPlayer=0.0:float,RegenArmor=1:int,ArmorAmount=1.0:float,ArmorPerPlayer=0.0:float,Rate=1.0:float,DamageDelay=0.0:float");
ScriptRegistrant<JMG_Utility_Toggle_Flight_On_Delay> JMG_Utility_Toggle_Flight_On_Delay_Registrant("JMG_Utility_Toggle_Flight_On_Delay","Delay:float");
ScriptRegistrant<JMG_Utility_Fainting_Soldier> JMG_Utility_Fainting_Soldier_Registrant("JMG_Utility_Fainting_Soldier","FaintAnimation:string,LayAnimation:string,StandAnimation:string,FaintSound=null:string,StandSound=null:string,ChangeArmorTypeWhenKnockedOut=0:int,ArmorTypeWhileKnockedOut=Blamo:string,TeamWhileKnockedOut=-2:int");
ScriptRegistrant<JMG_Utility_AI_Guardian_Infantry> JMG_Utility_AI_Guardian_Infantry_Registrant("JMG_Utility_AI_Guardian_Infantry","WanderingAIGroupID:int,WanderSpeed=1.0:float,FireRange=100.0:float,FaceTarget=1:int,CheckBlocked=1:int");
ScriptRegistrant<JMG_Utility_Set_Innate_Max_Wander_Distance> JMG_Utility_Set_Innate_Max_Wander_Distance_Registrant("JMG_Utility_Set_Innate_Max_Wander_Distance","Distance:float");
ScriptRegistrant<JMG_Utility_Switch_Weapon_To_Empty_Hands_Until_Custom> JMG_Utility_Switch_Weapon_To_Empty_Hands_Until_Custom_Registrant("JMG_Utility_Switch_Weapon_To_Empty_Hands_Until_Custom","Custom:int");
ScriptRegistrant<JMG_Utility_Set_Skin_And_Shield_Type_On_Custom> JMG_Utility_Set_Skin_And_Shield_Type_On_Custom_Registrant("JMG_Utility_Set_Skin_And_Shield_Type_On_Custom","Custom:int,SkinType=None:string,ShieldType=None:string");
ScriptRegistrant<JMG_Utility_Zone_Damage_All_In_ID_Range_On_Enter>JMG_Utility_Zone_Damage_All_In_ID_Range_On_Enter_Registrant("JMG_Utility_Zone_Damage_All_In_ID_Range_On_Enter","StartID:int,EndID:int,PlayerType=2:int,Damage=0.0:float,Warhead=None:string,Damager=0:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Zone_Send_Custom_To_All_With_Script>JMG_Utility_Zone_Send_Custom_To_All_With_Script_Registrant("JMG_Utility_Zone_Send_Custom_To_All_With_Script","PlayerType=2:int,AttachedScript=ScriptName:string,Custom=0:int,Param=0:int,Delay=0.0:float,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_HitPoints_In_Range_Change_Model> JMG_Utility_HitPoints_In_Range_Change_Model_Registrant("JMG_Utility_HitPoints_In_Range_Change_Model","LowerHitPointsPercent=0.0:float,UpperHitPointsPercent=100.0:float,Model=null:string");
ScriptRegistrant<JMG_Utility_HitPoints_In_Range_Attach_Preset> JMG_Utility_HitPoints_In_Range_Attach_Preset_Registrant("JMG_Utility_HitPoints_In_Range_Attach_Preset","LowerHitPointsPercent=0.0:float,UpperHitPointsPercent=100.0:float,Preset=null:string,BoneName=null:string");
ScriptRegistrant<JMG_Utility_HitPoints_In_Range_Enable_Spawner> JMG_Utility_HitPoints_In_Range_Enable_Spawner_Registrant("JMG_Utility_HitPoints_In_Range_Enable_Spawner","LowerHitPointsPercent=0.0:float,UpperHitPointsPercent=100.0:float,SpawnerID=0:int");
ScriptRegistrant<JMG_Utility_HitPoints_In_Range_Create_Preset> JMG_Utility_HitPoints_In_Range_Create_Preset_Registrant("JMG_Utility_HitPoints_In_Range_Create_Preset","LowerHitPointsPercent=0.0:float,UpperHitPointsPercent=100.0:float,Preset=null:string,Location=0.0 0.0 0.0:Vector3,Facing=0.0:float");
ScriptRegistrant<JMG_Utility_HitPoints_In_Range_Send_Custom> JMG_Utility_HitPoints_In_Range_Send_Custom_Registrant("JMG_Utility_HitPoints_In_Range_Send_Custom","LowerHitPointsPercent=0.0:float,UpperHitPointsPercent=100.0:float,ID=0:int,Custom=0:int,Param=0:int,Delay=0.0:float");
ScriptRegistrant<JMG_Utility_Custom_Set_Weather_Precipitation> JMG_Utility_Custom_Set_Weather_Precipitation_Registrant("JMG_Utility_Custom_Set_Weather_Precipitation","Custom:int,Set[Rain|Snow|Ash|]=0.0 0.0 0.0:Vector3,Change[Rain|Snow|Ash|]=1.0 1.0 1.0:Vector3,TransitionTime=0.0:float");
ScriptRegistrant<JMG_Utility_Custom_Set_Weather_Fog> JMG_Utility_Custom_Set_Weather_Fog_Registrant("JMG_Utility_Custom_Set_Weather_Fog","Custom:int,StartDistance=100.0:float,EndDistance=300.0:float,TransitionTime=0.0:float");
ScriptRegistrant<JMG_Utility_Custom_Set_Weather_Wind> JMG_Utility_Custom_Set_Weather_Wind_Registrant("JMG_Utility_Custom_Set_Weather_Wind","Custom:int,Heading=0.0:float,Speed=0.0:float,Variability=0.0:float,TransitionTime=0.0:float");
ScriptRegistrant<JMG_Utility_Custom_Set_Weather_Lightning> JMG_Utility_Custom_Set_Weather_Lightning_Registrant("JMG_Utility_Custom_Set_Weather_Lightning","Custom:int,Intensity=0.0:float,StartDistance=0.0:float,EndDistance=0.0:float,Heading=0.0:float,Distribution=0.0:float,TransitionTime=0.0:float");
ScriptRegistrant<JMG_Utility_Custom_Set_Weather_War_Blitz> JMG_Utility_Custom_Set_Weather_War_Blitz_Registrant("JMG_Utility_Custom_Set_Weather_War_Blitz","Custom:int,Intensity=0.0:float,StartDistance=0.0:float,EndDistance=0.0:float,Heading=0.0:float,Distribution=0.0:float,TransitionTime=0.0:float");
ScriptRegistrant<JMG_Utility_Custom_Set_Weather_Clouds> JMG_Utility_Custom_Set_Weather_Clouds_Registrant("JMG_Utility_Custom_Set_Weather_Clouds","Custom:int,Cover=0.0:float,Gloominess=0.0:float,TransitionTime=0.0:float");
ScriptRegistrant<JMG_Utility_Play_Music_On_Join_Custom_Change_Music> JMG_Utility_Play_Music_On_Join_Custom_Change_Music_Registrant("JMG_Utility_Play_Music_On_Join_Custom_Change_Music","Custom:int,Music=null:string,FadeOut:int,FadeIn:int,Repeat=1:int");
ScriptRegistrant<JMG_Utility_Zone_Send_Custom_If_Has_Weapon> JMG_Utility_Zone_Send_Custom_If_Has_Weapon_Registrant("JMG_Utility_Zone_Send_Custom_If_Has_Weapon","WeaponName:string,ID=0:int,Custom=0:int,Param=0:int,Delay=0.0:float,PlayerType=2:int,TriggerOnce=1:int,RemoveWeapon=0:int");
ScriptRegistrant<JMG_Utility_Receive_All_Customs_To_Send_Custom> JMG_Utility_Receive_All_Customs_To_Send_Custom_Registrant("JMG_Utility_Receive_All_Customs_To_Send_Custom","Custom0=0:int,Custom1=0:int,Custom2=0:int,Custom3=0:int,Custom4=0:int,Custom5=0:int,Custom6=0:int,Custom7=0:int,Custom8=0:int,Custom9=0:int,ID=0:int,Custom=0:int,Param=0:int,Delay=0.0:float,TriggerOnce=1:int,ResetCustom=0:int");
ScriptRegistrant<JMG_Utility_Zone_Teleport_To_Random_Wander_Point> JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Registrant("JMG_Utility_Zone_Teleport_To_Random_Wander_Point","WanderingAIGroupID=-1:int,SafeTeleportDistance=1.5:float,ChangeGroupIDCustom=0:int,PlayerType=2:int,RetryOnFailure=0:int");
ScriptRegistrant<JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Attach> JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Attach_Registrant("JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Attach","WanderingAIGroupID=-1:int,SafeTeleportDistance=1.5:float");
ScriptRegistrant<JMG_Utility_Zone_Set_Player_Type> JMG_Utility_Zone_Set_Player_Type_Registrant("JMG_Utility_Zone_Set_Player_Type","SetPlayerType=0:int,RequiredPlayerType=2:int");
ScriptRegistrant<JMG_Utility_Zone_Send_Custom_Enter> JMG_Utility_Zone_Send_Custom_Enter_Registrant("JMG_Utility_Zone_Send_Custom_Enter","ID=0:int,Custom=0:int,Param=0:int,Delay=0.0:float,PlayerType=2:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Custom_Destroy_Closest_Object_To_Position> JMG_Utility_Custom_Destroy_Closest_Object_To_Position_Registrant("JMG_Utility_Custom_Destroy_Closest_Object_To_Position","Custom:int,PresetName:string,Position:Vector3,MaxDistance=0.0:float");
ScriptRegistrant<JMG_Utility_Zone_Set_Spawner> JMG_Utility_Zone_Set_Spawner_Registrant("JMG_Utility_Zone_Set_Spawner","SpawnerID:int,PlayerType=2:int,Enable=1:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Persistant_Weapon_Powerup> JMG_Utility_Persistant_Weapon_Powerup_Registrant("JMG_Utility_Persistant_Weapon_Powerup","WeaponName:string,PowerupNameOverride=null:string,DisarmCustom=0:int");
ScriptRegistrant<JMG_Utility_Persistant_Weapon_zStandin> JMG_Utility_Persistant_Weapon_zStandin_Registrant("JMG_Utility_Persistant_Weapon_zStandin","PowerupName:string");
ScriptRegistrant<JMG_Utility_Persistant_Weapon_zAttached> JMG_Utility_Persistant_Weapon_zAttached_Registrant("JMG_Utility_Persistant_Weapon_zAttached","PowerupName:string,WeaponName:string,DisarmCustom:int");
ScriptRegistrant<JMG_Utility_Custom_Teleport_Players_Outside_Range_To_Wanderpoints> JMG_Utility_Custom_Teleport_Players_Outside_Range_To_Wanderpoints_Registrant("JMG_Utility_Custom_Teleport_Players_Outside_Range_To_Wanderpoints","Custom:int,ScanTeleportSpot:vector3,TeleportIgnoreDistance:float,WanderingAIGroupID:int,SafeTeleportDistance=1.5:float");
ScriptRegistrant<JMG_Utility_Custom_Set_Tile_Frame> JMG_Utility_Custom_Set_Tile_Frame_Registrant("JMG_Utility_Custom_Set_Tile_Frame","Custom:int,ID:int,Animation:string,Frame:Float");
ScriptRegistrant<JMG_Utility_Zone_Send_Custom_No_Weapon> JMG_Utility_Zone_Send_Custom_No_Weapon_Registrant("JMG_Utility_Zone_Send_Custom_No_Weapon","WeaponName:string,ID=0:int,Custom=0:int,Param=0:int,Delay=0.0:float,PlayerType=2:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Custom_Display_Briefing_Message> JMG_Utility_Custom_Display_Briefing_Message_Registrant("JMG_Utility_Custom_Display_Briefing_Message","Custom=0:int,FileName=null:string,TextTag=[]:string,MessageColor=239.0 228.0 176.0:Vector3,LineDelayTime=10.0:float,UISound=null:string");
ScriptRegistrant<JMG_Utility_Zone_Set_Player_Team> JMG_Utility_Zone_Set_Player_Team_Registrant("JMG_Utility_Zone_Set_Player_Team","SetPlayerTeam=0:int,RequiredPlayerTeam=2:int");
ScriptRegistrant<JMG_Utility_Objective_System_Objective_Update_Create> JMG_Utility_Objective_System_Objective_Update_Create_Registrant("JMG_Utility_Objective_System_Objective_Update_Create","NewObjectiveID:int,NewObjectiveStringID:int,ObjectiveMarkerObjectID:int,CompleteObjectiveID:int,Delay:float,NewObjectivePriority=1:int");
ScriptRegistrant<JMG_Utility_Objective_System_Objective_Remove_Custom> JMG_Utility_Objective_System_Objective_Remove_Custom_Registrant("JMG_Utility_Objective_System_Objective_Remove_Custom","Custom:int,ObjectiveID:int");
ScriptRegistrant<JMG_Utility_Delay_Then_Rotate_Camera> JMG_Utility_Delay_Then_Rotate_Camera_Registrant("JMG_Utility_Delay_Then_Rotate_Camera","Delay:float,Facing:float");
ScriptRegistrant<JMG_Utility_Damage_SmartGameObjects_In_Range> JMG_Utility_Damage_SmartGameObjects_In_Range_Registrant("JMG_Utility_Damage_SmartGameObjects_In_Range","Range=1.0:float,Damage=1.0:float,Warhead=None:string,Rate=0.1:float");
ScriptRegistrant<JMG_Utility_Unstick_Infantry_If_Stuck> JMG_Utility_Unstick_Infantry_If_Stuck_Registrant("JMG_Utility_Unstick_Infantry_If_Stuck","Rate=0.25:float,Distance=1.25:float");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_On_Count> JMG_Utility_Custom_Send_Custom_On_Count_Registrant("JMG_Utility_Custom_Send_Custom_On_Count","Custom:int,Count:int,ID=0:int,SendCustom:int,Param:int,Delay:float,ResetCustom:int");
ScriptRegistrant<JMG_Utility_Custom_Destroy_Self> JMG_Utility_Custom_Destroy_Self_Registrant("JMG_Utility_Custom_Destroy_Self","Custom:int");
ScriptRegistrant<JMG_Utility_Zone_Set_Player_Team2> JMG_Utility_Zone_Set_Player_Team2_Registrant("JMG_Utility_Zone_Set_Player_Team2","SetPlayerTeam=0:int,RequiredPlayerTeam=2:int");
ScriptRegistrant<JMG_Utility_Poke_Send_Custom> JMG_Utility_Poke_Send_Custom_Registrant("JMG_Utility_Poke_Send_Custom","ID=0:int,Custom=0:int,Param=0:int,Delay=0.0:float,TriggerOnce=1:int,PlayerType=2:int");
ScriptRegistrant<JMG_Utility_Set_Collision_Group> JMG_Utility_Set_Collision_Group_Registrant("JMG_Utility_Set_Collision_Group","CollisionGroupID:int");
ScriptRegistrant<JMG_Utility_Cap_Credits> JMG_Utility_Cap_Credits_Registrant("JMG_Utility_Cap_Credits","Credits:float,Team=2:int,Custom=0:int");
ScriptRegistrant<JMG_Utility_Custom_Apply_Damage> JMG_Utility_Custom_Apply_Damage_Registrant("JMG_Utility_Custom_Apply_Damage","Custom:int,ID=0:int,Damage:float,Warhead=None:string,DamagerID=0:int");
ScriptRegistrant<JMG_Utility_AI_Goto_Enemy> JMG_Utility_AI_Goto_Enemy_Registrant("JMG_Utility_AI_Goto_Enemy","HuntSearchDistance=-1.0:float,HuntSpeed=1.0:float,HuntArriveDistance=1.0:float,RandomHuntArriveDistance=0.0:float,HuntStealth=0:int,AttackSpeed=1.0:float,AttackDistance=-1.0:float,RandomAttackDistance=0.0:float,ReturnHome=1:int,ReturnHomeSpeed=1.0:float,WanderingAIGroupID=-1:int,WanderSpeed=1.0:float,CanSeeStealth=1:int,ShutdownEngineOnArrival=0:int,AttackCheckBlocked=1:int");
ScriptRegistrant<JMG_Utility_AI_Goto_Enemy_Not_Star> JMG_Utility_AI_Goto_Enemy_Not_Star_Registrant("JMG_Utility_AI_Goto_Enemy_Not_Star","HuntSearchDistance=-1.0:float,HuntSpeed=1.0:float,HuntArriveDistance=1.0:float,RandomHuntArriveDistance=0.0:float,HuntStealth=0:int,AttackSpeed=1.0:float,AttackDistance=-1.0:float,RandomAttackDistance=0.0:float,ReturnHome=1:int,ReturnHomeSpeed=1.0:float,WanderingAIGroupID=-1:int,WanderSpeed=1.0:float,CanSeeStealth=1:int,ShutdownEngineOnArrival=0:int,AttackCheckBlocked=1:int");
ScriptRegistrant<JMG_Utility_Grant_Key_On_Create> JMG_Utility_Grant_Key_On_Create_Registrant("JMG_Utility_Grant_Key_On_Create","Key:int,Grant=1:int");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom> JMG_Utility_Custom_Send_Custom_Registrant("JMG_Utility_Custom_Send_Custom","Custom:int,ID=0:int,SendCustom:int,Param:int,Delay:float");
ScriptRegistrant<JMG_Utility_Damage_Unoccupied_Vehicle> JMG_Utility_Damage_Unoccupied_Vehicle_Registrant("JMG_Utility_Damage_Unoccupied_Vehicle","Rate=0.1:float,Delay=60.0:float,DecreaseTick=0.1:float,IncreaseTick=0.1:float,Damage=1.0:float,Warhead=None:string");
ScriptRegistrant<JMG_Utility_Custom_Damage_All_Soldiers_On_Team> JMG_Utility_Custom_Damage_All_Soldiers_On_Team_Registrant("JMG_Utility_Custom_Damage_All_Soldiers_On_Team","Custom:int,Team:int,Damage:float,Warhead=None:string");
ScriptRegistrant<JMG_Utility_AI_Guardian_Vehicle> JMG_Utility_AI_Guardian_Vehicle_Registrant("JMG_Utility_AI_Guardian_Vehicle","WanderingAIGroupID:int,WanderSpeed=1.0:float,FireRange=100.0:float,FaceTarget=1:int,CheckBlocked=1:int,AimAtFeet=1:int,TurnOffEngineOnArrival=1:int");
ScriptRegistrant<JMG_Utility_Custom_Destroy_Closest_Model_To_Position> JMG_Utility_Custom_Destroy_Closest_Model_To_Position_Registrant("JMG_Utility_Custom_Destroy_Closest_Model_To_Position","Custom:int,Model:string,Position:Vector3,MaxDistance=0.0:float");
ScriptRegistrant<JMG_Utility_Send_Custom_On_Deaths_Controller> JMG_Utility_Send_Custom_On_Deaths_Controller_Registrant("JMG_Utility_Send_Custom_On_Deaths_Controller","MaxDeaths=100:int,DeathReminder=25:int,UrgentDeathReminder=10:int,StringID=12628:int,ReminderMessage=%d %s^ only %d more %s allowed!:string,Delim=^:string,ReminderMessageOrder=0:int,DeathSingular=casualty:string,DeathPlural=casualties:string,RemainingSingular=death:string,RemainingPlural=deaths:string,ReminderColor=1.0 0.0 0.0:Vector3,ID=0:int,Custom=0:int,Param=0:int,Delay=0:int,ReminderCustom=0:int,AddDeathsWhenNoPlayers=0.0:float,NoPlayersAddDeathSaftyTime=0:int");
ScriptRegistrant<JMG_Utility_Send_Custom_On_Deaths_Reporter> JMG_Utility_Send_Custom_On_Deaths_Reporter_Registrant("JMG_Utility_Send_Custom_On_Deaths_Reporter","");
ScriptRegistrant<JMG_Utility_AI_Goto_Player_Ignore_Object> JMG_Utility_AI_Goto_Player_Ignore_Object_Registrant("JMG_Utility_AI_Goto_Player_Ignore_Object","");
ScriptRegistrant<JMG_Utility_AI_Goto_Enemy_Ignore_Object> JMG_Utility_AI_Goto_Enemy_Ignore_Object_Registrant("JMG_Utility_AI_Goto_Enemy_Ignore_Object","");
ScriptRegistrant<JMG_Utility_AI_Goto_Enemy_Not_Star_Ignore_Object> JMG_Utility_AI_Goto_Enemy_Not_Star_Ignore_Object_Registrant("JMG_Utility_AI_Goto_Enemy_Not_Star_Ignore_Object","");
ScriptRegistrant<JMG_Utility_Custom_Set_Team_And_Model> JMG_Utility_Custom_Set_Team_And_Model_Registrant("JMG_Utility_Custom_Set_Team_And_Model","Custom:int,Model:string,Team:int,ResetActionPriority=100.0:float");
ScriptRegistrant<JMG_Utility_Death_Warhead_Create_Object> JMG_Utility_Death_Warhead_Create_Object_Registrant("JMG_Utility_Death_Warhead_Create_Object","WarheadName:string,Chance=1.0:float,Preset:string");
ScriptRegistrant<JMG_Utility_Zone_Change_Character_On_Preset_Enter> JMG_Utility_Zone_Change_Character_On_Preset_Enter_Registrant("JMG_Utility_Zone_Change_Character_On_Preset_Enter","EntererPreset:string,PlayerType:int,Preset:string");
ScriptRegistrant<JMG_Utility_Sync_HP_With_Object> JMG_Utility_Sync_HP_With_Object_Registrant("JMG_Utility_Sync_HP_With_Object","ID:int,Rate=0.1:float");
ScriptRegistrant<JMG_Utility_Set_Screen_Color_Fade_Controller> JMG_Utility_Set_Screen_Color_Fade_Controller_Registrant("JMG_Utility_Set_Screen_Color_Fade_Controller","Color=0.0 0.0 0.0:Vector3,Opacity=0.0:float");
ScriptRegistrant<JMG_Utility_Set_Screen_Color_Fade_On_Custom> JMG_Utility_Set_Screen_Color_Fade_On_Custom_Registrant("JMG_Utility_Set_Screen_Color_Fade_On_Custom","Custom:int,Color=0.0 0.0 0.0:Vector3,Opacity=0.0:float,Transition=0.0:float,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Set_Screen_Color_Fade_Reset_Player_Create> JMG_Utility_Set_Screen_Color_Fade_Reset_Player_Create_Registrant("JMG_Utility_Set_Screen_Color_Fade_Reset_Player_Create","Transition=0.0:float");
ScriptRegistrant<JMG_Utility_Set_Screen_Color_Fade_Reset_Player_Enter> JMG_Utility_Set_Screen_Color_Fade_Reset_Player_Enter_Registrant("JMG_Utility_Set_Screen_Color_Fade_Reset_Player_Enter","Transition:float,PlayerType=2:int");
ScriptRegistrant<JMG_Utility_Simple_Mech> JMG_Utility_Simple_Mech_Registrant("JMG_Utility_Simple_Mech","ForwardAnimation:string,BackwardAnimation:string,IdleAnimation:string,TurnLeftAnimation:string,TurnRightAnimation:string,ReleaseDelay=4:int");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Preset_In_Range> JMG_Utility_Send_Custom_When_Preset_In_Range_Registrant("JMG_Utility_Send_Custom_When_Preset_In_Range","Preset:string,Range:float,Rate=0.1:float,ID:int,Custom:int,Param:int,Delay=0.0:float,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Death_Create_Object_At_Bone_Position> JMG_Utility_Death_Create_Object_At_Bone_Position_Registrant("JMG_Utility_Death_Create_Object_At_Bone_Position","Preset:string,Bone:string,MaxDistance=1.5:float");
ScriptRegistrant<JMG_Utility_Enable_Loiter> JMG_Utility_Enable_Loiter_Registrant("JMG_Utility_Enable_Loiter","");
ScriptRegistrant<JMG_Utility_Custom_Switch_Weapon> JMG_Utility_Custom_Switch_Weapon_Registrant("JMG_Utility_Custom_Switch_Weapon","Custom:int,Weapon:string");
ScriptRegistrant<JMG_Utility_HUD_Count_Down> JMG_Utility_HUD_Count_Down_Registrant("JMG_Utility_HUD_Count_Down","TimeInSeconds:int,StringID=12628:int,Color=1.0 0.0 0.0:Vector3,WarningMessage=%d %s before launch.:string,Delim=^:string,HourSingular=hour:string,HourPlural=hours:string,MinuteSingular=minute:string,MinutePlural=minutes:string,SecondSingular=second:string,SecondPlural=seconds:string");
ScriptRegistrant<JMG_Utility_HUD_Count_Down_Send_Custom> JMG_Utility_HUD_Count_Down_Send_Custom_Registrant("JMG_Utility_HUD_Count_Down_Send_Custom","TriggerTime:int,ID:int,Custom:int,Param=0:int,Delay=0.0:float");
ScriptRegistrant<JMG_Utility_Zone_Screen_Fade> JMG_Utility_Zone_Screen_Fade_Registrant("JMG_Utility_Zone_Screen_Fade","Color:vector3,Opacity:float,Transition:float,PlayerType=2:int");
ScriptRegistrant<JMG_Utility_Custom_Triggers_Enemy_Seen> JMG_Utility_Custom_Triggers_Enemy_Seen_Registrant("JMG_Utility_Custom_Triggers_Enemy_Seen","Custom:int,Visible:int");
ScriptRegistrant<JMG_Utility_Screen_Fade_Red_On_Damage> JMG_Utility_Screen_Fade_Red_On_Damage_Registrant("JMG_Utility_Screen_Fade_Red_On_Damage","FadeColor=0.75 0.0 0.0:vector3,FadeOpacity=0.85:float");
ScriptRegistrant<JMG_Utility_Player_Count_Enable_Spawners> JMG_Utility_Player_Count_Enable_Spawners_Registrant("JMG_Utility_Player_Count_Enable_Spawners","StartID:int,EndID:int,PlayerCount[eq|<|>]=-1.0 -1.0 -1.0:vector3,PlayerCount[neq|between(|)]=-1.0 -1.0 -1.0:vector3,EnableOrDisableCustom=0:int,StartsEnabled=1:int");
ScriptRegistrant<JMG_Utility_AI_Engineer_Ignore_Target> JMG_Utility_AI_Engineer_Ignore_Target_Registrant("JMG_Utility_AI_Engineer_Ignore_Target","");
ScriptRegistrant<JMG_Utility_Give_Weapon_In_Range> JMG_Utility_Give_Weapon_In_Range_Registrant("JMG_Utility_Give_Weapon_In_Range","WeaponName:string,Location:vector3,Range:float,EnableOrDisableCustom:int,StartsEnabled=1:int,Rate=0.1:float,SelectWeapon=0:int,WeaponAmmo=1000:int,RefillRate=0.0:float,RefillAmount=0:int");
ScriptRegistrant<JMG_Utility_AI_Follow_Player_On_Poke_Controller> JMG_Utility_AI_Follow_Player_On_Poke_Controller_Registrant("JMG_Utility_AI_Follow_Player_On_Poke_Controller","MaxFollowers=-1:int");
ScriptRegistrant<JMG_Utility_AI_Follow_Player_On_Poke> JMG_Utility_AI_Follow_Player_On_Poke_Registrant("JMG_Utility_AI_Follow_Player_On_Poke","FollowDistance=1.0:float,FollowNearSpeed=0.25:float,FollowFarDistance=10.0:float,FollowFarSpeed=1.0:float,FollowVeryFarDistance=100.0:float,FollowVeryFarSpeed=10.0:float,FallBackWhenOutOfArmor=1:int,RunHomeSpeed=1.0:float,HealWhileAtHomeLocation=0.0:float,HUDMessagesColor=1.0 0.0 0.0:vector3,MessagePokeFollowerID:int,MessageFollowingYouID:int,MessageFollowingPlayerID:int,MessageMaxFollowersID:int,MessageHealingRequiredID:int,MessageFollowerLostID:int,MessageFollowerInjuredID:int,MessageFollowerKilledID:int,SendCustomID:int,PokedCustom:int,LostCustom:int,InjuredCustom:int,KilledCustom:int,HealedCustom:int,MaxFollowTime=3000:int,MaxIdleTime=300:int");
ScriptRegistrant<JMG_Utility_Timer_Damage_And_Teleport> JMG_Utility_Timer_Damage_And_Teleport_Registrant("JMG_Utility_Timer_Damage_And_Teleport","Delay:float,TeleportToObjectID:int,Location:vector3,DamageAmount:float,DamageWarhead=None:string,DamageSelf=1:int");
ScriptRegistrant<JMG_Utility_Zone_Send_Custom_If_Has_Script> JMG_Utility_Zone_Send_Custom_If_Has_Script_Registrant("JMG_Utility_Zone_Send_Custom_If_Has_Script","ScriptName:string,ID=0:int,Custom=0:int,Param=0:int,Delay=0.0:float,PlayerType=2:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Damage_When_Outside_Of_Range> JMG_Utility_Damage_When_Outside_Of_Range_Registrant("JMG_Utility_Damage_When_Outside_Of_Range","Warn_Distance=0.0:float,Damage_Distance=0.0:float,Vehicle_Warn_Distance=0.0:float,Vehicle_Damage_Distance=0.0:float,Aircraft_Warn_Distance=0.0:float,Aircraft_Damage_Distance=0.0:float,Max_Survive_Distance=0.0:float,Warning_StringID:string,Warning_Color=1.0 0.0 0.0:vector3,Screen_Fade_Color=0.25 0.0 0.0:vector3,DamageWarhead=None:string,InstantKillWarhead=BlamoKiller:string");
ScriptRegistrant<JMG_Utility_Send_Custom_When_No_More_Presets_Of_Type_Exist> JMG_Utility_Send_Custom_When_No_More_Presets_Of_Type_Exist_Registrant("JMG_Utility_Send_Custom_When_No_More_Presets_Of_Type_Exist","PresetName:string,StartsEnabled=0:int,EnableOrDisableCustom:int,ID:int,Custom:int,Param:int,Delay:float");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Player_Outside_Of_Range> JMG_Utility_Send_Custom_When_Player_Outside_Of_Range_Registrant("JMG_Utility_Send_Custom_When_Player_Outside_Of_Range","Distance:float,ID:int,Custom:int,Param:int,Delay:float,TriggerOnce=1:int,SleepTime=10:int,MaxRange=0.0:float");
ScriptRegistrant<JMG_Utility_Set_Team_To_Neutral_When_No_Armor> JMG_Utility_Set_Team_To_Neutral_When_No_Armor_Registrant("JMG_Utility_Set_Team_To_Neutral_When_No_Armor","");
ScriptRegistrant<JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist> JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist_Registrant("JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist","Team:string,StartsEnabled=0:int,EnableOrDisableCustom:int,ID:int,Custom:int,Param:int,Delay:float");
ScriptRegistrant<JMG_Utility_Custom_Create_Sound_At_Object_Bone> JMG_Utility_Custom_Create_Sound_At_Object_Bone_Registrant("JMG_Utility_Custom_Create_Sound_At_Object_Bone","Sound:string,Bone:string,Custom:int");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_On_Player_Count> JMG_Utility_Custom_Send_Custom_On_Player_Count_Registrant("JMG_Utility_Custom_Send_Custom_On_Player_Count","Custom:int,BaseCount:int,PlayerCount:int,ID=0:int,SendCustom:int,Param:int,Delay:float,ResetCustom:int");
ScriptRegistrant<JMG_Utility_Killed_By_Player_Send_Custom> JMG_Utility_Killed_By_Player_Send_Custom_Registrant("JMG_Utility_Killed_By_Player_Send_Custom","ID:int,Custom:int,Param:int,Delay:float");
ScriptRegistrant<JMG_Utility_AI_Guardian_Ignored> JMG_Utility_AI_Guardian_Ignored_Registrant("JMG_Utility_AI_Guardian_Ignored","");
ScriptRegistrant<JMG_Utility_AI_Vehicle_Ignored> JMG_Utility_AI_Vehicle_Ignored_Registrant("JMG_Utility_AI_Vehicle_Ignored","");
ScriptRegistrant<JMG_Utility_Killed_By_PresetID_Send_Custom> JMG_Utility_Killed_By_PresetID_Send_Custom_Registrant("JMG_Utility_Killed_By_PresetID_Send_Custom","PresetID:int,ID:int,Custom:int,Param:int,Delay:float");
ScriptRegistrant<JMG_Utility_Created_Override_AI_Soldier_Muzzle_Direction> JMG_Utility_Created_Override_AI_Soldier_Muzzle_Direction_Registrant("JMG_Utility_Created_Override_AI_Soldier_Muzzle_Direction","");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_On_Player_Count_HUD> JMG_Utility_Custom_Send_Custom_On_Player_Count_HUD_Registrant("JMG_Utility_Custom_Send_Custom_On_Player_Count_HUD","Custom:int,BaseCount:int,PlayerCount:int,ID=0:int,SendCustom:int,Param:int,Delay:float,ResetCustom:int,HudStringId:int,HudStringColor:Vector3");
ScriptRegistrant<JMG_Utility_Zone_Send_Custom_If_Has_Weapon_Ammo> JMG_Utility_Zone_Send_Custom_If_Has_Weapon_Ammo_Registrant("JMG_Utility_Zone_Send_Custom_If_Has_Weapon_Ammo","WeaponName:string,ID=0:int,Custom=0:int,Param=0:int,Delay=0.0:float,PlayerType=2:int,RemoveWeapon=0:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Custom_Remove_Weapon_From_All_Players> JMG_Utility_Custom_Remove_Weapon_From_All_Players_Registrant("JMG_Utility_Custom_Remove_Weapon_From_All_Players","Custom:int,WeaponName:string");
ScriptRegistrant<JMG_Utility_Custom_Destroy_All_Presets_By_Name> JMG_Utility_Custom_Destroy_All_Presets_By_Name_Registrant("JMG_Utility_Custom_Destroy_All_Presets_By_Name","Custom:int,PresetName:string");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Damage_Exceedes_Threshold> JMG_Utility_Send_Custom_When_Damage_Exceedes_Threshold_Registrant("JMG_Utility_Send_Custom_When_Damage_Exceedes_Threshold","DamageThreshold:float,LockoutTime:float,ID:int,Custom:int,Param:int,Delay:float");
ScriptRegistrant<JMG_Utility_Custom_Send_Translated_String_To_All_Players> JMG_Utility_Custom_Send_Translated_String_To_All_Players_Registrant("JMG_Utility_Custom_Send_Translated_String_To_All_Players","Custom:int,StringID:string,ColorRGB:Vector3,Repeatable=1:int");
ScriptRegistrant<JMG_Utility_Destroyed_Drop_Powerup> JMG_Utility_Destroyed_Drop_Powerup_Registrant("JMG_Utility_Destroyed_Drop_Powerup","WeaponPreset:string,PowerupName:string,HeightAdjust=0.0:float");
ScriptRegistrant<JMG_Utility_Pickup_Attach_Script> JMG_Utility_Pickup_Attach_Script_Registrant("JMG_Utility_Pickup_Attach_Script","Script:string,Params:string,Delim=@:string");
ScriptRegistrant<JMG_Utility_Objective_System_Objective_Status_Update_Custom> JMG_Utility_Objective_System_Objective_Status_Update_Custom_Registrant("JMG_Utility_Objective_System_Objective_Status_Update_Custom","Custom:int,ObjectiveID:int,NewObjectiveStringID:int,NewObjectiveMarkerObjectID:int");
ScriptRegistrant<JMG_Utility_Objective_System_Objective_Failed_Custom> JMG_Utility_Objective_System_Objective_Failed_Custom_Registrant("JMG_Utility_Objective_System_Objective_Failed_Custom","Custom:int,ObjectiveID:int");
ScriptRegistrant<JMG_Utility_Force_Player_Team_At_Gameover> JMG_Utility_Force_Player_Team_At_Gameover_Registrant("JMG_Utility_Force_Player_Team_At_Gameover","Team:int");
ScriptRegistrant<JMG_Utility_AI_Guardian_Generic> JMG_Utility_AI_Guardian_Generic_Registrant("JMG_Utility_AI_Guardian_Generic","WanderingAIGroupID:int,WanderSpeed=1.0:float,FireRange=100.0:float,FaceTarget=1:int,CheckBlocked=1:int,ArriveDistance=1.0:float,FlightHeight=0.0:float,TurnOffEngineOnArrival=1:int,UseSecondaryAttack=0:int");
ScriptRegistrant<JMG_Utility_Custom_Create_Object_In_Front_Of_Self> JMG_Utility_Custom_Create_Object_In_Front_Of_Self_Registrant("JMG_Utility_Custom_Create_Object_In_Front_Of_Self","Custom:int,PresetName:string,Distance:float,Height:float,Rotation:float");
ScriptRegistrant<JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist_Ignore> JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist_Ignore_Registrant("JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist_Ignore","Ignore:int");
ScriptRegistrant<JMG_Utility_Custom_Damage_All_Vehicles_On_Team> JMG_Utility_Custom_Damage_All_Vehicles_On_Team_Registrant("JMG_Utility_Custom_Damage_All_Vehicles_On_Team","Custom:int,Team:int,Damage:float,Warhead=None:string");
ScriptRegistrant<JMG_Utility_Kill_Unit_If_Not_Moving_Enough> JMG_Utility_Kill_Unit_If_Not_Moving_Enough_Registrant("JMG_Utility_Kill_Unit_If_Not_Moving_Enough","Time=60.0:float,Distance=1.0:float");
ScriptRegistrant<JMG_Utility_Custom_Display_Briefing_Message_StringID> JMG_Utility_Custom_Display_Briefing_Message_StringID_Registrant("JMG_Utility_Custom_Display_Briefing_Message_StringID","Custom=0:int,StringID:string,MessageColor=239.0 228.0 176.0:Vector3,LineDelayTime=10.0:float,UISound=null:string");
ScriptRegistrant<JMG_Utility_Destroy_Objects_In_ID_Range_On_Killed> JMG_Utility_Destroy_Objects_In_ID_Range_On_Killed_Registrant("JMG_Utility_Destroy_Objects_In_ID_Range_On_Killed","StartID:int,EndID:int");
ScriptRegistrant<JMG_Utility_Timer_Custom> JMG_Utility_Timer_Custom_Registrant("JMG_Utility_Timer_Custom","Time:float,ID:int,Message:int,Param=0:int,Repeat=0:int");
ScriptRegistrant<JMG_Utility_Zone_Change_Character_Model> JMG_Utility_Zone_Change_Character_Model_Registrant("JMG_Utility_Zone_Change_Character_Model","NewModel:string,PlayerType=2:int");
ScriptRegistrant<JMG_Utility_Timer_Change_Character_Models_On_Team> JMG_Utility_Timer_Change_Character_Models_On_Team_Registrant("JMG_Utility_Timer_Change_Character_Models_On_Team","Time:float,NewModel:string,PlayerType=2:int,Repeat=0:int,StartsEnabled=1:int,EnableCustom=0:int");
ScriptRegistrant<JMG_Utility_Timer_Change_Character_Models_On_HP> JMG_Utility_Timer_Change_Character_Models_On_HP_Registrant("JMG_Utility_Timer_Change_Character_Models_On_HP","Time:float,NewModel:string,MinHP:float,MaxHP:float,PlayerType=2:int,Repeat=0:int,StartsEnabled=1:int,EnableCustom=0:int");
ScriptRegistrant<JMG_Utility_Destroy_Send_Custom> JMG_Utility_Destroy_Send_Custom_Registrant("JMG_Utility_Destroy_Send_Custom","ID:int,Message:int,Param=0:int,Delay=0:float");
ScriptRegistrant<JMG_Utility_Set_Vehicle_Collisions_Ghost_When_Unoccupied> JMG_Utility_Set_Vehicle_Collisions_Ghost_When_Unoccupied_Registrant("JMG_Utility_Set_Vehicle_Collisions_Ghost_When_Unoccupied","");
ScriptRegistrant<JMG_Utility_Attach_Script_To_All_Players> JMG_Utility_Attach_Script_To_All_Players_Registrant("JMG_Utility_Attach_Script_To_All_Players","Script:string,Params:string,Delim=@:string");
ScriptRegistrant<JMG_Utility_Send_Custom_On_Powerup_Pickup> JMG_Utility_Send_Custom_On_Powerup_Pickup_Registrant("JMG_Utility_Send_Custom_On_Powerup_Pickup","ID:int,Message:int,Param=0:int,Delay=0:float");
ScriptRegistrant<JMG_Utility_Set_Bullets_On_Custom_Or_Damage> JMG_Utility_Set_Bullets_On_Custom_Or_Damage_Registrant("JMG_Utility_Set_Bullets_On_Custom_Or_Damage","WeaponName=null:string,Custom:int,TriggerOnDamage=1:int,SetBackpackBullets=-1:int,StartWithNoBullets=1:int,Repeat=0:int,FullClip=1:int");
ScriptRegistrant<JMG_Utility_Custom_Damage_All_Presets> JMG_Utility_Custom_Damage_All_Presets_Registrant("JMG_Utility_Custom_Damage_All_Presets","Custom:int,PresetName:string,Damage:float,Warhead=None:string");
ScriptRegistrant<JMG_Utility_Death_Weapon_Create_Object> JMG_Utility_Death_Weapon_Create_Object_Registrant("JMG_Utility_Death_Weapon_Create_Object","WeaponPreset:string,Chance=1.0:float,Preset:string");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Speed_Exceeds_Amount> JMG_Utility_Send_Custom_When_Speed_Exceeds_Amount_Registrant("JMG_Utility_Send_Custom_When_Speed_Exceeds_Amount","Speed:float,ID:int,Message:int,Param=0:int,Rate=0.1:float,Repeat=0:int,StartsEnabled=1:int,EnableCustom=0:int");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Speed_Below_Amount> JMG_Utility_Send_Custom_When_Speed_Below_Amount_Registrant("JMG_Utility_Send_Custom_When_Speed_Below_Amount","Speed:float,ID:int,Message:int,Param=0:int,Rate=0.1:float,Repeat=0:int,StartsEnabled=1:int,EnableCustom=0:int");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Velocity_Exceeds_Amount> JMG_Utility_Send_Custom_When_Velocity_Exceeds_Amount_Registrant("JMG_Utility_Send_Custom_When_Velocity_Exceeds_Amount","Velocity[Forward|Backward|Left]=-1.0 -1.0 -1.0:Vector3,Velocity[Right|Up|Down]=-1.0 -1.0 -1.0:Vector3,OnlyTriggerOn=0:int,OnlyTriggerOnMin[Horizontal|Vertical]=-1.0 -1.0 0.0:Vector3,OnlyTriggerOnMax[Horizontal|Vertical]=-1.0 -1.0 0.0:Vector3,ID:int,Message:int,Param=0:int,Rate=0.1:float,Repeat=0:int,StartsEnabled=1:int,EnableCustom=0:int");
ScriptRegistrant<JMG_Utility_Custom_Change_Character> JMG_Utility_Custom_Change_Character_Registrant("JMG_Utility_Custom_Change_Character","Custom:int,Preset:string,Cost:float,SuccessSound:string,FailSound:string,AllowRepurchase=0:int");
ScriptRegistrant<JMG_Utility_Apply_Damage_While_In_Zone> JMG_Utility_Apply_Damage_While_In_Zone_Registrant("JMG_Utility_Apply_Damage_While_In_Zone","Rate:float,DamageAmount:float,Warhead:string,DamagerId:int");
ScriptRegistrant<JMG_Utility_Apply_Damage_On_Timer_Base> JMG_Utility_Apply_Damage_While_In_Zone_Attached_Registrant("JMG_Utility_Apply_Damage_While_In_Zone_Attached","Rate:float,DamageAmount:float,Warhead:string,DamagerId:int");
ScriptRegistrant<JMG_Utility_Apply_Damage_On_Timer_Base> JMG_Utility_Apply_Damage_On_Timer_Registrant("JMG_Utility_Apply_Damage_On_Timer","Rate:float,DamageAmount:float,Warhead:string,DamagerId:int");
ScriptRegistrant<JMG_Utility_Zone_Send_Custom_Exit> JMG_Utility_Zone_Send_Custom_Exit_Registrant("JMG_Utility_Zone_Send_Custom_Exit","ID=0:int,Custom=0:int,Param=0:int,Delay=0.0:float,PlayerType=2:int,TriggerOnce=1:int");
ScriptRegistrant<JMG_Utility_Custom_Enable_Spawners_In_Range_Modular> JMG_Utility_Custom_Enable_Spawners_In_Range_Modular_Registrant("JMG_Utility_Custom_Enable_Spawners_In_Range_Modular","StartID:int,EndID:int,Custom:int,Enable=1:int,Mod=1:int");
ScriptRegistrant<JMG_Utility_Custom_Create_Explosion_At_Bone> JMG_Utility_Custom_Create_Explosion_At_Bone_Registrant("JMG_Utility_Custom_Create_Explosion_At_Bone","Custom:int,Explosion:string,Bone:string,Owner:int");
ScriptRegistrant<JMG_Utility_Custom_Send_Custom_Repeat_Ignore_Time> JMG_Utility_Custom_Send_Custom_Repeat_Ignore_Time_Registrant("JMG_Utility_Custom_Send_Custom_Repeat_Ignore_Time","Custom:int,ID:int,SendCustom:int,Param:int,Delay:float,IgnoreTime:float,StartsEnabled=1:int,EnableCustom=0:int");
ScriptRegistrant<JMG_Utility_Damage_Send_Custom> JMG_Utility_Damage_Send_Custom_Registrant("JMG_Utility_Damage_Send_Custom","MinDamage:float,Custom:int,Param:int,Delay:int,ID:int,SenderID:int,StartsEnabled=1:int,EnableCustom=0:int,Repeat=1:int");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Player_Inside_Of_Range> JMG_Utility_Send_Custom_When_Player_Inside_Of_Range_Registrant("JMG_Utility_Send_Custom_When_Player_Inside_Of_Range","Distance:float,ID:int,Custom:int,Param:int,Delay:float,TriggerOnce=1:int,SleepTime=10:int");
ScriptRegistrant<JMG_Utility_Custom_Enable_Spawners_In_Range_ModPlayer> JMG_Utility_Custom_Enable_Spawners_In_Range_ModPlayer_Registrant("JMG_Utility_Custom_Enable_Spawners_In_Range_ModPlayer","StartID:int,EndID:int,Custom:int,Enable=1:int,Mod=1:int,PlayerMulti=1.0:float,Invert=0:int");
ScriptRegistrant<JMG_Utility_Spawn_With_Last_Selected_Gun_Control> JMG_Utility_Spawn_With_Last_Selected_Gun_Control_Registrant("JMG_Utility_Spawn_With_Last_Selected_Gun_Control","");
ScriptRegistrant<JMG_Utility_Spawn_With_Last_Selected_Gun_Player> JMG_Utility_Spawn_With_Last_Selected_Gun_Player_Registrant("JMG_Utility_Spawn_With_Last_Selected_Gun_Player","GrantWeapon=1:int,GrantDelay=0.0:float");
ScriptRegistrant<JMG_Utility_Created_Give_Weapon> JMG_Utility_Created_Give_Weapon_Registrant("JMG_Utility_Created_Give_Weapon","WeaponName:string,GrantWeapon=1:int,GrantRounds=99999:int,GrantClips=1:int");
