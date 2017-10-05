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
#include "M01_BarnArea_EvacMonitor_JDG.h"

/*
M01 -> 103420
*/
void M01_BarnArea_EvacMonitor_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->chopperObjId, sizeof(this->chopperObjId), 2);
	Auto_Save_Variable(&this->waypathObjId, sizeof(this->waypathObjId), 3);
	Auto_Save_Variable(&this->ropeObjId, sizeof(this->ropeObjId), 4);
	Auto_Save_Variable(&this->field_2C, sizeof(this->field_2C), 5);
	Auto_Save_Variable(&this->airdropConversationId, sizeof(this->airdropConversationId), 6);
	Auto_Save_Variable(&this->field_34, sizeof(this->field_34), 7);
}

void M01_BarnArea_EvacMonitor_JDG::Created(GameObject *obj)
{
	this->field_1C = 0;
	this->field_2C = false;
	this->field_34 = false;
}

// TODO
void M01_BarnArea_EvacMonitor_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_Barn_Prisoner_01_JDG or M01_Barn_Prisoner_02_JDG or M01_Barn_Prisoner_03_JDG when killed
	if (param == 229)
	{
		GameObject *barnFemalePrisonerObj = Commands->Find_Object(101442);
		GameObject *billyObj = Commands->Find_Object(101444);
		GameObject *barnMalePrisonerPierreObj = Commands->Find_Object(101443);

		if (sender == barnFemalePrisonerObj || sender == billyObj || sender == barnMalePrisonerPierreObj)
		{
			this->field_34 = true;
		}
	}

	// Received from M01_Barn_Prisoner_01_JDG or M01_Barn_Prisoner_02_JDG or M01_Barn_Prisoner_03_JDG when action with id 4001 is complete
	else if (param == 4001)
	{
		if (++this->field_1C == 3 && !this->field_34)
		{
			this->airdropConversationId = Commands->Create_Conversation("M01_AirDrop_Conversation", 100, 1000.0f, false); // Nice work. I'm air-dropping supplies, look for them.
			Commands->Join_Conversation(NULL, this->airdropConversationId, false, false, true);
			Commands->Start_Conversation(this->airdropConversationId, this->airdropConversationId);
			Commands->Monitor_Conversation(obj, this->airdropConversationId);

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-0.923f, 143.58f, 0.647f));
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X1D_GDIPowerUpDrop.txt");
		}
	}

	// Received from M01_BarnArea_Air_Evac_Chopper_JDG when created
	else if (param == 14)
	{
		this->chopperObjId = Commands->Get_ID(sender);

		Commands->Debug_Message("**********************chopper id received.....%d\n", this->chopperObjId);
	}

	// Received from M01_BarnArea_Air_Evac_Waypath_JDG when created
	else if (param == 13)
	{
		this->waypathObjId = Commands->Get_ID(sender);

		Commands->Debug_Message("**********************waypath id received.....%d\n", this->waypathObjId);
	}

	// Received from M01_BarnArea_Air_Evac_Rope_JDG when created
	else if (param == 12)
	{
		this->ropeObjId = Commands->Get_ID(sender);

		Commands->Debug_Message("**********************rope id received.....%d\n", this->ropeObjId);
	}

	// Received from M01_Barn_Prisoner_01_JDG or M01_Barn_Prisoner_02_JDG or M01_Barn_Prisoner_03_JDG when action with id 4001 is complete
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
	else if (param == 75)
	{
		if (!this->field_2C)
		{
			this->field_2C = true;

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *invisObj = Commands->Create_Object("Invisible_Object", pos);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "XG_M01_BarnArea_EvacAnim.txt");
		}
	}

	// Received from M01_BarnArea_Air_Evac_Chopper_JDG when custom param 4 is received
	else if (param == 15)
	{
		Commands->Debug_Message("*******************************all barn civs on board--custom received by controller\n");

		GameObject *waypathObj = Commands->Find_Object(this->waypathObjId);
		if (waypathObj)
		{
			Commands->Send_Custom_Event(obj, waypathObj, 0, 27, 0.0f);
		}

		GameObject *chopperObj = Commands->Find_Object(this->chopperObjId);
		if (chopperObj)
		{
			Commands->Send_Custom_Event(obj, chopperObj, 0, 27, 0.0f);
		}

		GameObject *ropeObj = Commands->Find_Object(this->ropeObjId);
		if (chopperObj) // Not a mistake
		{
			Commands->Send_Custom_Event(obj, ropeObj, 0, 27, 0.0f);
		}

		if (this->field_34)
		{
			int conversationId = Commands->Create_Conversation("M01_HandOfNod_Alerted_Conversation", 100, 1000.0f, false); // That firefight alerted the Nod base. Destroy the Hand of Nod to stop further reinforcements.
			Commands->Join_Conversation(NULL, conversationId, false, false, true);
			Commands->Start_Conversation(conversationId, conversationId);
		}
	}
}

// TODO
void M01_BarnArea_EvacMonitor_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED && action_id == this->airdropConversationId)
	{
		int conversationId = Commands->Create_Conversation("M01_HandOfNod_Alerted_Conversation", 100, 1000.0f, false); // That firefight alerted the Nod base. Destroy the Hand of Nod to stop further reinforcements.
		Commands->Join_Conversation(NULL, conversationId, false, false, true);
		Commands->Start_Conversation(conversationId, conversationId);
	}
}

ScriptRegistrant<M01_BarnArea_EvacMonitor_JDG> M01_BarnArea_EvacMonitor_JDGRegistrant("M01_BarnArea_EvacMonitor_JDG", "");