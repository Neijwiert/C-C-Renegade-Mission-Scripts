/*
* A Command & Conquer: Renegade SSGM Plugin, containing all the single player mission scripts
* Copyright(C) 2017  Neijwiert
*
* This program is free software : you can redistribute it and / or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.If not, see <http://www.gnu.org/licenses/>.
*/

#include "General.h"
#include "M01_Mission_Controller_JDG.h"

/*
M01 -> 100376
*/
void M01_Mission_Controller_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->firstTimeHONEntered, sizeof(this->firstTimeHONEntered), 2);
	Auto_Save_Variable(&this->firstTimeHONGruntZoneEntered, sizeof(this->firstTimeHONGruntZoneEntered), 3);
	Auto_Save_Variable(&this->firstTimeHONDojoZoneEntered, sizeof(this->firstTimeHONDojoZoneEntered), 4);
	Auto_Save_Variable(&this->firstTimePrisonerPenSwitchPoked, sizeof(this->firstTimePrisonerPenSwitchPoked), 5);
	Auto_Save_Variable(&this->firstTimeRadarScrambled, sizeof(this->firstTimeRadarScrambled), 6);
	Auto_Save_Variable(&this->firstTimeStarLeavingGDIBase, sizeof(this->firstTimeStarLeavingGDIBase), 7);
	Auto_Save_Variable(&this->prisonerGateLowered, sizeof(this->prisonerGateLowered), 8);
	Auto_Save_Variable(&this->churchGuardMiniGunnerKilled, sizeof(this->churchGuardMiniGunnerKilled), 9);
	Auto_Save_Variable(&this->churchLoveShackMiniGunnerKilled, sizeof(this->churchLoveShackMiniGunnerKilled), 10);
	Auto_Save_Variable(&this->doingChinookReinforcementsPokedConversation, sizeof(this->doingChinookReinforcementsPokedConversation), 11);
	Auto_Save_Variable(&this->failedChurchEvac, sizeof(this->failedChurchEvac), 12);
	Auto_Save_Variable(&this->startedChurchEvac, sizeof(this->startedChurchEvac), 13);
	Auto_Save_Variable(&this->commKilled, sizeof(this->commKilled), 14);
	Auto_Save_Variable(&this->radarScrambled, sizeof(this->radarScrambled), 15);
	Auto_Save_Variable(&this->announcedPrisonerObjective, sizeof(this->announcedPrisonerObjective), 16);
	Auto_Save_Variable(&this->commSAMSiteObjectiveActive, sizeof(this->commSAMSiteObjectiveActive), 17);
	Auto_Save_Variable(&this->pokedPrisonPenGateSwitch, sizeof(this->pokedPrisonPenGateSwitch), 18);
	Auto_Save_Variable(&this->honEasySpawnedGuy1Alive, sizeof(this->honEasySpawnedGuy1Alive), 19);
	Auto_Save_Variable(&this->honEasySpawnedGuy2Alive, sizeof(this->honEasySpawnedGuy2Alive), 20);
	Auto_Save_Variable(&this->honEasySpawnedGuy3Alive, sizeof(this->honEasySpawnedGuy3Alive), 21);
	Auto_Save_Variable(&this->beachNodTurret1Destroyed, sizeof(this->beachNodTurret1Destroyed), 22);
	Auto_Save_Variable(&this->beachNodTurret2Destroyed, sizeof(this->beachNodTurret2Destroyed), 23);
	Auto_Save_Variable(&this->allBeachTurretsDestroyed, sizeof(this->allBeachTurretsDestroyed), 24);
	Auto_Save_Variable(&this->field_34, sizeof(this->field_34), 25);
	Auto_Save_Variable(&this->field_35, sizeof(this->field_35), 26);
	Auto_Save_Variable(&this->field_36, sizeof(this->field_36), 27);
	Auto_Save_Variable(&this->field_37, sizeof(this->field_37), 28);
	Auto_Save_Variable(&this->kaneConversationActive, sizeof(this->kaneConversationActive), 29);
	Auto_Save_Variable(&this->field_39, sizeof(this->field_39), 30);
	Auto_Save_Variable(&this->commBaseCommanderKilled, sizeof(this->commBaseCommanderKilled), 31);
	Auto_Save_Variable(&this->starInChurchArea, sizeof(this->starInChurchArea), 32);
	Auto_Save_Variable(&this->honWeatherSet, sizeof(this->honWeatherSet), 33);
	Auto_Save_Variable(&this->gdiBaseEncounterInactive, sizeof(this->gdiBaseEncounterInactive), 34);
	Auto_Save_Variable(&this->field_3E, sizeof(this->field_3E), 35);
	Auto_Save_Variable(&this->field_3F, sizeof(this->field_3F), 36);
	Auto_Save_Variable(&this->barnCivsEvacuated, sizeof(this->barnCivsEvacuated), 37);
	Auto_Save_Variable(&this->gunboatDestroyed, sizeof(this->gunboatDestroyed), 38);
	Auto_Save_Variable(&this->destroyedDetentionPenCivs, sizeof(this->destroyedDetentionPenCivs), 39);
	Auto_Save_Variable(&this->field_44, sizeof(this->field_44), 40);
	Auto_Save_Variable(&this->whackAMoleNodMinigunnerObjId, sizeof(this->whackAMoleNodMinigunnerObjId), 41);
	Auto_Save_Variable(&this->nodHonSpawnedSoldier1ObjId, sizeof(this->nodHonSpawnedSoldier1ObjId), 42);
	Auto_Save_Variable(&this->nodHonSpawnedSoldier2ObjId, sizeof(this->nodHonSpawnedSoldier2ObjId), 43);
	Auto_Save_Variable(&this->nodHonSpawnedSoldier3ObjId, sizeof(this->nodHonSpawnedSoldier3ObjId), 44);
	Auto_Save_Variable(&this->field_58, sizeof(this->field_58), 45);
	Auto_Save_Variable(&this->honEasySpawnedGuyCount, sizeof(this->honEasySpawnedGuyCount), 46);
	Auto_Save_Variable(&this->honEasySpawnedGuyCount2, sizeof(this->honEasySpawnedGuyCount2), 47);
	Auto_Save_Variable(&this->honEasySpawnedGuySpawnInterval, sizeof(this->honEasySpawnedGuySpawnInterval), 48);
	Auto_Save_Variable(&this->field_68, sizeof(this->field_68), 49);
	Auto_Save_Variable(&this->kaneHologramObjId, sizeof(this->kaneHologramObjId), 50);
	Auto_Save_Variable(&this->_00_N000ESoundId, sizeof(this->_00_N000ESoundId), 51);
	Auto_Save_Variable(&this->_00_N002ESoundId, sizeof(this->_00_N002ESoundId), 52);
	Auto_Save_Variable(&this->_00_N026ESoundId, sizeof(this->_00_N026ESoundId), 53);
	Auto_Save_Variable(&this->_00_N028ESoundId, sizeof(this->_00_N028ESoundId), 54);
	Auto_Save_Variable(&this->_00_N030ESoundId, sizeof(this->_00_N030ESoundId), 55);
	Auto_Save_Variable(&this->billysConversationConversationId, sizeof(this->billysConversationConversationId), 56);
	Auto_Save_Variable(&this->babushkasConversationConversationId, sizeof(this->babushkasConversationConversationId), 57);
	Auto_Save_Variable(&this->field_8C, sizeof(this->field_8C), 58);
	Auto_Save_Variable(&this->field_90, sizeof(this->field_90), 59);
	Auto_Save_Variable(&this->kaneAndNumber2Conversation1ConversationId, sizeof(this->kaneAndNumber2Conversation1ConversationId), 60);
	Auto_Save_Variable(&this->field_98, sizeof(this->field_98), 61);
	Auto_Save_Variable(&this->kaneAndHavocConversationConversationId, sizeof(this->kaneAndHavocConversationConversationId), 62);
	Auto_Save_Variable(&this->field_A0, sizeof(this->field_A0), 63);
	Auto_Save_Variable(&this->field_A4, sizeof(this->field_A4), 64);
	Auto_Save_Variable(&this->field_A8, sizeof(this->field_A8), 65);
	Auto_Save_Variable(&this->field_AC, sizeof(this->field_AC), 66);
	Auto_Save_Variable(&this->civ3ConversationConversationId, sizeof(this->civ3ConversationConversationId), 67);
	Auto_Save_Variable(&this->field_B4, sizeof(this->field_B4), 68);
	Auto_Save_Variable(&this->field_B8, sizeof(this->field_B8), 69);
	Auto_Save_Variable(&this->removeUnlockGateObjectiveConversationId, sizeof(this->removeUnlockGateObjectiveConversationId), 70);
	Auto_Save_Variable(&this->addTurretsObjectiveConversationId, sizeof(this->addTurretsObjectiveConversationId), 71);
	Auto_Save_Variable(&this->chinookSpawnedSoldierGDIObjId, sizeof(this->chinookSpawnedSoldierGDIObjId), 72);
	Auto_Save_Variable(&this->lastDamagedStarEnemyObjId, sizeof(this->lastDamagedStarEnemyObjId), 73);
	Auto_Save_Variable(&this->honTroopDropCount, sizeof(this->honTroopDropCount), 74);
	Auto_Save_Variable(&this->barnGuardDeathCount, sizeof(this->barnGuardDeathCount), 75);
	Auto_Save_Variable(&this->field_D4, sizeof(this->field_D4), 76);
	Auto_Save_Variable(&this->honKilled, sizeof(this->honKilled), 77);
	Auto_Save_Variable(&this->buildingKillStatus, sizeof(this->buildingKillStatus), 78);
	Auto_Save_Variable(&this->starAreaNumber, sizeof(this->starAreaNumber), 79);
	Auto_Save_Variable(&this->field_D8, sizeof(this->field_D8), 80);
	Auto_Save_Variable(&this->lockeChurchReminderConversationConversationId, sizeof(this->lockeChurchReminderConversationConversationId), 81);
	Auto_Save_Variable(&this->remindedStarOfChurchCivs, sizeof(this->remindedStarOfChurchCivs), 82);
	Auto_Save_Variable(&this->priestReminderConversationConversationId, sizeof(this->priestReminderConversationConversationId), 83);
	Auto_Save_Variable(&this->remindedStarOfMoreChurchCivs, sizeof(this->remindedStarOfMoreChurchCivs), 84);
	Auto_Save_Variable(&this->interiorNunConversationConversationId, sizeof(this->interiorNunConversationConversationId), 85);
	Auto_Save_Variable(&this->remindedStarOfMoreChurchCivs2, sizeof(this->remindedStarOfMoreChurchCivs2), 86);
	Auto_Save_Variable(&this->starEnteredHONGruntZone, sizeof(this->starEnteredHONGruntZone), 87);
	Auto_Save_Variable(&this->starInBarn, sizeof(this->starInBarn), 88);
	Auto_Save_Variable(&this->commSAMSiteDestroyed, sizeof(this->commSAMSiteDestroyed), 89);
}

