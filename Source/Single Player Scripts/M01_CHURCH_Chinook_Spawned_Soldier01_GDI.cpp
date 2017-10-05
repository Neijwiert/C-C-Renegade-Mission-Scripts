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
#include "M01_CHURCH_Chinook_Spawned_Soldier01_GDI.h"

// After 401 cinematic frames in X1I_GDI_Church_TroopDrop.txt
void M01_CHURCH_Chinook_Spawned_Soldier01_GDI::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->firstEscortConversationId, sizeof(this->firstEscortConversationId), 2);
}

void M01_CHURCH_Chinook_Spawned_Soldier01_GDI::Created(GameObject *obj)
{
	this->field_1C = 1;
}

void M01_CHURCH_Chinook_Spawned_Soldier01_GDI::Killed(GameObject *obj, GameObject *killer)
{
	if (!this->field_1C)
	{
		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 182, 0.0f);
	}
}

void M01_CHURCH_Chinook_Spawned_Soldier01_GDI::Damaged(GameObject *obj, GameObject *damager, float amount)
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
void M01_CHURCH_Chinook_Spawned_Soldier01_GDI::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from ourselves after 10 to 60 seconds after this block or when action with id firstEscortConversationId is complete
	if (param == 67)
	{
		if (this->field_1C)
		{
			Commands->Action_Reset(obj, 100.0f);

			Vector3 homeLocation;

			float rand = Commands->Get_Random(0.5, 6.5);
			if (rand >= 0.5f && rand < 1.5f)
			{
				homeLocation = Vector3(-139.048f, 318.861f, 9.066f);
			}
			else if (rand >= 1.5f && rand < 2.5f)
			{
				homeLocation = Vector3(-81.16f, 333.117f, 10.131f);
			}
			else if (rand >= 2.5f && rand < 3.5f)
			{
				homeLocation = Vector3(-138.786f, 388.25f, 15.093f);
			}
			else if (rand >= 3.5f && rand < 4.5f)
			{
				homeLocation = Vector3(-140.785f, 348.389f, 21.931f);
			}
			else if (rand >= 4.5f && rand < 5.5f)
			{
				homeLocation = Vector3(-126.169f, 335.223f, 8.997f);
			}
			else
			{
				homeLocation = Vector3(-140.792f, 342.98f, 10.387f);
			}

			Commands->Set_Innate_Soldier_Home_Location(obj, homeLocation, 10.0f);

			ActionParamsStruct params;
			params.Set_Basic(this, 50.0f, 67);
			params.Set_Movement(homeLocation, 0.8f, 3.0f);

			Commands->Action_Goto(obj, params);

			float randDelay = Commands->Get_Random(10.0, 60.0f);
			Commands->Send_Custom_Event(obj, obj, 0, 67, randDelay);
		}
	}

	// Received from M01_mission_Controller_JDG when param 179 is received
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

	// Received from M01_mission_Controller_JDG when param 179 is received
	else if (param == 181)
	{
		Commands->Action_Reset(obj, 100.0f);
		
		Vector3 pos = Commands->Get_Position(obj);
		Commands->Set_Innate_Soldier_Home_Location(obj, pos, 5.0f);

		this->field_1C = 1;
	}

	// Received from M01_mission_Controller_JDG when param 189 is received with param = lastDamagedStarEnemyObjId
	else if (param == 189)
	{
		if (!this->field_1C)
		{
			GameObject *lastDamagedStarEnemyObj = Commands->Find_Object(param);
			if (lastDamagedStarEnemyObj)
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 45);
				params.Set_Movement(lastDamagedStarEnemyObj, 0.8f, 5.0f);
				params.AttackActive = true;
				params.Set_Attack(lastDamagedStarEnemyObj, 50.0f, 0.0f, true);

				Commands->Modify_Action(obj, 45, params, true, true);
			}
		}
	}

	// Received from M01_mission_Controller_JDG when param 149 is received
	else if (param == 149)
	{
		if (!this->field_1C)
		{
			GameObject *M01GDIEscortConversationControllerGDIObj = Commands->Find_Object(103396);
			if (M01GDIEscortConversationControllerGDIObj)
			{
				Commands->Send_Custom_Event(obj, M01GDIEscortConversationControllerGDIObj, 0, 29, 0.0f);
			}

			GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
			Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 182, 0.0f);

			this->field_1C = 1;

			Commands->Action_Reset(obj, 100.0f);

			Vector3 homeLocation;

			float rand = Commands->Get_Random(0.5f, 3.5f);
			if (rand >= 0.5f && rand < 1.5f)
			{
				homeLocation = Vector3(-160.217f, 446.353f, 34.976f);
			}
			else if (rand >= 1.5f && rand < 2.5f)
			{
				homeLocation = Vector3(-173.371f, 437.383f, 29.711f);
			}
			else
			{
				homeLocation = Vector3(-138.958f, 435.696f, 27.001f);
			}

			Commands->Set_Innate_Soldier_Home_Location(obj, homeLocation, 3.0f);

			ActionParamsStruct params;
			params.Set_Basic(this, 50.0f, 39);
			params.Set_Movement(homeLocation, 0.3f, 3.0f);

			Commands->Action_Goto(obj, params);
		}
	}

	// Received from M01_mission_Controller_JDG when param 150 is received
	else if (param == 150)
	{
		if (!this->field_1C)
		{
			GameObject *M01GDIEscortConversationControllerGDIObj = Commands->Find_Object(103396);
			if (M01GDIEscortConversationControllerGDIObj)
			{
				Commands->Send_Custom_Event(obj, M01GDIEscortConversationControllerGDIObj, 0, 29, 0.0f);
			}

			GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
			Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 182, 0.0f);

			this->field_1C = 1;

			Commands->Action_Reset(obj, 100.0f);

			Vector3 homeLocation;

			float rand = Commands->Get_Random(0.5f, 3.5f);
			if (rand >= 0.5f && rand < 1.5f)
			{
				homeLocation = Vector3(-36.192f, 319.671f, 5.226f);
			}
			else if (rand >= 1.5f && rand < 2.5f)
			{
				homeLocation = Vector3(-49.614f, 363.304f, 10.816f);
			}
			else
			{
				homeLocation = Vector3(-66.361f, 339.288f, 8.772f);
			}

			Commands->Set_Innate_Soldier_Home_Location(obj, homeLocation, 3.0f);

			ActionParamsStruct params;
			params.Set_Basic(this, 50.0f, 39);
			params.Set_Movement(homeLocation, 0.3f, 3.0f);

			Commands->Action_Goto(obj, params);
		}
	}
}

