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
#include "M01_POW_Nod_Minigunner01_JDG.h"

/*
M01 -> 116388 116387
*/
void M01_POW_Nod_Minigunner01_JDG::Created(GameObject *obj)
{
	Commands->Set_Innate_Is_Stationary(obj, true);
}

void M01_POW_Nod_Minigunner01_JDG::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *gdiPOW1Obj = Commands->Find_Object(116386);
	if (gdiPOW1Obj)
	{
		Commands->Send_Custom_Event(obj, gdiPOW1Obj, 0, 22, 0.0f);
	}
}

ScriptRegistrant<M01_POW_Nod_Minigunner01_JDG> M01_POW_Nod_Minigunner01_JDGRegistrant("M01_POW_Nod_Minigunner01_JDG", "");