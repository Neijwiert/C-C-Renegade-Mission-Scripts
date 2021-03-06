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
#include "MX0_A02_ZONE_DEFAULT_ON.h"

// This script is never used
void MX0_A02_ZONE_DEFAULT_ON::Entered(GameObject *obj, GameObject *enterer)
{
	GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
	if (MX0A02ControllerObj)
	{
		Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 202, 0, 0.0f);
		Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 223, 1200017, 0.0f);
		Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 224, 1200023, 0.0f);
	}
}

ScriptRegistrant<MX0_A02_ZONE_DEFAULT_ON> MX0_A02_ZONE_DEFAULT_ONRegistrant("MX0_A02_ZONE_DEFAULT_ON", "");