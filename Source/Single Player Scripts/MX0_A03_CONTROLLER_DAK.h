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

class MX0_A03_CONTROLLER_DAK : public ScriptImpClass
{
	public:
		virtual	void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);

		int humveeObjId;
		int MX0GDIMiniGunner0BObjId;
		int reinforcementsEngineer1ObjId;
		int MX0A03TankObjId;
		int field_2C;
		int field_30;
		int field_34;
		DWORD aliveTurretCount;
		DWORD nodinfantryCount;
		int nodInfantryObj1Id;
		int nodInfantryObj2Id;
		DWORD field_48; // Not initialized and not used
		bool isNodHarvesterKilled;
		bool isNodBuggyDriverKilled;
		bool isStarStillInA03;
		DWORD nodLedgeDropCount;
};