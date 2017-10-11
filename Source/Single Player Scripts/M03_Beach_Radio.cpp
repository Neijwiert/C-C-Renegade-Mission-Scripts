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
#include "M03_Beach_Radio.h"

/*
M03 -> 1145036
*/
void M03_Beach_Radio::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->beachRadioConversationIndex, sizeof(this->beachRadioConversationIndex), 1);
	Auto_Save_Variable(&this->doingConversation, sizeof(this->doingConversation), 2);
	Auto_Save_Variable(&this->beachRadioConversationId, sizeof(this->beachRadioConversationId), 3);
}

void M03_Beach_Radio::Created(GameObject *obj)
{
	this->beachRadioConversationIndex = 0;
	this->doingConversation = false;
	this->beachRadioConversationId = 0;
}

void M03_Beach_Radio::Killed(GameObject *obj, GameObject *killer)
{
	Commands->Stop_Conversation(this->beachRadioConversationId);
}

void M03_Beach_Radio::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED)
	{
		this->doingConversation = false;
	}
}

void M03_Beach_Radio::Poked(GameObject *obj, GameObject *poker)
{
	if (this->beachRadioConversationIndex <= 2 && !this->doingConversation)
	{
		static const char *beachRadioConversations[3] =
		{
			"M03CON044", // GDI forces are closing on the southern shores, send reinforcements to my coordinates!
			"M03CON045", // I can't get a lock on your position, please respond!
			"M03CON046" // Any available units converge on the base facility, we need backup!
		};

		this->beachRadioConversationId = Commands->Create_Conversation(beachRadioConversations[this->beachRadioConversationIndex++], 0, 0.0f, true);
		Commands->Join_Conversation(NULL, this->beachRadioConversationId, true, true, true);
		Commands->Join_Conversation(poker, this->beachRadioConversationId, true, true, true);
		Commands->Start_Conversation(this->beachRadioConversationId, 0);
		Commands->Monitor_Conversation(obj, this->beachRadioConversationId);

		this->doingConversation = true;
	}
}

ScriptRegistrant<M03_Beach_Radio> M03_Beach_RadioRegistrant("M03_Beach_Radio", "");