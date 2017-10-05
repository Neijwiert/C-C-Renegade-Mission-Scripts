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
#include "M01_Nod_Commander_Conversation_Controller_GDI.h"

/*
M01 -> 103398
*/
void M01_Nod_Commander_Conversation_Controller_GDI::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->canCallForBackup, sizeof(this->canCallForBackup), 1);
	Auto_Save_Variable(&this->conversationId, sizeof(this->conversationId), 2);
}

void M01_Nod_Commander_Conversation_Controller_GDI::Created(GameObject *obj)
{
	this->canCallForBackup = true;
}

void M01_Nod_Commander_Conversation_Controller_GDI::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_GDI_GuardTower_NOD_Commander_JDG or M01_TailgunRun_NOD_Commander_JDG when param 66 is received
	if (param == 27)
	{
		if (sender == Commands->Find_Object(102360) && this->canCallForBackup) // Entrance barn zone area tower nod officer
		{
			this->canCallForBackup = false;

			this->conversationId = Commands->Create_Conversation("M01_Nod_Commander_Conv04", 100, 300.0f, true); // Enemy forces closing! Requesting backup!
			Commands->Join_Conversation(sender, this->conversationId, false, false, false);
			Commands->Start_Conversation(this->conversationId, this->conversationId);
			Commands->Monitor_Conversation(obj, this->conversationId);
		}
		else
		{
			int randInt = Commands->Get_Random_Int(0, 5);
			const char *conversationName;

			switch (randInt)
			{
				case 0:
					conversationName = "M01_Nod_Commander_Conv01"; // Incoming!  Radio for backup!

					break;
				case 1:
					conversationName = "M01_Nod_Commander_Conv02"; // We've got incoming, send reinforcements.

					break;
				case 2:
					conversationName = "M01_Nod_Commander_Conv03"; // Where are my reinforcements!

					break;
				case 3:
					conversationName = "M01_Nod_Commander_Conv04"; // Enemy forces closing! Requesting backup!

					break;
				default:
					conversationName = "M01_Nod_Commander_Conv05"; // What's the delay? Get reinforcements here now!

					break;
			}

			conversationId = Commands->Create_Conversation(conversationName, 90, 200.0f, true);
			Commands->Join_Conversation(sender, conversationId, false, false, false);
			Commands->Start_Conversation(conversationId, conversationId);
		}
	}
}

void M01_Nod_Commander_Conversation_Controller_GDI::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// See custom
	if (action_id == this->conversationId)
	{
		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 71, 0.0f);
	}
}

ScriptRegistrant<M01_Nod_Commander_Conversation_Controller_GDI> M01_Nod_Commander_Conversation_Controller_GDIRegistrant("M01_Nod_Commander_Conversation_Controller_GDI", "");