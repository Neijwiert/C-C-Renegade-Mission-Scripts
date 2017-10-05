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
#include "M01_GDIBase_POW_Conversation_Controller_JDG.h"

/*
M01 -> 121237
*/
void M01_GDIBase_POW_Conversation_Controller_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->doingConversation, sizeof(this->doingConversation), 1);
	Auto_Save_Variable(&this->conversationId, sizeof(this->conversationId), 2);
}

void M01_GDIBase_POW_Conversation_Controller_JDG::Created(GameObject *obj)
{
	this->doingConversation = false;
}

// TODO
void M01_GDIBase_POW_Conversation_Controller_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_Base_POW01_JDG when action with id 48 is complete
	// Received from M01_Base_POW02_JDG when action with id 49 is complete
	if (!type && param == 16 && !this->doingConversation)
	{
		this->doingConversation = true;

		GameObject *gdiPOW1Obj = Commands->Find_Object(116386);
		GameObject *gdiPOW2Obj = Commands->Find_Object(116385);
		if (sender == gdiPOW1Obj && gdiPOW1Obj)
		{
			// Our base commander is carrying some sort of weapon prototype. We cannot let it fall into Nod hands.
			// Good luck, sir.
			this->conversationId = Commands->Create_Conversation("M01_POW02_Conversation_01", 100, 1000.0f, false);
			Commands->Join_Conversation(gdiPOW1Obj, this->conversationId, false, true, true);
			
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			Commands->Join_Conversation(starObj, this->conversationId, false, false, false);
			Commands->Start_Conversation(this->conversationId, this->conversationId);
			Commands->Monitor_Conversation(obj, this->conversationId);
		}
		else if (sender == gdiPOW2Obj || gdiPOW2Obj)
		{
			// Thanks for the save. Did you find Captain Duncan yet?
			// I think he went down the path behind the Construction Yard.
			this->conversationId = Commands->Create_Conversation("M01_POW02_Conversation_01", 100, 1000.0f, false);
			Commands->Join_Conversation(gdiPOW2Obj, this->conversationId, false, true, true);

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			Commands->Join_Conversation(starObj, this->conversationId, false, false, false);
			Commands->Start_Conversation(this->conversationId, this->conversationId);
			Commands->Monitor_Conversation(obj, this->conversationId);
		}
		
		if (gdiPOW1Obj)
		{
			Commands->Send_Custom_Event(obj, gdiPOW1Obj, 0, 25, 0.0f);
		}

		if (gdiPOW2Obj)
		{
			Commands->Send_Custom_Event(obj, gdiPOW2Obj, 0, 25, 0.0f);
		}
	}
}

void M01_GDIBase_POW_Conversation_Controller_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered when done with conversation, see custom
	if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED && action_id == this->conversationId)
	{
		GameObject *M01ObjectivePogControllerJDGObj = Commands->Find_Object(105828);
		if (M01ObjectivePogControllerJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01ObjectivePogControllerJDGObj, 7, 102, 0.0f);
		}

		GameObject *M01GDIBasePOWEncounter02ControllerJDGObj = Commands->Find_Object(103331);
		if (M01GDIBasePOWEncounter02ControllerJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01GDIBasePOWEncounter02ControllerJDGObj, 0, 27, 0.0f);
		}

		GameObject *gdiMinigunnerObj = Commands->Find_Object(116382);
		if (gdiMinigunnerObj)
		{
			Commands->Send_Custom_Event(obj, gdiMinigunnerObj, 0, 31, 2.0f);
		}

		GameObject *M01BackPathEntranceZoneJDGObj = Commands->Find_Object(121433);
		if (M01BackPathEntranceZoneJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01BackPathEntranceZoneJDGObj, 0, 16, 0.0f);
		}
	}
}

ScriptRegistrant<M01_GDIBase_POW_Conversation_Controller_JDG> M01_GDIBase_POW_Conversation_Controller_JDGRegistrant("M01_GDIBase_POW_Conversation_Controller_JDG", "");