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
#include "M01_HON_Dojo_Trainer_JDG.h"

/*
M01 -> 102051
*/
void M01_HON_Dojo_Trainer_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->dojoSenseiConversationId, sizeof(this->dojoSenseiConversationId), 1);
	Auto_Save_Variable(&this->notDamagedByStar, sizeof(this->notDamagedByStar), 2);
	Auto_Save_Variable(&this->honKilled, sizeof(this->honKilled), 3);
}

void M01_HON_Dojo_Trainer_JDG::Created(GameObject *obj)
{
	this->honKilled = false;
	this->notDamagedByStar = true;

	Commands->Innate_Disable(obj);
}

void M01_HON_Dojo_Trainer_JDG::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *dojoCiv01Obj = Commands->Find_Object(102048);
	GameObject *dojoCiv02Obj = Commands->Find_Object(102049);
	GameObject *dojoCiv03Obj = Commands->Find_Object(102050);

	if (this->honKilled)
	{
		if (dojoCiv01Obj)
		{
			float randDelay = Commands->Get_Random(0.0f, 1.0f);
			Commands->Send_Custom_Event(obj, dojoCiv01Obj, 0, 165, randDelay);
		}

		if (dojoCiv02Obj)
		{
			float randDelay = Commands->Get_Random(0.0f, 1.0f);
			Commands->Send_Custom_Event(obj, dojoCiv02Obj, 0, 165, randDelay);
		}

		if (dojoCiv03Obj)
		{
			float randDelay = Commands->Get_Random(0.0f, 1.0f);
			Commands->Send_Custom_Event(obj, dojoCiv03Obj, 0, 165, randDelay);
		}
	}
	else
	{
		if (dojoCiv01Obj)
		{
			float randDelay = Commands->Get_Random(0.0f, 1.0f);
			Commands->Send_Custom_Event(obj, dojoCiv01Obj, 0, 28, randDelay);
		}

		if (dojoCiv02Obj)
		{
			float randDelay = Commands->Get_Random(0.0f, 1.0f);
			Commands->Send_Custom_Event(obj, dojoCiv02Obj, 0, 28, randDelay);
		}

		if (dojoCiv03Obj)
		{
			float randDelay = Commands->Get_Random(0.0f, 1.0f);
			Commands->Send_Custom_Event(obj, dojoCiv03Obj, 0, 28, randDelay);
		}
	}
}

void M01_HON_Dojo_Trainer_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (damager == Commands->Get_A_Star(pos) && this->notDamagedByStar)
	{
		this->notDamagedByStar = false;

		GameObject *dojoCiv01Obj = Commands->Find_Object(102048);
		if (dojoCiv01Obj)
		{
			float randDelay = Commands->Get_Random(0.0f, 1.0f);
			Commands->Send_Custom_Event(obj, dojoCiv01Obj, 0, 189, randDelay);
		}

		GameObject *dojoCiv02Obj = Commands->Find_Object(102049);
		if (dojoCiv02Obj)
		{
			float randDelay = Commands->Get_Random(0.0f, 1.0f);
			Commands->Send_Custom_Event(obj, dojoCiv02Obj, 0, 189, randDelay);
		}

		GameObject *dojoCiv03Obj = Commands->Find_Object(102050);
		if (dojoCiv03Obj)
		{
			float randDelay = Commands->Get_Random(0.0f, 1.0f);
			Commands->Send_Custom_Event(obj, dojoCiv03Obj, 0, 189, randDelay);
		}
	}
}

void M01_HON_Dojo_Trainer_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from ourselves after 5 seconds when param 16 is received
	if (param == 27)
	{
		Commands->Innate_Soldier_Enable_Enemy_Seen(obj, true);
		Commands->Innate_Soldier_Enable_Footsteps_Heard(obj, true);
	}

	// Received from M01_mission_Controller_JDG when param 122 is received
	else if (param == 165)
	{
		this->honKilled = true;

		Commands->Innate_Enable(obj);
		Commands->Innate_Soldier_Enable_Enemy_Seen(obj, true);
		Commands->Innate_Soldier_Enable_Footsteps_Heard(obj, true);
	}

	// Received from M01_mission_Controller_JDG when param 137 is received
	else if (param == 16)
	{
		Commands->Set_Innate_Aggressiveness(obj, 0.5f);
		Commands->Set_Innate_Take_Cover_Probability(obj, 0.5f);
		Commands->Innate_Soldier_Enable_Enemy_Seen(obj, false);
		Commands->Innate_Soldier_Enable_Footsteps_Heard(obj, false);

		// All right recruits! Give me five more and report to the armory! Is that clear?
		this->dojoSenseiConversationId = Commands->Create_Conversation("M01_Dojo_Sensei_Conversation", 100, 1000.0f, true);
		Commands->Join_Conversation(obj, this->dojoSenseiConversationId, false, true, true);
		Commands->Start_Conversation(this->dojoSenseiConversationId, this->dojoSenseiConversationId);
		Commands->Monitor_Conversation(obj, this->dojoSenseiConversationId);

		Commands->Send_Custom_Event(obj, obj, 0, 27, 5.0f);
	}
}

void M01_HON_Dojo_Trainer_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED && action_id == this->dojoSenseiConversationId)
	{
		GameObject *dojoCiv01Obj = Commands->Find_Object(102048);
		if (dojoCiv01Obj)
		{
			float randDelay = Commands->Get_Random(0.0f, 1.0f);
			Commands->Send_Custom_Event(obj, dojoCiv01Obj, 0, 16, randDelay);
		}

		GameObject *dojoCiv02Obj = Commands->Find_Object(102049);
		if (dojoCiv02Obj)
		{
			float randDelay = Commands->Get_Random(0.0f, 1.0f);
			Commands->Send_Custom_Event(obj, dojoCiv02Obj, 0, 16, randDelay);
		}

		GameObject *dojoCiv03Obj = Commands->Find_Object(102050);
		if (dojoCiv03Obj)
		{
			float randDelay = Commands->Get_Random(0.0f, 1.0f);
			Commands->Send_Custom_Event(obj, dojoCiv03Obj, 0, 16, randDelay);
		}
	}
}

ScriptRegistrant<M01_HON_Dojo_Trainer_JDG> M01_HON_Dojo_Trainer_JDGRegistrant("M01_HON_Dojo_Trainer_JDG", "");