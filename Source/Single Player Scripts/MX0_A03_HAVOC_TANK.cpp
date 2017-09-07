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
#include "MX0_A03_HAVOC_TANK.h"

// This script is never used
void MX0_A03_HAVOC_TANK::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->canDoEnteredEvents, sizeof(this->canDoEnteredEvents), 1);
}

void MX0_A03_HAVOC_TANK::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED && this->canDoEnteredEvents)
	{
		GameObject *MX0A03ControllerObj = Commands->Find_Object(1400041);
		if (MX0A03ControllerObj)
		{
			Commands->Send_Custom_Event(obj, MX0A03ControllerObj, 401, 0, 0.0f);
		}

		this->canDoEnteredEvents = false;
	}
}

ScriptRegistrant<MX0_A03_HAVOC_TANK>MX0_A03_HAVOC_TANKRegistrant("MX0_A03_HAVOC_TANK", "");