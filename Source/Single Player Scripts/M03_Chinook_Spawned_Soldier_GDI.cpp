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
#include "M03_Chinook_Spawned_Soldier_GDI.h"

// This script is never used
void M03_Chinook_Spawned_Soldier_GDI::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->area, sizeof(this->area), 1);
	Auto_Save_Variable(&this->sendTypeWhenKilled, sizeof(this->sendTypeWhenKilled), 2);
	Auto_Save_Variable(&this->targetId, sizeof(this->targetId), 3);
	Auto_Save_Variable(&this->field_2C, sizeof(this->field_2C), 4);
	Auto_Save_Variable(&this->field_2D, sizeof(this->field_2D), 5);
	Auto_Save_Variable(&this->field_28, sizeof(this->field_28), 6);
	Auto_Save_Variable(&this->followingStar, sizeof(this->followingStar), 7);
	Auto_Save_Variable(&this->lowPriorityFollowingStar, sizeof(this->lowPriorityFollowingStar), 8);
}

void M03_Chinook_Spawned_Soldier_GDI::Created(GameObject *obj)
{
	this->followingStar = false;
	this->area = Get_Int_Parameter("Area");
	this->sendTypeWhenKilled = Get_Int_Parameter("Send_Type_When_Killed");
	this->targetId = Get_Int_Parameter("Target_ID");
	this->field_2C = false;
	this->field_2D = false;
	this->field_28 = 0;

	Commands->Set_Innate_Aggressiveness(obj, 0.0f);

	Commands->Start_Timer(obj, this, 10.0f, 0);
	Commands->Start_Timer(obj, this, 3.0f, 9);
}

void M03_Chinook_Spawned_Soldier_GDI::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *targetObj = Commands->Find_Object(this->targetId);
	if (targetObj)
	{
		Commands->Send_Custom_Event(obj, targetObj, this->sendTypeWhenKilled, this->area, 0.0f);
	}

	if (this->followingStar)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Send_Custom_Event(obj, starObj, 3100, 3100, 0.0f);
	}
}

void M03_Chinook_Spawned_Soldier_GDI::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 999)
	{
		if (param == 999)
		{
			Commands->Action_Reset(obj, 100.0f);

			Vector3 moveLocation;
			if (this->area == 1)
			{
				moveLocation = Vector3(87.0f, -58.0f, 1.662f);
			}
			else if (this->area == 2)
			{
				moveLocation = Vector3(-125.0f, 13.0f, 9.362f);
			}
			else if (!this->area)
			{
				moveLocation = Vector3(-100.0f, -50.0f, 5.986f);
			}

			if (this->followingStar)
			{
				this->followingStar = false;

				Vector3 pos = Commands->Get_Position(obj);
				GameObject *starObj = Commands->Get_A_Star(pos);

				Commands->Send_Custom_Event(obj, starObj, 3100, 3100, 0.0f);

				int conversationId = Commands->Create_Conversation("M03CON042", 99, 200.0f, false); // I'm too far from my post. Orders, sir, sorry.
				Commands->Join_Conversation(obj, conversationId, true, true, true);
				Commands->Start_Conversation(conversationId, 100042);
				Commands->Monitor_Conversation(obj, conversationId);
			}

			ActionParamsStruct params;
			params.Set_Basic(this, 90.0f, 0);
			params.Set_Movement(moveLocation, 0.8f, 1.0f);
	
			Commands->Action_Goto(obj, params);
		}
	}
	else if (type == 1001)
	{
		if (param == 1001 && this->followingStar)
		{
			this->followingStar = false;

			Commands->Action_Reset(obj, 100.0f);

			Vector3 moveLocation;
			if (this->area == 1)
			{
				moveLocation = Vector3(87.0f, -58.0f, 1.662f);
			}
			else if (this->area == 2)
			{
				moveLocation = Vector3(-125.0f, 13.0f, 9.362f);
			}
			else if (!this->area)
			{
				moveLocation = Vector3(-100.0f, -50.0f, 5.986f);
			}

			ActionParamsStruct params;
			params.Set_Basic(this, 90.0f, 0);
			params.Set_Movement(moveLocation, 0.8f, 1.0f);

			Commands->Action_Goto(obj, params);
		}
	}
}

