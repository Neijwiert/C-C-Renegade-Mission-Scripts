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
M02 -> 1111112
*/
class M02_Respawn_Controller : public ScriptImpClass
{
	public:
		virtual	void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);
		virtual void Timer_Expired(GameObject *obj, int number);

		void Check_Respawns(int areaId);
		void Replacement_Vehicle(GameObject *obj, int areaId);
		void Use_Heli_Drop_01(int areaId);
		void Use_Heli_Drop_02(int areaId);
		void Use_Parachute_Drop(int areaId);
		bool Use_Spawners(int areaId);

		bool activeAreas[26];
		bool heliDrop01Active[26];
		bool heliDrop02Active[26];
		bool paraDropActive[26];
		BYTE nodSoldierCount[26];
		BYTE maxNodSoldierCount[26];
		int nodSoldierObjIds[26];
		int spawnerTwiddler;
		int field_124; // Assigned, but not used
		int convoyBridgeNodSAMSiteDestroyedCount;
		int nodTiberiumSiloDestroyedCount;
};