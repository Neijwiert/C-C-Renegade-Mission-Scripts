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

		void MX0_A02_Fill_Empty_Slot(GameObject *nodSoldierObj, bool a3);

		int soldierActorIds[9];
		bool field_40;
		bool field_41;
		bool field_42;
		bool field_43;
		bool field_44;
		bool field_45;
		bool field_46;
		bool field_47;
		DWORD field_48;
		DWORD field_4C;
		int destroyedHumveeObjId;
		int destroyedMedTankObjId;
		int apcObjId;
		DWORD field_5C;
		int field_60;
		int blockageObjId;
		int field_68;
		int field_6C;
		DWORD field_70;
		DWORD field_74;
		int field_78;
		int field_7C;
		int field_80;
		DWORD field_84;
};