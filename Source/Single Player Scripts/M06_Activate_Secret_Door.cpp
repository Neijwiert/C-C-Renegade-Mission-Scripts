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
#include "M06_Activate_Secret_Door.h"

/*
M03 -> 2013086
M06 -> 113907
*/
void M06_Activate_Secret_Door::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->pokedByStar, sizeof(this->pokedByStar), 1);
}

void M06_Activate_Secret_Door::Created(GameObject *obj)
{
	this->pokedByStar = false;
}

void M06_Activate_Secret_Door::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (action_id == 300608 && complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED)
	{
		int secretDoorId = Get_Int_Parameter("Secret_Door_ID");
		Commands->Static_Anim_Phys_Goto_Last_Frame(secretDoorId, "BK.BK");

		GameObject *M06ObjectiveControllerObj = Commands->Find_Object(100018);
		Commands->Send_Custom_Event(obj, M06ObjectiveControllerObj, 608, 1, 0.0f);
	}
}

void M06_Activate_Secret_Door::Poked(GameObject *obj, GameObject *poker)
{
	if (Commands->Is_A_Star(poker) && !this->pokedByStar)
	{
		this->pokedByStar = true;
		
		int conversationId = Commands->Create_Conversation("M06_CON061", 95, 0.0f, true); // More toys.

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, false, true, true);
		Commands->Start_Conversation(conversationId, 300608);
		Commands->Monitor_Conversation(obj, conversationId);
	}
}

ScriptRegistrant<M06_Activate_Secret_Door> M06_Activate_Secret_DoorRegistrant("M06_Activate_Secret_Door", "Secret_Door_ID=0:int");