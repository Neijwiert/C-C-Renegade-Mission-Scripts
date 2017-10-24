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
#include "M03_Beach_Scenario_Controller.h"

/*
M03 -> 1144729
*/
void M03_Beach_Scenario_Controller::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->beachTargetKilledCount, sizeof(this->beachTargetKilledCount), 1);
}

void M03_Beach_Scenario_Controller::Created(GameObject *obj)
{
	this->beachTargetKilledCount = 0;
}

void M03_Beach_Scenario_Controller::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from RMV_Trigger_Killed after 0 seconds when killed (id = 1144677, 1144448, 1144731, 1144732)
	if (++this->beachTargetKilledCount > 3)
	{
		this->beachTargetKilledCount = 0;

		GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);
		if (M03ObjectiveControllerObj)
		{
			if (Commands->Find_Object(1100003)) // Gunboat
			{
				Commands->Send_Custom_Event(obj, M03ObjectiveControllerObj, 301, 3, 0.0f);
				Commands->Send_Custom_Event(obj, M03ObjectiveControllerObj, 301, 1, 0.0f);

				int conversationId = Commands->Create_Conversation("M03CON013", 99, 2000.0f, true); // Nice work commando! I'm making my way around the island now.
				Commands->Join_Conversation(NULL, conversationId, true, true, true);
				Commands->Start_Conversation(conversationId, 100013);
				Commands->Monitor_Conversation(obj, conversationId);
				
				Vector3 pos = Commands->Get_Position(obj);
				GameObject *starObj = Commands->Get_A_Star(pos);

				Commands->Grant_Key(starObj, 20, true);
			}
		}
	}
}

ScriptRegistrant<M03_Beach_Scenario_Controller> M03_Beach_Scenario_ControllerRegistrant("M03_Beach_Scenario_Controller", "");