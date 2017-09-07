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

class MX0_Area4_Controller_DLS : public ScriptImpClass
{
	public:
		virtual void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);
		virtual void Timer_Expired(GameObject *obj, int number);

		int starAtAreaNumber;
		bool field_20;
		int field_24;
		int MX0GDIMiniGunner0BObjId;
		int field_2C;
		int nodLightTankObjId;
		int nodBuggyObjId;
		int field_38;
		int field_3C;
		int field_40;
		int field_44;
		int field_48;
		int field_4C;
		int MX0ObeliskObjId; // Structure
		int MX0NodObeliskObjId; // Weapon
		int field_58;
		int nodMobileArtilleryObjId;
		bool field_60;
		int field_64;
		bool field_68;
		int field_6C;
		int field_70;
		int field_74;
		int field_78;
		int field_7C;
};
