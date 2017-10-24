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
#include "M03_Conversation_Zone.h"

/*
M03 -> 2014713 2013087 2013086 1100001 1100005 1100013 1144502 2013899 2013900 2013901
*/
void M03_Conversation_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->troubleEnteringComms, sizeof(this->troubleEnteringComms), 1);
	Auto_Save_Variable(&this->gaveStarRefKey, sizeof(this->gaveStarRefKey), 2);
}

void M03_Conversation_Zone::Created(GameObject *obj)
{
	this->troubleEnteringComms = false;
	this->gaveStarRefKey = false;
}

void M03_Conversation_Zone::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from ourselves after 0 seconds when entered and convNum = 2 or 3 or 4 or 7
	// Received from M03_Objective_Controller after 0 seconds when objective completed with id 1002.
	if (type == 40004)
	{
		this->troubleEnteringComms = true;
	}
}

void M03_Conversation_Zone::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered when notified of comms on the right conversation, see timer number 100000
	if (action_id == 100002)
	{
		GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);
		Commands->Send_Custom_Event(obj, M03ObjectiveControllerObj, 300, 1, 0.0f);
		Commands->Send_Custom_Event(obj, M03ObjectiveControllerObj, 308, 3, 0.0f);
	}

	// Triggered when notified of approaching comms, see entered convNum = 3
	if (action_id == 100003)
	{
		GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);
		Commands->Send_Custom_Event(obj, M03ObjectiveControllerObj, 300, 1, 0.0f);

		Commands->Start_Timer(obj, this, 3.0f, 100000);
	}

	// Triggered when notified of locked up comms, see convNum = 4
	if (action_id == 100004)
	{
		GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);
		if (M03ObjectiveControllerObj)
		{
			Commands->Send_Custom_Event(obj, M03ObjectiveControllerObj, 307, 3, 0.0f);
		}
	}
}

void M03_Conversation_Zone::Timer_Expired(GameObject *obj, int number)
{
	// Triggered after 3 or 2 seconds, see action 100003 or entered convNum = 2
	if (number == 100000)
	{
		int conversationId = Commands->Create_Conversation("M03CON002", 99, 2000.0f, true); // That building on the right is the Communications Center.
		Commands->Join_Conversation(NULL, conversationId, true, true, true);
		Commands->Start_Conversation(conversationId, 100002);
		Commands->Monitor_Conversation(obj, conversationId);
	}
}