void M01_Mission_Controller_JDG::Created(GameObject *obj)
{
	Commands->Enable_Hibernation(obj, false);

	this->honKilled = 0;
	this->buildingKillStatus = 0;
	this->starAreaNumber = 0;
	this->field_1C = true;
	this->firstTimeHONEntered = true;
	this->firstTimeHONGruntZoneEntered = true;
	this->firstTimeHONDojoZoneEntered = true;
	this->firstTimePrisonerPenSwitchPoked = true;
	this->firstTimeRadarScrambled = true;
	this->firstTimeStarLeavingGDIBase = true;
	this->gdiBaseEncounterInactive = true;
	this->churchGuardMiniGunnerKilled = false;
	this->churchLoveShackMiniGunnerKilled = false;
	this->doingChinookReinforcementsPokedConversation = false;
	this->startedChurchEvac = false;
	this->failedChurchEvac = false;
	this->commKilled = false;
	this->radarScrambled = false;
	this->announcedPrisonerObjective = false;
	this->commSAMSiteObjectiveActive = false;
	this->pokedPrisonPenGateSwitch = false;
	this->honEasySpawnedGuy1Alive = false;
	this->honEasySpawnedGuy2Alive = false;
	this->honEasySpawnedGuy3Alive = false;
	this->beachNodTurret1Destroyed = false;
	this->beachNodTurret2Destroyed = false;
	this->allBeachTurretsDestroyed = false;
	this->field_34 = false;
	this->field_35 = false;
	this->field_36 = false;
	this->field_37 = false;
	this->kaneConversationActive = false;
	this->field_39 = false;
	this->commBaseCommanderKilled = false;
	this->starInChurchArea = false;
	this->honWeatherSet = false;
	this->field_3E = false;
	this->field_3F = false;
	this->barnCivsEvacuated = false;
	this->gunboatDestroyed = false;
	this->destroyedDetentionPenCivs = false;
	this->prisonerGateLowered = false;
	this->field_D8 = false;
	this->remindedStarOfChurchCivs = false;
	this->remindedStarOfMoreChurchCivs = false;
	this->remindedStarOfMoreChurchCivs2 = false;
	this->starEnteredHONGruntZone = false;
	this->starInBarn = false;
	this->commSAMSiteDestroyed = false;
	this->honTroopDropCount = 0;
	this->honEasySpawnedGuyCount = 0;
	this->honEasySpawnedGuyCount2 = 0;
	this->field_68 = 0;
	this->barnGuardDeathCount = 0;
	this->field_D4 = 0;
	this->honEasySpawnedGuySpawnInterval = 14.0f;
	this->chinookSpawnedSoldierGDIObjId = 0;
	this->lastDamagedStarEnemyObjId = 0;

	Commands->Set_Wind(0.0f, 0.0f, 0.0f, 0.0f);
	Commands->Set_Rain(0.0f, 0.0f, true);
	Commands->Set_Lightning(0.25f, 0.75f, 1.0f, 0.0f, 0.1f, 0.0f);

	Commands->Reveal_Encyclopedia_Character(19); // GDI_MiniGunner_0
	Commands->Reveal_Encyclopedia_Character(21); // GDI_MiniGunner_1Off
	Commands->Reveal_Encyclopedia_Character(14); // GDI_Grenadier_0
	Commands->Reveal_Encyclopedia_Character(38); // Nod_Minigunner_0
	Commands->Reveal_Encyclopedia_Character(18); // GDI_Female_Lieutenant
	Commands->Reveal_Encyclopedia_Character(16); // GDI_Brigadier_Locke
	Commands->Reveal_Encyclopedia_Character(39); // Nod_Minigunner_3Boss
	Commands->Reveal_Encyclopedia_Character(13); // GDI_Engineer_2SF
	Commands->Reveal_Encyclopedia_Character(15); // GDI_Engineer_2SF
	Commands->Reveal_Encyclopedia_Character(22); // GDI_Engineer_2SF
	Commands->Reveal_Encyclopedia_Character(26); // GDI_RocketSoldier_2SF
	Commands->Reveal_Encyclopedia_Character(24); // GDI_RocketSoldier_2SF
	Commands->Reveal_Encyclopedia_Character(25); // GDI_MiniGunner_1Off
	Commands->Reveal_Encyclopedia_Character(23); // GDI_Prisoners

	Commands->Reveal_Encyclopedia_Weapon(14); // POW_Pistol_Player
	Commands->Reveal_Encyclopedia_Weapon(20); // POW_Pistol_Player
	Commands->Reveal_Encyclopedia_Weapon(5); // POW_Pistol_Player

	Commands->Reveal_Encyclopedia_Vehicle(18); // Nod_SAM_Site and CnC_GDI_Orca
	Commands->Reveal_Encyclopedia_Vehicle(19); // CnC_GDI_Transport

	Commands->Reveal_Encyclopedia_Building(4); // SP_Power_Plant_GDI
	Commands->Reveal_Encyclopedia_Building(2); // SP_Con_Yard_GDI (Yes, conyard)
}

