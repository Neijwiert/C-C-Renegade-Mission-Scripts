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
#include "M02_Helipad.h"

// Created on level start
void M02_Helipad::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *M02ObjectiveControllerObj = Commands->Find_Object(1111112);
	if (M02ObjectiveControllerObj)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 206, 1, 0.0f);
	}
}

ScriptRegistrant<M02_Helipad> M02_HelipadRegistrant("M02_Helipad", "");