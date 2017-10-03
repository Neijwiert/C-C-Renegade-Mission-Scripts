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
#include "M01_Comm_Upstairs_Guard_JDG.h"

/*
M01 -> 101946
*/
void M01_Comm_Upstairs_Guard_JDG::Created(GameObject *obj)
{
	Commands->Innate_Disable(obj);
}

// TODO
void M01_Comm_Upstairs_Guard_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (param == 141)
	{
		Commands->Innate_Enable(obj);
		Commands->Set_Innate_Aggressiveness(obj, 0.75f);
		Commands->Set_Innate_Take_Cover_Probability(obj, 0.5f);

		ActionParamsStruct params;
		params.Set_Basic(this, 30.0f, 38);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.3f, 0.0f);
		params.WaypathID = 101940;
		params.WaypointStartID = 101944;
		params.WaypointEndID = 101941;

		Commands->Action_Goto(obj, params);
	}
}

// TODO
void M01_Comm_Upstairs_Guard_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_NORMAL)
	{
		if (action_id == 38)
		{
			float randDuration = Commands->Get_Random(1.0f, 10.0f);
			Commands->Start_Timer(obj, this, randDuration, 38);
		}
		else if (action_id == 39)
		{
			float randDuration = Commands->Get_Random(1.0f, 10.0f);
			Commands->Start_Timer(obj, this, randDuration, 39);
		}
	}
}

// TODO
void M01_Comm_Upstairs_Guard_JDG::Timer_Expired(GameObject *obj, int number)
{
	if (number == 38)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 30.0f, 39);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.3f, 0.0f);
		params.WaypathID = 101940;
		params.WaypointStartID = 101944;
		params.WaypointEndID = 101941;

		Commands->Action_Goto(obj, params);
	}
	else if (number == 39)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 30.0f, 38);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.3f, 0.0f);
		params.WaypathID = 101940;
		params.WaypointStartID = 101941;
		params.WaypointEndID = 101944;

		Commands->Action_Goto(obj, params);
	}
}

ScriptRegistrant<M01_Comm_Upstairs_Guard_JDG> M01_Comm_Upstairs_Guard_JDGRegistrant("M01_Comm_Upstairs_Guard_JDG", "");