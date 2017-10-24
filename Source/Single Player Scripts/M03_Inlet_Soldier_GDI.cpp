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
#include "M03_Inlet_Soldier_GDI.h"

// This script is never used
void M03_Inlet_Soldier_GDI::Created(GameObject *obj)
{
	int number = Get_Int_Parameter("Number");
	if (number == 1)
	{
		Commands->Set_Innate_Soldier_Home_Location(obj, Vector3(29.0f, -60.0f, 2.634f), 5.0f);
	}
	else if (!number)
	{
		Commands->Set_Innate_Soldier_Home_Location(obj, Vector3(75.0f, -28.0f, 2.001f), 5.0f);
	}
	else if (number == 2)
	{
		Commands->Start_Timer(obj, this, 10.0f, 0);
	}
}

void M03_Inlet_Soldier_GDI::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (action_id == 50)
	{
		Commands->Start_Timer(obj, this, 10.0f, 0);
	}
}

void M03_Inlet_Soldier_GDI::Timer_Expired(GameObject *obj, int number)
{
	ActionParamsStruct params;
	params.Set_Basic(this, 40.0f, 50);
	params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.3f, 1.0f);
	params.WaypathID = 1144691;

	Commands->Action_Goto(obj, params);
}

ScriptRegistrant<M03_Inlet_Soldier_GDI> M03_Inlet_Soldier_GDIRegistrant("M03_Inlet_Soldier_GDI", "Number:int");