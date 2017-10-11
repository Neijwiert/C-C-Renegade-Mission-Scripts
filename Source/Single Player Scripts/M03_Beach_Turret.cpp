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
#include "M03_Beach_Turret.h"

/*
M03 -> 1144677 1144678 1144679
*/
void M03_Beach_Turret::Created(GameObject *obj)
{
	Commands->Start_Timer(obj, this, 1.0f, 0);
}

void M03_Beach_Turret::Killed(GameObject *obj, GameObject *killer)
{
	Vector3 pos = Commands->Get_Position(obj);
	float facing = Commands->Get_Facing(obj);

	GameObject *nodTurretDestroyedObj = Commands->Create_Object("Nod_Turret_Destroyed", pos);
	Commands->Set_Facing(nodTurretDestroyedObj, facing);
	Commands->Attach_Script(nodTurretDestroyedObj, "M03_Destroyed_Turret", "");
}

void M03_Beach_Turret::Timer_Expired(GameObject *obj, int number)
{
	GameObject *gdiGunBoatObj = Commands->Find_Object(1100003);

	ActionParamsStruct params;
	params.Set_Basic(this, 90.0f, 0);
	params.Set_Attack(gdiGunBoatObj, 300.0f, 6.0f - Commands->Get_Difficulty_Level(), true);

	Commands->Action_Attack(obj, params);
}

ScriptRegistrant<M03_Beach_Turret> M03_Beach_TurretRegistrant("M03_Beach_Turret", "");