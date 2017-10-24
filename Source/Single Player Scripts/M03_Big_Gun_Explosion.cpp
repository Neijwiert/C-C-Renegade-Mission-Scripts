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
#include "M03_Big_Gun_Explosion.h"

// This script is never used
void M03_Big_Gun_Explosion::Created(GameObject *obj)
{
	Commands->Create_3D_Sound_At_Bone("Explosion_Large_07", obj, "O_WREAKAGE");
}

ScriptRegistrant<M03_Big_Gun_Explosion> M03_Big_Gun_ExplosionRegistrant("M03_Big_Gun_Explosion", "");