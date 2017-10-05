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
#include "M01_Church_Priest_JDG.h"

/*
M01 -> 101315
*/
void M01_Church_Priest_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->priestLuckyCharmsConversationId, sizeof(this->priestLuckyCharmsConversationId), 1);
	Auto_Save_Variable(&this->soundId, sizeof(this->soundId), 1);
}

void M01_Church_Priest_JDG::Created(GameObject *obj)
{
	Commands->Innate_Disable(obj);

	Vector3 pos = Commands->Get_Position(obj);
	this->soundId = Commands->Create_Sound("MX1DSGN_DSGN0020I1DSGN_SND", pos, obj);
}

void M01_Church_Priest_JDG::Killed(GameObject *obj, GameObject *killer)
{
	Commands->Stop_Sound(this->soundId, true);

	Commands->Create_Sound("EVA_Civilian_Killed", Vector3(0.0f, 0.0f, 0.0f), obj);

	GameObject *M01ChurchEvacControllerJDGObj = Commands->Find_Object(103394);
	Commands->Send_Custom_Event(obj, M01ChurchEvacControllerJDGObj, 0, 229, 0.0f);
}

// TODO
void M01_Church_Priest_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (param == 16)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 90.0f, 46);
		params.Set_Animation("H_A_Host_L2b", true);

		Commands->Action_Play_Animation(obj, params);

		Commands->Start_Sound(this->soundId);
	}
	else if (param == 173)
	{
		Commands->Stop_Sound(this->soundId, true);
		Commands->Enable_Hibernation(obj, false);

		ActionParamsStruct params;
		params.Set_Basic(this, 90.0f, 46);
		params.Set_Animation("H_A_Host_L2c", false);

		Commands->Action_Play_Animation(obj, params);
	}

	// Received from M01_ChurchArea_Air_Evac_Chopper_JDG when animation complete
	else if (param == 4001)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);
		Vector3 starPos = Commands->Get_Position(starObj);

		if (Commands->Get_Distance(pos, starPos) > 15.0f)
		{
			GameObject *M01ChurchEvacControllerJDGObj = Commands->Find_Object(103394);
			Vector3 M01ChurchEvacControllerJDGObjPos = Commands->Get_Position(M01ChurchEvacControllerJDGObj);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 4001);
			params.Set_Movement(M01ChurchEvacControllerJDGObjPos, 0.8f, 0.5f);

			Commands->Action_Goto(obj, params);
		}
		else
		{
			// GDI - I've been saving this for a special occasion. This seems to be it.
			this->priestLuckyCharmsConversationId = Commands->Create_Conversation("M01_Priest_LuckyCharms_Conversation", 100, 1000.0f, false);
			Commands->Join_Conversation(obj, this->priestLuckyCharmsConversationId, false, true, true);
			Commands->Join_Conversation(starObj, this->priestLuckyCharmsConversationId, false, false, false);
			Commands->Start_Conversation(this->priestLuckyCharmsConversationId, this->priestLuckyCharmsConversationId);
			Commands->Monitor_Conversation(obj, this->priestLuckyCharmsConversationId);
		}
	}
}

// TODO
void M01_Church_Priest_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED)
	{
		if (action_id == this->priestLuckyCharmsConversationId)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 49);
			params.Set_Animation("H_A_J12C", false);

			Commands->Action_Play_Animation(obj, params);

			Vector3 dataDiscPos = Commands->Get_Position(obj);
			dataDiscPos.Z += 0.75f;

			float randXOffset = Commands->Get_Random(-2.0f, 2.0f);
			float randYOffset = Commands->Get_Random(-2.0f, 2.0f);

			dataDiscPos.X += randXOffset;
			dataDiscPos.Y += randYOffset;
				
			GameObject *dataDisc = Commands->Create_Object("POW_Data_Disc", dataDiscPos);
			Commands->Attach_Script(dataDisc, "M01_Priest_Datadisc_JDG", "");

		}
	}
	else if(complete_reason == ACTION_COMPLETE_NORMAL)
	{
		if (action_id == 46)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 90.0f, 47);
			params.Set_Animation("H_A_Host_L1b", false);

			Commands->Action_Play_Animation(obj, params);
		}
		else if (action_id == 47)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 90.0f, 48);
			params.Set_Animation("H_A_Host_L1c", false);

			Commands->Action_Play_Animation(obj, params);
		}
		else if (action_id == 48)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 90.0f, 39);
			params.Set_Movement(Vector3(-135.571f, 340.21301f, 9.608f), 0.9f, 0.5f);

			Commands->Action_Goto(obj, params);
		}
		else if (action_id == 39)
		{
			Commands->Enable_Hibernation(obj, true);

			GameObject *M01PriestConversationZoneJDGObj = Commands->Find_Object(103395);
			if (M01PriestConversationZoneJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01PriestConversationZoneJDGObj, 0, 16, 0.0f);
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
		else if (action_id == 49)
		{
			GameObject *M01ChurchEvacControllerJDGObj = Commands->Find_Object(103394);
			Vector3 M01ChurchEvacControllerJDGObjPos = Commands->Get_Position(M01ChurchEvacControllerJDGObj);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 4001);
			params.Set_Movement(M01ChurchEvacControllerJDGObjPos, 0.8f, 0.5f);
			
			Commands->Action_Goto(obj, params);
		}
	}
}

void M01_Church_Priest_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "S_A_Human.XG_EV5_troop"))
	{
		Commands->Destroy_Object(obj);
	}
}

void M01_Church_Priest_JDG::Poked(GameObject *obj, GameObject *poker)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (poker == Commands->Get_A_Star(pos))
	{
		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 213, 0.0f);
	}
}

ScriptRegistrant<M01_Church_Priest_JDG> M01_Church_Priest_JDGRegistrant("M01_Church_Priest_JDG", "");