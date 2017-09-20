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
#include "M02_Approach_Vehicle.h"

/*
M02 -> 401008 401018 401038
*/
void M02_Approach_Vehicle::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M02_Objective_Zone with objId = (400267, 400187, 400185, 400189)
	if (!type && !param)
	{
		Commands->Enable_Engine(obj, true);

		int areaId = Get_Int_Parameter("Area_ID");

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 0);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f);

		switch (areaId)
		{
			case 1:
				params.WaypathID = 401009;

				break;
			case 4:
				params.WaypathID = 401031;

				break;
			case 6:
				params.WaypathID = 401039;

				break;
		}

		Commands->Action_Goto(obj, params);

		Commands->Start_Timer(obj, this, 6.0f, 0);
	}
}

void M02_Approach_Vehicle::Timer_Expired(GameObject *obj, int number)
{
	Commands->Apply_Damage(obj, 10000.0f, "Blamokiller", NULL);
}

ScriptRegistrant<M02_Approach_Vehicle> M02_Approach_VehicleRegistrant("M02_Approach_Vehicle", "Area_ID:int");