void M01_Mission_Controller_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_SOUND_ENDED)
	{
		if (param == this->_00_N000ESoundId)
		{
			this->_00_N002ESoundId = Commands->Create_Sound("00-N002E", Vector3(0.0f, 0.0f, 0.0f), obj);;

			Commands->Monitor_Sound(obj, this->_00_N002ESoundId);
		}
		else if (param == this->_00_N002ESoundId)
		{
			this->_00_N026ESoundId = Commands->Create_Sound("00-N026E", Vector3(0.0f, 0.0f, 0.0f), obj);

			Commands->Monitor_Sound(obj, this->_00_N026ESoundId);
		}
		else if (param == this->_00_N026ESoundId)
		{
			this->_00_N028ESoundId = Commands->Create_Sound("00-N028E", Vector3(0.0f, 0.0f, 0.0f), obj);

			Commands->Monitor_Sound(obj, this->_00_N028ESoundId);
		}
		else if (param == this->_00_N028ESoundId)
		{
			this->_00_N030ESoundId = Commands->Create_Sound("00-N030E", Vector3(0.0f, 0.0f, 0.0f), obj);

			Commands->Monitor_Sound(obj, this->_00_N030ESoundId);
		}
		else if (param == this->_00_N030ESoundId)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 99, 0.0f);

			this->commKilled = true;
		}
	}
	else  if(!type)
	{
		// Received from M01_Hand_of_Nod_Building_Script_JDG when killed
		if (param == 122)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 98, 2.0f);

			float rand = Commands->Get_Random(0.5f, 2.5f);
			if (rand < 0.5f || rand >= 1.5f)
			{
				Commands->Create_Sound("EVA_Nod_Structure_Destroyed", Vector3(0.0f, 0.0f, 0.0f), obj);
			}
			else
			{
				Commands->Create_Sound("EVA_Enemy_Structure_Destroyed", Vector3(0.0f, 0.0f, 0.0f), obj);
			}

			this->honKilled = 3;

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			Commands->Give_Points(starObj, 5000.0f, false);

			GameObject *honCafeteriaEatingDudeObj = Commands->Find_Object(101293);
			if (honCafeteriaEatingDudeObj)
			{
				Commands->Send_Custom_Event(obj, honCafeteriaEatingDudeObj, 0, 165, 0.0f);
			}

			GameObject *honCafeteriaWalkingGuyObj = Commands->Find_Object(101294);
			if (honCafeteriaWalkingGuyObj)
			{
				Commands->Send_Custom_Event(obj, honCafeteriaWalkingGuyObj, 0, 165, 0.0f);
			}

			GameObject *nonExistingObj = Commands->Find_Object(101295);
			if (nonExistingObj)
			{
				Commands->Send_Custom_Event(obj, nonExistingObj, 0, 165, 0.0f);
			}

			GameObject *honDormRoomMinigunnerObj = Commands->Find_Object(101296);
			if (honDormRoomMinigunnerObj)
			{
				Commands->Send_Custom_Event(obj, honDormRoomMinigunnerObj, 0, 165, 0.0f);
			}

			GameObject *honDormRoomRocketGuyObj = Commands->Find_Object(101297);
			if (honDormRoomRocketGuyObj)
			{
				Commands->Send_Custom_Event(obj, honDormRoomRocketGuyObj, 0, 165, 0.0f);
			}

			GameObject *honDormRoomFlamethrowerObj = Commands->Find_Object(101298);
			if (honDormRoomFlamethrowerObj)
			{
				Commands->Send_Custom_Event(obj, honDormRoomFlamethrowerObj, 0, 165, 0.0f);
			}

			GameObject *honDormRoomFlameGuyObj = Commands->Find_Object(101299);
			if (honDormRoomFlameGuyObj)
			{
				Commands->Send_Custom_Event(obj, honDormRoomFlameGuyObj, 0, 165, 0.0f);
			}

			GameObject *honDormRoomCrapperObj = Commands->Find_Object(101301);
			if (honDormRoomCrapperObj)
			{
				Commands->Send_Custom_Event(obj, honDormRoomCrapperObj, 0, 165, 0.0f);
			}

			GameObject *dojoSenseiObj = Commands->Find_Object(102051);
			if (dojoSenseiObj)
			{
				Commands->Send_Custom_Event(obj, dojoSenseiObj, 0, 165, 0.0f);
			}

			GameObject *dojoCiv01Obj = Commands->Find_Object(102048);
			if (dojoCiv01Obj)
			{
				Commands->Send_Custom_Event(obj, dojoCiv01Obj, 0, 165, 0.0f);
			}

			GameObject *dojoCiv02Obj = Commands->Find_Object(102049);
			if (dojoCiv02Obj)
			{
				Commands->Send_Custom_Event(obj, dojoCiv02Obj, 0, 165, 0.0f);
			}

			GameObject *dojoCiv03Obj = Commands->Find_Object(102050);
			if (dojoCiv03Obj)
			{
				Commands->Send_Custom_Event(obj, dojoCiv03Obj, 0, 165, 0.0f);
			}

			if (this->starAreaNumber != 5 && !this->starEnteredHONGruntZone)
			{
				GameObject *M01DestroyedBuilding1Soldier1Obj = Commands->Trigger_Spawner(100712);
				Commands->Attach_Script(M01DestroyedBuilding1Soldier1Obj, "M01_HON_FrontDoor_Evacuator_JDG", "");

				GameObject *M01DestroyedBuilding1Soldier2Obj = Commands->Trigger_Spawner(100712);
				Commands->Attach_Script(M01DestroyedBuilding1Soldier2Obj, "M01_HON_FrontDoor_Evacuator_JDG", "");

				GameObject *M01DestroyedBuilding1Soldier3Obj = Commands->Trigger_Spawner(100712);
				Commands->Attach_Script(M01DestroyedBuilding1Soldier3Obj, "M01_HON_FrontDoor_Evacuator_JDG", "");

				GameObject *M01DestroyedBuilding2Soldier1Obj = Commands->Trigger_Spawner(100713);
				Commands->Attach_Script(M01DestroyedBuilding2Soldier1Obj, "M01_HON_BackDoor_Evacuator_JDG", "");

				GameObject *M01DestroyedBuilding2Soldier2Obj = Commands->Trigger_Spawner(100713);
				Commands->Attach_Script(M01DestroyedBuilding2Soldier2Obj, "M01_HON_BackDoor_Evacuator_JDG", "");

				GameObject *M01DestroyedBuilding2Soldier3Obj = Commands->Trigger_Spawner(100713);
				Commands->Attach_Script(M01DestroyedBuilding2Soldier3Obj, "M01_HON_BackDoor_Evacuator_JDG", "");
			}
		}

		// Received from M01_Comm_Center_Building_Script_JDG when killed
		else if (param == 124)
		{
			if (this->starAreaNumber != 9)
			{
				GameObject *M01DestroyedBuilding3Soldier1Obj = Commands->Trigger_Spawner(100727);
				Commands->Attach_Script(M01DestroyedBuilding3Soldier1Obj, "M01_CommCenter_Evacuator_JDG", "");

				GameObject *M01DestroyedBuilding3Soldier2Obj = Commands->Trigger_Spawner(100727);
				Commands->Attach_Script(M01DestroyedBuilding3Soldier2Obj, "M01_CommCenter_Evacuator_JDG", "");

				GameObject *M01DestroyedBuilding3Soldier3Obj = Commands->Trigger_Spawner(100727);
				Commands->Attach_Script(M01DestroyedBuilding3Soldier3Obj, "M01_CommCenter_Evacuator_JDG", "");

				GameObject *M01DestroyedBuilding3Soldier4Obj = Commands->Trigger_Spawner(100727);
				Commands->Attach_Script(M01DestroyedBuilding3Soldier4Obj, "M01_CommCenter_Evacuator_JDG", "");

				GameObject *M01DestroyedBuilding3Soldier5Obj = Commands->Trigger_Spawner(100727);
				Commands->Attach_Script(M01DestroyedBuilding3Soldier5Obj, "M01_CommCenter_Evacuator_JDG", "");
			}

			if (!this->commKilled)
			{
				Commands->Send_Custom_Event(obj, obj, 0, 99, 0.0f);

				this->commKilled = true;
			}

			if (this->radarScrambled)
			{
				Commands->Send_Custom_Event(obj, obj, 0, 202, 0.0f);
			}

			float rand = Commands->Get_Random(0.5f, 2.5f);
			if (rand >= 0.5f && rand < 1.5f)
			{
				Commands->Create_Sound("EVA_Enemy_Structure_Destroyed", Vector3(0.0f, 0.0f, 0.0f), obj);
			}
			else if (rand >= 1.5f && rand <= 2.5f)
			{
				Commands->Create_Sound("EVA_Nod_Structure_Destroyed", Vector3(0.0f, 0.0f, 0.0f), obj);
			}

			this->buildingKillStatus = 3;
		}

		// Received from M01_Hand_Of_Nod_Interior_Zone when entered
		else if (param == 135)
		{
			this->starAreaNumber = 5;

			GameObject *M01AmbientSoundControllerJDGObj = Commands->Find_Object(100253);
			if (M01AmbientSoundControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01AmbientSoundControllerJDGObj, 0, 159, 0.0f);
				Commands->Send_Custom_Event(obj, M01AmbientSoundControllerJDGObj, 0, 135, 0.0f);
			}

			if (this->firstTimeHONEntered)
			{
				this->firstTimeHONEntered = false;

				Commands->Send_Custom_Event(obj, obj, 0, 160, 0.0f);
			}
		}

		// Received from M01_Hand_Of_Nod_Exterior_Zone when entered
		else if (param == 138)
		{
			this->starAreaNumber = 4;

			GameObject *M01AmbientSoundControllerJDGObj = Commands->Find_Object(100253);
			Commands->Send_Custom_Event(obj, M01AmbientSoundControllerJDGObj, 0, 158, 0.0f);
		}

		// Received from M01_Comm_Center_Interior_Zone when entered
		else if (param == 141)
		{
			this->starAreaNumber = 9;
			
			GameObject *M01AmbientSoundControllerJDGObj = Commands->Find_Object(100253);
			if (M01AmbientSoundControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01AmbientSoundControllerJDGObj, 0, 159, 0.0f);
				Commands->Send_Custom_Event(obj, M01AmbientSoundControllerJDGObj, 0, 141, 0.0f);
			}

			GameObject *honHologramNodOfficerObj = Commands->Find_Object(101936);
			if (honHologramNodOfficerObj)
			{
				Commands->Send_Custom_Event(obj, honHologramNodOfficerObj, 0, 141, 0.0f);
			}

			GameObject *nonExistingObj = Commands->Find_Object(101938);
			if (nonExistingObj)
			{
				Commands->Send_Custom_Event(obj, nonExistingObj, 0, 141, 0.0f);
			}

			GameObject *commUpstairsStationaryTechObj = Commands->Find_Object(101168);
			if (commUpstairsStationaryTechObj)
			{
				Commands->Send_Custom_Event(obj, commUpstairsStationaryTechObj, 0, 141, 0.0f);
			}

			GameObject *commWarRoomTech01Obj = Commands->Find_Object(101170);
			if (commWarRoomTech01Obj)
			{
				Commands->Send_Custom_Event(obj, commWarRoomTech01Obj, 0, 141, 0.0f);
			}

			GameObject *commWarRoomTech02Obj = Commands->Find_Object(101171);
			if (commWarRoomTech02Obj)
			{
				Commands->Send_Custom_Event(obj, commWarRoomTech02Obj, 0, 141, 0.0f);
			}

			GameObject *commComputerRoomTechObj = Commands->Find_Object(101172);
			if (commComputerRoomTechObj)
			{
				Commands->Send_Custom_Event(obj, commComputerRoomTechObj, 0, 141, 0.0f);
			}

			GameObject *commUpstairsGuardObj = Commands->Find_Object(101946);
			if (commUpstairsGuardObj)
			{
				Commands->Send_Custom_Event(obj, commUpstairsGuardObj, 0, 141, 0.0f);
			}
		}

		// Received from M01_Comm_Center_Exterior_Zone when entered
		else if (param == 142)
		{
			this->starAreaNumber = 8;

			GameObject *M01AmbientSoundControllerJDGObj = Commands->Find_Object(100253);
			Commands->Send_Custom_Event(obj, M01AmbientSoundControllerJDGObj, 0, 158, 0.0f);
		}

		// Received from M01_Tunnel_Interior_Zone when entered
		else if (param == 143)
		{
			this->starAreaNumber = 11;

			GameObject *M01AmbientSoundControllerJDGObj = Commands->Find_Object(100253);
			Commands->Send_Custom_Event(obj, M01AmbientSoundControllerJDGObj, 0, 159, 0.0f);
		}

		// Received from M01_Tunnel_Exterior_Zone when entered
		else if (param == 144)
		{
			this->starAreaNumber = 10;

			GameObject *M01AmbientSoundControllerJDGObj = Commands->Find_Object(100253);
			Commands->Send_Custom_Event(obj, M01AmbientSoundControllerJDGObj, 0, 158, 0.0f);
		}

		// Received from M01_Waterfall_Interior_Zone when entered
		else if (param == 145)
		{
			this->starAreaNumber = 13;

			GameObject *M01AmbientSoundControllerJDGObj = Commands->Find_Object(100253);
			Commands->Send_Custom_Event(obj, M01AmbientSoundControllerJDGObj, 0, 159, 0.0f);
		}

		// Received from M01_Waterfall_Exterior_Zone when entered
		else if (param == 146)
		{
			this->starAreaNumber = 12;

			GameObject *M01AmbientSoundControllerJDGObj = Commands->Find_Object(100253);
			Commands->Send_Custom_Event(obj, M01AmbientSoundControllerJDGObj, 0, 158, 0.0f);
		}

		// Received from M01_Hand_Of_Nod_Grunt_Zone when entered
		else if (param == 136)
		{
			this->starEnteredHONGruntZone = true;
			if (this->honKilled != 3 && this->firstTimeHONGruntZoneEntered)
			{
				this->firstTimeHONGruntZoneEntered = false;

				GameObject *honCafeteriaEatingDudeObj = Commands->Find_Object(101293);
				if (honCafeteriaEatingDudeObj)
				{
					Commands->Send_Custom_Event(obj, honCafeteriaEatingDudeObj, 0, 16, 0.0f);
				}

				GameObject *honCafeteriaWalkingGuyObj = Commands->Find_Object(101294);
				if (honCafeteriaWalkingGuyObj)
				{
					Commands->Send_Custom_Event(obj, honCafeteriaWalkingGuyObj, 0, 16, 0.0f);
				}

				GameObject *nonExistingObj = Commands->Find_Object(101295);
				if (nonExistingObj)
				{
					Commands->Send_Custom_Event(obj, nonExistingObj, 0, 16, 0.0f);
				}

				GameObject *honDormRoomMinigunnerObj = Commands->Find_Object(101296);
				if (honDormRoomMinigunnerObj)
				{
					Commands->Send_Custom_Event(obj, honDormRoomMinigunnerObj, 0, 16, 0.0f);
				}

				GameObject *honDormRoomRocketGuyObj = Commands->Find_Object(101297);
				if (honDormRoomRocketGuyObj)
				{
					Commands->Send_Custom_Event(obj, honDormRoomRocketGuyObj, 0, 16, 0.0f);
				}

				GameObject *honDormRoomFlamethrowerObj = Commands->Find_Object(101298);
				if (honDormRoomFlamethrowerObj)
				{
					Commands->Send_Custom_Event(obj, honDormRoomFlamethrowerObj, 0, 16, 0.0f);
				}

				GameObject *honDormRoomFlameGuyObj = Commands->Find_Object(101299);
				if (honDormRoomFlameGuyObj)
				{
					Commands->Send_Custom_Event(obj, honDormRoomFlameGuyObj, 0, 16, 0.0f);
				}

				GameObject *honDormRoomCrapperObj = Commands->Find_Object(101301);
				if (honDormRoomCrapperObj)
				{
					Commands->Send_Custom_Event(obj, honDormRoomCrapperObj, 0, 16, 0.0f);
				}
			}
		}

		// Received from M01_Hand_Of_Nod_Dojo_Zone when entered
		else if (param == 137)
		{
			if (this->honKilled != 3 && this->firstTimeHONDojoZoneEntered)
			{
				this->firstTimeHONDojoZoneEntered = false;

				GameObject *dojoSenseiObj = Commands->Find_Object(102051);
				if (dojoSenseiObj)
				{
					Commands->Send_Custom_Event(obj, dojoSenseiObj, 0, 16, 0.0f);
				}
			}
		}

		// Received from M01_HandOfNod_SAMSite_Script when killed
		else if (param == 123)
		{
			GameObject *honBuildingObj = Commands->Find_Object(153909);
			if (Commands->Get_Health(honBuildingObj) > 0.0f)
			{
				Commands->Send_Custom_Event(obj, obj, 0, 167, 3.0f);

				float randDelay1 = Commands->Get_Random(0.0f, 2.0f);
				Commands->Send_Custom_Event(obj, obj, 0, 168, randDelay1);

				float randDelay2 = Commands->Get_Random(2.0f, 4.0f);
				Commands->Send_Custom_Event(obj, obj, 0, 169, randDelay2);
			}
		}

		// Received from M01_CommCenter_SAMSite_Script when killed
		else if (param == 125)
		{
			this->commSAMSiteDestroyed = true;

			if (this->commSAMSiteObjectiveActive)
			{
				Commands->Send_Custom_Event(obj, obj, 0, 119, 0.0f);
			}

			if (this->commKilled)
			{
				// Good job, Havoc - there's hope for you yet.
				// Push the button next to the gate to open the Detention Center.
				this->removeUnlockGateObjectiveConversationId = Commands->Create_Conversation("M01_Remove_Unlock_Gate_Objective", 100, 1000.0f, false);
				Commands->Join_Conversation(NULL, this->removeUnlockGateObjectiveConversationId, false, false, true);
				Commands->Start_Conversation(this->removeUnlockGateObjectiveConversationId, this->removeUnlockGateObjectiveConversationId);
				Commands->Monitor_Conversation(obj, this->removeUnlockGateObjectiveConversationId);
			}
			
			Commands->Send_Custom_Event(obj, obj, 0, 120, 10.0f);
		}

		// Received from M01_Announce_TankAirstrikeZone_JDG when entered
		else if (param == 166)
		{
			Commands->Create_Sound("00-N036E", Vector3(0.0f, 0.0f, 0.0f), obj);
		}

		// Received from ourselves after 3 seconds when param 123 or 172 is received
		else if (param == 167)
		{
			Commands->Create_Sound("00-N180E", Vector3(0.0f, 0.0f, 0.0f), obj);
		}

		// Received from ourselves after 10 seconds when param 125 is received
		else if (param == 120)
		{
			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-296.388f, 497.455f, 29.122f));
			Commands->Set_Facing(invisObj, -130.0f);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X1I_GDI_CommCenter_TroopDrop.txt");
		}

		// Received from ourselves after 0 to 2 seconds when param 123 or 172 is received
		else if (param == 168)
		{
			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-147.1f, 558.8f, 4.4000001f));
			Commands->Set_Facing(invisObj, -130.0f);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X1I_GDI_HON_TroopDrop01a.txt");
		}

		// Received from ourselves after 2 to 4 seconds seconds when param 123 or 172 is received
		else if (param == 169)
		{
			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-154.679f, 519.009f, 4.9699998f));
			Commands->Set_Facing(invisObj, 130.0f);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X1I_GDI_HON_TroopDrop01b.txt");
		}

		// Never received
		else if (param == 172)
		{
			if (++this->honTroopDropCount == 3)
			{
				Commands->Send_Custom_Event(obj, obj, 0, 167, 3.0f);

				float rand = Commands->Get_Random(0.5f, 2.5f);
				if (rand >= 0.5f && rand < 1.5f)
				{
					float randDelay = Commands->Get_Random(0.0f, 2.0f);
					Commands->Send_Custom_Event(obj, obj, 0, 168, randDelay);
				}
				else
				{
					float randDelay = Commands->Get_Random(0.0f, 2.0f);
					Commands->Send_Custom_Event(obj, obj, 0, 169, randDelay);
				}
			}
		}

		// Received from M01_Entering_Church_Area_Zone when entered
		else if (param == 148)
		{
			if (!this->starInChurchArea)
			{
				this->starInChurchArea = true;

				Commands->Set_Wind(60.0f, 3.0f, 0.5f, 15.0f);
				Commands->Set_Rain(0.75f, 30.0f, false);
				Commands->Set_Lightning(0.5f, 0.25f, 0.75f, 10.0f, 0.33000001f, 5.0f);

				GameObject *nodHarvesterObj = Commands->Create_Object("Nod_Harvester", Vector3(-98.937f, 380.721f, 20.021999f));
				Commands->Attach_Script(nodHarvesterObj, "M01_HarvesterScript_New_JDG", "");

				GameObject *tibFieldGuard1Obj = Commands->Find_Object(103270);
				if (tibFieldGuard1Obj)
				{
					Commands->Send_Custom_Event(obj, tibFieldGuard1Obj, 0, 16, 5.0f);
				}
			}

			int conversationId = Commands->Create_Conversation("M01_Add_Church_Objective", 100, 1000.0f, false); // There are civilians in a nearby church.  Expect resistance.
			Commands->Join_Conversation(NULL, conversationId, false, false, true);
			Commands->Start_Conversation(conversationId, 0);

			Commands->Send_Custom_Event(obj, obj, 0, 83, 8.0f);

			GameObject *loveShackMinigunnerObj = Commands->Find_Object(101305);
			if (loveShackMinigunnerObj)
			{
				Commands->Send_Custom_Event(obj, loveShackMinigunnerObj, 0, 16, 0.0f);
			}

			GameObject *loveShackNunObj = Commands->Find_Object(101310);
			if (loveShackNunObj)
			{
				Commands->Send_Custom_Event(obj, loveShackNunObj, 0, 16, 0.0f);
			}

			GameObject *churchExteriorGuardObj = Commands->Find_Object(101311);
			if (churchExteriorGuardObj)
			{
				Commands->Send_Custom_Event(obj, churchExteriorGuardObj, 0, 16, 0.0f);
			}

			GameObject *churchBalconyMinigunnerObj = Commands->Find_Object(101312);
			if (churchBalconyMinigunnerObj)
			{
				Commands->Send_Custom_Event(obj, churchBalconyMinigunnerObj, 0, 16, 0.0f);
			}

			GameObject *churchInteriorGuardObj = Commands->Find_Object(101313);
			if (churchInteriorGuardObj)
			{
				Commands->Send_Custom_Event(obj, churchInteriorGuardObj, 0, 16, 0.0f);
			}

			GameObject *churchInteriorNunObj = Commands->Find_Object(101314);
			if (churchInteriorNunObj)
			{
				Commands->Send_Custom_Event(obj, churchInteriorNunObj, 0, 16, 0.0f);
			}

			GameObject *churchInteriorPriestObj = Commands->Find_Object(101315);
			if (churchInteriorPriestObj)
			{
				Commands->Send_Custom_Event(obj, churchInteriorPriestObj, 0, 16, 0.0f);
			}
		}

		// Received from ourselves after 8 seconds when param 148 is received
		else if (param == 83)
		{
			// Rescue Clergy
			// The local clergy are being held captive inside their own church. Eliminate the Nod forces in the area and escort the clergy to the air evacuation point outside.
			Commands->Add_Objective(100, OBJECTIVE_TYPE_SECONDARY, OBJECTIVE_STATUS_PENDING, 6272, NULL, 6139);

			GameObject *M01ObjectivePogControllerJDGObj = Commands->Find_Object(105828);
			if (M01ObjectivePogControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01ObjectivePogControllerJDGObj, 6, 100, 0.0f);
			}

			GameObject *loveShackNunObj = Commands->Find_Object(101310);
			if (loveShackNunObj)
			{
				Commands->Set_Obj_Radar_Blip_Shape(loveShackNunObj, RADAR_BLIP_SHAPE_OBJECTIVE);
				Commands->Set_Obj_Radar_Blip_Color(loveShackNunObj, RADAR_BLIP_COLOR_SECONDARY_OBJECTIVE);
			}

			GameObject *churchInteriorNunObj = Commands->Find_Object(101314);
			if (churchInteriorNunObj)
			{
				Commands->Set_Obj_Radar_Blip_Shape(churchInteriorNunObj, RADAR_BLIP_SHAPE_OBJECTIVE);
				Commands->Set_Obj_Radar_Blip_Color(churchInteriorNunObj, RADAR_BLIP_COLOR_SECONDARY_OBJECTIVE);
			}

			GameObject *churchInteriorPriestObj = Commands->Find_Object(101315);
			if (churchInteriorPriestObj)
			{
				Commands->Set_Obj_Radar_Blip_Shape(churchInteriorPriestObj, RADAR_BLIP_SHAPE_OBJECTIVE);
				Commands->Set_Obj_Radar_Blip_Color(churchInteriorPriestObj, RADAR_BLIP_COLOR_SECONDARY_OBJECTIVE);
			}
		}

		// Received from M01_GDI_GuardTower_NOD_Commander_JDG after 5 seconds when killed
		// Received from ourselves after 3 seconds when param 93 is received
		else if (param == 101)
		{
			Commands->Set_Objective_Status(104, OBJECTIVE_STATUS_ACCOMPLISHED);

			this->field_D8 = true;
		}

		// Received by ourselves after 2 seconds when custom type <= 0 and param = 122 is received
		else if (param == 98)
		{
			Commands->Set_Objective_Status(103, OBJECTIVE_STATUS_ACCOMPLISHED);
		}

		// Received from ourselves after 2 seconds when param 177 is received
		else if (param == 94)
		{
			Commands->Set_Objective_Status(100, OBJECTIVE_STATUS_ACCOMPLISHED);
		}

		// Received from M01_Church_EvacController_JDG when param 229 is received
		else if (param == 95)
		{
			Commands->Set_Objective_Status(100, OBJECTIVE_STATUS_FAILED);
		}

		// Received from M01_Church_LoveShack_MiniGunner_JDG when killed
		else if (param == 174)
		{
			this->churchLoveShackMiniGunnerKilled = true;

			GameObject *loveShackNunObj = Commands->Find_Object(101310);
			if (loveShackNunObj)
			{
				float randDelay = Commands->Get_Random(0.0f, 2.0f);
				Commands->Send_Custom_Event(obj, loveShackNunObj, 0, 174, randDelay);
			}

			if (this->churchLoveShackMiniGunnerKilled && this->churchGuardMiniGunnerKilled && !this->startedChurchEvac)
			{
				GameObject *M01ChurchEvacControllerJDGObj = Commands->Find_Object(103394);
				if (M01ChurchEvacControllerJDGObj)
				{
					Commands->Send_Custom_Event(obj, M01ChurchEvacControllerJDGObj, 0, 16, 3.0f);
				}

				this->startedChurchEvac = true;
			}
		}

		// Received from M01_Church_Guard_MiniGunner_JDG when killed
		else if (param == 173)
		{
			this->churchGuardMiniGunnerKilled = true;

			GameObject *churchInteriorNunObj = Commands->Find_Object(101314);
			if (churchInteriorNunObj)
			{
				float randDelay = Commands->Get_Random(0.0, 2.0);
				Commands->Send_Custom_Event(obj, churchInteriorNunObj, 0, 173, randDelay);
			}

			GameObject *churchInteriorPriestObj = Commands->Find_Object(101315);
			if (churchInteriorPriestObj)
			{
				float randDelay = Commands->Get_Random(0.0f, 2.0f);
				Commands->Send_Custom_Event(obj, churchInteriorPriestObj, 0, 173, randDelay);
			}

			if (this->churchLoveShackMiniGunnerKilled && this->churchGuardMiniGunnerKilled && !this->startedChurchEvac)
			{
				this->startedChurchEvac = true;

				GameObject *M01ChurchEvacControllerJDGObj = Commands->Find_Object(103394);
				if (M01ChurchEvacControllerJDGObj)
				{
					Commands->Send_Custom_Event(obj, M01ChurchEvacControllerJDGObj, 0, 16, 3.0f);
				}
			}
		}

		// Received from M01_Church_Interior_Nun_JDG or M01_Church_LoveShack_Nun_JDG or M01_Church_Priest_JDG after 1 second when action with id 4001 is complete
		// Received from ourselves after 2 seconds after this block
		else if (param == 177)
		{
			if (Commands->Find_Object(101310) || Commands->Find_Object(101314) || Commands->Find_Object(this->field_44) || Commands->Find_Object(101315)) // loveShackNunObj, churchInteriorNunObj, churchInteriorPriesObj
			{
				Commands->Send_Custom_Event(obj, obj, 0, 177, 2.0f);
			}
			else
			{
				if (!this->failedChurchEvac)
				{
					this->failedChurchEvac = true;

					Commands->Send_Custom_Event(obj, obj, 0, 94, 2.0f);
				}
			}
		}

		// Received from M01_Church_LoveShack_MiniGunner_JDG when killed
		// Received from M01_COMM_Commander_Guy when killed
		// Received from M01_HON_Commander_Guy when killed
		// Received from M01_HON_Dorm_Crapper_JDG when killed
		else if (param == 178)
		{
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);
			Vector3 senderObjPos = Commands->Get_Position(sender);

			senderObjPos.Z += 0.75f;

			if (Commands->Has_Key(starObj, 1))
			{
				if (Commands->Has_Key(starObj, 2))
				{
					if (Commands->Has_Key(starObj, 3))
					{
						Commands->Create_Object("POW_Health_100", senderObjPos);
					}
					else
					{
						Commands->Create_Object("Level_03_Keycard", senderObjPos);
					}
				}
				else
				{
					Commands->Create_Object("Level_02_Keycard", senderObjPos);
				}
			}
			else
			{
				GameObject *keyCardObj = Commands->Create_Object("Level_01_Keycard", senderObjPos);
				Commands->Attach_Script(keyCardObj, "M01_KeyCard01_Script_JDG", "");
			}
		}

		// Received from M01_CHURCH_Chinook_Spawned_Soldier01_GDI or M01_CHURCH_Chinook_Spawned_Soldier02_GDI when poked
		// Received from M01_COMM_Chinook_Spawned_Soldier_GDI when poked
		else if (param == 179)
		{
			int senderObjId = Commands->Get_ID(sender);
			if (this->doingChinookReinforcementsPokedConversation)
			{
				if (this->doingChinookReinforcementsPokedConversation && senderObjId == this->chinookSpawnedSoldierGDIObjId)
				{
					Commands->Send_Custom_Event(obj, sender, 0, 181, 0.0f);

					this->doingChinookReinforcementsPokedConversation = false;
				}
			}
			else
			{
				Commands->Send_Custom_Event(obj, sender, 0, 180, 0.0f);

				this->chinookSpawnedSoldierGDIObjId = senderObjId;
				this->doingChinookReinforcementsPokedConversation = true;
			}
		}

		// Received from M01_CHURCH_Chinook_Spawned_Soldier01_GDI or M01_CHURCH_Chinook_Spawned_Soldier02_GDI when killed
		// Received from M01_CHURCH_Chinook_Spawned_Soldier01_GDI or M01_CHURCH_Chinook_Spawned_Soldier02_GDI when param 149 or 150 is received
		// Received from M01_COMM_Chinook_Spawned_Soldier_GDI when killed
		else if (param == 182)
		{
			this->doingChinookReinforcementsPokedConversation = false;
		}

		// Received from M01_Barn_Point_Guard_01_JDG or M01_Barn_Point_Guard_02_JDG when damaged by star
		// Received from M01_CommCenter_Evacuator_JDG when damaged by star
		// Received from M01_CommCenter_SAMSite_Script when damaged by star
		// Received from M01_Flamethrower_Point_Guard_JDG when damaged by star
		// Received from M01_HandOfNod_SAMSite_Script when damaged by star
		// Received from M01_HON_BackDoor_Evacuator_JDG when damaged by star
		// Received from M01_HON_Cafeteria_Eating_Guy_JDG when damaged by star
		// Received from M01_HON_Cafeteria_Walking_Guy_JDG when damaged by star
		// Received from M01_HON_Dorm_ChemGuy_JDG when damaged by star
		// Received from M01_HON_Dorm_Crapper_JDG when damaged by star
		// Received from M01_HON_Dorm_FlameGuy_JDG when damaged by star
		// Received from M01_HON_Dorm_MiniGunner_JDG when damaged by star
		// Received from M01_HON_Dorm_RocketGuy_JDG when damaged by star
		// Received from M01_HON_Easy_Spawned_Guy_01_JDG when damaged by star
		// Received from M01_HON_Easy_Spawned_Guy_02_JDG when damaged by star
		// Received from M01_HON_Easy_Spawned_Guy_03_JDG when damaged by star
		// Received from M01_HON_FrontDoor_Evacuator_JDG when damaged by star
		else if (param == 189)
		{
			if (this->doingChinookReinforcementsPokedConversation)
			{
				this->lastDamagedStarEnemyObjId = Commands->Get_ID(sender);

				float randDelay = Commands->Get_Random(0.0f, 1.0f);
				GameObject *chinookSpawnedSoldierGDIObj = Commands->Find_Object(this->chinookSpawnedSoldierGDIObjId);
				Commands->Send_Custom_Event(obj, chinookSpawnedSoldierGDIObj, 189, this->lastDamagedStarEnemyObjId, randDelay);
			}
		}

		// Received from M01_Lose_Any_Church_Escorts_Zone when entered
		else if (param == 149)
		{
			if (this->doingChinookReinforcementsPokedConversation)
			{
				GameObject *chinookSpawnedSoldierGDIObj = Commands->Find_Object(this->chinookSpawnedSoldierGDIObjId);
				Commands->Send_Custom_Event(obj, chinookSpawnedSoldierGDIObj, 0, 149, 0.0f);
			}
		}

		// Received from M01_Lose_Any_Church_Escorts_Zone_02 when entered
		else if (param == 150)
		{
			if (this->doingChinookReinforcementsPokedConversation)
			{
				GameObject *chinookSpawnedSoldierGDIObj = Commands->Find_Object(this->chinookSpawnedSoldierGDIObjId);
				Commands->Send_Custom_Event(obj, chinookSpawnedSoldierGDIObj, 0, 150, 0.0f);
			}
		}

		// Received from M01_Lose_Any_HON_Escorts_Zone when entered
		else if (param == 151)
		{
			if (this->doingChinookReinforcementsPokedConversation)
			{
				GameObject *chinookSpawnedSoldierGDIObj = Commands->Find_Object(this->chinookSpawnedSoldierGDIObjId);
				Commands->Send_Custom_Event(obj, chinookSpawnedSoldierGDIObj, 0, 151, 0.0f);
			}
		}

		// Received from M01_Lose_Any_HON_Escorts_Zone_02 when entered
		else if (param == 152)
		{
			if (this->doingChinookReinforcementsPokedConversation)
			{
				GameObject *chinookSpawnedSoldierGDIObj = Commands->Find_Object(this->chinookSpawnedSoldierGDIObjId);
				Commands->Send_Custom_Event(obj, chinookSpawnedSoldierGDIObj, 0, 152, 0.0f);
			}
		}

		// Received when M01_Player_Is_Crossing_Bridge_Via_Cave_Zone or M01_Player_Is_Crossing_Bridge_Via_Church_Zone or M01_Player_Is_Crossing_Bridge_Zone M01_Player_is_Entering_GDI_Base_Zoneis entered
		else if (param == 153 || param == 154)
		{
			if (!this->starInChurchArea)
			{
				this->starInChurchArea = true;

				Commands->Set_Wind(60.0f, 3.0f, 0.5f, 15.0f);
				Commands->Set_Rain(0.75f, 30.0f, false);
				Commands->Set_Lightning(0.5f, 0.25f, 0.75f, 10.0f, 0.33000001f, 5.0f);

				GameObject *nodHarvesterObj = Commands->Create_Object("Nod_Harvester", Vector3(-98.937f, 380.721f, 20.021999f));
				Commands->Attach_Script(nodHarvesterObj, "M01_HarvesterScript_New_JDG", "");

				GameObject *tibFieldGuard1Obj = Commands->Find_Object(103270);
				if (tibFieldGuard1Obj)
				{
					Commands->Send_Custom_Event(obj, tibFieldGuard1Obj, 0, 16, 5.0f);
				}
			}

			Commands->Send_Custom_Event(obj, obj, 0, 74, 10.0f);

			GameObject *canyonTowerNodOfficerObj = Commands->Find_Object(102357);
			if (canyonTowerNodOfficerObj)
			{
				Commands->Send_Custom_Event(obj, canyonTowerNodOfficerObj, 0, 16, 0.0f);
			}
		}

		// Received from M01_Tiberium_Cave_Helicopter_JDG when entered
		else if (param == 195)
		{
			GameObject *nodApacheObj = Commands->Create_Object("NOD_Apache", Vector3(-40.708f, 375.387f, 30.275f));
			Commands->Set_Facing(nodApacheObj, -30.0f);
			Commands->Attach_Script(nodApacheObj, "M01_Tiberium_Cave_Helicopter_JDG", "");
		}

		// Received from M01_Announce_Hand_of_Nod_Zone when entered
		else if (param == 103)
		{
			if (!this->honWeatherSet)
			{
				this->honWeatherSet = true;

				Commands->Set_Wind(60.0f, 5.0f, 1.0f, 15.0f);
				Commands->Set_Rain(1.0f, 15.0f, false);
				Commands->Set_Lightning(0.5f, 0.0f, 0.25f, 0.0f, 1.0f, 15.0f);
			}

			// You're approaching the Hand of Nod - destroy it to hamper enemy reinforcements.
			// Destroy the SAM Site and I'll drop in a team to escort you to the Master Control Terminal.
			int conversationid = Commands->Create_Conversation("M01_Add_HON_Objective", 100, 1000.0f, false);
			Commands->Join_Conversation(NULL, conversationid, false, false, true);
			Commands->Start_Conversation(conversationid, 0);
			
			Commands->Send_Custom_Event(obj, obj, 0, 91, 10.0f);
		}

		// Received from ourselves after 10 seconds when param 103 is received
		else if (param == 91)
		{
			// Disable Hand of Nod
			// Disabling the Hand of Nod will dramatically reduce reinforcements. The building is easier to eliminate by destroying the Master Control Terminal inside.
			Commands->Add_Objective(103, OBJECTIVE_TYPE_SECONDARY, OBJECTIVE_STATUS_PENDING, 6274, NULL, 6141);
			Commands->Set_Objective_Radar_Blip(103, Vector3(-179.3f, 541.9f, 3.5179999f));

			GameObject *M01ObjectivePogControllerJDGObj = Commands->Find_Object(105828);
			if (M01ObjectivePogControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01ObjectivePogControllerJDGObj, 6, 103, 0.0f);
			}
		}

		// Received from M01_Comm_MCT_Placeholder_JDG when killed
		else if (param == 197)
		{
			if (this->buildingKillStatus != 3)
			{
				GameObject *nodCommCenterBuildingObj = Commands->Find_Object(153910);
				Commands->Apply_Damage(nodCommCenterBuildingObj, 100000000.0f, "BlamoKiller", obj);
			}
		}

		// Received from M01_HON_MCT_Placeholder_JDG when killed
		else if (param == 196)
		{
			if (this->honKilled != 3)
			{
				GameObject *nodHonBuildingObj = Commands->Find_Object(153909);
				Commands->Apply_Damage(nodHonBuildingObj, 100000000.0f, "BlamoKiller", obj);
			}
		}

		// Received from M01_Comm_Center_Player_Terminal_Zone when poked
		else if (param == 199)
		{
			if (!this->commKilled)
			{
				this->_00_N000ESoundId = Commands->Create_Sound("00-N000E", Vector3(0.0f, 0.0f, 0.0f), obj);
				Commands->Monitor_Sound(obj, this->_00_N000ESoundId);
			}
		}

		// Received when M01_Comm_Center_Pen_Gate is poked
		else if (param == 200)
		{
			if (this->commKilled)
			{
				if (!this->prisonerGateLowered)
				{
					this->prisonerGateLowered = true;

					Commands->Set_Objective_Status(113, OBJECTIVE_STATUS_ACCOMPLISHED);
					Commands->Static_Anim_Phys_Goto_Last_Frame(157984, 0); // Detention pen gate

					Commands->Create_Sound("Gate_Down_01", Vector3(-313.06f, 575.22f, 28.513f), obj);
					if (Commands->Find_Object(100034)) // Comm center sam site
					{
						Commands->Create_Logical_Sound(obj, 400004, Vector3(-315.383f, 572.805f, 27.622999f), 50.0f);

						Commands->Send_Custom_Event(obj, obj, 0, 107, 0.0f);
					}
					else
					{
						Commands->Create_Logical_Sound(obj, 400005, Vector3(-315.383f, 572.805f, 27.622999f), 50.0f);
					}

					if (!this->pokedPrisonPenGateSwitch)
					{
						Commands->Send_Custom_Event(obj, obj, 0, 100, 0.0f);

						this->pokedPrisonPenGateSwitch = true;
					}
				}
			}
			else if (this->firstTimePrisonerPenSwitchPoked)
			{
				this->firstTimePrisonerPenSwitchPoked = false;

				// Havoc, you should be near the detention center.
				// Use the mainframe in the Communications Center to unlock the detention gate.
				// Consider destroying the Nod Communications Center to hamper communications.
				int conversationid = Commands->Create_Conversation("M01_Add_Unlock_Gate_Objective", 100, 1000.0f, false);
				Commands->Join_Conversation(NULL, conversationid, false, false, true);
				Commands->Start_Conversation(conversationid, conversationid);
			}
			else
			{
				Commands->Create_Sound("00-N008E", Vector3(0.0f, 0.0f, 0.0f), obj);
			}
		}

		// Received by ourselves when param 200 is received
		// Received from ourselves when param 99 is received
		else if (param == 107)
		{
			// We can't extract until the SAM Site is destroyed.
			int conversationId = Commands->Create_Conversation("M01_Add_Comm_SAM_Objective", 100, 1000.0f, false);
			Commands->Join_Conversation(NULL, conversationId, false, false, true);
			Commands->Start_Conversation(conversationId, 0);

			Commands->Send_Custom_Event(obj, obj, 0, 89, 6.0f);

			this->commSAMSiteObjectiveActive = true;
		}

		// Received by ourselves after 6 seconds when param 107 is received
		else if (param == 89)
		{
			GameObject *commCenterNodSAMSiteObj = Commands->Find_Object(100034);
			if (commCenterNodSAMSiteObj)
			{
				// Disable SAM Site
				// Disable the SAM Site adjacent to the Nod Communications Center to allow for an air evacuation of the prisoners. Consider using C4 to destroy this emplacement.
				Commands->Add_Objective(108, OBJECTIVE_TYPE_PRIMARY, OBJECTIVE_STATUS_PENDING, 6220, NULL, 7613);
				Commands->Set_Objective_Radar_Blip_Object(108, commCenterNodSAMSiteObj);

				GameObject *M01ObjectivePogControllerJDGObj = Commands->Find_Object(105828);
				if (M01ObjectivePogControllerJDGObj)
				{
					Commands->Send_Custom_Event(obj, M01ObjectivePogControllerJDGObj, 6, 108, 0.0f);
				}
			}
		}
		else if (param == 106)
		{
			// Havoc, you should be near the detention center.
			// Use the mainframe in the Communications Center to unlock the detention gate.
			// Consider destroying the Nod Communications Center to hamper communications.
			int conversationId = Commands->Create_Conversation("M01_Add_Unlock_Gate_Objective", 100, 1000.0f, false);
			Commands->Join_Conversation(0, conversationId, false, false, true);
			Commands->Start_Conversation(conversationId, 0);
		}

		// Received from ourselves after 10 seconds when param 105 is received
		else if (param == 88)
		{
			// Access Communications Mainframe
			// The Nod Detention Center main gate is electronically locked by the Nod Communications Center. Hack the mainframe inside the Nod Communications Center to unlock the gate.
			Commands->Add_Objective(107, OBJECTIVE_TYPE_PRIMARY, OBJECTIVE_STATUS_PENDING, 6218, NULL, 6041);
			Commands->Set_Objective_HUD_Info_Position(107, 93.0, "POG_M01_1_02.tga", 7599, Vector3(-296.505f, 521.09f, 27.09f)); // Hack

			this->announcedPrisonerObjective = true;

			Commands->Set_HUD_Help_Text(6218, Vector3(0.196f, 0.882f, 0.196f)); // Access Communications Mainframe

			GameObject *M01CommCenterMainFramePokeObj = Commands->Find_Object(107809);
			if (M01CommCenterMainFramePokeObj)
			{
				Commands->Send_Custom_Event(obj, M01CommCenterMainFramePokeObj, 0, 108, 0.0f);
			}
		}

		// Received by ourselves when sound ended of field TODO
		// Received by ourselves when param 124 is received
		else if (param == 99)
		{
			if (this->commSAMSiteDestroyed)
			{
				// Good job, Havoc - there's hope for you yet.
				// Push the button next to the gate to open the Detention Center.
				this->removeUnlockGateObjectiveConversationId = Commands->Create_Conversation("M01_Remove_Unlock_Gate_Objective", 100, 1000.0f, false);
				Commands->Join_Conversation(NULL, this->removeUnlockGateObjectiveConversationId, false, false, true);
				Commands->Start_Conversation(this->removeUnlockGateObjectiveConversationId, this->removeUnlockGateObjectiveConversationId);
				Commands->Monitor_Conversation(obj, this->removeUnlockGateObjectiveConversationId);
			}
			else
			{
				Commands->Send_Custom_Event(obj, obj, 0, 107, 0.0f);
			}

			if (this->announcedPrisonerObjective)
			{
				Commands->Send_Custom_Event(obj, obj, 0, 118, 6.0f);
			}
		}

		// Received from ourselves after 6 seconds when param 99 is received
		else if (param == 118)
		{
			Commands->Set_Objective_Status(107, OBJECTIVE_STATUS_ACCOMPLISHED);
		}

		// Received from ourselves when param 125 is received
		else if (param == 119)
		{
			Commands->Set_Objective_Status(108, OBJECTIVE_STATUS_ACCOMPLISHED);

			this->commSAMSiteObjectiveActive = false;

			if (this->pokedPrisonPenGateSwitch)
			{
				Commands->Create_Logical_Sound(obj, 400005, Vector3(-315.383f, 572.805f, 27.622999f), 50.0f);

				Commands->Send_Custom_Event(obj, obj, 0, 4, 60.0f);
			}
		}

		// Received from M01_Scramble_Radar_Zone when entered
		else if (param == 201)
		{
			if (this->firstTimeRadarScrambled)
			{
				GameObject *M01PropagandaSoundsControllerJDGObj = Commands->Find_Object(103099);
				Commands->Send_Custom_Event(obj, M01PropagandaSoundsControllerJDGObj, 0, 16, 0.0f);

				this->firstTimeRadarScrambled = false;

				// Warning - localized radar jamming detected.
				// You're approaching the Nod Communications Center, it's scrambling your radar. Stay alert.
				int conversationId = Commands->Create_Conversation("M01_Radar_Scrambled_Conversation", 100, 1000.0f, false);
				Commands->Join_Conversation(NULL, conversationId, false, false, false);
				Commands->Join_Conversation(NULL, conversationId, false, false, false);
				Commands->Start_Conversation(conversationId, conversationId);
			}

			if (!this->radarScrambled && this->buildingKillStatus != 3)
			{
				this->radarScrambled = true;

				Commands->Enable_Radar(false);
			}
		}

		// Received from M01_UnScramble_Radar_Zone when entered
		// Received by ourselves when param 124 is received
		else if (param == 202)
		{
			if (this->radarScrambled)
			{
				Commands->Enable_Radar(true);

				this->radarScrambled = false;
			}
		}

		// Received from M01_Announce_Prisoner_Objective_Zone when entered
		else if (param == 105)
		{
			// Havoc, you should be near the detention center.
			// Use the mainframe in the Communications Center to unlock the detention gate.
			// Consider destroying the Nod Communications Center to hamper communications.
			int conversationId = Commands->Create_Conversation("M01_Add_Unlock_Gate_Objective", 100, 1000.0f, false);
			Commands->Join_Conversation(NULL, conversationId, false, false, true);
			Commands->Start_Conversation(conversationId, conversationId);

			Commands->Send_Custom_Event(obj, obj, 0, 88, 10.0f);

			Commands->Set_Objective_Status(106, OBJECTIVE_STATUS_ACCOMPLISHED);
		}

		// Received by ourselves when param 200 is received
		else if (param == 100)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 4, 0.0f);
		}

		// Received from M01_PrisonPen_Civilian_JDG or M01_PrisonPen_POW_JDG after 5 seconds when action id 49 or 42 is complete
		// Received from ourselves after 60 seconds when param 119 is received
		// Received from ourselves when param 100 is received
		else if (param == 4)
		{
			if (!this->commSAMSiteObjectiveActive && this->pokedPrisonPenGateSwitch)
			{
				Commands->Set_Wind(0.0f, 0.0f, 0.0f, 5.0f);
				Commands->Set_Rain(0.0f, 5.0f, false);
				Commands->Set_Lightning(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 5.0f);

				if (!this->destroyedDetentionPenCivs)
				{
					this->destroyedDetentionPenCivs = true;

					GameObject *detentionCiv1Obj = Commands->Find_Object(101929);
					GameObject *detentionCiv2Obj = Commands->Find_Object(101930);
					GameObject *detentionCiv3Obj = Commands->Find_Object(101931);
					GameObject *detentionSoldier1Obj = Commands->Find_Object(101926);
					GameObject *detentionSoldier2Obj = Commands->Find_Object(101927);
					GameObject *detentionSoldier3Obj = Commands->Find_Object(101928);

					if (detentionCiv1Obj)
					{
						Commands->Destroy_Object(detentionCiv1Obj);
					}

					if (detentionCiv2Obj)
					{
						Commands->Destroy_Object(detentionCiv2Obj);
					}

					if (detentionCiv3Obj)
					{
						Commands->Destroy_Object(detentionCiv3Obj);
					}

					if (detentionSoldier1Obj)
					{
						Commands->Destroy_Object(detentionSoldier1Obj);
					}

					if (detentionSoldier2Obj)
					{
						Commands->Destroy_Object(detentionSoldier2Obj);
					}

					if (detentionSoldier3Obj)
					{
						Commands->Destroy_Object(detentionSoldier3Obj);
					}

					Vector3 pos = Commands->Get_Position(obj);
					GameObject *starObj = Commands->Get_A_Star(pos);

					Commands->Set_Position(starObj, Vector3(51.023f, -35.918f, 0.24600001f));

					Commands->Set_Is_Rendered(obj, false);

					GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
					Commands->Attach_Script(invisObj, "Test_Cinematic", "X1Z_Finale.txt");
					
					Commands->Send_Custom_Event(obj, obj, 0, 5, 20.0f);
				}
			}
		}

		// Received from ourselves after 20 seconds when param 4 is received
		// Received from ourselves after 3 seconds when conversation ended
		else if (param == 5)
		{
			GameObject *M01PropagandaSoundsControllerJDGObj = Commands->Find_Object(103099);
			if (M01PropagandaSoundsControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01PropagandaSoundsControllerJDGObj, 0, 22, 0.0f);
			}

			Commands->Mission_Complete(true);
		}

		// Received after 618 cinematic frames in x1z_finale.txt
		else if (param == 0)
		{
			Commands->Mission_Complete(true);
		}

		// Received from M01_Whack_A_Mole_Enter_Zone_JDG when entered
		else if (param == 203)
		{
			GameObject *whackAMoleNodMinigunnerObj = Commands->Create_Object("Nod_MiniGunner_1Off", Vector3(-198.088f, 525.828f, 10.035f));
			Commands->Attach_Script(whackAMoleNodMinigunnerObj, "M01_Whack_A_Mole_Minigunner_JDG", "");

			this->whackAMoleNodMinigunnerObjId = Commands->Get_ID(whackAMoleNodMinigunnerObj);
		}

		// Received from M01_Whack_A_Mole_Exit_Zone_JDG when entered
		else if (param == 204)
		{
			GameObject *whackAMoleNodMinigunnerObj = Commands->Find_Object(this->whackAMoleNodMinigunnerObjId);
			if (whackAMoleNodMinigunnerObj)
			{
				Commands->Send_Custom_Event(obj, whackAMoleNodMinigunnerObj, 0, 204, 0.0f);
			}
		}

		// Received from M01_Turn_on_the_Hand_of_Nod_Zone_JDG when entered
		// Received from ourselves after this block
		else if (param == 205)
		{
			if (this->honKilled != 3)
			{
				this->honEasySpawnedGuySpawnInterval = 21.0f;

				if (this->honEasySpawnedGuyCount2 <= 6)
				{
					if (starAreaNumber != 5)
					{
						if (!this->honEasySpawnedGuy1Alive)
						{
							GameObject *nodHonSpawnedSoldier1Obj = Commands->Create_Object("Nod_MiniGunner_0", Vector3(-212.6f, 541.1f, 4.0f));
							Commands->Attach_Script(nodHonSpawnedSoldier1Obj, "M01_HON_Easy_Spawned_Guy_01_JDG", "");

							this->nodHonSpawnedSoldier1ObjId = Commands->Get_ID(nodHonSpawnedSoldier1Obj);
							this->honEasySpawnedGuy1Alive = true;
						}
						else if (!this->honEasySpawnedGuy2Alive)
						{
							GameObject *nodHonSpawnedSoldier2Obj = Commands->Create_Object("Nod_MiniGunner_0", Vector3(-217.2f, 540.2f, 4.0f));
							Commands->Attach_Script(nodHonSpawnedSoldier2Obj, "M01_HON_Easy_Spawned_Guy_02_JDG", "");

							this->nodHonSpawnedSoldier2ObjId = Commands->Get_ID(nodHonSpawnedSoldier2Obj);
							this->honEasySpawnedGuy2Alive = true;
						}
						else if (!this->honEasySpawnedGuy3Alive)
						{
							GameObject *nodHonSpawnedSoldier3Obj = Commands->Create_Object("Nod_FlameThrower_0", Vector3(-217.6f, 533.6f, 4.0f));
							Commands->Attach_Script(nodHonSpawnedSoldier3Obj, "M01_HON_Easy_Spawned_Guy_03_JDG", "");

							this->nodHonSpawnedSoldier3ObjId = Commands->Get_ID(nodHonSpawnedSoldier3Obj);
							this->honEasySpawnedGuy3Alive = true;
						}
					}

					this->honEasySpawnedGuyCount++;
					this->honEasySpawnedGuyCount2++;
				}

				GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
				Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 205, this->honEasySpawnedGuySpawnInterval);
			}
		}

		// Received from M01_HON_Easy_Spawned_Guy_03_JDG when action with id 38 is complete
		else if (param == 206)
		{
			if (this->honEasySpawnedGuyCount == 3)
			{
				GameObject *nodHonSpawnedSoldier1Obj = Commands->Find_Object(this->nodHonSpawnedSoldier1ObjId);
				if (nodHonSpawnedSoldier1Obj)
				{
					Commands->Send_Custom_Event(obj, nodHonSpawnedSoldier1Obj, 0, 65, 0.0f);
					this->honEasySpawnedGuy1Alive = false;
					this->nodHonSpawnedSoldier1ObjId = 0;
				}

				GameObject *nodHonSpawnedSoldier2Obj = Commands->Find_Object(this->nodHonSpawnedSoldier2ObjId);
				if (nodHonSpawnedSoldier2Obj)
				{
					Commands->Send_Custom_Event(obj, nodHonSpawnedSoldier2Obj, 0, 65, 0.0f);
					this->honEasySpawnedGuy2Alive = false;
					this->nodHonSpawnedSoldier2ObjId = 0;
				}

				GameObject *nodHonSpawnedSoldier3Obj = Commands->Find_Object(this->nodHonSpawnedSoldier3ObjId);
				if (nodHonSpawnedSoldier3Obj)
				{
					Commands->Send_Custom_Event(obj, nodHonSpawnedSoldier3Obj, 0, 65, 0.0);
					this->honEasySpawnedGuy3Alive = false;
					this->nodHonSpawnedSoldier3ObjId = 0;
				}

				this->honEasySpawnedGuyCount = 0;
			}
		}
		
		// Received from M01_HON_Easy_Spawned_Guy_01_JDG when killed
		else if (param == 208)
		{
			this->honEasySpawnedGuy1Alive = false;
			this->honEasySpawnedGuyCount--;
		}

		// Received from M01_HON_Easy_Spawned_Guy_02_JDG when killed
		else if (param == 209)
		{
			this->honEasySpawnedGuy2Alive = false;
			this->honEasySpawnedGuyCount--;
		}

		// Received from M01_HON_Easy_Spawned_Guy_03_JDG when killed
		else if (param == 210)
		{
			this->honEasySpawnedGuy3Alive = false;
			this->honEasySpawnedGuyCount--;
		}

		// Received from M01_HON_Easy_Spawned_Guy_01_JDG or M01_HON_Easy_Spawned_Guy_02_JDG  or M01_HON_Easy_Spawned_Guy_03_JDG when killed
		else if (param == 207)
		{
			this->honEasySpawnedGuyCount2--;
		}

		// Received from M01_Church_LoveShack_Nun_JDG when poked
		// Received from M01_Loveshack_Nun_Conversation_Zone_JDG when entered
		else if (param == 212)
		{
			if (Commands->Find_Object(101310) && !this->churchGuardMiniGunnerKilled && !this->remindedStarOfChurchCivs) // loveShackNunObj
			{
				this->remindedStarOfChurchCivs = true;

				this->lockeChurchReminderConversationConversationId = Commands->Create_Conversation("M01_Locke_ChurchReminder_Conversation", 45, 1000.0f, true); // I'm still detecting civilians inside the church.
				Commands->Join_Conversation(NULL, this->lockeChurchReminderConversationConversationId, false, false, true);
				Commands->Start_Conversation(this->lockeChurchReminderConversationConversationId, this->lockeChurchReminderConversationConversationId);
				Commands->Monitor_Conversation(obj, this->lockeChurchReminderConversationConversationId);
			}
		}

		// When M01_Church_Priest_JDG is poked
		// Received from M01_Priest_Conversation_Zone_JDG when entered
		else if (param == 213)
		{
			GameObject *churchInteriorPriestObj = Commands->Find_Object(101315);
			if (churchInteriorPriestObj && !this->churchLoveShackMiniGunnerKilled && !this->remindedStarOfMoreChurchCivs)
			{
				this->remindedStarOfMoreChurchCivs = true;

				this->priestReminderConversationConversationId = Commands->Create_Conversation("M01_PriestReminder_Conversation", 100, 1000.0f, false); // My gratitude, sir. Another member of our clergy is next door.
				Commands->Join_Conversation(churchInteriorPriestObj, this->priestReminderConversationConversationId, false, true, true);

				Vector3 pos = Commands->Get_Position(obj);
				GameObject *starObj = Commands->Get_A_Star(pos);

				Commands->Join_Conversation(starObj, this->priestReminderConversationConversationId, false, false, false);
				Commands->Start_Conversation(this->priestReminderConversationConversationId, this->priestReminderConversationConversationId);
				Commands->Monitor_Conversation(obj, this->priestReminderConversationConversationId);
			}
		}

		// Received from M01_Church_Interior_Nun_JDG when poked
		// Received from M01_Interior_Nun_Conversation_Zone_JDG when entered
		else if (param == 211)
		{
			GameObject *churchInteriorNunObj = Commands->Find_Object(101314);
			if (churchInteriorNunObj && !this->churchLoveShackMiniGunnerKilled && !this->remindedStarOfMoreChurchCivs2)
			{
				this->remindedStarOfMoreChurchCivs2 = true;

				this->interiorNunConversationConversationId = Commands->Create_Conversation("M01_Interior_Nun_Conversation", 100, 1000.0f, false); // Thank you.  Our sister is being interrogated next door.
				Commands->Join_Conversation(churchInteriorNunObj, this->interiorNunConversationConversationId, false, true, true);

				Vector3 pos = Commands->Get_Position(obj);
				GameObject *starObj = Commands->Get_A_Star(pos);

				Commands->Join_Conversation(starObj, this->interiorNunConversationConversationId, false, false, false);
				Commands->Start_Conversation(this->interiorNunConversationConversationId, this->interiorNunConversationConversationId);
				Commands->Monitor_Conversation(obj, this->interiorNunConversationConversationId);
			}
		}

		// Received from M01_TurretBeach_Turret_01_Script_JDG after 5 seconds when killed
		else if (param == 217)
		{
			if (!Commands->Find_Object(101434))// beachNodTurret1Obj
			{
				this->beachNodTurret1Destroyed = true;
			}

			if (!Commands->Find_Object(101435))// beachNodTurret2Obj
			{
				this->beachNodTurret2Destroyed = true;
			}

			if (this->beachNodTurret1Destroyed && this->beachNodTurret2Destroyed)
			{
				Commands->Send_Custom_Event(obj, obj, 0, 112, 0.0f);
			}
		}

		// Received from ourselves when param 117 is received
		else if (param == 219)
		{
			if (!this->allBeachTurretsDestroyed)
			{
				this->allBeachTurretsDestroyed = true;
			}
		}

		// Received from M01_BarnArea_NOD_Commander_Trigger_Zone_JDG or M01_BarnArea_NOD_Commander_Trigger_Zone02_JDG when entered
		else if (param == 93)
		{
			GameObject *entranceBarnAreaTowerNodOfficerObj = Commands->Find_Object(102360);
			if (entranceBarnAreaTowerNodOfficerObj)
			{
				// Eliminate Nod Officer
				// Nod Officers often coordinate with Command for reinforcements or artillery strikes. Eliminate the Nod Officer to prevent additional reinforcements from arriving.
				Commands->Add_Objective(104, OBJECTIVE_TYPE_SECONDARY, OBJECTIVE_STATUS_PENDING, 6268, NULL, 6135);
				Commands->Set_Obj_Radar_Blip_Shape(entranceBarnAreaTowerNodOfficerObj, RADAR_BLIP_SHAPE_OBJECTIVE);
				Commands->Set_Obj_Radar_Blip_Color(entranceBarnAreaTowerNodOfficerObj, RADAR_BLIP_COLOR_SECONDARY_OBJECTIVE);

				GameObject *M01ObjectivePogControllerJDGObj = Commands->Find_Object(105828);
				if (M01ObjectivePogControllerJDGObj)
				{
					Commands->Send_Custom_Event(obj, M01ObjectivePogControllerJDGObj, 6, 104, 0.0f);
				}

				if (this->field_D8)
				{
					Commands->Send_Custom_Event(obj, obj, 0, 101, 3.0f);
				}
			}
		}

		// Received from M01_Player_is_Leaving_GDI_Base_Zone when entered
		else if (param == 128)
		{
			if (this->firstTimeStarLeavingGDIBase)
			{
				this->firstTimeStarLeavingGDIBase = false;

				GameObject *beachNodTurret1Obj = Commands->Find_Object(101434);
				if (beachNodTurret1Obj)
				{
					Commands->Send_Custom_Event(obj, beachNodTurret1Obj, 0, 16, 0.0f);
				}

				GameObject *beachNodTurret2Obj = Commands->Find_Object(101435);
				if (beachNodTurret2Obj)
				{
					Commands->Send_Custom_Event(obj, beachNodTurret2Obj, 0, 16, 0.0f);
				}

				GameObject *gdiGunBoatObj = Commands->Find_Object(101477);
				if (gdiGunBoatObj)
				{
					Commands->Send_Custom_Event(obj, gdiGunBoatObj, 0, 16, 0.0f);
				}

				GameObject *beachTurretNodEngineerObj = Commands->Find_Object(101654);
				if (beachTurretNodEngineerObj)
				{
					Commands->Send_Custom_Event(obj, beachTurretNodEngineerObj, 0, 16, 0.0f);
				}

				Commands->Send_Custom_Event(obj, obj, 0, 110, 0.0f);

				GameObject *nonExistingObj = Commands->Find_Object(102476);
				if (nonExistingObj)
				{
					Commands->Send_Custom_Event(obj, nonExistingObj, 0, 16, 5.0f);
				}

				Commands->Set_Wind(60.0f, 1.0f, 0.5f, 120.0f);
				Commands->Set_Rain(0.5f, 120.0f, false);
			}
		}

		// Received from M01_Player_Is_Entering_Tailgun_Alley_Backway_JDG or M01_Player_Is_Entering_Tailgun_Alley_JDG when entered
		else if (param == 134)
		{
			GameObject *riverBridgeNodOfficerObj = Commands->Find_Object(102358);
			if (riverBridgeNodOfficerObj)
			{
				Commands->Send_Custom_Event(obj, riverBridgeNodOfficerObj, 0, 16, 8.0f);
			}

			GameObject *M01GDIBaseArtilleryControllerJDGObj = Commands->Find_Object(102294);
			if (M01GDIBaseArtilleryControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01GDIBaseArtilleryControllerJDGObj, 0, 24, 0.0f);
			}

			GameObject *openAreaNodLightTankObj = Commands->Find_Object(102435);
			if (openAreaNodLightTankObj)
			{
				Commands->Send_Custom_Event(obj, openAreaNodLightTankObj, 0, 16, 0.0f);
			}
		}

		// Received from M01_Player_is_Entering_GDI_Base_Zone when entered
		else if (param == 129)
		{
			GameObject *M01GDIBaseArtilleryControllerJDGObj = Commands->Find_Object(102294);
			if (M01GDIBaseArtilleryControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01GDIBaseArtilleryControllerJDGObj, 0, 16, 0.0f);
			}
		}

		// Received from ourselves when param 128 is received
		else if (param == 110)
		{
			// This is Gunboat Tango!  We're taking Nod Turret fire!  All available units please respond!
			// Take out the turrets to protect the gunboat and we'll land some firepower.
			this->addTurretsObjectiveConversationId = Commands->Create_Conversation("M01_Add_Turrets_Objective", 100, 1000.0f, false);
			Commands->Join_Conversation(NULL, this->addTurretsObjectiveConversationId, false, false, true);
			Commands->Join_Conversation(NULL, this->addTurretsObjectiveConversationId, false, false, true);
			Commands->Start_Conversation(this->addTurretsObjectiveConversationId, this->addTurretsObjectiveConversationId);
			Commands->Monitor_Conversation(obj, this->addTurretsObjectiveConversationId);
		}

		// Received from ourselves when conversation ended
		else if (param == 92)
		{
			// Destroy Nod Turrets
			// Destroying the turrets on the beach will help protect the GDI Gunboat, which coordinates with Command for additional support. Find these turrets and eliminate them.
			Commands->Add_Objective(109, OBJECTIVE_TYPE_SECONDARY, OBJECTIVE_STATUS_PENDING, 6269, NULL, 6136);
			
			GameObject *M01ObjectivePogControllerJDGObj = Commands->Find_Object(105828);
			if (M01ObjectivePogControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01ObjectivePogControllerJDGObj, 6, 109, 0.0f);
			}

			GameObject *beachNodTurret1Obj = Commands->Find_Object(101434);
			if (beachNodTurret1Obj)
			{
				Commands->Set_Obj_Radar_Blip_Shape(beachNodTurret1Obj, RADAR_BLIP_SHAPE_OBJECTIVE);
				Commands->Set_Obj_Radar_Blip_Color(beachNodTurret1Obj, RADAR_BLIP_COLOR_SECONDARY_OBJECTIVE);
			}
			
			GameObject *beachNodTurret2Obj = Commands->Find_Object(101435);
			if (beachNodTurret2Obj)
			{
				Commands->Set_Obj_Radar_Blip_Shape(beachNodTurret2Obj, RADAR_BLIP_SHAPE_OBJECTIVE);
				Commands->Set_Obj_Radar_Blip_Color(beachNodTurret2Obj, RADAR_BLIP_COLOR_SECONDARY_OBJECTIVE);
			}

			int conversationId = Commands->Create_Conversation("M01_Locke_Sending_C4_Conversation", 100, 1000.0f, false); // Havoc, C4 is inbound - use it.
			Commands->Join_Conversation(NULL, conversationId, false, false, true);
			Commands->Start_Conversation(conversationId, conversationId);

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-82.786f, 113.665f, 2.221f));
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X1D_GTower_FlareDrop.txt");
		}

		// Received from M01_Initial_Gunboat_Script_JDG when killed
		else if (param == 113)
		{
			if (!this->gunboatDestroyed)
			{
				this->gunboatDestroyed = true;

				Commands->Set_Objective_Status(109, OBJECTIVE_STATUS_FAILED);
			}
		}

		// Received from ourselves when param 217 is received
		else if (param == 112)
		{
			if (!this->gunboatDestroyed)
			{
				this->gunboatDestroyed = true;

				GameObject *gdiGunBoatObj = Commands->Find_Object(101477);
				if (gdiGunBoatObj)
				{
					Commands->Send_Custom_Event(obj, gdiGunBoatObj, 0, 112, 0.0f);

					int conversationId = Commands->Create_Conversation("M01_Remove_Turrets_Objective", 100, 1000.0f, false); // I'm sending you a Medium Tank.
					Commands->Join_Conversation(NULL, conversationId, false, false, true);
					Commands->Start_Conversation(conversationId, 0);
				}
				else
				{
					Commands->Send_Custom_Event(obj, obj, 0, 117, 5.0f);
				}
			}
		}

		// Received from ourselves after 5 seconds when param 112 is received
		else if (param == 117)
		{
			Commands->Set_Objective_Status(109, OBJECTIVE_STATUS_ACCOMPLISHED);

			GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
			Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 219, 0.0f);
		}

		// Received from M01_Barn_EntryZone_JDG when entered
		// Received from ourselves when param 227 is received
		else if (param == 114)
		{
			if (!this->field_3E && !this->starInBarn)
			{
				this->starInBarn = true;

				GameObject *barnFemalePrisonerObj = Commands->Find_Object(101442);
				if (barnFemalePrisonerObj)
				{
					Commands->Send_Custom_Event(obj, barnFemalePrisonerObj, 0, 27, 0.0f);
				}

				GameObject *billyObj = Commands->Find_Object(101444);
				if (billyObj)
				{
					Commands->Send_Custom_Event(obj, billyObj, 0, 27, 0.0f);
				}

				GameObject *barnMalePrisonerPierre = Commands->Find_Object(101443);
				if (barnMalePrisonerPierre)
				{
					Commands->Send_Custom_Event(obj, barnMalePrisonerPierre, 0, 27, 0.0f);
				}
			}
		}

		// Received from M01_BarnArea_Air_Evac_Chopper_JDG when custom param 4 is received
		else if (param == 115)
		{
			if (!this->barnCivsEvacuated)
			{
				this->barnCivsEvacuated = true;
			}
		}

		// Received from M01_TurretBeach_Chinook_Spawned_Soldier_NOD or M01_TurretBeach_Engineer_JDG when killed
		else if (param == 222)
		{
			GameObject *beachNodTurret1Obj = Commands->Find_Object(101434);
			GameObject *beachNodTurret2Obj = Commands->Find_Object(101435);
			if (beachNodTurret1Obj && beachNodTurret2Obj)
			{
				Commands->Create_Sound("M01EVAG_DSGN0268I1EVAG_SND", Vector3(0.0f, 0.0f, 0.0f), obj);

				GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-93.689f, 52.85f, -1.95f));
				Commands->Set_Facing(invisObj, 60.0f);
				Commands->Attach_Script(invisObj, "Test_Cinematic", "X1I_Nod_TurretBeach_TroopDrop.txt");
			}
		}

		// Received from M01_Billys_Conversation_Zone_JDG when param 225 is received
		else if (param == 225)
		{
			GameObject *billyObj = Commands->Find_Object(101444);
			if (billyObj && !this->field_35)
			{
				// Watch for machine gun and tanks over the hill. Use the small pass behind the house.
				// My friends need a ride, AT-7. Pick 'em up.
				// This is Air Transport Seven, we're evacuating your area. Stand by.
				this->billysConversationConversationId = Commands->Create_Conversation("M01_Billys_Conversation", 100, 1000.0f, true);;
				Commands->Join_Conversation(billyObj, this->billysConversationConversationId, true, true, true);

				Vector3 pos = Commands->Get_Position(obj);
				GameObject *starObj = Commands->Get_A_Star(pos);

				Commands->Join_Conversation(starObj, this->billysConversationConversationId, false, true, true);
				Commands->Join_Conversation(NULL, this->billysConversationConversationId, false, false, true);
				Commands->Start_Conversation(this->billysConversationConversationId, this->billysConversationConversationId);
				Commands->Monitor_Conversation(obj, this->billysConversationConversationId);
			}
		}

		// Received from M01_Barn_Babushkas_Conversation_Zone_JDG when param 226 is received
		// Received from M01_Barn_Prisoner_01_JDG when killed 
		else if (param == 226)
		{
			GameObject *barnFemalePrisonerObj = Commands->Find_Object(101442);
			GameObject *barnMalePrisonerPierreObj = Commands->Find_Object(101443);
			if (barnFemalePrisonerObj && !this->field_34)
			{
				// They took my family. They said they were here to help. What will I do?
				// I hope my family is okay...
				// My friends need a ride, AT-7. Pick 'em up.
				// This is Air Transport Seven, we're evacuating your area. Stand by.
				this->babushkasConversationConversationId = Commands->Create_Conversation("M01_Barn_Babushka_Conversation", 100, 1000.0f, true);
				Commands->Join_Conversation(barnFemalePrisonerObj, this->babushkasConversationConversationId, true, true, true);

				Vector3 pos = Commands->Get_Position(obj);
				GameObject *starObj = Commands->Get_A_Star(pos);

				Commands->Join_Conversation(starObj, this->babushkasConversationConversationId, false, false, false);
				Commands->Join_Conversation(NULL, this->babushkasConversationConversationId, false, false, false);
				Commands->Start_Conversation(this->babushkasConversationConversationId, this->babushkasConversationConversationId);
				Commands->Monitor_Conversation(obj, this->babushkasConversationConversationId);

				Commands->Send_Custom_Event(obj, barnFemalePrisonerObj, 0, 30, 0.0f);

				if (barnMalePrisonerPierreObj && !this->field_36)
				{
					Commands->Send_Custom_Event(obj, barnMalePrisonerPierreObj, 0, 37, 0.0f);
				}
			}
			else if (barnMalePrisonerPierreObj && !this->field_36)
			{
				// You're not going to hurt me?
				// Nope, but Nod will if you don't get on the chopper.
				// My friends need a ride, AT-7. Pick 'em up.
				// This is Air Transport Seven, we're evacuating your area. Stand by.
				this->civ3ConversationConversationId = Commands->Create_Conversation("M01_Civ03_Conversation", 100, 1000.0f, true);
				Commands->Join_Conversation(barnMalePrisonerPierreObj, this->civ3ConversationConversationId, true, true, true);

				Vector3 pos = Commands->Get_Position(obj);
				GameObject *starObj = Commands->Get_A_Star(pos);

				Commands->Join_Conversation(starObj, this->civ3ConversationConversationId, false, false, false);
				Commands->Join_Conversation(NULL, this->civ3ConversationConversationId, false, true, true);
				Commands->Start_Conversation(this->civ3ConversationConversationId, this->civ3ConversationConversationId);
				Commands->Monitor_Conversation(obj, this->civ3ConversationConversationId);
			}
		}

		// Received from M01_Barn_Door_Guard_JDG and M01_Barn_Point_Guard_01_JDG or M01_Barn_Point_Guard_02_JDG or M01_Barn_Talk_Guard_01_JDG when killed
		else if (param == 227)
		{
			if (++this->barnGuardDeathCount > 3)
			{
				Commands->Send_Custom_Event(obj, obj, 0, 114, 0.0f);
			}
		}

		// Never received
		else if (param == 198)
		{
			Commands->Debug_Message("***************************Starting the GDI base encounter\n");
			if (this->gdiBaseEncounterInactive)
			{
				this->gdiBaseEncounterInactive = false;
			}
		}

		// Received from M01_Comm_Center_Building_Script_JDG when damaged
		// Received from M01_Hand_of_Nod_Building_Script_JDG when damaged
		else if (param == 26)
		{
			if (sender == Commands->Find_Object(153909)) // nodHonBuildingObj
			{
				GameObject *honRepairEngineerObj = Commands->Find_Object(101948);
				Commands->Send_Custom_Event(obj, honRepairEngineerObj, 0, 189, 0.0f);
			}
			else if (sender == Commands->Find_Object(153910)) // nonCommCenterBuildingObj
			{
				GameObject *commRepairEngineerObj = Commands->Find_Object(101949);
				Commands->Send_Custom_Event(obj, commRepairEngineerObj, 0, 189, 0.0f);
			}
		}

		// Received from M01_Comm_Base_Commander_Conv_Start_Zone_JDG when entered
		else if (param == 214)
		{
			GameObject *hologramProjectorObj = Commands->Find_Object(157978);
			if (hologramProjectorObj)
			{
				GameObject *kaneHologramObj = Commands->Create_Object_At_Bone(hologramProjectorObj, "Nod_Kane_HologramHead", "KANEBONE");
				Commands->Attach_To_Object_Bone(kaneHologramObj, hologramProjectorObj, "KANEBONE");
				Commands->Set_Loiters_Allowed(kaneHologramObj, false);

				this->kaneHologramObjId = Commands->Get_ID(kaneHologramObj);

				GameObject *honHologramNodOfficerObj = Commands->Find_Object(101936);
				if (honHologramNodOfficerObj)
				{
					this->kaneConversationActive = true;

					// Am I to understand GDI has all our test subjects?
					// Kane, my lord, forgive me.
					// I will find someone with more faith to handle the situation.
					// But sir, we-
					// Then report to interrogation for faith restructuring.
					// Yes my lord. 
					this->kaneAndNumber2Conversation1ConversationId = Commands->Create_Conversation("M01_Kane_and_Number02_Conversation_01", 100, 1000.0f, true);
					Commands->Join_Conversation(kaneHologramObj, this->kaneAndNumber2Conversation1ConversationId, false, true, true);
					Commands->Join_Conversation(honHologramNodOfficerObj, this->kaneAndNumber2Conversation1ConversationId, false, true, true);
					Commands->Start_Conversation(this->kaneAndNumber2Conversation1ConversationId, this->kaneAndNumber2Conversation1ConversationId);
				}
			}
		}

		// Received from M01_Comm_Base_Commander_JDG when killed
		// Received from M01_Comm_Kane_n_Havoc_Conv_Start_Zone_JDG when entered
		else if (param == 216)
		{
			if (!this->commBaseCommanderKilled)
			{
				GameObject *kaneHologramObj = Commands->Find_Object(this->kaneHologramObjId);
				if (kaneHologramObj)
				{
					this->commBaseCommanderKilled = true;

					// Why do you interrupt me, GDI?  Hmm?
					// I got a present for ya!
					this->kaneAndHavocConversationConversationId = Commands->Create_Conversation("M01_Kane_and_Havoc_Conversation", 100, 1000.0f, false);
					Commands->Join_Conversation(kaneHologramObj, this->kaneAndHavocConversationConversationId, false, true, true);

					Vector3 pos = Commands->Get_Position(obj);
					GameObject *starObj = Commands->Get_A_Star(pos);

					Commands->Join_Conversation(starObj, this->kaneAndHavocConversationConversationId, false, false, true);
					Commands->Start_Conversation(this->kaneAndHavocConversationConversationId, this->kaneAndHavocConversationConversationId);
					Commands->Monitor_Conversation(obj, this->kaneAndHavocConversationConversationId);
				}
			}
		}

		// Never received
		else if (param == 72)
		{
			Commands->Create_Sound("M01EVAG_DSGN0268I1EVAG_SND", Vector3(0.0f, 0.0f, 0.0f), obj);

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-87.561f, 114.225f, 1.874f));
			Commands->Set_Facing(invisObj, 0.0f);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X1I_EasyNodChinook01.txt");
		}

		// Received from M01_GDI_GuardTower_NOD_Commander_JDG when param 71 is received
		// Received from M01_Nod_Commander_Conversation_Controller_GDI when conversation ended
		else if (param == 71)
		{
			Commands->Create_Sound("M01EVAG_DSGN0268I1EVAG_SND", Vector3(0.0f, 0.0f, 0.0f), obj);

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(12.703f, 84.163f, 1.898f));
			Commands->Set_Facing(invisObj, -130.0f);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X1I_EasyNodChinook01.txt");
		}

		// Received from M01_TailgunRun_NOD_Commander_JDG when param 73 is received
		else if (param == 73)
		{
			Commands->Create_Sound("M01EVAG_DSGN0268I1EVAG_SND", Vector3(0.0f, 0.0f, 0.0f), obj);

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-7.831f, 292.034f, 2.665f));
			Commands->Set_Facing(invisObj, 0.0f);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X1I_EasyNodChinook01.txt");
		}

		// Received from M01_ChurchArea_NOD_Commander_JDG when param 74 is received
		// Received from ourselves after 10 seconds when param 153 or 154 is received
		else if (param == 74)
		{
			Commands->Create_Sound("M01EVAG_DSGN0268I1EVAG_SND", Vector3(0.0f, 0.0f, 0.0f), obj);

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-91.898f, 424.178f, 20.761f));
			Commands->Set_Facing(invisObj, 0.0f);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X1I_EasyNodChinook.txt");
		}

		// Received from ourselves when conversation ended
		else if (param == 75)
		{
			GameObject *M01BarnAreaEvacMonitorJDGObj = Commands->Find_Object(103420); 
			if (M01BarnAreaEvacMonitorJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01BarnAreaEvacMonitorJDGObj, 0, 75, 0.0f);
			}
		}
	}
}

