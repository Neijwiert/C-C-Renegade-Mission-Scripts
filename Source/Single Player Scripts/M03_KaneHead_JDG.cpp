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
#include "M03_KaneHead_JDG.h"

void M03_KaneHead_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
}

// When M03_Holograph_EntryZone_JDG receives custom type any with param 8000
void M03_KaneHead_JDG::Created(GameObject *obj)
{
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *starObj = Commands->Get_A_Star(pos);

	if (starObj)
	{
		// You are quite the pest, GDI.  Let me see... Ah, Captain Parker, code name Havoc. Hmm... how cliché.
		// <signature laugh>
		int conversationId = Commands->Create_Conversation("M03CON061", 99, 30.0f, true);
		Commands->Join_Conversation(obj, conversationId, true, true, true);
		Commands->Join_Conversation(starObj, conversationId, true, true, true);
		Commands->Start_Conversation(conversationId, 100061);
		Commands->Monitor_Conversation(obj, conversationId);
	}
}

void M03_KaneHead_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from ourselves after 2 seconds, see action complete
	if (param == 8000)
	{
		Commands->Debug_Message("***************************delete custom received--kane should now vanish\n");

		Commands->Destroy_Object(obj);
	}
}

void M03_KaneHead_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED && action_id == 100061)
	{
		Commands->Debug_Message("***************************kane conversation 02 is over--sending delete custom\n");

		Commands->Send_Custom_Event(obj, obj, 0, 8000, 2.0f);
	}
}

ScriptRegistrant<M03_KaneHead_JDG> M03_KaneHead_JDGRegistrant("M03_KaneHead_JDG", "");