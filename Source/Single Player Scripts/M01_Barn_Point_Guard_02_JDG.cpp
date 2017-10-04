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
#include "M01_Barn_Point_Guard_02_JDG.h"

// This script is never used
void M01_Barn_Point_Guard_02_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->damagedByStar, sizeof(this->damagedByStar), 1);
}

void M01_Barn_Point_Guard_02_JDG::Created(GameObject *obj)
{
	this->damagedByStar = false;

	Vector3 pos = Commands->Get_Position(obj);
	Commands->Set_Innate_Soldier_Home_Location(obj, pos, 2.0f);
}

void M01_Barn_Point_Guard_02_JDG::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
	Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 227, 0.0f);
}

void M01_Barn_Point_Guard_02_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (obj)
	{
		Vector3 pos = Commands->Get_Position(obj);
		if (damager == Commands->Get_A_Star(pos) && !this->damagedByStar && Commands->Get_Health(obj) > 0.0f)
		{
			Commands->Create_Sound("WereOutGunned_2", pos, obj);

			GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
			Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 189, 0.0f);

			this->damagedByStar = true;
		}
	}
}

ScriptRegistrant<M01_Barn_Point_Guard_02_JDG> M01_Barn_Point_Guard_02_JDGRegistrant("M01_Barn_Point_Guard_02_JDG", "");