void M01_Mission_Controller_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED)
	{
		if (action_id == this->billysConversationConversationId)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 75, 0.0f);
		}
		else if (action_id == this->babushkasConversationConversationId)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 75, 0.0f);

			GameObject *barnFemalePrisonerObj = Commands->Find_Object(101442);
			if (barnFemalePrisonerObj)
			{
				if (!this->field_34)
				{
					Commands->Send_Custom_Event(obj, barnFemalePrisonerObj, 0, 31, 0.0f);
				}
			}
		}
		else if (action_id == this->civ3ConversationConversationId)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 75, 0.0f);
		}
		else if (action_id == this->addTurretsObjectiveConversationId)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 92, 0.0f);
		}
		else if (action_id == this->kaneAndNumber2Conversation1ConversationId)
		{
			this->kaneConversationActive = false;

			GameObject *nonExistingObj = Commands->Find_Object(101937);
			if (nonExistingObj)
			{
				Commands->Send_Custom_Event(obj, nonExistingObj, 0, 17, 0.0f);
			}
		}
		else if (action_id == this->field_98)
		{
			this->field_39 = false;
		}
		else if (action_id == this->kaneAndHavocConversationConversationId)
		{
			GameObject *kaneHologramObj = Commands->Find_Object(this->kaneHologramObjId);
			if (kaneHologramObj)
			{
				Commands->Destroy_Object(kaneHologramObj);
			}
		}
		else if (action_id == this->removeUnlockGateObjectiveConversationId)
		{
			// Open Detention Center
			// Access the Detention Center gate by accessing the nearby control panel.
			Commands->Add_Objective(113, OBJECTIVE_TYPE_PRIMARY, OBJECTIVE_STATUS_PENDING, 6219, NULL, 6042);
			Commands->Set_Objective_Radar_Blip(113, Vector3(-310.727f, 571.824f, 28.484f));
			Commands->Set_Objective_HUD_Info_Position(113, 95.0f, "POG_M01_1_03.tga", 7598, Vector3(-310.727f, 571.824f, 28.484f)); // Open
			Commands->Set_HUD_Help_Text(6219, Vector3(0.196f, 0.882f, 0.196f)); // Open Detention Center
		}
		else if (action_id == this->field_B8)
		{
			Commands->Create_2D_Sound("00-N048E");

			Commands->Send_Custom_Event(obj, obj, 0, 5, 3.0f);
		}
		else if (action_id == this->lockeChurchReminderConversationConversationId)
		{
			this->remindedStarOfChurchCivs = false;
		}
		else if (action_id == this->priestReminderConversationConversationId)
		{
			this->remindedStarOfMoreChurchCivs = false;
		}
		else if (action_id == this->interiorNunConversationConversationId)
		{
			this->remindedStarOfMoreChurchCivs2 = false;
		}
	}
}

ScriptRegistrant<M01_Mission_Controller_JDG> M01_Mission_Controller_JDGRegistrant("M01_Mission_Controller_JDG", "");