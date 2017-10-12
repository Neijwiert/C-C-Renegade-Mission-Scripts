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
#include "M03_Structure_Powerup_Drop.h"

/*
M03 -> 300059 300058 300056 1100020
*/
void M03_Structure_Powerup_Drop::Killed(GameObject *obj, GameObject *killer)
{
	Vector3 pos = Commands->Get_Position(obj);
	float facing = Commands->Get_Facing(obj);

	float turnedAroundFacingRad = DEG_TO_RADF(facing + 180.0f);
	pos.X += WWMath::Cos(turnedAroundFacingRad * 2.5f);
	pos.Y += WWMath::Sin(turnedAroundFacingRad * 2.5f);
	pos.Z += 0.5f;

	const char *powerup = Get_Parameter("Powerup");
	Commands->Create_Object(powerup, pos);
}

ScriptRegistrant<M03_Structure_Powerup_Drop> M03_Structure_Powerup_DropRegistrant("M03_Structure_Powerup_Drop", "Powerup:string");