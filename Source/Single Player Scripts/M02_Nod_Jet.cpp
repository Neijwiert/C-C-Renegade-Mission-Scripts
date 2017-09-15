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
#include "M02_Nod_Jet.h"

// When script zone with id 401982 at M02_Objective_Zone is entered
void M02_Nod_Jet::Created(GameObject *obj)
{
	Commands->Enable_Engine(obj, true);
	Commands->Disable_Physical_Collisions(obj);

	ActionParamsStruct params;
	params.Set_Basic(this, 100.0f, 1);
	params.Set_Movement(Vector3(1143.09f, 702.08f, 70.5f), 4.0f, 5.0f);

	Commands->Action_Goto(obj, params);
}

void M02_Nod_Jet::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered when we moved to our location, see created
	if (action_id == 1)
	{
		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M02_Nod_Jet> M02_Nod_JetRegistrant("M02_Nod_Jet", "");