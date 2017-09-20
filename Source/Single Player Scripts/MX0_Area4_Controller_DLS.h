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
M13 -> 1500017
*/
class MX0_Area4_Controller_DLS : public ScriptImpClass
{
	public:
		virtual void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);
		virtual void Timer_Expired(GameObject *obj, int number);

		int starAtAreaNumber;
		bool isA04Active;
		int humveeObjId;
		int MX0GDIMiniGunner0BObjId;
		int field_2C;
		int nodLightTankObjId;
		int nodBuggyObjId;
		int rightWallRocketTrooperObjId;
		int leftWallRocketTrooperObjId;
		int reinforcementsEngineer1ObjId;
		int field_44;
		int gdiRocketSoldier0ObjId;
		int gdiMiniGunner0ObjId;
		int MX0ObeliskObjId; // Structure
		int MX0NodObeliskObjId; // Weapon
		int gdiOrcaFLyoverObjId;
		int nodMobileArtilleryObjId;
		bool canDoFlyoverCinematic;
		int field_64;
		bool isObeliskSighted;
		int gdiReinforcementsDeathCount;
		int field_70;
		int field_74;
		int wrongWayConvTableIndex;
		int calledForStar; // Integer on purpose
};
