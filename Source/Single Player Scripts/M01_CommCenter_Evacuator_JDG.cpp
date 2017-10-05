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
#include "M01_CommCenter_Evacuator_JDG.h"

// When M01_Mission_Controller_JDG reives custom type 0 with param 124
void M01_CommCenter_Evacuator_JDG::Created(GameObject *obj)
{
	Commands->Enable_Hibernation(obj, false);

	Vector3 moveLocation;

	float rand = Commands->Get_Random(0.5f, 2.5f);
	if (rand >= 0.5 && rand < 1.5f)
	{
		moveLocation = Vector3(-286.90399f, 557.896f, 27.616f);
	}
	else
	{
		moveLocation = Vector3(-318.14899f, 530.86798f, 27.971f);
	}

	float randXOffset = Commands->Get_Random(-5.0f, 5.0f);
	float randYOffset = Commands->Get_Random(-5.0f, 5.0f);

	ActionParamsStruct params;
	params.Set_Basic(this, 90.0f, 4001);
	params.Set_Movement(Vector3(moveLocation.X + randXOffset, moveLocation.Y + randYOffset, moveLocation.Z), 1.0f, 2.0f);

	Commands->Action_Goto(obj, params);
}

void M01_CommCenter_Evacuator_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (damager == Commands->Get_A_Star(pos))
	{
		if (obj)
		{
			GameObject *M01MissionControllerObj = Commands->Find_Object(100376);
			Commands->Send_Custom_Event(obj, M01MissionControllerObj, 0, 189, 0.0f);
		}
	}
}

void M01_CommCenter_Evacuator_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered when we finished movement at created
	if (action_id == 4001 && complete_reason == ACTION_COMPLETE_NORMAL)
	{
		Commands->Enable_Hibernation(obj, true);
	}
}

ScriptRegistrant<M01_CommCenter_Evacuator_JDG> M01_CommCenter_Evacuator_JDGRegistrant("M01_CommCenter_Evacuator_JDG", "");