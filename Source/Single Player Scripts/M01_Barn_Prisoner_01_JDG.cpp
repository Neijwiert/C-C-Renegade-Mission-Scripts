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
#include "M01_Barn_Prisoner_01_JDG.h"

/*
M01 -> 101442
*/
void M01_Barn_Prisoner_01_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->field_1D, sizeof(this->field_1D), 2);
}

void M01_Barn_Prisoner_01_JDG::Created(GameObject *obj)
{
	this->field_1C = false;
	this->field_1D = false;

	Commands->Innate_Disable(obj);

	ActionParamsStruct params;
	params.Set_Basic(this, 100.0f, 46);
	params.Set_Animation("S_A_HUMAN.H_A_HOST_L2B", true);

	Commands->Action_Play_Animation(obj, params);
}

void M01_Barn_Prisoner_01_JDG::Killed(GameObject *obj, GameObject *killer)
{
	Commands->Create_Sound("EVA_Civilian_Killed", Vector3(0.0f, 0.0f, 0.0f), obj);

	if (this->field_1C)
	{
		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 226, 2.0f);
	}

	GameObject *M01BarnAreaEvacMonitorJDGObj = Commands->Find_Object(103420);
	if (M01BarnAreaEvacMonitorJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01BarnAreaEvacMonitorJDGObj, 0, 229, 0.0f);
	}
}

// TODO
void M01_Barn_Prisoner_01_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_mission_Controller_JDG when param 114 is received
	if (param == 27)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 47);
		params.Set_Animation("S_A_HUMAN.H_A_HOST_L2C", false);

		Commands->Action_Play_Animation(obj, params);
	}

	// Received from M01_mission_Controller_JDG when param 226 is received
	else if (param == 30)
	{
		this->field_1C = true;
	}

	// Received from M01_mission_Controller_JDG when conversation ended
	else if (param == 31)
	{
		this->field_1C = false;
	}

	// Received from M01_BarnArea_Air_Evac_Chopper_JDG when created
	else if (param == 75)
	{
		this->field_1D = true;

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

		Commands->Set_Player_Type(obj, SCRIPT_PLAYERTYPE_GDI);
		Commands->Set_Obj_Radar_Blip_Color(obj, RADAR_BLIP_COLOR_NEUTRAL);

		GameObject *M01BarnBabushkasConversationZoneJDGObj = Commands->Find_Object(101662);
		if (M01BarnBabushkasConversationZoneJDGObj)
		{
			Commands->Destroy_Object(M01BarnBabushkasConversationZoneJDGObj);
		}
	}
}

// TODO
void M01_Barn_Prisoner_01_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_NORMAL)
	{
		if (action_id == 39)
		{
			Commands->Set_Innate_Is_Stationary(obj, true);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 46);

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			params.Set_Attack(starObj, 0.0f, 0.0f, true);
			params.Set_Animation("S_A_HUMAN.H_A_A0C0", true);

			Commands->Action_Play_Animation(obj, params);

			GameObject *M01BarnBabushkasConversationZoneJDGObj = Commands->Find_Object(101662);
			if (M01BarnBabushkasConversationZoneJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01BarnBabushkasConversationZoneJDGObj, 0, 16, 0.0f);
			}

			if (!this->field_1D)
			{
				Commands->Enable_Hibernation(obj, true);
			}
		}
		else if (action_id == 4001)
		{
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.75f;

			static const char *drops[2] =
			{
				"POW_Health_100",
				"POW_Armor_100"
			};

			int randomDropIndex = Commands->Get_Random_Int(0, 2);
			Commands->Create_Object(drops[randomDropIndex], pos);

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
			Commands->Enable_Hibernation(obj, false);

			Commands->Action_Reset(obj, 100.0f);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 39);
			params.Set_Movement(Vector3(-51.0f, 214.2f, 5.5f), 1.0f, 0.5f);

			Commands->Action_Goto(obj, params);
		}
	}
}

void M01_Barn_Prisoner_01_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "S_A_Human.XG_EV5_troop"))
	{
		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_Barn_Prisoner_01_JDG> M01_Barn_Prisoner_01_JDGRegistrant("M01_Barn_Prisoner_01_JDG", "");