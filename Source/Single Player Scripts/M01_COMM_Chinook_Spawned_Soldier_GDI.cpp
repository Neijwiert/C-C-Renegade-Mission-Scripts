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
#include "M01_COMM_Chinook_Spawned_Soldier_GDI.h"

void M01_COMM_Chinook_Spawned_Soldier_GDI::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
}

// After 401/489 cinematic frames in X1I_GDI_CommCenter_TroopDrop.txt
void M01_COMM_Chinook_Spawned_Soldier_GDI::Created(GameObject *obj)
{
	this->field_1C = 1;
}

void M01_COMM_Chinook_Spawned_Soldier_GDI::Killed(GameObject *obj, GameObject *killer)
{
	if (!this->field_1C)
	{
		GameObject *M01MissionControllerObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerObj, 0, 182, 0.0f);
	}
}

void M01_COMM_Chinook_Spawned_Soldier_GDI::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (damager && !Commands->Get_Player_Type(damager) && !this->field_1C)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 45);
		params.Set_Movement(damager, 0.8f, 5.0f);
		params.Set_Attack(damager, 50.0f, 0.0f, true);
		params.AttackActive = true;
		params.AttackCheckBlocked = true;

		Commands->Modify_Action(obj, 45, params, true, true);
	}
}

// TODO
void M01_COMM_Chinook_Spawned_Soldier_GDI::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 189)
	{
		if (!this->field_1C)
		{
			GameObject *paramObj = Commands->Find_Object(param);
			if (paramObj)
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 45);
				params.Set_Movement(paramObj, 0.8f, 5.0f);
				params.AttackActive = true;
				params.Set_Attack(paramObj, 50.0f, 0.0f, true);

				Commands->Modify_Action(obj, 45, params, true, true);

				Commands->Debug_Message("**********************Player's escort has received orders to attack havocs target\n");
			}
		}
	}
	else
	{
		if (param == 67)
		{
			if (this->field_1C)
			{
				Commands->Action_Reset(obj, 100.0f);

				Vector3 homeLocation;

				float rand = Commands->Get_Random(0.5, 6.5);
				if (rand >= 0.5f && rand < 1.5f)
				{
					homeLocation = Vector3(-277.309f, 468.428f, 18.553f);
				}
				else if (rand >= 1.5f && rand < 2.5f)
				{
					homeLocation = Vector3(-271.237f, 535.962f, 27.935f);
				}
				else if (rand >= 2.5f && rand < 3.5f)
				{
					homeLocation = Vector3(-247.278f, 563.557f, 30.61f);
				}
				else if (rand >= 3.5f && rand < 4.5f)
				{
					homeLocation = Vector3(-296.349f, 583.76f, 27.739f);
				}
				else if (rand >= 4.5f && rand < 5.5f)
				{
					homeLocation = Vector3(-313.597f, 568.022f, 27.74f);
				}
				else
				{
					homeLocation = Vector3(-301.16f, 523.405f, 16.583f);
				}

				Commands->Set_Innate_Soldier_Home_Location(obj, homeLocation, 10.0f);

				ActionParamsStruct params;
				params.Set_Basic(this, 50.0f, 20);
				params.Set_Movement(homeLocation, 0.8f, 3.0f);

				Commands->Action_Goto(obj, params);

				float randDelay = Commands->Get_Random(10.0f, 60.0f);
				Commands->Send_Custom_Event(obj, obj, 0, 67, randDelay);
			}
		}
		else if (param == 180)
		{
			GameObject *M01GDIEscortConversationControllerGDIObj = Commands->Find_Object(103396);
			if (M01GDIEscortConversationControllerGDIObj)
			{
				Commands->Send_Custom_Event(obj, M01GDIEscortConversationControllerGDIObj, 0, 27, 0.0f);
			}

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 45);

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			params.Set_Movement(starObj, 1.0f, 2.0f);

			Commands->Action_Attack(obj, params);

			this->field_1C = 0;
		}
		else if (param == 181)
		{
			Commands->Action_Reset(obj, 100.0f);

			Vector3 pos = Commands->Get_Position(obj);
			Commands->Set_Innate_Soldier_Home_Location(obj, pos, 5.0f);

			this->field_1C = 1;
		}
	}
}

void M01_COMM_Chinook_Spawned_Soldier_GDI::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	if (!this->field_1C)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 45);
		params.Set_Movement(enemy, 0.8f, 5.0f);
		params.Set_Attack(enemy, 50.0f, 0.0f, true);
		params.AttackActive = true;
		params.AttackCheckBlocked = true;

		Commands->Modify_Action(obj, 45, params, true, true);
	}
}

// TODO
void M01_COMM_Chinook_Spawned_Soldier_GDI::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_NORMAL && action_id == 45 && !this->field_1C)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 45);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		params.Set_Movement(starObj, 1.0f, 2.0f);

		Commands->Action_Attack(obj, params);
	}
}

void M01_COMM_Chinook_Spawned_Soldier_GDI::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "s_a_human.h_a_troopdrop"))
	{
		Commands->Send_Custom_Event(obj, obj, 0, 67, 0.0f);
	}
}

void M01_COMM_Chinook_Spawned_Soldier_GDI::Poked(GameObject *obj, GameObject *poker)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (poker == Commands->Get_A_Star(pos))
	{
		GameObject *M01MissionControllerObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerObj, 0, 179, 0.0f);
	}
}

ScriptRegistrant<M01_COMM_Chinook_Spawned_Soldier_GDI> M01_COMM_Chinook_Spawned_Soldier_GDIRegistrant("M01_COMM_Chinook_Spawned_Soldier_GDI", "");