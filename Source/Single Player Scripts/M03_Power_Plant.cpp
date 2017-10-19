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
#include "M03_Power_Plant.h"

/*
M03 -> 1150003
*/
void M03_Power_Plant::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *commCenterBuildingObj = Commands->Find_Object(1150002);
	GameObject *refBuildingObj = Commands->Find_Object(1150001);

	Commands->Set_Building_Power(commCenterBuildingObj, false);
	Commands->Set_Building_Power(refBuildingObj, false);

	GameObject *nodHarvesterObj = Commands->Find_Object(1144518);
	Commands->Send_Custom_Event(obj, nodHarvesterObj, 7800, 7800, 0.0f);

	Commands->Enable_Radar(true);

	GameObject *M03CommCenterArrowObj = Commands->Find_Object(2009818);
	Commands->Send_Custom_Event(obj, M03CommCenterArrowObj, 40010, 0, 0.0f);
}

ScriptRegistrant<M03_Power_Plant> M03_Power_PlantRegistrant("M03_Power_Plant", "");