/*	Renegade Scripts.dll
	Copyright 2013 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "SoldierGameObj.h"
#include "WeaponBagClass.h"
#include "WeaponClass.h"
#include "WeaponMgr.h"
#include "GameObjManager.h"
#include "MoveablePhysClass.h"
#include "engine_tt.h"
#include "cTeam.h"
#include "PhysicsSceneClass.h"
#include "physcoltest.h"
#include "VehicleGameObj.h"

#include "jmgUtility.h"
#include "jmgBearHunter.h"

void JMG_Bear_Hunter_Player_Soldier::Created(GameObject *obj)
{
	Attach_Script_Once(obj,"JMG_Bear_Hunter_Kill_Score_Tracker","");
	playerId = JmgUtility::JMG_Get_Player_ID(obj);
	bearHunterScoreSystem.Get_Current_Player_Score_Node(playerId);
	if (Commands->Get_Preset_ID(obj) == 1000001140 && JMG_Bear_Hunter_Game_Control::gameState == JMG_Bear_Hunter_Game_Control::BossArrive && LifeSystem(obj))
		return;
	Commands->Give_PowerUp(obj,"POW_Binoculars",false);
	if (JMG_Bear_Hunter_Game_Control::weaponsUnlocked[0])
	{
		Commands->Give_PowerUp(obj,"POW_Twinbow",false);
		Commands->Give_PowerUp(obj,"POW_Twinbow_Explosive",false);
	}
	if (JMG_Bear_Hunter_Game_Control::weaponsUnlocked[6])
		Commands->Give_PowerUp(obj,"POW_Plasma_Rifle",false);
	GrantUnlockedWeapons(obj);
	PlayerData.getPlayerNode(obj);
	if (!PlayerData.players[playerId]->destroyTime || difftime(clock(),PlayerData.players[playerId]->destroyTime) > 500)
	{
		if (Commands->Get_Preset_ID(obj) == 1000001140)
		{
			Set_Background_Music_Player(obj,JMG_Bear_Hunter_Game_Control::currentSong);
			Commands->Start_Timer(obj,this,1.0f,6);
		}
	}
	PlayerData.players[playerId]->displayedKillMessage = false;
	Commands->Set_Objective_HUD_Info_Position(1,1,"PogNorth.tga",12701,Vector3(15.593f,-93.453f,-0.166f));
	Commands->Set_Objective_Radar_Blip(1,Vector3(15.593f,-93.453f,-0.166f));
	Commands->Set_Objective_Radar_Blip(2,Vector3(15.593f,-93.453f,-0.166f));
	Commands->Set_Objective_Radar_Blip(3,Vector3(15.593f,-93.453f,-0.166f));
	Commands->Set_Objective_Radar_Blip(4,Vector3(15.593f,-93.453f,-0.166f));
	Commands->Set_Objective_Radar_Blip(5,JMG_Bear_Hunter_Game_Control::centerOfBase);
	Commands->Set_Objective_Radar_Blip(6,JMG_Bear_Hunter_Game_Control::centerOfBase);
	Commands->Set_Objective_Radar_Blip(7,JMG_Bear_Hunter_Game_Control::centerOfBase);
	sprintf(primaryWeapon[playerId],"%s",Get_Current_Weapon(obj));
	LastTime = clock();
	Commands->Set_Player_Type(obj,1);
	LastPosition = Commands->Get_Position(obj);
	bullets = 0;
	clipBullets = 0;
	if (!PlayerData.players[playerId]->hasGotIntro)
	{
		Commands->Start_Timer(obj,this,10.0f,2);
		Commands->Start_Timer(obj,this,16.0f,3);
		Commands->Start_Timer(obj,this,22.0f,4);
		PlayerData.players[playerId]->killsWithoutGettingHurt = 0;
	}
	if (PlayerData.players[playerId]->firstJoin)
		Commands->Start_Timer(obj,this,0.1f,5);
	if (JMG_Bear_Hunter_Game_Control::gameState >= JMG_Bear_Hunter_Game_Control::MutationStart)
	{
		Set_Screen_Fade_Opacity_Player(obj,(JMG_Bear_Hunter_Game_Control::bearTransition/100.0f)*0.15f,0.0f);
		Set_Screen_Fade_Color_Player(obj,0.0f,0.0f,0.0f,0.0f);
	}
	if (JMG_Bear_Hunter_Game_Control::diedInWater[playerId])
	{
		Set_Screen_Fade_Color_Player(obj,0.0f,0.0f,0.0f,0.0f);
		Set_Screen_Fade_Opacity_Player(obj,0.0f,0.0f);
		JMG_Bear_Hunter_Game_Control::diedInWater[playerId] = false;
	}
	if (JMG_Bear_Hunter_Game_Control::bearTransition && JMG_Bear_Hunter_Game_Control::bearTransition < 100)
		Commands->Start_Timer(obj,this,0.1f,7);
	Commands->Start_Timer(obj,this,0.1f,1);
	if ((JMG_Bear_Hunter_Game_Control::gameState != JMG_Bear_Hunter_Game_Control::BossArrive || JMG_Bear_Hunter_Game_Control::remainingLives[playerId] >= 0) && Get_Player_Name(obj) && _wcsicmp(WideStringClass(Get_Player_Name(obj)), The_Game()->Get_Mvp_Name()) == 0 && _stricmp("CNC_Spawned_Soldier",Commands->Get_Preset_Name(obj)))
		switch (The_Game()->Get_Mvp_Count())
		{
		case 0:break;
		case 1:Commands->Set_Model(obj,"FAWN_");break;
		case 2:Commands->Set_Model(obj,"DOE_");break;
		case 3:Commands->Set_Model(obj,"STAG_");break;
		case 4:Commands->Set_Model(obj,"BUCK_");break;
		default:Commands->Set_Model(obj,"_Moose");break;
		}
	Attach_Script_Once(obj,"JFW_Send_Self_Custom_On_Key","VehBind,454269,0");
	Commands->Attach_Script(obj,"JMG_Rp2_Dedicated_Server_Sound_Emulator","");
	Commands->Start_Timer(obj,this,2.5f,8);
}
void JMG_Bear_Hunter_Player_Soldier::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 454269)
	{
		if (Get_Vehicle(obj))
			return;
		NewObjectiveSystemControl.Display_All_Objectives(obj);
	}
	if (message == 454270)
	{
		if (param == 0)
		{
			Commands->Give_PowerUp(obj,"POW_Twinbow",false);
			Commands->Give_PowerUp(obj,"POW_Twinbow_Explosive",false);
		}
		if (param == 1)
			Commands->Give_PowerUp(obj,"POW_Break_Action_Shotgun",false);
		if (param == 2)
			Commands->Give_PowerUp(obj,"POW_FAMAS_F1",false);
		if (param == 3)
			Commands->Give_PowerUp(obj,"POW_PPSh_41",false);
		if (param == 4)
		{
			Commands->Give_PowerUp(obj,"POW_AA12",false);
			Commands->Give_PowerUp(obj,"POW_AA12_HE",false);
		}
		if (param == 5)
			Commands->Give_PowerUp(obj,"POW_93R",false);
		if (param == 6)
			Commands->Give_PowerUp(obj,"POW_Plasma_Rifle",false);
	}
}
void JMG_Bear_Hunter_Player_Soldier::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		time_t CurrentTime = clock();
		double TimeDiff = difftime(CurrentTime,LastTime);
		LastTime = CurrentTime;
		Vector3 Pos = Commands->Get_Position(obj);
		if (TimeDiff)
			JMG_Bear_Hunter_Player_Soldier::speed[playerId] = Commands->Get_Distance(Pos,LastPosition) * (1000.0f/TimeDiff);
		LastPosition = Pos;
		Commands->Start_Timer(obj,this,0.1f,1);
	}
	if (number == 2)
		switch (JMG_Bear_Hunter_Game_Control::gameState)
		{
		case JMG_Bear_Hunter_Game_Control::LeaveBase:case JMG_Bear_Hunter_Game_Control::HuntBears:JmgUtility::DisplayChatMessage(obj,127,127,255,"San Casina Rug Co: Hello, you must be here for the money, as I'm sure you've heard, San Casina Rug Co. is in desperate need for bear rugs, and we are willing to pay 30 dollars a pelt.");return;
		case JMG_Bear_Hunter_Game_Control::MutationStart:JmgUtility::DisplayChatMessage(obj,127,127,255,"San Casina Rug Co: Hello, I'm sure you came down to make some money killing bears, got some good news for you: a tiberium meteorite hit, and mutated the wildlife, mutant bear pelts are far more valuable, head north and join in the fun!");return;
		case JMG_Bear_Hunter_Game_Control::FallBack:case JMG_Bear_Hunter_Game_Control::DefendPrez:JmgUtility::DisplayChatMessage(obj,127,127,255,"San Casina Rug Co: Welcome, its a bad time for you to have joined us, a tiberium meteorite hit, and now we're under attack by mutant wildlife, grab a gun and help us defend the President of Corporate America!");return;
		case JMG_Bear_Hunter_Game_Control::SecureBase:JmgUtility::DisplayChatMessage(obj,127,127,255,"San Casina Rug Co: Greetings, victory is at hand, all we need to do is kill the remaining mutants inside the base!");return;
		case JMG_Bear_Hunter_Game_Control::BossArrive:JmgUtility::DisplayChatMessage(obj,127,127,255,"San Casina Rug Co: Greetings, Kill the boss and this will be all over!");return;
		}
	if (number == 3)
		JmgUtility::DisplayChatMessage(obj,127,127,255,"San Casina Rug Co: Feel free to buy weapons at your nearest PCT, and once your ready, head north into the swamp and start making some money!");
	if (number == 4)
	{
		PlayerData.players[playerId]->hasGotIntro = true;
		JmgUtility::DisplayChatMessage(obj,127,127,255,"San Casina Rug Co: Oh, and as a tempory promotion, we are giving out an extra $50.00 every time someone kills 10 in a row without getting hurt! And we'll give out $1,000.00 to anyone that can kill 250!");
	}
	if (number == 5)
	{
		PlayerData.players[playerId]->firstJoin = false;
		Set_Money(playerId,0);
	}
	if (number == 6)
	{
		Vector3 speed = Vector3();
		if (obj->As_SoldierGameObj())
			obj->As_SoldierGameObj()->Get_Velocity(speed);
		if (JmgUtility::SimpleDistance(Vector3(),speed) > 0.0f)
		{
			Set_HUD_Help_Text_Player(obj,JMG_Bear_Hunter_Game_Control::objective,Vector3(0,1,0));
			NewObjectiveSystemControl.Display_All_Objectives(obj);
		}
		else
			Commands->Start_Timer(obj,this,1.0f,6);
	}
	if (number == 7)
	{
		Set_Screen_Fade_Color_Player(obj,0.0f,0.0f,0.0f,0.0f);
		Set_Screen_Fade_Opacity_Player(obj,0.15f,(100.0f-JMG_Bear_Hunter_Game_Control::bearTransition)*2.5f);
	}
	if (number == 8)
	{
		Vector3 pos = Commands->Get_Position(obj);
		MoveablePhysClass *mphys = obj->As_PhysicalGameObj() ? obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
		if (mphys && !mphys->Can_Teleport(Matrix3D(pos)))
		{
			mphys->Find_Teleport_Location(pos,2.5f,&pos);
			Commands->Set_Position(obj,pos);
		}
	}
}
void JMG_Bear_Hunter_Player_Soldier::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage > 0)
		PlayerData.players[playerId]->killsWithoutGettingHurt = 0;
}
void JMG_Bear_Hunter_Player_Soldier::Killed(GameObject *obj,GameObject *killer)
{
	BearHunterScoreSystem::BHScoreNode *scoreNode = bearHunterScoreSystem.Get_Current_Player_Score_Node(JmgUtility::JMG_Get_Player_ID(obj));
	if (PlayerData.players[playerId])
	{
		PlayerData.players[playerId]->deaths++;
		scoreNode->Deaths++;
	}
	if (playerId && !JMG_Bear_Hunter_Game_Control::remainingLives[playerId])
		JMG_Bear_Hunter_Game_Control::playerAlive[playerId] = false;
	char deathMsg[500];
	unsigned int killerWarheadId = Get_Damage_Warhead();
	if (!killer)
		if (JMG_Bear_Hunter_Game_Control::deathWarheadId == killerWarheadId)
		{
			sprintf(deathMsg,"%s realized %s could not swim",JmgUtility::Get_The_Units_Name2(obj),JmgUtility::JMG_Get_Units_Sex3(obj));
			scoreNode->TimesDrown++;
		}
		else if (JMG_Bear_Hunter_Game_Control::earthWarheadId == killerWarheadId)
		{
			sprintf(deathMsg,"%s fell to %s death",JmgUtility::Get_The_Units_Name2(obj),JmgUtility::JMG_Get_Units_Sex2(obj));
			scoreNode->TimesFallen++;
		}
		else
			sprintf(deathMsg,"%s died",JmgUtility::Get_The_Units_Name(obj));
	else if (killer == obj)
		sprintf(deathMsg,"%s killed %s",JmgUtility::Get_The_Units_Name(obj),JmgUtility::JMG_Get_Units_Sex(obj));
	else
		sprintf(deathMsg,"%s killed %s",JmgUtility::Get_The_Units_Name(killer),JmgUtility::Get_The_Units_Name2(obj));
	if (Commands->Is_A_Star(obj) && !PlayerData.players[playerId]->displayedKillMessage)
		JmgUtility::MessageAllPlayers(255,255,0,deathMsg);
	PlayerData.players[playerId]->displayedKillMessage = true;
	if (killer && Is_Script_Attached(killer,"JMG_Bear_Hunter_Animal_Control"))
		Set_Team_Score(0,Get_Team_Score(0)+100.0f);
	if (!hasGrenadeVest[playerId])
		return;
	hasGrenadeVest[playerId] = false;
	Vector3 pos = Commands->Get_Position(obj);
	int remoteMines = Get_Total_Bullets(obj,"CnC_Weapon_MineRemote_Player")+Get_Total_Bullets(obj,"CnC_Weapon_MineRemote_Player_2Max"),proxiMines = Get_Total_Bullets(obj,"Weapon_MineProximity_Player"),timedMines = Get_Total_Bullets(obj,"CnC_Weapon_MineTimed_Player_2Max")+Get_Total_Bullets(obj,"CnC_Weapon_MineTimed_Player");
	float distance = 0;
	remoteMines *= 2;proxiMines *= 2;timedMines *= 2;
	while (remoteMines || proxiMines || timedMines)
	{
		CreateExplosion(obj,"Remote Mine Explosion Twiddler",pos,&distance,&remoteMines);
		CreateExplosion(obj,"Proximity Mine Explosion Twiddler",pos,&distance,&proxiMines);
		CreateExplosion(obj,"Timed Mine Explosion Twiddler",pos,&distance,&timedMines);
	}
}
void JMG_Bear_Hunter_Player_Soldier::Destroyed(GameObject *obj)
{
	if (!PlayerData.players[playerId])
		return;
	PlayerData.players[playerId]->destroyTime = clock();
	JMG_Bear_Hunter_Player_Soldier::speed[playerId] = 0;
}
void JMG_Bear_Hunter_Player_Soldier::CreateExplosion(GameObject *obj,const char *explosion,Vector3 pos,float *distance,int *explosives)
{
	if (!*explosives)
		return;
	*explosives = *explosives-1;
	*distance = *distance + 0.25f;
	float range = Commands->Get_Random(0.0f,*distance);
	float Rotation = Commands->Get_Random(-180.0f,180.0f);
	pos.X += cos(Rotation*PI180)*range;
	pos.Y += sin(Rotation*PI180)*range;
	pos.Z += Commands->Get_Random(-1.0,1.0)*range;
	Commands->Create_Explosion(explosion,pos,obj);
}
bool JMG_Bear_Hunter_Player_Soldier::LifeSystem(GameObject *obj)
{
	if (!_stricmp(Commands->Get_Preset_Name(obj),"CNC_Spectator"))
		return true;
	if (JMG_Bear_Hunter_Game_Control::remainingLives[playerId] > -2)
		JMG_Bear_Hunter_Game_Control::remainingLives[playerId]--;
	if (JMG_Bear_Hunter_Game_Control::remainingLives[playerId] == -1)
	{
		Change_Character(obj,"CNC_Spectator");
		char firstMsg[220];
		sprintf(firstMsg,"pamsg %d You can't spawn, its up to the remaining players to win!",playerId);
		Console_Input(firstMsg);
		return true;
	}
	if (JMG_Bear_Hunter_Game_Control::remainingLives[playerId] == -2)
	{
		Change_Character(obj,"CNC_Spectator");
		return true;
	}
	char livesMsg[220];
	sprintf(livesMsg,"You have %d %s remaining.",JMG_Bear_Hunter_Game_Control::remainingLives[playerId],JMG_Bear_Hunter_Game_Control::remainingLives[playerId] != 1 ? "lifes" : "life");
	if (JMG_Bear_Hunter_Game_Control::remainingLives[playerId])
		JmgUtility::DisplayChatMessage(obj,11,194,255,livesMsg);
	else
		JmgUtility::DisplayChatMessage(obj,11,194,255,"You are on your last life.");
	return false;
}
void JMG_Bear_Hunter_Player_Soldier::GrantUnlockedWeapons(GameObject *obj)
{
	switch (Commands->Get_Preset_ID(obj))
	{
	case 1000000910:case 1000001140:case 1000000948:case 1000000902:case 1000000930:
		if (!JMG_Bear_Hunter_Game_Control::weaponsUnlocked[1])
			return;
		Commands->Give_PowerUp(obj,"POW_Break_Action_Shotgun",false);
		break;
	case 1000000912:case 1000000940:case 1000000960:case 1000000920:case 1000000928:case 1000000932:
		if (!JMG_Bear_Hunter_Game_Control::weaponsUnlocked[2])
			return;
		Commands->Give_PowerUp(obj,"POW_FAMAS_F1",false);
		break;
	case 1000000942:case 1000000914:case 1000000922:case 1000000934:case 1000000952:case 1000000904:
		if (!JMG_Bear_Hunter_Game_Control::weaponsUnlocked[3])
			return;
		Commands->Give_PowerUp(obj,"POW_PPSh_41",false);
		break;
	case 1000000916:case 1000000918:case 1000000944:case 1000001046:case 1000000954:case 1000000956:case 1000000924:case 1000000926:case 1000000906:case 1000000908:case 1000000936:case 1000000938:
		if (!JMG_Bear_Hunter_Game_Control::weaponsUnlocked[4])
			return;
		Commands->Give_PowerUp(obj,"POW_AA12",false);
		Commands->Give_PowerUp(obj,"POW_AA12_HE",false);
		break;
	case 1000000958:case 1000000900:
		if (!JMG_Bear_Hunter_Game_Control::weaponsUnlocked[5])
			return;
		Commands->Give_PowerUp(obj,"POW_93R",false);
		break;
	}
}
void JMG_Rp2_Dedicated_Server_Sound_Emulator::Created(GameObject *obj)
{
	weaponSoundRange = 40000.0f;
	vehicleId = 0;
	vehicleRounds = -1;
	SoundOverrideTime = 0;
	PlayerID = JmgUtility::JMG_Get_Player_ID(obj);
	LastWeaponID = JmgUtility::JMG_Get_Current_Weapon_ID(obj);
	LastBullets = Get_Current_Total_Bullets(obj);
	PlayerEmulatedSoundList[PlayerID] = PlayerEmulatedSound();
	Commands->Start_Timer(obj,this,0.25f,1); 
}
void JMG_Rp2_Dedicated_Server_Sound_Emulator::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == EmulatedSoundCustom)
	{
		SoundOverrideTime = param;
		PlayerEmulatedSoundList[PlayerID].SetPlayerEmulatedSound(360000,PlayerEmulatedSound::stNoticeMe);
	}
}
void JMG_Rp2_Dedicated_Server_Sound_Emulator::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		int WeaponID = JmgUtility::JMG_Get_Current_Weapon_ID(obj);
		int CurrentBullets = Get_Current_Total_Bullets(obj);
		GameObject *vehicle = Get_Vehicle(obj);
		int currentVehicleRounds = vehicleRounds;
		int currentVehicleId = vehicleId;
		if (vehicle)
		{
			currentVehicleRounds = Get_Current_Bullets(vehicle);
			currentVehicleId = Commands->Get_ID(vehicle);
		}
		if (SoundOverrideTime)
			SoundOverrideTime--;
		else if (currentVehicleId == vehicleId && currentVehicleRounds != vehicleRounds)
		{
			PlayerEmulatedSoundList[PlayerID].SetPlayerEmulatedSound(40000,PlayerEmulatedSound::stGunshot);
			SoundOverrideTime = 5;
		}
		else if (WeaponID == LastWeaponID && CurrentBullets < LastBullets)
		{
			PlayerEmulatedSoundList[PlayerID].SetPlayerEmulatedSound(weaponSoundRange,PlayerEmulatedSound::stGunshot);
			SoundOverrideTime = 5;
		}
		else if (JMG_Bear_Hunter_Player_Soldier::speed[PlayerID] > 15)
			PlayerEmulatedSoundList[PlayerID].SetPlayerEmulatedSound(0.0f,PlayerEmulatedSound::stNone);
		else if (JMG_Bear_Hunter_Player_Soldier::speed[PlayerID] > 5)
			PlayerEmulatedSoundList[PlayerID].SetPlayerEmulatedSound(900.0f,PlayerEmulatedSound::stFootsteps);
		else if (JMG_Bear_Hunter_Player_Soldier::speed[PlayerID] > 0)
			PlayerEmulatedSoundList[PlayerID].SetPlayerEmulatedSound(100.0f,PlayerEmulatedSound::stFootsteps);
		else
			PlayerEmulatedSoundList[PlayerID].SetPlayerEmulatedSound(0.0f,PlayerEmulatedSound::stNone);
		if (WeaponID != LastWeaponID)
		{
			LastWeaponID = WeaponID;
			switch (WeaponID)
			{
			case 409610052:case 409610005:case 409610127:case 1000001422: weaponSoundRange = 0.0f;
				case 1393:case 3078:case 409610116:case 409610117:case 409610114:case 409610115:case 1000001128: weaponSoundRange = 2500.0f;break;
				case 1000001082:case 409610007:case 409610006:case 1000001394:case 1000001423: weaponSoundRange = 10000.0f;break;
				case 1000001073:case 2264:case 1000001399: weaponSoundRange = 22500.0f;break;
				case 409610124:case 409610125: weaponSoundRange = 160000.0f;break;
				case 409610123: weaponSoundRange = 250000.0f;break;
				default: weaponSoundRange = 40000.0f;break;
			}
		}
		LastBullets = CurrentBullets;
		if (currentVehicleId != vehicleId)
			vehicleId = currentVehicleId;
		vehicleRounds = currentVehicleRounds;
		Commands->Start_Timer(obj,this,0.1f,1); 
	}
}
void JMG_Rp2_Dedicated_Server_Sound_Emulator::Killed(GameObject *obj,GameObject *killer)
{
	PlayerEmulatedSoundList[PlayerID].SetPlayerEmulatedSound(0.0f,PlayerEmulatedSound::stNone);
	if (Is_Script_Attached(obj,"JMG_Rp2_Dedicated_Server_Sound_Emulator"))
		Remove_Script(obj,"JMG_Rp2_Dedicated_Server_Sound_Emulator");
}
void JMG_Rp2_Dedicated_Server_Sound_Emulator::Destroyed(GameObject *obj)
{
	PlayerEmulatedSoundList[PlayerID].SetPlayerEmulatedSound(0.0f,PlayerEmulatedSound::stNone);
}
JMG_Bear_Hunter_Game_Control::JMG_Bear_Hunter_Game_Control()
{
	cleanUpNeeded = false;
	for (int x = 0;x < 2;x++)
		easTowers[x] = EASTowerNode::EASTowerNode();
	myObject = NULL;
	PlayerData = TempPlayerData();
	bearHunterScoreSystem = BearHunterScoreSystem();
	AIPurchaseSystemControl = AIPurchaseSystem();
}
void JMG_Bear_Hunter_Game_Control::Created(GameObject *obj)
{
	hasBeenInjured = false;
	spawnKarma = 0;
	smoothFade = false;
	truckTimeExtended = false;
	cleanUpNeeded = true;
	Set_Nod_Soldier_Name("CNC_Spawned_Soldier");
	Set_GDI_Soldier_Name("CNC_Spawned_Soldier");
	bearHunterScoreSystem.LoadData();
	alarmSwitchActive = false;
	hasActivatedTower = false;
	JMG_Bear_Hunter_Oil_Rig::oilRigCount = 0;
	turretsDestroyed = 0;
	earthWarheadId = ArmorWarheadManager::Get_Warhead_Type("Earth");
	deathWarheadId = ArmorWarheadManager::Get_Warhead_Type("Death");
	JMG_Bear_Hunter_Player_Assist_AI::maxWait = -1;
	for (int x = 0;x < 128;x++)
	{
		aiIgnorePlayers[x] = 0;
		remainingLives[x] = 0;
		playerAlive[x] = true;
	}
	bossRabitDead = 0;
	bearTransition = 0;
	objective = 12693;
	NewObjectiveSystemControl.Add_Objective(1,NewObjectiveSystem::Primary,NewObjectiveSystem::Pending,12693,"",12693,Vector3(15.593f,-93.453f,-0.166f));
	spawnGroup = 0;
	atCabin = false;
	winWait = 0;
	gameOver = false;
	hasGotTurrets = false;
	maxPlayerCount = 0;
	pctObjectArray[0] = PCTDataObject(650179,"PCT_DERP_IND.PCT_DERP_IND");
	pctObjectArray[1] = PCTDataObject(650255,"PCT_DERP_IND.PCT_DERP_IND");
	pctObjectArray[2] = PCTDataObject(650171,"PCT_INDUSTRIAL1.PCT_INDUSTRIAL1");
	pctObjectArray[3] = PCTDataObject(650175,"PCT_INDUSTRIAL1.PCT_INDUSTRIAL1");
	pctObjectArray[4] = PCTDataObject(650176,"PCT_INDUSTRIAL1.PCT_INDUSTRIAL1");
	pctObjectArray[5] = PCTDataObject(650177,"PCT_INDUSTRIAL1.PCT_INDUSTRIAL1");
	pctObjectArray[6] = PCTDataObject(650178,"PCT_INDUSTRIAL1.PCT_INDUSTRIAL1");
	pctObjectArray[7] = PCTDataObject(650266,"PCT_INDUSTRIAL1.PCT_INDUSTRIAL1");
	pctObjectArray[8] = PCTDataObject(650268,"PCT_INDUSTRIAL1.PCT_INDUSTRIAL1");
	pctObjectArray[9] = PCTDataObject(650269,"PCT_INDUSTRIAL1.PCT_INDUSTRIAL1");
	pctObjectArray[10] = PCTDataObject(650197,"PCT_SHOUSE.PCT_SHOUSE");
	pctObjectArray[11] = PCTDataObject(650256,"PCT_WALLS.PCT_WALLS");
	pctObjectArray[12] = PCTDataObject(650259,"PCT_WALLS.PCT_WALLS");
	pctObjectArray[13] = PCTDataObject(650260,"PCT_WALLS.PCT_WALLS");
	pctObjectArray[14] = PCTDataObject(650272,"PCT_WALLS.PCT_WALLS");
	pctObjectArray[15] = PCTDataObject(650162,"PCT_WAREHOUSE.PCT_WAREHOUSE");
	pctObjectArray[16] = PCTDataObject(650317,"PCT_GMAXIND.PCT_GMAXIND");
	pctObjectArray[17] = PCTDataObject(650318,"PCT_GMAXIND.PCT_GMAXIND");
	pctObjectArray[18] = PCTDataObject(650478,"PCT_WALLS.PCT_WALLS");
	pctObjectArray[19] = PCTDataObject(650487,"PCT_COMTOWER.PCT_COMTOWER");
	starGold = false;
	gameState = LeaveBase;
	gameTime = 0;
	maxBears = 100;
	musicOn = false;
	meteoriteTime = 0;
	myObject = obj;
	BearHunterGameControlSystem.Load(Get_Parameter("PositionFile"));
	JMG_Bear_Hunter_Game_Control::mutantBears = 0;
	JMG_Bear_Hunter_Engineer_AI::engineersDead = 0;
	JMG_Bear_Hunter_Engineer_AI::scanAgainForEngineerDeaths = 0;
	lastUpdateEngineersDead = 1;
	for (int x = 0;x < 5;x++)
	{
		JMG_Bear_Hunter_Engineer_AI::engineerIds[x] = 0;
		JMG_Bear_Hunter_Engineer_AI::engineerMode[x] = false;
		engineerSpawnedOnce[x] = false;
	}
	engineerScanTime = 0;
	for (int x = 0;x < 6;x++)
	{
		weaponsFound[x] = false;
		weaponsReturned[x] = false;
		weaponsUnlocked[x] = false;
	}
	weaponsUnlocked[6] = false;
	blackBearId = 0;
	int pumpJackIds[7] = {601041,601038,601044,601040,601045,601039,601043};
	for (int x = 0;x < 7;x++)
	{
		GameObject *pumpJack = Commands->Find_Object(pumpJackIds[x]);
		if (pumpJack)
			Commands->Set_Health(pumpJack,((x+1)%4+2)*5.0f);
	}
	playersDead = false;
	Commands->Start_Timer(obj,this,1.0f,2);
	Commands->Start_Timer(obj,this,1.0f,1);
	Commands->Start_Timer(obj,this,30.0,7);
	Commands->Start_Timer(obj,this,10.0,9);
	Commands->Start_Timer(obj,this,10.0,10);
	Commands->Start_Timer(obj,this,5.0,11);
	Commands->Start_Timer(obj,this,5.0,12);
	Commands->Start_Timer(obj,this,60.0f,13);
	Commands->Start_Timer(obj,this,0.6f,14);
	Commands->Attach_Script(obj,"JMG_Bear_Hunter_Wolf_Pack_Definition","1,Wild_Wolf,5,218.230 180.027 0.022,10.0,120,30,60.0,15.0,0.5,0");
	Commands->Attach_Script(obj,"JMG_Bear_Hunter_Wolf_Pack_Definition","2,Wild_Wolf,10,420.0 -1.14 0.59,50.0,240,30,60.0,15.0,1.5,1");
	Commands->Attach_Script(obj,"JMG_Bear_Hunter_Wolf_Pack_Definition","3,Wild_Wolf,3,-400.0 230.0 -0.11,50.0,120,30,60.0,15.0,1.5,1");
	Commands->Attach_Script(obj,"JMG_Bear_Hunter_Wolf_Pack_Controller","");
	Commands->Attach_Script(obj,"JMG_Utility_Dynamic_Clock_Control","");
}
void JMG_Bear_Hunter_Game_Control::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		for (int x = 0;x < 128;x++)
			if (aiIgnorePlayers[x] > 0)
				aiIgnorePlayers[x]--;
		bearHunterScoreSystem.IncreasePlayerTimeScores();
		if (The_Game()->Is_Game_Over())
		{
			if (cleanUpNeeded)
				EndGameDataCleanup(obj,true);
			else
				Console_Input("msg JMG_Bear_Hunter_Game_Control ERROR: Could not save, cleanup already called once this round!");
			return;
		}
		else if (JMG_Bear_Hunter_Game_Control::gameState != JMG_Bear_Hunter_Game_Control::BossArrive)
		{
			int aiCount = JmgUtility::MathClampInt(15-Get_Player_Count(),0,maxTotalBearHunterPlayerAssistAI);
			for (int x = 0;x < aiCount;x++)
			{
				GameObject *ai = Commands->Find_Object(JMG_Bear_Hunter_Player_Assist_AI::aiIds[x]);
				if (!ai)
				{
					Vector3 pos = Vector3(-98.27f,-824.0f,0.51f);
					ai = Commands->Create_Object("CnC_GDI_Minigunner",pos);
					char params[220];
					sprintf(params,"%d",x);
					Commands->Attach_Script(ai,"JMG_Bear_Hunter_Player_Assist_AI",params);
					JMG_Bear_Hunter_Player_Assist_AI::aiIds[x] = Commands->Get_ID(ai);
				}
				else if (Commands->Get_Health(ai) && !Is_Script_Attached(ai,"JMG_Bear_Hunter_Player_Assist_AI"))
				{
					char params[220];
					sprintf(params,"%d",x);
					Commands->Attach_Script(ai,"JMG_Bear_Hunter_Player_Assist_AI",params);
				}
			}
		}
		if (!JMG_Bear_Hunter_Game_Control::gameOver && Get_Team_Player_Count(1))
			for (int x = 1;x < 128;x++)
			{
				GameObject *player = Get_GameObj(x);
				if (!player)
					continue;
				Change_Team_2(player,1);
			}
		if (JMG_Bear_Hunter_Game_Control::gameState == JMG_Bear_Hunter_Game_Control::BossArrive && !Get_Player_Count())
		{
			bearHunterScoreSystem.EndGameUpdatePlayerStats(false);
			Console_Input("win 0");
			for (int x = 0;x < 5;x++)
				Commands->Create_Sound("Mission_Failed",Commands->Get_Position(obj),obj);
		}
		if (JMG_Bear_Hunter_Game_Control::gameState == JMG_Bear_Hunter_Game_Control::BossArrive && !playersDead)
		{
			bool playersAlive = false;
			for (int x = 0;x < 128;x++)
			{
				GameObject *player = Get_GameObj(x);
				if (!player || Get_Player_Type(player) == -4)
					continue;
				if (!playerAlive[x])
					continue;
				playersAlive = true;
			}
			if (!playersAlive)
				playersDead = true;
		}
		if (bossRabitDead)
		{
			bossRabitDead--;
			if (!bossRabitDead)
			{
				JMG_Bear_Hunter_Game_Control::gameOver = true;
				for (int x = 1;x < 128;x++)
				{
					GameObject *player = Get_GameObj(x);
					if (!player)
						continue;
					Change_Team_2(player,1);
				}
				Console_Input("win 1");
				bearHunterScoreSystem.EndGameUpdatePlayerStats(true);
				for (int x = 0;x < 5;x++)
					Commands->Create_Sound("Mission_Accomplished",Commands->Get_Position(obj),obj);
			}
		}
		int playerCount = Get_Player_Count();
		if (playerCount > maxPlayerCount)
		{
			GameObject *mainObject = Commands->Find_Object(JMG_Bear_Hunter_Game_Control::mutantTargetId);
			if (mainObject)
			{
				float originalMaxHealth = Commands->Get_Max_Health(mainObject);
				float newMaxHealth = 500.0f-playerCount*3.5f;
				Set_Max_Health(obj,newMaxHealth);
				float newHealth = Commands->Get_Health(obj)*(newMaxHealth/originalMaxHealth);
				if (newHealth < 1.0f)
					Commands->Set_Health(obj,1.0f);
				else
					Commands->Set_Health(obj,newHealth);
			}
			maxPlayerCount = playerCount;
		}
		if (gameState < FallBack)
		{
			bool playerInRange = false;
			for (int x = 0;x < maxTotalBearHunterPlayerAssistAI && !playerInRange;x++)
			{
				GameObject *aiPlayer = Commands->Find_Object(JMG_Bear_Hunter_Player_Assist_AI::aiIds[x]);
				if (!aiPlayer)
					continue;
				if (JmgUtility::SimpleFlatDistance(Commands->Get_Position(aiPlayer),Vector3(16.235f,0.0f,0.0f)) < 22500)
					playerInRange = true;
			}
			for (int x = 1;x < 128 && !playerInRange;x++)
			{
				GameObject *player = Get_GameObj(x);
				if (!player || !Commands->Get_Health(player) || Get_Player_Type(player) == -4)
					continue;
				if (JmgUtility::SimpleFlatDistance(Commands->Get_Position(player),Vector3(16.235f,0.0f,0.0f)) < 22500)
					playerInRange = true;
			}
			if (atCabin != playerInRange)
			{
				atCabin = playerInRange;
				spawnGroup = playerInRange;
				if (atCabin)
				{
					if (gameState < MutationStart && !alarmSwitchActive)
					{
						sprintf(currentSong,"03-ammoclip.mp3");
						Commands->Fade_Background_Music("03-ammoclip.mp3",10000,5000);
					}
					NewObjectiveSystemControl.Set_Objective_Mission(11,12695,12695);
					Commands->Set_Objective_HUD_Info_Position(3,2,"POGCabin.tga",7387,Vector3(15.593f,-93.453f,-0.166f));
					if (NewObjectiveSystemControl.Get_Objective_Status(13) == NewObjectiveSystem::NotDefined)
					{
						JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: There is an alert system placed at the cabin, activating it would defintly attact attention of the bears.");
						NewObjectiveSystemControl.Add_Objective(13,NewObjectiveSystem::Secondary,NewObjectiveSystem::Pending,12739,"",12739);
					}
				} 
				if (!atCabin)
				{
					if (gameState < MutationStart && !alarmSwitchActive)
					{
						sprintf(currentSong,"SpecForce_Sneak_02.mp3");//04-ambient industrial
						Commands->Fade_Background_Music("SpecForce_Sneak_02.mp3",5000,5000);
					}
					NewObjectiveSystemControl.Set_Objective_Mission(11,12729,12729);
				}
			}
		}
		if (JMG_Bear_Hunter_Player_Assist_AI::maxWait > 0)
			JMG_Bear_Hunter_Player_Assist_AI::maxWait--;
		for (int x = 0;x < 6;x++)
			randomSelectableSpawnPoints[x].DecreaseValue();
		for (int x = 0;x < 2;x++)
			if (easTowers[x].activeTime)
			{
				GameObject *switchObj = Commands->Find_Object(easTowers[x].id);
				if (switchObj)
				{
					Vector3 switchPos = Commands->Get_Position(switchObj);
					bool inRange = false;
					for (int j = 1;j < 128;j++)
					{
						GameObject *player = Get_GameObj(j);
						if (!player || Get_Player_Type(player) == -4)
							continue;
						if (JmgUtility::SimpleDistance(switchPos,Commands->Get_Position(player)) < 22500)
						{
							inRange = true;
							Commands->Send_Custom_Event(obj,player,EmulatedSoundCustom,15,0);
						}
					}
					if (!inRange && easTowers[x].activeTime == -1)
					{
						smoothFade = true;
						Commands->Send_Custom_Event(obj,switchObj,4511911,0,0);
					}
				}
		}
		if (gameState < MutationStart)
		{
			if (easTowers[0].activeTime)
			{
				if (!alarmSwitchActive)
				{
					alarmSwitchActive = true;	
					sprintf(currentSong,"SpecForce_Moments01.mp3");
					Commands->Fade_Background_Music("SpecForce_Moments01.mp3",5000,5000);
				}
			}
			else if (!easTowers[0].activeTime && alarmSwitchActive)
			{
				alarmSwitchActive = false;
				sprintf(currentSong,atCabin ? "03-ammoclip.mp3" : "SpecForce_Sneak_02.mp3");
				if (smoothFade)
				{
					smoothFade = false;
					Commands->Fade_Background_Music(atCabin ? "03-ammoclip.mp3" : "SpecForce_Sneak_02.mp3",5000,5000);
				}
				else
				{
					Commands->Set_Background_Music(atCabin ? "03-ammoclip.mp3" : "SpecForce_Sneak_02.mp3");
					GameObject *switchObj = Commands->Find_Object(easTowers[0].id);
					for (int x = 0;x < 128;x++)
					{
						GameObject *player = Get_GameObj(x);
						if (!player || Get_Player_Type(player) == -4 || JmgUtility::SimpleDistance(Commands->Get_Position(switchObj),Commands->Get_Position(player)) >= 22500)
							continue;
						JmgUtility::DisplayChatMessage(player,127,127,255,"Bear Rug Co: Great work holding those bears off, here is a small bonus!");
						Commands->Give_Money(player,250.0f,0);
						BearHunterScoreSystem::BHScoreNode *node = bearHunterScoreSystem.Get_Current_Player_Score_Node(x);
						if (node)
							node->SurvivedAlarm++;
					}
					for (int x = 0;x < maxTotalBearHunterPlayerAssistAI;x++)
						JMG_Bear_Hunter_Player_Assist_AI::cash[x] += 1000;
				}
			}
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
	if (number == 2)
	{
		Vector3 centerOfBase = Vector3(-0.532f,-705.389f,0.0f);
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player || Get_Player_Type(player) == -4)
				continue;
			if (JmgUtility::SimpleFlatDistance(Commands->Get_Position(player),centerOfBase) > 34225.0f)
			{
				gameState = HuntBears;
				bearHunterScoreSystem.GameStarted();
				Commands->Start_Timer(obj,this,1.0f,3);
				gameTime = 1800;
				JMG_Bear_Hunter_Player_Assist_AI::maxWait = 0;
				NewObjectiveSystemControl.Set_Objective_Status(1,NewObjectiveSystem::Accomplished);
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: We need more rugs, get out there and kill us some bears!");
				NewObjectiveSystemControl.Add_Objective(2,NewObjectiveSystem::Primary,NewObjectiveSystem::Pending,12694,"",12694);
				sprintf(currentSong,"SpecForce_Sneak_02.mp3");
				Commands->Fade_Background_Music("SpecForce_Sneak_02.mp3",10000,10000);
				objective = 12694;
				JmgUtility::SetHUDHelpText(objective,Vector3(0,1,0));
				Commands->Set_Objective_HUD_Info_Position(2,1,"PogBear.tga",12700,Vector3(15.593f,-93.453f,-0.166f));
				Commands->Start_Timer(obj,this,0.1f,16);
				return;
			}
		}
		Commands->Start_Timer(obj,this,1.0f,2);
	}
	if (number == 3)
	{
		if (gameTime)
		{
			gameTime--;
			if (JMG_Bear_Hunter_Power_Transformer::online && JMG_Bear_Hunter_Game_Control::turretsDestroyed < 4 && gameTime > 60)
				attackSubstationChance = (gameTime/300.0f)*JmgUtility::MathClamp(Get_Player_Count()*Get_Player_Count()/100.00f,0.01f,1.0f);
			else
				attackSubstationChance = 0.0f;
			if (gameTime == 1780)
			{
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: There is a little cabin up north with some supplies, if you head up there you'll have easy hunting.");
				NewObjectiveSystemControl.Add_Objective(11,NewObjectiveSystem::Secondary,NewObjectiveSystem::Pending,12729,"",12729,Vector3(15.593f,-93.453f,-0.166f));
			}
			if (gameTime == 1770)
			{
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Oh also, we'll pay everyone big bucks if you can deliver the turret supplies from the warehouse to the North East.");
				NewObjectiveSystemControl.Add_Objective(9,NewObjectiveSystem::Tertiary,NewObjectiveSystem::Pending,12727,"",12727,Vector3(434.101f,539.071f,1.142f));
			}
			if (gameTime == 1760)
			{
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: One last thing, we had a helicopter go down in the swamp to the North West there are 5 surviving engineers that could use an escort back to base; we will of course pay for their safe return.");
				NewObjectiveSystemControl.Add_Objective(15,NewObjectiveSystem::Bonus,NewObjectiveSystem::Pending,12741,"",12741,3);
			}
			if (gameTime == 1730)
			{
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: 7 of our oil rigs to the north have gone off-line, if you head up there and activate them we'll give you a cut of the profits.");
				NewObjectiveSystemControl.Add_Objective(16,NewObjectiveSystem::Bonus,NewObjectiveSystem::Pending,12758,"",12758,4);
			}
			if (gameTime == 1700)
			{
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: 3 other Pump Jacks have fallen into disrepair in the East, repair them for additional profit.");
				NewObjectiveSystemControl.Add_Objective(17,NewObjectiveSystem::Bonus,NewObjectiveSystem::Pending,12748,"",12748,5);
			}
			if (gameTime == 1500)
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Half way there, you guys better get those bears while you can, because they won't be worth much when we hit our quota!");
			if (gameTime == 1196)
			{
				gameState = MutationStart;
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Wow! Was that a Tiberium meteorite that just hit?");
				BearHunterWolfHivemindSystem::BearHunterWolfHivemindNode *node = bearHunterWolfHivemindControl.find(2);
				if (node)
					node->maxPackSize = 5;
				Commands->Start_Timer(obj,this,2.5f,6);
				sprintf(currentSong,"14 Tiberian Sun - Storm Coming.mp3");
				Commands->Fade_Background_Music("14 Tiberian Sun - Storm Coming.mp3",10000,10000);
				Commands->Set_Rain(0.5,250.0f,true);
				Commands->Set_Clouds(1.0f,1.0f,250.0f);
				Commands->Set_Lightning(0.15f,0.6f,1.0f,0.0f,1.0f,250.0f);
				Commands->Set_Fog_Range(10.0f,150.0f,250.0f);
				for (int x = 1;x < 128;x++)
				{
					GameObject *player = Get_GameObj(x);
					if (!player)
						continue;
					Set_Screen_Fade_Color_Player(player,0.0f,0.0f,0.0f,0.0f);
					Set_Screen_Fade_Opacity_Player(player,0.15f,250.0f);
				}
				GameObject *blackbear = Commands->Find_Object(blackBearId);
				if (blackbear)
					Commands->Destroy_Object(blackbear);
				if (NewObjectiveSystemControl.Get_Mission_Text_Id(20) == 12786)
					NewObjectiveSystemControl.Remove_Objective(20);
			}
			if (gameTime == 1190)
			{
				int livingPumpjacks = 0,inactivePumpjackCount = 0;
				int pumpJackIds[7] = {601041,601038,601044,601040,601045,601039,601043};
				for (int x = 0;x < 7;x++)
				{
					GameObject *pumpJack = Commands->Find_Object(pumpJackIds[x]);
					if (pumpJack && Commands->Get_Health(pumpJack))
					{
						livingPumpjacks++;
						for (int y = 0;y < 7;y++)
							if (JMG_Bear_Hunter_Oil_Rig::pumpJacks[y].pumpId == pumpJackIds[x] && !JMG_Bear_Hunter_Oil_Rig::pumpJacks[y].active)
								inactivePumpjackCount++;
					}
				}
				if (NewObjectiveSystemControl.Get_Objective_Status(16) == NewObjectiveSystem::Pending)
				{
					JMG_Bear_Hunter_Oil_Rig::oilRigCount = 7-inactivePumpjackCount;
					if (livingPumpjacks != 7 && inactivePumpjackCount)
					{
						char successMsg[220];
						sprintf(successMsg,"Bear Rug Co: There are %d inactive pumpjacks to the North that survied that meteor impact, hurry up and activate them.",inactivePumpjackCount);
						JmgUtility::MessageAllPlayers(127,127,255,successMsg);
						unsigned int textId = (7-inactivePumpjackCount == 1) ? 12752 : (7-inactivePumpjackCount == 2) ? 12753 : (7-inactivePumpjackCount == 3) ? 12754 : (7-inactivePumpjackCount == 4) ? 12755 : 12756;
						NewObjectiveSystemControl.Set_Objective_Mission(16,textId,textId);
					}
					if (!livingPumpjacks)
					{
						if (!inactivePumpjackCount)
							JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: All of the pump jacks to the North were destroyed by that meteor, guess we should have repaired them!");
						else
							JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Looks like that meteor impact also wiped out all the pumpjacks to the North!");
						NewObjectiveSystemControl.Set_Objective_Status(16,NewObjectiveSystem::Failed);
					}
				}
			}
			if (gameTime == 1180)
			{
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Looks like it was, and mutation has already spread to the local bears, I bet those pelts would be worth way more than a normal bears'... You should head north, there is a little house with supplies.");
				objective = 12696;
				JmgUtility::SetHUDHelpText(objective,Vector3(0,1,0));
				NewObjectiveSystemControl.Set_Objective_Status(2,NewObjectiveSystem::Accomplished);
				NewObjectiveSystemControl.Add_Objective(4,NewObjectiveSystem::Primary,NewObjectiveSystem::Pending,12696,"",12696,Vector3(15.593f,-93.453f,-0.166f));
				Commands->Set_Objective_HUD_Info_Position(4,1,"PogMutantBear.tga",12700,Vector3(15.593f,-93.453f,-0.166f));
			}
			if (gameTime == 900)
			{
				BearHunterWolfHivemindSystem::BearHunterWolfHivemindNode *node = bearHunterWolfHivemindControl.find(1);
				if (node)
					node->maxPackSize = 0;
				node = bearHunterWolfHivemindControl.find(2);
				if (node)
					node->maxPackSize = 0;
				node = bearHunterWolfHivemindControl.find(3);
				if (node)
					node->maxPackSize = 0;
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Local deer have begun to show effects of mutation, we're locking the base down for the President of Corporate America's safety!");
				Commands->Static_Anim_Phys_Goto_Last_Frame(650184,"BH_GATE_1.BH_GATE_1");
				Commands->Static_Anim_Phys_Goto_Last_Frame(650183,"BH_GATE_1.BH_GATE_1");
				Commands->Static_Anim_Phys_Goto_Last_Frame(650190,"BH_GATE_1.BH_GATE_1");
				Commands->Static_Anim_Phys_Goto_Last_Frame(650189,"BH_GATE_1.BH_GATE_1");
			}
			if (gameTime == 875)
			{
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Shit! Looks like there has been a failure in the gate control mainframe, fall back and defend the President of Corporate America!");
				sprintf(currentSong,"SpecForce_Moments02.mp3");
				Commands->Fade_Background_Music("SpecForce_Moments02.mp3",10000,10000);
				objective = 12697;
				JmgUtility::SetHUDHelpText(objective,Vector3(0,1,0));
				NewObjectiveSystemControl.Set_Objective_Status(3,NewObjectiveSystem::Accomplished);
				NewObjectiveSystemControl.Set_Objective_Status(4,NewObjectiveSystem::Accomplished);
				NewObjectiveSystemControl.Remove_Objective(11);
				NewObjectiveSystemControl.Add_Objective(6,NewObjectiveSystem::Primary,NewObjectiveSystem::Pending,12698,"",12698,JMG_Bear_Hunter_Game_Control::centerOfBase);
				Commands->Set_Objective_HUD_Info_Position(6,1,"PogPrez.tga",7387,JMG_Bear_Hunter_Game_Control::centerOfBase);
				NewObjectiveSystemControl.Add_Objective(5,NewObjectiveSystem::Secondary,NewObjectiveSystem::Pending,12697,"",12697,JMG_Bear_Hunter_Game_Control::centerOfBase);
				Commands->Set_Objective_HUD_Info_Position(5,1,"PogDERP.tga",7387,JMG_Bear_Hunter_Game_Control::centerOfBase);
				if (NewObjectiveSystemControl.Get_Objective_Status(13) == NewObjectiveSystem::Pending)
					NewObjectiveSystemControl.Set_Objective_Status(13,NewObjectiveSystem::Failed);
			}
			if (gameTime == 835 && !JMG_Bear_Hunter_Game_Control::hasGotTurrets && NewObjectiveSystemControl.Get_Objective_Status(10) == NewObjectiveSystem::Pending)
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Our engineers report if the truck isn't back in the next 60 seconds they won't be able to get them set up!");
			if (gameTime == 775)
			{
				gameState = FallBack;
				spawnGroup = 2;
				Vector3 truckPos;
				if (!JMG_Bear_Hunter_Game_Control::hasGotTurrets)
					if (NewObjectiveSystemControl.Get_Objective_Status(10) == NewObjectiveSystem::Pending && NewObjectiveSystemControl.Get_Radar_Blip_Position(10,&truckPos) && JmgUtility::SimpleFlatDistance(JMG_Bear_Hunter_Game_Control::centerOfBase,truckPos) <= 160000)
					{
						truckTimeExtended = true;
						JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: We see the truck is getting close, the engineers are going to give you another 45 seconds! Everyone else fall back to base!");
					}
					else
					{
						JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Forget about the turret parts there isn't enough time! Fall back to base!");
						if (NewObjectiveSystemControl.Get_Objective_Status(10) != NewObjectiveSystem::Accomplished)
						{
							NewObjectiveSystemControl.Remove_Objective(9);
							NewObjectiveSystemControl.Remove_Objective(10);
						}
					}
				else
					JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Everyone fall back to base!");
				Commands->Enable_Spawner(600536,false);
				if (NewObjectiveSystemControl.Get_Objective_Status(14) == NewObjectiveSystem::Pending)
					NewObjectiveSystemControl.Remove_Objective(14);
				if (NewObjectiveSystemControl.Get_Objective_Status(15) == NewObjectiveSystem::Pending)
					NewObjectiveSystemControl.Set_Objective_Status(15,NewObjectiveSystem::Hidden);
			}
			if (gameTime == 730 && !JMG_Bear_Hunter_Game_Control::hasGotTurrets && truckTimeExtended)
			{
				Vector3 truckPos;
				if (NewObjectiveSystemControl.Get_Objective_Status(10) == NewObjectiveSystem::Pending && NewObjectiveSystemControl.Get_Radar_Blip_Position(10,&truckPos) && JmgUtility::SimpleFlatDistance(JMG_Bear_Hunter_Game_Control::centerOfBase,truckPos) <= 10000)
					JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: We're giving you another 15 seconds to get those turret parts inside DERP Industries beings your so close!");
				else
				{
					truckTimeExtended = false;
					if (NewObjectiveSystemControl.Get_Objective_Status(10) != NewObjectiveSystem::Accomplished)
					{
						JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: We need all the engineers back to work on the gates, there is no more time for the turret parts!");
						NewObjectiveSystemControl.Remove_Objective(9);
						NewObjectiveSystemControl.Remove_Objective(10);
					}
				}
			}
			if (gameTime == 715 && !JMG_Bear_Hunter_Game_Control::hasGotTurrets && truckTimeExtended && NewObjectiveSystemControl.Get_Objective_Status(10) != NewObjectiveSystem::Accomplished)
			{
				truckTimeExtended = false;
				if (NewObjectiveSystemControl.Get_Objective_Status(10) != NewObjectiveSystem::Accomplished)
				{
					JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Forget the turret parts and fall back to base, there isn't any more time to set them up!");
					NewObjectiveSystemControl.Remove_Objective(9);
					NewObjectiveSystemControl.Remove_Objective(10);
				}
			}
			if (gameTime == 700)
			{
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: This could get pretty intense, if you have a chance try to head east, you will find a radio tower, if you can activate it we can call for backup.");
				NewObjectiveSystemControl.Add_Objective(21,NewObjectiveSystem::Secondary,NewObjectiveSystem::Pending,12789,"",12789,Vector3(-512.0f,-270.705f,76.11f));
			}
			if (gameTime == 624)
			{
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Watch your backs, there are mutant dogs out there now!");
			}
			if (gameTime == 400 && objective != 12698)
			{
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: This is just getting to be too much, fall back and defend the President of Corporate America!");
				NewObjectiveSystemControl.Remove_Objective(5);
				JmgUtility::SetHUDHelpText(objective,Vector3(0,1,0));
				objective = 12698;
			}
			if (gameTime == 395)
			{
				sprintf(currentSong,"Suspence 09.mp3");
				Commands->Fade_Background_Music("Suspence 09.mp3",10000,10000);
			}
			if (gameTime == 320)
			{
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Looks like cats are infected now too, I think I'll just be leaving...");
				sprintf(currentSong,"304824_Goliath__Club_Remix_.mp3");
				Commands->Fade_Background_Music("304824_Goliath__Club_Remix_.mp3",10000,10000);
			}
			if (gameTime == JMG_Bear_Hunter_Game_Control::medicineDropInTime)
			{
				GameObject *cinematic = Commands->Create_Object("Daves Arrow",Vector3(74.53f,-621.425f,10.0f));
				Commands->Set_Is_Rendered(cinematic,false);
				Commands->Attach_Script(cinematic,"Test_Cinematic","medicine_drop.txt");
			}
			if (gameTime == 240)
			{
				if (NewObjectiveSystemControl.Get_Objective_Status(21) == NewObjectiveSystem::Pending && NewObjectiveSystemControl.Get_Mission_Text_Id(21) != 12790)
					JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: The gate will be back on-line in the next 4 minutes, if you can get the signal sent within the next minute don't bother!");
				gameState = DefendPrez;
			}
			if (gameTime == 180)
			{
				if (NewObjectiveSystemControl.Get_Objective_Status(21) == NewObjectiveSystem::Pending && NewObjectiveSystemControl.Get_Mission_Text_Id(21) != 12790)
				{
					JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Forget about the communications tower, there isn't enough time anymore focus all resources on defending the president!");
					NewObjectiveSystemControl.Remove_Objective(21);
				}
			}
			if (gameTime == 150)
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Oh god, is that a moose? What are they doing out here… Never mind, defend the President of Corperate America!");
			if ((gameTime == 600 || gameTime == 300) || (!(gameTime % 60) && gameTime > 0 && gameTime < 300))
			{
				char timeMsg[220];
				sprintf(timeMsg,"Bear Rug Co: The gate control mainframe will be repaired in %d minute%s",gameTime/60,gameTime/60 == 1 ? "!" : "s!");
				JmgUtility::MessageAllPlayers(127,127,255,timeMsg);
			}
			if (gameTime == 30 || gameTime == 15 || gameTime == 10)
			{
				char timeMsg[220];
				sprintf(timeMsg,"Bear Rug Co: The gate control mainframe will be repaired in %d seconds!",gameTime);
				JmgUtility::MessageAllPlayers(127,127,255,timeMsg);
			}
			if (gameTime && gameTime <= 5)
			{
				char timeMsg[220];
				sprintf(timeMsg,"Bear Rug Co: %d",gameTime);
				JmgUtility::MessageAllPlayers(127,127,255,timeMsg);
			}
			if (gameTime == 0)
			{
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Mainframe back on-line, gates are closing!");
				Commands->Static_Anim_Phys_Goto_Last_Frame(650185,"BH_GATE_1.BH_GATE_1");
				Commands->Static_Anim_Phys_Goto_Last_Frame(650186,"BH_GATE_1.BH_GATE_1");
				Commands->Static_Anim_Phys_Goto_Last_Frame(650187,"BH_GATE_1.BH_GATE_1");
				Commands->Static_Anim_Phys_Goto_Last_Frame(650188,"BH_GATE_1.BH_GATE_1");
				Commands->Start_Timer(obj,this,12,4);
				if (JMG_Bear_Hunter_Comanche::id)
					JmgUtility::MessageAllPlayers(127,127,255,"Comanche 4573: We're getting a high priority call, you should be able to clean up the rest, good luck!");
				return;
			}
			soundAttentionWhoreNotify = Get_Player_Count() ? 0.0025f+(Get_Player_Count()-1)*0.001f : 0.0025f;
			if (bearTransition != 100 && gameTime < 900)
				bearTransition = 100;
			int maxTotalEnemies = maxBears+Get_Player_Count();
			JMG_Bear_Hunter_Game_Control::mutantRespawnTime = JmgUtility::MathClamp(15.0f*(1-JmgUtility::MathClampInt(gameTime,0,1200)/1200.0f),0.0f,15.0f);
			JMG_Bear_Hunter_Game_Control::mutantBears = (int)(bearTransition == 100 ? maxTotalEnemies : (bearTransition ? maxTotalEnemies*(bearTransition/100.0f) : 0));
			CreatePositionZ = 500-1000*(1-gameTime/1800.0f)+2;
			GamePercent = JmgUtility::MathClamp(1-(gameTime-Get_Player_Count()*2.5f)/1210.0f,0.0f,10.0f);//Was 7.5
			int normalBears = (int)(!bearTransition ? maxTotalEnemies : (bearTransition == 100 ? 0 : maxTotalEnemies*(1-bearTransition/100.0f)));
			for (int x = bearObjectListControl.objectCount;x < normalBears;x++)
			{
				Vector3 pos = BearHunterGameControlSystem.LookupNearestPosition(Vector2(Commands->Get_Random(-500.0f,500.0f),Commands->Get_Random(-350.0f,500.0f)));
				bool isBlackBear = Commands->Get_Random(0.0f,1.0f) < 0.1 ? true : false;
				GameObject *ai = isBlackBear ? Commands->Create_Object("Bear_Black",pos) : Commands->Create_Object("Normal_Bear",pos);
				Commands->Set_Facing(ai,Commands->Get_Random(-180,180));
				MoveablePhysClass *mphys = ai->As_PhysicalGameObj() ? ai->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
				if (mphys)
					mphys->Find_Teleport_Location(pos,2.0f,&pos);
				pos.Z += 0.1f;
				Commands->Set_Position(ai,pos);
				Commands->Set_Loiters_Allowed(ai,true);
				bearObjectListControl += ai;
				Attach_Script_Once(ai,"JMG_Rp2_Hostile_Mutant_AI","");
				Attach_Script_Once(ai,"JMG_Bear_Hunter_Bear_Tracker","");
				Attach_Script_Once(ai,"JMG_Bear_Hunter_Animal_Control","");
				Attach_Script_Once(ai,"JMG_Bear_Hunter_Player_Count_Scaled_Object_Health","");
			}
			if (bearObjectListControl.objectCount > normalBears)
				bearObjectListControl.DestroyLeastSignificant();
			if (gameTime == 1200)
			{
				GameObject *Meteor = Commands->Create_Object("Meteorite_Animation",BearHunterGameControlSystem.LookupNearestPosition(Vector2(0.0f,200.0f)));
				Commands->Set_Model(Meteor,"meteorite");
				Commands->Set_Animation(Meteor,"meteorite.meteorite",false,0,0.0f,30.0f,false);
				Commands->Send_Custom_Event(Meteor,obj,987988,0,1.0f);
			}
		}
		Commands->Start_Timer(obj,this,1.0f,3);
	}
	if (number == 4)
	{
		gameState = SecureBase;
		JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Kill all remaining mutants inside the walls!");
		objective = 12699;
		JmgUtility::SetHUDHelpText(objective,Vector3(0,1,0));
		NewObjectiveSystemControl.Set_Objective_Status(6,NewObjectiveSystem::Accomplished);
		NewObjectiveSystemControl.Add_Objective(7,NewObjectiveSystem::Primary,NewObjectiveSystem::Pending,12699,"",12699,Vector3(434.101f,539.071f,1.142f));
		Commands->Set_Objective_HUD_Info(7,1,"PogMutants.tga",7378);
		sprintf(currentSong,"sakura battle theme.mp3");
		Commands->Fade_Background_Music("sakura battle theme.mp3",10000,10000);
		Commands->Static_Anim_Phys_Goto_Last_Frame(650185,"BH_GATE_1.BH_GATE_1");
		Commands->Static_Anim_Phys_Goto_Last_Frame(650186,"BH_GATE_1.BH_GATE_1");
		Commands->Static_Anim_Phys_Goto_Last_Frame(650187,"BH_GATE_1.BH_GATE_1");
		Commands->Static_Anim_Phys_Goto_Last_Frame(650188,"BH_GATE_1.BH_GATE_1");
		Vector3 centerOfBase = Vector3(-0.532f,-705.389f,0.0f);
		for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
		{
			SmartGameObj* o = current->Data();
			if (o && Is_Script_Attached(o,"JMG_Bear_Hunt_Mutant_Attacker") && JmgUtility::SimpleFlatDistance(Commands->Get_Position(o),centerOfBase) > 22500)
				Commands->Apply_Damage(o,1000.0f,"None",o);
		}
		bearObjectListControl.DestroyOutsideOfRange(22500,centerOfBase);
		mutantObjectListControl.DestroyOutsideOfRange(22500,centerOfBase);
		Commands->Start_Timer(obj,this,1.0f,5);
		Commands->Start_Timer(obj,this,1.0f,15);
		Commands->Start_Timer(obj,this,2.0f,15);
	}
	if (number == 5)
	{
		if (!bearObjectListControl.objectCount && !mutantObjectListControl.objectCount)
		{
			if (The_Game()->Is_Game_Over())
				return;
			bool isOver = true;
			for (int x = 0;x < 128;x++)
			{
				GameObject *player = Get_GameObj(x);
				if (JMG_Bear_Hunter_Game_Control::remainingLives[x]  && (!player || (player && Get_Player_Type(player) != -4)))
					isOver = false;
			}
			if (isOver && gameState == JMG_Bear_Hunter_Game_Control::BossArrive)
				NewObjectiveSystemControl.Set_Objective_Status(12,NewObjectiveSystem::Failed);
			winWait++;
			if (winWait == 3)
			{
				NewObjectiveSystemControl.Set_Objective_Status(7,NewObjectiveSystem::Accomplished);
				Commands->Fade_Background_Music("null.mp3",5000,5000);
				sprintf(currentSong,"null.mp3");
				spawnKarma = 0;
				if (JMG_Bear_Hunter_President_Controller::lastHPRecorded >= 500.0)
					spawnKarma++;
				if (!JMG_Bear_Hunter_Game_Control::hasBeenInjured)
					spawnKarma += 2;
				if (!JMG_Bear_Hunter_Game_Control::hasBeenInjured)
					JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: I cannot believe it; the president did not even get hurt once, if I did not know better, I would think you were cheating!");
				else
					JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Great work guys, I think thats all of them!");
			}
			if (winWait == 13)
			{
				Commands->Set_Wind(1.0f,60.0f,1.0f,40.0f);
				Commands->Set_Rain(3.0f,30.0f,true);
				int turretState = 0;
				if (NewObjectiveSystemControl.Get_Objective_Status(23) != NewObjectiveSystem::NotDefined)
				{
					if (NewObjectiveSystemControl.Get_Mission_Text_Id(23) == 12799)
						NewObjectiveSystemControl.Set_Objective_Status(23,NewObjectiveSystem::Failed);
					if (NewObjectiveSystemControl.Get_Mission_Text_Id(23) == 12798)
					{
						NewObjectiveSystemControl.Set_Objective_Status(23,NewObjectiveSystem::Accomplished);
						turretState = 1;
					}
				}
				else
					turretState = 2;
				if (turretState)
					JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Just in time too, looks like the turret control computer was about to burn up. Shutting it down now...");
				if (0.7f < (1.0f*NewObjectiveSystemControl.Get_Objective_Priority_Count(NewObjectiveSystem::Secondary,NewObjectiveSystem::Accomplished)+NewObjectiveSystemControl.Get_Objective_Priority_Count(NewObjectiveSystem::Tertiary,NewObjectiveSystem::Accomplished)+NewObjectiveSystemControl.Get_Objective_Priority_Count(NewObjectiveSystem::Quaternary,NewObjectiveSystem::Accomplished))/(1.0f*NewObjectiveSystemControl.Get_Objective_Priority_Count(NewObjectiveSystem::Secondary)+NewObjectiveSystemControl.Get_Objective_Priority_Count(NewObjectiveSystem::Tertiary)+NewObjectiveSystemControl.Get_Objective_Priority_Count(NewObjectiveSystem::Quaternary)))
					spawnKarma++;
				if (JMG_Bear_Hunter_Power_Transformer::online && !JMG_Bear_Hunter_Game_Control::turretsDestroyed)
					spawnKarma++;
				for (int x = 1;x < 128;x++)
				{
					if (Get_Money(x) > 2000)
					{
						spawnKarma++;
						break;
					}
				}
				for (int x = 1;x < 128;x++)
				{
					GameObject *player = Get_GameObj(x);
					if (!player || Get_Player_Type(player) == -4)
						continue;
					BearHunterScoreSystem::BHScoreNode *playerScoreNode = bearHunterScoreSystem.Get_Current_Player_Score_Node(x);
					if (playerScoreNode)
						if (turretState == 1)
							playerScoreNode->SubstationOnLineAtEnd++;
						else if (turretState == 2)
							playerScoreNode->SubstationNotDamaged++;
				}
				for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
				{
					SmartGameObj* o = current->Data();
					if (!o)
						continue;
					int presetId = Commands->Get_Preset_ID(o);
					if (presetId == 1000001055 || presetId == 1000001176 || presetId == 1000001536 || presetId == 1000001820)
					{
						Commands->Send_Custom_Event(o,o,2411911,false,0);
						Remove_All_Scripts(o);
						Commands->Action_Reset(o,1);
						Commands->Action_Reset(o,10);
						Commands->Action_Reset(o,100);
						Commands->Attach_Script(o,"JMG_Bear_Hunter_AI_Ignore_Up_To_25_Meters","");
					}
					if (presetId == 1000001820)
						Commands->Set_Model(o,"SearchLightOff");
					const char *weap = Get_Current_Weapon(o);
					if (o->As_VehicleGameObj() && weap && _stricmp(weap,"Weapon_Super_Repair"))
					{
						PhysClass *vpc = o->As_PhysicalGameObj()->Peek_Physical_Object();
						if (vpc && (vpc->As_WheeledVehicleClass() || vpc->As_MotorcycleClass() || vpc->As_TrackedVehicleClass() || vpc->As_VTOLVehicleClass()))
							continue;
						if (!Is_Script_Attached(o,"JMG_Bear_Hunter_AI_Ignore_Up_To_25_Meters"))
							Commands->Attach_Script(o,"JMG_Bear_Hunter_AI_Ignore_Up_To_25_Meters","");
						if (Has_Weapon(o,"Weapon_Super_Repair"))
							Commands->Select_Weapon(o,"Weapon_Super_Repair");
						else
						{
							Commands->Give_PowerUp(o,"POW_Super_Heal",false);
							if (Has_Weapon(o,"Weapon_Super_Repair"))
								Commands->Select_Weapon(o,"Weapon_Super_Repair");
						}
					}
				}
			}
			if (winWait == 23)
			{
				if (spawnKarma <= 1)
					JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Haha, its a good thing this isn't a video game, otherwise I'd have expected some sort of super mutant boss.");
				else if (spawnKarma <= 3)
					JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Haha, its a good thing this isn't a video game, otherwise I'd have expected some sort of super mutant boss, with maybe some support bosses because you did so well.");
				else
					JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: However, it is a good thing you people are not cheaters, otherwise karma would try to find a way to balance the odds.");
				Commands->Set_Lightning(1.0f,0.0f,0.25f,0.0f,1.0f,20.0f);
				sprintf(currentSong,"304557_Goliath.mp3");
			}
			if (winWait == 33)
				Commands->Fade_Background_Music("304557_Goliath.mp3",10000,10000);
			if (winWait == 43)
			{
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Well, we should have seen this coming, hurry guys, kill it and we'll call it a day!");
				Commands->Create_Object("Mutant_Rabbit_Boss",Vector3(0.0f,0.0f,0.0f));
				if (spawnKarma >= 2)
					for (int x = 0;x < spawnKarma;x++)
						Commands->Create_Object("Giant_Deer_Blue",Vector3(0.0f,0.0f,0.0f));
				Commands->Start_Timer(obj,this,0.1f,17);
			}
			if (winWait == 300)
				JmgUtility::MessageAllPlayers(200,127,255,"President of Corporate America: I'm not feeling so well, please hurry and kill those mutants!");
			if (winWait >= 300)
			{
				GameObject *president = Commands->Find_Object(JMG_Bear_Hunter_Game_Control::mutantTargetId);
				if (president)
					Commands->Apply_Damage(president,5.0f,"None",NULL);
				else
					return;
			}
		}
		else
			winWait = 0;
		Commands->Start_Timer(obj,this,1.0f,5);
	}
	if (number == 6)
	{
		if (bearTransition == 75)
		{
			Commands->Fade_Background_Music("Action 05 00.mp3",62500,62500);
			sprintf(currentSong,"Action 05 00.mp3");
		}
		if (gameTime < 1200 && bearTransition < 100)
		{
			bearTransition++;
			Commands->Start_Timer(obj,this,2.5f,6);
		}
	}
	if (number == 7)
	{
		for (int x = 0;x < maxTotalBearHunterPlayerAssistAI;x++)
			if (JMG_Bear_Hunter_Player_Assist_AI::aiDeaths[x])
				JMG_Bear_Hunter_Player_Assist_AI::aiDeaths[x]--;
		for (int x = 1;x < 128;x++)
			if (PlayerData.players[x] && PlayerData.players[x]->deaths)
				PlayerData.players[x]->deaths--;
		Commands->Start_Timer(obj,this,60.0,7);
	}
	if (number == 9)
	{
		GameObject *switchObj = Commands->Find_Object(easTowers[1].id);
		if (switchObj)
		{
			Vector3 switchPos = Commands->Get_Position(switchObj);
			if (CheckIfPlayerInRange(switchPos,22500))
			{
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: By the way, there is an active security system in the complex, you might want to deactivate it before you take the truck.");
				NewObjectiveSystemControl.Add_Objective(14,NewObjectiveSystem::Quaternary,NewObjectiveSystem::Pending,12740,"",12740);
				return;
			}
		}
		Commands->Start_Timer(obj,this,10.0,9);
	}
	if (number == 10)
	{
		bool missionUpdate = false;
		for (int x = 0;x < 5;x++)
		{
			GameObject *ai = Commands->Find_Object(JMG_Bear_Hunter_Engineer_AI::engineerIds[x]);
			if (!ai && (JMG_Bear_Hunter_Engineer_AI::engineerMode[x] || (!JMG_Bear_Hunter_Engineer_AI::engineerMode[x] && !engineerSpawnedOnce[x])))
			{
				Vector3 createPosition = engineerSpawnLocations[x];
				if (JMG_Bear_Hunter_Engineer_AI::engineerMode[x])
				{
					Rp2SimplePositionSystem::SimplePositionNode *posNode = randomSelectableSpawnPoints[5].GetRandom();
					if (posNode)
						createPosition = posNode->position;
				}
				ai = Commands->Create_Object(Commands->Get_Random(0.0f,1.0f) < 0.5 ? "CnC_Nod_Engineer_AI" : "CnC_GDI_Engineer_AI",createPosition);
				Commands->Set_Player_Type(ai,1);
				engineerSpawnedOnce[x] = true;
				if (JMG_Bear_Hunter_Engineer_AI::engineerMode[x])
					Commands->Attach_Script(ai,"JMG_Bear_Hunter_Engineer_AI","Weapon_RepairGun_Player,Weapon_Pistol_Player");
				else
				{
					Commands->Select_Weapon(ai,"Weapon_Pistol_Player");
					char params[512];
					sprintf(params,"%d,50.0,1,1,1,0",x);
					Commands->Attach_Script(ai,"JMG_Bear_Hunter_Engineer_Follow_Player_When_Near",params);
				}
				MoveablePhysClass *mphys = ai->As_PhysicalGameObj() ? ai->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
				if (mphys)
					mphys->Find_Teleport_Location(createPosition,1.0f,&createPosition);
				Commands->Set_Position(ai,createPosition);
				JMG_Bear_Hunter_Engineer_AI::engineerIds[x] = Commands->Get_ID(ai);
			}
			else if (!JMG_Bear_Hunter_Engineer_AI::engineerMode[x])
			{
				float dist = JmgUtility::SimpleFlatDistance(Commands->Get_Position(ai),Vector3(-0.389f,-705.611f,0.0f));
				if (dist < 27390.25)
				{
					JMG_Bear_Hunter_Engineer_AI::engineerMode[x] = true;
					if (Is_Script_Attached(ai,"JMG_Bear_Hunter_Engineer_Follow_Player_When_Near"))
						Remove_Script(ai,"JMG_Bear_Hunter_Engineer_Follow_Player_When_Near");
					GameObject *vehicle = Get_Vehicle(ai);
					if (vehicle)
					{
						Soldier_Transition_Vehicle(ai);
						Commands->Action_Reset(vehicle,100);
					}
					Commands->Action_Reset(ai,100);
					Commands->Select_Weapon(ai,"Weapon_RepairGun_Player");
					Commands->Set_Player_Type(ai,1);
					ActionParamsStruct params;
					Commands->Action_Reset(ai,1000);
					params.Set_Basic(this,100,10);
					params.Set_Movement(Vector3(-0.532f,-705.389f,0.0f),1.0f,10.0f,false);
					params.MoveFollow = false;
					params.MovePathfind = true;
					Commands->Action_Goto(ai,params);
					Commands->Attach_Script(ai,"JMG_Bear_Hunter_Engineer_AI","Weapon_RepairGun_Player,Weapon_Pistol_Player");
					entineersReturned++;
					float money = 125;
					if (entineersReturned == 5)
						money = 500;
					JMG_Bear_Hunter_Game_Control::ObjectiveCompleteReward(money);
					GameObject *guide = Get_GameObj(JMG_Bear_Hunter_Engineer_Follow_Player_When_Near::followPlayerId[x]);
					if (guide)
					{
						Commands->Give_Points(guide,250.0f,false);
						BearHunterScoreSystem::BHScoreNode *node = bearHunterScoreSystem.Get_Current_Player_Score_Node(JmgUtility::JMG_Get_Player_ID(guide));
						node->BonusObjectivesCompleted++;
						node->totalObjectivesCompleted++;
						node->ObjectiveEngineersSaved++;
					}
					missionUpdate = true;
				}
			}
		}
		if (engineerScanTime)
			engineerScanTime--;
		if (missionUpdate && !engineerScanTime)
		{
			engineerScanTime = 10;
			if (entineersReturned == 5)
			{
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Thats all of them, great work guys!");
				NewObjectiveSystemControl.Set_Objective_Status(15,NewObjectiveSystem::Accomplished);
			}
			else
			{
				int totalEngineers = entineersReturned+JMG_Bear_Hunter_Engineer_AI::engineersDead;
				char successMsg[220];
				if (totalEngineers == 5)
				{
					sprintf(successMsg,"Bear Rug Co: Looks like we saved %d of the 5 engineers, it's good to see some of them back.",entineersReturned);
					JmgUtility::MessageAllPlayers(127,127,255,successMsg);
					NewObjectiveSystemControl.Remove_Objective(15);
				}
				else
				{
					sprintf(successMsg,"Bear Rug Co: Good work bringing another engineer home, %d more to go.",5-totalEngineers);
					JmgUtility::MessageAllPlayers(127,127,255,successMsg);
					unsigned int textId = (totalEngineers == 1) ? 12743 : (totalEngineers == 2) ? 12744 : (totalEngineers == 3) ? 12745 : 12746;
					NewObjectiveSystemControl.Set_Objective_Mission(15,textId,textId);
				}
			}
		}
		if (JMG_Bear_Hunter_Engineer_AI::scanAgainForEngineerDeaths)
			JMG_Bear_Hunter_Engineer_AI::scanAgainForEngineerDeaths--;
		if (JMG_Bear_Hunter_Engineer_AI::engineersDead != lastUpdateEngineersDead)
		{
			lastUpdateEngineersDead = JMG_Bear_Hunter_Engineer_AI::engineersDead;
			switch (JMG_Bear_Hunter_Engineer_AI::engineersDead)
			{
			case 1:JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Looks like we lost an engineer.");break;
			case 2:JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Another engineer has died.");break;
			case 3:JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: We've lost another engineer! Be more careful, these people have families!");break;
			case 4:JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Four engineers dead, maybe you should just let the last one hide.");break;
			case 5:JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: We've lost all the engineers, I understand you guys didn't have training in fighting killer animals, but we still expected better.");break;
			}
			int totalEngineers = entineersReturned+JMG_Bear_Hunter_Engineer_AI::engineersDead;
			if (JMG_Bear_Hunter_Engineer_AI::engineersDead == 5)
				NewObjectiveSystemControl.Set_Objective_Status(15,NewObjectiveSystem::Failed);
			else if (totalEngineers == 5)
			{
				char engineerMsg[220];
				sprintf(engineerMsg,"Bear Rug Co: Looks like we saved %d of the 5 engineers, it's good to see some of them back.",entineersReturned);
				JmgUtility::MessageAllPlayers(127,127,255,engineerMsg);
				NewObjectiveSystemControl.Remove_Objective(15);
			}
			else
			{
				unsigned int textId = (totalEngineers == 1) ? 12743 : (totalEngineers == 2) ? 12744 : (totalEngineers == 3) ? 12745 : 12746;
				NewObjectiveSystemControl.Set_Objective_Mission(15,textId,textId);
			}
		}
		Commands->Start_Timer(obj,this,1.0f,10);
	}
	if (number == 11)
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player || Get_Player_Type(player) == -4)
				continue;
			if (JmgUtility::SimpleFlatDistance(Vector3(50.0f,-685.0f,0),Commands->Get_Position(player)) > 2500.0f)
			{
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: While you are hunting for bears keep an eye out for weapons containers, 6 of them have been misplaced and we will let you use the weapons if you bring them back to base.");
				NewObjectiveSystemControl.Add_Objective(18,NewObjectiveSystem::Bonus,NewObjectiveSystem::Pending,12772,"",12772,1);
				return;
			}
		}
		Commands->Start_Timer(obj,this,5.0,11);
	}
	if (number == 12)
	{
		GameObject *giantDeer = Commands->Create_Object("Giant_Deer",Vector3(424.87f,133.22f,0.22f));
		Commands->Attach_Script(giantDeer,"JMG_Bear_Hunter_Giant_Deer_Boss","");
		blackBearId = Commands->Get_ID(giantDeer);
		for (int x = 0;x < 6;x++)
		{
			Rp2SimplePositionSystem::SimplePositionNode *spot = randomWeaponContainerSpawnPositions[x].GetRandom();
			if (!spot)
			{
				Console_Input("msg JMG_Bear_Hunter_Game_Control ERROR: could not find random position for timer 12!");
				continue;
			}
			GameObject *powerup = Commands->Create_Object("POW_Weapons_Container",spot->position);
			Commands->Set_Facing(powerup,spot->facing);
			char params[512];
			sprintf(params,"%d",x);
			Commands->Attach_Script(powerup,"JMG_Bear_Hunter_Weapons_Container",params);
		}
	}
	if (number == 13)
	{
		if (The_Game()->Is_Game_Over())
			return;
		bearHunterScoreSystem.StateHighScore();
		Commands->Start_Timer(obj,this,60.0f,13);
	}
	if (number == 14)
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player || !PlayerData.players[x] || !Has_Weapon(player,"Weapon_Binoculars"))
				continue;
			if (PlayerData.players[x]->binocularTime < 100)
			{
				PlayerData.players[x]->binocularTime++;
				Set_Bullets(player,"Weapon_Binoculars",PlayerData.players[x]->binocularTime);
			}
			if (PlayerData.players[x]->binocularTime == 100 && Get_Bullets(player,"Weapon_Binoculars") < 100)
			{
				NewObjectiveSystemControl.Display_All_Objectives(player);
				PlayerData.players[x]->binocularTime = 0;
				Set_Bullets(player,"Weapon_Binoculars",PlayerData.players[x]->binocularTime);
			}
		}
		Commands->Start_Timer(obj,this,0.6f,14);
	}
	if (number == 15)
	{
		Vector3 centerOfBase = Vector3(-0.532f,-705.389f,0.0f);
		for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
		{
			SmartGameObj* o = current->Data();
			if (o && Is_Script_Attached(o,"JMG_Bear_Hunt_Mutant_Attacker") && JmgUtility::SimpleFlatDistance(Commands->Get_Position(o),centerOfBase) > 22500)
				Commands->Apply_Damage(o,1000.0f,"None",o);
		}
		bearObjectListControl.DestroyOutsideOfRange(22500,centerOfBase);
		mutantObjectListControl.DestroyOutsideOfRange(22500,centerOfBase);
	}
	if (number == 16)
	{
		if (gameState == SecureBase)
		{
			Vector3 centerOfBase = Vector3(-0.532f,-705.389f,0.0f);
			mutantObjectListControl.DestroyOutsideOfRange(22500,centerOfBase);
			return;
		}
		AiMutantRespawnSystem.UpdateTeamAndSpawnCount(JMG_Bear_Hunter_Game_Control::mutantBears,JMG_Bear_Hunter_Game_Control::MutantSpawn);
		Commands->Start_Timer(obj,this,0.1f,16);
	}
	if (number == 17)
	{
		if (JMG_Bear_Hunter_Game_Control::gameState == JMG_Bear_Hunter_Game_Control::BossArrive)
			for (int x = 1;x < 128;x++)
			{
				GameObject *player = Get_GameObj(x);
				if (!player)
					continue;
				if (JMG_Bear_Hunter_Game_Control::remainingLives[x] >= 0 && Get_Player_Type(player) != -4)
					continue;
				if (!Has_Weapon(player,"Weapon_Empty_Hands"))
				{
					Commands->Destroy_Object(player);
					continue;
				}
				const char *weap = Get_Current_Weapon(player);
				if (weap && _stricmp(weap,"Weapon_Empty_Hands"))
					Commands->Select_Weapon(player,"Weapon_Empty_Hands");
				if (_stricmp(Commands->Get_Preset_Name(player),"CNC_Spectator"))
					Change_Character(player,"CNC_Spectator");
			}
		Commands->Start_Timer(obj,this,0.1f,17);
	}
}
void JMG_Bear_Hunter_Game_Control::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 91191120)
	{
		easTowers[param] = EASTowerNode::EASTowerNode(sender);
	}
	if (message == 91191121)
	{
		int senderId = Commands->Get_ID(sender);
		for (int x = 0;x < 2;x++)
			if (easTowers[x] == senderId)
			{
				easTowers[x].activeTime = param;
				break;
			}
	}
	if (message == 987988)
	{
		for (int x = 0;x < 3;x++)
			Commands->Create_Sound("SFX.Dynamite_Explosion2",Commands->Get_Position(sender),sender);
		Commands->Create_Explosion("Explosion_Meteor",Commands->Get_Position(sender),sender);
		int pumpJackIds[7] = {601041,601038,601044,601040,601045,601039,601043};
		for (int x = 0;x < 7;x++)
		{
			GameObject *pumpJack = Commands->Find_Object(pumpJackIds[x]);
			if (pumpJack)
			{
				float health = Commands->Get_Health(pumpJack)-50.0f;
				if (health < 0)
					Commands->Apply_Damage(pumpJack,9999.9f,"BlamoKiller",sender);
				else
					Commands->Set_Health(pumpJack,health);
			}
		}
		if (easTowers[0].id == 601036)
		{
			GameObject *switchObj = Commands->Find_Object(easTowers[0].id);
			if (switchObj)
			{
				Vector3 switchPos = Commands->Get_Position(switchObj);
				if (easTowers[0].activeTime)
				{
					smoothFade = true;
					Commands->Send_Custom_Event(obj,switchObj,4511911,0,0);
				}
				Vector3 createPos = Commands->Get_Position(switchObj);
				createPos.Z += 1.5f;
				createPos.Y -= 0.05f;
				GameObject *sparks = Commands->Create_Object("Daves Arrow",createPos);
				Commands->Set_Facing(sparks,Commands->Get_Facing(switchObj));
				Commands->Set_Model(sparks,"vis_sparks");
				Commands->Enable_HUD_Pokable_Indicator(switchObj,false);
				Remove_All_Scripts(switchObj);
			}
		}
		Commands->Destroy_Object(sender);
	}
}
void JMG_Bear_Hunter_Game_Control::Destroyed(GameObject *obj)
{
	if (cleanUpNeeded)
		EndGameDataCleanup(obj,false);
	myObject = NULL;
}
bool JMG_Bear_Hunter_Game_Control::CheckIfPlayerInRange(Vector3 pos,float distance)
{
	for (int j = 1;j < 128;j++)
	{
		GameObject *player = Get_GameObj(j);
		if (!player || Get_Player_Type(player) == -4)
			continue;
		if (JmgUtility::SimpleDistance(pos,Commands->Get_Position(player)) < distance)
			return true;
	}
	return false;
}
void JMG_Bear_Hunter_Game_Control::EndGameDataCleanup(GameObject *obj,bool saveData)
{
	AiMutantRespawnSystem.ClearList();
	JMG_Bear_Hunter_Game_Control::gameState = LeaveBase;
	for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
	{
		SmartGameObj* o = current->Data();
		if (o && o != obj)
			Remove_All_Scripts(o);
	}
	bearTransition = 0;
	myObject = NULL;
	mutantTargetId = 600172;
	gameOver = false;
	gameTime = 0;
	spawnGroup = 0;
	objective = 0;
	entineersReturned = 0;
	sprintf(currentSong,"SpecForce_Sneak_01.mp3");
	for (int x = 0;x < maxTotalBearHunterPlayerAssistAI;x++)
	{
		GameObject *ai = Commands->Find_Object(JMG_Bear_Hunter_Player_Assist_AI::aiIds[x]);
		if (ai)
			Commands->Destroy_Object(ai);
		JMG_Bear_Hunter_Player_Assist_AI::cash[x] = 0;
		JMG_Bear_Hunter_Player_Assist_AI::aiDeaths[x] = 0;
	}
	BearHunterGameControlSystem.emptyList();
	mutantObjectListControl.Empty_List();
	bearObjectListControl.Empty_List();
	PlayerData.Cleanup();
	AIPurchaseSystemControl.Empty_List();
	NewObjectiveSystemControl.~NewObjectiveSystem();
	for (int x = 0;x < 6;x++)
	{
		randomSelectableSpawnPoints[x].Empty_List();
		aiDefensePoints[x].Empty_List();
		weaponsFound[x] = false;
		weaponsReturned[x] = false;
		weaponsUnlocked[x] = false;
		randomWeaponContainerSpawnPositions[x].Empty_List();
		ComancheDefensePoints[x].Empty_List();
	}
	weaponsUnlocked[6] = false;
	aiPtLocations.Empty_List();
	if (saveData)
		bearHunterScoreSystem.SaveData();
	bearHunterScoreSystem.Cleanup();
	if (Is_Script_Attached(obj,"JMG_Bear_Hunter_Game_Control"))
		Remove_Script(obj,"JMG_Bear_Hunter_Game_Control");
	Set_Nod_Soldier_Name("CnC_Nod_Minigunner_0");
	Set_GDI_Soldier_Name("CnC_GDI_MiniGunner_0");
	for (int x = 0;x < 7;x++)
		JMG_Bear_Hunter_Oil_Rig::pumpJacks[x] = JMG_Bear_Hunter_Oil_Rig::PumpjackNode(0);
	cleanUpNeeded = false;
}
GameObject *JMG_Bear_Hunter_Game_Control::MutantSpawn()
{
	const char *preset;
	if (gameTime > 900)// mutants
		preset = "Mutant_Bear";
	else if (gameTime > 625)// deer
		preset = (Commands->Get_Random(0.0f,1.0f)<0.5) ? "Mutant_Bear" : "Mutant_Deer";
	else if (gameTime > 400)//dogs
		preset = (Commands->Get_Random(0.0f,1.0f)<0.333) ? "Mutant_Bear" : (Commands->Get_Random(0.0,1.0f)<0.5f) ? "Mutant_Deer" : "Mutant_Dog";
	else if (gameTime > 330)//cat
		preset = (Commands->Get_Random(0.0f,1.0f)<0.25) ? ((Commands->Get_Random(0.0,1.0f)<0.5f) ? "Mutant_Bear" : "Mutant_Cat") : ((Commands->Get_Random(0.0,1.0f)<0.5f) ? "Mutant_Deer" : "Mutant_Dog");
	else if (gameTime > 210)//ion cats
		preset = (Commands->Get_Random(0.0f,1.0f)<0.25) ? ((Commands->Get_Random(0.0,1.0f)<0.5f) ? "Mutant_Bear" : ((Commands->Get_Random(0.0,1.0f)<0.6) ? "Mutant_Cat" : "Mutant_Cat_Blue")) : ((Commands->Get_Random(0.0,1.0f)<0.5f) ? "Mutant_Deer" : "Mutant_Dog");
	else if (gameTime > 150)// nuke cats
		preset = (Commands->Get_Random(0.0f,1.0f)<0.25) ? ((Commands->Get_Random(0.0,1.0f)<0.5f) ? "Mutant_Bear" : ((Commands->Get_Random(0.0,1.0f)<0.6) ? "Mutant_Cat" : (Commands->Get_Random(0.0,1.0)<0.8) ? "Mutant_Cat_Blue" : "Mutant_Cat_Crazy")) : ((Commands->Get_Random(0.0,1.0f)<0.5f) ? "Mutant_Deer" : "Mutant_Dog");
	else
		preset = ((Commands->Get_Random(0.0,1.0f)<0.05f) ? "Mutant_Moose" : (Commands->Get_Random(0.0f,1.0f)<0.25) ? ((Commands->Get_Random(0.0,1.0f)<0.5f) ? "Mutant_Bear" : ((Commands->Get_Random(0.0,1.0f)<0.6) ? "Mutant_Cat" : (Commands->Get_Random(0.0,1.0)<0.8) ? "Mutant_Cat_Blue" : "Mutant_Cat_Crazy")) : ((Commands->Get_Random(0.0,1.0f)<0.5f) ? "Mutant_Deer" : "Mutant_Dog"));
	Vector3 pos = BearHunterGameControlSystem.LookupNearestPosition(Vector2(Commands->Get_Random(-500.0f,500.0f),Commands->Get_Random(JmgUtility::MathClamp(CreatePositionZ-250,-600,500),CreatePositionZ)));
	GameObject *ai = Commands->Create_Object(preset,pos);
	Commands->Set_Facing(ai,Commands->Get_Random(-180,180));
	MoveablePhysClass *mphys = ai->As_PhysicalGameObj() ? ai->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
	if (mphys)
		mphys->Find_Teleport_Location(pos,2.0f,&pos);
	pos.Z += 1.0f;
	Commands->Set_Position(ai,pos);
	float maxHealth = Commands->Get_Max_Health(ai);
	float calculatedHealth = JmgUtility::MathClamp(maxHealth*(GamePercent*0.5f+Commands->Get_Random(0.0f,0.1f)),1.0f,maxHealth*(0.025f+Get_Player_Count()*0.075f));//0.025 easy
	Commands->Set_Health(ai,calculatedHealth);
	Set_Max_Health(ai,calculatedHealth*2.0f);
	mutantObjectListControl += ai;
	char params[512];
	sprintf(params,"%.2f",JmgUtility::MathClamp(JmgUtility::MathClamp(GamePercent*0.95f,0.35f,999.9f)+Commands->Get_Random(-0.1f,0.1f),0.35f,0.9f+Get_Player_Count()*0.01f));
	Attach_Script_Once(ai,"JMG_Bear_Hunt_Mutant_Attacker",params);
	Attach_Script_Once(ai,"JMG_Bear_Hunter_Mutant_Tracker","");
	Attach_Script_Once(ai,"JMG_Bear_Hunter_Animal_Control","");
	Attach_Script_Once(ai,"JMG_Bear_Hunter_Mutant_Respawn_Tracker","");
	return ai;
}
void JMG_Rp2_Hostile_Mutant_AI::Created(GameObject *obj)
{
	damaged = false;
	returnHome = 0;
	PlayerCount = 1;
	CanHearDelay = 0;
	LastSeen = 0;
	currentTargetID = 0;
	secondaryEnemyId = 0;
	huntEnemy = 0;
	waitcount = 0;
	vehicleChaseTime = 0;
	vehicleChaseTimeReset = 0;
	maxVehicleChaseTime = Commands->Get_Random_Int(300,900);
	returnHomeDelay = Commands->Get_Random_Int(10,90);
	lastPos = homelocation = Commands->Get_Position(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,5.0f,1);
	Commands->Start_Timer(obj,this,0.1f,2);
	Commands->Start_Timer(obj,this,0.25f,3);
	Commands->Start_Timer(obj,this,1.25f,5);
}
void JMG_Rp2_Hostile_Mutant_AI::Enemy_Seen(GameObject *obj,GameObject *seen)	
{
	if (seen->As_SmartGameObj() && seen->As_SmartGameObj()->Is_Stealthed() && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 900)
		return;
	if (Is_Script_Attached(seen,"JMG_Bear_Hunter_AI_Ignore"))
		return;
	if (Is_Script_Attached(seen,"JMG_Bear_Hunter_AI_Ignore_Up_To_75_Meters") && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 5625)
		return;
	if (Is_Script_Attached(seen,"JMG_Bear_Hunter_AI_Ignore_Up_To_150_Meters") && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 22500)
		return;
	if (Is_Script_Attached(seen,"JMG_AI_Ignore_Object"))
		return;
	if (Commands->Is_A_Star(seen) && JMG_Bear_Hunter_Game_Control::aiIgnorePlayers[JmgUtility::JMG_Get_Player_ID(seen)])
		return;
	if (vehicleChaseTimeReset && (seen->As_VehicleGameObj() || Get_Vehicle(seen)) && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 225)
		return;
	int SeenID = Commands->Get_ID(seen);
	if (!currentTargetID)
	{
		LastSeen = 30;
		huntEnemy = 1;
		currentTargetID = SeenID;
	}
	else if (currentTargetID == SeenID)
		LastSeen = 30;
	if (currentTargetID && !secondaryEnemyId && SeenID != currentTargetID)
		secondaryEnemyId = SeenID;
	if (SeenID == secondaryEnemyId)
		lastSeenSecondary = 30;
}
void JMG_Rp2_Hostile_Mutant_AI::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 91185423)
	{
		GameObject *Player = JmgUtility::FindNearestPlayer(Commands->Get_Position(obj));
		if (!Player)
			return;
		ActionParamsStruct params;
		params.Set_Movement(Commands->Get_Position(Player),1.0,10.0f,false);
		params.MovePathfind = true;
		Commands->Action_Goto(obj,params);
	}
}
void JMG_Rp2_Hostile_Mutant_AI::Timer_Expired(GameObject *obj,int number)
{
	if (2 == number)
	{
		if (CanHearDelay)
			CanHearDelay--;
		if (LastSeen)
		{
			LastSeen--;
			if (!LastSeen && currentTargetID)
			{
				Commands->Action_Reset(obj,10);
				currentTargetID = 0;
				GameObject *secondaryEnemy = Commands->Find_Object(secondaryEnemyId);
				if (secondaryEnemy && Commands->Get_Health(secondaryEnemy))
				{
					currentTargetID = secondaryEnemyId;
					secondaryEnemyId = 0;
					ActionParamsStruct params;
					Commands->Get_Action_Params(obj,params);
					params.Set_Movement(secondaryEnemy,1.0,1.4f,false);
					params.MovePathfind = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(secondaryEnemy)) < 25.0 ? false : true;
					Commands->Action_Goto(obj,params);
					params.Set_Basic(this,999,10);
					LastSeen = lastSeenSecondary;
				}
			}
		}
		if (lastSeenSecondary)
		{
			lastSeenSecondary--;
			if (!lastSeenSecondary)
				secondaryEnemyId = 0;
		}
		if (currentTargetID)
		{
			if (vehicleChaseTimeReset)
				vehicleChaseTimeReset--;
			GameObject *target = Commands->Find_Object(currentTargetID);
			if (target && target->As_VehicleGameObj() || Get_Vehicle(target))
			{
				vehicleChaseTime++;
				if (vehicleChaseTime >= maxVehicleChaseTime && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(target)) > 225)
				{
					vehicleChaseTime = 0;
					Commands->Action_Reset(obj,10);
					currentTargetID = 0;
					target = NULL;
					vehicleChaseTimeReset = Commands->Get_Random_Int(300,450);
				}
			}
			if (!target || Commands->Get_Health(target) == 0.0f)
			{
				waitcount = 0;
				currentTargetID = 0;
				Commands->Action_Reset(obj,10);
				GameObject *secondaryEnemy = Commands->Find_Object(secondaryEnemyId);
				if (secondaryEnemy && Commands->Get_Health(secondaryEnemy))
				{
					currentTargetID = secondaryEnemyId;
					secondaryEnemyId = 0;
					ActionParamsStruct params;
					Commands->Get_Action_Params(obj,params);
					params.Set_Movement(secondaryEnemy,1.0,1.4f,false);
					params.MovePathfind = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(secondaryEnemy)) < 25.0 ? false : true;
					Commands->Action_Goto(obj,params);
					params.Set_Basic(this,999,10);
					LastSeen = lastSeenSecondary;
				}
				else
				{
					returnHome = 0;
					returnHomeDelay = Commands->Get_Random_Int(1,30);
				}
			}
			else if (JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(target)) > 40000.0f && huntEnemy == 1)
			{
				currentTargetID = 0;
				waitcount = 0;
				if (Commands->Get_Random(0.0f,1.0f) < 0.66f)
					Commands->Action_Reset(obj,10);
			}
			else
			{
				waitcount++;
				if (waitcount > 2)
				{
					waitcount = 0;
					ActionParamsStruct params;
					Commands->Get_Action_Params(obj,params);
					params.Set_Movement(target,1.0,1.4f,false);
					params.MovePathfind = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(target)) < 25.0 ? false : true;
					Commands->Action_Goto(obj,params);
					params.Set_Basic(this,999,10);
					if (Get_Vehicle(target) || target->As_VehicleGameObj())
					{
						params.AttackCheckBlocked = false;
						params.Set_Attack(target,12.5,1,Get_Player_Count() < 10 ? true : false);
					}
					else
						params.Set_Attack(target,3.0f,1,Get_Player_Count() < 10 ? true : false);
					Commands->Action_Attack(obj,params);
				}
			}
		}
		Vector3 Pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(lastPos,Pos) < 0.1f && JmgUtility::SimpleDistance(lastPos,homelocation) > 25.0f)
		{
			returnHome++;
			if (returnHome > 100)
				Commands->Set_Position(obj,homelocation);
			else if (returnHome == 95 || returnHome == returnHomeDelay)
			{
				Commands->Action_Reset(obj,10);
				ActionParamsStruct params;
				params.Set_Basic(this,999,10);
				params.MovePathfind = true;
				params.Set_Movement(homelocation,1.0f,4.0f,false);
				Commands->Action_Goto(obj,params);
			}
		}
		else
			returnHome = 0;
		lastPos = Pos;
		Commands->Start_Timer(obj,this,0.1f,2);
	}
	if (1 == number)
	{
		GameObject *secondaryEnemy = Commands->Find_Object(secondaryEnemyId);
		if (secondaryEnemyId && (!secondaryEnemy || !Commands->Get_Health(secondaryEnemy)))
			secondaryEnemyId = 0;
		Commands->Start_Timer(obj,this,5.0f,1);
	}
	if (3 == number)
	{
		Vector3 myPos = Commands->Get_Position(obj);
		for (int x = 0;x < 128;x++)
			if (PlayerEmulatedSoundList[x].SoundType)
			{
				GameObject *Player = Get_GameObj(x);
				if (!Player || !Commands->Get_Health(Player) || Get_Player_Type(Player) == -4)
					continue;
				Vector3 Pos = Commands->Get_Position(Player);
				if (JmgUtility::SimpleDistance(Pos,myPos) <= PlayerEmulatedSoundList[x].Range)
				{
					CombatSound CS = CombatSound();
					CS.Creator = Player;
					CS.Position = Pos;
					CS.sound = PlayerEmulatedSoundList[x].SoundType;
					SoundDHeard(obj,CS);
				}
			}
		Commands->Start_Timer(obj,this,0.1f,3);
	}
	if (number == 5 && Commands->Get_Health(obj))
	{
		Vector3 pos = Commands->Get_Position(obj);
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player || Get_Vehicle(player) || Get_Player_Type(player) == -4)
				continue;
			if (JmgUtility::SimpleFlatDistance(pos,Commands->Get_Position(player)) <= 0.25 && JmgUtility::SimpleDistance(pos,Commands->Get_Position(player)) <= 25)
				Commands->Apply_Damage(player,9999.9f,"None",obj);
		}
		Commands->Start_Timer(obj,this,1.25f,5);
	}
}
void JMG_Rp2_Hostile_Mutant_AI::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damager || Commands->Get_Player_Type(damager) != 1)
		return;
	if (!Commands->Get_Health(damager))
		return;
	if (!damaged && damage > 0)
		damaged = true;
	if (Commands->Is_A_Star(damager) && JMG_Bear_Hunter_Game_Control::aiIgnorePlayers[JmgUtility::JMG_Get_Player_ID(damager)])
		return;
	if (Is_Script_Attached(damager,"JMG_AI_Ignore_Object"))
		return;
	GameObject *target = Commands->Find_Object(currentTargetID);
	if (target)
	{
		float targetDistance = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(target));
		float damagerDistance = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(damager));
		if (targetDistance < damagerDistance)
			return;
	}
	LastSeen = 30;
	huntEnemy = 0;
	waitcount = 0;
	currentTargetID = Commands->Get_ID(damager);
	if (damager->As_VehicleGameObj() || Get_Vehicle(damager))
		vehicleChaseTimeReset = 0;
}
void JMG_Rp2_Hostile_Mutant_AI::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason)
{
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{

		if (Commands->Get_Health(obj) <= 0)
			return;
		GameObject *target = Commands->Find_Object(currentTargetID);
		int playerId = JmgUtility::JMG_Get_Player_ID(target);
		if (playerId && Commands->Get_Health(target))
			JMG_Bear_Hunter_Game_Control::aiIgnorePlayers[playerId] = 30;
	}
}
void JMG_Rp2_Hostile_Mutant_AI::SoundDHeard(GameObject *obj,const CombatSound &sound)
{
	if (!sound.Creator)
		return;
	if (!Commands->Is_A_Star(sound.Creator) || CanHearDelay || currentTargetID)// was 225 
		return;
	Vector3 MyPos = Commands->Get_Position(obj);
	float dist = JmgUtility::SimpleDistance(sound.Position,MyPos);
	bool RandSucess = Commands->Get_Random(0.00f,1.00f) < 0.0000025;
	bool Success = false;//(dist > 225 && Commands->Get_Random(0.00f,1.00f) > 0.015)fail
	if (dist < 144)// Sound is near, must investigate
		Success = true;
	else if (sound.sound == 6 && dist < 250000 && Commands->Get_Random(0.00f,1.00f) < JMG_Bear_Hunter_Game_Control::soundAttentionWhoreNotify)//player just wants to be noticed
		Success = true;
	else if (Commands->Get_Random(0.00f,1.00f) < 0.00025 && sound.sound != 5)// Bad Luck
		Success = true;
	else if (sound.sound == 5 && Commands->Get_Random(0.00f,1.00f) < 0.0015)// Sound is bullet hits
		Success = true;
	else if (dist < 2025 && sound.sound != 5)// Sound is near and not bullet hits must check
		Success = true;
	else if (dist < 8100 && sound.sound != 5 && Commands->Get_Random(0.00f,1.00f) < 0.0025)// Sound is near and not bullet hits decided to check
		Success = true;
	else if (dist < 90000 && sound.sound != 5 && JMG_Rp2_Hostile_Mutant_AI::CanInvestigateSound)
		if (RandSucess && JmgUtility::SimpleDistance(JMG_Rp2_Hostile_Mutant_AI::AllowedSoundPos,Vector3(0.0f,0.0f,0.0f)) < 0.1f)// Sound is really close but player is far away
			Success = true;// Sound is far away but chances allowed it to successfully fall into the special "Everyone hates you" condition
		else if (JmgUtility::SimpleDistance(JMG_Rp2_Hostile_Mutant_AI::AllowedSoundPos,sound.Position) < 0.1f)
			Success = true;// Also heard the "Everyone hates you sound
	if (!Success)
		return;
	CanHearDelay = Commands->Get_Random_Int(30,90);
	ActionParamsStruct params;
	Commands->Get_Action_Params(obj,params);
	params.Set_Movement(Commands->Get_Position(sound.Creator),1.0f,sound.sound == 6 ? 25.0f : 1.4f,false);
	params.MovePathfind = true;
	if (JMG_Rp2_Hostile_Mutant_AI::CanInvestigateSound && JmgUtility::SimpleDistance(JMG_Rp2_Hostile_Mutant_AI::AllowedSoundPos,Vector3(0.0f,0.0f,0.0f)) < 0.1f && RandSucess)
		JMG_Rp2_Hostile_Mutant_AI::AllowedSoundPos = sound.Position;
	Commands->Action_Goto(obj,params);
}
void JMG_Bear_Hunt_Mutant_Attacker::Created(GameObject *obj)
{
	targetedId = 0;
	targetDistance = 0.0f;
	targetUpdate = 0;
	lastPos = Commands->Get_Position(obj);
	stuckCount = 0;
	ignoreDamage = false;
	ignoreEnemies = false;
	weaponRange = 3.3489f;
	attackRange = 1.547f;
	myNode = mutantObjectListControl.find(obj);
	myNode->enemyId = 0;
	Timer_Expired(obj,2);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,0.1f,1);
	Commands->Start_Timer(obj,this,180.0f,3);
	Commands->Start_Timer(obj,this,2.5f,5);
}
void JMG_Bear_Hunt_Mutant_Attacker::Enemy_Seen(GameObject *obj,GameObject *seen)	
{
	if (seen->As_SmartGameObj() && seen->As_SmartGameObj()->Is_Stealthed() && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 900)
		return;
	if (Is_Script_Attached(seen,"JMG_Bear_Hunter_AI_Ignore"))
		return;
	if (Is_Script_Attached(seen,"JMG_Bear_Hunter_AI_Ignore_Up_To_75_Meters") && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 5625)
		return;
	if (Is_Script_Attached(seen,"JMG_Bear_Hunter_AI_Ignore_Up_To_150_Meters") && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 22500)
		return;
	if (Is_Script_Attached(seen,"JMG_AI_Ignore_Object"))
		return;
	if (myNode->enemyId && targetUpdate)
		return;
	if (ignoreEnemies)
	{
		int presetId = Commands->Get_Preset_ID(seen);
		if (presetId != 1000001055 && presetId != 1000001536 && presetId != 1000001688 && presetId != 1000001690 && presetId != 1000001820)
			return;
	}
	if (myNode->enemyId && JmgUtility::SimpleDistance(Commands->Get_Position(seen),Commands->Get_Position(obj)) > targetDistance)
		return;
	int playerId = JmgUtility::JMG_Get_Player_ID(seen);
	if (playerId && JMG_Bear_Hunter_Game_Control::aiIgnorePlayers[JmgUtility::JMG_Get_Player_ID(seen)])
		return;
	 int SeenID = Commands->Get_ID(seen);
	 targetUpdate = 40;
	if (!myNode->enemyId)
	{
		LastSeen = seen->As_VehicleGameObj() && seen->As_VehicleGameObj()->Peek_Physical_Object()->As_DecorationPhysClass() ? -1 : 60;
		myNode->enemyId = SeenID;
	}
	else if (myNode->enemyId == SeenID)
		LastSeen = seen->As_VehicleGameObj() && seen->As_VehicleGameObj()->Peek_Physical_Object()->As_DecorationPhysClass() ? -1 : 60;
	if (myNode->enemyId && !secondaryEnemyId && SeenID != myNode->enemyId)
		secondaryEnemyId = SeenID;
	if (SeenID == secondaryEnemyId)
		lastSeenSecondary = 12;
}
void JMG_Bear_Hunt_Mutant_Attacker::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!Commands->Get_Health(obj))
			return;
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *target = myNode->enemyId ? Commands->Find_Object(myNode->enemyId) : NULL;
		if (!target || !Commands->Get_Health(target) || Commands->Get_Player_Type(target) == -2)
		{
			myNode->enemyId = 0;
			GameObject *mainObject = NULL;
			if (Commands->Get_Random(0.0f,1.0f) < Get_Player_Count()*0.05)
			{
				mainObject = Commands->Find_Object(JMG_Bear_Hunter_Radio_Tower_Switch::supportId);
				myNode->enemyId = Commands->Get_ID(mainObject);
			}
			else if (Commands->Get_Random(0.0f,1.0f) < JMG_Bear_Hunter_Game_Control::attackSubstationChance)
			{
				mainObject = Commands->Find_Object(Commands->Get_Random(0.0f,1.0f) < 0.9f ? JMG_Bear_Hunter_Power_Transformer::mctIds[0] : JMG_Bear_Hunter_Power_Transformer::mctIds[1]);
				myNode->enemyId = Commands->Get_ID(mainObject);
			}
			else
				mainObject = Commands->Find_Object(JMG_Bear_Hunter_Game_Control::mutantTargetId);
			if (mainObject)
				AttackTarget(obj,mainObject,JmgUtility::SimpleDistance(Commands->Get_Position(mainObject),pos));
		}
		else if (target)
		{
			targetDistance = JmgUtility::SimpleDistance(pos,Commands->Get_Position(target));
			if (targetDistance >= 160000)
				myNode->enemyId = 0;
			AttackTarget(obj,target,targetDistance);
		}
		if (targetUpdate)
			targetUpdate--;
		float dist = JmgUtility::SimpleDistance(pos,lastPos);
		if (dist < 1.0f)	
		{
			stuckCount++;
			if (stuckCount == 25)
			{
				targetedId = 0;
				Commands->Action_Reset(obj,10);
			}
			if (stuckCount > 50)
				if (JMG_Bear_Hunter_Game_Control::gameState != JMG_Bear_Hunter_Game_Control::SecureBase)
					Commands->Apply_Damage(obj,9999.9f,"BlamoKiller",obj);
				else
				{
					stuckCount = 40;
					Vector3 pos = Commands->Get_Position(obj);
					pos.Z += 1.25f;
					MoveablePhysClass *mphys = obj->As_PhysicalGameObj() ? obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
					if (mphys && mphys->Can_Teleport(Matrix3D(pos)))
						Commands->Set_Position(obj,pos);
				}
		}
		else
		{
			stuckCount = 0;
			lastPos = pos;
		}
		if (LastSeen > 0)
		{
			LastSeen--;
			if (!LastSeen && myNode->enemyId)
			{
				Commands->Action_Reset(obj,10);
				myNode->enemyId = 0;
				GameObject *secondaryEnemy = Commands->Find_Object(secondaryEnemyId);
				if (secondaryEnemy && Commands->Get_Health(secondaryEnemy))
				{
					myNode->enemyId = secondaryEnemyId;
					secondaryEnemyId = 0;
					ActionParamsStruct params;
					Commands->Get_Action_Params(obj,params);
					params.Set_Movement(secondaryEnemy,1.0,1.4f,false);
					params.MovePathfind = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(secondaryEnemy)) < 25.0 ? false : true;
					Commands->Action_Goto(obj,params);
					params.Set_Basic(this,999,10);
					LastSeen = lastSeenSecondary;
				}
			}
		}
		if (lastSeenSecondary)
		{
			lastSeenSecondary--;
			if (!lastSeenSecondary)
				secondaryEnemyId = 0;
		}
		Commands->Start_Timer(obj,this,0.25f,1);
	}
	if (number == 2)
	{
		if (!JMG_Bear_Hunter_Game_Control::gameTime)
			ignoreEnemies = true;
		else
			ignoreEnemies = Commands->Get_Random(0.0f,1.0f) < ((JMG_Bear_Hunter_Game_Control::gameTime < 100) ? 0.75f : (JMG_Bear_Hunter_Game_Control::gameTime < 400) ? 0.33f : (JMG_Bear_Hunter_Game_Control::gameTime < 600) ? 0.25f : (JMG_Bear_Hunter_Game_Control::gameTime < 875) ? 0.15f : 0.0f) ? true : false;
		Commands->Start_Timer(obj,this,Commands->Get_Random(30,60),2);
	}
	if (number == 3)
	{
		Commands->Apply_Damage(obj,999.9f,"Blamokiller",obj);
	}
	if (number == 4)
	{
		obj->As_PhysicalGameObj()->Peek_Physical_Object()->Set_Collision_Group(HOVER_UNIT_COLLISION_GROUP);
	}
	if (number == 5)
	{
		if (!JMG_Bear_Hunter_Power_Transformer::online && myNode && (myNode->enemyId == JMG_Bear_Hunter_Power_Transformer::mctIds[0] || myNode->enemyId == JMG_Bear_Hunter_Power_Transformer::mctIds[1]))
		{
			myNode->enemyId = 0;
			Commands->Action_Reset(obj,100);
		}
		Commands->Start_Timer(obj,this,2.5f,5);
	}
}
void JMG_Bear_Hunt_Mutant_Attacker::AttackTarget(GameObject *obj,GameObject *target,float distance)
{
	int tempId = Commands->Get_ID(target);
	if (targetedId == tempId)
		return;
	targetedId = tempId;
	ActionParamsStruct params;
	Commands->Get_Action_Params(obj,params);
	params.Set_Movement(target,Get_Float_Parameter("Speed"),attackRange,false);
	params.MovePathfind = distance < 25 ? false : true;
	Commands->Action_Goto(obj,params);
	params.Set_Basic(this,100,10);
	if (distance < weaponRange)
		params.AttackForceFire = true;
	else
		params.AttackForceFire = false;
	if (Get_Vehicle(target) || target->As_VehicleGameObj())
	{
		params.AttackCheckBlocked = false;
		params.Set_Attack(target,7.5f,0.0f,true);
	}
	else
		params.Set_Attack(target,3.0f,0.0f,true);
	Commands->Action_Attack(obj,params);
}
void JMG_Bear_Hunt_Mutant_Attacker::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage > 0 && !damager || obj == damager || (myNode->enemyId && targetUpdate) || myNode->enemyId == Commands->Get_ID(damager) || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	if (JMG_Bear_Hunter_Game_Control::aiIgnorePlayers[JmgUtility::JMG_Get_Player_ID(damager)])
		return;
	if (Is_Script_Attached(damager,"JMG_AI_Ignore_Object"))
		return;
	float enemyDist = 0;
	int enemyId = Commands->Get_ID(damager);
	if ((myNode->enemyId && myNode->enemyId != enemyId) || ignoreDamage)
		enemyDist = JmgUtility::SimpleDistance(Commands->Get_Position(damager),Commands->Get_Position(obj));
	if (myNode->enemyId && myNode->enemyId != enemyId && enemyDist > targetDistance)
		return;
	if (ignoreDamage && enemyDist > 56.25)
		return;
	if (!_stricmp(Get_Skin(obj),"Blamo"))
		return;
	 myNode->enemyId = Commands->Get_ID(damager);
	 targetUpdate = 40;
}
void JMG_Bear_Hunt_Mutant_Attacker::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason)
{
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{
		GameObject *target = Commands->Find_Object(myNode->enemyId);
		myNode->enemyId = 0;
		if (Commands->Get_Health(obj) <= 0)
			return;
		int playerId = JmgUtility::JMG_Get_Player_ID(target);
		if (playerId && Commands->Get_Health(target))
			JMG_Bear_Hunter_Game_Control::aiIgnorePlayers[playerId] = 15;
		GameObject *mainObject = Commands->Find_Object(JMG_Bear_Hunter_Game_Control::mutantTargetId);
		if (mainObject)
			AttackTarget(obj,mainObject,JmgUtility::SimpleDistance(Commands->Get_Position(mainObject),Commands->Get_Position(obj)));
	}
}
void JMG_Bear_Hunter_Animal_Control::Killed(GameObject *obj,GameObject *killer)
{
	/*if (killer && killer != obj && !Commands->Is_A_Star(killer))
		Set_Team_Kills(Get_Player_Type(killer) == 1 ? 1 : 0,Get_Team_Kills(Get_Player_Type(killer) == 1 ? 1 : 0)+1);
	Set_Team_Deaths(0,Get_Team_Deaths(0)+1);*/
	if (!Commands->Is_A_Star(killer))
	{
		int killerId = Commands->Get_ID(killer);
		if (Is_Script_Attached(killer,"JMG_Bear_Hunter_Player_Assist_AI"))
			for (int x = 0;x < maxTotalBearHunterPlayerAssistAI;x++)
				if (JMG_Bear_Hunter_Player_Assist_AI::aiIds[x] == killerId)
				{
					JMG_Bear_Hunter_Player_Assist_AI::cash[x] += Get_Death_Points(obj)*2.0f;
					break;
				}
		return;
	}
	int playerId = JmgUtility::JMG_Get_Player_ID(killer);
	PlayerData.players[playerId]->killsWithoutGettingHurt++;
	if (!(PlayerData.players[playerId]->killsWithoutGettingHurt % 10))
	{
		Commands->Give_Money(killer,50.0f,false);
		JmgUtility::DisplayChatMessage(killer,11,194,255,"San Casina Rug Co: Congrats, here's your $50.00 for 10 in a row without being hurt!");
	}
	PlayerData.players[playerId]->totalKills++;
	if (!(PlayerData.players[playerId]->totalKills % 250))
	{
		Commands->Give_Money(killer,1000.0f,false);
		JmgUtility::DisplayChatMessage(killer,11,194,255,"San Casina Rug Co: $1,000.00 for the 250 pelts!");
	}
	if (Commands->Get_Random(0.0f,1.0f) < (1.0f-JMG_Bear_Hunter_Game_Control::gameTime/2000.0f)*0.25f)
	{
		GameObject *powerup = NULL;
		float random = Commands->Get_Random(0.0,1.0),random2 = Commands->Get_Random(0.0f,1.0f);
		if (random < 0.3f)
			Create_Powerup(powerup,obj,random2,"POW_Money_1","POW_Money_2","POW_Money_3");
		else if (random < 0.55f)
			Create_Powerup(powerup,obj,random2,"POW_Armor_1","POW_Armor_2","POW_Armor_3");
		else if (random < 0.8f)
			Create_Powerup(powerup,obj,random2,"POW_Health_1","POW_Health_2","POW_Health_3");
		else
			Create_Powerup(powerup,obj,random2,"POW_Ammo_1","POW_Ammo_2","POW_Ammo_3");
	}
}
void JMG_Bear_Hunter_Animal_Control::Create_Powerup(GameObject *powerup,GameObject *obj,float random,const char *powerup1,const char *powerup2,const char *powerup3)
{
	Vector3 pos = Commands->Get_Position(obj);
	pos.Z += 0.25f;
	if (random < 0.6f)
		powerup = Commands->Create_Object(powerup1,pos);
	else if (random < 0.9f)
		powerup = Commands->Create_Object(powerup2,pos);
	else
		powerup = Commands->Create_Object(powerup3,pos);
	Commands->Attach_Script(powerup,"JMG_Bear_Hunter_Powerup_Tracker","");
	Commands->Set_Facing(powerup,Commands->Get_Facing(obj));
}
void JMG_Security_Camera_Behavior::Created(GameObject *obj)
{
	zHeight = 1000.0f*tan(Get_Float_Parameter("IdleAimZAngleModifier")*PI180);
	enabled = true;
	SeenTime = 0;
	EnemyID = 0;
	GameObject *floodLight = Commands->Create_Object_At_Bone(obj,Get_Parameter("FloodLightPreset"),"headlight00");
	if (floodLight)
	{
		Commands->Disable_All_Collisions(floodLight);
		Commands->Attach_To_Object_Bone(floodLight,obj,"headlight00");
		floodLightId = Commands->Get_ID(floodLight);
	}
	else
		floodLightId = 0;
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Enable_Hibernation(obj,false);
	Commands->Start_Timer(obj,this,0.25f,1);
}
void JMG_Security_Camera_Behavior::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	int seenID = Commands->Get_ID(seen);
	if (!EnemyID)
	{
		Commands->Create_Sound(Get_Parameter("Alarm_Sound"),Commands->Get_Position(obj),obj);
		SeenTime = 10;
		EnemyID = seenID;
		ActionParamsStruct params;
		params.Set_Basic(this,70,1);
		if (Get_Int_Parameter("Is_Gun"))
			params.Set_Attack(seen,Get_Float_Parameter("Delay") ? 0.0f : 9999.0f,0,true);
		else
			params.Set_Attack(seen,0.0f,0,true);
		Commands->Action_Attack(obj,params);
		if (!Get_Float_Parameter("Delay"))
		{
			if (Get_Int_Parameter("Alarm_ID") && Get_Int_Parameter("Alarm_Message"))
			{
				GameObject *target = Commands->Find_Object(Get_Int_Parameter("Alarm_ID"));
				Commands->Send_Custom_Event(obj,target,Get_Int_Parameter("Alarm_Message"),EnemyID,0);
			}
		}
		else
			Commands->Send_Custom_Event(obj,obj,47498612,EnemyID,Get_Float_Parameter("Delay"));
	}
	else if (seenID == EnemyID)
		SeenTime = 10;
}
void JMG_Security_Camera_Behavior::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 47498612 && EnemyID == param)
	{
		if (!enabled)
			return;
		if (Get_Int_Parameter("Is_Gun"))
		{
			GameObject *seen = Commands->Find_Object(EnemyID);
			if (seen)
			{
				ActionParamsStruct params;
				params.Set_Basic(this,70,1);
				params.Set_Attack(seen,9999.0f,0,true);
				params.AttackForceFire = true;
				Commands->Action_Attack(obj,params);
			}
		}
		if (Get_Int_Parameter("Alarm_ID") && Get_Int_Parameter("Alarm_Message"))
		{
			GameObject *target = Commands->Find_Object(Get_Int_Parameter("Alarm_ID"));
			Commands->Send_Custom_Event(obj,target,Get_Int_Parameter("Alarm_Message"),EnemyID,0);
			Commands->Create_Sound(Get_Parameter("Alarm_Sound"),Commands->Get_Position(obj),obj);
			Commands->Send_Custom_Event(obj,obj,47498613,EnemyID,Get_Float_Parameter("Delay"));
		}
	}
	if (message == 47498613 && EnemyID == param)
	{
		if (!enabled)
			return;
		if (Get_Int_Parameter("Alarm_ID") && Get_Int_Parameter("Alarm_Message"))
		{
			GameObject *target = Commands->Find_Object(Get_Int_Parameter("Alarm_ID"));
			Commands->Send_Custom_Event(obj,target,Get_Int_Parameter("Alarm_Message"),EnemyID,0);
			Commands->Create_Sound(Get_Parameter("Alarm_Sound"),Commands->Get_Position(obj),obj);
			Commands->Send_Custom_Event(obj,obj,47498613,EnemyID,Get_Float_Parameter("Delay"));
		}
	}
	if (message == Get_Int_Parameter("Power_Message"))
	{
		if (enabled && !param)
		{
			enabled = false;
			originalTeam = Commands->Get_Player_Type(obj);
			Commands->Set_Player_Type(obj,-2);
			Commands->Action_Reset(obj,90);
			GameObject *floodLight = Commands->Find_Object(floodLightId);
			if (floodLight)
				Commands->Destroy_Object(floodLight);
			Commands->Enable_Enemy_Seen(obj,false);
		}
		if (!enabled && param)
		{
			enabled = true;
			Commands->Set_Player_Type(obj,originalTeam);
			GameObject *floodLight = Commands->Create_Object_At_Bone(obj,Get_Parameter("FloodLightPreset"),"headlight00");
			if (floodLight)
			{
				Commands->Attach_To_Object_Bone(floodLight,obj,"headlight00");
				floodLightId = Commands->Get_ID(floodLight);
			}
			Commands->Enable_Enemy_Seen(obj,true);
		}
	}
}
void JMG_Security_Camera_Behavior::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 Pos = Commands->Get_Position(obj);
		float Angle = Get_Float_Parameter("Angle")/4;
		float Facing = Commands->Get_Facing(obj);
		CameraFacingCount = 0;
		IncreaseOrDecreaseCount = 1;
		Facing = Facing-(Angle*2);
		float ZPos = Commands->Get_Bone_Position(obj,"MuzzleA0").Z+zHeight;
		float TempFacing = Facing*PI180;
		CameraFacingLocation[0] = Pos;
		CameraFacingLocation[0].X += cos(TempFacing)*1000.0f;
		CameraFacingLocation[0].Y += sin(TempFacing)*1000.0f;
		CameraFacingLocation[0].Z = ZPos;
		Facing = Facing+Angle;
		TempFacing = Facing*PI180;
		CameraFacingLocation[1] = Pos;
		CameraFacingLocation[1].X += cos(TempFacing)*1000.0f;
		CameraFacingLocation[1].Y += sin(TempFacing)*1000.0f;
		CameraFacingLocation[1].Z = ZPos;
		Facing = Facing+Angle;
		TempFacing = Facing*PI180;
		CameraFacingLocation[2] = Pos;
		CameraFacingLocation[2].X += cos(TempFacing)*1000.0f;
		CameraFacingLocation[2].Y += sin(TempFacing)*1000.0f;
		CameraFacingLocation[2].Z = ZPos;
		Facing = Facing+Angle;
		TempFacing = Facing*PI180;
		CameraFacingLocation[3] = Pos;
		CameraFacingLocation[3].X += cos(TempFacing)*1000.0f;
		CameraFacingLocation[3].Y += sin(TempFacing)*1000.0f;
		CameraFacingLocation[3].Z = ZPos;
		Facing = Facing+Angle;
		TempFacing = Facing*PI180;
		CameraFacingLocation[4] = Pos;
		CameraFacingLocation[4].X += cos(TempFacing)*1000.0f;
		CameraFacingLocation[4].Y += sin(TempFacing)*1000.0f;
		CameraFacingLocation[4].Z = ZPos;
		CameraFacingUpdateTime = Get_Float_Parameter("Angle")*0.0085f;
		if (CameraFacingUpdateTime < 0.01f)
			CameraFacingUpdateTime = 0.1f;
		Commands->Start_Timer(obj,this,0.0f,2);
		Commands->Start_Timer(obj,this,0.1f,3);
	}
	if (number == 2)
	{
		if (!enabled)
		{
			Commands->Start_Timer(obj,this,CameraFacingUpdateTime*Get_Float_Parameter("UpdateRateMultiplier"),2);
			return;
		}
		if (!EnemyID)
		{
			CameraFacingCount += IncreaseOrDecreaseCount;
			if (CameraFacingCount == 0 || CameraFacingCount == 4)
				IncreaseOrDecreaseCount = IncreaseOrDecreaseCount == -1 ? 1 : -1;
			ActionParamsStruct params;
			params.Set_Basic(this,70,1);
			params.Set_Attack(CameraFacingLocation[CameraFacingCount],0,0,true);
			Commands->Action_Attack(obj,params);
		}
		Commands->Start_Timer(obj,this,CameraFacingUpdateTime*Get_Float_Parameter("UpdateRateMultiplier"),2);
	}
	if (number == 3)
	{
		if (!enabled)
		{
			Commands->Start_Timer(obj,this,0.1f,3);
			return;
		}
		if (SeenTime)
		{
			SeenTime--;
			if (!SeenTime)
				EnemyID = 0;
		}
		if (EnemyID)
		{
			GameObject *enemy = Commands->Find_Object(EnemyID);
			if (!enemy || !Commands->Get_Health(enemy))
			{
				EnemyID = 0;
				SeenTime = 0;
			}
		}
		Commands->Start_Timer(obj,this,0.1f,3);
	}
}
void JMG_Security_Camera_Behavior::Destroyed(GameObject *obj)
{
	GameObject *floodLight = Commands->Find_Object(floodLightId);
	if (floodLight)
		Commands->Destroy_Object(floodLight);
}
void JMG_Bear_Hunt_Mutant_Cat_Explosion::Killed(GameObject *obj,GameObject *killer)
{
	if (!killer || killer == obj)
		Commands->Create_Explosion(Get_Parameter("KillSelfExplosion"),Commands->Get_Position(obj),obj);
	else
		Commands->Create_Explosion(Get_Parameter("KilledExplosion"),Commands->Get_Position(obj),killer);
}
void JMG_Bear_Hunter_President_Controller::Created(GameObject *obj)
{
	regenTime = 15;
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
	hurtRecently = 0;
	lastHPRecorded = 500.0;
	lastDisplayHealth = 0;
	lastTime = clock();
	Commands->Set_Player_Type(obj,1);
	Commands->Set_Animation(obj,"s_a_human.h_a_a0a0",true,0,0,-1,true);
	Commands->Attach_Script(obj,"JMG_Bear_Hunter_Engineer_Ignore","");
	JMG_Bear_Hunter_Game_Control::mutantTargetId = Commands->Get_ID(obj);
	Commands->Start_Timer(obj,this,Commands->Get_Random(7.5f,22.5f),1);
	Commands->Start_Timer(obj,this,1.0f,2);
	Commands->Start_Timer(obj,this,0.1f,3);
}
void JMG_Bear_Hunter_President_Controller::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		const char *animations[18] = {"s_a_human.h_a_a0a0_l01","s_a_human.h_a_a0a0_l02","s_a_human.h_a_a0a0_l03","s_a_human.h_a_a0a0_l04","s_a_human.h_a_a0a0_l05","s_a_human.h_a_a0a0_l06","s_a_human.h_a_a0a0_l12","s_a_human.h_a_a0a0_l13","s_a_human.h_a_a0a0_l15","s_a_human.h_a_a0a0_l41","s_a_human.h_a_a0a0_l42","s_a_human.h_a_a0a0_l70","s_a_human.h_a_j11c","s_a_human.h_a_j15c","s_a_human.h_a_j17c","s_a_human.h_a_j22c","s_a_human.h_a_j24c","s_a_human.h_a_x33c"};
		if (!injuredAnimation)
			Commands->Set_Animation(obj,animations[Commands->Get_Random_Int(0,18)],false,0,0,-1,true);
		Commands->Start_Timer(obj,this,Commands->Get_Random(7.5f,22.5f),1);
	}
	if (number == 2)
	{
		if (hurtRecently)
			hurtRecently--;
		if (!hurtRecently && !mutantObjectListControl.countInRangeTargetingId(Commands->Get_Position(obj),10.0f,JMG_Bear_Hunter_Game_Control::mutantTargetId) && JMG_Bear_Hunter_Game_Control::gameState != JMG_Bear_Hunter_Game_Control::BossArrive)
		{
			lastHPRecorded = JmgUtility::MathClampDouble(lastHPRecorded+1.0,0.0,500.0);
			Commands->Set_Shield_Strength(obj,(float)lastHPRecorded);
			damagedEvent(obj,false,false);
		}
		Commands->Start_Timer(obj,this,1.0f,2);
	}
	if (number == 3)
	{
		int enemyCount = mutantObjectListControl.countInRangeTargetingId(Commands->Get_Position(obj),10.0f,JMG_Bear_Hunter_Game_Control::mutantTargetId);
		double calculatedDamageToApply = enemyCount*0.2125f;//0.425
		calculatedDamageToApply += enemyCount ? 0.02*JmgUtility::MathClampInt(Get_Player_Count(),1,128) : 0.0;
		if (calculatedDamageToApply)
		{
			JMG_Bear_Hunter_Game_Control::hasBeenInjured = true;
			lastHPRecorded = JmgUtility::MathClampDouble(lastHPRecorded-calculatedDamageToApply,0.0,500.0);
			Commands->Set_Shield_Strength(obj,(float)lastHPRecorded);
			damagedEvent(obj,true,true);
		}
		Commands->Start_Timer(obj,this,0.1f,3);
	}
}
void JMG_Bear_Hunter_President_Controller::Poked(GameObject *obj, GameObject *poker)
{
	int playerId = JmgUtility::JMG_Get_Player_ID(poker);
	if (!playerId)
		return;
	if (Commands->Get_Player_Type(poker) == -4)
		return;
	if (Is_Script_Attached(poker,"JMG_Prez_Medical_Needle_Player_Control"))
	{
		NewObjectiveSystemControl.Set_Objective_Status(22,NewObjectiveSystem::Accomplished);
		BearHunterScoreSystem::BHScoreNode *playerScoreNode = bearHunterScoreSystem.Get_Current_Player_Score_Node(playerId);
		if (playerScoreNode)
			playerScoreNode->ReturnedMedicalNeedle++;
		JmgUtility::DisplayChatMessage(poker,200,127,255,"President of Corporate America: Thanks!");
		JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Good work, he'll feel better in no time!");
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player)
				continue;
			Commands->Give_Money(player,1000.0f,0);
		}
		const char *weap = Get_Current_Weapon(poker);
		if (weap && !_stricmp(weap,"Weapon_Needle"))
			Commands->Select_Weapon(poker,"Weapon_Pistol_Player");
		Remove_Weapon(poker,"Weapon_Needle");
		Remove_Script(poker,"JMG_Prez_Medical_Needle_Player_Control");
		regenTime = 5;
		return;
	}
	if ((PlayerData.players[playerId]->talkToPrezTime && difftime(clock(),PlayerData.players[playerId]->talkToPrezTime) < 10000.0f) || (JMG_Bear_Hunter_Game_Control::gameState == JMG_Bear_Hunter_Game_Control::BossArrive && !JMG_Bear_Hunter_Game_Control::playerAlive[playerId]))
		return;
	PlayerData.players[playerId]->talkToPrezTime = clock();

	if (JMG_Bear_Hunter_Game_Control::gameState == JMG_Bear_Hunter_Game_Control::LeaveBase)
		Greeting(poker);
	else if (JMG_Bear_Hunter_Game_Control::gameState == JMG_Bear_Hunter_Game_Control::BossArrive)
		Rabbit(poker);
	else if (JMG_Bear_Hunter_Game_Control::gameTime)
	{
		float percentOfHealth = Get_Hitpoints(obj)/Get_Max_Hitpoints(obj);
		if (percentOfHealth > 0.66)
			Greeting(poker);
		else if (percentOfHealth > 0.33)
			InPain(poker);
		else
			Critical(poker);
	}
	else
		Congrats(poker);
}
void JMG_Bear_Hunter_President_Controller::Animation_Complete(GameObject* obj,const char *anim)
{
	injuredAnimation = false;
	Commands->Set_Animation(obj,"s_a_human.h_a_a0a0",true,0,0,-1,true);
}
void JMG_Bear_Hunter_President_Controller::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damage)
		return;
	if (damage < 0)
	{
		Commands->Set_Shield_Strength(obj,(float)lastHPRecorded);
		return;
	} 
	int playerId = JmgUtility::JMG_Get_Player_ID(damager);
	if (playerId || JMG_Bear_Hunter_Game_Control::gameState == JMG_Bear_Hunter_Game_Control::BossArrive) 
	{
		lastHPRecorded = JmgUtility::MathClampDouble(lastHPRecorded-damage,0.0,500.0);
		Commands->Set_Shield_Strength(obj,(float)lastHPRecorded);
		damagedEvent(obj,false,true);
		if (!playerId)
			return;
		if (difftime(clock(),PlayerData.players[playerId]->friendlyFireTime) > 10000.0f)
		{
			PlayerData.players[playerId]->friendlyFireTime = clock();
			JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Friendly fire!");
			PlayerData.players[playerId]->friendlyFireCount++;
			if (!(PlayerData.players[playerId]->friendlyFireCount % 3))
			{
				char attackmsg[220];
				sprintf(attackmsg,"President of Corporate America: Help! %s keeps attacking me!",Get_Player_Name(damager));
				JmgUtility::MessageAllPlayers(200,127,255,attackmsg);
			}
		}
		return;
	}
	Commands->Set_Shield_Strength(obj,(float)lastHPRecorded);
}
void JMG_Bear_Hunter_President_Controller::Killed(GameObject *obj,GameObject *killer)
{
	 if (JMG_Bear_Hunter_Game_Control::gameOver)
		 return;
	JMG_Bear_Hunter_Game_Control::gameOver = true;
	bearHunterScoreSystem.Get_Current_Player_Score_Node(JmgUtility::JMG_Get_Player_ID(killer))->KilledPresident++;
	for (int x = 1;x < 128;x++)
	{
		GameObject *player = Get_GameObj(x);
		if (!player)
			continue;
		Change_Team_2(player,1);
	}
	bearHunterScoreSystem.EndGameUpdatePlayerStats(false);
	Console_Input("win 0");
	for (int x = 0;x < 5;x++)
		Commands->Create_Sound("Mission_Failed",Commands->Get_Position(obj),obj);
}
void JMG_Bear_Hunter_President_Controller::damagedEvent(GameObject *obj,bool announce,bool palyinjured)
{
	if (lastHPRecorded <= 0.0f)
		Commands->Apply_Damage(obj,999.99f,"BlamoKiller",0);
	int newFrame = (int)((1-lastHPRecorded/500.0f)*4.0);
	if (palyinjured)
		hurtRecently = regenTime;
	if (newFrame != lastDisplayHealth)
	{
		for (int x = 0;x < 20;x++)
			Commands->Static_Anim_Phys_Goto_Frame(pctObjectArray[x].id,(float)newFrame,pctObjectArray[x].animation);
		lastDisplayHealth = newFrame;
	}
	if (palyinjured && !injuredAnimation)
	{
		injuredAnimation = true;
		const char *animations[13] = {"s_a_human.h_a_811A","s_a_human.h_a_812A","s_a_human.h_a_821A","s_a_human.h_a_822A","s_a_human.h_a_831A","s_a_human.h_a_832A","s_a_human.h_a_841A","s_a_human.h_a_842A","s_a_human.h_a_851A","s_a_human.h_a_852A","s_a_human.h_a_861A","s_a_human.h_a_862A","s_a_human.h_a_871A"};
		Commands->Set_Animation(obj,animations[Commands->Get_Random_Int(0,13)],false,0,0,-1,true);
	}
	if (announce && difftime(clock(),lastTime) > 10000.0f)
	{
		JmgUtility::MessageAllPlayers(200,127,255,"President of Corporate America: Help, I'm under attack!");
		lastTime = clock();
	}
}
void JMG_Bear_Hunter_President_Controller::Greeting(GameObject *obj)
{
	float random = Commands->Get_Random(0.0f,1.0f);
	if (random < 0.1)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Hey");
	else if (random < 0.2)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: What's up");
	else if (random < 0.3)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Sup'");
	else if (random < 0.4)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: How's it going?");
	else if (random < 0.5)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Get out there and kill some bears.");
	else if (random < 0.6)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Smell that? That's the smell of money!");
	else if (random < 0.7)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: You better head out, I've got 32 other factories to visit today!");
	else if (random < 0.8)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Rugs rugs rugs!");
	else if (random < 0.9)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Being the president of so many companies can be difficult.");
	else
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Hello.");
}
void JMG_Bear_Hunter_President_Controller::InPain(GameObject *obj)
{
	float random = Commands->Get_Random(0.0f,1.0f);
	if (random < 0.1)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: My arm...");
	else if (random < 0.2)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Uggg... my head...");
	else if (random < 0.3)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: I think my leg is broken...");
	else if (random < 0.4)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: I think I pulled my back...");
	else if (random < 0.5)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Good to see help is here...");
	else if (random < 0.6)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: I'm holding up, for now...");
	else if (random < 0.7)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Did you guys secure the walls?");
	else if (random < 0.8)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Guess we won? Otherwise you wouldn't be here!");
	else if (random < 0.9)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Did the gates close?");
	else
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: I think I twisted my ankle...");
}
void JMG_Bear_Hunter_President_Controller::Critical(GameObject *obj)
{
	float random = Commands->Get_Random(0.0f,1.0f);
	if (random < 0.1)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: I've seen better days...");
	else if (random < 0.2)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: This isn't looking good...");
	else if (random < 0.3)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Better get out there and defend me, those bears aint gonna pay you...");
	else if (random < 0.4)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Guess they really didn't want to be walked on...");
	else if (random < 0.5)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: I feel dizzy...");
	else if (random < 0.6)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: I'm seeing stars...");
	else if (random < 0.7)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Are more coming?");
	else if (random < 0.8)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: I can't see straight...");
	else if (random < 0.9)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: So... many... animals...");
	else
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Fuck me...");
}
void JMG_Bear_Hunter_President_Controller::Congrats(GameObject *obj)
{
	float random = Commands->Get_Random(0.0f,1.0f);
	if (random < 0.1)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Congrats!");
	else if (random < 0.2)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Good job!");
	else if (random < 0.3)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: I'll make sure you get a bonus!");
	else if (random < 0.4)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: That was close.");
	else if (random < 0.5)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: We did it.");
	else if (random < 0.6)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: So many rugs... I'm going to open another factory.");
	else if (random < 0.7)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Hmmmmm... I feel like are crew might be a bit smaller.");
	else if (random < 0.8)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Guess we hired the right people!");
	else if (random < 0.9)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: I'm going to go on vacation.");
	else
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Screw PETA!");
}
void JMG_Bear_Hunter_President_Controller::Rabbit(GameObject *obj)
{
	float random = Commands->Get_Random(0.0f,1.0f);
	if (random < 0.1)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Kill the wabbit, kill the wabbit, kill the wabbit!");
	else if (random < 0.2)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Shit, that's one big bunny!");
	else if (random < 0.3)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: What the hell is this bullshit!");
	else if (random < 0.4)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Did you see it jump that wall?");
	else if (random < 0.5)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Should have brought carrots...");
	else if (random < 0.6)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: They all want to get me!");
	else if (random < 0.7)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Try to hang in there, its only a rabbit.");
	else if (random < 0.8)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: Its ears are kind of fun to watch...");
	else if (random < 0.9)
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: I never liked rabbits.");
	else
		JmgUtility::DisplayChatMessage(obj,200,127,255,"President of Corporate America: No!");
}
void JMG_Bear_Hunter_Turret_Death_Alert::Killed(GameObject *obj,GameObject *killer)
{
	JMG_Bear_Hunter_Game_Control::turretsDestroyed++;
	if (JMG_Bear_Hunter_Game_Control::turretsDestroyed == 1)
		JMG_Bear_Hunter_Game_Control::medicineDropInTime = JMG_Bear_Hunter_Game_Control::gameTime-30;
	bearHunterScoreSystem.Get_Current_Player_Score_Node(JmgUtility::JMG_Get_Player_ID(killer))->KilledTurrets++;
	if (JMG_Bear_Hunter_Game_Control::gameTime && JMG_Bear_Hunter_Game_Control::gameTime <= 700 && JMG_Bear_Hunter_Game_Control::objective != 12698 && JMG_Bear_Hunter_Game_Control::turretsDestroyed >= 5)
	{
		JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: This is just getting to be too much, fall back and defend the President of Corporate America!");
		NewObjectiveSystemControl.Remove_Objective(5);
		JmgUtility::SetHUDHelpText(JMG_Bear_Hunter_Game_Control::objective,Vector3(0,1,0));
		JMG_Bear_Hunter_Game_Control::objective = 12698;
	}
	Vector3 turretPos = Commands->Get_Position(obj),baseLocation = Vector3(-0.532f,-705.389f,0.0f);
	turretPos.X -= baseLocation.X;
	turretPos.Y -= baseLocation.Y;
	float compassDegree = atan2(turretPos.Y,turretPos.X)* (180.0f / PI);
	char turretMsg[220];
	sprintf(turretMsg,"Bear Rug Co: The %s Cannon Emplacement was destroyed!",JmgUtility::JMG_Get_Compass_Directions(compassDegree));
	JmgUtility::MessageAllPlayers(127,127,255,turretMsg);
}
void JMG_Powerup_Grant_Cash::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		Commands->Give_Money(sender,Get_Float_Parameter("Money"),false);
	}
}
void JMG_Bear_Hunter_Powerup_Tracker::Created(GameObject *obj)
{
	PowerupLimiterSystemControl += obj;
}
void JMG_Bear_Hunter_Powerup_Tracker::Destroyed(GameObject *obj)
{
	PowerupLimiterSystemControl -= obj;
}
void JMG_Bear_Hunter_Grenade_Vest_Powerup::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		JMG_Bear_Hunter_Player_Soldier::hasGrenadeVest[JmgUtility::JMG_Get_Player_ID(sender)] = true;
	}
}
void JMG_Powerup_Grant_Weapon_Clips::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		char weap[256];
		sprintf(weap,"%s",Get_Current_Weapon(sender));
		if (Get_Max_Total_Bullets(sender,weap) <= 0 || Get_Total_Bullets(sender,weap) == Get_Max_Total_Bullets(sender,weap))
			sprintf(weap,"%s",JMG_Bear_Hunter_Player_Soldier::primaryWeapon[JmgUtility::JMG_Get_Player_ID(sender)]);
		const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(Get_Current_Weapon(sender),true);
		if (ammo && ammo->BeaconDefID)
			sprintf(weap,"%s",JMG_Bear_Hunter_Player_Soldier::primaryWeapon[JmgUtility::JMG_Get_Player_ID(sender)]);
		if (Get_Max_Total_Bullets(sender,weap) <= 0)
			return;
		int grantBulltes = Get_Max_Bullets(sender,weap)*Get_Int_Parameter("GrantClips");
		int neededBullets = Get_Max_Total_Bullets(sender,weap)-Get_Total_Bullets(sender,weap);
		if (!neededBullets)
			return;
		if (grantBulltes >= neededBullets)
			Set_Clip_Bullets(sender,weap,Get_Clip_Bullets(sender,weap)+neededBullets);
		else
			Set_Clip_Bullets(sender,weap,Get_Clip_Bullets(sender,weap)+grantBulltes);
	}
}
void JMG_Bear_Hunter_SpawnPoint::Created(GameObject *obj)
{
	randomSelectableSpawnPoints[Get_Int_Parameter("GroupId")] += obj;
	Commands->Destroy_Object(obj);
}
void JMG_Bear_Hunter_Player_Spawn::Created(GameObject *obj)
{
	if (Get_Player_Type(obj) != 0 && Get_Player_Type(obj) != 1)
		return;
	Rp2SimplePositionSystem::SimplePositionNode *posNode = NULL;
	if (JMG_Bear_Hunter_Game_Control::spawnGroup != 4)
		posNode = randomSelectableSpawnPoints[(PlayerData.players[JmgUtility::JMG_Get_Player_ID(obj)] && PlayerData.players[JmgUtility::JMG_Get_Player_ID(obj)]->deaths >= 5 && JMG_Bear_Hunter_Game_Control::spawnGroup == 1) ? 0 : JMG_Bear_Hunter_Game_Control::spawnGroup].GetRandomLowestValue();
	else
		posNode = randomSelectableSpawnPoints[JMG_Bear_Hunter_Game_Control::spawnGroup].GetSpotNotVisibileFromSpot(JMG_Bear_Hunt_Final_Boss::bossPos);
	if (!posNode)
	{
		Commands->Start_Timer(obj,this,0.1f,999); 
		return;
	}
	posNode->value += 3;
	Vector3 pos = posNode->position;
	MoveablePhysClass *mphys = obj->As_PhysicalGameObj() ? obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
	if (mphys)
		mphys->Find_Teleport_Location(pos,1.0f,&pos);
	Commands->Set_Position(obj,pos);
}
void JMG_Bear_Hunter_Player_Spawn::Timer_Expired(GameObject *obj,int number)
{
	if (number == 999)
	{
		Created(obj);
	}
}
void JMG_Bear_Hunter_Player_Vehicle::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		Commands->Set_Player_Type(obj,1);
		Commands->Send_Custom_Event(obj,obj,98795,98795,0.1f);
	}
	if (message == 98795 && param == 98795)
	{
		Commands->Set_Player_Type(obj,1);
	}
}
void JMG_Bear_Hunter_Player_Vehicle::Killed(GameObject *obj,GameObject *killer)
{
	bearHunterScoreSystem.Get_Current_Player_Score_Node(JmgUtility::JMG_Get_Player_ID(killer))->VehiclesDestroyed++;
	GameObject *driver = Get_Vehicle_Driver(obj);
	if (!driver)
		return;
	BearHunterScoreSystem::BHScoreNode *node = bearHunterScoreSystem.Get_Current_Player_Score_Node(JmgUtility::JMG_Get_Player_ID(driver));
	node->VehiclesLost++;
	switch (Commands->Get_Preset_ID(obj))
	{
	case 1000001144:case 1000001158:case 1000001162:case 1000001160:
		node->JazzsLost++;break;
	case 1000001321:case 1000001323:case 1000001327:case 1000001325:
		node->CleasansLost++;break;
	case 1000001310:
		node->TrucksLost++;break;
	case 1000001306:
		node->TanksLost++;break;
	case 1000001174:
		node->TurretTruckLost++;break;
	}
}
void JMG_Bear_Hunter_Bear_Tracker::Destroyed(GameObject *obj)
{
	bearObjectListControl -= obj;
}
void JMG_Bear_Hunter_Mutant_Tracker::Destroyed(GameObject *obj)
{
	mutantObjectListControl -= obj;
}
void JMG_AI_Ignore_Object::Created(GameObject *obj)
{
	Commands->Set_Is_Visible(obj,false);
	if (obj->As_VehicleGameObj())
		obj->As_VehicleGameObj()->Set_Is_Scripts_Visible(false);
}
void JMG_Bear_Hunter_Security_Turret_Truck::Created(GameObject *obj)
{
	firstTime = true;
	Commands->Set_Animation(obj,"v_secPartsTruck.v_secPartsTruck",false,0,0.0f,0.0f,false);
	if (JMG_Bear_Hunter_Game_Control::gameTime && JMG_Bear_Hunter_Game_Control::gameTime > 775)
	{
		NewObjectiveSystemControl.Set_Objective_Status(9,NewObjectiveSystem::Hidden);
		NewObjectiveSystemControl.Remove_Objective(9);
		NewObjectiveSystemControl.Add_Objective(9,NewObjectiveSystem::Tertiary,NewObjectiveSystem::Pending,12727,"",12727,obj);
		NewObjectiveSystemControl.Set_Objective_Status(10,NewObjectiveSystem::Hidden);
		NewObjectiveSystemControl.Remove_Objective(10);
	}
	JMG_Bear_Hunter_Security_Turret_Truck::id = Commands->Get_ID(obj);
}
void JMG_Bear_Hunter_Security_Turret_Truck::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (Get_Vehicle_Driver(obj) && Get_Vehicle_Driver(obj) != sender)
		return;
	if (message == CUSTOM_EVENT_VEHICLE_EXITED && !Get_Vehicle_Driver(obj) && (JMG_Bear_Hunter_Game_Control::gameTime > 775 || JMG_Bear_Hunter_Game_Control::truckTimeExtended) && !JMG_Bear_Hunter_Game_Control::hasGotTurrets)
	{
		NewObjectiveSystemControl.Set_Radar_Blip(10,obj);
		JmgUtility::DisplayChatMessage(sender,127,127,255,"Bear Rug Co: Get back in that truck, it must make it back to base!");
	}
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED && (JMG_Bear_Hunter_Game_Control::gameTime > 775 || JMG_Bear_Hunter_Game_Control::truckTimeExtended) && !JMG_Bear_Hunter_Game_Control::hasGotTurrets)
	{
		if (NewObjectiveSystemControl.Get_Objective_Status(14) == NewObjectiveSystem::Pending)
		{
			JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Looks like you forgot to deactivate the alarm, get out of there while you can, we're tracking lots of movement in the swamp!");
			NewObjectiveSystemControl.Set_Objective_Status(14,NewObjectiveSystem::Failed);
		}
		else
			JmgUtility::DisplayChatMessage(sender,127,127,255,"Bear Rug Co: Now all you have to do is return the truck to base.");
		if (firstTime)
		{
			firstTime = false;
			GameObject *switchObj = Commands->Find_Object(601037);
			Commands->Send_Custom_Event(obj,switchObj,5911911,60,0);
		}
		NewObjectiveSystemControl.Set_Objective_Status(9,NewObjectiveSystem::Accomplished);
		NewObjectiveSystemControl.Add_Objective(10,NewObjectiveSystem::Tertiary,NewObjectiveSystem::Pending,12722,"",12722,JMG_Bear_Hunter_Game_Control::centerOfBase);
		NewObjectiveSystemControl.Set_Radar_Blip(10,JMG_Bear_Hunter_Game_Control::centerOfBase);
	}
}
void JMG_Bear_Hunter_Security_Turret_Truck::Destroyed(GameObject *obj)
{
	if (NewObjectiveSystemControl.Get_Objective_Status(10) != NewObjectiveSystem::Accomplished)
	{
		NewObjectiveSystemControl.Set_Objective_Status(9,NewObjectiveSystem::Hidden);
		NewObjectiveSystemControl.Set_Objective_Status(10,NewObjectiveSystem::Failed);
		NewObjectiveSystemControl.Remove_Objective(9);
		NewObjectiveSystemControl.Remove_Objective(10);
		if (JMG_Bear_Hunter_Game_Control::gameTime > 775)
			JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: The truck was lost, GMax will pull another one out of the storage area for you shortly.");
	}
}
void JMG_Bear_Hunter_Security_Turret_Truck_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (JMG_Bear_Hunter_Game_Control::hasGotTurrets || Commands->Get_Preset_ID(enterer) != 1000001174)
		return;
	Commands->Enable_Spawner(600536,false);
	JMG_Bear_Hunter_Game_Control::hasGotTurrets = true;
	NewObjectiveSystemControl.Set_Objective_Status(10,NewObjectiveSystem::Accomplished);
	Commands->Set_Animation(enterer,"v_secPartsTruck.v_secPartsTruck",false,0,1.0f,1.0f,false);
	for (int x = 1;x < 128;x++)
	{
		GameObject *player = Get_GameObj(x);
		if (!player)
			continue;
		Commands->Give_Money(player,1000.0f,0);
	}
	if (JMG_Bear_Hunter_Game_Control::gameTime <= 775 && !JMG_Bear_Hunter_Game_Control::truckTimeExtended)
	{
		JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Thanks for the turret parts, its too late for us to set up the turrets, but here is your payment!");
		return;
	}
	GameObject *driver = Get_Vehicle_Driver(obj);
	if (driver)
	{
		Commands->Give_Points(driver,250.0f,false);
		BearHunterScoreSystem::BHScoreNode *node = bearHunterScoreSystem.Get_Current_Player_Score_Node(JmgUtility::JMG_Get_Player_ID(driver));
		node->BonusObjectivesCompleted++;
		node->totalObjectivesCompleted++;
		node->ObjectiveTurretTruck++;
	}
	GameObject *turret = Commands->Create_Object("CnC_Ramjet_Camera",Vector3(66.195f,-697.317f,7.148f));
	Commands->Set_Facing(turret,135.0f);
	Commands->Attach_Script(turret,"JMG_AI_Ignore_Object","");
	turret = Commands->Create_Object("CnC_Ramjet_Camera",Vector3(45.460f,-676.490f,7.148f));
	Commands->Set_Facing(turret,-45.0f);
	Commands->Attach_Script(turret,"JMG_AI_Ignore_Object","");
	turret = Commands->Create_Object("CnC_Ramjet_Camera",Vector3(43.841f,-659.129f,4.829f));
	Commands->Set_Facing(turret,-90.0f);
	Commands->Attach_Script(turret,"JMG_AI_Ignore_Object","");
	turret = Commands->Create_Object("CnC_Ramjet_Camera",Vector3(50.220f,-701.401f,11.333f));
	Commands->Set_Facing(turret,90.0f);
	Commands->Attach_Script(turret,"JMG_AI_Ignore_Object","");
	JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Thanks for the turret parts, we'll get these up right away, and here is your payment!");
}
void JMG_Bear_Hunt_Final_Boss::Created(GameObject *obj)
{
	moveSpeed = 0.5;
	rabbitSize = 1.0f;
	bossPos = Commands->Get_Position(obj);
	JMG_Bear_Hunter_Game_Control::gameState = JMG_Bear_Hunter_Game_Control::BossArrive;
	NewObjectiveSystemControl.Add_Objective(8,NewObjectiveSystem::Primary,NewObjectiveSystem::Pending,12728,"",12728,obj);
	NewObjectiveSystemControl.Add_Objective(12,NewObjectiveSystem::Secondary,NewObjectiveSystem::Pending,12731,"",12731);
	JMG_Bear_Hunter_Game_Control::spawnGroup = 4;
	actionUpdate = 0;
	attackingPlayer = false;
	Set_Skin(obj,"Blamo");
	Commands->Set_Shield_Type(obj,"Blamo");
	int finalMultiplier = JMG_Bear_Hunter_Game_Control::spawnKarma > 3 ? JMG_Bear_Hunter_Game_Control::spawnKarma : 1;
	Set_Max_Health(obj,JmgUtility::MathClampInt(Get_Player_Count()+1,1,64)*83.0f*finalMultiplier);//was 41.5f
	Commands->Set_Health(obj,JmgUtility::MathClampInt(Get_Player_Count()+1,1,64)*83.0f*finalMultiplier);//was 41.5f
	if (JmgUtility::MathClampInt(Get_Player_Count()+1,1,64)*83.0f >= 2000.0f)
	{
		Set_Max_Shield_Strength(obj,JmgUtility::MathClampInt(Get_Player_Count()-64,0,64)*83.0f*finalMultiplier);
		Commands->Set_Shield_Strength(obj,JmgUtility::MathClampInt(Get_Player_Count()-64,0,64)*83.0f*finalMultiplier);
	}
	else
		Set_Max_Shield_Strength(obj,0.0f);
	lastHealth = Commands->Get_Health(obj);
	lastArmor = Commands->Get_Shield_Strength(obj);
	int grantLives = JmgUtility::MathClampInt(6*(1/max(Get_Player_Count(),1)),1,11);
	for (int x = 1;x < 128;x++)
		if (!Get_GameObj(x))
		{
			JMG_Bear_Hunter_Game_Control::remainingLives[x] = 0;
			JMG_Bear_Hunter_Game_Control::aiIgnorePlayers[x] = -1;
		}
		else
			JMG_Bear_Hunter_Game_Control::remainingLives[x] = grantLives;
	GameObject *jump = Commands->Create_Object("Daves Arrow",Vector3(0.532f,-705.389f,0.0f));
	float minFacing[8] = {0,30,90,115,150,-180,-145,-70},maxFacing[8] = {30,65,110,140,180,-155,-110,-45};
	float probability[8] = {0.133f,0.289f,0.378f,0.489f,0.622f,0.733f,0.889f,1.0f};
	float random = Commands->Get_Random(0.0f,1.0f)*JmgUtility::MathClamp(Get_Player_Count()/32.0f,0.0f,1.0f);
	JMG_Bear_Hunt_Final_Boss::bossChoose = 7;
	for (int x = 7;x > -1;x--)
		if (random <= probability[x])
			JMG_Bear_Hunt_Final_Boss::bossChoose = x;
	Commands->Set_Facing(jump,Commands->Get_Random(minFacing[JMG_Bear_Hunt_Final_Boss::bossChoose],maxFacing[JMG_Bear_Hunt_Final_Boss::bossChoose]));
	Commands->Set_Model(jump,"rabbitJump");
	Commands->Set_Animation(jump,"rabbitJump.rabbitJump",false,0,0.0f,90.0f,false);
	Commands->Attach_To_Object_Bone(obj,jump,"Bunny");
	jumperId = Commands->Get_ID(jump);
	obj->As_SoldierGameObj()->Set_Scale_Across_Network(1.5f);
	Commands->Start_Timer(obj,this,3.0f,1);
}
void JMG_Bear_Hunt_Final_Boss::Enemy_Seen(GameObject *obj,GameObject *seen)	
{
	if (seen->As_SmartGameObj() && seen->As_SmartGameObj()->Is_Stealthed() && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 900)
		return;
	if (Is_Script_Attached(seen,"JMG_Bear_Hunter_AI_Ignore_Up_To_25_Meters") && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 625)
		return;
	if (enemyId && targetUpdate)
		return;
	if (enemyId && JmgUtility::SimpleDistance(Commands->Get_Position(seen),Commands->Get_Position(obj)) > targetDistance)
		return;
	if (JMG_Bear_Hunter_Game_Control::aiIgnorePlayers[JmgUtility::JMG_Get_Player_ID(seen)])
		return;
	 enemyId = Commands->Get_ID(seen);
	 if (!JMG_Bear_Hunter_Game_Control::playersDead && enemyId == JMG_Bear_Hunter_Game_Control::mutantTargetId)
		 return;
	 if (Commands->Is_A_Star(seen))
	 {
		attackingPlayer = true;;
		targetUpdate = 40;
	 }
	 else
	 {
		attackingPlayer = false;
		targetUpdate = 0;
	 }
}
void JMG_Bear_Hunt_Final_Boss::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		GameObject *jumper = Commands->Find_Object(jumperId);
		if (jumper && Get_Animation_Frame(jumper) < 90)
			Commands->Start_Timer(obj,this,0.1f,1);
		else
		{
			GameObject *jumper = Commands->Find_Object(jumperId);
			if (jumper)
				Commands->Destroy_Object(jumper);
			Set_Skin(obj,"BossRabbitHealth");
			Commands->Set_Shield_Type(obj,"BossRabbitHealth");
			targetedId = 0;
			targetDistance = 0.0f;
			targetUpdate = 0;
			lastPos = Commands->Get_Position(obj);
			stuckCount = 0;
			enemyId = 0;
			Commands->Enable_Enemy_Seen(obj,true);
			Commands->Start_Timer(obj,this,0.1f,2);
			for (int x = 0;x < 8;x++)
			{
				GameObject *light = Commands->Find_Object(600911+x);
				char ModelName[16];
				sprintf(ModelName,"%s",Get_Model(light));
				float AnimationFrame = Get_Animation_Frame(light);
				Commands->Set_Model(light,ModelName);
				char Anim[32];
				sprintf(Anim,"%s.%s",ModelName,ModelName);
				Commands->Set_Animation(light,Anim,true,0,AnimationFrame,-1,false);
				char bonename[32];
				for (int x = 0;x < 2;x++)
				{
					GameObject *lightEffect = Commands->Create_Object("Warning_Light_EffectN",Commands->Get_Position(light));
					sprintf(bonename,"LightPos%d",x+1);
					Commands->Attach_To_Object_Bone(lightEffect,light,bonename);
				}
			}
		}
	}
	if (number == 2)
	{
		if (Commands->Get_Health(obj) <= 0)
			return;
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *target = enemyId ? Commands->Find_Object(enemyId) : NULL;
		ChooseTarget(obj,pos,target);
		if (targetUpdate)
			targetUpdate--;
		if (actionUpdate)
			actionUpdate--;
		float dist = JmgUtility::SimpleDistance(pos,lastPos);
		if (dist < 1.0f)
		{
			stuckCount++;
			if (stuckCount > 50)
				Respawn(obj);
		}
		else
		{
			stuckCount = 0;
			lastPos = pos;
		}
		if (!inRange(obj) || pos.Z < -5.0f)
			Respawn(obj);
		if (!inRange(target))
		{
			enemyId = 0;
			attackingPlayer = false;
		}
		bossPos = Commands->Get_Position(obj);
		if (target && JmgUtility::SimpleDistance(Commands->Get_Position(target),bossPos) < 4.0f)
		{
			int playerId = JmgUtility::JMG_Get_Player_ID(target);
			if (Commands->Get_Health(target) && !PlayerData.players[playerId]->displayedKillMessage)
			{
				char deathMsg[220];
				if (Commands->Is_A_Star(target))
					sprintf(deathMsg,"%s was eaten alive by the giant Mutant Rabbit!",Get_Player_Name(target));
				else
					sprintf(deathMsg,"A %s was eaten alive by the giant Mutant Rabbit!",Get_Translated_Preset_Name(target));
				JmgUtility::MessageAllPlayers(255,255,0,deathMsg);
				PlayerData.players[playerId]->displayedKillMessage = true;
				Commands->Apply_Damage(target,9999.9f,"BlamoKiller",obj);
				Commands->Apply_Damage(obj,-100.0f,"None",target);
				moveSpeed = JmgUtility::MathClamp(moveSpeed+0.05f,0.5f,1.0f);
				rabbitSize = JmgUtility::MathClamp(rabbitSize+0.1f,1.0f,2.0f);
				BearHunterScoreSystem::BHScoreNode *node = bearHunterScoreSystem.Get_Current_Player_Score_Node(JmgUtility::JMG_Get_Player_ID(target));
				if (node)
					node->EatenByRabbit++;
			}
		}
		Commands->Start_Timer(obj,this,0.25f,2);
	}
}
void JMG_Bear_Hunt_Final_Boss::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damager && (JMG_Bear_Hunter_Game_Control::earthWarheadId == Get_Damage_Warhead() || obj == damager) && damage > 0.0f && lastHealth)
	{
		Commands->Set_Health(obj,lastHealth);
		Commands->Set_Shield_Strength(obj,lastArmor);
		return;
	}
	if (damage > 30.0f && damager)
	{
		if (lastArmor)
		{
			lastArmor -= damage*0.5f;
			if (lastArmor < 0)
			{
				lastHealth += lastArmor;
				lastArmor = 0;
			}
		}
		else
			lastHealth -= damage*0.5f;
		if (lastHealth <= 0)
		{
			Commands->Apply_Damage(0,9999.9f,"BlamoKiller",obj);
			return;
		}
		Commands->Set_Health(obj,lastHealth);
		Commands->Set_Shield_Strength(obj,lastArmor);
	}
	lastHealth = Commands->Get_Health(obj);
	lastArmor = Commands->Get_Shield_Strength(obj);
	if (damage <= 0 || !damager || obj == damager || (enemyId && targetUpdate) || enemyId == Commands->Get_ID(damager) || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	if (JMG_Bear_Hunter_Game_Control::aiIgnorePlayers[JmgUtility::JMG_Get_Player_ID(damager)])
		return;
	if (enemyId && JmgUtility::SimpleDistance(Commands->Get_Position(damager),Commands->Get_Position(obj)) > targetDistance)
		return;
	 enemyId = Commands->Get_ID(damager);
	 if (Commands->Is_A_Star(damager))
	 {
		attackingPlayer = true;
		targetUpdate = 40;
	 }
	 else
	 {
		attackingPlayer = false;
		targetUpdate = 0;
	 }
}
void JMG_Bear_Hunt_Final_Boss::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason)
{
	if (!Commands->Get_Health(obj))
		return;
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{
		GameObject *target = Commands->Find_Object(enemyId);
		enemyId = 0;
		if (Commands->Get_Health(obj) <= 0)
			return;
		int playerId = JmgUtility::JMG_Get_Player_ID(target);
		if (playerId && Commands->Get_Health(target) && JMG_Bear_Hunter_Game_Control::aiIgnorePlayers[playerId] != -1)
			JMG_Bear_Hunter_Game_Control::aiIgnorePlayers[playerId] = 5;
		ChooseTarget(obj,Commands->Get_Position(obj),NULL);
	}
	if (reason == ACTION_COMPLETE_PATH_BAD_START)
	{
		Respawn(obj);
		enemyId = 0;
		Console_Input("msg JMG_Bear_Hunt_Final_Boss ERROR: Couldn't find pathfield!");
	}
}
void JMG_Bear_Hunt_Final_Boss::Killed(GameObject *obj,GameObject *killer)
{
	JMG_Bear_Hunter_Game_Control::bossRabitDead = 5;
	char deathMsg[220];
	if (Commands->Is_A_Star(killer))
		sprintf(deathMsg,"%s killed the giant Mutant Rabbit!",Get_Player_Name(killer));
	else
		sprintf(deathMsg,"A %s killed the giant Mutant Rabbit!",Get_Translated_Preset_Name(killer));
	JmgUtility::MessageAllPlayers(255,255,0,deathMsg);
	JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: We did it, the President of Corporate America has been saved!");
	NewObjectiveSystemControl.Set_Objective_Status(8,NewObjectiveSystem::Accomplished);
}
void JMG_Bear_Hunt_Final_Boss::AttackTarget(GameObject *obj,GameObject *target,float distance)
{
	int tempId = Commands->Get_ID(target);
	if (targetedId == tempId && actionUpdate)
		return;
	actionUpdate = 10;
	Commands->Action_Reset(obj,10);
	targetedId = tempId;
	ActionParamsStruct params;
	params.Set_Movement(target,moveSpeed,target->As_SoldierGameObj() ? 0.0f : 5.0f,false);
	params.MovePathfind = true;
	Commands->Action_Goto(obj,params);
	if ((distance < 40000.0f && JMG_Bear_Hunter_Game_Control::mutantTargetId != targetedId) || distance < 625)
		params.AttackForceFire = true;
	else
		params.AttackForceFire = false;
	params.Set_Basic(this,100,10,AI_STATE_COMBAT);
	params.Set_Attack(target,200.0f,0.0f,Get_Player_Count() <= 10 ? true : false);
	Commands->Action_Attack(obj,params);
}
void JMG_Bear_Hunt_Final_Boss::Respawn(GameObject *obj)
{
	Rp2SimplePositionSystem::SimplePositionNode *posNode = randomSelectableSpawnPoints[5].GetRandom();
	Vector3 pos = Vector3(77.169f,-623.643f,2.485f);
	if (posNode)
		pos = posNode->position;
	MoveablePhysClass *mphys = obj->As_PhysicalGameObj() ? obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
	if (mphys)
		mphys->Find_Teleport_Location(pos,1.0f,&pos);
	Commands->Set_Position(obj,pos);
}
void JMG_Bear_Hunt_Final_Boss::ChooseTarget(GameObject *obj,Vector3 pos,GameObject *target)
{
	if (!attackingPlayer)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *closestStar = NULL;
		float closestDist = 0;
		for (int x = 0;x < maxTotalBearHunterPlayerAssistAI;x++)
		{
			GameObject *aiPlayer = Commands->Find_Object(JMG_Bear_Hunter_Player_Assist_AI::aiIds[x]);
			if (!aiPlayer || !inRange(aiPlayer) || !Commands->Get_Health(aiPlayer))
				continue;
			float tempDist = JmgUtility::SimpleDistance(pos,Commands->Get_Position(aiPlayer));
			if (!closestStar || tempDist < closestDist)
			{
				closestStar = aiPlayer;
				closestDist = tempDist;
				target = aiPlayer;
				enemyId = Commands->Get_ID(aiPlayer);
				attackingPlayer = true;
			}
		}
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player || JMG_Bear_Hunter_Game_Control::aiIgnorePlayers[x] || !Commands->Get_Health(player) || Get_Player_Type(player) == -4)
				continue;
			if (!inRange(player))
			{
				JMG_Bear_Hunter_Game_Control::aiIgnorePlayers[x] = 60;
				continue;
			}
			float tempDist = JmgUtility::SimpleDistance(pos,Commands->Get_Position(player));
			if (!closestStar || tempDist < closestDist)
			{
				closestStar = player;
				closestDist = tempDist;
				target = player;
				enemyId = Commands->Get_ID(player);
				attackingPlayer = true;
			}
		}
	}
	if (!target || !Commands->Get_Health(target))
	{
		targetUpdate = 0;
		enemyId = 0;
		attackingPlayer = false;
		if (!JMG_Bear_Hunter_Game_Control::playersDead)
			return;
		GameObject *mainObject = Commands->Find_Object(JMG_Bear_Hunter_Game_Control::mutantTargetId);
		if (mainObject)
			AttackTarget(obj,mainObject,JmgUtility::SimpleDistance(Commands->Get_Position(mainObject),pos));
	}
	else if (target)
	{
		targetDistance = JmgUtility::SimpleDistance(pos,Commands->Get_Position(target));
		if (targetDistance >= 160000)
			enemyId = 0;
		AttackTarget(obj,target,targetDistance);
	}
}
bool JMG_Bear_Hunt_Final_Boss::inRange(GameObject *obj)
{
	Vector3 pos = Commands->Get_Position(obj);
	float dist = JmgUtility::SimpleFlatDistance(pos,Vector3(-0.389f,-705.611f,0.0f));
	if (dist < 27390.25)
		return true;
	if (dist >= 27390.25 && pos.Z >= 11.0f && dist < 36100.0f)
		return true;
	return false;
}
void JMG_Bear_Hunter_Spectator::Created(GameObject *obj)
{
	Commands->Set_Player_Type(obj,-4);
	Set_Screen_Fade_Opacity_Player(obj,0.05f,0.0f);
	Set_Screen_Fade_Color_Player(obj,0.0f,0.0f,1.0f,0.0f);
	playerId = JmgUtility::JMG_Get_Player_ID(obj);
	JMG_Bear_Hunter_Game_Control::aiIgnorePlayers[playerId] = -1;
	PlayerData.getPlayerNode(obj);
	if (!PlayerData.players[playerId]->destroyTime || difftime(clock(),PlayerData.players[playerId]->destroyTime) > 500)
		Set_Background_Music_Player(obj,JMG_Bear_Hunter_Game_Control::currentSong);
	Lock_Soldier_Collision_Group(obj,SOLDIER_GHOST_COLLISION_GROUP);

	if (Get_Player_Type(obj) != 0 && Get_Player_Type(obj) != 1)
		return;
	Rp2SimplePositionSystem::SimplePositionNode *posNode = randomSelectableSpawnPoints[0].GetRandom();
	if (!posNode)
		return;
	Vector3 pos = posNode->position;
	MoveablePhysClass *mphys = obj->As_PhysicalGameObj() ? obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
	if (mphys)
		mphys->Find_Teleport_Location(pos,1.0f,&pos);
	Commands->Set_Position(obj,pos);
}
void JMG_Bear_Hunter_Spectator::Destroyed(GameObject *obj)
{
	PlayerData.players[playerId]->destroyTime = clock();
}
void JMG_Bear_Hunter_Dummy_Script::Created(GameObject *obj)
{
}
void JMG_Set_Player_Type_On_Create::Created(GameObject *obj)
{
	Commands->Set_Player_Type(obj,Get_Int_Parameter("PlayerType"));
}
void JMG_Bear_Hunter_Player_Assist_AI::Created(GameObject *obj)
{
	respawning = false;
	enemyDistance = 0;
	lastHealth = Commands->Get_Health(obj);
	lastArmor = Commands->Get_Shield_Strength(obj);
	if (maxWait >= 0)
		maxWait = maxWait+(maxWait < 20 ? 25 : Commands->Get_Random_Int(1,3));
	currentDefensePoint = NULL;
	lastAiState = aiState = NoAction;
	requiredDestinationRange = 0;
	requiredDestinationRangeSquared = 0;
	moveSpeed = 1.0f;
	lastSeen = 0;
	enemyId = 0;
	actionRefresh = 15;
	ptRefillBlock = 0;
	sprintf(primaryWeapon,"%s",Get_Current_Weapon(obj));
	if (!_stricmp(Commands->Get_Preset_Name(obj),"CnC_GDI_Minigunner"))
	{
		Respawn(obj);
		if (Commands->Get_Random(0.0f,1.0f) < 0.35)
			SetAIState(obj,AccessPT,aiPtLocations.GetNearestVector(Commands->Get_Position(obj)),1.0f,1.0f,NULL);
	}
	lastGameState = JMG_Bear_Hunter_Game_Control::gameState;
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,0.1f,1);
	Commands->Start_Timer(obj,this,1.0f,2);
	Commands->Start_Timer(obj,this,Commands->Get_Random(15.0f,60.0f),3);
	Commands->Start_Timer(obj,this,2.0f,5);
	Commands->Start_Timer(obj,this,2.0f,6);
}
void JMG_Bear_Hunter_Player_Assist_AI::Enemy_Seen(GameObject *obj,GameObject *seen)	
{
	int seenId = Commands->Get_ID(seen);
	if (!enemyId || enemyId == seenId || !lastSeen || JmgUtility::SimpleDistance(Commands->Get_Position(seen),Commands->Get_Position(obj)) < enemyDistance)
	{
		if (enemyId != seenId)
			actionRefresh = 1;
		enemyId = seenId;
		lastSeen = 30;
	}
}
void JMG_Bear_Hunter_Player_Assist_AI::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!Commands->Get_Health(obj))
			return;
		if (lastSeen)
		{
			lastSeen--;
			if (!lastSeen)
				enemyId = 0;
		}
		if (enemyId)
		{
			GameObject *target = Commands->Find_Object(enemyId);
			if (!target || !Commands->Get_Health(target))
			{
				enemyId = 0;
				actionRefresh = 1;
			}
		}
		if (!maxWait && aiState == WaitForGroup)
			SetAIState(obj,MoveToCabin,Vector3(),5.0f,0.85f,aiDefensePoints[1].GetRandom(0));
		if (aiState == NoAction && !enemyId)
			ChooseAction(obj);
		if ((aiState == AccessPT || aiState == PTReload) && JmgUtility::SimpleDistance(Commands->Get_Position(obj),targetDestination) <= requiredDestinationRange)
		{
			if (aiState == PTReload)
			{
				ptRefillBlock = 30;
				Set_Clip_Bullets(obj,primaryWeapon,Get_Max_Total_Bullets(obj,primaryWeapon));
				Commands->Select_Weapon(obj,primaryWeapon);
				Commands->Set_Health(obj,Commands->Get_Max_Health(obj));
				Commands->Set_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj));
				SetAIState(obj,NoAction,Vector3(),0.0f,NULL);
			}
			if (aiState == AccessPT)
				AIPurchaseSystemControl.PurchaseSoldier(obj,&JMG_Bear_Hunter_Player_Assist_AI::cash[Get_Int_Parameter("ID")]);
		}
		if (actionRefresh)
			actionRefresh--;
		if (aiState != NoAction)
			if (aiState != FollowPlayer)
				GotoLocation(obj,NULL);
			else
				GotoLocation(obj,JmgUtility::FindNearestPlayer(Commands->Get_Position(obj)));
		Commands->Start_Timer(obj,this,0.1f,1);
	}
	if (number == 2)
	{
		if (ptRefillBlock)
			ptRefillBlock--;
		if ((!Get_Total_Bullets(obj,primaryWeapon) || Commands->Get_Health(obj) < Commands->Get_Max_Health(obj)*0.75f) && aiState != PTReload && !ptRefillBlock)
		{
			if (!Get_Total_Bullets(obj,primaryWeapon))
				Commands->Select_Weapon(obj,"Weapon_Pistol_Player");
			SetAIState(obj,PTReload,aiPtLocations.GetNearestVector(Commands->Get_Position(obj)),1.0f,1.0f,NULL);
		}
		if (lastGameState != JMG_Bear_Hunter_Game_Control::gameState)
		{
			ChooseAction(obj);
			lastGameState = JMG_Bear_Hunter_Game_Control::gameState;
		}
		GameObject *seen = Commands->Find_Object(enemyId);
		if (seen)
			enemyDistance = JmgUtility::SimpleDistance(Commands->Get_Position(seen),Commands->Get_Position(obj));
		Commands->Start_Timer(obj,this,1.0f,2);
	}
	if (number == 3)
	{
		ChooseAction(obj);
		Commands->Start_Timer(obj,this,Commands->Get_Random(7.5f,25.0f),3);
	}
	if (number == 4)
	{
		if (Commands->Get_Health(obj))
			Respawn(obj);
	}
	if (number == 5)
	{
		if (!enemyId && Commands->Get_Random(0.0f,1.0f) <= 0.35f)
		{
			ActionParamsStruct params;
			Commands->Get_Action_Params(obj,params);
			Vector3 pos = Commands->Get_Position(obj);
			float rotation = Commands->Get_Random(0,9)*45;
			pos.X += cos(rotation)*1000.0f;
			pos.Y += sin(rotation)*1000.0f;
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
		if (!enemyId && JmgUtility::SimpleDistance(Vector3(),speed) <= 0.0f)
		{
			ActionParamsStruct params;
			Commands->Get_Action_Params(obj,params);
			Vector3 pos = Commands->Get_Position(obj);
			float rotation = Commands->Get_Random(0,9)*45;
			pos.X += cos(rotation)*1000.0f;
			pos.Y += sin(rotation)*1000.0f;
			params.Set_Face_Location(pos,repeatTime);
			Commands->Action_Face_Location(obj,params);
		}
		Commands->Start_Timer(obj,this,repeatTime,6);
	}
}
void JMG_Bear_Hunter_Player_Assist_AI::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damager && (JMG_Bear_Hunter_Game_Control::earthWarheadId == Get_Damage_Warhead() || obj == damager) && damage > 0.0f && lastHealth)
	{
		 Commands->Set_Health(obj,lastHealth);
		 Commands->Set_Shield_Strength(obj,lastArmor);
		return;
	}
	lastHealth = Commands->Get_Health(obj);
	lastArmor = Commands->Get_Shield_Strength(obj);
	if (Commands->Get_Player_Type(damager) == 1)
		return;
	int seenId = Commands->Get_ID(damager);
	if (!enemyId || enemyId == seenId || !lastSeen || JmgUtility::SimpleDistance(Commands->Get_Position(damager),Commands->Get_Position(obj)) < enemyDistance)
	{
		if (enemyId != seenId)
			actionRefresh = 1;
		enemyId = seenId;
		lastSeen = 30;
	}
}
void JMG_Bear_Hunter_Player_Assist_AI::Killed(GameObject *obj,GameObject *killer)
{
	SetAIState(obj,NoAction,Vector3(),0.0f,NULL);
	/*if (killer && killer != obj && !Commands->Is_A_Star(killer))
		Set_Team_Kills(Commands->Get_Player_Type(killer) == 1 ? 1 : 0,Get_Team_Kills(Commands->Get_Player_Type(killer) == 1 ? 1 : 0)+1);
	Set_Team_Deaths(1,Get_Team_Deaths(1)+1);*/
	aiDeaths[Get_Int_Parameter("ID")]++;
	if (Is_Script_Attached(obj,"JMG_Bear_Hunter_Player_Assist_AI"))
		Remove_Script(obj,"JMG_Bear_Hunter_Player_Assist_AI");
}
void JMG_Bear_Hunter_Player_Assist_AI::Destroyed(GameObject *obj)
{
	SetAIState(obj,NoAction,Vector3(),0.0f,NULL);
}
void JMG_Bear_Hunter_Player_Assist_AI::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason)
{
	if (!Commands->Get_Health(obj))
		return;
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{
		SetAIState(obj,GoHome,Vector3(3.25f,-600.0f,0.5f),5.0f,1.0f);
	}
	if (reason == ACTION_COMPLETE_PATH_BAD_START || reason == ACTION_COMPLETE_MOVE_NO_PROGRESS_MADE)
	{
		if (!respawning)
		{
			respawning = true;
			Commands->Start_Timer(obj,this,2.5f,4);
		}
	}
}
void JMG_Bear_Hunter_Player_Assist_AI::GotoLocation(GameObject *obj,GameObject *player)
{
	if (lastAiState == aiState && actionRefresh)
		return;
	actionRefresh = 60;
	lastAiState = aiState;
	ActionParamsStruct params;
	params.Set_Basic(this,100,10);
	if (player)
	{
		params.Set_Movement(player,moveSpeed,requiredDestinationRange,false);
		params.MoveFollow = true;
	}
	else
		params.Set_Movement(targetDestination,moveSpeed,requiredDestinationRange,false);
	params.MovePathfind = true;
	Commands->Action_Goto(obj,params);
	if (enemyId)
	{
		GameObject *target = Commands->Find_Object(enemyId);
		if (!target)
		{
			enemyId = 0;
			return;
		}
		params.Set_Basic(this,100,10);
		/*if (distance < weaponRange)
			params.AttackForceFire = true;
		else
			params.AttackForceFire = false;*/
		params.Set_Attack(target,300.0f,1.0f,true);
		params.AttackCheckBlocked = false;
		Commands->Action_Attack(obj,params);
	}
}
void JMG_Bear_Hunter_Player_Assist_AI::SetAIState(GameObject *obj,AIStates newState,Vector3 pos,float distance,float speed,Rp2SimplePositionSystem::SimplePositionNode *newNode)
{
	if (newState < aiState && newState != NoAction)
		return;
	if (newNode)
		newNode->value++;
	if (currentDefensePoint)
		currentDefensePoint->value--;
	currentDefensePoint = newNode;
	aiState = newState;
	if (newState == NoAction)
		return;
	if (newNode)
		targetDestination = newNode->position;
	else
		targetDestination = pos;
	moveSpeed = speed;
	requiredDestinationRange = distance;
	requiredDestinationRangeSquared = distance*distance;
	Commands->Action_Reset(obj,100.0f);
}
void JMG_Bear_Hunter_Player_Assist_AI::ChooseAction(GameObject *obj)
{
	switch (JMG_Bear_Hunter_Game_Control::gameState)
	{
	case JMG_Bear_Hunter_Game_Control::LeaveBase:
		SetAIState(obj,WaitForGroup,Vector3(),2.5f,1.0f,aiDefensePoints[5].GetRandom(0));break;
	case JMG_Bear_Hunter_Game_Control::HuntBears:
		if (!JMG_Bear_Hunter_Game_Control::spawnGroup)
			SetAIState(obj,WaitForGroup,Vector3(),2.5f,1.0f,aiDefensePoints[0].GetRandom(0));
		else
			SetAIState(obj,HoldCabin,Vector3(),2.5f,1.0f,aiDefensePoints[2].GetRandom(0));
		break;
	case JMG_Bear_Hunter_Game_Control::MutationStart:
		if (!JMG_Bear_Hunter_Game_Control::spawnGroup)
			SetAIState(obj,WaitForGroup,Vector3(),2.5f,1.0f,aiDefensePoints[0].GetRandom(0));
		else
			SetAIState(obj,HoldCabin,Vector3(),2.5f,1.0f,aiDefensePoints[2].GetRandom(0));
		break;
	case JMG_Bear_Hunter_Game_Control::FallBack:case JMG_Bear_Hunter_Game_Control::BossArrive:
		SetAIState(obj,DefendGate,Vector3(),2.5f,1.0f,aiDefensePoints[3].GetRandom(0));break;
	case JMG_Bear_Hunter_Game_Control::DefendPrez:case JMG_Bear_Hunter_Game_Control::SecureBase:
		SetAIState(obj,DefendPres,Vector3(),2.5f,1.0f,aiDefensePoints[4].GetRandom(0));break;
	}
}
void JMG_Bear_Hunter_Player_Assist_AI::Respawn(GameObject *obj)
{
	Toggle_Fly_Mode(obj);
	Rp2SimplePositionSystem::SimplePositionNode *posNode = randomSelectableSpawnPoints[(aiDeaths[Get_Int_Parameter("ID")] >= 5 && JMG_Bear_Hunter_Game_Control::spawnGroup == 1) ? 0 : JMG_Bear_Hunter_Game_Control::spawnGroup].GetRandom();
	Vector3 pos = Vector3(77.169f,-623.643f,2.485f);
	if (posNode)
		pos = posNode->position;
	Commands->Set_Position(obj,pos);
	MoveablePhysClass *mphys = obj->As_PhysicalGameObj() ? obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
	if (mphys)
		mphys->Find_Teleport_Location(pos,1.0f,&pos);
	Commands->Set_Position(obj,pos);
	Toggle_Fly_Mode(obj);
	respawning = false;
}
void JMG_Bear_Hunter_AI_PT_Location::Created(GameObject *obj)
{
	aiPtLocations += new Rp2SimplePositionSystem::SimplePositionNode(obj,Get_Int_Parameter("PTSetting"));
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Bear_Hunter_AI_PT_Location::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Commands->Destroy_Object(obj);
	}
}
void JMG_Bear_Hunter_AI_Defense_Point::Created(GameObject *obj)
{
	aiDefensePoints[Get_Int_Parameter("GroupId")] += obj;
	Commands->Destroy_Object(obj);
}
void JMG_Bear_Hunter_Warning_Light_Zone::Created(GameObject *obj)
{
	lightOn = false;
	GameObject *light = Commands->Find_Object(Get_Int_Parameter("LightID"));
	if (light)
	{
		sprintf(ModelName,"%s",Get_Model(light));
		char Anim[32];
		sprintf(Anim,"%sOf",ModelName);
		Commands->Set_Model(obj,ModelName);
		sprintf(Anim,"%sOf.%sOf",ModelName,ModelName);
		Commands->Set_Animation(obj,Anim,false,0,0,(float)Commands->Get_Random_Int(0,36),false);
	}
	warnTimeCountdown = 0;
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Bear_Hunter_Warning_Light_Zone::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (warnTimeCountdown)
		{
			warnTimeCountdown--;
			if (!warnTimeCountdown && lightOn)
			{
				GameObject *light = Commands->Find_Object(Get_Int_Parameter("LightID"));
				float AnimationFrame = Get_Animation_Frame(light);
				char Anim[32];
				sprintf(Anim,"%sOf",ModelName);
				Commands->Set_Model(obj,ModelName);
				sprintf(Anim,"%sOf.%sOf",ModelName,ModelName);
				Commands->Set_Animation(light,Anim,false,0,AnimationFrame,AnimationFrame,false);
				for (int x = 0;x < 2;x++)
				{
					GameObject *lightEffect = Commands->Find_Object(LightEffectIDs[x]);
					if (lightEffect)
						Commands->Destroy_Object(lightEffect);
				}
				lightOn = false;
			}
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Bear_Hunter_Warning_Light_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (!JMG_Bear_Hunter_Game_Control::gameTime)
		return;
	if (Commands->Get_Player_Type(enterer) == 0 || Commands->Get_Player_Type(enterer) == -3)
	{
		if (!lightOn)
		{
			lightOn = true;
			GameObject *light = Commands->Find_Object(Get_Int_Parameter("LightID"));
			float AnimationFrame = Get_Animation_Frame(light);
			Commands->Set_Model(light,ModelName);
			char Anim[32];
			sprintf(Anim,"%s.%s",ModelName,ModelName);
			Commands->Set_Animation(light,Anim,true,0,AnimationFrame,-1,false);
			char bonename[32];
			for (int x = 0;x < 2;x++)
			{
				GameObject *lightEffect = Commands->Create_Object(JMG_Bear_Hunter_Game_Control::gameTime <= 900 ? "Warning_Light_EffectN" : "Warning_Light_Effect",Commands->Get_Position(light));
				sprintf(bonename,"LightPos%d",x+1);
				Commands->Attach_To_Object_Bone(lightEffect,light,bonename);
				LightEffectIDs[x] = Commands->Get_ID(lightEffect);
			}
		}
		warnTimeCountdown = 10;
	}
}
void JMG_Bear_Hunter_Vehicle_Purchase_System::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Bear_Hunter_Vehicle_Purchase_System::Timer_Expired(GameObject *obj,int number)
{
	if (number < 3)
	{
		GameObject *owner = Get_Vehicle_Owner(obj);
		if (owner)
		{
			Rp2SimplePositionSystem::SimplePositionNode *node = aiPtLocations.GetNearest(Commands->Get_Position(owner));
			if (node)
				switch (node->value)
				{
				case 1:AirDropFunction(obj,Vector3(68.678f,-29.790f,2.397f),-135.0f,600977,600953,600943,600948);break;
				case 2:AirDropFunction(obj,Vector3(420.334f,575.931f,3.639f),-180.0f,600982,600970,600965,600960);break;
				case 3:AirDropFunction(obj,Vector3(-4.633f,-694.783f,1.797f),90.0f,601217,601202,601207,601212);break;
				case 4:AirDropFunction(obj,Vector3(-104.684f,-624.778f,1.683f),-45.000,601318,601308,601303,601313);break;
				case 5:AirDropFunction(obj,Vector3(-24.754f,-575.035f,1.683f),-90.0f,601335,601330,600267,601325);break;
				case 6:AirDropFunction(obj,Vector3(74.501f,-595.507f,1.683f),-135.0f,601352,601347,601337,601342);break;
				case 7:AirDropFunction(obj,Vector3(119.738f,-729.807f,1.683f),-180.000f,601369,601359,601354,601364);break;
				case 8:AirDropFunction(obj,Vector3(23.741f,-836.318f,1.683f),90.0f,60164944,601660,601655,601650);break;
				case 9:AirDropFunction(obj,Vector3(-495.0f,-268.525f,1.683f),0.0f,601733,601744,601739,601734);break;
				default:AirDropFunction(obj,Vector3(5.650f,-752.443f,2.397f),-135.0f,601413,601424,601419,601414);break;
				}
			Commands->Action_Reset(obj,100.0f);
		}
		else
		{
			Commands->Start_Timer(obj,this,0.25f,number+1);
			return;
		}
		if (Is_Script_Attached(obj,"JMG_Bear_Hunter_Vehicle_Purchase_System"))
			Remove_Script(obj,"JMG_Bear_Hunter_Vehicle_Purchase_System");
	}
	if (number == 3)
	{
		if (Is_Script_Attached(obj,"JMG_Bear_Hunter_Vehicle_Purchase_System"))
			Remove_Script(obj,"JMG_Bear_Hunter_Vehicle_Purchase_System");
	}
}
void JMG_Bear_Hunter_Vehicle_Purchase_System::AirDropFunction(GameObject *obj,Vector3 pos,float facing,int deathZoneId,int waypath1,int waypath2,int waypath3)
{
	Commands->Set_Position(obj,pos);
	Commands->Set_Facing(obj,facing);
	char params[512];
	sprintf(params,"%d,%d,Drop_Helicopter",deathZoneId,Commands->Get_Random(0.0,1.0f) < 0.33 ? waypath1 : Commands->Get_Random(0.0f,1.0f) < 0.5f ? waypath2 : waypath3);
	Commands->Attach_Script(obj,"JMG_Airdrop_Attached_Object_On_Create",params);
}		
void JMG_Airdrop_Attached_Object_On_Create::Created(GameObject *obj)
{
	sprintf(skinType,"%s",Get_Skin(obj));
	Set_Skin(obj,"Blamo");
	sprintf(armorType,"%s",Get_Shield_Type(obj));
	Commands->Set_Shield_Type(obj,"Blamo");
	entered = false;
	timeUp = false;
	Vector3 createLocation = Commands->Get_Position(obj);
	origPos = createLocation;
	origFacing = JmgUtility::rotationClamp(Commands->Get_Facing(obj),180);
	GameObject *CinematicObj = CreateAObject(obj,NULL,"Daves Arrow","XG_TransprtBone","XG_TransprtBone.XG_HD_TTraj",createLocation,origFacing,0,"none",700,1000);
	GameObject *Helicopter = CreateAObject(obj,NULL,Get_Parameter("HelicopterPreset"),"null","v_GDI_trnspt.v_GDI_trnspt",createLocation,origFacing+180,CinematicObj,"BN_Trajectory",700,-1);
	Set_Skin(Helicopter,"Blamo");
	Commands->Enable_Engine(Helicopter,true);
	GameObject *Harness = CreateAObject(obj,NULL,"Daves Arrow","XG_HD_HTraj","XG_HD_HTraj.XG_HD_HTraj",createLocation,origFacing,0,"none",437,1000);//drop at 14.56 seconds
	CreateAObject(obj,obj,"null","null","null",createLocation,origFacing,Harness,"BN_Trajectory",0,1000);
	Commands->Disable_Physical_Collisions(obj);
	Commands->Set_Player_Type(obj,-2);
	Commands->Attach_Script(obj,"JMG_RP2_Set_Object_Team_On_Timer","14.3,68541,-3");
	Commands->Attach_Script(obj,"JMG_RP2_Set_Physical_Collisions_On_Timer","17.5,68541,1 1 1");
	CreateAObject(obj,NULL,"Daves Arrow","XG_HD_Harness","XG_HD_Harness.XG_HD_Harness",createLocation,origFacing,0,"none",437,1000);
	Commands->Send_Custom_Event(obj,obj,92562142,0,15.0f);
	Commands->Send_Custom_Event(obj,obj,92562143,0,25.0f);
}
GameObject *JMG_Airdrop_Attached_Object_On_Create::CreateAObject(GameObject *obj,GameObject *Object,const char *Preset,const char *Model,const char *Animation,const Vector3 &Pos,float Facing,GameObject *GAttachTo,const char *Bone,float DeathFrame,float LastFrame)
{
	if (!Object)
		Object = Commands->Create_Object(Preset,Pos);
	Commands->Set_Facing(Object,Facing);
	if (_stricmp(Model,"null"))
		Commands->Set_Model(Object,Model);
	if (_stricmp(Animation,"null"))
		Commands->Set_Animation(Object,Animation,1,0,0,LastFrame,1);
	if (GAttachTo)
		Commands->Attach_To_Object_Bone(Object,GAttachTo,Bone);
	if (DeathFrame)
		Commands->Send_Custom_Event(Object,obj,92562141,0,DeathFrame/30.0f);
	return Object;
}
void JMG_Airdrop_Attached_Object_On_Create::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		Commands->Action_Reset(obj,100);
		entered = true;
		if (timeUp)
		{
			Set_Skin(obj,skinType);
			Commands->Set_Shield_Type(obj,armorType);
			if (Is_Script_Attached(obj,"JMG_Airdrop_Attached_Object_On_Create"))
				Remove_Script(obj,"JMG_Airdrop_Attached_Object_On_Create");
		}
	}
	if (message == 92562141)
	{
		Commands->Destroy_Object(sender);
	}
	if (message == 92562142)
	{
		Commands->Enable_Collisions(obj);
		Commands->Set_Position(obj,origPos);
		GameObject *Zone = Commands->Find_Object(Get_Int_Parameter("DeathZoneID"));
		if (Zone)
			for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
			{
				if (!current->Data())
					continue;
				ScriptableGameObj *obj2 = current->Data()->As_ScriptableGameObj();
				if (obj2 && obj2 != obj && IsInsideZone(Zone,obj2))
					Commands->Apply_Damage(obj2,999.9f,"None",0);
			}
		if (Get_Int_Parameter("WaypathId"))
		{
			ActionParamsStruct params;
			params.Set_Basic(this,100,1);
			params.MoveArrivedDistance = 1.0f;
			params.MoveCrouched = false;
			params.MoveSpeed = 1.0f;
			params.WaypathID = Get_Int_Parameter("WaypathId");
			params.WaypathSplined = true;
			params.ShutdownEngineOnArrival = true;
			params.AttackActive = false;
			params.AttackCheckBlocked = false;
			Commands->Action_Goto(obj,params);
		}
	}
	if (message == 92562143)
	{
		timeUp = true;
		if (entered)
		{
			Set_Skin(obj,skinType);
			Commands->Set_Shield_Type(obj,armorType);
			if (Is_Script_Attached(obj,"JMG_Airdrop_Attached_Object_On_Create"))
				Remove_Script(obj,"JMG_Airdrop_Attached_Object_On_Create");
		}
	}
}
void JMG_Animate_While_Firing::Created(GameObject *obj)
{
	spinDown = 0;
	firing = false;
	bulletCount = Get_Current_Bullets(obj);
	sprintf(animation,"%s.%s",Get_Model(obj),Get_Model(obj));
	Commands->Set_Animation(obj,animation,false,0,0,0,true);
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Animate_While_Firing::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		int currentBulletCount = Get_Current_Bullets(obj);
		if (bulletCount == currentBulletCount)
		{
			if (firing && spinDown >= (unsigned int)Get_Int_Parameter("SpinDownTime"))
			{
				Commands->Set_Animation(obj,animation,false,0,Get_Animation_Frame(obj),Get_Animation_Frame(obj),true);
				firing = false;
			}
			else
				spinDown++;
		}
		else if (bulletCount > currentBulletCount)
		{
			if (!firing)
			{
				spinDown = 0;
				Commands->Set_Animation(obj,animation,true,0,Get_Animation_Frame(obj),-1,true);
				firing = true;
			}
		}
		if (bulletCount != currentBulletCount)
			bulletCount = currentBulletCount;
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Bear_Hunter_Alarm_Switch::Created(GameObject *obj)
{
	waitingToDeactivate = false;
	switchPosition = Get_Int_Parameter("StartPosition");
	ActivateSwitch(obj,switchPosition);
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
	GameObject *notifyObject = Commands->Find_Object(Get_Int_Parameter("NotifyId"));
	if (notifyObject)
		Commands->Send_Custom_Event(obj,notifyObject,Get_Int_Parameter("CreateNotifyMessage"),Get_Int_Parameter("CreateNotifyParam"),0);
}
void JMG_Bear_Hunter_Alarm_Switch::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == Get_Int_Parameter("AlarmTriggerMessage") && param && switchPosition == 1)
	{
		waitingToDeactivate = false;
		if (param)
		{
			UpdateTower(2);
			Commands->Send_Custom_Event(obj,obj,Get_Int_Parameter("AlarmTriggerMessage"),1,(float)param);
		}
		GameObject *notifyObject = Commands->Find_Object(Get_Int_Parameter("NotifyId"));
		if (notifyObject)
			Commands->Send_Custom_Event(obj,notifyObject,Get_Int_Parameter("NotifyMessage"),param,0);
	}
	if (message == Get_Int_Parameter("AlarmTriggerMessage") && !param && switchPosition == 1)
	{
		waitingToDeactivate = false;
		UpdateTower(0);
		GameObject *notifyObject = Commands->Find_Object(Get_Int_Parameter("NotifyId"));
		if (notifyObject)
			Commands->Send_Custom_Event(obj,notifyObject,Get_Int_Parameter("NotifyMessage"),0,0);
	}
	if (message == Get_Int_Parameter("SwitchSwitchCustom"))
	{
		waitingToDeactivate = false;
		switchPosition = param;
		ActivateSwitch(obj,switchPosition);
	}
	if (message == 7854621)
	{
		if (!waitingToDeactivate)
			return;
		if (param)
		{
			Commands->Send_Custom_Event(obj,obj,7854621,param-1,1.0f);
			return;
		}
		switchPosition = 0;
		ActivateSwitch(obj,switchPosition);
		waitingToDeactivate = false;
	}
}
void JMG_Bear_Hunter_Alarm_Switch::Poked(GameObject *obj, GameObject *poker)
{
	if (waitingToDeactivate)
		return;
	if (Commands->Get_Player_Type(poker) == -4)
		return;
	if (Get_Int_Parameter("DeactivateTime"))
	{
		waitingToDeactivate = true;
		Commands->Send_Custom_Event(obj,obj,7854621,Get_Int_Parameter("DeactivateTime"),1.0f);
	}
	switchPosition = switchPosition == 2 ? 0 : (switchPosition == 1 ? 0 : 2);
	ActivateSwitch(obj,switchPosition);
}
void JMG_Bear_Hunter_Alarm_Switch::ActivateSwitch(GameObject *obj,int position)
{
	Commands->Create_Sound(Get_Parameter("LeverPullSound"),Commands->Get_Position(obj),obj);
	char animation[32];
	sprintf(animation,"%s.%s",Get_Model(obj),Get_Model(obj));
	Commands->Set_Animation(obj,animation,false,0,(float)position,(float)position,false);
	UpdateTower(position);
	GameObject *notifyObject = Commands->Find_Object(Get_Int_Parameter("NotifyId"));
	if (notifyObject)
		Commands->Send_Custom_Event(obj,notifyObject,Get_Int_Parameter("NotifyMessage"),(position == 2 ? -1 : 0),0);
}
void JMG_Bear_Hunter_Alarm_Switch::UpdateTower(int position)
{
	if (!position)
		Commands->Static_Anim_Phys_Goto_Frame(Get_Int_Parameter("TowerId"),0,Get_Parameter("TowerAnimation"));
	else if (position == 2)
		Commands->Static_Anim_Phys_Goto_Frame(Get_Int_Parameter("TowerId"),1,Get_Parameter("TowerAnimation"));
}
void JMG_Pickup_Display_Message::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		JmgUtility::DisplayChatMessage(sender,225,225,0,Get_Parameter("Message"));
	}
}
void JMG_AI_Follow_Player_When_Near::Created(GameObject *obj)
{
	weaponRange = 10.0f;
	const char *weap = Get_Current_Weapon(obj);
	if (weap)
	{
		const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(weap,true);
		if (ammo)
			weaponRange = ammo->Range;
	}
	attackUpdate = 0;
	targetId = 0;
	targetDistance = 0;
	playerId = 0;
	lastSeenTime = 0;
	lastMoveSpeed = 1.0f;
	followDistance = Get_Float_Parameter("FollowDistance")*Get_Float_Parameter("FollowDistance");
	followMaxDistance = (Get_Float_Parameter("FollowDistance")+25.0f)*(Get_Float_Parameter("FollowDistance")+25.0f);
	createLocation = Commands->Get_Position(obj);
	Commands->Enable_Enemy_Seen(obj,Get_Int_Parameter("CanEngageEnemies") ? true : false);
	if (Get_Int_Parameter("TeamAsFollowingPlayer"))
		Commands->Set_Player_Type(obj,-2);
	Commands->Start_Timer(obj,this,1.0f,1);
	Commands->Start_Timer(obj,this,10.0f,2);
}
void JMG_AI_Follow_Player_When_Near::Enemy_Seen(GameObject *obj,GameObject *seen)	
{
	if (seen->As_SmartGameObj() && seen->As_SmartGameObj()->Is_Stealthed() && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 900)
		return;
	if (targetId && lastSeenTime)
		return;
	float tempDist = JmgUtility::SimpleDistance(Commands->Get_Position(seen),Commands->Get_Position(obj));
	int seenId = Commands->Get_ID(seen);
	if (!targetId || tempDist < targetDistance)
	{
		targetDistance = tempDist;
		targetId = seenId;
		attackUpdate = 0;
	}
	if (targetId == seenId)
		lastSeenTime = 2;
}
void JMG_AI_Follow_Player_When_Near::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 myPos = Commands->Get_Position(obj);
		GameObject *player = JmgUtility::FindNearestPlayer(myPos);
		int tempPlayerId = JmgUtility::JMG_Get_Player_ID(player);
		float playerDistance = JmgUtility::SimpleDistance(myPos,Commands->Get_Position(player));
		if (player && playerId ? (playerDistance < followMaxDistance) : (playerDistance < followDistance))
		{
			if (!playerId || (tempPlayerId != playerId && Get_Int_Parameter("AlwaysFollowNearestPlayer")))
			{
				if (Get_Int_Parameter("TeamAsFollowingPlayer"))
					Commands->Set_Player_Type(obj,Commands->Get_Player_Type(player));
				lastMoveSpeed = GetSpeed(player,playerDistance);
				playerId = tempPlayerId;
				ActionParamsStruct params;
				params.Set_Basic(this,100,10);
				params.Set_Movement(player,lastMoveSpeed,2.5f,false);
				params.MoveFollow = true;
				params.MovePathfind = playerDistance < 625 ? false : true;
				Commands->Action_Goto(obj,params);
			}
		}
		else if (playerId != 0)
		{
			playerId = 0;
			if (Get_Int_Parameter("TeamAsFollowingPlayer"))
				Commands->Set_Player_Type(obj,-2);
			if (Get_Int_Parameter("ReturnToOriginalPosition"))
			{
				lastMoveSpeed = 0.9f;
				ActionParamsStruct params;
				params.Set_Basic(this,100,10);
				params.Set_Movement(createLocation,lastMoveSpeed,0.0f,false);
				params.MoveFollow = false;
				params.MovePathfind = true;
				Commands->Action_Goto(obj,params);
			}
			else
				Commands->Action_Reset(obj,100.0f);
		}
		if (player)
		{
			float tempMoveSpeed = GetSpeed(player,playerDistance);
			if (tempMoveSpeed != lastMoveSpeed)
			{
				lastMoveSpeed = tempMoveSpeed;
				ActionParamsStruct params;
				Commands->Get_Action_Params(obj,params);
				params.MoveSpeed = lastMoveSpeed;
				params.MovePathfind = playerDistance < 625 ? false : true;
				Commands->Action_Goto(obj,params);
			}
		}
		if (attackUpdate)
			attackUpdate--;
		if (lastSeenTime)
			lastSeenTime--;
		GameObject *target = Commands->Find_Object(targetId);
		if (targetId && (!target || !Commands->Get_Health(target)))
		{
			targetId = 0;
			lastMoveSpeed = GetSpeed(player,playerDistance);
			ActionParamsStruct params;
			params.Set_Basic(this,100,10);
			params.Set_Movement(player,lastMoveSpeed,2.5f,false);
			params.MoveFollow = true;
			params.MovePathfind = playerDistance < 625 ? false : true;
			Commands->Action_Goto(obj,params);
		}
		else if (target && Commands->Get_Health(target))
		{
			targetDistance = JmgUtility::SimpleDistance(Commands->Get_Position(target),myPos);
			if (!attackUpdate)
			{
				attackUpdate = 25;
				ActionParamsStruct params;
				Commands->Get_Action_Params(obj,params);
				params.Set_Attack(target,weaponRange,0.0f,true);
				Commands->Action_Attack(obj,params);
			}
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
	if (number == 2)
	{
		Vector3 myPos = Commands->Get_Position(obj);
		GameObject *player = Get_GameObj(playerId);
		float playerDistance = player ? JmgUtility::SimpleDistance(myPos,Commands->Get_Position(player)) : 0.0f;
		if (player && playerDistance < followMaxDistance)
		{
			lastMoveSpeed = GetSpeed(player,playerDistance);
			ActionParamsStruct params;
			params.Set_Basic(this,100,10);
			params.Set_Movement(player,lastMoveSpeed,2.5f,false);
			params.MoveFollow = true;
			params.MovePathfind = playerDistance < 625 ? false : true;
			Commands->Action_Goto(obj,params);
			GameObject *target = Commands->Find_Object(targetId);
			if (target && Commands->Get_Health(target))
			{
				params.Set_Attack(target,weaponRange,0.0f,true);
				Commands->Action_Attack(obj,params);
			}
		}
		Commands->Start_Timer(obj,this,5.0,2);
	}
}
void JMG_AI_Follow_Player_When_Near::Destroyed(GameObject *obj)
{
}
inline float JMG_AI_Follow_Player_When_Near::GetSpeed(GameObject *player,float distance)
{
	return !player ? 0.9f : (distance > 2500) ? 2.0f :  (distance > 1225) ? 1.5f : (distance > 625) ? 1.25f : (distance > 225) ? 1.0f : (distance > 100) ? 0.75f : 0.5f;
}
void JMG_Bear_Hunter_Engineer_Follow_Player_When_Near::Created(GameObject *obj)
{
	weaponRange = 10.0f;
	const char *weap = Get_Current_Weapon(obj);
	if (weap)
	{
		const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(weap,true);
		if (ammo)
			weaponRange = ammo->Range;
	}
	attackUpdate = 0;
	targetId = 0;
	targetDistance = 0;
	followPlayerId[Get_Int_Parameter("MyId")] = 0;
	lastSeenTime = 0;
	followPlayerId[Get_Int_Parameter("MyId")] = 0;
	lastMoveSpeed = 1.0f;
	followDistance = Get_Float_Parameter("FollowDistance")*Get_Float_Parameter("FollowDistance");
	followMaxDistance = (Get_Float_Parameter("FollowDistance")+25.0f)*(Get_Float_Parameter("FollowDistance")+25.0f);
	createLocation = Commands->Get_Position(obj);
	Commands->Enable_Enemy_Seen(obj,Get_Int_Parameter("CanEngageEnemies") ? true : false);
	if (Get_Int_Parameter("TeamAsFollowingPlayer"))
		Commands->Set_Player_Type(obj,-2);
	Commands->Start_Timer(obj,this,1.0f,1);
	Commands->Start_Timer(obj,this,10.0f,2);
	Commands->Start_Timer(obj,this,11.0f,5);
	Commands->Start_Timer(obj,this,11.0f,6);
}
void JMG_Bear_Hunter_Engineer_Follow_Player_When_Near::Enemy_Seen(GameObject *obj,GameObject *seen)	
{
	if (Get_Vehicle(obj))
		return;
	if (seen->As_SmartGameObj() && seen->As_SmartGameObj()->Is_Stealthed() && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 900)
		return;
	if (targetId && lastSeenTime)
		return;
	float tempDist = JmgUtility::SimpleDistance(Commands->Get_Position(seen),Commands->Get_Position(obj));
	int seenId = Commands->Get_ID(seen);
	if (!targetId || tempDist < targetDistance)
	{
		targetDistance = tempDist;
		targetId = seenId;
		attackUpdate = 0;
	}
	if (targetId == seenId)
		lastSeenTime = 2;
}
void JMG_Bear_Hunter_Engineer_Follow_Player_When_Near::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		Vector3 myPos = Commands->Get_Position(obj);
		GameObject *player = JmgUtility::FindNearestPlayer(myPos);
		GameObject *playerVehicle = Get_Vehicle(player);
		if (Get_Vehicle(obj))
		{
			if (!playerVehicle)
			{
				Commands->Action_Reset(obj,100);
				Soldier_Transition_Vehicle(obj);
				Commands->Action_Reset(Get_Vehicle(obj),100);
			}
		}
		else
		{
			int tempPlayerId = JmgUtility::JMG_Get_Player_ID(player);
			float playerDistance = JmgUtility::SimpleDistance(myPos,Commands->Get_Position(player));
			if (player && playerVehicle && Get_Vehicle_Occupant_Count(playerVehicle) < Get_Vehicle_Seat_Count(playerVehicle))
			{
				if (JmgUtility::SimpleDistance(myPos,Commands->Get_Position(playerVehicle)) <= 25.0f)
				{
					targetId = 0;
					lastSeenTime = 0;
					attackUpdate = 0;
					Force_Vehicle_Entry(obj,playerVehicle);
				}
			}
			if (player && !Get_Vehicle(obj) && followPlayerId[Get_Int_Parameter("MyId")] ? (playerDistance < followMaxDistance) : (playerDistance < followDistance))
			{
				if (!followPlayerId[Get_Int_Parameter("MyId")] || (tempPlayerId != followPlayerId[Get_Int_Parameter("MyId")] && Get_Int_Parameter("AlwaysFollowNearestPlayer")))
				{
					if (Get_Int_Parameter("TeamAsFollowingPlayer"))
						Commands->Set_Player_Type(obj,Commands->Get_Player_Type(player));
					lastMoveSpeed = GetSpeed(player,playerDistance);
					followPlayerId[Get_Int_Parameter("MyId")] = tempPlayerId;
					ActionParamsStruct params;
					params.Set_Basic(this,100,10);
					params.Set_Movement(player,lastMoveSpeed,2.5f,false);
					params.MoveFollow = true;
					params.MovePathfind = playerDistance < 625 ? false : true;
					if (!targetId && Commands->Get_Random(0.0f,1.0f) <= 0.35f)
					{
						Vector3 pos = Commands->Get_Position(obj);
						pos.X += Commands->Get_Random(0.0f,1.0f) < 0.33f ? -500.0f : Commands->Get_Random(0.0f,1.0f) < 0.5f ? 500.0f : 0.0f;
						pos.Y += Commands->Get_Random(0.0f,1.0f) < 0.33f ? -500.0f : Commands->Get_Random(0.0f,1.0f) < 0.5f ? 500.0f : 0.0f;
						params.Set_Look(pos,30.0f);
					}
					Commands->Action_Goto(obj,params);
				}
			}
			else if (followPlayerId[Get_Int_Parameter("MyId")] != 0)
			{
				followPlayerId[Get_Int_Parameter("MyId")] = 0;
				if (Get_Int_Parameter("TeamAsFollowingPlayer"))
					Commands->Set_Player_Type(obj,-2);
				if (Get_Int_Parameter("ReturnToOriginalPosition"))
				{
					lastMoveSpeed = 0.9f;
					ActionParamsStruct params;
					params.Set_Basic(this,100,10);
					params.Set_Movement(createLocation,lastMoveSpeed,0.0f,false);
					params.MoveFollow = false;
					params.MovePathfind = true;
					if (!targetId && Commands->Get_Random(0.0f,1.0f) <= 0.35f)
					{
						Vector3 pos = Commands->Get_Position(obj);
						pos.X += Commands->Get_Random(0.0f,1.0f) < 0.33f ? -500.0f : Commands->Get_Random(0.0f,1.0f) < 0.5f ? 500.0f : 0.0f;
						pos.Y += Commands->Get_Random(0.0f,1.0f) < 0.33f ? -500.0f : Commands->Get_Random(0.0f,1.0f) < 0.5f ? 500.0f : 0.0f;
						params.Set_Look(pos,30.0f);
					}
					Commands->Action_Goto(obj,params);
				}
				else
					Commands->Action_Reset(obj,100.0f);
			}
			if (player)
			{
				float tempMoveSpeed = GetSpeed(player,playerDistance);
				if (tempMoveSpeed != lastMoveSpeed)
				{
					lastMoveSpeed = tempMoveSpeed;
					ActionParamsStruct params;
					Commands->Get_Action_Params(obj,params);
					params.MoveSpeed = lastMoveSpeed;
					params.MovePathfind = playerDistance < 625 ? false : true;
					if (!targetId && Commands->Get_Random(0.0f,1.0f) <= 0.35f)
					{
						Vector3 pos = Commands->Get_Position(obj);
						pos.X += Commands->Get_Random(0.0f,1.0f) < 0.33f ? -500.0f : Commands->Get_Random(0.0f,1.0f) < 0.5f ? 500.0f : 0.0f;
						pos.Y += Commands->Get_Random(0.0f,1.0f) < 0.33f ? -500.0f : Commands->Get_Random(0.0f,1.0f) < 0.5f ? 500.0f : 0.0f;
						params.Set_Look(pos,30.0f);
					}
					Commands->Action_Goto(obj,params);
				}
			}
			if (attackUpdate)
				attackUpdate--;
			if (lastSeenTime)
				lastSeenTime--;
			GameObject *target = Commands->Find_Object(targetId);
			if (targetId && (!target || !Commands->Get_Health(target)))
			{
				targetId = 0;
				lastMoveSpeed = GetSpeed(player,playerDistance);
				ActionParamsStruct params;
				params.Set_Basic(this,100,10);
				params.Set_Movement(player,lastMoveSpeed,2.5f,false);
				params.MoveFollow = true;
				params.MovePathfind = playerDistance < 625 ? false : true;
				Commands->Action_Goto(obj,params);
			}
			else if (target && Commands->Get_Health(target))
			{
				targetDistance = JmgUtility::SimpleDistance(Commands->Get_Position(target),myPos);
				if (!attackUpdate)
				{
					attackUpdate = 25;
					ActionParamsStruct params;
					Commands->Get_Action_Params(obj,params);
					params.Set_Attack(target,weaponRange,0.0f,true);
					Commands->Action_Attack(obj,params);
				}
			}
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
	if (number == 2)
	{
		Vector3 myPos = Commands->Get_Position(obj);
		GameObject *player = Get_GameObj(followPlayerId[Get_Int_Parameter("MyId")]);
		float playerDistance = player ? JmgUtility::SimpleDistance(myPos,Commands->Get_Position(player)) : 0.0f;
		if (player && playerDistance < followMaxDistance && !Get_Vehicle(obj))
		{
			lastMoveSpeed = GetSpeed(player,playerDistance);
			ActionParamsStruct params;
			params.Set_Basic(this,100,10);
			params.Set_Movement(player,lastMoveSpeed,2.5f,false);
			params.MoveFollow = true;
			params.MovePathfind = playerDistance < 625 ? false : true;
			Commands->Action_Goto(obj,params);
			GameObject *target = Commands->Find_Object(targetId);
			if (target && Commands->Get_Health(target))
			{
				params.Set_Attack(target,weaponRange,0.0f,true);
				Commands->Action_Attack(obj,params);
			}
		}
		Commands->Start_Timer(obj,this,5.0,2);
	}
	if (number == 5)
	{
		if (!targetId && Commands->Get_Random(0.0f,1.0f) <= 0.35f && !Get_Vehicle(obj))
		{
			ActionParamsStruct params;
			Commands->Get_Action_Params(obj,params);
			Vector3 pos = Commands->Get_Position(obj);
			float rotation = Commands->Get_Random(0,9)*45;
			pos.X += cos(rotation)*1000.0f;
			pos.Y += sin(rotation)*1000.0f;
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
		if (!targetId && JmgUtility::SimpleDistance(Vector3(),speed) <= 0.0f && !Get_Vehicle(obj))
		{
			ActionParamsStruct params;
			Commands->Get_Action_Params(obj,params);
			Vector3 pos = Commands->Get_Position(obj);
			float rotation = Commands->Get_Random(0,9)*45;
			pos.X += cos(rotation)*1000.0f;
			pos.Y += sin(rotation)*1000.0f;
			params.Set_Face_Location(pos,repeatTime);
			Commands->Action_Face_Location(obj,params);
		}
		Commands->Start_Timer(obj,this,repeatTime,6);
	}
}
void JMG_Bear_Hunter_Engineer_Follow_Player_When_Near::Killed(GameObject *obj,GameObject *killer)
{
	for (int x = 0;x < 5;x++)
		if (Commands->Get_ID(obj) == JMG_Bear_Hunter_Engineer_AI::engineerIds[x])
		{
			if (!JMG_Bear_Hunter_Engineer_AI::engineerMode[x])
				JMG_Bear_Hunter_Engineer_AI::engineersDead++;
			JMG_Bear_Hunter_Engineer_AI::scanAgainForEngineerDeaths = 1;
			break;
		}
}
inline float JMG_Bear_Hunter_Engineer_Follow_Player_When_Near::GetSpeed(GameObject *player,float distance)
{
	return !player ? 0.9f : (distance > 2500) ? 2.0f :  (distance > 1225) ? 1.5f : (distance > 625) ? 1.25f : (distance > 225) ? 1.0f : (distance > 100) ? 0.75f : 0.5f;
}
void JMG_Bear_Hunter_Engineer_AI::Created(GameObject *obj)
{
	const AmmoDefinitionClass *ammo = Get_Weapon_Ammo_Definition(Get_Parameter("RepaiarGun_Preset"),true);
	if (ammo)
		repairGunRange = ammo->Range;
	ammo = Get_Weapon_Ammo_Definition(Get_Parameter("Weapon_Preset"),true);
	if (ammo)
		weaponRange = ammo->Range;
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
	Commands->Start_Timer(obj,this,0.25f,1);
	Commands->Start_Timer(obj,this,1.0f,5);
	Commands->Start_Timer(obj,this,1.0f,6);
	Commands->Enable_Enemy_Seen(obj,true);
}
void JMG_Bear_Hunter_Engineer_AI::Enemy_Seen(GameObject *obj,GameObject *seen)	
{
	if (seen->As_SmartGameObj() && seen->As_SmartGameObj()->Is_Stealthed() && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 900)
		return;
	if (Is_Script_Attached(seen,"JMG_Bear_Hunter_AI_Ignore_Up_To_25_Meters") && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 625)
		return;
	if (targetId && targetUpdate)
		return;
	if (targetId && JmgUtility::SimpleDistance(Commands->Get_Position(seen),Commands->Get_Position(obj)) > targetDistance)
		return;
	if (JMG_Bear_Hunter_Game_Control::aiIgnorePlayers[JmgUtility::JMG_Get_Player_ID(seen)])
		return;
	targetId = Commands->Get_ID(seen);
	targetUpdate = 20;
}
void JMG_Bear_Hunter_Engineer_AI::Timer_Expired(GameObject *obj,int number)
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
				float tempDistance = JmgUtility::SimpleDistance(pos,Commands->Get_Position(o));
				if ((!repairTarget || tempDistance < nearestObject) && inRange(o) && !Is_Script_Attached(o,"JMG_Bear_Hunter_Engineer_Ignore"))
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
		else if (target)
			AttackTarget(obj,target,target,false,false);
		if (targetUpdate)
			targetUpdate--;
		if (actionUpdate)
			actionUpdate--;
		if (!target && !repairTarget && samePosition >= 10)
		{ 
			Rp2SimplePositionSystem::SimplePositionNode *posNode = (Commands->Get_Player_Type(obj) == 1 ? ((JMG_Bear_Hunter_Game_Control::turretsDestroyed >= 4 || !JMG_Bear_Hunter_Power_Transformer::online) ? aiDefensePoints[4].GetRandom() : aiDefensePoints[3].GetRandom()) : randomSelectableSpawnPoints[5].GetRandom());
			if (posNode)
			{
				Commands->Action_Reset(obj,100);
				ActionParamsStruct params;
				params.Set_Movement(posNode->position,1.0f,2.5f,false);
				params.MovePathfind = true;
				Commands->Action_Goto(obj,params);
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
		if (!targetId && !repairTargetId && Commands->Get_Random(0.0f,1.0f) <= 0.35f)
		{
			ActionParamsStruct params;
			Commands->Get_Action_Params(obj,params);
			Vector3 pos = Commands->Get_Position(obj);
			float rotation = Commands->Get_Random(0,9)*45;
			pos.X += cos(rotation)*1000.0f;
			pos.Y += sin(rotation)*1000.0f;
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
			pos.X += cos(rotation)*1000.0f;
			pos.Y += sin(rotation)*1000.0f;
			params.Set_Face_Location(pos,repeatTime);
			Commands->Action_Face_Location(obj,params);
		}
		Commands->Start_Timer(obj,this,repeatTime,6);
	}
}
void JMG_Bear_Hunter_Engineer_AI::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage <= 0 || !damager || obj == damager || (targetId && targetUpdate) || targetId == Commands->Get_ID(damager) || Commands->Get_Player_Type(damager) == Commands->Get_Player_Type(obj))
		return;
	if (JMG_Bear_Hunter_Game_Control::aiIgnorePlayers[JmgUtility::JMG_Get_Player_ID(damager)])
		return;
	if (targetId && JmgUtility::SimpleDistance(Commands->Get_Position(damager),Commands->Get_Position(obj)) > targetDistance)
		return;
	targetId = Commands->Get_ID(damager);
	targetUpdate = 40;
}
void JMG_Bear_Hunter_Engineer_AI::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason)
{
}
void JMG_Bear_Hunter_Engineer_AI::Killed(GameObject *obj,GameObject *killer)
{
}
void JMG_Bear_Hunter_Engineer_AI::AttackTarget(GameObject *obj,GameObject *target,GameObject *secondaryTarget,bool repairTarget,bool useRepairGun)
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
		params.Set_Movement(target,1.25f,repairGunRange*0.5f,false);
	else if (secondaryTarget)
		params.Set_Movement(secondaryTarget,1.0f,weaponRange*0.5f,false);
	else
		params.Set_Movement(moveLocation,1.0f,repairGunRange*0.5f,false);
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
	else if (target && ((repairTarget && useRepairGun) || (!repairTarget && !useRepairGun)))
	{
		params.MoveObject = target;
		params.Set_Attack(target,repairGun ? repairGunRange : weaponRange,0.0f,true);
		Commands->Action_Attack(obj,params);
	}
}
bool JMG_Bear_Hunter_Engineer_AI::inRange(GameObject *obj)
{
	Vector3 pos = Commands->Get_Position(obj);
	float dist = JmgUtility::SimpleFlatDistance(pos,Vector3(-0.389f,-705.611f,0.0f));
	if (dist < 27390.25)
		return true;
	if (dist >= 27390.25 && pos.Z >= 11.0f && dist < 36100.0f)
		return true;
	return false;
}
bool JMG_Bear_Hunter_Engineer_AI::Valid_Repair_Target(GameObject *obj,GameObject *target,int playerType)
{
	if (!target || target == obj || Commands->Get_Player_Type(target) != playerType || Get_Hitpoints(target) >= Get_Max_Hitpoints(target))
		return false;
	if (target->As_SoldierGameObj() && Get_Hitpoints(target) > Get_Max_Hitpoints(target)*0.5f)
		return false;
	if (target->As_VehicleGameObj() && Get_Vehicle_Mode(target) != VEHICLE_TYPE_TURRET && Get_Hitpoints(target) > Get_Max_Hitpoints(target)*0.75f)
		return false;
	if (target->As_VehicleGameObj() && Get_Vehicle_Mode(target) == VEHICLE_TYPE_TURRET && (Get_Hitpoints(target) > Get_Max_Hitpoints(target)*0.9f || !JMG_Bear_Hunter_Power_Transformer::online))
		return false;
	if (!target->As_SoldierGameObj() || (target->As_SoldierGameObj() && !Get_Vehicle(target)))
		return true;
	return false;
}
void JMG_Bear_Hunter_Oil_Rig::Created(GameObject *obj)
{
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
	active = false;
	char animation[32];
	sprintf(animation,"%sd",Get_Parameter("Animation"));
	Commands->Set_Animation(obj,animation,false,0,-1,Commands->Get_Random(0.0f,170.0f),false);
	Commands->Start_Timer(obj,this,0.1f,1);
	if (Get_Int_Parameter("StartsEnabled"))
		Poked(obj,obj);
	else
	{
		int id = Commands->Get_ID(obj);
		for (int x = 0;x < 7;x++)
			if (!JMG_Bear_Hunter_Oil_Rig::pumpJacks[x].pumpId || JMG_Bear_Hunter_Oil_Rig::pumpJacks[x].pumpId == id)
			{
				JMG_Bear_Hunter_Oil_Rig::pumpJacks[x] = JMG_Bear_Hunter_Oil_Rig::PumpjackNode(id);
				break;
			}
	}
}
void JMG_Bear_Hunter_Oil_Rig::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		float frame = Get_Animation_Frame(obj);
		if (active && frame >= Get_Float_Parameter("StartFrame") && frame <= Get_Float_Parameter("EndFrame"))
		{
			for (int x = 1;x < 128;x++)
			{
				GameObject *player = Get_GameObj(x);
				if (!player)
					continue;
				Commands->Give_Money(player,0.15f,false);
			}
			for (int x = 0;x < maxTotalBearHunterPlayerAssistAI;x++)
				JMG_Bear_Hunter_Player_Assist_AI::cash[x] += 0.3f;
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Bear_Hunter_Oil_Rig::Poked(GameObject *obj, GameObject *poker)
{
	if (Commands->Get_Player_Type(poker) == -4)
		return;
	if (!active)
	{
		Commands->Enable_HUD_Pokable_Indicator(obj,false);
		active = true;
		Commands->Set_Animation(obj,Get_Parameter("Animation"),true,0,Get_Animation_Frame(obj),-1,true);
		if (!Get_Int_Parameter("StartsEnabled"))
		{
			int id = Commands->Get_ID(obj);
			for (int x = 0;x < 7;x++)
				if (JMG_Bear_Hunter_Oil_Rig::pumpJacks[x].pumpId == id)
				{
					JMG_Bear_Hunter_Oil_Rig::pumpJacks[x].active = true;
					break;
				}
			oilRigCount++;
			if (oilRigCount == 7)
			{
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Thats all of them, great work guys!");
				NewObjectiveSystemControl.Set_Objective_Status(16,NewObjectiveSystem::Accomplished);
			}
			else
			{
				char successMsg[220];
				sprintf(successMsg,"Bear Rug Co: Oil rig on-line, %d more to go.",7-oilRigCount);
				JmgUtility::MessageAllPlayers(127,127,255,successMsg);
				unsigned int textId = (oilRigCount == 1) ? 12752 : (oilRigCount == 2) ? 12753 : (oilRigCount == 3) ? 12754 : (oilRigCount == 4) ? 12755 : 12756;
				NewObjectiveSystemControl.Set_Objective_Mission(16,textId,textId);
			}
			Commands->Give_Points(poker,250.0f,false);
			BearHunterScoreSystem::BHScoreNode *node = bearHunterScoreSystem.Get_Current_Player_Score_Node(JmgUtility::JMG_Get_Player_ID(poker));
			node->BonusObjectivesCompleted++;
			node->totalObjectivesCompleted++;
			node->ObjectiveOilRigsActivated++;
		}
	}
}
void JMG_Bear_Hunter_Oil_Rig::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *rig = Commands->Create_Object("Oil_Well_Destroyed",Commands->Get_Position(obj));
	Commands->Set_Facing(rig,Commands->Get_Facing(obj));
	char model[32];
	sprintf(model,"DeadRig%0d",Commands->Get_Random_Int(0,5));
	Commands->Set_Model(rig,model);
}
void JMG_Bear_Hunter_Damaged_Oil_Rig::Created(GameObject *obj)
{
	rigCount = 3;
	repaired = false;
	Commands->Set_Animation(obj,Get_Parameter("Animation"),true,0,0.0f,-1,true);
}
void JMG_Bear_Hunter_Damaged_Oil_Rig::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (repaired)
		return;
	if (Commands->Get_Health(obj) == Commands->Get_Max_Health(obj))
	{
		repaired = true;
		GameObject *rig = Commands->Create_Object("Oil_Well",Commands->Get_Position(obj));
		Commands->Set_Facing(rig,Commands->Get_Facing(obj));
		char params[256];
		sprintf(params,"%s,%.2f,%.2f,1",Get_Parameter("RepairAnimation"),Get_Float_Parameter("StartFrame"),Get_Float_Parameter("EndFrame"));
		Commands->Attach_Script(rig,"JMG_Bear_Hunter_Oil_Rig",params);
		if (NewObjectiveSystemControl.Set_Objective_Status(17,NewObjectiveSystem::Accomplished))
		{
			Commands->Give_Points(damager,250.0f,false);
			BearHunterScoreSystem::BHScoreNode *node = bearHunterScoreSystem.Get_Current_Player_Score_Node(JmgUtility::JMG_Get_Player_ID(obj));
			node->BonusObjectivesCompleted++;
			node->totalObjectivesCompleted++;
			node->ObjectiveOilRigsRepaired++;
		}
		Commands->Destroy_Object(obj);
	}
}
void JMG_Bear_Hunter_Damaged_Oil_Rig::Killed(GameObject *obj,GameObject *killer)
{
	GameObject *rig = Commands->Create_Object("Oil_Well_Destroyed",Commands->Get_Position(obj));
	Commands->Set_Facing(rig,Commands->Get_Facing(obj));
	char model[32];
	sprintf(model,"DeadRig%0d",Commands->Get_Random_Int(0,5));
	Commands->Set_Model(rig,model);
	rigCount--;
	if (!rigCount)
		NewObjectiveSystemControl.Set_Objective_Status(17,NewObjectiveSystem::Failed);
}
void JMG_Complete_Objective_On_Poke::Poked(GameObject *obj, GameObject *poker)
{
	if (Commands->Get_Player_Type(poker) == -4)
		return;
	if (!(NewObjectiveSystemControl.Get_Objective_Status(Get_Int_Parameter("ObjectiveId")) == NewObjectiveSystem::Pending || NewObjectiveSystemControl.Get_Objective_Status(Get_Int_Parameter("ObjectiveId")) == NewObjectiveSystem::Hidden))
		return;
	NewObjectiveSystemControl.Set_Objective_Status(Get_Int_Parameter("ObjectiveId"),NewObjectiveSystem::Accomplished);
	if (Get_Float_Parameter("TeamPayout"))
		JMG_Bear_Hunter_Game_Control::ObjectiveCompleteReward(Get_Float_Parameter("TeamPayout"));
	if (Get_Float_Parameter("PlayerPoints"))
		Commands->Give_Points(poker,Get_Float_Parameter("PlayerPoints"),false);
}
void JMG_Bear_Hunter_Weapons_Container_Spawn_Point::Created(GameObject *obj)
{
	randomWeaponContainerSpawnPositions[Get_Int_Parameter("GroupId")] += obj;
	Commands->Destroy_Object(obj);
}
void JMG_Bear_Hunter_Weapons_Container::Created(GameObject *obj)
{
	createLocation = Commands->Get_Position(obj);
}
void JMG_Bear_Hunter_Weapons_Container::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		if (Get_Int_Parameter("WeaponType") < 6 && !JMG_Bear_Hunter_Game_Control::weaponsFound[Get_Int_Parameter("WeaponType")])
		{
			JMG_Bear_Hunter_Game_Control::weaponsFound[Get_Int_Parameter("WeaponType")] = true;
			int count = 0;
			for (int x = 0;x < 6;x++)
				if (JMG_Bear_Hunter_Game_Control::weaponsFound[x])
					count++;
			if (count == 6)
				NewObjectiveSystemControl.Set_Objective_Status(18,NewObjectiveSystem::Accomplished);
			else
				NewObjectiveSystemControl.Set_Objective_Mission(18,12772+count,12772+count);
			Commands->Give_Points(sender,250.0f,false);
			BearHunterScoreSystem::BHScoreNode *node = bearHunterScoreSystem.Get_Current_Player_Score_Node(JmgUtility::JMG_Get_Player_ID(obj));
			node->BonusObjectivesCompleted++;
			node->totalObjectivesCompleted++;
			node->ObjectiveWeaponsFound++;
			JMG_Bear_Hunter_Game_Control::weaponsReturned[Get_Int_Parameter("WeaponType")] = true;
			count = 0;
			for (int x = 0;x < 6;x++)
				if (JMG_Bear_Hunter_Game_Control::weaponsReturned[x])
					count++;
			if (count == 1)
				NewObjectiveSystemControl.Add_Objective(19,NewObjectiveSystem::Bonus,NewObjectiveSystem::Pending,12784,"",12784,2);
			else
				NewObjectiveSystemControl.Set_Objective_Mission(19,12785-count,12785-count);
		}
		char params[512];
		sprintf(params,"%d,%.2f %.2f %.2f",Get_Int_Parameter("WeaponType"),createLocation.X,createLocation.Y,createLocation.Z);
		Commands->Attach_Script(sender,"JMG_Bear_Hunter_Weapons_Container_Attached",params);
	}
}
void JMG_Bear_Hunter_Weapons_Container_Attached::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,2.5f,1);
}
void JMG_Bear_Hunter_Weapons_Container_Attached::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (JmgUtility::SimpleFlatDistance(Commands->Get_Position(obj),Vector3(-0.389f,-705.611f,0.0f)) < 27390.25)
		{
			for (int x = 1;x < 128;x++)
			{
				GameObject *player = Get_GameObj(x);
				if (!player)
					continue;
				Commands->Send_Custom_Event(player,player,454270,Get_Int_Parameter("WeaponType"),0);
			}
			JMG_Bear_Hunter_Game_Control::weaponsUnlocked[Get_Int_Parameter("WeaponType")] = true;
			switch (Get_Int_Parameter("WeaponType"))
			{
			case 0:JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Good work, all infantry classes now have a Twin Bow.");break;
			case 1:JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Great work, basic infantry now have a 10 gauge shotgun.");break;
			case 2:JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Due to your hard work, tier 1 infantry now have a FAMAS F1.");break;
			case 3:JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Excellent work, tier 2 infantry now have the PPSh-41!");break;
			case 4:JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Enjoy the AA12 with your tier 3 infantry!");break;
			case 5:JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Engineers can now use the Beretta 93R.");break;
			case 6:JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Wow, looks like that bear had blueprints for a plasma rifle, all classes now have a plasma rifle!");break;
			}
			if (Get_Int_Parameter("WeaponType") < 6)
			{
				JMG_Bear_Hunter_Game_Control::weaponsReturned[Get_Int_Parameter("WeaponType")] = false;
				int count = 0;
				for (int x = 0;x < 6;x++)
					if (JMG_Bear_Hunter_Game_Control::weaponsReturned[x])
						count++;
				if (!count)
				{
					NewObjectiveSystemControl.Set_Objective_Status(19,NewObjectiveSystem::Accomplished);
					NewObjectiveSystemControl.Remove_Objective(19);
				}
				else
					NewObjectiveSystemControl.Set_Objective_Mission(19,12785-count,12785-count);
				BearHunterScoreSystem::BHScoreNode *node = bearHunterScoreSystem.Get_Current_Player_Score_Node(JmgUtility::JMG_Get_Player_ID(obj));
				node->BonusObjectivesCompleted++;
				node->totalObjectivesCompleted++;
				node->ObjectiveWeaponsReturned++;
				Commands->Give_Points(obj,250.0f,false);
			}
			if (Get_Int_Parameter("WeaponType") == 6)
			{
				BearHunterScoreSystem::BHScoreNode *node = bearHunterScoreSystem.Get_Current_Player_Score_Node(JmgUtility::JMG_Get_Player_ID(obj));
				node->BonusObjectivesCompleted++;
				node->totalObjectivesCompleted++;
				node->ObjectivePlasmaRifleReturned++;
				NewObjectiveSystemControl.Set_Objective_Status(20,NewObjectiveSystem::Accomplished);
				Commands->Give_Points(obj,250.0f,false);
			}
			drop = false;
			const char *weap = Get_Current_Weapon(obj);
			if (weap)
			{
				if (!_stricmp(weap,"Weapon_Weapons_Container"))
					Commands->Select_Weapon(obj,"Weapon_Pistol_Player");
				Remove_Weapon(obj,"Weapon_Weapons_Container");
			}
			Remove_Script(obj,"JMG_Bear_Hunter_Weapons_Container_Attached");
			return;
		}
		Commands->Start_Timer(obj,this,2.5f,1);
	}
}
void JMG_Bear_Hunter_Weapons_Container_Attached::Destroyed(GameObject *obj)
{
	if (!drop)
		return;
	GameObject *powerup = Commands->Create_Object("POW_Weapons_Container",Get_Vector3_Parameter("CreateLocation"));
	Commands->Set_Facing(powerup,Commands->Get_Random(-180.0f,180.0f));
	char params[512];
	sprintf(params,"%d",Get_Int_Parameter("WeaponType"));
	Commands->Attach_Script(powerup,"JMG_Bear_Hunter_Weapons_Container",params);
}
void JMG_Bear_Hunter_Giant_Deer_Boss::Created(GameObject *obj)
{
	Set_Max_Health(obj,775.0f);
	Set_Max_Shield_Strength(obj,775.0f);
	Commands->Set_Health(obj,775.0f);
	Commands->Set_Shield_Strength(obj,775.0f);
	damaged = false;
	returnHome = 0;
	PlayerCount = 1;
	CanHearDelay = 0;
	LastSeen = 0;
	currentTargetID = 0;
	waitcount = 0;
	lastPos = homelocation = Commands->Get_Position(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,5.0f,1);
	Commands->Start_Timer(obj,this,0.1f,2);
}
void JMG_Bear_Hunter_Giant_Deer_Boss::Enemy_Seen(GameObject *obj,GameObject *seen)	
{
	if (seen->As_SmartGameObj() && seen->As_SmartGameObj()->Is_Stealthed() && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 900)
		return;
	if (Is_Script_Attached(seen,"JMG_Bear_Hunter_AI_Ignore"))
		return;
	if (Is_Script_Attached(seen,"JMG_Bear_Hunter_AI_Ignore_Up_To_75_Meters") && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 5625)
		return;
	if (Is_Script_Attached(seen,"JMG_Bear_Hunter_AI_Ignore_Up_To_150_Meters") && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 22500)
		return;
	if (Is_Script_Attached(seen,"JMG_AI_Ignore_Object"))
		return;
	if (JmgUtility::SimpleDistance(homelocation,Commands->Get_Position(seen)) > 40000.0f)
		return;
	int SeenID = Commands->Get_ID(seen);
	if (!currentTargetID)
	{
		if (JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) <= 1.567504)
			return;
		LastSeen = 30;
		currentTargetID = SeenID;
	}
	else if (currentTargetID == SeenID)
	{
		if (JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) <= 1.567504)
			LastSeen = 1;
		else
			LastSeen = 30;
	}
	if (NewObjectiveSystemControl.Get_Objective_Status(20) == NewObjectiveSystem::NotDefined)
	{
		JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: That deer looks real exotic, kill it so we can stuff it and sell it for big bucks!!");
		NewObjectiveSystemControl.Add_Objective(20,NewObjectiveSystem::Bonus,NewObjectiveSystem::Pending,12786,"",12786,6);
	}
}
void JMG_Bear_Hunter_Giant_Deer_Boss::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 91185423)
	{
		GameObject *Player = JmgUtility::FindNearestPlayer(Commands->Get_Position(obj));
		if (!Player)
			return;
		ActionParamsStruct params;
		params.Set_Movement(Commands->Get_Position(Player),1.0,10.0f,false);
		params.MovePathfind = true;
		Commands->Action_Goto(obj,params);
	}
}
void JMG_Bear_Hunter_Giant_Deer_Boss::Timer_Expired(GameObject *obj,int number)
{
	if (2 == number)
	{
		if (CanHearDelay)
			CanHearDelay--;
		if (LastSeen)
		{
			LastSeen--;
			if (!LastSeen && currentTargetID)
			{
				Commands->Action_Reset(obj,10);
				currentTargetID = 0;
			}
		}
		if (currentTargetID)
		{
			GameObject *target = Commands->Find_Object(currentTargetID);
			if (!target || Commands->Get_Health(target) == 0.0f)
				ReturnHome(obj);
			else if (JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(target)) > 40000.0f || JmgUtility::SimpleDistance(Commands->Get_Position(obj),homelocation) > 40000.0f)
				ReturnHome(obj);
			else
			{
				waitcount++;
				if (waitcount > 2)
				{
					waitcount = 0;
					ActionParamsStruct params;
					Commands->Get_Action_Params(obj,params);
					params.Set_Movement(target,1.0,15.0f,false);
					params.MovePathfind = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(target)) < 25.0 ? false : true;
					Commands->Action_Goto(obj,params);
					params.Set_Basic(this,999,10);
					if (Get_Vehicle(target) || target->As_VehicleGameObj())
					{
						params.AttackCheckBlocked = false;
						params.Set_Attack(target,30.0f,1,true);
					}
					else
						params.Set_Attack(target,30.0f,1,true);
					Commands->Action_Attack(obj,params);
				}
			}
		}
		else if (JmgUtility::SimpleDistance(Commands->Get_Position(obj),homelocation) > 40000.0f)
			ReturnHome(obj);
		Commands->Start_Timer(obj,this,0.1f,2);
	}
	if (1 == number)
	{
		Vector3 Pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(lastPos,Pos) < 2.0 && JmgUtility::SimpleDistance(lastPos,homelocation) > 25.0f)
		{
			returnHome++;
			if (returnHome > 10)
				Commands->Set_Position(obj,homelocation);
			else if (returnHome > 3)
				ReturnHome(obj);
		}
		else
			returnHome = 0;
		lastPos = Pos;
		int TempPlayerCount = Get_Player_Count();
		if (PlayerCount < TempPlayerCount)
		{
			float additionalHealth = (TempPlayerCount-PlayerCount)*50.0f;
			Set_Max_Health(obj,Commands->Get_Max_Health(obj)+additionalHealth);
			Set_Max_Shield_Strength(obj,Commands->Get_Max_Shield_Strength(obj)+additionalHealth);
			Commands->Set_Health(obj,Commands->Get_Health(obj)+additionalHealth);
			Commands->Set_Shield_Strength(obj,Commands->Get_Shield_Strength(obj)+additionalHealth);
			PlayerCount = TempPlayerCount;
		}
		Commands->Start_Timer(obj,this,5.0f,1);
	}
}
void JMG_Bear_Hunter_Giant_Deer_Boss::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damager || Commands->Get_Player_Type(damager) != 1)
		return;
	if (Is_Script_Attached(damager,"JMG_AI_Ignore_Object"))
		return;
	if (JmgUtility::SimpleDistance(homelocation,Commands->Get_Position(damager)) > 40000.0f)
	{
		if (damage > 0)
			Commands->Apply_Damage(obj,-damage,"None",damager);
		return;
	}
	if (!damaged && damage > 0)
		damaged = true;
	GameObject *target = Commands->Find_Object(currentTargetID);
	Vector3 pos = Commands->Get_Position(obj);
	if (target)
	{
		float targetDistance = JmgUtility::SimpleDistance(pos,Commands->Get_Position(target));
		float damagerDistance = JmgUtility::SimpleDistance(pos,Commands->Get_Position(damager));
		if (targetDistance < damagerDistance)
			return;
	}
	if (JmgUtility::SimpleDistance(pos,Commands->Get_Position(damager)) <= 1.567504)
		return;
	LastSeen = 120;
	waitcount = 0;
	currentTargetID = Commands->Get_ID(damager);
	if (NewObjectiveSystemControl.Get_Objective_Status(20) == NewObjectiveSystem::NotDefined)
	{
		JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: That deer looks real exotic, kill it so we can stuff it and sell it for big bucks!");
		NewObjectiveSystemControl.Add_Objective(20,NewObjectiveSystem::Bonus,NewObjectiveSystem::Pending,12786,"",12786,6);
	}
}
void JMG_Bear_Hunter_Giant_Deer_Boss::Killed(GameObject *obj,GameObject *killer)
{
	JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: Good work killing that exotic deer, grab its carcass and return it to base and we’ll swap it out for a new weapon prototype we’re working on.");
	NewObjectiveSystemControl.Set_Objective_Mission(20,12787,12787);
	Commands->Give_Points(killer,250.0f,false);
	Vector3 pos = Commands->Get_Position(obj);
	pos.Z += 0.5f;
	GameObject *powerup = Commands->Create_Object("POW_Weapons_Container",pos);
	Commands->Set_Facing(powerup,Commands->Get_Random(-180.0f,180.0f));
	Commands->Attach_Script(powerup,"JMG_Bear_Hunter_Weapons_Container","6");
}
void JMG_Bear_Hunter_Giant_Deer_Boss::ReturnHome(GameObject *obj)
{
	currentTargetID = 0;
	waitcount = 0;
	ActionParamsStruct params;
	params.Set_Basic(this,999,10);
	params.MovePathfind = true;
	params.Set_Movement(homelocation,2.0f,0,false);
	Commands->Action_Goto(obj,params);
}
void JMG_Bear_Hunter_Kill_Score_Tracker::Killed(GameObject *obj,GameObject *killer)
{
	if (Commands->Get_Preset_ID(killer) == 1000001536)
	{
		int DamagerID = Commands->Get_ID(killer);
		for (int x = 1;x < 128;x++)
			if (PlayerData.players[x] && PlayerData.players[x]->sentryTurretId == DamagerID)
			{
				bearHunterScoreSystem.Get_Current_Player_Score_Node(x)->KillsWithSentryTurret++;
				GameObject *player = Get_GameObj(x);
				if (player)
				{
					Commands->Give_Points(player,Get_Death_Points(obj)*0.5f,false);
					cPlayer *thePlayer = Find_Player(x);
					thePlayer->Set_Kills(thePlayer->Get_Kills()+1);
				}
			}
		return;
	}
	if (!Commands->Is_A_Star(killer))
		return;
	BearHunterScoreSystem::BHScoreNode *node = bearHunterScoreSystem.Get_Current_Player_Score_Node(JmgUtility::JMG_Get_Player_ID(killer));
	if (!node)
		return;
	if (Get_Vehicle(killer))
		node->VehicleKills++;
	node->Kills++;
	if (killer == obj)
		node->KilledSelf++;
	else if (Commands->Is_A_Star(obj))
		node->KilledPlayers++;
	else
		switch (Commands->Get_Preset_ID(obj))
		{
		case 1000001024:node->KilledBears++;break;
		case 1000001476:node->KilledBlackBears++;break;
		case 1000001022:node->KilledMutantBears++;break;
		case 1000000771:node->KilledMutantDeer++;break;
		case 1000000774:node->KilledMutantCats++;break;
		case 1000001042:node->KilledMutantCatsB++;break;
		case 1000001044:node->KilledMutantCatsR++;break;
		case 1000001183:node->KilledMutantRabbits++;break;
		case 1000001536:node->KilledSentryTurrets++;break;
		case 1000001784:node->GiantDeerKilled++;break;
		case 1000001794:node->WolfKilled++;break;
		case 1000001809:node->MutantDogKilled++;break;
		case 1000001845:node->BlueDeerKilled++;break;
		case 1000001959:node->MooseKilled++;break;
		default:node->KilledHumanAi++;break;
		}
}
void JMG_Bear_Hunter_Powerup_Score_Control::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		BearHunterScoreSystem::BHScoreNode *node = bearHunterScoreSystem.Get_Current_Player_Score_Node(JmgUtility::JMG_Get_Player_ID(sender));
		node->PickedupTotalPowerups++;
		node->totalPowerupsPickedup++;
		switch (Get_Int_Parameter("PowerupType"))
		{
		case 0:
			node->PickedupHealthPowerups++;
			node->PickedupHealthTotal += Get_Int_Parameter("Value");
			break;
		case 1:
			node->PickedupArmorPowerups++;
			node->PickedupArmorTotal += Get_Int_Parameter("Value");
			break;
		case 2:
			node->PickedupCashPowerups++;
			node->PickedupCashTotal += Get_Int_Parameter("Value");
			break;
		case 3:
			node->PickedupAmmoPowerups++;
			node->PickedupAmmoTotal += Get_Int_Parameter("Value");
			break;
		case 4:
			if (!JMG_Bear_Hunter_Player_Soldier::hasGrenadeVest[JmgUtility::JMG_Get_Player_ID(sender)])
				node->C4VestPowerups++;
			break;
		}
	}
}
void JMG_Bear_Hunter_Increase_Score_On_Poke::Poked(GameObject *obj, GameObject *poker)
{
	if (Commands->Get_Player_Type(poker) == -4)
		return;
	BearHunterScoreSystem::BHScoreNode *node = bearHunterScoreSystem.Get_Current_Player_Score_Node(JmgUtility::JMG_Get_Player_ID(poker));
	switch (Get_Int_Parameter("ScoreType"))
	{
	case 0:
		node->BonusObjectivesCompleted++;
		node->totalObjectivesCompleted++;
		node->ObjectiveActivatedAlarm++;
		break;
	case 1:
		node->BonusObjectivesCompleted++;
		node->totalObjectivesCompleted++;
		node->ObjectiveTurretTruckAlarm++;
		break;
	}
	if (Is_Script_Attached(obj,"JMG_Bear_Hunter_Increase_Score_On_Poke"))
		Remove_Script(obj,"JMG_Bear_Hunter_Increase_Score_On_Poke");
}
void JMG_Bear_Hunter_Radio_Tower_Control::Created(GameObject *obj)
{
	JMG_Bear_Hunter_Radio_Tower_Control::destroyed = false;
	Commands->Set_Animation(obj,"radiotower.RadioTowerO",true,0,0.0f,-1,true);
	GameObject *switchCtrl = Commands->Create_Object("Radio_Tower_Switch",Vector3(-512.5f,-107.5f,76.0f));
	Commands->Attach_To_Object_Bone(switchCtrl,obj,"switchBox");
	JMG_Bear_Hunter_Radio_Tower_Control::radioTowerId = Commands->Get_ID(obj);
}
void JMG_Bear_Hunter_Radio_Tower_Control::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 57457)
	{
		if (!sender)
			return;
		Commands->Attach_To_Object_Bone(sender,NULL,"origin");
	}
	if (message == 57458)
	{
		if (JMG_Bear_Hunter_Radio_Tower_Control::destroyed)
			return;
		JMG_Bear_Hunter_Game_Control::hasActivatedTower = true;
		GameObject *Comanche = Commands->Create_Object("AI_Guardian_Comanche",Vector3(0.0f,1000.0f,75.0f));
		Commands->Attach_Script(Comanche,"JMG_Bear_Hunter_Guardian_Aircraft","10.0,Weapon_Comanche_Rocket,v_comanche.v_comanchec,v_comanche.v_comancheo,v_comanche.v_comanches,v_comanche.v_comanched");
		Commands->Attach_Script(Comanche,"JMG_Bear_Hunter_Comanche","");
		JmgUtility::MessageAllPlayers(127,127,255,"Comanche 4573: This is Comanche 4573 answering your distress call, we will be there shortly to provide fire support, just hold on!");
		NewObjectiveSystemControl.Set_Objective_Status(21,NewObjectiveSystem::Accomplished);
	}
	if (message == 57459)
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player)
				continue;
			Vector3 pos = Commands->Get_Position(player);
			MoveablePhysClass *mphys = player->As_PhysicalGameObj() ? player->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
			if (mphys && !mphys->Can_Teleport(Matrix3D(pos)))
			{
				mphys->Find_Teleport_Location(pos,2.5f,&pos);
				Commands->Set_Position(player,pos);
			}
		}
	}
}
void JMG_Bear_Hunter_Radio_Tower_Switch::Created(GameObject *obj)
{
	activated = false;
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
	Commands->Set_Animation(obj,"RdoTwrSwtch.RdoTwrSwtch",false,0,0.0f,0.0f,false);
}
void JMG_Bear_Hunter_Radio_Tower_Switch::Poked(GameObject *obj, GameObject *poker)
{
	if (activated)
		return;
	if (Commands->Get_Player_Type(poker) == -4)
		return;
	if (NewObjectiveSystemControl.Get_Objective_Status(21) == NewObjectiveSystem::NotDefined)
	{
		JmgUtility::DisplayChatMessage(poker,127,127,255,"Bear Rug Co: We have the Communications Tower turned off right now because it was scaring off the wild life, leave it off!");
		return;
	}
	if (NewObjectiveSystemControl.Get_Objective_Status(21) == NewObjectiveSystem::Removed)
	{
		JmgUtility::DisplayChatMessage(poker,127,127,255,"Bear Rug Co: Sorry, its too late to activate the tower now.");
		return;
	}
	Commands->Enable_HUD_Pokable_Indicator(obj,false);
	Commands->Give_Points(obj,250.0f,false);
	activated = true;
	JmgUtility::MessageAllPlayers(127,127,255,"Distress Signal Transmission In Progress: We are under attack by mutants on Bear Island and we need help!");
	NewObjectiveSystemControl.Set_Objective_Mission(21,12790,12790);
	Commands->Set_Animation(obj,"RdoTwrSwtch.RdoTwrSwtch",false,0,2.0f,2.0f,false);
	GameObject *radioTower = Commands->Find_Object(JMG_Bear_Hunter_Radio_Tower_Control::radioTowerId);
	if (radioTower)
	{
		Commands->Set_Animation(radioTower,"radiotower.RadioTowerA",true,0,0.0f,-1,true);
		Commands->Send_Custom_Event(obj,radioTower,57458,0,85.0f+Get_Player_Count()*5.0f);
	}
	bearHunterScoreSystem.Get_Current_Player_Score_Node(JmgUtility::JMG_Get_Player_ID(poker))->ActivatedCommTower++;
	GameObject *defendPoint = Commands->Create_Object("Radio_Tower_Base_Support",Vector3(-507.953f,-273.623f,0.91f));
	JMG_Bear_Hunter_Radio_Tower_Switch::supportId = Commands->Get_ID(defendPoint);
	Commands->Attach_Script(defendPoint,"JMG_Bear_Hunter_Radio_Tower_Support","");
}
void JMG_Bear_Hunter_Radio_Tower_Support::Killed(GameObject *obj,GameObject *killer)
{
	JMG_Bear_Hunter_Radio_Tower_Switch::supportId = 0;
	GameObject *radioTower = Commands->Find_Object(JMG_Bear_Hunter_Radio_Tower_Control::radioTowerId);
	if (!radioTower)
		return;
	Commands->Send_Custom_Event(radioTower,radioTower,57459,0,5.0f);
	if (NewObjectiveSystemControl.Get_Objective_Status(21) != NewObjectiveSystem::Accomplished)
	{
		JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: The communications tower was destroyed before anyone could answer our distress call!");
		NewObjectiveSystemControl.Set_Objective_Status(21,NewObjectiveSystem::Failed);
	}
	JMG_Bear_Hunter_Radio_Tower_Control::destroyed = true;
	Commands->Set_Animation(radioTower,"radiotower.RadioTowerD",false,0,0.0f,120.0f,false);
	Commands->Static_Anim_Phys_Goto_Last_Frame(601750,"RadioTowerH.RadioTowerHD");
	for (int x = 1;x < 128;x++)
	{
		float dist = -1.0f;
		char bestBone[32];
		GameObject *player = Get_GameObj(x);
		if (!player)
			continue;
		Vector3 pos = Commands->Get_Position(player);
		if (!(pos.X < -508.5f && pos.X > -517.5f && pos.Y < -265.205f && pos.Y > -274.205f))
			continue;
		for (int y = 0;y < 158;y++)
		{
			char boneName[32];
			sprintf(boneName,"attach%02d",y);
			float tempDist = JmgUtility::SimpleDistance(Commands->Get_Bone_Position(radioTower,boneName),pos);
			if (dist == -1 || tempDist < dist)
			{
				dist = tempDist;
				sprintf(bestBone,"%s",boneName);
			}
		}
		if (dist != -1 && dist < 6.25)
		{
			Commands->Set_Animation(player,"s_a_human.h_a_a0a0",false,0,0.0f,0.0f,false);
			Commands->Attach_To_Object_Bone(player,radioTower,bestBone);
			Commands->Send_Custom_Event(player,radioTower,57457,0,4);
		}
	}
}
void JMG_Bear_Hunter_Death_Water_Zone::Entered(GameObject *obj,GameObject *enterer)
{
	if (Commands->Is_A_Star(enterer))
	{
		Set_Screen_Fade_Color_Player(enterer,0.102f,0.086f,0.098f,0.0f);
		Set_Screen_Fade_Opacity_Player(enterer,0.9f,0.0f);
		JMG_Bear_Hunter_Game_Control::diedInWater[JmgUtility::JMG_Get_Player_ID(enterer)] = true;
	}
	if (Commands->Is_A_Star(enterer) || !obj->As_SoldierGameObj())
		Commands->Apply_Damage(enterer,9999.9f,"Death",0);
}
void JMG_Bear_Hunter_Guardian_Aircraft::Created(GameObject *obj)
{
	firstMove = 2;
	putAwayTime = 0;
	newPosTime = 0;
	sprintf(primaryWeapon,"%s",Get_Current_Weapon(obj) ? Get_Current_Weapon(obj) : "\0");
	hasSecondaryWeapon = Has_Weapon(obj,Get_Parameter("AltWeapon")) ? true : false;
	const WeaponDefinitionClass *weapDef = Get_Weapon_Definition(Get_Parameter("AltWeapon"));
	if (weapDef)
		altReloadTime = weapDef->ReloadTime;
	else
		hasSecondaryWeapon = false;
	if (_stricmp(Get_Parameter("IdlePrimaryAnim"),"null"))
		Commands->Set_Animation(obj,Get_Parameter("IdlePrimaryAnim"),true,0,0.0f,-1,true);
	secondaryState = SECONDARY_READY;
	enemyID = 0;
	enemyTimeOutTime = 0;
	movePos = lastPos = Commands->Get_Position(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Enable_Engine(obj,true);
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Bear_Hunter_Guardian_Aircraft::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (!_stricmp(Get_Skin(seen),"Blamo") || !_stricmp(Get_Shield_Type(seen),"Blamo") || !Commands->Get_Health(seen))
		return;
	if (Is_Script_Attached(seen,"JMG_Bear_Hunter_Player_Vehicle"))
	{
		int SeatCount = Get_Vehicle_Seat_Count(seen);
		if (SeatCount && !Get_Vehicle_Occupant_Count(seen))
			return;
	}
	float tempDist = JmgUtility::SimpleDistance(Commands->Get_Position(seen),lastPos);
	if (enemyID && tempDist > enemyDistance)
		return;
	putAwayTime = 5;
	if (!enemyID || !enemyTimeOutTime || tempDist < enemyDistance)
	{
		enemyDistance = tempDist;
		enemyID = Commands->Get_ID(seen);
		enemyTimeOutTime = 2;
		Attack_Move_Update(obj);
	}
	else if (enemyID == Commands->Get_ID(seen))
	{
		enemyDistance = tempDist;
		enemyTimeOutTime = 2;
	}
}
void JMG_Bear_Hunter_Guardian_Aircraft::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 98432145)
	{
		Set_Bullets(obj,Get_Parameter("AltWeapon"),Get_Max_Bullets(obj,Get_Parameter("AltWeapon")));
		secondaryState = SECONDARY_READY;
	}
}
void JMG_Bear_Hunter_Guardian_Aircraft::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (enemyTimeOutTime)
		{
			enemyTimeOutTime--;
			if (!enemyTimeOutTime)
				enemyID = 0;
		}
		if (enemyID)
		{
			GameObject *Target = Commands->Find_Object(enemyID);
			if (!Target || !Commands->Get_Health(Target) || Commands->Get_Player_Type(Target) == Commands->Get_Player_Type(obj))
				enemyID = 0;
		}
		if (secondaryState == SECONDARY_READY && enemyID && Get_Bullets(obj,Get_Parameter("AltWeapon")))
		{
			secondaryState = SECONDARY_SWITCHING;
			if (!_stricmp(Get_Parameter("PrimaryToSecondaryAnim"),"null"))
				Switch_Weapon(obj,false);
			else
				Commands->Set_Animation(obj,Get_Parameter("PrimaryToSecondaryAnim"),false,0,0.0f,-1,true);
		}
		if (secondaryState == SECONDARY_ACTIVE && !Get_Bullets(obj,Get_Parameter("AltWeapon")))
		{
			secondaryState = SECONDARY_RELOADING;
			if (!_stricmp(Get_Parameter("SecondaryToPrimaryAnimation"),"null"))
				Switch_Weapon(obj,true);
			else
				Commands->Set_Animation(obj,Get_Parameter("SecondaryToPrimaryAnimation"),false,0,0.0f,-1,true);
		}
		Vector3 gPos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(gPos,lastPos) < 25.0)
		{
			if (newPosTime)
				newPosTime--;
			if (!newPosTime)
			{
				DPNode = ComancheDefensePoints[JMG_Bear_Hunter_Game_Control::gameTime > 15 ? 0 : 1].GetRandomExcluding(DPNode);
				if (DPNode)
					Select_New_Location(obj);
				firstMove--;
			}
		}
		if (putAwayTime)
		{
			putAwayTime--;
			if (!putAwayTime && (secondaryState != SECONDARY_RELOADING))
				Set_Bullets(obj,Get_Parameter("AltWeapon"),0);
		}
		lastPos = gPos;
		Attack_Move_Update(obj);
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Bear_Hunter_Guardian_Aircraft::Animation_Complete(GameObject* obj,const char *anim)
{
	if (!_stricmp(Get_Parameter("PrimaryToSecondaryAnim"),anim))
		Switch_Weapon(obj,false);
	if (!_stricmp(Get_Parameter("SecondaryToPrimaryAnimation"),anim))
		Switch_Weapon(obj,true);
}
void JMG_Bear_Hunter_Guardian_Aircraft::Select_New_Location(GameObject *obj)
{
	if (!DPNode)
		return;
	Vector3 GoToPos = Commands->Get_Position(obj);
	if (DPNode)
	{
		newPosTime = Commands->Get_Random_Int(3,8);
		GoToPos = DPNode->position;
		GoToPos.Z += Get_Float_Parameter("FlightHeight");
		GoToPos.Z += Commands->Get_Safe_Flight_Height(DPNode->position.X,DPNode->position.Y)-(Get_Float_Parameter("FlightHeight")*0.5f);
		movePos = GoToPos;
	}
	Commands->Action_Reset(obj,100);
}
void JMG_Bear_Hunter_Guardian_Aircraft::Attack_Move_Update(GameObject *obj)
{
	ActionParamsStruct params;
	params.Set_Movement(movePos,1.0f,1.0f,false);
	params.MovePathfind = false;
	params.Set_Basic(this,100,10);
	GameObject *Target = Commands->Find_Object(enemyID);
	if (Target)
	{
		params.Set_Attack(Target,100.0f,0.0f,usePrimary);
		params.AttackForceFire = true;
		if (!firstMove && JMG_Bear_Hunter_Game_Control::gameTime)
			params.AttackFaceTarget = true;
		params.ForceFacing = false;
		params.IgnoreFacing = true;
		Commands->Action_Goto(obj,params);
		Commands->Action_Attack(obj,params);
	}
	else
	{
		params.ForceFacing = true;
		params.Set_Face_Location(Commands->Get_Position(Get_GameObj(1)),5.0f);
		Commands->Action_Face_Location(obj,params);
		Commands->Action_Goto(obj,params);
	}
}
void JMG_Bear_Hunter_Guardian_Aircraft::Switch_Weapon(GameObject *obj,bool primary)
{
	usePrimary = primary;
	if (!primary && hasSecondaryWeapon)
	{
		secondaryState = SECONDARY_ACTIVE;
		Commands->Select_Weapon(obj,Get_Parameter("AltWeapon"));
		if (_stricmp(Get_Parameter("IdleSecondaryAnim"),"null"))
			Commands->Set_Animation(obj,Get_Parameter("IdleSecondaryAnim"),true,0,0.0f,-1,true);
	}
	if (primary && primaryWeapon[0] != '\0')
	{
		secondaryState = SECONDARY_RELOADING;
		Commands->Send_Custom_Event(obj,obj,98432145,0,altReloadTime);
		Commands->Select_Weapon(obj,primaryWeapon);
		if (_stricmp(Get_Parameter("IdlePrimaryAnim"),"null"))
			Commands->Set_Animation(obj,Get_Parameter("IdlePrimaryAnim"),true,0,0.0f,-1,true);
	}
	Attack_Move_Update(obj);
}
void JMG_Bear_Hunter_Comanche::Created(GameObject *obj)
{
	Commands->Attach_Script(obj,"JMG_AI_Ignore_Object","");
	Commands->Disable_Physical_Collisions(obj);
	id = Commands->Get_ID(obj);
}
void JMG_Bear_Hunter_Comanche::Killed(GameObject *obj,GameObject *killer)
{
	id = 0;
	JmgUtility::MessageAllPlayers(127,127,255,"Comanche 4573: We've been hit, we're going down we're goi...");
}
void JMG_Bear_Hunter_Comanche::Destroyed(GameObject *obj)
{
	id = 0;
}
void JMG_Bear_Hunter_Comanche_Defense_Point::Created(GameObject *obj)
{
	ComancheDefensePoints[Get_Int_Parameter("GroupId")] += obj;
	Commands->Destroy_Object(obj);
}
void JMG_Bear_Hunter_Defense_Gun_Beacon::Created(GameObject *obj)
{
	GameObject *Player = NULL;
	Player = JmgUtility::FindNearestPlayer(Commands->Get_Position(obj));
	if (!Player)
	{
		Commands->Destroy_Object(obj);
		return;
	}
	Vector3 PlacePosition = Commands->Get_Position(Player);
	PlacePosition.Z += 0.75f;
	GameObject *DefGun = Commands->Create_Object("Deployed_Sentry_Gun",PlacePosition);
	Commands->Set_Facing(DefGun,Commands->Get_Facing(Player));
	char params[256];
	sprintf(params,"%d,%d",Commands->Get_ID(obj),JmgUtility::JMG_Get_Player_ID(Player));
	Commands->Attach_Script(DefGun,"JMG_Bear_Hunter_Defense_Gun_Powerup",params);
}
void JMG_Bear_Hunter_Defense_Gun_Powerup::Created(GameObject *obj)
{
	Count = 0;
	GameObject *Player = NULL;
	Player = JmgUtility::FindNearestPlayer(Commands->Get_Position(obj));
	if (!Player)
	{
		Commands->Destroy_Object(obj);
		return;
	}
	GameObject *DefGun = Commands->Find_Object(PlayerData.players[Get_Int_Parameter("PlayerID")]->sentryTurretId);
	if (DefGun)
	{
		float refund = 500.0f*Commands->Get_Health(DefGun)/Commands->Get_Max_Health(DefGun);
		char refundMsg[220];
		sprintf(refundMsg,"You have been refudned $%s because you can only have one placed sentry gun at a time",JmgUtility::formatDigitGrouping(refund));
		JmgUtility::DisplayChatMessage(Player,127,127,255,refundMsg);
		Commands->Give_Money(Player,refund,false);
		Commands->Destroy_Object(DefGun);
	}
	GameObject *tester = Commands->Create_Object("CNC_Spectator",Vector3(-476.546f,-43.671f,0.193f));
	char params[512];
	sprintf(params,"%.2f %.2f %.2f,%d",Commands->Get_Position(obj).X,Commands->Get_Position(obj).Y,Commands->Get_Position(obj).Z,Commands->Get_ID(obj));
	Commands->Attach_Script(tester,"JMG_Bear_Hunter_Sentry_Turret_Placement_Tester",params);
	Commands->Start_Timer(obj,this,0.1f,4);
}
void JMG_Bear_Hunter_Defense_Gun_Powerup::Timer_Expired(GameObject *obj,int number)
{
	if (number == 4)
	{
		Count++;
		GameObject *player = Get_GameObj(Get_Int_Parameter("PlayerID"));
		GameObject *aBeacon = Commands->Find_Object(Get_Int_Parameter("BeaconID"));
		if (player)
			Commands->Set_Facing(obj,Commands->Get_Facing(player));
		if (Count >= 70)
		{
			PlayerData.players[Get_Int_Parameter("PlayerID")]->sentryTurretId = Commands->Get_ID(obj);
			if (aBeacon)
				Commands->Destroy_Object(aBeacon);
			if (!JMG_Bear_Hunter_Game_Control::winWait && JMG_Bear_Hunter_Game_Control::gameState != JMG_Bear_Hunter_Game_Control::BossArrive)
			{
				char params[512];
				sprintf(params,"90.0,0,1,0.0,0,Beep,%d,1.0,2411911",Get_Int_Parameter("PlayerID"));
				Commands->Attach_Script(obj,"JMG_Bear_Hunter_Camera_Behavior",params);
			}
			else
			{
				Remove_All_Scripts(obj);
				Commands->Attach_Script(obj,"JMG_Bear_Hunter_AI_Ignore_Up_To_25_Meters","");
			}
			return;
		}
		if (!aBeacon || !Commands->Get_Health(aBeacon))
		{
			Commands->Destroy_Object(obj);
			return;
		}
		Commands->Start_Timer(obj,this,0.1f,4);
	}
}
void JMG_Bear_Hunter_Defense_Gun_Powerup::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 4987321)
	{
		Commands->Destroy_Object(obj);
		GameObject *player = Get_GameObj(Get_Int_Parameter("PlayerID"));
		if (player)
			Set_Bullets(player,"Weapon_Sentry_Gun_Beacon",1);
		JmgUtility::DisplayChatMessage(player,255,25,25,"This cannot be placed here.");
	}
}
void JMG_Bear_Hunter_Defense_Gun_Powerup::Destroyed(GameObject *obj)
{
	GameObject *aBeacon = Commands->Find_Object(Get_Int_Parameter("BeaconID"));
	if (aBeacon)
		Commands->Destroy_Object(aBeacon);
	PlayerData.players[Get_Int_Parameter("PlayerID")]->sentryTurretId = 0;
}
void JMG_Bear_Hunter_Camera_Behavior::Created(GameObject *obj)
{
	playerScoreNode = bearHunterScoreSystem.Get_Current_Player_Score_Node(JmgUtility::JMG_Get_Player_ID(obj));
	if (playerScoreNode)
		playerScoreNode->SentryTurretsPlaced++;
	enabled = true;
	Commands->Enable_HUD_Pokable_Indicator(obj,true);
	Commands->Set_Animation(obj,"mobDefGun.mobDefGun",false,0,0,0,false);
	cameraActive = true;
	Commands->Enable_Hibernation(obj,false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	EnemyID = 0;
	Vector3 Pos = Commands->Get_Position(obj);
	float Angle = Get_Float_Parameter("Angle")/4;
	float Facing = Commands->Get_Facing(obj);
	CameraFacingCount = 0;
	IncreaseOrDecreaseCount = 1;
	Facing = Facing-(Angle*2);
	float TempFacing = Facing*PI180;
	float ZPos = Commands->Get_Bone_Position(obj,"MuzzleA0").Z;
	CameraFacingLocation[0] = Pos;
	CameraFacingLocation[0].X += cos(TempFacing)*1000.0f;
	CameraFacingLocation[0].Y += sin(TempFacing)*1000.0f;
	CameraFacingLocation[0].Z = ZPos;
	Facing = Facing+Angle;
	TempFacing = Facing*PI180;
	CameraFacingLocation[1] = Pos;
	CameraFacingLocation[1].X += cos(TempFacing)*1000.0f;
	CameraFacingLocation[1].Y += sin(TempFacing)*1000.0f;
	CameraFacingLocation[1].Z = ZPos;
	Facing = Facing+Angle;
	TempFacing = Facing*PI180;
	CameraFacingLocation[2] = Pos;
	CameraFacingLocation[2].X += cos(TempFacing)*1000.0f;
	CameraFacingLocation[2].Y += sin(TempFacing)*1000.0f;
	CameraFacingLocation[2].Z = ZPos;
	Facing = Facing+Angle;
	TempFacing = Facing*PI180;
	CameraFacingLocation[3] = Pos;
	CameraFacingLocation[3].X += cos(TempFacing)*1000.0f;
	CameraFacingLocation[3].Y += sin(TempFacing)*1000.0f;
	CameraFacingLocation[3].Z = ZPos;
	Facing = Facing+Angle;
	TempFacing = Facing*PI180;
	CameraFacingLocation[4] = Pos;
	CameraFacingLocation[4].X += cos(TempFacing)*1000.0f;
	CameraFacingLocation[4].Y += sin(TempFacing)*1000.0f;
	CameraFacingLocation[4].Z = ZPos;
	CameraFacingUpdateTime = Get_Float_Parameter("Angle")*0.0085f;
	if (CameraFacingUpdateTime < 0.01f)
		CameraFacingUpdateTime = 0.1f;
	Commands->Start_Timer(obj,this,0.1f,2);
	Commands->Start_Timer(obj,this,0.1f,3);
}
void JMG_Bear_Hunter_Camera_Behavior::Enemy_Seen(GameObject *obj,GameObject *seen)
{
	if (!cameraActive)
		return;
	int seenID = Commands->Get_ID(seen);
	if (!EnemyID)
	{
		Commands->Create_Sound(Get_Parameter("Alarm_Sound"),Commands->Get_Position(obj),obj);
		SeenTime = 10;
		EnemyID = seenID;
		ActionParamsStruct params;
		params.Set_Basic(this,70,1);
		if (Get_Int_Parameter("Is_Gun"))
			params.Set_Attack(seen,Get_Float_Parameter("Delay") ? 0.0f : 9999.0f,0,true);
		else
			params.Set_Attack(seen,0.0f,0,true);
		params.AttackForceFire = true;
		Commands->Action_Attack(obj,params);
		if (!Get_Float_Parameter("Delay"))
		{
			if (Get_Int_Parameter("Alarm_ID") && Get_Int_Parameter("Alarm_Message"))
			{
				GameObject *target = Commands->Find_Object(Get_Int_Parameter("Alarm_ID"));
				Commands->Send_Custom_Event(obj,target,Get_Int_Parameter("Alarm_Message"),EnemyID,0);
			}
		}
		else
			Commands->Send_Custom_Event(obj,obj,47498612,EnemyID,Get_Float_Parameter("Delay"));
	}
	else if (seenID == EnemyID)
		SeenTime = 10;
}
void JMG_Bear_Hunter_Camera_Behavior::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 47498612 && EnemyID == param)
	{
		if (!enabled)
			return;
		if (Get_Int_Parameter("Is_Gun"))
		{
			GameObject *seen = Commands->Find_Object(EnemyID);
			if (seen)
			{
				ActionParamsStruct params;
				params.Set_Basic(this,70,1);
				params.Set_Attack(seen,9999.0f,0,true);
				params.AttackForceFire = true;
				Commands->Action_Attack(obj,params);
			}
		}
		if (Get_Int_Parameter("Alarm_ID") && Get_Int_Parameter("Alarm_Message"))
		{
			GameObject *target = Commands->Find_Object(Get_Int_Parameter("Alarm_ID"));
			Commands->Send_Custom_Event(obj,target,Get_Int_Parameter("Alarm_Message"),EnemyID,0);
			Commands->Create_Sound(Get_Parameter("Alarm_Sound"),Commands->Get_Position(obj),obj);
			Commands->Send_Custom_Event(obj,obj,47498613,EnemyID,Get_Float_Parameter("Delay"));
		}
	}
	if (message == 47498613 && EnemyID == param)
	{
		if (!enabled)
			return;
		if (Get_Int_Parameter("Alarm_ID") && Get_Int_Parameter("Alarm_Message"))
		{
			GameObject *target = Commands->Find_Object(Get_Int_Parameter("Alarm_ID"));
			Commands->Send_Custom_Event(obj,target,Get_Int_Parameter("Alarm_Message"),EnemyID,0);
			Commands->Create_Sound(Get_Parameter("Alarm_Sound"),Commands->Get_Position(obj),obj);
			Commands->Send_Custom_Event(obj,obj,47498613,EnemyID,Get_Float_Parameter("Delay"));
		}
	}
	if (message == Get_Int_Parameter("Power_Message"))
	{
		if (enabled && !param)
		{
			enabled = false;
			Commands->Set_Player_Type(obj,-2);
			Commands->Action_Reset(obj,90);
			Commands->Set_Animation(obj,"mobDefGun.mobDefGun",false,0,1,1,false);
			Commands->Enable_Enemy_Seen(obj,false);
		}
		if (!enabled && param)
		{
			enabled = true;
			Commands->Set_Player_Type(obj,1);
			if (cameraActive)
				Commands->Set_Animation(obj,"mobDefGun.mobDefGun",false,0,0,0,false);
			Commands->Enable_Enemy_Seen(obj,true);
		}
	}
}
void JMG_Bear_Hunter_Camera_Behavior::Timer_Expired(GameObject *obj,int number)
{
	if (number == 2)
	{
		if (!EnemyID && cameraActive)
		{
			CameraFacingCount += IncreaseOrDecreaseCount;
			if (CameraFacingCount == 0 || CameraFacingCount == 4)
				IncreaseOrDecreaseCount = IncreaseOrDecreaseCount == -1 ? 1 : -1;
			ActionParamsStruct params;
			params.Set_Basic(this,70,1);
			params.Set_Attack(CameraFacingLocation[CameraFacingCount],0,0,true);
			Commands->Action_Attack(obj,params);
		}
		Commands->Start_Timer(obj,this,CameraFacingUpdateTime*Get_Float_Parameter("UpdateRateMultiplier"),2);
	}
	if (number == 3)
	{
		GameObject *enemy = Commands->Find_Object(EnemyID);
		if (!enemy || !Commands->Get_Health(enemy))
			EnemyID = 0;
		if (SeenTime)
		{
			SeenTime--;
			if (!SeenTime)
				EnemyID = 0;
		}
		Commands->Start_Timer(obj,this,0.1f,3);
	}
}
void JMG_Bear_Hunter_Camera_Behavior::Poked(GameObject *obj,GameObject *poker)
{
	if (!enabled)
		return;
	if (Commands->Get_Player_Type(poker) == -4)
		return;
	int PlayerID = JmgUtility::JMG_Get_Player_ID(poker);
	if (PlayerID != Get_Int_Parameter("PlayerID"))
	{
		JmgUtility::DisplayChatMessage(poker,200,75,200,"Sorry, you don't own this...");
		return;
	}
	cameraActive = !cameraActive;
	if (cameraActive)
	{
		Commands->Set_Player_Type(obj,1);
		Commands->Set_Animation(obj,"mobDefGun.mobDefGun",false,0,0,0,false);
		JmgUtility::DisplayChatMessage(poker,200,75,200,"Sentry gun on-line...");
	}
	else
	{
		Commands->Set_Player_Type(obj,-2);
		Commands->Action_Reset(obj,90);
		Commands->Set_Animation(obj,"mobDefGun.mobDefGun",false,0,1,1,false);
		JmgUtility::DisplayChatMessage(poker,200,75,200,"Sentry gun powering down...");
	}
}
void JMG_Bear_Hunter_Camera_Behavior::Killed(GameObject *obj,GameObject *killer)
{
	if (playerScoreNode)
		playerScoreNode->SentryTurretsLost++;
}
void JMG_Bear_Hunter_Sentry_Turret_Placement_Tester::Created(GameObject *obj)
{
	Set_Skin(obj,"Blamo");
	Commands->Set_Player_Type(obj,-2);
	Commands->Start_Timer(obj,this,0.1f,1);
	Commands->Start_Timer(obj,this,0.5f,1);
	Commands->Start_Timer(obj,this,1.0f,1);
	Commands->Start_Timer(obj,this,5.0f,1);
	Commands->Start_Timer(obj,this,7.0f,2);
}
void JMG_Bear_Hunter_Sentry_Turret_Placement_Tester::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		ActionParamsStruct params;
		Commands->Get_Action_Params(obj,params);
		params.Set_Movement(Get_Vector3_Parameter("Location"),50.0f,0,false);
		params.MovePathfind = true;
		params.Set_Basic(this,100,10);
		Commands->Action_Goto(obj,params);
	}
	if (number == 2)
	{
		Commands->Destroy_Object(obj);
	}
}
void JMG_Bear_Hunter_Sentry_Turret_Placement_Tester::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason)
{
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{
		GameObject *aBeacon = Commands->Find_Object(Get_Int_Parameter("BeaconID"));
		if (aBeacon)
			Commands->Send_Custom_Event(aBeacon,aBeacon,4987321,0,0);
		Commands->Destroy_Object(obj);
	}
}
void JMG_Powerup_Prez_Medical_Needle::Created(GameObject *obj)
{
	if (NewObjectiveSystemControl.Get_Objective_Status(22) == NewObjectiveSystem::NotDefined)
	{
		JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: President of Corporate America had some painkillers called in, they airdropped them outside of DERP, we'll pay everyone $1,000.00 if you can return it to the him.");
		NewObjectiveSystemControl.Add_Objective(22,NewObjectiveSystem::Tertiary,NewObjectiveSystem::Pending,12795,"",12795,obj);
	}
	else
	{
		NewObjectiveSystemControl.Set_Objective_Mission(22,12795,12795);
		NewObjectiveSystemControl.Set_Radar_Blip(22,obj);
	}
}
void JMG_Powerup_Prez_Medical_Needle::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		BearHunterScoreSystem::BHScoreNode *playerScoreNode = bearHunterScoreSystem.Get_Current_Player_Score_Node(JmgUtility::JMG_Get_Player_ID(sender));
		if (playerScoreNode)
			playerScoreNode->PickedUpMedicalNeedle++;
		Commands->Attach_Script(sender,"JMG_Prez_Medical_Needle_Player_Control","");
	}
}
void JMG_Prez_Medical_Needle_Player_Control::Created(GameObject *obj)
{
	NewObjectiveSystemControl.Set_Objective_Mission(22,12796,12796);
	NewObjectiveSystemControl.Set_Radar_Blip(22,Vector3(45.146f,-689.362f,0.240f));
	pickupLocation = Commands->Get_Position(obj);
}
void JMG_Prez_Medical_Needle_Player_Control::Killed(GameObject *obj,GameObject *killer)
{
	if (!killer || killer == obj)
		return;
	pickupLocation = Commands->Get_Position(obj);
}
void JMG_Prez_Medical_Needle_Player_Control::Destroyed(GameObject *obj)
{
	if (NewObjectiveSystemControl.Get_Objective_Status(22) == NewObjectiveSystem::Pending)
		Commands->Create_Object("POW_Needle",pickupLocation);
}
void JMG_Prez_Cinematic_C130::Created(GameObject *obj)
{
	Commands->Enable_Engine(obj,true);
}
void JMG_Bear_Hunter_Power_Transformer::Created(GameObject *obj)
{
	Commands->Set_Animation(obj,"transformer.transformer",false,0,0,0,true);
	mctIds[0] = Get_Int_Parameter("MctId1");
	mctIds[1] = Get_Int_Parameter("MctId2");
	lastHealthPercent = 100;
	objectiveActive = false;
	JMG_Bear_Hunter_Power_Transformer::online = true;
	announceTime = clock();
}
void JMG_Bear_Hunter_Power_Transformer::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (damage)
	{
		float shieldHealth = Commands->Get_Shield_Strength(obj);
		int tempPercent = (int)(shieldHealth/Commands->Get_Max_Shield_Strength(obj)*100.0);
		if (tempPercent != lastHealthPercent)
		{
			lastHealthPercent = tempPercent;
			UpdateHealthDisplays(shieldHealth);
		}
	}
	if (damage > 0)
		if (!objectiveActive)
		{
			if (Commands->Get_Shield_Strength(obj) < 490.0f)
			{
				JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: The power substation is under attack, defend it or else we'll lose power to the turrets!");
				NewObjectiveSystemControl.Add_Objective(23,NewObjectiveSystem::Secondary,NewObjectiveSystem::Pending,12798,"",12798,obj);
				objectiveActive = true;
			}
			announceTime = clock();
		}
		else if (difftime(clock(),announceTime) > 10000.0f)
		{
			JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: The power substation is still under attack!");
			announceTime = clock();
		}
	if (JMG_Bear_Hunter_Power_Transformer::online && !Commands->Get_Shield_Strength(obj))
	{
		JMG_Bear_Hunter_Power_Transformer::online = false;
		announceTime = clock();
		BearHunterScoreSystem::BHScoreNode *playerScoreNode = bearHunterScoreSystem.Get_Current_Player_Score_Node(JmgUtility::JMG_Get_Player_ID(damager));
		if (playerScoreNode)
			playerScoreNode->RepairedSubstation--;
		Commands->Set_Player_Type(obj,-2);
		JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: The power substation has been critically damaged, all turrets are down, get out there and repair it!");
		Commands->Create_Explosion("Explosion_IonCannonBeacon",Vector3(187.902f,-721.522f,1.852f),0);
		NewObjectiveSystemControl.Set_Objective_Mission(23,12799,12799);
		Commands->Set_Animation(obj,"transformer.transformer",false,0,1.0,1.0,true);
		ToggleTurrets(false);
		UpdateMCTs(-2);
	}
	if (!JMG_Bear_Hunter_Power_Transformer::online && Commands->Get_Shield_Strength(obj) >= Commands->Get_Max_Shield_Strength(obj)-1.0f)
	{
		JMG_Bear_Hunter_Power_Transformer::online = true;
		announceTime = clock();
		BearHunterScoreSystem::BHScoreNode *playerScoreNode = bearHunterScoreSystem.Get_Current_Player_Score_Node(JmgUtility::JMG_Get_Player_ID(damager));
		if (playerScoreNode)
			playerScoreNode->RepairedSubstation++;
		Commands->Set_Player_Type(obj,1);
		JmgUtility::MessageAllPlayers(127,127,255,"Bear Rug Co: The power substation has been repaired, turrets back on-line, try to defend it this time!");
		NewObjectiveSystemControl.Set_Objective_Mission(23,12798,12798);
		Commands->Set_Animation(obj,"transformer.transformer",false,0,0,0,true);
		ToggleTurrets(true);
		UpdateMCTs(1);
	}
}
void JMG_Bear_Hunter_Power_Transformer::ToggleTurrets(bool enabled)
{
	for (SLNode<SmartGameObj> *current = GameObjManager::SmartGameObjList.Head();current;current = current->Next())
	{
		SmartGameObj* o = current->Data();
		if (!o)
			continue;
		int presetId = Commands->Get_Preset_ID(o);
		if (presetId == 1000001055 || presetId == 1000001536 || presetId == 1000001820)
			Commands->Send_Custom_Event(o,o,2411911,enabled,0);
		if (presetId == 1000001176)
			if (enabled)
				Commands->Select_Weapon(o,"Weapon_Ramjet_Turret");
			else
				Commands->Select_Weapon(o,"Weapon_Ramjet_Turret_Lowpower");
		if (presetId == 1000001820)
			if (enabled)
				Commands->Set_Model(o,"SearchLight");
			else
				Commands->Set_Model(o,"SearchLightOff");
	}
}
void JMG_Bear_Hunter_Power_Transformer::UpdateHealthDisplays(float shieldStrength)
{
	GameObject *buildingController = Commands->Find_Object(Get_Int_Parameter("BuildingId"));
	if (buildingController)
	{
		Commands->Set_Health(buildingController,2.0f);
		Commands->Apply_Damage(buildingController,0.01f,"None",0);
		Commands->Set_Health(buildingController,2.0f);
		Commands->Set_Shield_Strength(buildingController,shieldStrength);
	}
	buildingController = Commands->Find_Object(Get_Int_Parameter("MctId1"));
	if (buildingController)
		Commands->Set_Shield_Strength(buildingController,shieldStrength);
	buildingController = Commands->Find_Object(Get_Int_Parameter("MctId2"));
	if (buildingController)
		Commands->Set_Shield_Strength(buildingController,shieldStrength);
}
void JMG_Bear_Hunter_Power_Transformer::UpdateMCTs(int team)
{
	GameObject *buildingController = Commands->Find_Object(Get_Int_Parameter("MctId1"));
	if (buildingController)
		Commands->Set_Player_Type(buildingController,team);
	buildingController = Commands->Find_Object(Get_Int_Parameter("MctId2"));
	if (buildingController)
		Commands->Set_Player_Type(buildingController,team);
}
void JMG_Bear_Hunter_Power_Transformer_MCT::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	GameObject *building = Commands->Find_Object(Get_Int_Parameter("BuildingId"));
	if (building)
		Commands->Apply_Damage(building,damage,"None",0);
}
void JMG_Bear_Hunter_Mutant_Respawn_Tracker::Killed(GameObject *obj,GameObject *killer)
{
	if (!killer || killer == obj)
	{
		AiMutantRespawnSystem.PlayerDeath(Commands->Get_ID(obj),JMG_Bear_Hunter_Game_Control::mutantBears,0.0f);
		return;
	}
	float deathDist = JmgUtility::SimpleDistance(Vector3(45.146f,-689.362f,0.240f),Commands->Get_Position(obj));	
	AiMutantRespawnSystem.PlayerDeath(Commands->Get_ID(obj),JMG_Bear_Hunter_Game_Control::mutantBears,JMG_Bear_Hunter_Game_Control::gameTime < 25.0f ? 0.0f : 15.0f*(1-JmgUtility::MathClamp(deathDist,0.0f,5625.0f)/5625.0f));
}
void JMG_Bear_Hunter_Mutant_Respawn_Tracker::Destroyed(GameObject *obj)
{
	float deathDist = JmgUtility::SimpleDistance(Vector3(45.146f,-689.362f,0.240f),Commands->Get_Position(obj));	
	AiMutantRespawnSystem.PlayerDeath(Commands->Get_ID(obj),JMG_Bear_Hunter_Game_Control::mutantBears,JMG_Bear_Hunter_Game_Control::gameTime < 25.0f ? 0.0f : 15.0f*(1-JmgUtility::MathClamp(deathDist,0.0f,5625.0f)/5625.0f));
}
void JMG_Wandering_AI::Created(GameObject *obj)
{
	if (!JMG_Wandering_AI_Controller::setup)
	{
		Console_Input("msg JMG_Wandering_AI_Wander_Point ERROR:: Make sure you have the script JMG_Wandering_AI_Controller placed on the map!");
		Commands->Destroy_Object(obj);
		return;
	}
	damaged = false;
	returnHome = 0;
	PlayerCount = 1;
	CanHearDelay = 0;
	LastSeen = 0;
	currentTargetID = 0;
	secondaryEnemyId = 0;
	huntEnemy = 0;
	waitcount = 0;
	vehicleChaseTime = 0;
	vehicleChaseTimeReset = 0;
	maxVehicleChaseTime = Commands->Get_Random_Int(300,900);
	wanderDelay = 1;
	lastPos = homelocation = Commands->Get_Position(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,5.0f,1);
	Commands->Start_Timer(obj,this,0.1f,2);
	Commands->Start_Timer(obj,this,0.25f,3);
}
void JMG_Wandering_AI::Enemy_Seen(GameObject *obj,GameObject *seen)	
{
	if (seen->As_SmartGameObj() && seen->As_SmartGameObj()->Is_Stealthed() && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 900)
		return;
	if (Is_Script_Attached(seen,"JMG_Bear_Hunter_AI_Ignore"))
		return;
	if (Is_Script_Attached(seen,"JMG_Bear_Hunter_AI_Ignore_Up_To_75_Meters") && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 5625)
		return;
	if (Is_Script_Attached(seen,"JMG_Bear_Hunter_AI_Ignore_Up_To_150_Meters") && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 22500)
		return;
	if (Is_Script_Attached(seen,"JMG_AI_Ignore_Object"))
		return;
	if (Commands->Is_A_Star(seen) && JMG_Wandering_AI_Controller::aiIgnorePlayers[JmgUtility::JMG_Get_Player_ID(seen)])
		return;
	if (vehicleChaseTimeReset && (seen->As_VehicleGameObj() || Get_Vehicle(seen)) && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 225)
		return;
	int SeenID = Commands->Get_ID(seen);
	if (!currentTargetID)
	{
		LastSeen = 30;
		huntEnemy = 1;
		currentTargetID = SeenID;
	}
	else if (currentTargetID == SeenID)
		LastSeen = 30;
	if (currentTargetID && !secondaryEnemyId && SeenID != currentTargetID)
		secondaryEnemyId = SeenID;
	if (SeenID == secondaryEnemyId)
		lastSeenSecondary = 30;
}
void JMG_Wandering_AI::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 91185423)
	{
		GameObject *Player = JmgUtility::FindNearestPlayer(Commands->Get_Position(obj));
		if (!Player)
			return;
		ActionParamsStruct params;
		params.Set_Movement(Commands->Get_Position(Player),1.0,10.0f,false);
		params.MovePathfind = true;
		Commands->Action_Goto(obj,params);
	}
}
void JMG_Wandering_AI::Timer_Expired(GameObject *obj,int number)
{
	if (2 == number)
	{
		if (CanHearDelay)
			CanHearDelay--;
		if (LastSeen)
		{
			LastSeen--;
			if (!LastSeen && currentTargetID)
			{
				Commands->Action_Reset(obj,10);
				currentTargetID = 0;
				GameObject *secondaryEnemy = Commands->Find_Object(secondaryEnemyId);
				if (secondaryEnemy && Commands->Get_Health(secondaryEnemy))
				{
					currentTargetID = secondaryEnemyId;
					secondaryEnemyId = 0;
					ActionParamsStruct params;
					Commands->Get_Action_Params(obj,params);
					params.Set_Movement(secondaryEnemy,1.0,1.4f,false);
					params.MovePathfind = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(secondaryEnemy)) < 25.0 ? false : true;
					Commands->Action_Goto(obj,params);
					params.Set_Basic(this,999,10);
					LastSeen = lastSeenSecondary;
				}
			}
		}
		if (lastSeenSecondary)
		{
			lastSeenSecondary--;
			if (!lastSeenSecondary)
				secondaryEnemyId = 0;
		}
		if (currentTargetID)
		{
			if (vehicleChaseTimeReset)
				vehicleChaseTimeReset--;
			GameObject *target = Commands->Find_Object(currentTargetID);
			if (target && target->As_VehicleGameObj() || Get_Vehicle(target))
			{
				vehicleChaseTime++;
				if (vehicleChaseTime >= maxVehicleChaseTime && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(target)) > 225)
				{
					vehicleChaseTime = 0;
					Commands->Action_Reset(obj,10);
					currentTargetID = 0;
					target = NULL;
					vehicleChaseTimeReset = Commands->Get_Random_Int(300,450);
				}
			}
			if (!target || Commands->Get_Health(target) == 0.0f)
			{
				waitcount = 0;
				currentTargetID = 0;
				Commands->Action_Reset(obj,10);
				GameObject *secondaryEnemy = Commands->Find_Object(secondaryEnemyId);
				if (secondaryEnemy && Commands->Get_Health(secondaryEnemy))
				{
					currentTargetID = secondaryEnemyId;
					secondaryEnemyId = 0;
					ActionParamsStruct params;
					Commands->Get_Action_Params(obj,params);
					params.Set_Movement(secondaryEnemy,1.0,1.4f,false);
					params.MovePathfind = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(secondaryEnemy)) < 25.0 ? false : true;
					Commands->Action_Goto(obj,params);
					params.Set_Basic(this,999,10);
					LastSeen = lastSeenSecondary;
				}
				else
				{
					returnHome = 600;
					wanderDelay = Commands->Get_Random_Int(1,30);
				}
			}
			else if (JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(target)) > 40000.0f && huntEnemy == 1)
			{
				currentTargetID = 0;
				waitcount = 0;
				if (Commands->Get_Random(0.0f,1.0f) < 0.66f)
					Commands->Action_Reset(obj,10);
			}
			else
			{
				waitcount++;
				if (waitcount > 2)
				{
					waitcount = 0;
					ActionParamsStruct params;
					Commands->Get_Action_Params(obj,params);
					params.Set_Movement(target,1.0,Get_Float_Parameter("Attack_Move_Range"),false);
					params.MovePathfind = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(target)) < 25.0 ? false : true;
					Commands->Action_Goto(obj,params);
					params.Set_Basic(this,999,10);
					if (Get_Vehicle(target) || target->As_VehicleGameObj())
					{
						params.AttackCheckBlocked = false;
						params.Set_Attack(target,Get_Float_Parameter("Vehicle_Range"),1,Get_Player_Count() < 10 ? true : false);
					}
					else
						params.Set_Attack(target,Get_Float_Parameter("Normal_Range"),1,Get_Player_Count() < 10 ? true : false);
					Commands->Action_Attack(obj,params);
				}
			}
		}
		Vector3 Pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(lastPos,Pos) < 0.1f)
		{
			if (returnHome)
				returnHome--;
			if (!returnHome && JmgUtility::SimpleDistance(lastPos,homelocation) > 25.0f)
				Commands->Set_Position(obj,homelocation);
			if (wanderDelay)
				wanderDelay--;
			if (!wanderDelay)
			{
				Commands->Action_Reset(obj,10);
				ActionParamsStruct params;
				params.Set_Basic(this,999,10);
				params.MovePathfind = true;
				params.Set_Movement(GetRandomPosition(),Get_Float_Parameter("Wander_Speed"),Commands->Get_Random(4,16),false);
				Commands->Action_Goto(obj,params);
				returnHome = 600;
				wanderDelay = Commands->Get_Random_Int(10,90);
			}
		}
		else
			returnHome = 600;
		lastPos = Pos;
		Commands->Start_Timer(obj,this,0.1f,2);
	}
	if (1 == number)
	{
		GameObject *secondaryEnemy = Commands->Find_Object(secondaryEnemyId);
		if (secondaryEnemyId && (!secondaryEnemy || !Commands->Get_Health(secondaryEnemy)))
			secondaryEnemyId = 0;
		Commands->Start_Timer(obj,this,5.0f,1);
	}
	if (3 == number)
	{
		Vector3 myPos = Commands->Get_Position(obj);
		for (int x = 0;x < 128;x++)
			if (PlayerEmulatedSoundList[x].SoundType)
			{
				GameObject *Player = Get_GameObj(x);
				if (!Player || Get_Player_Type(Player) == -4 || !Commands->Get_Health(Player))
					continue;
				Vector3 Pos = Commands->Get_Position(Player);
				if (JmgUtility::SimpleDistance(Pos,myPos) <= PlayerEmulatedSoundList[x].Range)
				{
					CombatSound CS = CombatSound();
					CS.Creator = Player;
					CS.Position = Pos;
					CS.sound = PlayerEmulatedSoundList[x].SoundType;
					SoundDHeard(obj,CS);
				}
			}
		Commands->Start_Timer(obj,this,0.1f,3);
	}
}
void JMG_Wandering_AI::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damager || Commands->Get_Player_Type(damager) != 1)
		return;
	if (!Commands->Get_Health(damager))
		return;
	if (!damaged && damage > 0)
		damaged = true;
	if (Commands->Is_A_Star(damager) && JMG_Wandering_AI_Controller::aiIgnorePlayers[JmgUtility::JMG_Get_Player_ID(damager)])
		return;
	if (Is_Script_Attached(damager,"JMG_AI_Ignore_Object"))
		return;
	GameObject *target = Commands->Find_Object(currentTargetID);
	if (target)
	{
		float targetDistance = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(target));
		float damagerDistance = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(damager));
		if (targetDistance < damagerDistance)
			return;
	}
	LastSeen = 30;
	huntEnemy = 0;
	waitcount = 0;
	currentTargetID = Commands->Get_ID(damager);
	if (damager->As_VehicleGameObj() || Get_Vehicle(damager))
		vehicleChaseTimeReset = 0;
}
void JMG_Wandering_AI::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason)
{
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{

		if (Commands->Get_Health(obj) <= 0)
			return;
		GameObject *target = Commands->Find_Object(currentTargetID);
		int playerId = JmgUtility::JMG_Get_Player_ID(target);
		if (playerId && Commands->Get_Health(target))
			JMG_Wandering_AI_Controller::aiIgnorePlayers[playerId] = 30;
	}
}
void JMG_Wandering_AI::SoundDHeard(GameObject *obj,const CombatSound &sound)
{
	if (!sound.Creator)
		return;
	if (!Commands->Is_A_Star(sound.Creator) || CanHearDelay || currentTargetID)// was 225 
		return;
	Vector3 MyPos = Commands->Get_Position(obj);
	float dist = JmgUtility::SimpleDistance(sound.Position,MyPos);
	bool RandSucess = Commands->Get_Random(0.00f,1.00f) < 0.0000025;
	bool Success = false;//(dist > 225 && Commands->Get_Random(0.00f,1.00f) > 0.015)fail
	if (dist < 144)// Sound is near, must investigate
		Success = true;
	else if (sound.sound == 6 && dist < 250000 && Commands->Get_Random(0.00f,1.00f) < JMG_Wandering_AI_Controller::soundAttentionWhoreNotify)//player just wants to be noticed
		Success = true;
	else if (Commands->Get_Random(0.00f,1.00f) < 0.00025 && sound.sound != 5)// Bad Luck
		Success = true;
	else if (sound.sound == 5 && Commands->Get_Random(0.00f,1.00f) < 0.0015)// Sound is bullet hits
		Success = true;
	else if (dist < 2025 && sound.sound != 5)// Sound is near and not bullet hits must check
		Success = true;
	else if (dist < 8100 && sound.sound != 5 && Commands->Get_Random(0.00f,1.00f) < 0.0025)// Sound is near and not bullet hits decided to check
		Success = true;
	else if (dist < 90000 && sound.sound != 5 && JMG_Wandering_AI::CanInvestigateSound)
		if (RandSucess && JmgUtility::SimpleDistance(JMG_Wandering_AI::AllowedSoundPos,Vector3(0.0f,0.0f,0.0f)) < 0.1f)// Sound is really close but player is far away
			Success = true;// Sound is far away but chances allowed it to successfully fall into the special "Everyone hates you" condition
		else if (JmgUtility::SimpleDistance(JMG_Wandering_AI::AllowedSoundPos,sound.Position) < 0.1f)
			Success = true;// Also heard the "Everyone hates you sound
	if (!Success)
		return;
	CanHearDelay = Commands->Get_Random_Int(30,90);
	ActionParamsStruct params;
	Commands->Get_Action_Params(obj,params);
	params.Set_Movement(Commands->Get_Position(sound.Creator),1.0f,sound.sound == 6 ? 25.0f : 1.4f,false);
	params.MovePathfind = true;
	if (JMG_Wandering_AI::CanInvestigateSound && JmgUtility::SimpleDistance(JMG_Wandering_AI::AllowedSoundPos,Vector3(0.0f,0.0f,0.0f)) < 0.1f && RandSucess)
		JMG_Wandering_AI::AllowedSoundPos = sound.Position;
	Commands->Action_Goto(obj,params);
}
Vector3 JMG_Wandering_AI::GetRandomPosition()
{
	Rp2SimplePositionSystem::SimplePositionNode *node = NULL;
	if (Get_Int_Parameter("GroupID") == -1)
		node = JMG_Wandering_AI_Controller::wanderPoints.GetRandom();
	else
		node = JMG_Wandering_AI_Controller::wanderPoints.GetRandomFromGroup(Get_Int_Parameter("GroupID"));
	if (!node)
	{
		Console_Input("msg JMG_Wandering_AI ERROR: No wander points could be found for that group!");
		return Vector3();
	}
	return node->position;
}
void JMG_Wandering_AI_Controller::Created(GameObject *obj)
{
	for (int x = 0;x < 128;x++)
		aiIgnorePlayers[x] = 0;
	Commands->Start_Timer(obj,this,1.0f,1);
}
void JMG_Wandering_AI_Controller::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		for (int x = 0;x < 128;x++)
		{
			if (aiIgnorePlayers[x] > 0)
				aiIgnorePlayers[x]--;
			GameObject *player = Get_GameObj(x);
			if (player && !Is_Script_Attached(player,"JMG_Rp2_Dedicated_Server_Sound_Emulator"))
			Commands->Attach_Script(player,"JMG_Rp2_Dedicated_Server_Sound_Emulator","");
		}
		Commands->Start_Timer(obj,this,1.0f,1);
	}
}
void JMG_Wandering_AI_Controller::Destroyed(GameObject *obj)
{
	JMG_Wandering_AI_Controller::setup = false;
	JMG_Wandering_AI_Controller::wanderPoints.Empty_List();
}
void JMG_Wandering_AI_Wander_Point::Created(GameObject *obj)
{
	if (!JMG_Wandering_AI_Controller::setup)
	{
		Console_Input("msg JMG_Wandering_AI_Wander_Point ERROR:: Make sure you have the script JMG_Wandering_AI_Controller placed on the map!");
		Commands->Destroy_Object(obj);
		return;
	}
	Rp2SimplePositionSystem::SimplePositionNode *node = new Rp2SimplePositionSystem::SimplePositionNode(obj);
	node->value = Get_Int_Parameter("GroupId");
	JMG_Wandering_AI_Controller::wanderPoints += node;
	Commands->Destroy_Object(obj);
}
void JMG_Utility_Custom_Spawn_System_Controller::Destroyed(GameObject *obj)
{
	JMG_Utility_Custom_Spawn_System_Controller::setup = false;
	JMG_Utility_Custom_Spawn_System_Controller::spawnPoints.Empty_List();
}
void JMG_Utility_Custom_Spawn_System_Point::Created(GameObject *obj)
{
	if (!JMG_Utility_Custom_Spawn_System_Controller::setup)
	{
		Console_Input("msg JMG_Utility_Custom_Spawn_System_Point ERROR:: Make sure you have the script JMG_Utility_Custom_Spawn_System_Controller placed on the map!");
		Commands->Destroy_Object(obj);
		return;
	}
	Rp2SimplePositionSystem::SimplePositionNode *node = new Rp2SimplePositionSystem::SimplePositionNode(obj);
	node->value = Get_Int_Parameter("GroupId");
	JMG_Utility_Custom_Spawn_System_Controller::spawnPoints += node;
	Commands->Destroy_Object(obj);
}
void JMG_Utility_Custom_Spawn_System::Created(GameObject *obj)
{
	if (Get_Int_Parameter("Spawn_Point_Group") != -1 && !JMG_Utility_Custom_Spawn_System_Controller::setup)
	{
		Console_Input("msg JMG_Utility_Custom_Spawn_System_Point ERROR:: Make sure you have the script JMG_Utility_Custom_Spawn_System_Controller placed on the map!");
		Commands->Destroy_Object(obj);
		return;
	}
	spawnLimit = Get_Int_Parameter("Spawn_Limit");
	enabled = Get_Int_Parameter("Starts_Enabled") ? true : false;
	totalChance = 0;
	char parameter[128];
	for (int x = 0;x < 10;x++)
	{
		sprintf(parameter,"Chance%0d",x);
		totalChance += Get_Int_Parameter(parameter);
		available[x] = Get_Int_Parameter(parameter) ? true : false;
		eachChance[x] = totalChance;
	}
	if (Get_Int_Parameter("Delay_First_Spawn"))
	{
		avaliableToSpawn = 0;
		Commands->Send_Custom_Event(obj,obj,40340,this->Get_ID(),0);
	}
	else
		avaliableToSpawn = Get_Int_Parameter("Max_Spawned_At_Once");
	Commands->Start_Timer(obj,this,2.5f,1);
}
void JMG_Utility_Custom_Spawn_System::Timer_Expired(GameObject *obj,int number)
{
	if (1 == number)
	{
		if (avaliableToSpawn && enabled && spawnLimit)
		{
			if (spawnLimit > 0)
				spawnLimit--;
			Vector3 pos = Commands->Get_Position(obj);
			float facing = Commands->Get_Facing(obj);
			if (Get_Int_Parameter("Spawn_Point_Group") != -1)
			{
				Rp2SimplePositionSystem::SimplePositionNode *node = JMG_Utility_Custom_Spawn_System_Controller::spawnPoints.GetRandomFromGroup(Get_Int_Parameter("Spawn_Point_Group"));
				if (node)
				{
					pos = node->position;
					facing = node->facing;
				}
			}
			GameObject *spawn = Create_Preset(pos);
			if (spawn)
			{
				Commands->Set_Facing(spawn,facing);
				MoveablePhysClass *mphys = (spawn->As_PhysicalGameObj() ? spawn->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL);
				if (mphys && mphys->Find_Teleport_Location(pos,1.25f,&pos))
					Commands->Set_Position(spawn,pos);
				if (!mphys || mphys->Can_Teleport(Matrix3D(pos)))
				{
					avaliableToSpawn--;
					char params[128];
					sprintf(params,"%d,%d",Commands->Get_ID(obj),this->Get_ID());
					Commands->Attach_Script(spawn,"JMG_Utility_Custom_Spawn_System_Attached",params);
				}
				else
					Commands->Destroy_Object(spawn);
			}
		}
		Commands->Start_Timer(obj,this,0.25f,1);
	}
}
void JMG_Utility_Custom_Spawn_System::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 40340 && param == this->Get_ID())
	{
		Commands->Send_Custom_Event(obj,obj,43594,this->Get_ID(),Get_Float_Parameter("Respawn_Delay")+(Get_Float_Parameter("Random_Delay") ? Commands->Get_Random(0,Get_Float_Parameter("Random_Delay")) : 0));
	}
	if (message == 43594 && param == this->Get_ID())
	{
		avaliableToSpawn++;
	}
	if (message == Get_Int_Parameter("Enable_Message"))
	{
		enabled = true;
	}
	if (message == Get_Int_Parameter("Disable_Message"))
	{
		enabled = false;
	}
}
GameObject *JMG_Utility_Custom_Spawn_System::Create_Preset(Vector3 location)
{
	int random = totalChance ? Commands->Get_Random_Int(0,totalChance+1) : 0;
	for (int x = 9;x >= 0;x--)
		if (((x == 10 && random >= eachChance[x]) || (x <= 9 && random < eachChance[x+1] && random >= eachChance[x])) && available[x])
		{
			char parameter[128];
			sprintf(parameter,"Preset%0d",x);
			return Commands->Create_Object(Get_Parameter(parameter),location);
		}
	return NULL;
}
void JMG_Utility_Custom_Spawn_System_Attached::Destroyed(GameObject *obj)
{
	GameObject *spawner = Commands->Find_Object(Get_Int_Parameter("SpawnerID"));
	if (spawner)
		Commands->Send_Custom_Event(obj,spawner,40340,Get_Int_Parameter("ScriptID"),0);
}
void JMG_Bear_Hunter_Player_Count_Scaled_Object_Health::Created(GameObject *obj)
{
	originalMaxHealth = Commands->Get_Max_Health(obj);
	originalPoints = Get_Death_Points(obj);
	damaged = false;
	PlayerCount = 1;
	Commands->Start_Timer(obj,this,5.0f,1);
	Timer_Expired(obj,4);
}
void JMG_Bear_Hunter_Player_Count_Scaled_Object_Health::Timer_Expired(GameObject *obj,int number)
{
	if (1 == number)
	{
		int TempPlayerCount = Get_Player_Count();
		if (PlayerCount < TempPlayerCount)
		{
			float additionalHealth = (TempPlayerCount-PlayerCount)*2.5f;
			originalMaxHealth += additionalHealth;
			PlayerCount = TempPlayerCount;
		}
		Commands->Start_Timer(obj,this,5.0f,1);
	}
	if (number == 4)
	{
		if (damaged)
			return;
		Commands->Set_Health(obj,JmgUtility::MathClamp(originalMaxHealth*0.5f+Get_Player_Count()*25.0f,originalMaxHealth*0.333f,originalMaxHealth));
		Set_Death_Points(obj,(Commands->Get_Health(obj)/originalMaxHealth)*originalPoints);
		Commands->Start_Timer(obj,this,Commands->Get_Random(10,20),4);
	}
}
void JMG_Bear_Hunter_Player_Count_Scaled_Object_Health::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damager || Commands->Get_Player_Type(damager) != 1)
		return;
	if (!Commands->Get_Health(damager))
		return;
	if (!damaged && damage > 0)
		damaged = true;
}
void JMG_Bear_Hunter_Wolf_Pack_Definition::Created(GameObject *obj)
{
	if (!bearHunterWolfHivemindControl.addNode(Get_Int_Parameter("PackId"),Get_Parameter("PresetName"),Get_Int_Parameter("PackSize"),Get_Vector3_Parameter("TerritoryCenterPosition"),Get_Float_Parameter("TerritoryRange"),Get_Int_Parameter("WanderTime"),Get_Int_Parameter("WanderTimeRandom"),Get_Float_Parameter("RespawnTime"),Get_Float_Parameter("RandomRespawnTime"),Get_Float_Parameter("SoundScaler"),Get_Int_Parameter("ReactToGlobalSound") ? true : false))
	{
		char errorMsg[220];
		sprintf(errorMsg,"msg JMG_Bear_Hunter_Wolf_Pack_Definition ERROR: Wolf Pack ID %d already defined!",Get_Int_Parameter("PackId"));
		Console_Input(errorMsg);
	}
}
void JMG_Bear_Hunter_Wolf_Pack_Controller::Created(GameObject *obj)
{
	bearHunterWolfHivemindControl.controllerId = Commands->Get_ID(obj);
	Commands->Start_Timer(obj,this,5.0f,1);
}
void JMG_Bear_Hunter_Wolf_Pack_Controller::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		bearHunterWolfHivemindControl.update();
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Bear_Hunter_Wolf_Pack_Controller::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 9348470)
	{
		BearHunterWolfHivemindSystem::BearHunterWolfHivemindNode *node = bearHunterWolfHivemindControl.find(param);
		if (!node)
			return;
		Commands->Send_Custom_Event(obj,obj,9348471,param,0);
	}
	if (message == 9348471)
	{
		BearHunterWolfHivemindSystem::BearHunterWolfHivemindNode *node = bearHunterWolfHivemindControl.find(param);
		if (!node)
			return;
		if (node)
			node->currentPackSize--;
	}
}
void JMG_Bear_Hunter_Wolf_Pack_Controller::Destroyed(GameObject *obj)
{
	bearHunterWolfHivemindControl.Empty_List();
}
void JMG_Bear_Hunter_Wolf::Created(GameObject *obj)
{
	controller = bearHunterWolfHivemindControl.find(Get_Int_Parameter("WolfPackID"));
	if (!controller)
	{
		char errorMsg[220];
		sprintf(errorMsg,"msg JMG_Bear_Hunter_Wolf ERROR: Wolf Pack ID %d not defined!",Get_Int_Parameter("WolfPackID"));
		Console_Input(errorMsg);
		Remove_Script(obj,"JMG_Bear_Hunter_Wolf");
		return;
	}
	controller->currentPackSize++;
	controller->livingCount++;
	damaged = false;
	returnHome = 0;
	PlayerCount = 1;
	CanHearDelay = 0;
	waitcount = 0;
	vehicleChaseTime = 0;
	vehicleChaseTimeReset = 0;
	soundUpdate = 0;
	maxVehicleChaseTime = Commands->Get_Random_Int(300,900);
	returnHomeDelay = Commands->Get_Random_Int(10,90);
	lastPos = Commands->Get_Position(obj);
	Commands->Enable_Enemy_Seen(obj,true);
	Commands->Start_Timer(obj,this,5.0f,1);
	Commands->Start_Timer(obj,this,0.1f,2);
	Commands->Start_Timer(obj,this,0.25f,3);
	Commands->Start_Timer(obj,this,1.25f,5);
}
void JMG_Bear_Hunter_Wolf::Enemy_Seen(GameObject *obj,GameObject *seen)	
{
	if (seen->As_SmartGameObj() && seen->As_SmartGameObj()->Is_Stealthed() && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 900)
		return;
	if (Is_Script_Attached(seen,"JMG_Bear_Hunter_AI_Ignore"))
		return;
	if (Is_Script_Attached(seen,"JMG_Bear_Hunter_AI_Ignore_Up_To_75_Meters") && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 5625)
		return;
	if (Is_Script_Attached(seen,"JMG_Bear_Hunter_AI_Ignore_Up_To_150_Meters") && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 22500)
		return;
	if (Is_Script_Attached(seen,"JMG_AI_Ignore_Object"))
		return;
	if (Commands->Is_A_Star(seen) && JMG_Bear_Hunter_Game_Control::aiIgnorePlayers[JmgUtility::JMG_Get_Player_ID(seen)])
		return;
	if (vehicleChaseTimeReset && (seen->As_VehicleGameObj() || Get_Vehicle(seen)) && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(seen)) > 225)
		return;
	int SeenID = Commands->Get_ID(seen);
	if (!currentTargetId)
	{
		lastSeen = 30;
		controller->huntEnemy = true;
		controller->heardSound = false;
		currentTargetId = SeenID;
		if (!controller->secondaryTargetId)
		{
			controller->secondaryTargetId = SeenID;
			controller->lastSeenSecondary = 30;
		}
	}
	else if (currentTargetId == SeenID)
		lastSeen = 30;
	if (currentTargetId && !controller->secondaryTargetId && SeenID != currentTargetId)
	{
		controller->secondaryTargetId = SeenID;
		controller->lastSeenSecondary = 30;
	}
	if (SeenID == controller->secondaryTargetId && currentTargetId != controller->secondaryTargetId)
		controller->lastSeenSecondary = 30;
}
void JMG_Bear_Hunter_Wolf::Custom(GameObject *obj,int message,int param,GameObject *sender)
{
	if (message == 91185423)
	{
		GameObject *Player = JmgUtility::FindNearestPlayer(Commands->Get_Position(obj));
		if (!Player)
			return;
		ActionParamsStruct params;
		params.Set_Movement(Commands->Get_Position(Player),1.0,10.0f,false);
		params.MovePathfind = true;
		Commands->Action_Goto(obj,params);
	}
}
void JMG_Bear_Hunter_Wolf::Timer_Expired(GameObject *obj,int number)
{
	if (2 == number)
	{
		if (CanHearDelay)
			CanHearDelay--;
		if (soundUpdate)
			soundUpdate--;
		if (lastSeen)
		{
			lastSeen--;
			if (!lastSeen && currentTargetId)
			{
				Commands->Action_Reset(obj,10);
				currentTargetId = 0;
				GameObject *secondaryEnemy = Commands->Find_Object(controller->secondaryTargetId);
				if (secondaryEnemy && Commands->Get_Health(secondaryEnemy))
				{
					currentTargetId = controller->secondaryTargetId;
					ActionParamsStruct params;
					Commands->Get_Action_Params(obj,params);
					params.Set_Movement(secondaryEnemy,1.0,1.4f,false);
					params.MovePathfind = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(secondaryEnemy)) < 25.0 ? false : true;
					Commands->Action_Goto(obj,params);
					params.Set_Basic(this,999,10);
					lastSeen = controller->lastSeenSecondary;
				}
			}
		}
		if (currentTargetId)
		{
			if (vehicleChaseTimeReset)
				vehicleChaseTimeReset--;
			GameObject *target = Commands->Find_Object(currentTargetId);
			if (target && target->As_VehicleGameObj() || Get_Vehicle(target))
			{
				vehicleChaseTime++;
				if (vehicleChaseTime >= maxVehicleChaseTime && JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(target)) > 225)
				{
					vehicleChaseTime = 0;
					Commands->Action_Reset(obj,10);
					controller->secondaryTargetId = 0;
					target = NULL;
					vehicleChaseTimeReset = Commands->Get_Random_Int(300,450);
				}
			}
			if (!target || Commands->Get_Health(target) == 0.0f)
			{
				waitcount = 0;
				controller->secondaryTargetId = 0;
				Commands->Action_Reset(obj,10);
				GameObject *secondaryEnemy = Commands->Find_Object(controller->secondaryTargetId);
				if (secondaryEnemy && Commands->Get_Health(secondaryEnemy))
				{
					currentTargetId = controller->secondaryTargetId;
					controller->secondaryTargetId = 0;
					ActionParamsStruct params;
					Commands->Get_Action_Params(obj,params);
					params.Set_Movement(secondaryEnemy,1.0,1.4f,false);
					params.MovePathfind = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(secondaryEnemy)) < 25.0 ? false : true;
					Commands->Action_Goto(obj,params);
					params.Set_Basic(this,999,10);
					lastSeen = controller->lastSeenSecondary;
				}
				else
				{
					returnHome = 0;
					returnHomeDelay = Commands->Get_Random_Int(1,30);
				}
			}
			else if (JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(target)) > 40000.0f && controller->huntEnemy)
			{
				currentTargetId = 0;
				waitcount = 0;
				if (Commands->Get_Random(0.0f,1.0f) < 0.66f)
					Commands->Action_Reset(obj,10);
			}
			else
			{
				waitcount++;
				if (waitcount > 2)
				{
					waitcount = 0;
					ActionParamsStruct params;
					Commands->Get_Action_Params(obj,params);
					params.Set_Movement(target,1.0,1.4f,false);
					params.MovePathfind = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(target)) < 25.0 ? false : true;
					Commands->Action_Goto(obj,params);
					params.Set_Basic(this,999,10);
					if (Get_Vehicle(target) || target->As_VehicleGameObj())
					{
						params.AttackCheckBlocked = false;
						params.Set_Attack(target,12.5,1,Get_Player_Count() < 10 ? true : false);
					}
					else
						params.Set_Attack(target,3.0f,1,Get_Player_Count() < 10 ? true : false);
					Commands->Action_Attack(obj,params);
				}
			}
		}
		else if (!soundUpdate && controller->heardSound)
		{
			soundUpdate = 30;
			ActionParamsStruct params;
			Commands->Get_Action_Params(obj,params);
			params.Set_Movement(controller->soundHeardPos,1.0f,controller->soundId == 6 ? 25.0f : 1.4f,false);
			params.MovePathfind = true;
			Commands->Action_Goto(obj,params);
		}
		Vector3 Pos = Commands->Get_Position(obj);
		if (JmgUtility::SimpleDistance(lastPos,Pos) < 0.1f)
		{
			returnHome++;
			if (returnHome > 240 && JmgUtility::SimpleDistance(lastPos,controller->targetWanderPosition) > 25.0f)
			{
				Vector3 pos = controller->targetWanderPosition;
				MoveablePhysClass *mphys = obj->As_PhysicalGameObj() ? obj->As_PhysicalGameObj()->Peek_Physical_Object()->As_MoveablePhysClass() : NULL;
				if (mphys && !mphys->Can_Teleport(Matrix3D(pos)))
				{
					mphys->Find_Teleport_Location(pos,2.5f,&pos);
					Commands->Set_Position(obj,pos);
				}
			}
			else if (!(returnHome % 95) || returnHome == returnHomeDelay)
			{
				Commands->Action_Reset(obj,10);
				ActionParamsStruct params;
				params.Set_Basic(this,999,10);
				params.MovePathfind = true;
				Vector3 randomTargetLocation = controller->targetWanderPosition;
				for (int x = 0;x < 5;x++)
					if (Get_Random_Pathfind_Spot(randomTargetLocation,controller->territoryRange,&randomTargetLocation))
					{
						params.Set_Movement(randomTargetLocation,1.0f,4.0f,false);
						Commands->Action_Goto(obj,params);
						break;
					}
			}
		}
		else
			returnHome = 0;
		lastPos = Pos;
		Commands->Start_Timer(obj,this,0.1f,2);
	}
	if (1 == number)
	{
		GameObject *secondaryEnemy = Commands->Find_Object(controller->secondaryTargetId);
		if (controller->secondaryTargetId && (!secondaryEnemy || !Commands->Get_Health(secondaryEnemy)))
			controller->secondaryTargetId = 0;
		Commands->Start_Timer(obj,this,5.0f,1);
	}
	if (3 == number)
	{
		Vector3 myPos = Commands->Get_Position(obj);
		for (int x = 0;x < 128;x++)
			if (PlayerEmulatedSoundList[x].SoundType)
			{
				GameObject *Player = Get_GameObj(x);
				if (!Player || Get_Player_Type(Player) == -4 || !Commands->Get_Health(Player))
					continue;
				Vector3 Pos = Commands->Get_Position(Player);
				if (JmgUtility::SimpleDistance(Pos,myPos) <= PlayerEmulatedSoundList[x].Range)
				{
					CombatSound CS = CombatSound();
					CS.Creator = Player;
					CS.Position = Pos;
					CS.sound = PlayerEmulatedSoundList[x].SoundType;
					SoundDHeard(obj,CS);
				}
			}
		Commands->Start_Timer(obj,this,0.1f,3);
	}
	if (number == 5 && Commands->Get_Health(obj))
	{
		Vector3 pos = Commands->Get_Position(obj);
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player || Get_Player_Type(player) == -4 || Get_Vehicle(player))
				continue;
			if (JmgUtility::SimpleFlatDistance(pos,Commands->Get_Position(player)) <= 0.25 && JmgUtility::SimpleDistance(pos,Commands->Get_Position(player)) <= 25)
				Commands->Apply_Damage(player,9999.9f,"None",obj);
		}
		Commands->Start_Timer(obj,this,1.25f,5);
	}
}
void JMG_Bear_Hunter_Wolf::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damager || Commands->Get_Player_Type(damager) != 1)
		return;
	if (!Commands->Get_Health(damager))
		return;
	if (!damaged && damage > 0)
		damaged = true;
	if (Commands->Is_A_Star(damager) && JMG_Bear_Hunter_Game_Control::aiIgnorePlayers[JmgUtility::JMG_Get_Player_ID(damager)])
		return;
	if (Is_Script_Attached(damager,"JMG_AI_Ignore_Object"))
		return;
	GameObject *target = Commands->Find_Object(currentTargetId);
	if (target)
	{
		float targetDistance = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(target));
		float damagerDistance = JmgUtility::SimpleDistance(Commands->Get_Position(obj),Commands->Get_Position(damager));
		if (targetDistance < damagerDistance)
			return;
	}
	lastSeen = 30;
	controller->huntEnemy = false;
	controller->heardSound = false;
	waitcount = 0;
	currentTargetId = Commands->Get_ID(damager);
	if (!controller->secondaryTargetId)
	{
		controller->secondaryTargetId = currentTargetId;
		controller->lastSeenSecondary = 30;
	}
	if (damager->As_VehicleGameObj() || Get_Vehicle(damager))
		vehicleChaseTimeReset = 0;
}
void JMG_Bear_Hunter_Wolf::Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason)
{
	if (reason == ACTION_COMPLETE_PATH_BAD_DEST)
	{

		if (Commands->Get_Health(obj) <= 0)
			return;
		GameObject *target = Commands->Find_Object(currentTargetId);
		int playerId = JmgUtility::JMG_Get_Player_ID(target);
		if (playerId && Commands->Get_Health(target))
			JMG_Bear_Hunter_Game_Control::aiIgnorePlayers[playerId] = 30;
	}
	if (reason == MOVEMENT_COMPLETE_ARRIVED)
	{
		controller->heardSound = false;
	}
}
void JMG_Bear_Hunter_Wolf::SoundDHeard(GameObject *obj,const CombatSound &sound)
{
	if (!sound.Creator || !controller)
		return;
	if (!Commands->Is_A_Star(sound.Creator) || CanHearDelay || currentTargetId || controller->heardSound)// was 225 
		return;
	Vector3 MyPos = Commands->Get_Position(obj);
	float dist = JmgUtility::SimpleDistance(sound.Position,MyPos)*controller->listenerScale;
	bool Success = false;//(dist > 225 && Commands->Get_Random(0.00f,1.00f) > 0.015)fail
	if (dist < 144)// Sound is near, must investigate
		Success = true;
	else if (sound.sound == 6 && controller->hearsGlobalSounds && dist < 250000 && Commands->Get_Random(0.00f,1.00f) < JMG_Bear_Hunter_Game_Control::soundAttentionWhoreNotify)//player just wants to be noticed
		Success = true;
	else if (Commands->Get_Random(0.00f,1.00f) < 0.00025 && sound.sound != 5)// Bad Luck
		Success = true;
	else if (sound.sound == 5 && Commands->Get_Random(0.00f,1.00f) < 0.0015)// Sound is bullet hits
		Success = true;
	else if (dist < 2025 && sound.sound != 5)// Sound is near and not bullet hits must check
		Success = true;
	else if (dist < 8100 && sound.sound != 5 && Commands->Get_Random(0.00f,1.00f) < 0.0025)// Sound is near and not bullet hits decided to check
		Success = true;
	else if (dist < 90000 && sound.sound != 5 && !controller->heardSound)// Sound is really close but player is far away
		Success = true;// Sound is far away but chances allowed it to successfully fall into the special "Everyone hates you" condition
	if (!Success)
		return;
	CanHearDelay = Commands->Get_Random_Int(30,90);
	controller->heardSound = true;
	controller->soundId = sound.sound;
	controller->soundHeardPos = sound.Position;
}
void JMG_Bear_Hunter_Wolf::Destroyed(GameObject *obj)
{
	if (!controller)
		return;
	controller->livingCount--;
	GameObject *packControl = Commands->Find_Object(Get_Int_Parameter("ControllerId"));
	if (packControl)
		Commands->Send_Custom_Event(obj,packControl,9348470,Get_Int_Parameter("WolfPackID"),0);
}
void JMG_Bear_Hunter_Wolf_Wander_Point::Created(GameObject *obj)
{
	Commands->Start_Timer(obj,this,1.0f,5);
}
void JMG_Bear_Hunter_Wolf_Wander_Point::Timer_Expired(GameObject *obj,int number)
{
	BearHunterWolfHivemindSystem::BearHunterWolfHivemindNode *node = bearHunterWolfHivemindControl.find(Get_Int_Parameter("WolfPackID"));
	if (!node)
	{
		char errorMsg[220];
		sprintf(errorMsg,"msg JMG_Bear_Hunter_Wolf ERROR: Wolf Pack ID %d not defined!",Get_Int_Parameter("WolfPackID"));
		Console_Input(errorMsg);
		return;
	}
	node->wanderPositionNodes += obj;
	Commands->Destroy_Object(obj);
}
void JMG_Bear_Hunter_Armored_Car::Created(GameObject *obj)
{
	lightsOn = false;
	Commands->Set_Animation(obj,"ArmoredCar.ArmoredCar",false,0,0,0,true);
	Commands->Start_Timer(obj,this,0.1f,1);
}
void JMG_Bear_Hunter_Armored_Car::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		if (!Get_Current_Bullets(obj))
		{
			Set_Current_Bullets(obj,1);
			lightsOn = !lightsOn;
			if (lightsOn)
			{
				Remove_Script(obj,"JMG_Bear_Hunter_AI_Ignore_Up_To_75_Meters");
				Commands->Set_Animation(obj,"ArmoredCar.ArmoredCar",false,0,1,1,true);
			}
			else
			{
				Attach_Script_Once(obj,"JMG_Bear_Hunter_AI_Ignore_Up_To_75_Meters","");
				Commands->Set_Animation(obj,"ArmoredCar.ArmoredCar",false,0,0,0,true);
			}
		}
		Commands->Start_Timer(obj,this,0.1f,1);
	}
}
void JMG_Bear_Hunt_Final_Boss_Support::Created(GameObject *obj)
{// You're doing to good if you see these guys
	Set_Skin(obj,"Blamo");
	Commands->Set_Shield_Type(obj,"Blamo");
	Set_Max_Health(obj,JmgUtility::MathClampInt(Get_Player_Count()+1,1,64)*10.37f);
	Commands->Set_Health(obj,JmgUtility::MathClampInt(Get_Player_Count()+1,1,64)*10.37f);
	if (JmgUtility::MathClampInt(Get_Player_Count()+1,1,64)*10.37f >= 2000.0f)
	{
		Set_Max_Shield_Strength(obj,JmgUtility::MathClampInt(Get_Player_Count()-64,0,64)*10.37f);
		Commands->Set_Shield_Strength(obj,JmgUtility::MathClampInt(Get_Player_Count()-64,0,64)*10.37f);
	}
	else
		Set_Max_Shield_Strength(obj,0.0f);
	GameObject *jump = Commands->Create_Object("Daves Arrow",Vector3(0.532f,-705.389f,0.0f));
	float minFacing[8] = {0,30,90,115,150,-180,-145,-70},maxFacing[8] = {30,65,110,140,180,-155,-110,-45};
	JMG_Bear_Hunt_Final_Boss::bossChoose++;
	if (JMG_Bear_Hunt_Final_Boss::bossChoose >= 8)
		JMG_Bear_Hunt_Final_Boss::bossChoose = 0;
	Commands->Set_Facing(jump,Commands->Get_Random(minFacing[JMG_Bear_Hunt_Final_Boss::bossChoose],maxFacing[JMG_Bear_Hunt_Final_Boss::bossChoose]));
	Commands->Set_Model(jump,"rabbitJump");
	Commands->Set_Animation(jump,"rabbitJump.rabbitJump",false,0,0.0f,90.0f,false);
	Commands->Attach_To_Object_Bone(obj,jump,"Bunny");
	jumperId = Commands->Get_ID(jump);
	lastHealth = Commands->Get_Health(obj);
	lastArmor = Commands->Get_Shield_Strength(obj);
	Commands->Start_Timer(obj,this,3.0f,1);
}
void JMG_Bear_Hunt_Final_Boss_Support::Timer_Expired(GameObject *obj,int number)
{
	if (number == 1)
	{
		GameObject *jumper = Commands->Find_Object(jumperId);
		if (jumper && Get_Animation_Frame(jumper) < 90)
			Commands->Start_Timer(obj,this,0.1f,1);
		else
		{
			GameObject *jumper = Commands->Find_Object(jumperId);
			if (jumper)
				Commands->Destroy_Object(jumper);
			Set_Skin(obj,"BossRabbitHealth");
			Commands->Set_Shield_Type(obj,"BossRabbitHealth");
			Commands->Attach_Script(obj,"JMG_Bear_Hunter_Engineer_AI","Weapon_Blue_Deer_Repair,Weapon_Blue_Deer_Attack");
		}
	}
}
void JMG_Bear_Hunt_Final_Boss_Support::Damaged(GameObject *obj,GameObject *damager,float damage)
{
	if (!damager && (JMG_Bear_Hunter_Game_Control::earthWarheadId == Get_Damage_Warhead() || obj == damager) && damage > 0.0f && lastHealth)
	{
		Commands->Set_Health(obj,lastHealth);
		Commands->Set_Shield_Strength(obj,lastArmor);
		return;
	}
	if (damage > 30.0f && damager)
	{
		if (lastArmor)
		{
			lastArmor -= damage*0.5f;
			if (lastArmor < 0)
			{
				lastHealth += lastArmor;
				lastArmor = 0;
			}
		}
		else
			lastHealth -= damage*0.5f;
		if (lastHealth <= 0)
		{
			Commands->Apply_Damage(0,9999.9f,"BlamoKiller",obj);
			return;
		}
		Commands->Set_Health(obj,lastHealth);
		Commands->Set_Shield_Strength(obj,lastArmor);
	}
	lastHealth = Commands->Get_Health(obj);
	lastArmor = Commands->Get_Shield_Strength(obj);
}
bool JMG_Utility_AI_Vehicle::GetRandomPosition(Vector3 *position)
{
	Rp2SimplePositionSystem::SimplePositionNode *node = NULL;
	if (Get_Int_Parameter("WanderingAIGroupID") != -1)
		node = JMG_Wandering_AI_Controller::wanderPoints.GetRandomFromGroup(Get_Int_Parameter("WanderingAIGroupID"));
	if (!node)
	{
		Console_Input("msg JMG_Utility_AI_Vehicle ERROR: No wander points could be found for that group!");
		*position = Vector3();
		return false;
	}
	*position = node->position;
	return true;
}
bool JMG_Utility_AI_Vehicle::GetRandomPositionOutsideOfRange(Vector3 *position)
{
	Rp2SimplePositionSystem::SimplePositionNode *node = NULL;
	if (Get_Int_Parameter("WanderingAIGroupID") != -1)
		node = JMG_Wandering_AI_Controller::wanderPoints.GetRandomOutsideOfRangeInGroup(Get_Int_Parameter("WanderingAIGroupID"),Get_Float_Parameter("MinAttackRange")*Get_Float_Parameter("MinAttackRange"),*position);
	if (!node)
	{
		*position = Vector3();
		return false;
	}
	*position = node->position;
	return true;
}
bool JMG_Utility_AI_Guardian_Aircraft::Get_A_Defense_Point(Vector3 *position)
{
	Rp2SimplePositionSystem::SimplePositionNode *node = NULL;
	if (Get_Int_Parameter("WanderingAIGroupID") != -1)
		node = JMG_Wandering_AI_Controller::wanderPoints.GetRandomFromGroup(Get_Int_Parameter("WanderingAIGroupID"));
	if (!node)
	{
		Console_Input("msg JMG_Utility_AI_Guardian_Aircraft ERROR: No wander points could be found for that group!");
		return false;
	}
	*position = node->position;
	return true;
}
ScriptRegistrant<JMG_Bear_Hunter_Player_Soldier> JMG_Bear_Hunter_Player_Soldier_Registrant("JMG_Bear_Hunter_Player_Soldier","");
ScriptRegistrant<JMG_Rp2_Dedicated_Server_Sound_Emulator> JMG_Rp2_Dedicated_Server_Sound_Emulator_Registrant("JMG_Rp2_Dedicated_Server_Sound_Emulator","");
ScriptRegistrant<JMG_Bear_Hunter_Game_Control> JMG_Bear_Hunter_Game_Control_Registrant("JMG_Bear_Hunter_Game_Control","PositionFile=DhSwampDeerPositions.ecw:string");
ScriptRegistrant<JMG_Rp2_Hostile_Mutant_AI> JMG_Rp2_Hostile_Mutant_AI_Registrant("JMG_Rp2_Hostile_Mutant_AI","");
ScriptRegistrant<JMG_Bear_Hunt_Mutant_Attacker> JMG_Bear_Hunt_Mutant_Attacker_Registrant("JMG_Bear_Hunt_Mutant_Attacker","Speed:float");
ScriptRegistrant<JMG_Bear_Hunter_Animal_Control> JMG_Bear_Hunter_Animal_Control_Registrant("JMG_Bear_Hunter_Animal_Control","");
ScriptRegistrant<JMG_Security_Camera_Behavior> JMG_Security_Camera_Behavior_Registrant("JMG_Security_Camera_Behavior","Angle:float,Alarm_ID=0:int,Is_Gun=1:int,Delay=0:float,Alarm_Message=0:int,Alarm_Sound=Beep:string,UpdateRateMultiplier=1.0:float,IdleAimZAngleModifier=0.0:float,FloodLightPreset=null:string,Power_Message=0:int");
ScriptRegistrant<JMG_Bear_Hunt_Mutant_Cat_Explosion> JMG_Bear_Hunt_Mutant_Cat_Explosion_Registrant("JMG_Bear_Hunt_Mutant_Cat_Explosion","KillSelfExplosion:string,KilledExplosion:string");
ScriptRegistrant<JMG_Bear_Hunter_President_Controller> JMG_Bear_Hunter_President_Controller_Registrant("JMG_Bear_Hunter_President_Controller","");
ScriptRegistrant<JMG_Bear_Hunter_Turret_Death_Alert> JMG_Bear_Hunter_Turret_Death_Alert_Registrant("JMG_Bear_Hunter_Turret_Death_Alert","");
ScriptRegistrant<JMG_Powerup_Grant_Cash> JMG_Powerup_Grant_Cash_Registrant("JMG_Powerup_Grant_Cash","Money:float");
ScriptRegistrant<JMG_Bear_Hunter_Powerup_Tracker> JMG_Bear_Hunter_Powerup_Tracker_Registrant("JMG_Bear_Hunter_Powerup_Tracker","");
ScriptRegistrant<JMG_Bear_Hunter_Grenade_Vest_Powerup> JMG_Bear_Hunter_Grenade_Vest_Powerup_Registrant("JMG_Bear_Hunter_Grenade_Vest_Powerup","");
ScriptRegistrant<JMG_Powerup_Grant_Weapon_Clips> JMG_Powerup_Grant_Weapon_Clips_Registrant("JMG_Powerup_Grant_Weapon_Clips","GrantClips:int");
ScriptRegistrant<JMG_Bear_Hunter_SpawnPoint> JMG_Bear_Hunter_SpawnPoint_Registrant("JMG_Bear_Hunter_SpawnPoint","GroupId:int");
ScriptRegistrant<JMG_Bear_Hunter_Player_Spawn> JMG_Bear_Hunter_Player_Spawn_Registrant("JMG_Bear_Hunter_Player_Spawn","");
ScriptRegistrant<JMG_Bear_Hunter_Player_Vehicle> JMG_Bear_Hunter_Player_Vehicle_Registrant("JMG_Bear_Hunter_Player_Vehicle","");
ScriptRegistrant<JMG_Bear_Hunter_Bear_Tracker> JMG_Bear_Hunter_Bear_Tracker_Registrant("JMG_Bear_Hunter_Bear_Tracker","");
ScriptRegistrant<JMG_Bear_Hunter_Mutant_Tracker> JMG_Bear_Hunter_Mutant_Tracker_Registrant("JMG_Bear_Hunter_Mutant_Tracker","");
ScriptRegistrant<JMG_AI_Ignore_Object> JMG_AI_Ignore_Object_Registrant("JMG_AI_Ignore_Object","");
ScriptRegistrant<JMG_Bear_Hunter_Security_Turret_Truck> JMG_Bear_Hunter_Security_Turret_Truck_Registrant("JMG_Bear_Hunter_Security_Turret_Truck","");
ScriptRegistrant<JMG_Bear_Hunter_Security_Turret_Truck_Zone> JMG_Bear_Hunter_Security_Turret_Truck_Zone_Registrant("JMG_Bear_Hunter_Security_Turret_Truck_Zone","");
ScriptRegistrant<JMG_Bear_Hunt_Final_Boss> JMG_Bear_Hunt_Final_Boss_Registrant("JMG_Bear_Hunt_Final_Boss","");
ScriptRegistrant<JMG_Bear_Hunter_Spectator> JMG_Bear_Hunter_Spectator_Registrant("JMG_Bear_Hunter_Spectator","");
ScriptRegistrant<JMG_Set_Player_Type_On_Create> JMG_Set_Player_Type_On_Create_Registrant("JMG_Set_Player_Type_On_Create","PlayerType:int");
ScriptRegistrant<JMG_Bear_Hunter_Player_Assist_AI> JMG_Bear_Hunter_Player_Assist_AI_Registrant("JMG_Bear_Hunter_Player_Assist_AI","ID:int");
ScriptRegistrant<JMG_Bear_Hunter_AI_PT_Location> JMG_Bear_Hunter_AI_PT_Location_Registrant("JMG_Bear_Hunter_AI_PT_Location","PTSetting=0:int");
ScriptRegistrant<JMG_Bear_Hunter_AI_Defense_Point> JMG_Bear_Hunter_AI_Defense_Point_Registrant("JMG_Bear_Hunter_AI_Defense_Point","GroupId:int");
ScriptRegistrant<JMG_Bear_Hunter_Warning_Light_Zone> JMG_Bear_Hunter_Warning_Light_Zone_Registrant("JMG_Bear_Hunter_Warning_Light_Zone","LightID:int");
ScriptRegistrant<JMG_Bear_Hunter_Vehicle_Purchase_System> JMG_Bear_Hunter_Vehicle_Purchase_System_Registrant("JMG_Bear_Hunter_Vehicle_Purchase_System","");
ScriptRegistrant<JMG_Airdrop_Attached_Object_On_Create> JMG_Airdrop_Attached_Object_On_Create_Registrant("JMG_Airdrop_Attached_Object_On_Create","DeathZoneID:int,WaypathId=0:int,HelicopterPreset=GDI_Transport_Helicopter:string");
ScriptRegistrant<JMG_Animate_While_Firing> JMG_Animate_While_Firing_Registrant("JMG_Animate_While_Firing","SpinDownTime=0:int");
ScriptRegistrant<JMG_Bear_Hunter_Dummy_Script> JMG_Bear_Hunter_AI_Ignore_Registrant("JMG_Bear_Hunter_AI_Ignore","");
ScriptRegistrant<JMG_Bear_Hunter_Dummy_Script> JMG_Bear_Hunter_AI_Ignore_Up_To_25_Meters_Registrant("JMG_Bear_Hunter_AI_Ignore_Up_To_25_Meters","");
ScriptRegistrant<JMG_Bear_Hunter_Dummy_Script> JMG_Bear_Hunter_AI_Ignore_Up_To_75_Meters_Registrant("JMG_Bear_Hunter_AI_Ignore_Up_To_75_Meters","");
ScriptRegistrant<JMG_Bear_Hunter_Dummy_Script> JMG_Bear_Hunter_AI_Ignore_Up_To_150_Meters_Registrant("JMG_Bear_Hunter_AI_Ignore_Up_To_150_Meters","");
ScriptRegistrant<JMG_Bear_Hunter_Alarm_Switch> JMG_Bear_Hunter_Alarm_Switch_Registrant("JMG_Bear_Hunter_Alarm_Switch","TowerId:int,TowerAnimation:string,StartPosition:int,AlarmTriggerMessage:int,SwitchSwitchCustom:int,NotifyId=0:int,NotifyMessage=0:int,CreateNotifyMessage=0:int,CreateNotifyParam=0:int,LeverPullSound=Pull_Lever:string,DeactivateTime=0:int");
ScriptRegistrant<JMG_Pickup_Display_Message> JMG_Pickup_Display_Message_Registrant("JMG_Pickup_Display_Message","Message:string");
ScriptRegistrant<JMG_AI_Follow_Player_When_Near> JMG_AI_Follow_Player_When_Near_Registrant("JMG_AI_Follow_Player_When_Near","FollowDistance=50.0:float,ReturnToOriginalPosition=0:int,CanEngageEnemies=0:int,TeamAsFollowingPlayer=0:int,AlwaysFollowNearestPlayer=1:int");
ScriptRegistrant<JMG_Bear_Hunter_Engineer_Follow_Player_When_Near> JMG_Bear_Hunter_Engineer_Follow_Player_When_Near_Registrant("JMG_Bear_Hunter_Engineer_Follow_Player_When_Near","MyId:int,FollowDistance=50.0:float,ReturnToOriginalPosition=0:int,CanEngageEnemies=0:int,TeamAsFollowingPlayer=0:int,AlwaysFollowNearestPlayer=1:int");
ScriptRegistrant<JMG_Bear_Hunter_Engineer_AI> JMG_Bear_Hunter_Engineer_AI_Registrant("JMG_Bear_Hunter_Engineer_AI","RepaiarGun_Preset:string,Weapon_Preset:string");
ScriptRegistrant<JMG_Bear_Hunter_Dummy_Script> JMG_Bear_Hunter_Engineer_Ignore_Registrant("JMG_Bear_Hunter_Engineer_Ignore","");
ScriptRegistrant<JMG_Bear_Hunter_Oil_Rig> JMG_Bear_Hunter_Oil_Rig_Registrant("JMG_Bear_Hunter_Oil_Rig","Animation:string,StartFrame:float,EndFrame:float,StartsEnabled=0:int");
ScriptRegistrant<JMG_Bear_Hunter_Damaged_Oil_Rig> JMG_Bear_Hunter_Damaged_Oil_Rig_Registrant("JMG_Bear_Hunter_Damaged_Oil_Rig","Animation=oilRigSkeleton.oilRigAnimJ0:string,RepairAnimation=oilRigSkeleton.OilRigAnim0:string,StartFrame=0.0:float,EndFrame=0.0:float");
ScriptRegistrant<JMG_Complete_Objective_On_Poke> JMG_Complete_Objective_On_Poke_Registrant("JMG_Complete_Objective_On_Poke","ObjectiveId:int,TeamPayout:float,PlayerPoints:float");
ScriptRegistrant<JMG_Bear_Hunter_Weapons_Container_Spawn_Point> JMG_Bear_Hunter_Weapons_Container_Spawn_Point_Registrant("JMG_Bear_Hunter_Weapons_Container_Spawn_Point","GroupId:int");
ScriptRegistrant<JMG_Bear_Hunter_Weapons_Container> JMG_Bear_Hunter_Weapons_Container_Registrant("JMG_Bear_Hunter_Weapons_Container","WeaponType:int");
ScriptRegistrant<JMG_Bear_Hunter_Weapons_Container_Attached> JMG_Bear_Hunter_Weapons_Container_Attached_Registrant("JMG_Bear_Hunter_Weapons_Container_Attached","WeaponType:int,CreateLocation:Vector3");
ScriptRegistrant<JMG_Bear_Hunter_Giant_Deer_Boss> JMG_Bear_Hunter_Giant_Deer_Boss_Registrant("JMG_Bear_Hunter_Giant_Deer_Boss","");
ScriptRegistrant<JMG_Bear_Hunter_Kill_Score_Tracker> JMG_Bear_Hunter_Kill_Score_Tracker_Registrant("JMG_Bear_Hunter_Kill_Score_Tracker","");
ScriptRegistrant<JMG_Bear_Hunter_Powerup_Score_Control> JMG_Bear_Hunter_Powerup_Score_Control_Registrant("JMG_Bear_Hunter_Powerup_Score_Control","PowerupType:int,Value:int");
ScriptRegistrant<JMG_Bear_Hunter_Increase_Score_On_Poke> JMG_Bear_Hunter_Increase_Score_On_Poke_Registrant("JMG_Bear_Hunter_Increase_Score_On_Poke","ScoreType:int");
ScriptRegistrant<JMG_Bear_Hunter_Radio_Tower_Control> JMG_Bear_Hunter_Radio_Tower_Control_Registrant("JMG_Bear_Hunter_Radio_Tower_Control","");
ScriptRegistrant<JMG_Bear_Hunter_Radio_Tower_Switch> JMG_Bear_Hunter_Radio_Tower_Switch_Registrant("JMG_Bear_Hunter_Radio_Tower_Switch","");
ScriptRegistrant<JMG_Bear_Hunter_Radio_Tower_Support> JMG_Bear_Hunter_Radio_Tower_Support_Registrant("JMG_Bear_Hunter_Radio_Tower_Support","");
ScriptRegistrant<JMG_Bear_Hunter_Death_Water_Zone> JMG_Bear_Hunter_Death_Water_Zone_Registrant("JMG_Bear_Hunter_Death_Water_Zone","");
ScriptRegistrant<JMG_Bear_Hunter_Guardian_Aircraft> JMG_Bear_Hunter_Guardian_Aircraft_Registrant("JMG_Bear_Hunter_Guardian_Aircraft","FlightHeight=25.0:float,AltWeapon=null:string,IdlePrimaryAnim=null:string,IdleSecondaryAnim=null:string,PrimaryToSecondaryAnim=null:string,SecondaryToPrimaryAnimation=null:string");
ScriptRegistrant<JMG_Bear_Hunter_Comanche> JMG_Bear_Hunter_Comanche_Registrant("JMG_Bear_Hunter_Comanche","");
ScriptRegistrant<JMG_Bear_Hunter_Comanche_Defense_Point> JMG_Bear_Hunter_Comanche_Defense_Point_Registrant("JMG_Bear_Hunter_Comanche_Defense_Point","GroupId:int");
ScriptRegistrant<JMG_Bear_Hunter_Defense_Gun_Beacon> JMG_Bear_Hunter_Defense_Gun_Beacon_Registrant("JMG_Bear_Hunter_Defense_Gun_Beacon","");
ScriptRegistrant<JMG_Bear_Hunter_Defense_Gun_Powerup> JMG_Bear_Hunter_Defense_Gun_Powerup_Registrant("JMG_Bear_Hunter_Defense_Gun_Powerup","BeaconID:int,PlayerID:int");
ScriptRegistrant<JMG_Bear_Hunter_Camera_Behavior> JMG_Bear_Hunter_Camera_Behavior_Registrant("JMG_Bear_Hunter_Camera_Behavior","Angle:float,Alarm_ID=0:int,Is_Gun=1:int,Delay=0:float,Alarm_Message=0:int,Alarm_Sound=Beep:string,PlayerID=0:int,UpdateRateMultiplier=1.0:float,Power_Message=0:int");
ScriptRegistrant<JMG_Bear_Hunter_Sentry_Turret_Placement_Tester> JMG_Bear_Hunter_Sentry_Turret_Placement_Tester_Registrant("JMG_Bear_Hunter_Sentry_Turret_Placement_Tester","Location:Vector3,BeaconId:int");
ScriptRegistrant<JMG_Powerup_Prez_Medical_Needle> JMG_Powerup_Prez_Medical_Needle_Registrant("JMG_Powerup_Prez_Medical_Needle","");
ScriptRegistrant<JMG_Prez_Medical_Needle_Player_Control> JMG_Prez_Medical_Needle_Player_Control_Registrant("JMG_Prez_Medical_Needle_Player_Control","");
ScriptRegistrant<JMG_Prez_Cinematic_C130> JMG_Prez_Cinematic_C130_Registrant("JMG_Prez_Cinematic_C130","");
ScriptRegistrant<JMG_Bear_Hunter_Power_Transformer> JMG_Bear_Hunter_Power_Transformer_Registrant("JMG_Bear_Hunter_Power_Transformer","BuildingId:int,MctId1=0:int,MctId2=0:int");
ScriptRegistrant<JMG_Bear_Hunter_Power_Transformer_MCT> JMG_Bear_Hunter_Power_Transformer_MCT_Registrant("JMG_Bear_Hunter_Power_Transformer_MCT","BuildingId:int");
ScriptRegistrant<JMG_Bear_Hunter_Mutant_Respawn_Tracker> JMG_Bear_Hunter_Mutant_Respawn_Tracker_Registrant("JMG_Bear_Hunter_Mutant_Respawn_Tracker","");
ScriptRegistrant<JMG_Wandering_AI> JMG_Wandering_AI_Registrant("JMG_Wandering_AI","GroupID=-1:int,Wander_Speed=1.0:float,Normal_Range=3.0:float,Vehicle_Range=12.5:float,Attack_Move_Range=1.4:float");
ScriptRegistrant<JMG_Wandering_AI_Controller> JMG_Wandering_AI_Controller_Registrant("JMG_Wandering_AI_Controller","");
ScriptRegistrant<JMG_Wandering_AI_Wander_Point> JMG_Wandering_AI_Wander_Point_Registrant("JMG_Wandering_AI_Wander_Point","GroupId:int");
ScriptRegistrant<JMG_Utility_Custom_Spawn_System_Controller> JMG_Utility_Custom_Spawn_System_Controller_Registrant("JMG_Utility_Custom_Spawn_System_Controller","");
ScriptRegistrant<JMG_Utility_Custom_Spawn_System_Point> JMG_Utility_Custom_Spawn_System_Point_Registrant("JMG_Utility_Custom_Spawn_System_Point","GroupId:int");
ScriptRegistrant<JMG_Utility_Custom_Spawn_System> JMG_Utility_Custom_Spawn_System_Registrant("JMG_Utility_Custom_Spawn_System","Spawn_Point_Group=-1:int,Max_Spawned_At_Once=1:int,Spawn_Limit=-1:int,Respawn_Delay=10.0:float,Random_Delay=0.0:float,Delay_First_Spawn=0:int,Starts_Enabled=1:int,Enable_Message=403401,Disable_Message=403400,Preset0=null:string,Preset1=null:string,Preset2=null:string,Preset3=null:String,Preset4=null:string,Preset5=null:string,Preset6=null:string,Preset7=null:string,Preset8=null:string,Preset9=null:string,Chance0=0:int,Chance1=0:int,Chance2=0:int,Chance3=0:int,Chance4=0:int,Chance5=0:int,Chance6=0:int,Chance7=0:int,Chance8=0:int,Chance9=0:int");
ScriptRegistrant<JMG_Utility_Custom_Spawn_System_Attached> JMG_Utility_Custom_Spawn_System_Attached_Registrant("JMG_Utility_Custom_Spawn_System_Attached","SpawnerID:int,ScriptID:int");
ScriptRegistrant<JMG_Bear_Hunter_Player_Count_Scaled_Object_Health> JMG_Bear_Hunter_Player_Count_Scaled_Object_Health_Registrant("JMG_Bear_Hunter_Player_Count_Scaled_Object_Health","");
ScriptRegistrant<JMG_Bear_Hunter_Wolf_Pack_Controller> JMG_Bear_Hunter_Wolf_Pack_Controller_Registrant("JMG_Bear_Hunter_Wolf_Pack_Controller","");
ScriptRegistrant<JMG_Bear_Hunter_Wolf_Pack_Definition> JMG_Bear_Hunter_Wolf_Pack_Definition_Registrant("JMG_Bear_Hunter_Wolf_Pack_Definition","PackId:int,PresetName:string,PackSize:int,TerritoryCenterPosition:Vector3,TerritoryRange:float,WanderTime:int,WanderTimeRandom:int,RespawnTime:float,RandomRespawnTime:float,SoundScaler:float,ReactToGlobalSound:int");
ScriptRegistrant<JMG_Bear_Hunter_Wolf> JMG_Bear_Hunter_Wolf_Registrant("JMG_Bear_Hunter_Wolf","WolfPackID:int,ControllerId:int");
ScriptRegistrant<JMG_Bear_Hunter_Wolf_Wander_Point> JMG_Bear_Hunter_Wolf_Wander_Point_Registrant("JMG_Bear_Hunter_Wolf_Wander_Point","WolfPackID:int");
ScriptRegistrant<JMG_Bear_Hunter_Armored_Car> JMG_Bear_Hunter_Armored_Car_Registrant("JMG_Bear_Hunter_Armored_Car","");
ScriptRegistrant<JMG_Bear_Hunt_Final_Boss_Support> JMG_Bear_Hunt_Final_Boss_Support_Registrant("JMG_Bear_Hunt_Final_Boss_Support","");