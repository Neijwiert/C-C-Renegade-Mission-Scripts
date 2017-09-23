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
#include "M01_BasalBuilding_Population_JDG.h"

/*
M01 -> 103287 103286 103279 103278 103277 103276 106266 114146 106265
M11 -> 101276 101275 101274 101273 101102 101101 101100 101770 101771
*/
void M01_BasalBuilding_Population_JDG::Created(GameObject *obj)
{
	Vector3 pos = Commands->Get_Position(obj);
	float randHomeRadius = Commands->Get_Random(2.0f, 10.0f);
	Commands->Set_Innate_Soldier_Home_Location(obj, pos, randHomeRadius);
}

ScriptRegistrant<M01_BasalBuilding_Population_JDG> M01_BasalBuilding_Population_JDGRegistrant("M01_BasalBuilding_Population_JDG", "");