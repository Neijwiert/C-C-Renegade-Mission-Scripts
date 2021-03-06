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
#include "M01_HON_FrontDoor_Evacuator_JDG.h"

// Created when M01_Mission_Controller received custom type 0 with param 122
void M01_HON_FrontDoor_Evacuator_JDG::Created(GameObject *obj)
{
	Commands->Enable_Hibernation(obj, false);

	ActionParamsStruct params;
	params.Set_Basic(this, 90.0f, 67);
	params.Set_Movement(Vector3(-150.3f, 532.0f, 4.2f), 1.0f, 0.25f);

	Commands->Action_Goto(obj, params);
}

void M01_HON_FrontDoor_Evacuator_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (damager == Commands->Get_A_Star(pos))
	{
		if (damager)
		{
			GameObject *M01MissionControllerObj = Commands->Find_Object(100376);
			Commands->Send_Custom_Event(obj, M01MissionControllerObj, 0, 189, 0.0f);
		}
	}
}

void M01_HON_FrontDoor_Evacuator_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered when action_id 67 is done
	if (action_id == 4001)
	{
		if (complete_reason == ACTION_COMPLETE_NORMAL)
		{
			Commands->Enable_Hibernation(obj, true);
		}
	}

	// Triggered after creation movement
	else if (action_id == 67 || complete_reason == ACTION_COMPLETE_NORMAL)
	{
		float rand = Commands->Get_Random(0.5f, 3.5f);

		Vector3 moveLocation;
		if (rand >= 0.5f && rand < 1.5f)
		{
			moveLocation = Vector3(-156.63699f, 553.16302f, 4.295f);
		}
		else if (rand >= 1.5f && rand < 2.5f)
		{
			moveLocation = Vector3(-145.746f, 531.96399f, 4.271f);
		}
		else
		{
			moveLocation = Vector3(-163.828f, 515.28998f, 4.258f);
		}

		float randXOffset = Commands->Get_Random(-10.0f, 10.0f);
		float randYOffset = Commands->Get_Random(-10.0f, 10.0f);

		ActionParamsStruct params;
		params.Set_Basic(this, 90.0f, 4001);
		params.Set_Movement(Vector3(moveLocation.X + randXOffset, moveLocation.Y + randYOffset, moveLocation.Z), 1.0f, 3.0f);

		Commands->Action_Goto(obj, params);
	}
}

ScriptRegistrant<M01_HON_FrontDoor_Evacuator_JDG> M01_HON_FrontDoor_Evacuator_JDGRegistrant("M01_HON_FrontDoor_Evacuator_JDG", "");