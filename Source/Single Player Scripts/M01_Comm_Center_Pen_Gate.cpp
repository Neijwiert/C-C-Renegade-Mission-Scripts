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
#include "M01_Comm_Center_Pen_Gate.h"

/*
M01 -> 101117
*/
void M01_Comm_Center_Pen_Gate::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
}

void M01_Comm_Center_Pen_Gate::Created(GameObject *obj)
{
	this->field_1C = false;

	Commands->Enable_HUD_Pokable_Indicator(obj, true);
}

// TODO
void M01_Comm_Center_Pen_Gate::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		if (param == 125)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 16, 5.0f);
		}
		else if (param == 16)
		{
			this->field_1C = true;
		}
	}
}

void M01_Comm_Center_Pen_Gate::Poked(GameObject *obj, GameObject *poker)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (poker == Commands->Get_A_Star(pos) && this->field_1C)
	{
		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 200, 0.0f);
	}
}

ScriptRegistrant<M01_Comm_Center_Pen_Gate> M01_Comm_Center_Pen_GateRegistrant("M01_Comm_Center_Pen_Gate", "");