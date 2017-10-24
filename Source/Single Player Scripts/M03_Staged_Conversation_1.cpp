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
#include "M03_Staged_Conversation_1.h"

/*
M03 -> 1144674
*/
void M03_Staged_Conversation_1::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from RMV_Trigger_Zone after 0 seconds when entered. type = 1000, param = 1000 (id = 1144675)
	if (type == 1000 && param == 1000)
	{
		const char *strSoldier1Id = Get_Parameter(0);
		int soldier1Id = atoi(strSoldier1Id);
		Commands->Find_Object(soldier1Id); // Just a Find_Object call here

		const char *strSoldier2Id = Get_Parameter(1);
		int soldier2Id = atoi(strSoldier2Id);
		Commands->Find_Object(soldier2Id); // Just a Find_Object call here
	}
}

void M03_Staged_Conversation_1::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED)
	{
		const char *strSoldier1Id = Get_Parameter(0);
		int soldier1Id = atoi(strSoldier1Id);
		GameObject *soldier1Obj = Commands->Find_Object(soldier1Id);

		const char *strSoldier2Id = Get_Parameter(1);
		int soldier2Id = atoi(strSoldier2Id);
		GameObject *soldier2Obj = Commands->Find_Object(soldier2Id);

		if (soldier1Obj)
		{
			Commands->Send_Custom_Event(obj, soldier1Obj, 100, 100, 0.0f);
		}

		if (soldier2Obj)
		{
			Commands->Send_Custom_Event(obj, soldier2Obj, 100, 100, 0.0f);
		}
	}
}

ScriptRegistrant<M03_Staged_Conversation_1> M03_Staged_Conversation_1Registrant("M03_Staged_Conversation_1", "Soldier_1_ID:int, Soldier_2_ID:int");