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
#include "M01_BarnArea_NOD_Commander_Trigger_Zone_JDG.h"

// This script is never used
void M01_BarnArea_NOD_Commander_Trigger_Zone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		GameObject *entranceBarnAreaTowerNodOfficerObj = Commands->Find_Object(102360);
		if (entranceBarnAreaTowerNodOfficerObj)
		{
			Commands->Send_Custom_Event(obj, entranceBarnAreaTowerNodOfficerObj, 0, 16, 0.0f);

			GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
			Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 93, 5.0f);
		}

		GameObject *nonExistingObj = Commands->Find_Object(102361);
		GameObject *M01BarnAreaNODCommanderTriggerZone02JDGObj = Commands->Find_Object(103343);

		if (nonExistingObj)
		{
			Commands->Destroy_Object(nonExistingObj);
		}

		if (M01BarnAreaNODCommanderTriggerZone02JDGObj)
		{
			Commands->Destroy_Object(M01BarnAreaNODCommanderTriggerZone02JDGObj);
		}
	}
}

ScriptRegistrant<M01_BarnArea_NOD_Commander_Trigger_Zone_JDG> M01_BarnArea_NOD_Commander_Trigger_Zone_JDGRegistrant("M01_BarnArea_NOD_Commander_Trigger_Zone_JDG", "");