void M03_Chinook_Spawned_Soldier_GDI::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (action_id)
	{
		if (complete_reason == ACTION_COMPLETE_LOW_PRIORITY)
		{
			if (this->followingStar)
			{
				this->lowPriorityFollowingStar = true;

				Commands->Start_Timer(obj, this, 5.0f, 3);
			}
		}
		else if (complete_reason == ACTION_COMPLETE_NORMAL)
		{
			GameObject *unknownObj = Commands->Find_Object(action_id);
			if (unknownObj)
			{
				Commands->Send_Custom_Event(obj, unknownObj, 3000, 3000, 0.0f);
			}

			GameObject *targetObj = Commands->Find_Object(this->targetId);
			if (targetObj)
			{
				Commands->Send_Custom_Event(obj, targetObj, 12000, 12000, 0.0f);
			}

			this->field_2C = true;
		}
	}
	else
	{
		this->field_2D = false;
	}
}

void M03_Chinook_Spawned_Soldier_GDI::Timer_Expired(GameObject *obj, int number)
{
	// Triggered after 5 seconds, see action_complete action_id > 0
	if (number == 3)
	{
		if (this->followingStar)
		{
			if (this->lowPriorityFollowingStar)
			{
				this->lowPriorityFollowingStar = false;

				ActionParamsStruct params;
				params.Set_Basic(this, 89.0f, 5);

				Vector3 pos = Commands->Get_Position(obj);
				GameObject *starObj = Commands->Get_A_Star(pos);

				params.Set_Movement(starObj, 1.0f, 3.0f);
				params.MoveFollow = true;

				Commands->Action_Goto(obj, params);
			}
		}
	}

	// Triggered after 3 seconds, see create
	else if (number == 9)
	{
		float randMoveDistance = Commands->Get_Random(-12.0f, 12.0f);

		Vector3 pos = Commands->Get_Position(obj);
		float facing = Commands->Get_Facing(obj);

		pos.X += WWMath::Cos(DEG_TO_RADF(facing)) * randMoveDistance;
		pos.Y += WWMath::Sin(DEG_TO_RADF(facing)) * randMoveDistance;

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 40);
		params.Set_Movement(pos, 0.8f, 1.0f);

		Commands->Action_Goto(obj, params);
	}

	// Triggered after 10 seconds after create or this block
	else
	{
		if (this->field_28 <= 2)
		{
			this->field_28++;
		}

		if (!this->field_2C)
		{
			this->field_2D = false;

			Commands->Start_Timer(obj, this, 10.0f, 0);
		}
	}
}

void M03_Chinook_Spawned_Soldier_GDI::Poked(GameObject *obj, GameObject *poker)
{
	int unknown = 1;
	Commands->Send_Custom_Event(obj, poker, 3000, reinterpret_cast<int>(&unknown), 0.0f);

	if (unknown == 1)
	{
		int conversationId = Commands->Create_Conversation("M03CON041", 99, 200.0f, false); //I'll stay right here, sir.
		Commands->Join_Conversation(obj, conversationId, true, true, true);
		Commands->Start_Conversation(conversationId, 100041);
		Commands->Monitor_Conversation(obj, conversationId);
	}
	else if (unknown)
	{
		if (unknown == -1 && Commands->Get_Difficulty_Level() != 2)
		{
			int conversationId = Commands->Create_Conversation("M03CON041", 99, 200.0f, false); //I'll stay right here, sir.
			Commands->Join_Conversation(obj, conversationId, true, true, true);
			Commands->Start_Conversation(conversationId, 100041);
			Commands->Monitor_Conversation(obj, conversationId);

			this->followingStar = false;

			Commands->Action_Reset(obj, 100.0f);
		}
	}
	else
	{
		Vector3 pos = Commands->Get_Position(obj);

		if (this->area == 1)
		{
			Commands->Set_Innate_Soldier_Home_Location(obj, pos, 2000.0f);
		}

		int conversationId = Commands->Create_Conversation("M03CON040", 99, 200.0f, false); // Okay sir, I'm right behind you.
		Commands->Join_Conversation(obj, conversationId, true, true, true);
		Commands->Start_Conversation(conversationId, 100040);
		Commands->Monitor_Conversation(obj, conversationId);

		this->followingStar = true;

		ActionParamsStruct params;
		params.Set_Basic(this, 89.0f, 5);

		GameObject *starObj = Commands->Get_A_Star(pos);

		params.Set_Movement(starObj, 1.0f, 3.0f);
		params.MoveFollow = true;

		Commands->Action_Goto(obj, params);
	}
}

ScriptRegistrant<M03_Chinook_Spawned_Soldier_GDI> M03_Chinook_Spawned_Soldier_GDIRegistrant("M03_Chinook_Spawned_Soldier_GDI", "Area:int, Send_Type_When_Killed:int, Target_ID:int");