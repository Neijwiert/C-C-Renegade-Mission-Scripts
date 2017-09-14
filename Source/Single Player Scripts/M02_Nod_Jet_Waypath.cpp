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
#include "M02_Nod_Jet_Waypath.h"

// When timer number 9 triggers in M02_Objective_Zone
void M02_Nod_Jet_Waypath::Created(GameObject *obj)
{
	Commands->Enable_Engine(obj, true);
	Commands->Disable_Physical_Collisions(obj);

	ActionParamsStruct params;
	params.Set_Basic(this, 100.0f, 1);
	params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 4.0f, 4.0f);
	params.WaypathID = 403389;
	params.WaypathSplined = true;

	Commands->Action_Goto(obj, params);
}

void M02_Nod_Jet_Waypath::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered when action from created is complete
	if (action_id == 1)
	{
		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M02_Nod_Jet_Waypath> M02_Nod_Jet_WaypathRegistrant("M02_Nod_Jet_Waypath", "");