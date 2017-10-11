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
#include "M03_Destroyed_Turret.h"

void M03_Destroyed_Turret::Created(GameObject *obj)
{
	Commands->Start_Timer(obj, this, 1.0f, 0);
	Commands->Start_Timer(obj, this, 4.0f, 1);
}

void M03_Destroyed_Turret::Timer_Expired(GameObject *obj, int number)
{
	// After 1 second on creation
	if (!number)
	{
		Commands->Create_2D_Sound("EVA_Enemy_Structure_Destroyed");
	}
}

ScriptRegistrant<M03_Destroyed_Turret> M03_Destroyed_TurretRegistrant("M03_Destroyed_Turret", "");