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
#include "M01_Medium_Tank01_JDG.h"

// After 1 cinematic frame in X1D_FodderHover_MTank.txt
void M01_Medium_Tank01_JDG::Created(GameObject *obj)
{
	GameObject *M01HovercraftExplosionControllerJDGObj = Commands->Find_Object(121839);
	if (M01HovercraftExplosionControllerJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01HovercraftExplosionControllerJDGObj, 0, 228, 0.0f);
	}
}

ScriptRegistrant<M01_Medium_Tank01_JDG> M01_Medium_Tank01_JDGRegistrant("M01_Medium_Tank01_JDG", "");