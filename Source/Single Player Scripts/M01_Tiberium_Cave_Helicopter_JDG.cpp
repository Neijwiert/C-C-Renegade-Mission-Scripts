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
#include "M01_Tiberium_Cave_Helicopter_JDG.h"

// When M01_Mission_Controller_JDG receives custom type 0 with param 195
void M01_Tiberium_Cave_Helicopter_JDG::Created(GameObject *obj)
{
	Commands->Innate_Disable(obj);
	Commands->Enable_Hibernation(obj, false);
	Commands->Disable_Physical_Collisions(obj);

	ActionParamsStruct params;
	params.Set_Basic(this, 100.0f, 16);
	params.Set_Movement(Vector3(-42.883f, 376.457f, 18.025f), 0.005f, 0.25f);
	params.AttackActive = false;
	params.AttackCheckBlocked = false;
	params.MovePathfind = false;
	Commands->Action_Attack(obj, params);

	float randDelay = Commands->Get_Random(1.0f, 2.0f);
	Commands->Send_Custom_Event(obj, obj, 0, 27, randDelay);
}

void M01_Tiberium_Cave_Helicopter_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from ourselves after a random delay of 1 to 2 seconds, see created
	if (param == 27)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 16);
		params.Set_Movement(Vector3(-42.883f, 376.457f, 18.025f), 0.05f, 0.25f);
		params.AttackActive = true;
		params.AttackCheckBlocked = false;
		params.MovePathfind = false;

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		params.Set_Attack(starObj, 100.0f, 0.0f, true);

		Commands->Modify_Action(obj, 16, params, true, true);

		float randDelay = Commands->Get_Random(1.0f, 2.0f);
		Commands->Send_Custom_Event(obj, obj, 0, 28, randDelay);
	}

	// Received from ourselves after a random delay of 1 to 2 seconds, see param 27
	else if (param == 28)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 38);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 1.0f, 0.75f);
		params.WaypathID = 100964;
		params.WaypointStartID = 100965;
		params.WaypointEndID = 101366;
		params.WaypathSplined = true;
		params.AttackActive = true;
		params.AttackCheckBlocked = true;
		params.MovePathfind = false;

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		params.Set_Attack(starObj, 100.0f, 0.0f, true);

		Commands->Action_Attack(obj, params);

		Commands->Send_Custom_Event(obj, obj, 0, 29, 15.0f);
	}

	// Received from ourselves after 15 seconds, see param 28
	else if (param == 29)
	{
		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_Tiberium_Cave_Helicopter_JDG> M01_Tiberium_Cave_Helicopter_JDGRegistrant("M01_Tiberium_Cave_Helicopter_JDG", "");