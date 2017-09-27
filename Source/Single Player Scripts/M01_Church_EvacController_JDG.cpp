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
#include "M01_Church_EvacController_JDG.h"

/*
M01 -> 103394
*/
void M01_Church_EvacController_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->waypathObjId, sizeof(this->waypathObjId), 1);
	Auto_Save_Variable(&this->chopperObjId, sizeof(this->chopperObjId), 2);
	Auto_Save_Variable(&this->ropeObjId, sizeof(this->ropeObjId), 3);
	Auto_Save_Variable(&this->protectClergyConversationId, sizeof(this->protectClergyConversationId), 4);
	Auto_Save_Variable(&this->field_2C, sizeof(this->field_2C), 5);
	Auto_Save_Variable(&this->field_30, sizeof(this->field_30), 6);
}

void M01_Church_EvacController_JDG::Created(GameObject *obj)
{
	this->field_2C = 0;
	this->field_30 = false;
}

// TODO
void M01_Church_EvacController_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (param == 16)
	{
		// We're not done - Nod is closing in. Protect the civilians until we can evacuate!
		// I'm sending reinforcements, contact them for backup.
		this->protectClergyConversationId = Commands->Create_Conversation("M01_Protect_Clergy_Conversation", 100, 1000.0f, false);
		Commands->Join_Conversation(NULL, this->protectClergyConversationId, false, false, true);
		Commands->Start_Conversation(this->protectClergyConversationId, this->protectClergyConversationId);
		Commands->Monitor_Conversation(obj, this->protectClergyConversationId);
	}
	else if (param == 15)
	{
		GameObject *chopperObj = Commands->Find_Object(this->chopperObjId);
		GameObject *waypathObj = Commands->Find_Object(this->waypathObjId);
		GameObject *ropeObj = Commands->Find_Object(this->ropeObjId);

		if (chopperObj)
		{
			Commands->Send_Custom_Event(obj, chopperObj, 0, 27, 0.0f);
		}

		if (waypathObj)
		{
			Commands->Send_Custom_Event(obj, waypathObj, 0, 27, 0.0f);
		}

		if (ropeObj)
		{
			Commands->Send_Custom_Event(obj, ropeObj, 0, 27, 0.0f);
		}
	}
	else if (param == 229)
	{
		if (++this->field_2C == 3)
		{
			GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
			Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 95, 0.0f);
		}
	}
	else if (param == 14)
	{
		this->chopperObjId = Commands->Get_ID(sender);

		Commands->Debug_Message("**********************chopper id received.....%d\n", this->chopperObjId);
	}
	else if (param == 13)
	{
		this->waypathObjId = Commands->Get_ID(sender);

		Commands->Debug_Message("**********************waypath id received.....%d\n", this->waypathObjId);
	}
	else if (param == 12)
	{
		this->ropeObjId = Commands->Get_ID(sender);

		Commands->Debug_Message("**********************rope id received.....%d\n", this->ropeObjId);
	}
	else if (param == 10)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *troopBoneObj = Commands->Create_Object("Invisible_Object", pos);
		Commands->Set_Model(troopBoneObj, "XG_EV5_troopBN");
		Commands->Attach_Script(troopBoneObj, "M01_Evac_TroopBone_JDG", "");
		Commands->Innate_Disable(sender);
		Commands->Attach_To_Object_Bone(sender, troopBoneObj, "Troop_L");

		if (troopBoneObj)
		{
			Commands->Set_Animation(troopBoneObj, "XG_EV5_troopBN.XG_EV5_troopBN", false, NULL, 0.0f, -1.0f, false);
		}
		else
		{
			Commands->Debug_Message("troopbone not created yet");
		}

		Commands->Set_Animation(sender, "S_A_Human.XG_EV5_troop", false, NULL, 0.0f, -1.0f, false);
	}
}

// TODO
void M01_Church_EvacController_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED && action_id == this->protectClergyConversationId)
	{
		if (!this->field_30)
		{
			this->field_30 = true;

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *invisObj = Commands->Create_Object("Invisible_Object", pos);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "XG_M01_ChurchArea_EvacAnim.txt");
		}

		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-140.9f, 320.2f, 8.5f));
		Commands->Set_Facing(invisObj, -54.0f);
		Commands->Attach_Script(invisObj, "Test_Cinematic", "X1I_GDI_Church_TroopDrop.txt");

		Commands->Create_Sound("00-N180E", Vector3(0.0f, 0.0f, 0.0f), obj);
	}
}

ScriptRegistrant<M01_Church_EvacController_JDG> M01_Church_EvacController_JDGRegistrant("M01_Church_EvacController_JDG", "");