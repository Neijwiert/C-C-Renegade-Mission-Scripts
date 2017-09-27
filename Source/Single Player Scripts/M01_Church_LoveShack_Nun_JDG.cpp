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
#include "M01_Church_LoveShack_Nun_JDG.h"

/*
M01 -> 101310
*/
void M01_Church_LoveShack_Nun_JDG::Created(GameObject *obj)
{
	Commands->Innate_Disable(obj);
}

void M01_Church_LoveShack_Nun_JDG::Killed(GameObject *obj, GameObject *killer)
{
	Commands->Create_Sound("EVA_Civilian_Killed", Vector3(0.0f, 0.0f, 0.0f), obj);

	GameObject *M01ChurchEvacControllerJDGObj = Commands->Find_Object(103394);
	Commands->Send_Custom_Event(obj, M01ChurchEvacControllerJDGObj, 0, 229, 0.0f);

	GameObject *M01ChurchLoveShackInterrogationConvZoneJDGObj = Commands->Find_Object(103392);
	if (M01ChurchLoveShackInterrogationConvZoneJDGObj)
	{
		Commands->Destroy_Object(M01ChurchLoveShackInterrogationConvZoneJDGObj);
	}
}

// TODO
void M01_Church_LoveShack_Nun_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (param == 16)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 90.0f, 38);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.3f, 0.25f);
		params.WaypathID = 100750;
		params.WaypointStartID = 100753;
		params.WaypointEndID = 100758;
		
		Commands->Action_Goto(obj, params);
	}
	else if (param == 174)
	{
		Commands->Enable_Hibernation(obj, false);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 39);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.8f, 1.0f);
		params.WaypathID = 103383;
		params.WaypointStartID = 103384;
		params.WaypointEndID = 103390;

		Commands->Action_Goto(obj, params);
	}
	else if (param == 4001)
	{
		GameObject *M01ChurchEvacControllerJDGObj = Commands->Find_Object(103394);
		Vector3 M01ChurchEvacControllerJDGObjPos = Commands->Get_Position(M01ChurchEvacControllerJDGObj);

		Commands->Action_Reset(obj, 100.0f);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 4001);
		params.Set_Movement(M01ChurchEvacControllerJDGObjPos, 0.8f, 1.0f);
		
		Commands->Action_Goto(obj, params);
	}
}

// TODO
void M01_Church_LoveShack_Nun_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_NORMAL)
	{
		if (action_id == 4001)
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
		else if (action_id == 39)
		{
			Commands->Enable_Hibernation(obj, true);

			GameObject *M01LoveShackNunConversationZoneJDGObj = Commands->Find_Object(101368);
			if (M01LoveShackNunConversationZoneJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01LoveShackNunConversationZoneJDGObj, 0, 16, 1.0f);
			}
		}
	}
}

void M01_Church_LoveShack_Nun_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "S_A_Human.XG_EV5_troop"))
	{
		Commands->Destroy_Object(obj);
	}
}

void M01_Church_LoveShack_Nun_JDG::Poked(GameObject *obj, GameObject *poker)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (poker == Commands->Get_A_Star(pos))
	{
		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event((ScriptableGameObj *)this, M01MissionControllerJDGObj, 0, 212, 0.0f);
	}
}

ScriptRegistrant<M01_Church_LoveShack_Nun_JDG> M01_Church_LoveShack_Nun_JDGRegistrant("M01_Church_LoveShack_Nun_JDG", "");