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
#include "M01_HON_Cafeteria_Walking_Guy_JDG.h"

/*
M01 -> 101294
*/
void M01_HON_Cafeteria_Walking_Guy_JDG::Created(GameObject *obj)
{
	Commands->Innate_Disable(obj);
}

void M01_HON_Cafeteria_Walking_Guy_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
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
void M01_HON_Cafeteria_Walking_Guy_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (obj)
	{
		// Received from M01_mission_Controller_JDG when param 136 is received
		if (param == 16)
		{
			Commands->Innate_Enable(obj);

			ActionParamsStruct params;
			params.Set_Basic(this, 45.0f, 38);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.3f, 0.25f);
			params.WaypathID = 100661;
			params.WaypointStartID = 100662;
			params.WaypointEndID = 100663;

			Commands->Action_Goto(obj, params);
		}

		// Received from M01_mission_Controller_JDG when param 122 is received
		else if (param == 165)
		{
			Commands->Innate_Enable(obj);
			Commands->Enable_Hibernation(obj, false);

			Vector3 moveLocation;

			float rand = Commands->Get_Random(0.5, 4.5);
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
			float randYOffset = Commands->Get_Random(-10.0f, 10.0f);

			ActionParamsStruct params;
			params.Set_Basic(this, 90.0f, 4001);
			params.Set_Movement(Vector3(moveLocation.X + randXOffset, moveLocation.Y + randYOffset, moveLocation.Z), 1.0f, 3.0f);

			Commands->Action_Goto(obj, params);
		}
	}
}

// TODO
void M01_HON_Cafeteria_Walking_Guy_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_NORMAL)
	{
		if (action_id == 38)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 45.0f, 63);
			params.Set_Face_Location(Vector3(-170.179f, 520.595f, 5.999f), 1.0f);

			Commands->Action_Face_Location(obj, params);
		}
		else if (action_id == 63)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 45.0f, 46);
			params.Set_Animation("H_A_J17C", false);

			Commands->Action_Play_Animation(obj, params);
		}
		else if (action_id == 46)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 45.0f, 39);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.35f, 0.25f);
			params.WaypathID = 100674;
			params.WaypointStartID = 100676;
			params.WaypointEndID = 100682;

			Commands->Action_Goto(obj, params);
		}
		else if (action_id == 39)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 45.0f, 47);
			params.Set_Animation("H_A_V42A", false);

			Commands->Action_Play_Animation(obj, params);
		}
		else if (action_id == 4001)
		{
			Commands->Enable_Hibernation(obj, true);
		}
		else if(action_id == 47)
		{
			Commands->Set_Position(obj, Vector3(-176.299f, 532.229f, -4.6f));
			Commands->Set_Facing(obj, 60.0f);

			ActionParamsStruct params;
			params.Set_Basic(this, 45.0f, 100);
			params.Set_Animation("H_A_V11A", true);

			Commands->Action_Play_Animation(obj, params);
		}
	}
}

ScriptRegistrant<M01_HON_Cafeteria_Walking_Guy_JDG> M01_HON_Cafeteria_Walking_Guy_JDGRegistrant("M01_HON_Cafeteria_Walking_Guy_JDG", "");