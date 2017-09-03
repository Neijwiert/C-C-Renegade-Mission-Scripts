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
		virtual void Timer_Expired(GameObject *obj, int number);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);

		void MX0_A02_Fill_Empty_Slot(GameObject *a2, bool a3);

		int field_1C[9];
		BYTE field_40;
		BYTE field_41;
		BYTE field_42;
		BYTE field_43;
		BYTE field_44;
		BYTE field_45;
		BYTE field_46;
		BYTE field_47;
		DWORD field_48;
		DWORD field_4C;
		int destroyedHumveeObjId;
		int destroyedMedTankObjId;
		int apcObjId;
		DWORD field_5C;
		DWORD field_60;
		int blockageObjId;
		DWORD field_68;
		DWORD field_6C;
		DWORD field_70;
		DWORD field_74;
		DWORD field_78;
		DWORD field_7C;
		DWORD field_80;
		DWORD field_84;
};