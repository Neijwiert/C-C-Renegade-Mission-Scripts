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
#include "M01_GuardTower02_Sniper_Target01_JDG.h"

void M01_GuardTower02_Sniper_Target01_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->sniperTargetNodMinigunnerObjId, sizeof(this->sniperTargetNodMinigunnerObjId), 1);
	Auto_Save_Variable(&this->conversationId, sizeof(this->conversationId), 2);
	Auto_Save_Variable(&this->field_24, sizeof(this->field_24), 3);
}

// When M01_GDI_GuardTower02_SniperRifle_JDG is picked up
void M01_GuardTower02_Sniper_Target01_JDG::Created(GameObject *obj)
{
	this->field_24 = true;

	Commands->Enable_Hibernation(obj, false);

	GameObject *sniperTargetNodMinigunnerObj = Commands->Create_Object("Nod_MiniGunner_0", Vector3(-93.882f, 110.504f, 1.644f));
	Commands->Attach_Script(sniperTargetNodMinigunnerObj, "M01_GuardTower02_Sniper_Target02_JDG", "");
	Commands->Send_Custom_Event(obj, sniperTargetNodMinigunnerObj, 0, 14, 1.0f);
	this->sniperTargetNodMinigunnerObjId = Commands->Get_ID(sniperTargetNodMinigunnerObj);

	ActionParamsStruct params;
	params.Set_Basic(this, 80.0f, 38);
	params.Set_Movement(Vector3(-92.496002f, 117.75f, 1.752f), 0.8f, 1.0f);

	Commands->Action_Goto(obj, params);
}

void M01_GuardTower02_Sniper_Target01_JDG::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *M01GuardTower02SniperTowerZoneJDGObj = Commands->Find_Object(123645);
	if (M01GuardTower02SniperTowerZoneJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01GuardTower02SniperTowerZoneJDGObj, 0, 22, 0.0f);
	}

	if (this->field_24)
	{
		GameObject *sniperTargetNodMinigunnerObj = Commands->Find_Object(this->sniperTargetNodMinigunnerObjId);
		if (sniperTargetNodMinigunnerObj)
		{
			Commands->Send_Custom_Event(obj, sniperTargetNodMinigunnerObj, 0, 22, 0.0f);
		}
	}
}

// TODO
void M01_GuardTower02_Sniper_Target01_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type && param == 22)
	{
		Commands->Action_Reset(obj, 100.0f);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 46);
		params.Set_Animation("H_A_A0A0_L21", false);

		Commands->Action_Play_Animation(obj, params);
	}
}

// TODO
void M01_GuardTower02_Sniper_Target01_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED)
	{
		if (action_id == this->conversationId)
		{
			this->field_24 = false;

			Commands->Set_Innate_Is_Stationary(obj, false);

			ActionParamsStruct params;
			params.Set_Basic(this, 50.0f, 39);
			params.Set_Movement(Vector3(46.749001f, 197.062f, 6.211f), 0.8f, 1.0f);

			Commands->Action_Goto(obj, params);

			GameObject *sniperTargetNodMinigunnerObj = Commands->Find_Object(this->sniperTargetNodMinigunnerObjId);
			if (sniperTargetNodMinigunnerObj)
			{
				Commands->Send_Custom_Event(obj, sniperTargetNodMinigunnerObj, 0, 16, 0.0f);
			}
		}
	}
	else if (complete_reason == ACTION_COMPLETE_NORMAL)
	{
		if (action_id == 38)
		{
			Commands->Set_Innate_Is_Stationary(obj, true);

			GameObject *sniperTargetNodMinigunnerObj = Commands->Find_Object(this->sniperTargetNodMinigunnerObjId);
			if (sniperTargetNodMinigunnerObj)
			{
				// Civillian maggots, they're all GDI sympothisers
				// Kane's reconditioning camp will set them straight
				// I need to move those prisoners out - protect the turrets, Brother!
				// Understood. Repairs are underway. See you back at the Hand.
				this->conversationId = Commands->Create_Conversation("M01_SniperTower_Conversation", 100, 80.0f, true);
				Commands->Join_Conversation(obj, this->conversationId, false, true, true);
				Commands->Join_Conversation(sniperTargetNodMinigunnerObj, this->conversationId, false, true, true);
				Commands->Start_Conversation(this->conversationId, this->conversationId);
				Commands->Monitor_Conversation(obj, this->conversationId);
			}
		}
		else if (action_id == 39)
		{
			Commands->Enable_Hibernation(obj, true);
		}
		else if (action_id == 46)
		{
			Commands->Set_Innate_Is_Stationary(obj, false);

			Vector3 pos = Commands->Get_Position(obj);
			Commands->Set_Innate_Soldier_Home_Location(obj, pos, 5.0f);
		}
	}
}

ScriptRegistrant<M01_GuardTower02_Sniper_Target01_JDG> M01_GuardTower02_Sniper_Target01_JDGRegistrant("M01_GuardTower02_Sniper_Target01_JDG", "");