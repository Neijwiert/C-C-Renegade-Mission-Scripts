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
#include "M01_CommCenter_SAMSite_Script.h"

/*
M01 -> 100034
*/
void M01_CommCenter_SAMSite_Script::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
	Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 125, 0.0f);

	GameObject *penGateSwitch = Commands->Find_Object(101117);
	if (penGateSwitch)
	{
		Commands->Send_Custom_Event(obj, penGateSwitch, 0, 125, 0.0f);
	}
}

void M01_CommCenter_SAMSite_Script::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (damager == Commands->Get_A_Star(pos))
	{
		if (obj)
		{
			GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
			Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 189, 0.0f);
		}
	}
}

ScriptRegistrant<M01_CommCenter_SAMSite_Script> M01_CommCenter_SAMSite_ScriptRegistrant("M01_CommCenter_SAMSite_Script", "");