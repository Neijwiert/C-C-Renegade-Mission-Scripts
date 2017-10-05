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
#include "M01_Comm_Center_Player_Terminal_Zone.h"

/*
M01 -> 107809
*/
void M01_Comm_Center_Player_Terminal_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->hacked, sizeof(this->hacked), 1);
	Auto_Save_Variable(&this->commKilled, sizeof(this->commKilled), 2);
}

void M01_Comm_Center_Player_Terminal_Zone::Created(GameObject *obj)
{
	this->commKilled = false;

	Commands->Enable_HUD_Pokable_Indicator(obj, true);
	Commands->Display_Health_Bar(obj, false);

	this->hacked = false;
}

void M01_Comm_Center_Player_Terminal_Zone::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		// Received from M01_mission_Controller_JDG when param 88 is received
		if (param == 108)
		{
			Commands->Set_Obj_Radar_Blip_Shape(obj, RADAR_BLIP_SHAPE_OBJECTIVE);
			Commands->Set_Obj_Radar_Blip_Color(obj, RADAR_BLIP_COLOR_PRIMARY_OBJECTIVE);
		}

		// Received from M01_Comm_Center_Building_Script_JDG when killed
		else if (param == 124)
		{
			this->commKilled = true;

			Commands->Enable_HUD_Pokable_Indicator(obj, false);
		}
	}
}

void M01_Comm_Center_Player_Terminal_Zone::Poked(GameObject *obj, GameObject *poker)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (poker == Commands->Get_A_Star(pos) && !this->hacked && !this->commKilled)
	{
		this->hacked = true;

		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 199, 0.0f);

		Commands->Set_Obj_Radar_Blip_Shape(obj, -1);
		Commands->Set_Obj_Radar_Blip_Color(obj, -1);

		GameObject *M01GateSwitchTutorialZoneJDGObj = Commands->Find_Object(120844);
		if (M01GateSwitchTutorialZoneJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01GateSwitchTutorialZoneJDGObj, 0, 16, 0.0f);
		}

		Commands->Enable_HUD_Pokable_Indicator(obj, false);
	}
}

ScriptRegistrant<M01_Comm_Center_Player_Terminal_Zone> M01_Comm_Center_Player_Terminal_ZoneRegistrant("M01_Comm_Center_Player_Terminal_Zone", "");