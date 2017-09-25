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
#include "M00_VisceroidInnate_DAY.h"

/*
M01 -> 106907 106908 106909
M03 -> 1204952 1204951
M11 -> 101996 101995 100394 101402 101403
*/
void M00_VisceroidInnate_DAY::Created(GameObject *obj)
{
	Commands->Set_Loiters_Allowed(obj, false);

	Commands->Set_Animation(obj, "C_Visceroid.C_Visceroid", true, NULL, 0.0f, -1.0f, false);
}

void M00_VisceroidInnate_DAY::Killed(GameObject *obj, GameObject *killer)
{
	Commands->Set_Animation(obj, "C_Visceroid.C_Visceroid_Die", false, NULL, 0.0f, -1.0f, false);
}

ScriptRegistrant<M00_VisceroidInnate_DAY> M00_VisceroidInnate_DAYRegistrant("M00_VisceroidInnate_DAY", "");