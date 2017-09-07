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
#include "M00_Vehicle_Regen_DAK.h"

void M00_Vehicle_Regen_DAK::Created(GameObject *obj)
{
	Commands->Send_Custom_Event(obj, obj, 0, 0, 0.0f);
}

void M00_Vehicle_Regen_DAK::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		if (Commands->Get_Max_Health(obj) > Commands->Get_Health(obj))
		{
			Commands->Apply_Damage(obj, -2.0f, "RegenHealth", NULL);
		}

		Commands->Send_Custom_Event(obj, obj, 0, 0, 1.0f);
	}
}

ScriptRegistrant<M00_Vehicle_Regen_DAK> M00_Vehicle_Regen_DAKRegistrant("M00_Vehicle_Regen_DAK", "");