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
#include "M03_Tailgun.h"

// This script is never used
void M03_Tailgun::Killed(GameObject *obj, GameObject *killer)
{
	int controllerId = Get_Int_Parameter("Controller_ID");

	GameObject *controllerObj = Commands->Find_Object(controllerId);
	if (controllerObj)
	{
		Commands->Send_Custom_Event(obj, controllerObj, 200, 200, 0.0f);
	}
}

ScriptRegistrant<M03_Tailgun> M03_TailgunRegistrant("M03_Tailgun", "Controller_ID:int");