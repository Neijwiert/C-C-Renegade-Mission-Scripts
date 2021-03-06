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
#include "MX0_A03_NOD_TURRET.h"

/*
M13 -> 1400146 1400144
*/
void MX0_A03_NOD_TURRET::Created(GameObject *obj)
{
	GameObject *MX0A03ControllerObj = Commands->Find_Object(1400041);
	if (MX0A03ControllerObj)
	{
		Commands->Send_Custom_Event(obj, MX0A03ControllerObj, 403, 0, 0.0f);
	}
}

void MX0_A03_NOD_TURRET::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *MX0A03ControllerObj = Commands->Find_Object(1400041);
	if (MX0A03ControllerObj)
	{
		Commands->Send_Custom_Event(obj, MX0A03ControllerObj, 404, 0, 0.0f);
	}
}

ScriptRegistrant<MX0_A03_NOD_TURRET> MX0_A03_NOD_TURRETRegistrant("MX0_A03_NOD_TURRET", "");