void M01_CHURCH_Chinook_Spawned_Soldier01_GDI::Enemy_Seen(GameObject *obj, GameObject *enemy)
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
void M01_CHURCH_Chinook_Spawned_Soldier01_GDI::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED)
	{
		if (action_id == this->firstEscortConversationId)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 67, 0.0f);
		}
	}
	else if(complete_reason == ACTION_COMPLETE_NORMAL)
	{
		if (action_id == 38)
		{
			if (obj)
			{
				Vector3 pos = Commands->Get_Position(obj);
				GameObject *starObj = Commands->Get_A_Star(pos);

				if (starObj)
				{
					// Sir - call us if you need backup.
					// Now you're talkin'.
					this->firstEscortConversationId = Commands->Create_Conversation("M01_FirstEscort_Conversation", 100, 50.0f, true);
					Commands->Join_Conversation(obj, this->firstEscortConversationId, true, true, true);
					Commands->Join_Conversation(starObj, this->firstEscortConversationId, true, true, true);
					Commands->Start_Conversation(this->firstEscortConversationId, this->firstEscortConversationId);
					Commands->Monitor_Conversation(obj, this->firstEscortConversationId);
				}
			}
		}
		if (action_id == 45)
		{
			if (!this->field_1C)
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 45);

				Vector3 pos = Commands->Get_Position(obj);
				GameObject *starObj = Commands->Get_A_Star(pos);

				params.Set_Movement(starObj, 1.0f, 2.0f);

				Commands->Action_Attack(obj, params);
			}
		}
	}
}

void M01_CHURCH_Chinook_Spawned_Soldier01_GDI::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "s_a_human.h_a_troopdrop"))
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 38);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		params.Set_Movement(starObj, 0.8f, 5.0f);

		Commands->Action_Goto(obj, params);
	}
}

void M01_CHURCH_Chinook_Spawned_Soldier01_GDI::Poked(GameObject *obj, GameObject *poker)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (poker == Commands->Get_A_Star(pos))
	{
		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 179, 0.0f);
	}
}

ScriptRegistrant<M01_CHURCH_Chinook_Spawned_Soldier01_GDI> M01_CHURCH_Chinook_Spawned_Soldier01_GDIRegistrant("M01_CHURCH_Chinook_Spawned_Soldier01_GDI", "");