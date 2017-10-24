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
#include "M03_Intro_Substitute.h"

/*
M03 -> 1100012
*/
void M03_Intro_Substitute::Created(GameObject *obj)
{
	GameObject *introCinematicsObj = Commands->Find_Object(1111000);
	if (introCinematicsObj)
	{
		Commands->Send_Custom_Event(obj, introCinematicsObj, 100, 100, 0.0f); // Create hovercraft controller
		Commands->Send_Custom_Event(obj, introCinematicsObj, 200, 200, 14.0f); // Create chinook troop drop - beach

		Commands->Start_Timer(obj, this, 14.0f, 0);
	}

	if (Commands->Find_Object(1100004)) // M03ObjectiveControllerObj
	{
		Commands->Start_Timer(obj, this, 8.5f, 9997);
	}
}

void M03_Intro_Substitute::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M03_Objective_Controller after 0 seconds when custom type 100063 is received
	if (type == 999)
	{
		Commands->Start_Timer(obj, this, 3.0f, 9998);
	}
}

void M03_Intro_Substitute::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered when locke said need to find scientist's whereabouts, see action id 100039
	if (action_id == 100001)
	{
		GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);
		Commands->Send_Custom_Event(obj, M03ObjectiveControllerObj, 300, 3, 0.0f);

		int conversationId = Commands->Create_Conversation("M03CON012", 99, 2000.0f, false); // This is GDI gunboat captain Soanso, calling any available GDI units to the beachhead area. We've got heavy resistance, requesting backup!
		Commands->Join_Conversation(NULL, conversationId, true, true, true);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, true, true);
		Commands->Start_Conversation(conversationId, 100012);
		Commands->Monitor_Conversation(obj, conversationId);
	}

	// Triggered when locke said nice landing, see timer expired
	else if (action_id == 100039)
	{
		// Havoc - nice landing. We'll need info on the scientist's whereabouts. Find the Nod Communications Center and download the information.
		// Time to rock and roll!
		int conversationId = Commands->Create_Conversation("M03CON001", 99, 2000.0f, false);
		Commands->Join_Conversation(NULL, conversationId, true, true, true);
		
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, true, true);
		Commands->Start_Conversation(conversationId, 100001);
		Commands->Monitor_Conversation(obj, conversationId);
	}

	// Triggered when gunboat called for help, see action id 100001
	else if (action_id == 100012)
	{
		GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);
		Commands->Send_Custom_Event(obj, M03ObjectiveControllerObj, 301, 3, 0.0f);
	}
}

void M03_Intro_Substitute::Timer_Expired(GameObject *obj, int number)
{
	// Triggered after 8.5 seconds, see create
	if (number == 9997)
	{
		// Nice landing Nicky. Figured your friends could use a welcoming party.
		// I'm coming for you.
		int conversationId = Commands->Create_Conversation("M03CON039", 99, 2000.0f, false);
		Commands->Join_Conversation(NULL, conversationId, true, true, true);
		
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, true, true);
		Commands->Start_Conversation(conversationId, 100039);
		Commands->Monitor_Conversation(obj, conversationId);
	}
}

ScriptRegistrant<M03_Intro_Substitute> M03_Intro_SubstituteRegistrant("M03_Intro_Substitute", "");