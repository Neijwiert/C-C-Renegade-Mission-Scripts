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
#include "RMV_M03_Comm_Center_Terminal.h"

/*
M03 -> 1100009
*/
void RMV_M03_Comm_Center_Terminal::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->pokedByStar, sizeof(this->pokedByStar), 1);
}

void RMV_M03_Comm_Center_Terminal::Created(GameObject *obj)
{
	this->pokedByStar = false;
}

void RMV_M03_Comm_Center_Terminal::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	float maxHealth = Commands->Get_Max_Health(obj);
	Commands->Set_Health(obj, maxHealth);
}

void RMV_M03_Comm_Center_Terminal::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// When conversation is done, see poked
	if (action_id == 100008)
	{
		GameObject *M03MissionControllerObj = Commands->Find_Object(1100004);
		Commands->Send_Custom_Event(obj, M03MissionControllerObj, 40000, 0, 0.0f);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Grant_Key(starObj, 5, false);

		Commands->Create_Object("Boss", Vector3(-160.69f, 76.47f, 16.27f));
	}
}

void RMV_M03_Comm_Center_Terminal::Poked(GameObject *obj, GameObject *poker)
{
	if (!this->pokedByStar && Commands->Is_A_Star(poker))
	{
		this->pokedByStar = true;

		GameObject *M03MissionControllerObj = Commands->Find_Object(1100004);
		Commands->Send_Custom_Event(obj, M03MissionControllerObj, 308, 1, 0.0f);

		// Flight Data Download Complete
		// That should do it. The flight plan indicates the aircraft recently passed this location and is heading toward the mainland.
		int conversationId = Commands->Create_Conversation("M03CON008", 99, 2000.0f, true);
		Commands->Join_Conversation(NULL, conversationId, true, true, true);
		Commands->Join_Conversation(NULL, conversationId, true, true, true);
		Commands->Start_Conversation(conversationId, 100008);
		Commands->Monitor_Conversation(obj, conversationId);
	}
}

ScriptRegistrant<RMV_M03_Comm_Center_Terminal> RMV_M03_Comm_Center_TerminalRegistrant("RMV_M03_Comm_Center_Terminal", "");