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
#include "M03_Objective_Tracker.h"

/*
M03 -> 1100004
*/
void M03_Objective_Tracker::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->missionStatusCounter, sizeof(this->missionStatusCounter), 1);
}

void M03_Objective_Tracker::Created(GameObject *obj)
{
	this->missionStatusCounter = 0;
}

void M03_Objective_Tracker::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M03_CommCenter_Arrow after 0 seconds when custom type 40010 is received, type = 309, param = 1
	// Received from M03_Conversation_Zone after 0 seconds when action id 100002 is completed. type = 300, param = 1
	// Received from M03_Conversation_Zone after 0 seconds when action id 100003 is completed. type = 300, param = 1
	// Received from M03_Conversation_Zone after 0 seconds when entered. type = 300, param = 1
	// Received from RMV_M03_Comm_Center_Terminal after 0 seconds when poked. type = 308, param = 1
	if (type == 300 || type == 308 || type == 309)
	{
		if (param == 1)
		{
			this->missionStatusCounter++;
		}
	}

	// Never received
	if (type == 8000)
	{
		if (param == 8000 && sender)
		{
			Commands->Send_Custom_Event(obj, sender, 8000, this->missionStatusCounter, 0.0f);
		}
	}
}

ScriptRegistrant<M03_Objective_Tracker> M03_Objective_TrackerRegistrant("M03_Objective_Tracker", "");