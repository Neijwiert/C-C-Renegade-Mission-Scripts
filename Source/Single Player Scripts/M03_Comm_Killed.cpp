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

#include "General.h"
#include "M03_Comm_Killed.h"

/*
M03 -> 1150002
*/
void M03_Comm_Killed::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->initialHealth, sizeof(this->initialHealth), 2);
}

void M03_Comm_Killed::Created(GameObject *obj)
{
	this->field_1C = false;
	this->initialHealth = Commands->Get_Health(obj);
}

void M03_Comm_Killed::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *M03CommCenterArrowObj = Commands->Find_Object(2009818);
	Commands->Send_Custom_Event(obj, M03CommCenterArrowObj, 40003, 0, 0.0f);

	Commands->Enable_Radar(true);
}

void M03_Comm_Killed::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (!this->field_1C)
	{
		Commands->Set_Health(obj, this->initialHealth);
	}
}

// TODO
void M03_Comm_Killed::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 40023)
	{
		this->field_1C = true;
	}
}

ScriptRegistrant<M03_Comm_Killed> M03_Comm_KilledRegistrant("M03_Comm_Killed", "");