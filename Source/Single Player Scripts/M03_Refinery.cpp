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
#include "M03_Refinery.h"

/*
M03 -> 1150001
*/
void M03_Refinery::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);
	Commands->Send_Custom_Event(obj, M03ObjectiveControllerObj, 311, 3, 0.0f);
	Commands->Send_Custom_Event(obj, M03ObjectiveControllerObj, 311, 1, 0.0f);
}

ScriptRegistrant<M03_Refinery> M03_RefineryRegistrant("M03_Refinery", "");