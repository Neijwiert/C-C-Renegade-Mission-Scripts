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
#include "M01_TurretBeach_GDI_Guy_01_JDG.h"

/*
x1e_hover_troop.txt at frame(s): 1
*/
void M01_TurretBeach_GDI_Guy_01_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->turnOverTankConversationId, sizeof(this->turnOverTankConversationId), 1);
}

void M01_TurretBeach_GDI_Guy_01_JDG::Created(GameObject *obj)
{
	int objId = Commands->Get_ID(obj);

	GameObject *M01BarnAreaAIExitZoneJDG1Obj = Commands->Find_Object(112015);
	GameObject *M01BarnAreaAIExitZoneJDG2Obj = Commands->Find_Object(112016);

	if (M01BarnAreaAIExitZoneJDG1Obj)
	{
		Commands->Send_Custom_Event(obj, M01BarnAreaAIExitZoneJDG1Obj, 12, objId, 0.0f);
	}

	if (M01BarnAreaAIExitZoneJDG2Obj)
	{
		Commands->Send_Custom_Event(obj, M01BarnAreaAIExitZoneJDG2Obj, 12, objId, 0.0f);
	}
}

void M01_TurretBeach_GDI_Guy_01_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_BarnArea_AI_ExitZone_JDG when entered
	if (!type && param == 23)
	{
		Commands->Action_Reset(obj, 100.0f);

		ActionParamsStruct params;
		params.Set_Basic(this, 55.0f, 38);
		params.Set_Movement(Vector3(-49.146f, 201.741f, 1.59f), 0.8f, 1.0f);

		Commands->Action_Goto(obj, params);

		Commands->Set_Innate_Soldier_Home_Location(obj, Vector3(-49.146f, 201.741f, 1.59f), 5.0f);
	}
}

void M01_TurretBeach_GDI_Guy_01_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// When done moving to star, see animation complete
	if (action_id == 65)
	{
		this->turnOverTankConversationId = Commands->Create_Conversation("M01_TurretBeach_TurnOverTank_Conversation", 100, 50.0f, true); // This conversation does not exist
		Commands->Join_Conversation(obj, this->turnOverTankConversationId, true, true, true);
		Commands->Start_Conversation(this->turnOverTankConversationId, this->turnOverTankConversationId);
		Commands->Monitor_Conversation(obj, this->turnOverTankConversationId);
	}
	else if (action_id == this->turnOverTankConversationId)
	{
		if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 80.0f, 38);
			params.Set_Movement(Vector3(-49.7f, 109.7f, 2.0f), 0.8f, 0.5f);

			Commands->Action_Goto(obj, params);
		}
	}

	// When done with movement, see conversation ended turnOverTankConversationId or custom param 23
	else if (action_id == 38)
	{
		Vector3 pos = Commands->Get_Position(obj);
		Commands->Set_Innate_Soldier_Home_Location(obj, pos, 10.0f);
	}
}

void M01_TurretBeach_GDI_Guy_01_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "S_A_Human.H_A_X1E_Run"))
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 65);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		params.Set_Movement(starObj, 0.8f, 6.0f);

		Commands->Action_Goto(obj, params);
	}
}

ScriptRegistrant<M01_TurretBeach_GDI_Guy_01_JDG> M01_TurretBeach_GDI_Guy_01_JDGRegistrant("M01_TurretBeach_GDI_Guy_01_JDG", "");