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
#include "MX0_A03_HUMVEE.h"

void MX0_A03_HUMVEE::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(this->targetObjectIds, sizeof(this->targetObjectIds), 1);
	Auto_Save_Variable(&this->targetObjectIndex, sizeof(this->targetObjectIndex), 2);
	Auto_Save_Variable(&this->currentTargetObjId, sizeof(this->currentTargetObjId), 3);
}

void MX0_A03_HUMVEE::Created(GameObject *obj)
{
	this->targetObjectIndex = 1;
	this->targetObjectIds[1] = 1400081; // Nod Buggy
	this->targetObjectIds[2] = 1400001; // Nod Harvester

	Commands->Attach_Script(obj, "M00_Send_Object_ID", "1400041,1,0");
	Commands->Attach_Script(obj, "M00_Send_Object_ID", "1500017,1,1.0f");
	Commands->Attach_Script(obj, "M00_Send_Object_ID", "1500020,1,1.0f");

	// Reinforcements have arrived.
	// Eagle Base, we’re on the ground
	// Havoc, stay close to that Hum-Vee scout.
	int conversationId = Commands->Create_Conversation("MX0_A03_01", 0, 0.0f, true);

	Commands->Join_Conversation(NULL, conversationId, true, true, true);
	Commands->Start_Conversation(conversationId, 1);

	ActionParamsStruct params;
	params.Set_Basic(this, 90.0f, 0);
	params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.5f, 1.0f);
	params.WaypathID = 1400043;

	Commands->Action_Goto(obj, params);
}

void MX0_A03_HUMVEE::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 1)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 90.0f, 1);
		params.AttackActive = false;

		Commands->Modify_Action(obj, 1, params, false, true);

		float randInterval = Commands->Get_Random(1.0f, 3.0f);
		Commands->Send_Custom_Event(obj, obj, 2, 0, randInterval);
	}
	else if (type == 2)
	{
		if (Commands->Find_Object(this->currentTargetObjId))
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 90.0f, 1);
			params.AttackActive = true;

			Commands->Modify_Action(obj, 1, params, false, true);
			
			float randInterval = Commands->Get_Random(3.0f, 6.0f);
			Commands->Send_Custom_Event(obj, obj, 1, 0, randInterval);
		}
	}
	else if (type == 3)
	{
		Commands->Debug_Message("***** DAK ***** Humvee: Recieved custom to move to section 4.\n");

		Commands->Action_Reset(obj, 100.0f);

		ActionParamsStruct params;
		params.Set_Basic(this, 90.0f, 2);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.5f, 1.0f);
		params.WaypathID = 1400070;

		Commands->Action_Goto(obj, params);
	}
}

void MX0_A03_HUMVEE::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (!action_id)
	{
		this->currentTargetObjId = this->targetObjectIds[this->targetObjectIndex];
		GameObject *currentTargetObj = Commands->Find_Object(this->currentTargetObjId);
		if (currentTargetObj)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 90.0f, 1);
			params.Set_Attack(currentTargetObj, 60.0f, 0.25f, true);

			Commands->Action_Attack(obj, params);
		}
	}
	else if (action_id == 1)
	{
		this->currentTargetObjId = this->targetObjectIds[++this->targetObjectIndex]; // This is unsafe and it will go out of bounds if the current target is not killed yet
		GameObject *currentTargetObj = Commands->Find_Object(this->currentTargetObjId);
		if (currentTargetObj)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 90.0f, 1);
			params.Set_Attack(currentTargetObj, 60.0f, 0.25f, true);

			Commands->Action_Attack(obj, params);
		}
		else
		{
			Commands->Innate_Enable(obj);
		}
	}
}

ScriptRegistrant<MX0_A03_HUMVEE> MX0_A03_HUMVEERegistrant("MX0_A03_HUMVEE", "");