
#include "general.h"

#include "JmgUtility.h"
#include "GameObjManager.h"
#include "VehicleGameObj.h"
#include "WeaponClass.h"
#include "VehicleGameObjDef.h"
#include "OffenseObjectClass.h"
#include "SoldierGameObj.h"
#include "BuildingGameObj.h"
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
			Commands->Send_Custom_Event(obj,sendId,Get_Int_Parameter("Death_Message"),Commands->Get_ID(damager),0);
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
			Commands->Send_Custom_Event(obj,sendId,Get_Int_Parameter("Repaired_Message"),Commands->Get_ID(damager),0);
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
void JMG_Turret_Spawn::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
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
	if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		if (hasDriver && obj->As_VehicleGameObj() && !Get_Vehicle_Occupant_Count(obj))
		{
			hasDriver = false;
			GameObject *turret = Commands->Find_Object(turretId);
			if (turret)
				Commands->Action_Reset(turret,100);
		}
	}
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
	Force_Position_Update(obj);
	Commands->Start_Timer(obj,this,0.25f,1);
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
				GameObject *spawnedObject = Commands->Create_Object(Get_Parameter("SpawnPreset"),createPos);
				Commands->Set_Facing(spawnedObject,Commands->Get_Facing(obj));
				MoveablePhysClass *mphys = spawnedObject->As_PhysicalGameObj() ? spawnedObject->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
				if (!mphys || mphys->Can_Teleport(Matrix3D(createPos)))
				{
					if (spawnLimit > 0)
						spawnLimit--;
					spawnedId = Commands->Get_ID(spawnedObject);
					CalculateRespawnTime();
					char params[256];
					sprintf(params,"%d",Commands->Get_ID(obj));
					Commands->Attach_Script(spawnedObject,"JMG_Utility_Basic_Spawner_Attach",params);
				}
				else
				{
					Commands->Destroy_Object(spawnedObject);
					respawnTime = 0.1f;
				}
			}
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Utility_Basic_Spawner::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (Get_Int_Parameter("EnableMessage") && message == Get_Int_Parameter("EnableMessage"))
	{
		if (param)
		{
			enabled = true;
			CalculateRespawnTime();
		}
		else
			enabled = false;
	}
	if (message == 6873521)
	{
		CalculateRespawnTime();
		spawnedId = 0;
	}
}
void JMG_Utility_Basic_Spawner::CalculateRespawnTime()
{
	respawnTime = min(Get_Int_Parameter("DelayOnStartup") ? (Get_Float_Parameter("RespawnTime")+(Get_Float_Parameter("RespawnTimeRandom") ? Commands->Get_Random(-Get_Float_Parameter("RespawnTime"),Get_Float_Parameter("RespawnTime")) : 0.0f)) : 0,0.1f);
}
void JMG_Utility_Basic_Spawner_Attach::Destroyed(GameObject *obj)
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
	repairTargetId = 0;
	actionUpdate = 0;
	repairGun = true;
	lastPos = Commands->Get_Position(obj);
	samePosition = 0;
	lastRepairTargetId = 0;
	lastTarget = NULL;
	lastSecondaryTarget = NULL;
	lastRepairTarget = false;
	lastUseRepairGun = false;
	for (int x = 1;x < 128;x++)
		aiIgnorePlayers[x] = 0;
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
		GameObject *lastRep = Commands->Find_Object(lastRepairTargetId);
		if (!lastRep || Get_Hitpoints(lastRep) >= Get_Max_Hitpoints(lastRep) || !Commands->Get_Health(lastRep))
		{
			lastRep = NULL;
			lastRepairTargetId = 0;
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
				if ((!repairTarget || tempDistance < nearestObject))
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
					if ((!repairTarget || tempDistance < nearestObject))
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
					if ((!repairTarget || tempDistance < nearestObject))
					{
						repairTarget = o;
						nearestObject = tempDistance;
					}
				}
			}
		if (repairTarget)
			lastRepairTargetId = repairTargetId = Commands->Get_ID(repairTarget);
		if (repairTargetId && !repairTarget)
		{
			repairTargetId = 0;
			actionUpdate = 0;
			samePosition = 100;
		}
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
		if (!target && !repairTarget && samePosition >= 10 && (Get_Float_Parameter("PatrolRange") > 0 ? true : false))
		{
			Vector3 pos = centerLocation,returnPos;
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
		if (JmgUtility::SimpleDistance(pos,lastPos) < 0.1)
			samePosition++;
		else
			samePosition = 0;
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
			pos.X += cos(rotation)*Get_Float_Parameter("PatrolRange");
			pos.Y += sin(rotation)*Get_Float_Parameter("PatrolRange");
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
			pos.X += cos(rotation)*Get_Float_Parameter("PatrolRange");
			pos.Y += sin(rotation)*Get_Float_Parameter("PatrolRange");
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
	if (!target || target == obj || Commands->Get_Player_Type(target) != playerType || Get_Hitpoints(target) >= Get_Max_Hitpoints(target))
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
	if (message == Get_Int_Parameter("Message"))
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
	if (Get_Int_Parameter("WanderingAIGroupID") != -1)
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
	if(!attacking)
		RunAttack(obj,SetTarget(damager));
	else 
		RunAttack(obj,SelectTarget(obj,damager));
	lastSeenCount = Commands->Get_Random_Int(6,15);
}
void JMG_Utility_AI_Vehicle::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	Vector3 enemyPos = Commands->Get_Position(seen);
	float dist = JmgUtility::SimpleDistance(Commands->Get_Position(obj),enemyPos);
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
		Vector3 mypos = Commands->Get_Position(obj);
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
	if (!moving)
	{
		drivingBackward= false;
		return;
	}
	Vector3 current = Commands->Get_Position(obj);
	if (reverseTime)
	{
		reverseTime--;
		stuckCount = 0;
		mypos = current;
		drivingBackward = false;
		AttackMove(obj,Commands->Find_Object(lastAction.targetId),lastAction.following,lastAction.position,lastAction.useAmmo,0.0f,true,currentAction.arriveDistance);
		return;
	}
	if(JmgUtility::SimpleDistance(mypos,current) < 1.0f)
	{
		if(stuckCount > 2)
		{
			reverseTime = Commands->Get_Random_Int(1,6);
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
	Vector3 mypos = Commands->Get_Position(obj);
	float dist = 9999.9f;
	if (target)
		if (followTarget)
			dist = JmgUtility::SimpleDistance(Commands->Get_Position(target),mypos);
		else
			dist = JmgUtility::SimpleDistance(currentAction.position,mypos);
	if (dist > minDistanceSquared+1089.0f)
		retreatPos = mypos;
	if (dist < minDistanceSquared && !(Get_Int_Parameter("SquishInfantry") && target->As_SoldierGameObj()))
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
	params.Set_Basic(this,10,10);
	if (target)
	{
		JMGVehicleAmmo ammo = (fireMode ? primary : secondary);
		float weaponError = 0.0f;
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
		Commands->Action_Attack(obj,params);
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
	Commands->Action_Goto(obj,params);
	if (target)
		Commands->Action_Attack(obj,params);
}
JMGVehicleAmmo JMG_Utility_AI_Vehicle::DefineAmmo(const AmmoDefinitionClass *ammo)
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
	if (fadeOut && fadeIn)
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
		if (CheckPlayerType(poker,Get_Int_Parameter("PlayerType")))
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
		JmgUtility::MessageAllPlayers((int)Get_Vector3_Parameter("ColorRGB").X,(int)Get_Vector3_Parameter("ColorRGB").Y,(int)Get_Vector3_Parameter("ColorRGB").Z,Get_Parameter("Message"));
	if (!Get_Int_Parameter("Repeatable"))
		Destroy_Script();
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
	togglePathfind = 0;
	pathfindOn = false;
	forcePathfindOverride = false;
	stuckReturnHome = 0;
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
					params.MovePathfind = forcePathfindOverride ? pathfindOn : true;
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
					params.MovePathfind = forcePathfindOverride ? pathfindOn : JmgUtility::SimpleDistance(Commands->Get_Position(target),Commands->Get_Position(obj)) <= noPathfindRange ? false : true;
					Commands->Action_Goto(obj,params);
					params.Set_Basic(this,999,10);
					params.Set_Attack(target,Get_Float_Parameter("StartAttackDistance"),1,true);
					Commands->Action_Attack(obj,params);
				}
			}
			Vector3 pos = Commands->Get_Position(obj);
			if (JmgUtility::SimpleDistance(pos,lastPos) < 1.0)
			{
				stuckReturnHome++;
				if (stuckReturnHome > 150)
				{
					stuckTime = 0;
					togglePathfind = 0;
					pathfindOn = false;
					forcePathfindOverride = false;
					stuckReturnHome = 0;
					Commands->Set_Position(obj,homelocation);
				}
				if (stuckTime < 30)
					stuckTime++;
				if (stuckTime == 30)
				{
					forcePathfindOverride = true;
					togglePathfind++;
					if (togglePathfind >= 30)
					{
						pathfindOn = !pathfindOn;
						togglePathfind = 0;
					}
				}
				lastPos = Commands->Get_Position(obj);
			}
			else
			{
				stuckReturnHome = 0;
				stuckTime--;
				if (!stuckTime)
					forcePathfindOverride = false;
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
		sprintf(params,"%d,%d,%s,%d,%d",this->Get_ID(),Commands->Get_ID(obj),Get_Parameter("RepairedPreset"),Get_Vector3_Parameter("MatchTeam[Preview|Deployed|Repaired]").Z,Commands->Get_Player_Type(obj));
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
	Commands->Send_Custom_Event(obj,enter,347341,0,0);
}
void JMG_Utility_Swimming_Zone::Exited(GameObject *obj,GameObject *exiter)
{
	if (!exiter->As_SoldierGameObj())
		return;
	Commands->Send_Custom_Event(obj,exiter,347340,0,0);
}
JMG_Utility_Swimming_Zone::PlayerWaterNode JMG_Utility_Swimming_Zone::waterNode[128] = {JMG_Utility_Swimming_Zone::PlayerWaterNode()};
bool JMG_Utility_Swimming_Infantry::isUnderwater[128] = {false};
bool JMG_Utility_Swimming_Infantry::isInWater[128] = {false};
void JMG_Utility_Swimming_Infantry::Created(GameObject *obj)
{
	heartBeatSoundId = 0;
	pantSoundId = 0;
	playerId = JmgUtility::JMG_Get_Player_ID(obj);
	underwater = false;
	waterZoneCount = 0;
	startedFadeRed = false;
	drownTime = 0.0f;
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
				Set_Screen_Fade_Opacity_Player(obj,0.0f,0.1f);
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
		}
	}
	if (message == 347341)
	{
		if (!waterZoneCount)
		{
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
	Set_Screen_Fade_Opacity_Player(obj,0.0f,0.0f);
	Set_Fog_Range_Player(obj,JMG_Utility_Swimming_Zone::fogMinDistance,JMG_Utility_Swimming_Zone::fogMaxDistance,0.0f);
	DestroySoundEmitter(&heartBeatSoundId);
	DestroySoundEmitter(&pantSoundId);
}
void JMG_Utility_Swimming_Infantry::Detach(GameObject *obj)
{
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
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")))
		return;
	for (int x = Get_Int_Parameter("StartID");x <= Get_Int_Parameter("EndID");x++)
		Commands->Enable_Spawner(x,Get_Int_Parameter("Enable") ? true : false);
}
void JMG_Utility_Display_Message_On_Vehicle_Enter::Created(GameObject *obj)
{
	for (int x = 0;x < 128;x++)
		hasShownMessage[x] = false;
}
void JMG_Utility_Display_Message_On_Vehicle_Enter::Custom(GameObject *obj,int type,int param,GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
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
	if (CheckPlayerType(enter,Get_Int_Parameter("PlayerType")))
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
				EnemyID = 0;
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
	if (!Commands->Is_A_Star(damager))
		return;
	if (Commands->Get_Player_Type(damager) != Commands->Get_Player_Type(obj))
		return;
	Commands->Give_Money(damager,-damage*Get_Damage_Points(obj),false);
	if (Commands->Get_Money(damager) < 0.0f)
		Set_Money(JmgUtility::JMG_Get_Player_ID(damager),0.0f);
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
			if (Get_Int_Parameter("SendMessageObjectID"))
				Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("SendMessageObjectID")),Get_Int_Parameter("NearToBuildingMessage"),0,0);
			else
				Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("NearToBuildingMessage"),0,0);
		}
		if (ClosestDist > Get_Float_Parameter("CloseToBuildingDistance") && nearBuilding)
		{
			nearBuilding = false;
			if (Get_Int_Parameter("SendMessageObjectID"))
				Commands->Send_Custom_Event(obj,Commands->Find_Object(Get_Int_Parameter("SendMessageObjectID")),Get_Int_Parameter("FarFromBuildingMessage"),0,0);
			else
				Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("FarFromBuildingMessage"),0,0);
		}
		Commands->Start_Timer(obj,this,Get_Float_Parameter("CheckRate"),1);
	}
}
void JMG_Utility_AI_Engineer_Repair_Target::Created(GameObject *obj)
{
}
void JMG_Utility_Reset_Screen_Fade_And_Fog_On_Destroy::Destroyed(GameObject *obj)
{
	Set_Screen_Fade_Opacity_Player(obj,0.0f,0.0f);
	Set_Fog_Range_Player(obj,JMG_Utility_Swimming_Zone::fogMinDistance,JMG_Utility_Swimming_Zone::fogMaxDistance,0.0f);
}
void JMG_Utility_Reset_Screen_Fade_And_Fog_On_Destroy::Detach(GameObject *obj)
{
	Destroyed(obj);
}
ScriptRegistrant<JMG_Utility_Check_If_Script_Is_In_Library> JMG_Utility_Check_If_Script_Is_In_Library_Registrant("JMG_Utility_Check_If_Script_Is_In_Library","ScriptName:string,CppName:string");
ScriptRegistrant<JMG_Send_Custom_When_Custom_Sequence_Matched> JMG_Send_Custom_When_Custom_Sequence_Matched_Registrant("JMG_Send_Custom_When_Custom_Sequence_Matched","Success_Custom=0:int,Correct_Step_Custom=0:int,Partial_Failure_Custom=0:int,Failure_Custom=0:int,Send_To_ID=0:int,Custom_0=0:int,Custom_1=0:int,Custom_2=0:int,Custom_3=0:int,Custom_4=0:int,Custom_5=0:int,Custom_6=0:int,Custom_7=0:int,Custom_8=0:int,Custom_9=0:int,Disable_On_Success=1:int,Disable_On_Failure=0:int,Starts_Enabled=1:int,Enable_Custom=0:int,Correct_Step_Saftey=0:int,Failure_Saftey=1:int,Max_Failures=1:int");
ScriptRegistrant<JMG_Utility_Change_Model_On_Timer> JMG_Utility_Change_Model_On_Timer_Registrant("JMG_Utility_Change_Model_On_Timer","Model=null:string,Time=0:float");
ScriptRegistrant<JMG_Utility_Emulate_DamageableStaticPhys> JMG_Utility_Emulate_DamageableStaticPhys_Registrant("JMG_Utility_Emulate_DamageableStaticPhys","Animation=null:string,Killed_Explosion=null:string,Live_Loop_Start=-1.0:float,Live_Loop_End=-1.0:float,Live_Twitch_Start=-1.0:float,Live_Twitch_End=-1.0:float,Death_Transition_Start=-1.0:float,Death_Transition_End=-1.0:float,Death_Loop_Start=-1.0:float,Death_Loop_End=-1.0:float,Death_Twitch_Start=-1.0:float,Death_Twitch_End=-1.0:float,Live_Transition_Start=-1.0:float,Live_Transition_End=-1.0:float,Repaired_Explosion=null:string,Play_Twitches_To_Completion:int,Send_To_ID:int,Death_Message:int,Repaired_Message:int,Damage_Points:float,Death_Points:float,Heal_Points:float,Repair_Points:float,Neutral_When_Dead:int,Disable_Collision_When_Dead:int");
ScriptRegistrant<JMG_Utility_Display_HUD_Info_Text_All_Players_Custom> JMG_Utility_Display_HUD_Info_Text_To_All_Players_On_Custom_Registrant("JMG_Utility_Display_HUD_Info_Text_All_Players_Custom","Custom:int,StringId:int,ColorRGB:Vector3,Repeatable=1:int");
ScriptRegistrant<JMG_Utility_Display_HUD_Info_Text_To_Sender_On_Custom> JMG_Utility_Display_HUD_Info_Text_To_Sender_On_Custom_Registrant("JMG_Utility_Display_HUD_Info_Text_To_Sender_On_Custom","Custom:int,StringId:int,ColorRGB:Vector3,Repeatable=1:int");
ScriptRegistrant<JMG_Utility_Soldier_Transition_On_Custom> JMG_Utility_Soldier_Transition_On_Custom_Registrant("JMG_Utility_Soldier_Transition_On_Custom","Custom:int");
ScriptRegistrant<JMG_Utility_Poke_Send_Self_Custom> JMG_Utility_Poke_Send_Self_Custom_Registrant("JMG_Utility_Poke_Send_Self_Custom","Custom:int,Param:int,Delay:float,LockoutTime=-1.0:float");
ScriptRegistrant<JMG_Turret_Spawn> JMG_Turret_Spawn_Registrant("JMG_Turret_Spawn","Turret_Preset:string,Bone_Name=Tur_Mount:string");
ScriptRegistrant<JMG_Utility_Sync_System_Object> JMG_Utility_Sync_System_Object_Registrant("JMG_Utility_Sync_System_Object","");
ScriptRegistrant<JMG_Utility_Sync_System_Controller> JMG_Utility_Sync_System_Controller_Registrant("JMG_Utility_Sync_System_Controller","Sync_Rate=1.0:float");
ScriptRegistrant<JMG_Utility_Sync_Object_Periodically> JMG_Utility_Sync_Object_Periodically_Registrant("JMG_Utility_Sync_Object_Periodically","Sync_Rate=1.0:float");
ScriptRegistrant<JMG_Utility_Basic_Spawner> JMG_Utility_Basic_Spawner_Registrant("JMG_Utility_Basic_Spawner","SpawnPreset:string,RespawnTime=0.0:float,RespawnTimeRandom=0:float,StartsEnabled=1:int,EnableMessage=0:int,SpawnLimit=-1:int,DelayOnStartup=0:int");
ScriptRegistrant<JMG_Utility_Basic_Spawner_Attach> JMG_Utility_Basic_Spawner_Attach_Registrant("JMG_Utility_Basic_Spawner_Attach","ControllerId:int");
ScriptRegistrant<JMG_Utility_AI_Engineer> JMG_Utility_AI_Engineer_Registrant("JMG_Utility_AI_Engineer","RepaiarGun_Preset=Weapon_RepairGun_Player:string,Weapon_Preset=Weapon_Pistol_Player:string,PatrolRange=50.0:float,BaseCenterPoint=0.0 0.0 0.0:vector3,MinHP[Soldiers|Vehicles|Turrets]=0.5 0.75 0.9:vector3,MinHP[EngineerTarget|C4|Beacon]=0.25 0.0 0.0:vector3,Priority[RepairTargets|C4|Beacons]=1.0 1.0 1.0:vector3");
ScriptRegistrant<JMG_Utility_Set_Object_Visibility_For_Player_On_Custom> JMG_Utility_Set_Object_Visibility_For_Player_On_Custom_Registrant("JMG_Utility_Set_Object_Visibility_For_Player_On_Custom","Message:int,PlayerID:int,Visible:int");
ScriptRegistrant<JMG_Utility_AI_Vehicle> JMG_Utility_AI_Vehicle_Registrant("JMG_Utility_AI_Vehicle","MaxRange=25.0:float,MinAttackRange=0.0:float,FollowTarget=1:int,ReturnHome=1:int,ForceFire=0:int,VTOLHover=0.0:float,vsSoldier=1:int,vsVehicle=1:int,vsAircraft=1:int,WanderingAIGroupID=-1:int,MinRandWander=60.0:float,MaxRandWander=90.0:float,SquishInfantry=1:int,EnableTransitions=0:int,UsePathfind=1:int,DisableEngineOnArrival=1:int,WeaponError=-1.0:float,MaxHuntRange=0.0:float");
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
ScriptRegistrant<JMG_Utility_Swimming_Zone> JMG_Utility_Swim_While_In_Zone_Registrant("JMG_Utility_Swimming_Zone","WaterColor[R|G|B]=0.28 0.43 0.55:vector3,WaterOpacity=0.5:float,WaterMinViewDistance=5.0:float,WaterMaxViewDistance=15.0:float");
ScriptRegistrant<JMG_Utility_Swimming_Infantry> JMG_Utility_Swimming_Infantry_Registrant("JMG_Utility_Swimming_Infantry","WeaponPreset=Weapon_Swimming_Animations:string,DrownTime=10.0:float,StarDrownSequence=3.0:float,GaspForBreath=SFX.SwimmingGaspForBreath:string,PantingSoundEmitterModel=s_panting:string,HeartBeatSoundEmitterModel=s_heartBeat:string,DrownDamageRate=2.5:float,CatchBreathRate=0.33:float");
ScriptRegistrant<JMG_Utility_Zone_Enable_Spawners_In_Range> JMG_Utility_Zone_Enable_Spawners_In_Range_Registrant("JMG_Utility_Zone_Enable_Spawners_In_Range","StartID:int,EndID:int,PlayerType=2:int,Enable=1:int");
ScriptRegistrant<JMG_Utility_Display_Message_On_Vehicle_Enter> JMG_Utility_Display_Message_On_Vehicle_Enter_Registrant("JMG_Utility_Display_Message_On_Vehicle_Enter","StringId:int,MessageOverride=null:string,Color[R|G|B]=0.0 1.0 0.0:vector3,DriverOnly=1:int,ShowOnce=1:int,PlayerType=2:int");
ScriptRegistrant<JMG_Utility_Zone_Apply_Damage_On_Enter> JMG_Utility_Zone_Apply_Damage_On_Enter_Registrant("JMG_Utility_Zone_Apply_Damage_On_Enter","ID:int,DamageAmount:float,Warhead=None:string,DamageOccupants=1:int,PlayerType=2:int,OnlyOnce=0:int");
ScriptRegistrant<JMG_Utility_AI_Guardian_Aircraft> JMG_Utility_AI_Guardian_Aircraft_Registrant("JMG_Utility_AI_Guardian_Aircraft","WanderingAIGroupID:int,FlightHeight=25.0:float,FireRange=100.0:float,FaceTarget=1:int");
ScriptRegistrant<JMG_Utility_Switch_Weapon_While_Primary_Empty> JMG_Utility_Switch_Weapon_While_Primary_Empty_Registrant("JMG_Utility_Switch_Weapon_While_Primary_Empty","PrimaryWeapon=null:string,SecondaryWeapon=null:string,ReloadTime=0.0:float,PrimaryToSecondaryTime=-1.0:float,SecondaryToPrimaryTime=-1.0:float,IdlePrimaryAnim=null:string,IdleSecondaryAnim=null:string,PrimaryToSecondaryAnim=null:string,SecondaryToPrimaryAnim=null:string,PrimaryToSecondarySound=null:string,SecondaryToPrimarySound=null:string");
ScriptRegistrant<JMG_Utility_Send_Custom_When_Near_Building> JMG_Utility_Send_Custom_When_Near_Building_Registrant("JMG_Utility_Send_Custom_When_Near_Building","SendMessageObjectID=0:int,NearToBuildingMessage:int,FarFromBuildingMessage:int,CloseToBuildingDistance=1.0:float,BuildingPlayerType=2:int,CheckDeadBuildings=1:int,CheckRate=0.25:float");
ScriptRegistrant<JMG_Utility_AI_Engineer_Repair_Target> JMG_Utility_AI_Engineer_Repair_Target_Registrant("JMG_Utility_AI_Engineer_Repair_Target","");
ScriptRegistrant<JMG_Utility_Reset_Screen_Fade_And_Fog_On_Destroy> JMG_Utility_Reset_Screen_Fade_And_Fog_On_Destroy_Registrant("JMG_Utility_Reset_Screen_Fade_And_Fog_On_Destroy","");



