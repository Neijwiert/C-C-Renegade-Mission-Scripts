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
#include "M02_Nod_Vehicle.h"

// TODO
void M02_Nod_Vehicle::Created(GameObject *obj)
{
	Commands->Enable_Enemy_Seen(obj, true);
	Commands->Enable_Engine(obj, true);

	int areaId = Get_Int_Parameter("Area_ID");

	int wayPathId;
	switch (areaId)
	{
		case 2:
			wayPathId = 400375;

			break;
		case 3:
			wayPathId = 400416;

			break;
		case 4:
			wayPathId = 400454;

			break;
		default:
			wayPathId = -1;

			break;
	}

	if (wayPathId != -1)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 98.0f, 0);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 1.0f, 5.0f);
		params.WaypathID = wayPathId;
		params.WaypathSplined = false;

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		params.Set_Attack(starObj, 300.0f, 0.0f, true);

		Commands->Action_Attack(obj, params);
	}

	Commands->Start_Timer(obj, this, 15.0f, 1);
}

void M02_Nod_Vehicle::Sound_Heard(GameObject *obj, const CombatSound & sound)
{
	if (sound.sound == 1000)
	{
		if (sound.Creator)
		{
			Commands->Send_Custom_Event(obj, sound.Creator, 100, 1, 0.0f);
		}
	}
}

void M02_Nod_Vehicle::Timer_Expired(GameObject *obj, int number)
{
	// Triggered after 15 seconds on creation and 15 seconds after this block
	if (number == 1)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		if (starObj)
		{
			Vector3 starPos = Commands->Get_Position(starObj);
			if (Commands->Get_Distance(pos, starPos) > 300.0f)
			{
				Commands->Apply_Damage(obj, 10000.0f, "Blamokiller", NULL);
			}
		}

		Commands->Start_Timer(obj, this, 15.0f, 1);
	}
}

ScriptRegistrant<M02_Nod_Vehicle> M02_Nod_VehicleRegistrant("M02_Nod_Vehicle", "Area_ID:int");