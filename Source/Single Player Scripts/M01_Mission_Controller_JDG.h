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

		bool field_1C;
		bool field_1D;
		bool field_1E;
		bool field_1F;
		bool field_20;
		bool field_21;
		bool field_22;
		bool field_23;
		bool field_24;
		bool field_25;
		bool field_26;
		bool field_27;
		bool field_28;
		bool field_29;
		bool field_2A;
		bool field_2B;
		bool field_2C;
		bool field_2D;
		bool field_2E;
		bool field_2F;
		bool field_30;
		bool field_31;
		bool field_32;
		bool field_33;
		bool field_34;
		bool field_35;
		bool field_36;
		bool field_37;
		bool field_38;
		bool field_39;
		bool field_3A;
		bool field_3B;
		bool field_3C;
		bool field_3D;
		bool field_3E;
		bool field_3F;
		bool field_40;
		bool field_41;
		bool field_42;
		DWORD field_44;
		int field_48;
		int field_4C;
		int field_50;
		int field_54;
		DWORD field_58;
		DWORD field_5C;
		DWORD field_60;
		float field_64;
		DWORD field_68;
		DWORD field_6C;
		int field_70;
		int field_74;
		int field_78;
		int field_7C;
		int field_80;
		DWORD field_84;
		DWORD field_88;
		DWORD field_8C;
		DWORD field_90;
		DWORD field_94;
		DWORD field_98;
		DWORD field_9C;
		DWORD field_A0;
		DWORD field_A4;
		DWORD field_A8;
		DWORD field_AC;
		DWORD field_B0;
		DWORD field_B4;
		DWORD field_B8;
		int field_BC;
		int field_C0;
		int field_C4;
		int field_C8;
		DWORD field_CC;
		DWORD field_D0;
		DWORD field_D4;
		bool field_D8;
		int field_DC;
		bool field_E0;
		int field_E4;
		bool field_E8;
		int field_EC;
		bool field_F0;
		bool field_F1;
		bool field_F2;
		bool field_F3;
		DWORD field_F4;
		DWORD field_F8;
		DWORD field_FC;
};