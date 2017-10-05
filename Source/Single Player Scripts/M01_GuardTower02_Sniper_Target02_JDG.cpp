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
#include "M01_GuardTower02_Sniper_Target02_JDG.h"

void M01_GuardTower02_Sniper_Target02_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->guardTower2SniperTarget1ObjId, sizeof(this->guardTower2SniperTarget1ObjId), 1);
	Auto_Save_Variable(&this->canSendCustomToSniperTarget1, sizeof(this->canSendCustomToSniperTarget1), 2);
}

// When M01_GuardTower02_Sniper_Target01_JDG is created
void M01_GuardTower02_Sniper_Target02_JDG::Created(GameObject *obj)
{
	this->canSendCustomToSniperTarget1 = true;

	Commands->Enable_Hibernation(obj, false);

	ActionParamsStruct params;
	params.Set_Basic(this, 80.0f, 38);
	params.Set_Movement(Vector3(-92.927002f, 115.587f, 1.847f), 0.3f, 1.0f);

	Commands->Action_Goto(obj, params);
}

void M01_GuardTower02_Sniper_Target02_JDG::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *M01GuardTower02SniperTowerZoneJDGObj = Commands->Find_Object(123645);
	if (M01GuardTower02SniperTowerZoneJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01GuardTower02SniperTowerZoneJDGObj, 0, 22, 0.0f);
	}

	if (this->canSendCustomToSniperTarget1)
	{
		GameObject *guardTower2SniperTarget1Obj = Commands->Find_Object(this->guardTower2SniperTarget1ObjId);
		if (guardTower2SniperTarget1Obj)
		{
			Commands->Send_Custom_Event(obj, guardTower2SniperTarget1Obj, 0, 22, 0.0f);
		}
	}
}

void M01_GuardTower02_Sniper_Target02_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		// Received from M01_GuardTower02_Sniper_Target01_JDG when conversation ended
		if (param == 16)
		{
			this->canSendCustomToSniperTarget1 = false;

			Commands->Set_Innate_Is_Stationary(obj, false);

			ActionParamsStruct params;
			params.Set_Basic(this, 50.0f, 38);
			params.Set_Movement(Vector3(-79.399f, 112.473f, 2.373f), 0.3f, 1.0f);

			Commands->Action_Goto(obj, params);
		}

		// Received from M01_GuardTower02_Sniper_Target01_JDG when killed
		else if (param == 22)
		{
			Commands->Action_Reset(obj, 100.0f);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 46);
			params.Set_Animation("H_A_A0A0_L21", false);

			Commands->Action_Play_Animation(obj, params);
		}

		// Received from M01_GuardTower02_Sniper_Target01_JDG after 1 second when created
		else if (param == 14)
		{
			this->guardTower2SniperTarget1ObjId = Commands->Get_ID(sender);
		}
	}
}

void M01_GuardTower02_Sniper_Target02_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_NORMAL)
	{
		// When movement complete, see create or param 16
		if (action_id == 38)
		{
			Commands->Set_Innate_Is_Stationary(obj, true);
		}

		// When animation complete, see param 22
		else if (action_id == 46)
		{
			Commands->Set_Innate_Is_Stationary(obj, false);

			Vector3 pos = Commands->Get_Position(obj);
			Commands->Set_Innate_Soldier_Home_Location(obj, pos, 5.0f);
		}
	}
}

ScriptRegistrant<M01_GuardTower02_Sniper_Target02_JDG> M01_GuardTower02_Sniper_Target02_JDGRegistrant("M01_GuardTower02_Sniper_Target02_JDG", "");