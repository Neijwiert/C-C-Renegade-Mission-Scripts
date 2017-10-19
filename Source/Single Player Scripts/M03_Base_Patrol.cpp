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
#include "M03_Base_Patrol.h"

/*
M03 -> 1141162 1144451
*/
void M03_Base_Patrol::Created(GameObject *obj)
{
	ActionParamsStruct params;
	params.Set_Basic(this, 49.0f, 0);
	
	Commands->Innate_Soldier_Enable_Footsteps_Heard(obj, false);

	params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.3f, 1.0f);
	params.WaypathID = Get_Int_Parameter("WaypathID");

	Commands->Action_Goto(obj, params);
}

void M03_Base_Patrol::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (!action_id && complete_reason == ACTION_COMPLETE_LOW_PRIORITY)
	{
		Commands->Innate_Soldier_Enable_Footsteps_Heard(obj, true);
	}
}

ScriptRegistrant<M03_Base_Patrol> M03_Base_PatrolRegistrant("M03_Base_Patrol", "WaypathID:int");