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
#include "M01_HON_Cafeteria_Eating_Guy_JDG.h"

/*
M01 -> 101293
*/
void M01_HON_Cafeteria_Eating_Guy_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
}

void M01_HON_Cafeteria_Eating_Guy_JDG::Created(GameObject *obj)
{
	this->field_1C = 0;

	Commands->Innate_Disable(obj);
	Commands->Set_Facing(obj, -125.0f);
}

void M01_HON_Cafeteria_Eating_Guy_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
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

// TODO
void M01_HON_Cafeteria_Eating_Guy_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (obj)
	{
		if (param == 16)
		{
			Commands->Innate_Enable(obj);

			ActionParamsStruct params;
			params.Set_Basic(this, 60.0f, 100);
			params.Set_Animation("H_A_V11A", true);

			Commands->Action_Play_Animation(obj, params);
		}
		else if (param == 165)
		{
			Commands->Innate_Enable(obj);
			Commands->Enable_Hibernation(obj, false);

			Vector3 moveLocation;

			float rand = Commands->Get_Random(0.5f, 4.5f);
			if (rand >= 0.5f && rand < 1.5f)
			{
				moveLocation = Vector3(-156.63699f, 553.16302f, 4.2950001f);
			}
			else if (rand >= 1.5f && rand < 2.5f)
			{
				moveLocation = Vector3(-145.746f, 531.96399f, 4.2709999f);
			}
			else if (rand >= 2.5f && rand < 3.5f)
			{
				moveLocation = Vector3(-163.828f, 515.28998f, 4.2579999f);
			}
			else
			{
				moveLocation = Vector3(-221.94701f, 515.12402f, 4.0120001f);
			}

			float randXOffset = Commands->Get_Random(-10.0f, 10.0f);
			float randYOffset = Commands->Get_Random(-10.0, 10.0);

			ActionParamsStruct params;
			params.Set_Basic(this, 90.0f, 4001);
			params.Set_Movement(Vector3(moveLocation.X + randXOffset, moveLocation.Y + randYOffset, moveLocation.Z), 1.0f, 3.0f);

			Commands->Action_Goto(obj, params);
		}
	}
}

// TODO
void M01_HON_Cafeteria_Eating_Guy_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (action_id == 4001 && complete_reason == ACTION_COMPLETE_NORMAL)
	{
		Commands->Enable_Hibernation(obj, true);
	}
}

ScriptRegistrant<M01_HON_Cafeteria_Eating_Guy_JDG> M01_HON_Cafeteria_Eating_Guy_JDGRegistrant("M01_HON_Cafeteria_Eating_Guy_JDG", "");