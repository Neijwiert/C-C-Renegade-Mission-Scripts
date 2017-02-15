/*
 * A Command & Conquer: Renegade SSGM Plugin, containing all the M13 (or M00) mission scripts
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
#include "MX0_Engineer_Goto.h"

void MX0_Engineer_Goto::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(bool), 1);
}

void MX0_Engineer_Goto::Created(GameObject *obj)
{
	// Initialize member variables
	this->field_1C = false;
}

void MX0_Engineer_Goto::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 105)
	{
		int gotoDest1 = Get_Int_Parameter("GotoDest1");
		GameObject *paramObj = Commands->Find_Object(param);
		Commands->Send_Custom_Event(obj, paramObj, 100, gotoDest1, 0.0f);
	}
	else if (type == 106)
	{
		int gotoDest2 = Get_Int_Parameter("GotoDest2");
		GameObject *paramObj = Commands->Find_Object(param);
		Commands->Send_Custom_Event(obj, paramObj, 100, gotoDest2, 0.0f);
	}
}

void MX0_Engineer_Goto::Entered(GameObject *obj, GameObject *enterer)
{
	if (!this->field_1C)
	{
		this->field_1C = true;

		int count = Get_Int_Parameter("Count");
		GameObject *missionStartDME = Commands->Find_Object(1200001);
		Commands->Send_Custom_Event(obj, missionStartDME, 102, count, 0.0f);
		Commands->Send_Custom_Event(obj, missionStartDME, 104, 1, 0.0f);
		Commands->Send_Custom_Event(obj, missionStartDME, 104, 2, 0.0f);
	}
}

ScriptRegistrant<MX0_Engineer_Goto> MX0_Engineer_GotoRegistrant("MX0_Engineer_Goto", "GotoDest1:int, GotoDest2:int, Count:int");