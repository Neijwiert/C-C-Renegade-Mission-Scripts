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
#include "M01_Base_POW02_JDG.h"

/*
M01 -> 116385
*/
void M01_Base_POW02_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 2);
}

void M01_Base_POW02_JDG::Created(GameObject *obj)
{
	Commands->Set_Obj_Radar_Blip_Color(obj, RADAR_BLIP_COLOR_GDI);
	Commands->Set_Loiters_Allowed(obj, false);

	ActionParamsStruct params;
	params.Set_Basic(this, 100.0f, 46);
	params.Set_Animation("H_A_HOST_L2B", true);

	Commands->Action_Play_Animation(obj, params);

	this->field_1C = false;
}

void M01_Base_POW02_JDG::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *M01GDIBasePOWEncounter02ControllerJDGObj = Commands->Find_Object(103331);
	if (M01GDIBasePOWEncounter02ControllerJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01GDIBasePOWEncounter02ControllerJDGObj, 0, 22, 0.0f);
	}
}

void M01_Base_POW02_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{	
	if (Commands->Get_Health(obj) <= 10.0f)
	{
		Commands->Set_Health(obj, 10.0f);
	}
}

// TODO
void M01_Base_POW02_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		// Received from M01_Base_POW01_JDG when custom type <= 0 and param 22 is received
		if (param == 16)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 47);
			params.Set_Animation("H_A_HOST_L2C", false);

			Commands->Action_Play_Animation(obj, params);
		}
		else if (param == 4002)
		{
			Commands->Enable_Hibernation(obj, false);
			Commands->Set_Innate_Is_Stationary(obj, false);

			GameObject *M01GDIBasePOWEncounter02ControllerJDGObj = Commands->Find_Object(103331);
			Vector3 M01GDIBasePOWEncounter02ControllerJDGObjPos = Commands->Get_Position(M01GDIBasePOWEncounter02ControllerJDGObj);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 39);
			params.Set_Movement(M01GDIBasePOWEncounter02ControllerJDGObjPos, 0.8f, 1.0f);

			Commands->Action_Goto(obj, params);
		}
		else if (param == 25)
		{
			this->field_1C = false;

			Commands->Enable_HUD_Pokable_Indicator(obj, false);
		}
	}
}

// TODO
void M01_Base_POW02_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_NORMAL)
	{
		if (action_id == 47)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 48);
			params.Set_Animation("H_A_HOST_L1C", false);

			Commands->Action_Play_Animation(obj, params);

			Commands->Set_Innate_Is_Stationary(obj, true);
		}
		else if (action_id == 48)
		{
			static const char *animations[4] =
			{
				"H_A_J01C",
				"H_A_J18C",
				"H_A_J24C",
				"H_A_J26C"
			};

			int randomAnimationIndex = Commands->Get_Random_Int(0, 4);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 49);
			params.Set_Animation(animations[randomAnimationIndex], false);

			Commands->Action_Play_Animation(obj, params);
		}
		else if (action_id == 49)
		{
			if (!Commands->Find_Object(116386))// gdiPOW01Obj
			{
				GameObject *M01GDIBasePOWConversationControllerJDGObj = Commands->Find_Object(121237);
				if (M01GDIBasePOWConversationControllerJDGObj)
				{
					Commands->Send_Custom_Event(obj, M01GDIBasePOWConversationControllerJDGObj, 0, 16, 0.0f);
				}
			}
		}
		else if (action_id == 39)
		{
			GameObject *M01GDIBaseEvacMonitorJDGObj = Commands->Find_Object(103419);
			if (M01GDIBaseEvacMonitorJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01GDIBaseEvacMonitorJDGObj, 0, 4001, 0.0f);
			}

			GameObject *M01GDIBasePOWEncounter02ControllerJDGObj = Commands->Find_Object(103331);
			if (M01GDIBasePOWEncounter02ControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01GDIBasePOWEncounter02ControllerJDGObj, 0, 10, 0.0f);
			}
		}
	}
}

void M01_Base_POW02_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "S_A_Human.XG_EV5_troop"))
	{
		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_Base_POW02_JDG> M01_Base_POW02_JDGRegistrant("M01_Base_POW02_JDG", "");