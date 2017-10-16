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
#include "M10_Elevator_All_Zone.h"

/*
M03 -> 1100005 1148346
*/
void M10_Elevator_All_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->zoneEntered, sizeof(this->zoneEntered), 1);
}

void M10_Elevator_All_Zone::Created(GameObject *obj)
{
	this->zoneEntered = false;
}

void M10_Elevator_All_Zone::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		this->zoneEntered = true;

		int controllerNum = Get_Int_Parameter("Controller_num");
		GameObject *controllerObj = Commands->Find_Object(controllerNum);
		Commands->Send_Custom_Event(obj, controllerObj, 40022, 0, 0.0f);
	}
}

ScriptRegistrant<M10_Elevator_All_Zone> M10_Elevator_All_ZoneRegistrant("M10_Elevator_All_Zone", "Controller_num:int");