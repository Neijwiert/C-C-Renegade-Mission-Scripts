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

class MX0_A02_Controller : public ScriptImpClass
{
	public:
		virtual	void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);
		virtual void Timer_Expired(GameObject *obj, int number);

		void MX0_A02_Fill_Empty_Slot(GameObject *nodSoldierObj, bool isMinigunner);

		int soldierActorIds[9];
		bool starAtSniperArea; // Initialized, re-assigned, but not used
		bool spawnedA02Objects;
		bool canDoYeahTaunt;
		bool doingEvent;
		bool shouldWaitForFreeSlots;
		bool shouldDestroySpawners;
		bool helicopter0Killed;
		bool medTankEntered;
		int soldierActorTankReadyTauntNumber;
		int soldierActorWaitForHavocTauntActionId;
		int destroyedHumveeObjId;
		int destroyedMedTankObjId;
		int apcObjId;
		int sniper1ObjId;
		int sniper2ObjId;
		int blockageObjId;
		int progressCounter;
		int nodRocketSoldierActorIndex;
		int reinforcementsEngineer1ObjId;
		int reinforcementsEngineer2ObjId;
		int soldierActorCallingHavocTauntActionId;
		int soldierActorEnemyKilledTauntActionId;
		int starKillCount;
		int soldierActorWrongWayTauntActorIndex;
};