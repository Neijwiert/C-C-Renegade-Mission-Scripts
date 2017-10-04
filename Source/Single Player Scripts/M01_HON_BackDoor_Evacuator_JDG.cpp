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
#include "M01_HON_BackDoor_Evacuator_JDG.h"

// When M01_HON_BackDoor_Evacuator_JDG receives custom with type <= 0 and param 122
void M01_HON_BackDoor_Evacuator_JDG::Created(GameObject *obj)
{
	Commands->Enable_Hibernation(obj, false);
	
	float randXOffset = Commands->Get_Random(-10.0f, 10.0f);
	float randYOffset = Commands->Get_Random(-10.0f, 10.0f);

	ActionParamsStruct params;
	params.Set_Basic(this, 90.0f, 4001);
	params.Set_Movement(Vector3(-221.94701f + randXOffset, 515.12402f + randYOffset, 4.012f), 1.0f, 3.0f);

	Commands->Action_Goto(obj, params);
}

void M01_HON_BackDoor_Evacuator_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (damager == Commands->Get_A_Star(pos))
	{
		if (obj)
		{
			GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
			Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 189, 0.0f);
		}
	}
}

void M01_HON_BackDoor_Evacuator_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered when we moved to the location at create
	if (action_id == 4001 && complete_reason == ACTION_COMPLETE_NORMAL)
	{
		Commands->Enable_Hibernation(obj, true);
	}
}

ScriptRegistrant<M01_HON_BackDoor_Evacuator_JDG> M01_HON_BackDoor_Evacuator_JDGRegistrant("M01_HON_BackDoor_Evacuator_JDG", "");