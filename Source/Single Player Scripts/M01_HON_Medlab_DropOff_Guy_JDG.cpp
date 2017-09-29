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
#include "M01_HON_Medlab_DropOff_Guy_JDG.h"

// After 401/489 cinematic frames in X1I_HON_Medlab_Chinook.txt
void M01_HON_Medlab_DropOff_Guy_JDG::Created(GameObject *obj)
{
	Commands->Innate_Disable(obj);
	Commands->Enable_Hibernation(obj, false);
}

void M01_HON_Medlab_DropOff_Guy_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// When the movement of animation_complete or below is complete
	if (action_id == 38 && complete_reason == ACTION_COMPLETE_NORMAL)
	{
		Commands->Destroy_Object(obj);
	}
	else
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 38);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.8f, 1.0f);
		params.WaypathID = 125265;

		Commands->Action_Goto(obj, params);
	}
}

void M01_HON_Medlab_DropOff_Guy_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "S_A_Human.H_A_TroopDrop"))
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 38);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.8f, 1.0f);
		params.WaypathID = 125265;

		Commands->Action_Goto(obj, params);
	}
}

ScriptRegistrant<M01_HON_Medlab_DropOff_Guy_JDG> M01_HON_Medlab_DropOff_Guy_JDGRegistrant("M01_HON_Medlab_DropOff_Guy_JDG", "");