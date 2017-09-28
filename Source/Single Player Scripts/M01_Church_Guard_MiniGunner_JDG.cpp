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
#include "M01_Church_Guard_MiniGunner_JDG.h"

/*
M01 -> 101313
*/
void M01_Church_Guard_MiniGunner_JDG::Created(GameObject *obj)
{
	Vector3 pos = Commands->Get_Position(obj);
	Commands->Set_Innate_Soldier_Home_Location(obj, pos, 5.0f);
}

void M01_Church_Guard_MiniGunner_JDG::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
	Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 173, 0.0f);

	GameObject *nonExistingObj = Commands->Find_Object(103393);
	if (nonExistingObj)
	{
		Commands->Send_Custom_Event(obj, nonExistingObj, 0, 173, 0.0f);
	}
}

void M01_Church_Guard_MiniGunner_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *starObj = Commands->Get_A_Star(pos);

	if (damager == starObj)
	{
		if (obj)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 38);
			params.Set_Movement(starObj, 0.3f, 5.0f);
			params.Set_Attack(starObj, 15.0f, 0.0f, true);

			Commands->Action_Attack(obj, params);
		}
	}
}

ScriptRegistrant<M01_Church_Guard_MiniGunner_JDG> M01_Church_Guard_MiniGunner_JDGRegistrant("M01_Church_Guard_MiniGunner_JDG", "");