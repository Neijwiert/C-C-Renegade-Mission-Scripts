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

#pragma once

#include <scripts.h>

/*
M01 -> 100376
*/
class M01_Mission_Controller_JDG : public ScriptImpClass
{
	public:
		virtual	void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);
		virtual void Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason);

		bool field_1C; // Assigned, but not used
		bool firstTimeHONEntered;
		bool firstTimeHONGruntZoneEntered;
		bool firstTimeHONDojoZoneEntered;
		bool firstTimePrisonerPenSwitchPoked;
		bool firstTimeRadarScrambled;
		bool firstTimeStarLeavingGDIBase;
		bool prisonerGateLowered;
		bool churchGuardMiniGunnerKilled;
		bool churchLoveShackMiniGunnerKilled;
		bool doingChinookReinforcementsPokedConversation;
		bool failedChurchEvac;
		bool startedChurchEvac;
		bool commKilled;
		bool radarScrambled;
		bool announcedPrisonerObjective;
		bool commSAMSiteObjectiveActive;
		bool pokedPrisonPenGateSwitch;
		bool honEasySpawnedGuy1Alive;
		bool honEasySpawnedGuy2Alive;
		bool honEasySpawnedGuy3Alive;
		bool beachNodTurret1Destroyed;
		bool beachNodTurret2Destroyed;
		bool allBeachTurretsDestroyed;
		bool field_34; // Assigned and used, but never changed after create
		bool field_35; // Assigned and used, but never changed after create
		bool field_36; // Assigned and used, but never changed after create
		bool field_37; // Assigned, but not used
		bool kaneConversationActive; // Assigned, but not used
		bool field_39; // Assigned, but not used
		bool commBaseCommanderKilled;
		bool starInChurchArea;
		bool honWeatherSet;
		bool gdiBaseEncounterInactive;
		bool field_3E; // Assigned and used, but never changed after create
		bool field_3F; // Assigned, but not used
		bool barnCivsEvacuated;
		bool gunboatDestroyed;
		bool destroyedDetentionPenCivs;
		DWORD field_44; // Never assigned, but used
		int whackAMoleNodMinigunnerObjId;
		int nodHonSpawnedSoldier1ObjId;
		int nodHonSpawnedSoldier2ObjId;
		int nodHonSpawnedSoldier3ObjId;
		DWORD field_58; // Never assigned and never used
		int honEasySpawnedGuyCount;
		int honEasySpawnedGuyCount2;
		float honEasySpawnedGuySpawnInterval;
		DWORD field_68; // Assigned, but not used
		int kaneHologramObjId;
		int _00_N000ESoundId;
		int _00_N002ESoundId;
		int _00_N026ESoundId;
		int _00_N028ESoundId;
		int _00_N030ESoundId;
		int billysConversationConversationId;
		int babushkasConversationConversationId;
		DWORD field_8C; // Never assigned and never used
		DWORD field_90; // Never assigned and never used
		int kaneAndNumber2Conversation1ConversationId;
		int field_98; // Never assigned, but used
		int kaneAndHavocConversationConversationId;
		DWORD field_A0; // Never assigned and never used
		DWORD field_A4; // Never assigned and never used
		DWORD field_A8; // Never assigned and never used
		DWORD field_AC; // Never assigned and never used
		int civ3ConversationConversationId;
		DWORD field_B4; // Never assigned and never used
		int field_B8; // Never assigned, but used
		int removeUnlockGateObjectiveConversationId;
		int addTurretsObjectiveConversationId;
		int chinookSpawnedSoldierGDIObjId;
		int lastDamagedStarEnemyObjId;
		int honTroopDropCount;
		int barnGuardDeathCount;
		DWORD field_D4; // Assigned, but not used
		bool field_D8; // Assigned, but not used
		int lockeChurchReminderConversationConversationId;
		bool remindedStarOfChurchCivs;
		int priestReminderConversationConversationId;
		bool remindedStarOfMoreChurchCivs;
		int interiorNunConversationConversationId;
		bool remindedStarOfMoreChurchCivs2;
		bool starEnteredHONGruntZone;
		bool starInBarn;
		bool commSAMSiteDestroyed;
		int honKilled;
		int commKilled;
		int starAreaNumber;
};