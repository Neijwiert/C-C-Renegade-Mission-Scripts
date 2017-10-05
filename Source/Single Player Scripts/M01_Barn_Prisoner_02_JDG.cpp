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
#include "M01_Barn_Prisoner_02_JDG.h"

/*
M01 -> 101444
*/
void M01_Barn_Prisoner_02_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
}

void M01_Barn_Prisoner_02_JDG::Created(GameObject *obj)
{
	this->field_1C = false;

	Commands->Innate_Disable(obj);

	ActionParamsStruct params;
	params.Set_Basic(this, 100.0f, 46);
	params.Set_Animation("S_A_HUMAN.H_A_HOST_L2B", true);

	Commands->Action_Play_Animation(obj, params);
}

void M01_Barn_Prisoner_02_JDG::Killed(GameObject *obj, GameObject *killer)
{
	Commands->Create_Sound("EVA_Civilian_Killed", Vector3(0.0f, 0.0f, 0.0f), obj);

	GameObject *M01BarnAreaEvacMonitorJDGObj = Commands->Find_Object(103420);
	if (M01BarnAreaEvacMonitorJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01BarnAreaEvacMonitorJDGObj, 0, 229, 0.0f);
	}
}

// TODO
void M01_Barn_Prisoner_02_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_mission_Controller_JDG when param 114 is received
	if (param == 27)
	{
		Commands->Enable_Hibernation(obj, false);
		Commands->Action_Reset(obj, 100.0f);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 47);
		params.Set_Animation("S_A_HUMAN.H_A_HOST_L2C", false);

		Commands->Action_Play_Animation(obj, params);
	}

	// Received from M01_BarnArea_Air_Evac_Chopper_JDG when created
	else if (param == 75)
	{
		this->field_1C = true;

		Commands->Enable_Hibernation(obj, false);
	}

	// Received from M01_BarnArea_Air_Evac_Chopper_JDG when animation is complete
	else if (param == 4002)
	{
		Commands->Enable_Hibernation(obj, false);

		GameObject *M01BarnAreaEvacMonitorJDGObj = Commands->Find_Object(103420);
		Vector3 M01BarnAreaEvacMonitorJDGObjPos = Commands->Get_Position(M01BarnAreaEvacMonitorJDGObj);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 4001);
		params.Set_Movement(M01BarnAreaEvacMonitorJDGObjPos, 0.8f, 1.0f);

		Commands->Action_Goto(obj, params);

		GameObject *M01BillysConversationZoneJDGObj = Commands->Find_Object(101661);
		if (M01BillysConversationZoneJDGObj)
		{
			Commands->Destroy_Object(M01BillysConversationZoneJDGObj);
		}
	}
}

// TODO
void M01_Barn_Prisoner_02_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_NORMAL)
	{
		if (action_id == 38)
		{
			GameObject *M01BillysConversationZoneJDGObj = Commands->Find_Object(101661);
			if (M01BillysConversationZoneJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01BillysConversationZoneJDGObj, 0, 16, 0.0f);
			}

			if (!this->field_1C)
			{
				Commands->Enable_Hibernation(obj, true);
			}
		}
		else if (action_id == 4001)
		{
			Vector3 dropPos = Commands->Get_Position(obj);
			dropPos.Z += 0.75f;

			static const char *drops[2] =
			{
				"POW_Health_100",
				"POW_Armor_100"
			};

			int randomDropIndex = Commands->Get_Random_Int(0, 2);
			Commands->Create_Object(drops[randomDropIndex], dropPos);

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);
			Commands->Give_Points(starObj, 2000.0f, false);

			GameObject *M01BarnAreaEvacMonitorJDGObj = Commands->Find_Object(103420);
			if (M01BarnAreaEvacMonitorJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01BarnAreaEvacMonitorJDGObj, 0, 4001, 0.0f);
				Commands->Send_Custom_Event(obj, M01BarnAreaEvacMonitorJDGObj, 0, 10, 0.0f);
			}
		}
		else if (action_id == 47)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 48);
			params.Set_Animation("S_A_HUMAN.H_A_HOST_L1C", false);

			Commands->Action_Play_Animation(obj, params);

			Commands->Set_Innate_Is_Stationary(obj, true);

		}
		else if (action_id == 48)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 38);
			params.Set_Movement(Vector3(-73.572f, 138.013f, 0.329f), 0.8f, 0.5f);

			Commands->Action_Goto(obj, params);

			Commands->Set_Player_Type(obj, SCRIPT_PLAYERTYPE_GDI);
			Commands->Set_Obj_Radar_Blip_Color(obj, RADAR_BLIP_COLOR_NEUTRAL);
		}
	}
}

void M01_Barn_Prisoner_02_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "S_A_Human.XG_EV5_troop"))
	{
		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_Barn_Prisoner_02_JDG> M01_Barn_Prisoner_02_JDGRegistrant("M01_Barn_Prisoner_02_JDG", "");