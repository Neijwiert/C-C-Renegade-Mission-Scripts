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
#include "M01_BackPath_EntranceZone_JDG.h"

/*
M01 -> 121433
*/
void M01_BackPath_EntranceZone_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		// Received from ourselves after entered
		if (param == 27)
		{
			GameObject *gdiMinigunnerObj = Commands->Find_Object(116382);
			if (gdiMinigunnerObj)
			{
				Commands->Send_Custom_Event(obj, gdiMinigunnerObj, 0, 32, 0.0f);
				Commands->Send_Custom_Event(obj, obj, 0, 28, 2.0f);
			}
		}
		
		// Received from ourselves, see param 27
		else if (param == 28)
		{
			GameObject *gdiGrenadierObj = Commands->Find_Object(116383);
			if (gdiGrenadierObj)
			{
				Commands->Send_Custom_Event(obj, gdiGrenadierObj, 0, 32, 0.0f);
			}

			Commands->Destroy_Object(obj);
		}
	}
}

void M01_BackPath_EntranceZone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		Commands->Send_Custom_Event(obj, obj, 0, 27, 0.0f);
	}
}

ScriptRegistrant<M01_BackPath_EntranceZone_JDG> M01_BackPath_EntranceZone_JDGRegistrant("M01_BackPath_EntranceZone_JDG", "");