void M03_Conversation_Zone::Entered(GameObject *obj, GameObject *enterer)
{
	int convNum = Get_Int_Parameter("Conv_Num");

	if (this->troubleEnteringComms)
	{
		if (convNum == 4)
		{
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			if (!Commands->Has_Key(starObj, 1) && !this->gaveStarRefKey)
			{
				this->gaveStarRefKey = true;

				// Locke, I'm having a tough time finding a security card.
				// Check inside the Tiberium Refinery, I'm picking up officer communications from there.
				int conversationId = Commands->Create_Conversation("M03CON006", 99, 2000.0f, true);
				Commands->Join_Conversation(starObj, conversationId, true, true, true);
				Commands->Join_Conversation(NULL, conversationId, true, true, true);
				Commands->Start_Conversation(conversationId, 100006);
				Commands->Monitor_Conversation(obj, conversationId);
			}
		}
	}
	else
	{
		if (convNum == 2)
		{
			this->troubleEnteringComms = true;

			GameObject *garageElevatorTopScriptZoneObj = Commands->Find_Object(1144502);
			Commands->Send_Custom_Event(obj, garageElevatorTopScriptZoneObj, 40004, 0, 0.0);

			GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);
			Commands->Send_Custom_Event(obj, M03ObjectiveControllerObj, 300, 1, 0.0);
			Commands->Send_Custom_Event(obj, M03ObjectiveControllerObj, 308, 3, 0.0f);

			Commands->Start_Timer(obj, this, 2.0f, 100000);
		}
		else if (convNum == 3)
		{
			this->troubleEnteringComms = true;

			GameObject *ppGateInsideBaseScriptZoneObj = Commands->Find_Object(1100005);
			Commands->Send_Custom_Event(obj, ppGateInsideBaseScriptZoneObj, 40004, 0, 0.0f);

			// Looks like you're approaching the Communications Center now. Keep an eye out.
			// Affirmative.
			int conversationId = Commands->Create_Conversation("M03CON003", 99, 2000.0f, true);
			Commands->Join_Conversation(NULL, conversationId, true, true, true);

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			Commands->Join_Conversation(starObj, conversationId, true, true, true);
			Commands->Start_Conversation(conversationId, 100003);
			Commands->Monitor_Conversation(obj, conversationId);
		}
		else if (convNum == 4)
		{
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);
			if (!Commands->Has_Key(starObj, 1))
			{
				this->troubleEnteringComms = true;

				GameObject *commCenterMCTScriptZoneObj = Commands->Find_Object(2013086);
				Commands->Send_Custom_Event(obj, commCenterMCTScriptZoneObj, 40004, 0, 0.0f);

				GameObject *commCenterComputerRoomScriptZoneObj = Commands->Find_Object(2013087);
				Commands->Send_Custom_Event(obj, commCenterComputerRoomScriptZoneObj, 40004, 0, 0.0f);

				// Looks like the interior is locked up. You should be able to find a security card somewhere in the base area, probably on a Nod officer.
				// Sounds like more fun than invading an island stronghold. I'm on it.
				int conversationId = Commands->Create_Conversation("M03CON004", 99, 2000.0f, true);
				Commands->Join_Conversation(NULL, conversationId, true, true, true);
				Commands->Join_Conversation(starObj, conversationId, true, true, true);
				Commands->Start_Conversation(conversationId, 100004);
				Commands->Monitor_Conversation(obj, conversationId);
			}
		}
		else if (convNum == 7)
		{
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);
			if (Commands->Has_Key(starObj, 1))
			{
				this->troubleEnteringComms = true;

				GameObject *commCenterElevatorEntranceScriptZoneObj = Commands->Find_Object(2013901);
				Commands->Send_Custom_Event(obj, commCenterElevatorEntranceScriptZoneObj, 40004, 0, 0.0f);

				GameObject *commCenterGarageSideEntranceScriptZoneObj = Commands->Find_Object(2013900);
				Commands->Send_Custom_Event(obj, commCenterGarageSideEntranceScriptZoneObj, 40004, 0, 0.0f);

				GameObject *commCenterPPSideEntranceScriptZoneObj = Commands->Find_Object(2013899);
				Commands->Send_Custom_Event(obj, commCenterPPSideEntranceScriptZoneObj, 40004, 0, 0.0f);

				int conversationId = Commands->Create_Conversation("M03CON007", 99, 2000.0f, true); // Head inside and locate the mainframe, you should be able to access it with your EVA unit.
				Commands->Join_Conversation(NULL, conversationId, true, true, true);
				Commands->Start_Conversation(conversationId, 100007);
				Commands->Monitor_Conversation(obj, conversationId);
			}
		}
		else if (convNum == 9)
		{
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);
			if (Commands->Has_Key(starObj, 1))
			{
				this->troubleEnteringComms = true;

				int conversationId = Commands->Create_Conversation("M03CON009", 99, 2000.0f, true); // You're getting closer to the mainframe. It should be downstairs on your left.
				Commands->Join_Conversation(NULL, conversationId, true, true, true);
				Commands->Start_Conversation(conversationId, 100009);
				Commands->Monitor_Conversation(obj, conversationId);
			}
		}
		else if (convNum == 16)
		{
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);
			if (Commands->Has_Key(starObj, 20) != 1)
			{
				this->troubleEnteringComms = true;
				
				int conversationId = Commands->Create_Conversation("M03CON016", 99, 2000.0f, true); //This is GDI gunboat captain Soanso, we've still got resistance on the beachhead. I'm not going to last out here without support!
				Commands->Join_Conversation(NULL, conversationId, true, true, true);
				Commands->Start_Conversation(conversationId, 100016);
				Commands->Monitor_Conversation(obj, conversationId);
			}
		}
		else if (convNum == 22)
		{
			this->troubleEnteringComms = true;

			// Commando, don't leave me hanging! Take out those SAM Sites!
			// Affirmative.
			int conversationId = Commands->Create_Conversation("M03CON022", 99, 2000.0f, true);
			Commands->Join_Conversation(NULL, conversationId, true, true, true);

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			Commands->Join_Conversation(starObj, conversationId, true, true, true);
			Commands->Start_Conversation(conversationId, 100022);
			Commands->Monitor_Conversation(obj, conversationId);
		}
		else if (convNum == 61)
		{
			this->troubleEnteringComms = true;

			// You are quite the pest, GDI.  Let me see... Ah, Captain Parker, code name Havoc. Hmm... how cliché.
			// <signature laugh>
			int conversationId = Commands->Create_Conversation("M03CON061", 99, 2000.0f, true);

			GameObject *nonExistingObj = Commands->Find_Object(2016365);
			Commands->Join_Conversation(nonExistingObj, conversationId, true, true, true);
		
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			Commands->Join_Conversation(starObj, conversationId, true, true, true);
			Commands->Start_Conversation(conversationId, 100061);
			Commands->Monitor_Conversation(obj, conversationId);
		}
	}
}

ScriptRegistrant<M03_Conversation_Zone> M03_Conversation_ZoneRegistrant("M03_Conversation_Zone", "Conv_Num:int");