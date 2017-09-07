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
#include "MX0_A03_GDI_TROOPER_ONE.h"

void MX0_A03_GDI_TROOPER_ONE::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
}

// When MX0_A03_CONTROLLER_DAK receives custom type 401
void MX0_A03_GDI_TROOPER_ONE::Created(GameObject *obj)
{
	Commands->Attach_Script(obj, "M00_Send_Object_ID", "1500017,3,1.0f");
	Commands->Innate_Disable(obj);

	this->field_1C = true;
}

void MX0_A03_GDI_TROOPER_ONE::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		Commands->Action_Reset(obj, 100.0f);

		ActionParamsStruct params;
		params.Set_Basic(this, 91.0f, 1);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.8f, 1.0f);
		params.WaypathID = 1400124;

		Commands->Action_Goto(obj, params);
	}
	else if (type == 1)
	{
		Commands->Action_Reset(obj, 100.0f);

		ActionParamsStruct params;
		params.Set_Basic(this, 91.0f, 0);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.8f, 1.0f);
		params.WaypathID = 1400117;

		Commands->Action_Goto(obj, params);
	}
	else if (type == 2)
	{
		Commands->Action_Reset(obj, 100.0f);

		ActionParamsStruct params;
		params.Set_Basic(this, 91.0f, 2);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.8f, 1.0f);
		params.WaypathID = 1400132;
		
		Commands->Action_Goto(obj, params);
	}
	else if (type == 3)
	{
		if (this->field_1C)
		{
			int conversationId = Commands->Create_Conversation("MX0_A03_05", 0, 0.0f, true); // Blast that Harvester for us, sir!
			Commands->Join_Conversation(obj, conversationId, false, false, true);
			Commands->Start_Conversation(conversationId, 0);

			Commands->Send_Custom_Event(obj, obj, 3, 0, 7.0f);
		}
	}
	else if (type == 4)
	{
		this->field_1C = false;
	}
	else if (type == 5)
	{
		Commands->Action_Reset(obj, 100.0f);

		GameObject *nodBuggyObj = Commands->Find_Object(1400081);
		if (nodBuggyObj)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 90.0f, 3);
			params.Set_Attack(nodBuggyObj, 60.0f, 0.25f, true);

			Commands->Action_Attack(obj, params);
		}
	}
}

void MX0_A03_GDI_TROOPER_ONE::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered when done walking into a03. See custom type <= 0
	if (action_id == 1)
	{
		Commands->Set_Innate_Is_Stationary(obj, true);

		Commands->Innate_Enable(obj);
	}
}

ScriptRegistrant<MX0_A03_GDI_TROOPER_ONE> MX0_A03_GDI_TROOPER_ONERegistrant("MX0_A03_GDI_TROOPER_ONE", "");