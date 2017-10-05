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
#include "M01_GDI_Escort_Conversation_Controller_GDI.h"

/*
M01 -> 103396
*/
void M01_GDI_Escort_Conversation_Controller_GDI::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->canNotifyStarWeStayPut, sizeof(this->canNotifyStarWeStayPut), 1);
}

void M01_GDI_Escort_Conversation_Controller_GDI::Created(GameObject *obj)
{
	this->canNotifyStarWeStayPut = true;
}

void M01_GDI_Escort_Conversation_Controller_GDI::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_CHURCH_Chinook_Spawned_Soldier01_GDI or M01_CHURCH_Chinook_Spawned_Soldier02_GDI when param 180 is received
	// Received from M01_COMM_Chinook_Spawned_Soldier_GDI when param 180 is received
	if (param == 27)
	{
		const char *conversationName;

		int randInt = Commands->Get_Random_Int(0, 3);
		if (!randInt)
		{
			// Stick close, move out.
			// Right with you sir.
			conversationName = "M01_GDI_Escort_Yes01";
		}
		else if (randInt == 1)
		{
			// I need backup.
			// On your tail, sir.
			conversationName = "M01_GDI_Escort_Yes02";
		}
		else
		{
			// Tag - you're it.
			// Got you covered, sir.
			conversationName = "M01_GDI_Escort_Yes03";
		}

		int conversationId = Commands->Create_Conversation(conversationName, 90, 50.0f, true);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, false, false, false);
		Commands->Join_Conversation(sender, conversationId, false, false, false);
		Commands->Start_Conversation(conversationId, conversationId);
	}

	// Received from M01_CHURCH_Chinook_Spawned_Soldier01_GDI or M01_CHURCH_Chinook_Spawned_Soldier02_GDI when param 149 or 150 is received
	if (param == 29)
	{
		const char *conversationName;
		if (this->canNotifyStarWeStayPut)
		{
			this->canNotifyStarWeStayPut = false;

			// Locke wants backup to hold this position, sir.  End of the line for me.
			// You're my backup.  What's the deal?
			// Sorry, sir. Orders.
			// Fine, stay then. Maybe Nod will play with me.
			conversationName = "M01_GDI_Escort_MustStayHere01";
		}
		else
		{
			// Locke wants backup to hold this position, sir.  End of the line for me.
			// Fine, stay then. Maybe Nod will play with me.
			conversationName = "M01_GDI_Escort_MustStayHere02";
		}
		
		int conversationId = Commands->Create_Conversation(conversationName, 90, 50.0f, true);
		Commands->Join_Conversation(sender, conversationId, false, false, false);
		
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, false, false, false);
		Commands->Start_Conversation(conversationId, conversationId);
	}
}

ScriptRegistrant<M01_GDI_Escort_Conversation_Controller_GDI> M01_GDI_Escort_Conversation_Controller_GDIRegistrant("M01_GDI_Escort_Conversation_Controller_GDI", "");