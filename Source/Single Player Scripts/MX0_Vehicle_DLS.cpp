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
#include "MX0_Vehicle_DLS.h"

void MX0_Vehicle_DLS::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(this->attackLocations, sizeof(this->attackLocations), 1);
	Auto_Save_Variable(&this->attackLocationIndex, sizeof(this->attackLocationIndex), 2);
	Auto_Save_Variable(&this->speed, sizeof(this->speed), 3);
}

// When MX0_Area4_Controller_DLS receives custom with type 445000 
// When MX0_Area4_Controller_DLS has timer with number <= 0 triggered
void MX0_Vehicle_DLS::Created(GameObject *obj)
{
	Commands->Enable_Enemy_Seen(obj, true);
	Commands->Enable_Engine(obj, true);

	this->attackLocations[0] = Get_Int_Parameter("Attack_Loc0");
	this->attackLocations[1] = Get_Int_Parameter("Attack_Loc1");
	this->attackLocations[2] = Get_Int_Parameter("Attack_Loc2");
	this->attackLocations[3] = Get_Int_Parameter("Attack_Loc3");
	this->attackLocationIndex = 0;
	this->speed = Get_Float_Parameter("Speed");

	ActionParamsStruct params;
	params.Set_Basic(this, 95.0f, 10);

	GameObject *attackLocationObj = Commands->Find_Object(this->attackLocations[this->attackLocationIndex]);
	Vector3 attackLocationObjPos = Commands->Get_Position(attackLocationObj);

	params.Set_Movement(attackLocationObjPos, this->speed, 5.0f);

	GameObject *muzzleForwardTargetObj = Commands->Find_Object(1500024);

	params.Set_Attack(muzzleForwardTargetObj, 0.0f, 0.0f, true);

	Commands->Action_Attack(obj, params);
}

void MX0_Vehicle_DLS::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from MX0_Area4_Controller_DLS when timer number <= 0 or 1 or 2 triggers
	if (type == 445001)
	{
		this->attackLocationIndex = param;

		ActionParamsStruct params;
		params.Set_Basic(this, 95.0f, 10);

		GameObject *attackLocationObj = Commands->Find_Object(this->attackLocations[this->attackLocationIndex]);
		Vector3 attackLocationObjPos = Commands->Get_Position(attackLocationObj);

		params.Set_Movement(attackLocationObjPos, this->speed, 5.0f);

		GameObject *muzzleForwardTargetObj = Commands->Find_Object(1500024);

		params.Set_Attack(muzzleForwardTargetObj, 0.0f, 0.0f, true);

		Commands->Action_Attack(obj, params);

		Commands->Debug_Message("Attack_Loc [%d] = %d \n", this->attackLocationIndex, this->attackLocations[this->attackLocationIndex]);
	}

	// Received from MX0_Area4_Controller_DLS when timer number <= 0 or 1 or 2 or 4 triggers
	else if (type == 445002)
	{
		if (param == 445010)
		{
			int conversationId = Commands->Create_Conversation("MX0_A04_CON001", 100, 200.0f, false); // Eagle Base– We found it!
			Commands->Join_Conversation(obj, conversationId, false, true, true);
			Commands->Start_Conversation(conversationId, 0);
			Commands->Monitor_Conversation(obj, conversationId);
		}
		else if (param == 445020)
		{
			int conversationId = Commands->Create_Conversation("MX0_A04_CON013", 100, 200.0f, false); // We need you up here, Sir.
			Commands->Join_Conversation(obj, conversationId, false, true, true);
			Commands->Start_Conversation(conversationId, 10);
		}
	}
}

void MX0_Vehicle_DLS::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	Commands->Debug_Message("ID %d sees Enemy ID %d \n", Commands->Get_ID(obj), Commands->Get_ID(enemy));

	ActionParamsStruct params;
	params.Set_Basic(this, 95.0f, 10);

	GameObject *attackLocationObj = Commands->Find_Object(this->attackLocations[this->attackLocationIndex]);
	Vector3 attackLocationObjPos = Commands->Get_Position(attackLocationObj);

	params.Set_Movement(attackLocationObjPos, this->speed, 5.0f);
	params.Set_Attack(enemy, 200.0f, 5.0f, true);

	Commands->Action_Attack(obj, params);
}

void MX0_Vehicle_DLS::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered when the following conversation completed:
	// - Eagle Base– We found it!
	// At custom type 445002
	if (!action_id)
	{
		if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED)
		{
			// Confirmed.  Excellent work!
			// Air strikes inbound.
			int conversationId = Commands->Create_Conversation("MX0_A04_CON002", 100, 200.0f, false);
			Commands->Join_Conversation(NULL, conversationId, false, true, true);
			Commands->Start_Conversation(conversationId, 1);
		}
	}

	// Triggered when conversation from action_id <= 0 is completed
	else if (action_id == 1)
	{
		if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED)
		{
			int conversationId = Commands->Create_Conversation("MX0_A04_CON003", 100, 200.0f, false); // They have an Obelisk!
			Commands->Join_Conversation(obj, conversationId, false, true, true);
			Commands->Start_Conversation(conversationId, 1);
		}
	}

	// Triggered when an attack completed
	else if (action_id == 10)
	{
		Commands->Debug_Message("Action_Complete");
	}
}

ScriptRegistrant<MX0_Vehicle_DLS> MX0_Vehicle_DLSRegistrant("MX0_Vehicle_DLS", "Attack_Loc0=0:int, Attack_Loc1=0:int, Attack_Loc2=0:int, Attack_Loc3=0:int, Speed=1.0:float");