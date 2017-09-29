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
#include "M01_Church_Interior_Nun_JDG.h"

/*
M01 -> 101314
*/
void M01_Church_Interior_Nun_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->soundId, sizeof(this->soundId), 2);
}

void M01_Church_Interior_Nun_JDG::Created(GameObject *obj)
{
	Commands->Innate_Disable(obj);

	this->field_1C = false;

	Vector3 pos = Commands->Get_Position(obj);
	this->soundId = Commands->Create_Sound("MX1DSGN_DSGN0021I1DSGN_SND", pos, obj);
}

void M01_Church_Interior_Nun_JDG::Killed(GameObject *obj, GameObject *killer)
{
	Commands->Stop_Sound(this->soundId, true);

	Commands->Create_Sound("EVA_Civilian_Killed", Vector3(0.0f, 0.0f, 0.0f), obj);

	GameObject *M01ChurchEvacControllerJDGObj = Commands->Find_Object(103394);
	Commands->Send_Custom_Event(obj, M01ChurchEvacControllerJDGObj, 0, 229, 0.0f);
}

// TODO
void M01_Church_Interior_Nun_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (param == 16)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 90.0f, 0);
		params.Set_Animation("S_A_Human.H_A_Host_L2b", true);

		Commands->Action_Play_Animation(obj, params);

		Commands->Start_Sound(this->soundId);
	}
	else if (param == 173)
	{
		Commands->Stop_Sound(this->soundId, true);
		Commands->Enable_Hibernation(obj, 0);

		ActionParamsStruct params;
		params.Set_Basic(this, 90.0f, 46);
		params.Set_Animation("S_A_Human.H_A_Host_L2c", false);

		Commands->Action_Play_Animation(obj, params);
	}
	else if (param == 4001)
	{
		GameObject *M01ChurchEvacControllerJDGObj = Commands->Find_Object(103394);
		Vector3 M01ChurchEvacControllerJDGObjPos = Commands->Get_Position(M01ChurchEvacControllerJDGObj);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 4001);
		params.Set_Movement(M01ChurchEvacControllerJDGObjPos, 0.8f, 1.0f);

		Commands->Action_Goto(obj, params);
	}
}

// TODO
void M01_Church_Interior_Nun_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_NORMAL)
	{
		if (action_id == 46)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 47);
			params.Set_Animation("S_A_Human.H_A_Host_L1b", false);

			Commands->Action_Play_Animation(obj, params);
		}
		else if (action_id == 47)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 48);
			params.Set_Animation("S_A_Human.H_A_Host_L1c", false);

			Commands->Action_Play_Animation(obj, params);

			Vector3 pos = Commands->Get_Position(obj);
			Commands->Create_Sound("MX1DSGN_DSGN0022I1DSGN_SND", pos, obj);
		}
		else if (action_id == 48)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 39);
			params.Set_Movement(Vector3(-136.738f, 325.073f, 8.167f), 0.8f, 0.5f);

			Commands->Action_Goto(obj, params);
		}
		else if (action_id == 39)
		{
			Commands->Enable_Hibernation(obj, true);

			GameObject *M01InteriorNunConversationZoneJDGObj = Commands->Find_Object(101370);
			if (M01InteriorNunConversationZoneJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01InteriorNunConversationZoneJDGObj, 0, 16, 1.0f);
			}
		}
		else if (action_id == 4001)
		{
			GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
			Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 177, 1.0f);

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);
			Commands->Give_Points(starObj, 2000.0f, false);
			
			pos.Z += 0.75f;

			static const char *drops[2] = 
			{
				"POW_Health_100",
				"POW_Armor_100"
			};

			int randDropIndex = Commands->Get_Random_Int(0, 2);
			Commands->Create_Object(drops[randDropIndex], pos);

			GameObject *M01ChurchAreaEvacMonitorJDGObj = Commands->Find_Object(103421);
			if (M01ChurchAreaEvacMonitorJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01ChurchAreaEvacMonitorJDGObj, 0, 4001, 0.0f);
			}

			GameObject *M01ChurchEvacControllerJDGObj = Commands->Find_Object(103394);
			if (M01ChurchEvacControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01ChurchEvacControllerJDGObj, 0, 10, 0.0f);
			}
		}
	}
}

void M01_Church_Interior_Nun_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "S_A_Human.XG_EV5_troop"))
	{
		Commands->Destroy_Object(obj);
	}
}

void M01_Church_Interior_Nun_JDG::Poked(GameObject *obj, GameObject *poker)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (poker == Commands->Get_A_Star(pos))
	{
		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 211, 0.0f);
	}
}

ScriptRegistrant<M01_Church_Interior_Nun_JDG> M01_Church_Interior_Nun_JDGRegistrant("M01_Church_Interior_Nun_JDG", "");