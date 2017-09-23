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
#include "M01_TriggerZone_GDIBase_BaseCommander_JDG.h"

/*
M01 -> 106267 106268
*/
void M01_TriggerZone_GDIBase_BaseCommander_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		GameObject *captainDuncanObj = Commands->Find_Object(106050);
		if (captainDuncanObj)
		{
			Commands->Send_Custom_Event(obj, captainDuncanObj, 0, 16, 0.0f);
		}

		GameObject *M01TriggerZoneGDIBaseBaseCommanderJDG1Obj = Commands->Find_Object(106267);
		if (M01TriggerZoneGDIBaseBaseCommanderJDG1Obj)
		{
			Commands->Destroy_Object(M01TriggerZoneGDIBaseBaseCommanderJDG1Obj);
		}

		GameObject *M01TriggerZoneGDIBaseBaseCommanderJDG2Obj = Commands->Find_Object(106268);
		if (M01TriggerZoneGDIBaseBaseCommanderJDG2Obj)
		{
			Commands->Destroy_Object(M01TriggerZoneGDIBaseBaseCommanderJDG2Obj);
		}

		GameObject *nonExisting1Obj = Commands->Find_Object(105615);
		if (nonExisting1Obj)
		{
			Commands->Destroy_Object(nonExisting1Obj);
		}

		GameObject *nonExisting2Obj = Commands->Find_Object(113733);
		if (nonExisting2Obj)
		{
			Commands->Destroy_Object(nonExisting2Obj);
		}

		GameObject *nonExisting3Obj = Commands->Find_Object(103328);
		GameObject *nonExisting4Obj = Commands->Find_Object(103327);
		if (nonExisting3Obj)
		{
			if (nonExisting4Obj)
			{
				Commands->Set_Objective_Status(114, OBJECTIVE_STATUS_FAILED);
			}

			Commands->Destroy_Object(nonExisting3Obj);
		}

		if (nonExisting4Obj)
		{
			Commands->Destroy_Object(nonExisting4Obj);
		}

		GameObject *nonExisting5Obj = Commands->Find_Object(103334);
		if (nonExisting5Obj)
		{
			Commands->Destroy_Object(nonExisting5Obj);
		}

		GameObject *nonExisting6Obj = Commands->Find_Object(103329);
		if (nonExisting6Obj)
		{
			Commands->Destroy_Object(nonExisting6Obj);
		}

		GameObject *nonExisting7Obj = Commands->Find_Object(103330);
		if (nonExisting7Obj)
		{
			Commands->Destroy_Object(nonExisting7Obj);
		}

		GameObject *nonExisting8Obj = Commands->Find_Object(103332);
		if (nonExisting8Obj)
		{
			Commands->Destroy_Object(nonExisting8Obj);
		}

		GameObject *nonExisting9Obj = Commands->Find_Object(114348);
		if (nonExisting9Obj)
		{
			Commands->Destroy_Object(nonExisting9Obj);
		}

		GameObject *nonExisting10Obj = Commands->Find_Object(104685);
		if (nonExisting10Obj)
		{
			Commands->Send_Custom_Event(obj, nonExisting10Obj, 0, 2, 0.0f);
			Commands->Send_Custom_Event(obj, nonExisting10Obj, 0, 42, 0.0f);
		}
	}
}

ScriptRegistrant<M01_TriggerZone_GDIBase_BaseCommander_JDG> M01_TriggerZone_GDIBase_BaseCommander_JDGRegistrant("M01_TriggerZone_GDIBase_BaseCommander_JDG", "");