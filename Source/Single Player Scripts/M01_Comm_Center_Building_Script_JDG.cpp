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
#include "M01_Comm_Center_Building_Script_JDG.h"

/*
M01 -> 153910
*/
void M01_Comm_Center_Building_Script_JDG::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
	Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 124, 0.0f);

	GameObject *M01AmbientSoundControllerJDGObj = Commands->Find_Object(100253);
	Commands->Send_Custom_Event(obj, M01AmbientSoundControllerJDGObj, 0, 124, 0.0f);

	GameObject *M01CommCenterMainFramePokeObj = Commands->Find_Object(107809);
	if (M01CommCenterMainFramePokeObj)
	{
		Commands->Send_Custom_Event(obj, M01CommCenterMainFramePokeObj, 0, 124, 0.0f);
	}

	GameObject *M01GateSwitchTutorialZoneJDGObj = Commands->Find_Object(120844);
	if (M01GateSwitchTutorialZoneJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01GateSwitchTutorialZoneJDGObj, 0, 16, 0.0f);
	}
}

void M01_Comm_Center_Building_Script_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (Commands->Find_Object(101949)) // commRepairEngineerObj
	{
		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 26, 0.0f);
	}
}

ScriptRegistrant<M01_Comm_Center_Building_Script_JDG> M01_Comm_Center_Building_Script_JDGRegistrant("M01_Comm_Center_Building_Script_JDG", "");