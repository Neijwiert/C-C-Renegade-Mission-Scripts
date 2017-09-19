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
#include "M02_Destroy_Objective.h"

void M02_Destroy_Objective::Killed(GameObject *obj, GameObject *killer)
{
	int objectiveId = Get_Int_Parameter("Objective_ID");
	
	GameObject *M02ObjectiveControllerObj = Commands->Find_Object(1111112);
	if (M02ObjectiveControllerObj)
	{
		if (objectiveId != 222)
		{
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, objectiveId, 1, 0.0f);
		} 

		// SAM site
		if (objectiveId == 212)
		{
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 113, 3, 0.0f);
		}

		// SAM site
		else if (objectiveId == 214)
		{
			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(580.37f, 889.69f, -0.57f));
			if (invisObj)
			{
				Commands->Set_Facing(invisObj, -180.0f);
				Commands->Attach_Script(invisObj, "Test_Cinematic", "X2I_GDI_Drop02_Rocket_24.txt");
			}
		}

		// SAM sites
		else if (objectiveId == 215 || objectiveId == 216)
		{
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 115, 0, 0.0f);
		}

		// HON
		else if (objectiveId == 204)
		{
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 21, 0.0f);
		}

		// Tiberium silo
		else if (objectiveId == 222)
		{
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 116, 0, 0.0f);
		}
	}
}

void M02_Destroy_Objective::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (Get_Int_Parameter("Objective_ID") == 214)
	{
		GameObject *entranceSkiResortTowerNodEngineerObj = Commands->Find_Object(400510);
		if (entranceSkiResortTowerNodEngineerObj)
		{
			Commands->Send_Custom_Event(obj, entranceSkiResortTowerNodEngineerObj, 99, 214, 0.0f);
		}
	}
}

ScriptRegistrant<M02_Destroy_Objective> M02_Destroy_ObjectiveRegistrant("M02_Destroy_Objective", "Objective_ID:int");