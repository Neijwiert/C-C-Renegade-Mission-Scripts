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
#include "M01_GDIBase_FirstChinookFlamethrowerGuy_JDG.h"

// This script is never used
void M01_GDIBase_FirstChinookFlamethrowerGuy_JDG::Created(GameObject *obj)
{
	GameObject *nonExisting1Obj = Commands->Find_Object(104685);
	GameObject *nonExisting2Obj = Commands->Find_Object(104686);

	if (nonExisting1Obj)
	{
		Commands->Send_Custom_Event(obj, nonExisting1Obj, 0, 31, 0.0f);
	}

	if (nonExisting2Obj)
	{
		Commands->Send_Custom_Event(obj, nonExisting2Obj, 0, 31, 0.0f);
	}
}

ScriptRegistrant<M01_GDIBase_FirstChinookFlamethrowerGuy_JDG> M01_GDIBase_FirstChinookFlamethrowerGuy_JDGRegistrant("M01_GDIBase_FirstChinookFlamethrowerGuy_JDG", "");