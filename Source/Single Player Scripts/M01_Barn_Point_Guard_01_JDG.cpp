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
#include "M01_Barn_Point_Guard_01_JDG.h"

/*
M01 -> 101439
*/
void M01_Barn_Point_Guard_01_JDG::Created(GameObject *obj)
{
	Commands->Set_Innate_Is_Stationary(obj, true);
}

void M01_Barn_Point_Guard_01_JDG::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
	Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 227, 0.0f);
}

void M01_Barn_Point_Guard_01_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (obj)
	{
		Vector3 pos = Commands->Get_Position(obj);
		if (damager == Commands->Get_A_Star(pos))
		{
			Commands->Set_Innate_Is_Stationary(obj, false);

			GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
			Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 189, 0.0f);

			GameObject *nonExistingObj = Commands->Find_Object(101658);
			if (nonExistingObj)
			{
				Commands->Send_Custom_Event(obj, nonExistingObj, 0, 189, 0.0f);
			}
		}
	}
}

ScriptRegistrant<M01_Barn_Point_Guard_01_JDG> M01_Barn_Point_Guard_01_JDGRegistrant("M01_Barn_Point_Guard_01_JDG", "");