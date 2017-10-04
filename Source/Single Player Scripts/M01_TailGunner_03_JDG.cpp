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
#include "M01_TailGunner_03_JDG.h"

// This script is never used
void M01_TailGunner_03_JDG::Created(GameObject *obj)
{
	Commands->Innate_Disable(obj);

	GameObject *tailGun3Obj = Commands->Find_Object(108709);
	if (tailGun3Obj)
	{
		Vector3 tailGun3ObjPos = Commands->Get_Position(tailGun3Obj);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 38);
		params.Set_Movement(tailGun3ObjPos, 0.25f, 0.35f);

		Commands->Action_Goto(obj, params);
	}
}

void M01_TailGunner_03_JDG::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *tailGun3Obj = Commands->Find_Object(108709);
	if (tailGun3Obj)
	{
		Commands->Send_Custom_Event(obj, tailGun3Obj, 0, 230, 0.0f);
	}
}

void M01_TailGunner_03_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// When we moved to the tailgun on create
	if (action_id == 38 && complete_reason == ACTION_COMPLETE_NORMAL)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 39);

		Commands->Action_Enter_Exit(obj, params);
	}
}

ScriptRegistrant<M01_TailGunner_03_JDG> M01_TailGunner_03_JDGRegistrant("M01_TailGunner_03_JDG", "");