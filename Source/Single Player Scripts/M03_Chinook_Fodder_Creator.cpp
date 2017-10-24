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
#include "M03_Chinook_Fodder_Creator.h"

/*
M03 -> 1122334
*/
void M03_Chinook_Fodder_Creator::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from RMV_Trigger_Zone after 0 seconds when entered. type = 3000, param = 3000 (id = 1100006)
	if (Commands->Find_Object(1100003) && type == 3000 && param == 3000) // GDI Gunboat
	{
		GameObject *M03MissionControllerObj = Commands->Find_Object(1100004);
		if (M03MissionControllerObj)
		{
			Commands->Send_Custom_Event(obj, M03MissionControllerObj, 302, 3, 0.0f);
		}
	}
}

void M03_Chinook_Fodder_Creator::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// When conversation has ended, see timer expired
	if (action_id == 100043)
	{
		GameObject *M03MissionControllerObj = Commands->Find_Object(1100004);
		if (M03MissionControllerObj)
		{
			Commands->Send_Custom_Event(obj, M03MissionControllerObj, 302, 3, 0.0f);
		}
	}
}

void M03_Chinook_Fodder_Creator::Timer_Expired(GameObject *obj, int number)
{
	// Never triggered
	if (number == 9998)
	{
		int conversationId = Commands->Create_Conversation("M03CON043", 99, 2000.0f, false); // Pull out, pull out! I've got SAMs on the radar, pull - <static>
		Commands->Join_Conversation(NULL, conversationId, true, true, true);
		Commands->Start_Conversation(conversationId, 100043);
		Commands->Monitor_Conversation(obj, conversationId);
	}
}

ScriptRegistrant<M03_Chinook_Fodder_Creator> M03_Chinook_Fodder_CreatorRegistrant("M03_Chinook_Fodder_Creator", "");