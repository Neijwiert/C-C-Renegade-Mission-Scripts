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
#include "M02_Destroy_Vehicle.h"

// After 178 cinematic frames in x2f_orcasam.txt
void M02_Destroy_Vehicle::Created(GameObject *obj)
{
	Commands->Enable_Engine(obj, true);
	Commands->Disable_Physical_Collisions(obj);

	Commands->Start_Timer(obj, this, 4.0f, 1);
}

void M02_Destroy_Vehicle::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (damager != Commands->Get_A_Star(pos))
	{
		Commands->Apply_Damage(obj, 10000.0f, "Blamokiller", NULL);
	}
}

void M02_Destroy_Vehicle::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	ActionParamsStruct params;
	params.Set_Basic(this, 90.0f, 1);
	params.Set_Attack(enemy, 200.0f, 0.0f, true);
	params.AttackActive = true;

	Commands->Action_Attack(obj, params);
}

void M02_Destroy_Vehicle::Timer_Expired(GameObject *obj, int number)
{
	// Triggered after 4 seconds on creation
	if (number == 1)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 99.0f, 0);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 2.5f, 1.0f);
		params.WaypathID = 400993;
		params.WaypathSplined = true;

		Commands->Action_Goto(obj, params);

		Commands->Start_Timer(obj, this, 20.0f, 2);
	}

	// Triggered after 20 seconds after timer number 1
	else if (number == 2)
	{
		Commands->Apply_Damage(obj, 10000.0f, "Blamokiller", NULL);
	}
}

ScriptRegistrant<M02_Destroy_Vehicle> M02_Destroy_VehicleRegistrant("M02_Destroy_Vehicle", "");