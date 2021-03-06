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
#include "M01_Base_POW01_JDG.h"

/*
M01 -> 116386
*/
void M01_Base_POW01_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->powNodSoldierDeathCount, sizeof(this->powNodSoldierDeathCount), 1);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 2);
}

void M01_Base_POW01_JDG::Created(GameObject *obj)
{
	this->powNodSoldierDeathCount = 0;

	Commands->Set_Obj_Radar_Blip_Color(obj, RADAR_BLIP_COLOR_GDI);
	Commands->Set_Loiters_Allowed(obj, false);

	ActionParamsStruct params;
	params.Set_Basic(this, 100.0f, 46);
	params.Set_Animation("H_A_HOST_L2B", true);

	Commands->Action_Play_Animation(obj, params);

	this->field_20 = false;
}

void M01_Base_POW01_JDG::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *M01GDIBasePOWEncounter02ControllerJDGObj = Commands->Find_Object(103331);
	if (M01GDIBasePOWEncounter02ControllerJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01GDIBasePOWEncounter02ControllerJDGObj, 0, 22, 0.0f);
	}
}

void M01_Base_POW01_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (Commands->Get_Health(obj) <= 10.0f)
	{
		Commands->Set_Health(obj, 10.0f);
	}
}

void M01_Base_POW01_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		// Received from M01_POW_Nod_Minigunner01_JDG when killed
		if (param == 22)
		{
			if (++this->powNodSoldierDeathCount == 2)
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 47);
				params.Set_Animation("H_A_HOST_L2C", false);

				Commands->Action_Play_Animation(obj, params);

				GameObject *gdiPOW2Obj = Commands->Find_Object(116385);
				if (gdiPOW2Obj)
				{
					Commands->Send_Custom_Event(obj, gdiPOW2Obj, 0, 16, 1.0f);
				}

				GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(55.062f, 60.164001f, -1.0f));
				Commands->Set_Facing(invisObj, -45.0f);
				Commands->Attach_Script(invisObj, "Test_Cinematic", "X1D_GDIPOW_FlareDrop.txt");
			}
		}

		// Received from M01_GDIBase_POWEncounter02_Controller_JDG after 3 seconds when animation complete
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

		// Received from M01_GDIBase_POW_Conversation_Controller_JDG when custom type <= 0 and param 16 is received
		else if (param == 25)
		{
			this->field_20 = false;

			Commands->Enable_HUD_Pokable_Indicator(obj, false);
		}
	}
}

void M01_Base_POW01_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_NORMAL)
	{
		// When animation complete, see param 22
		if (action_id == 47)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 48);
			params.Set_Animation("H_A_HOST_L1C", false);

			Commands->Action_Play_Animation(obj, params);

			Commands->Set_Innate_Is_Stationary(obj, true);
		}

		// When animation complete, see param 47
		else if (action_id == 48)
		{
			GameObject *M01GDIBasePOWConversationControllerJDGObj = Commands->Find_Object(121237);
			if (M01GDIBasePOWConversationControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01GDIBasePOWConversationControllerJDGObj, 0, 16, 0.0f);
			}
		}

		// When moved to stationary location, see param 4002
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

void M01_Base_POW01_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "S_A_Human.XG_EV5_troop"))
	{
		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_Base_POW01_JDG> M01_Base_POW01_JDGRegistrant("M01_Base_POW01_JDG", "");