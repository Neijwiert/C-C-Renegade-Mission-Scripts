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
#include "M01_DataDisc_TextController_JDG.h"

/*
M01 -> 117188
*/
void M01_DataDisc_TextController_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->doingTutorialConversation, sizeof(this->doingTutorialConversation), 1);
}

void M01_DataDisc_TextController_JDG::Created(GameObject *obj)
{
	this->doingTutorialConversation = false;
}

// TODO
void M01_DataDisc_TextController_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 1)
	{
		// Received from M01_C4_Tutorial_Zone_JDG when entered
		// Received from M01_C4_TutorialScript_JDG when granted
		if (param == 1)
		{
			if (!this->doingTutorialConversation)
			{
				// Press '9' to select Remote C4
				// Left-click your mouse to place C4
				// Right-click your mouse to Detonate
				int conversationId = Commands->Create_Conversation("M01_C4_Tutorial_Conversation", 100, 1000.0f, true);
				Commands->Join_Conversation(NULL, conversationId, false, false, true);
				Commands->Start_Conversation(conversationId, conversationId);

				Commands->Set_HUD_Help_Text(8276, Vector3(0.196f, 0.882f, 0.196f)); // Press '9' to select Remote C4\n

				Commands->Send_Custom_Event(obj, obj, 1, 2, 2.0f);

				this->doingTutorialConversation = true;
			}
		}

		// Received from ourselves after 2 seconds after type 1 param 1
		else if (param == 2)
		{
			Commands->Set_HUD_Help_Text(8281, Vector3(0.196f, 0.882f, 0.196f)); // Left-click your mouse to place C4\n

			Commands->Send_Custom_Event(obj, obj, 1, 3, 2.0f);
		}

		// Received from ourselves after 2 seconds after type 1 param 2
		else if (param == 3)
		{
			Commands->Set_HUD_Help_Text(8282, Vector3(0.196f, 0.882f, 0.196f)); // Right-click your mouse to Detonate\n

			Commands->Send_Custom_Event(obj, obj, 1, 4, 2.0f);
		}

		// Received from ourselves after 2 seconds after type 1 param 3
		else if (param == 4)
		{
			this->doingTutorialConversation = false;
		}
	}
	else if (type == 2)
	{
		// Received from M01_GDI_GuardTower02_SniperRifle_JDG when granted
		// Received from M01_SniperRifle_01_JDG when granted
		// Received from M01_SniperRifle_02_JDG when granted
		if (param == 1)
		{
			if (!this->doingTutorialConversation)
			{
				// Press '3' to select the Sniper Rifle
				// Right-click your mouse for Sniper Mode
				// Use the mouse wheel or 'G' and 'T' to Zoom
				int conversationId = Commands->Create_Conversation("M01_Sniper_Tutorial_Conversation", 100, 1000.0f, true);
				Commands->Join_Conversation(NULL, conversationId, false, false, true);
				Commands->Start_Conversation(conversationId, conversationId);

				Commands->Set_HUD_Help_Text(8275, Vector3(0.196f, 0.882f, 0.196f)); // Press '3' to select the Sniper Rifle\n

				Commands->Send_Custom_Event(obj, obj, 2, 2, 2.0f);

				this->doingTutorialConversation = true;
			}
		}

		// Received from ourselves after 2 seconds after type 2 param 1
		else if (param == 2)
		{
			Commands->Set_HUD_Help_Text(8279, Vector3(0.196f, 0.882f, 0.196f)); // Right-click your mouse for Sniper Mode\n

			Commands->Send_Custom_Event(obj, obj, 2, 3, 2.0f);
		}

		// Received from ourselves after 2 seconds after type 2 param 2
		else if (param == 3)
		{
			Commands->Set_HUD_Help_Text(8280, Vector3(0.196f, 0.882f, 0.196f)); // Use the mouse wheel or 'G' and 'T' to Zoom\n

			Commands->Send_Custom_Event(obj, obj, 2, 4, 2.0f);
		}

		// Received from ourselves after 2 seconds after type 2 param 3
		else if (param == 4)
		{
			this->doingTutorialConversation = false;
		}
	}
	else if (!type)
	{
		// Received from M01_Beach_Datadisc_JDG after 2 seconds when granted
		// Received from M01_Medlab_Datadisc_JDG after 2 seconds when granted 
		// Received from M01_Priest_Datadisc_JDG after 2 seconds when granted
		// Received from M01_Shed_Datadisc_JDG after 2 seconds when granted
		// Received from M01_TibCave01_Datadisc_JDG after 2 seconds when granted
		// Received from M01_TibCave02_Datadisc_JDG after 2 seconds when granted
		if (param == 10)
		{
			this->doingTutorialConversation = true;
			
			int conversationId = Commands->Create_Conversation("M01_Press_ESC_for_Map_Conversation", 100, 1000.0f, true); // Press 'M' for the Mission Map
			Commands->Join_Conversation(0, conversationId, false, false, true);
			Commands->Start_Conversation(conversationId, conversationId);

			Commands->Set_HUD_Help_Text(8274, Vector3(0.196f, 0.882f, 0.196f)); // Press 'M' for the Mission Map\n

			Commands->Send_Custom_Event(obj, obj, 0, 11, 2.0f);
		}

		// Received from ourselves after 2 seconds after type <= 0 and param 10
		else
		{
			this->doingTutorialConversation = false;
		}
	}
}

ScriptRegistrant<M01_DataDisc_TextController_JDG> M01_DataDisc_TextController_JDGRegistrant("M01_DataDisc_TextController_JDG", "");