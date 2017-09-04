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
#include "MX0_A02_HELICOPTER.h"

// After MX0_A02_Controller receives custom type 232 and progressCounter is 3 or 4
void MX0_A02_HELICOPTER::Created(GameObject *obj)
{
	if (Get_Int_Parameter("ActorID"))
	{
		Commands->Start_Timer(obj, this, 23.0f, 211);
	}
}

void MX0_A02_HELICOPTER::Killed(GameObject *obj, GameObject *killer)
{
	if (Get_Int_Parameter("ActorID"))
	{
		GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
		if (MX0A02ControllerObj)
		{
			Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 246, 0, 0.0f);
		}
	}
	else
	{
		GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
		if (MX0A02ControllerObj)
		{
			Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 241, 0, 0.0f);
		}
	}
}

void MX0_A02_HELICOPTER::Timer_Expired(GameObject *obj, int number)
{
	// Triggered 23 seconds after created with an ActorID > 0
	if (number == 211)
	{
		Commands->Apply_Damage(obj, 10000.0f, "Blamokiller", NULL);
	}
}

ScriptRegistrant<MX0_A02_HELICOPTER> MX0_A02_HELICOPTERRegistrant("MX0_A02_HELICOPTER", "ActorID=0:int");