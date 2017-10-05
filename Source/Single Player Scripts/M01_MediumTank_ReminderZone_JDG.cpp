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
#include "M01_MediumTank_ReminderZone_JDG.h"

/*
M01 -> 122848
*/
void M01_MediumTank_ReminderZone_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->field_1D, sizeof(this->field_1D), 2);
	Auto_Save_Variable(&this->objId, sizeof(this->objId), 3);
	Auto_Save_Variable(&this->getInTankConversationIndex, sizeof(this->getInTankConversationIndex), 4);
}

void M01_MediumTank_ReminderZone_JDG::Created(GameObject *obj)
{
	this->field_1C = false;
	this->field_1D = false;
	this->getInTankConversationIndex = 0;
}

// TODO
void M01_MediumTank_ReminderZone_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		// Received from M01_Medium_Tank_JDG when created
		if (param == 16)
		{
			this->field_1C = true;

			Commands->Start_Timer(obj, this, 10.0f, 0);

			this->objId = Commands->Get_ID(obj); // Yes, for some reason it is storing this object's id
		}

		// Received from M01_Medium_Tank_JDG when entered
		else if (param == 27)
		{
			this->field_1D = true;
		}

		// Received from M01_Medium_Tank_JDG when exited
		else if (param == 28)
		{
			this->field_1D = false;
		}
	}
}

// TODO
void M01_MediumTank_ReminderZone_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED && action_id == this->getInTankConversationId)
	{
		Commands->Start_Timer(obj, this, 30.0f, 0);
	}
}

// TODO
void M01_MediumTank_ReminderZone_JDG::Timer_Expired(GameObject *obj, int number)
{
	if (!this->field_1D)
	{
		this->getInTankConversationIndex++;
		if (Commands->Find_Object(this->objId))
		{
			const char *conversationName = NULL;
			if (this->getInTankConversationIndex == 1)
			{
				conversationName = "M01_GetInTheTank_01_Conversation"; // Your tank should be on the beach by now, Havoc.  It's all yours.
			}
			else if (this->getInTankConversationIndex == 2)
			{
				conversationName = "M01_GetInTheTank_02_Conversation"; // Havoc, you don't HAVE to take the tank, but it's standing by if you want it.
			}

			if (conversationName != NULL)
			{
				this->getInTankConversationId = Commands->Create_Conversation(conversationName, 100, 30.0f, false);
				Commands->Join_Conversation(NULL, this->getInTankConversationId, false, true, true);
				Commands->Start_Conversation(this->getInTankConversationId, this->getInTankConversationId);
				Commands->Monitor_Conversation(obj, this->getInTankConversationId);
			}
		}
	}
}

void M01_MediumTank_ReminderZone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos) && this->field_1C && !this->field_1D)
	{
		int conversationId = Commands->Create_Conversation("M01_TunnelTank_Reminder_Conversation", 100, 30.0f, false); // Havoc, I wouldn't take that road without a tank.
		Commands->Join_Conversation(NULL, conversationId, false, true, true);
		Commands->Start_Conversation(conversationId, conversationId);

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_MediumTank_ReminderZone_JDG> M01_MediumTank_ReminderZone_JDGRegistrant("M01_MediumTank_ReminderZone_JDG", "");