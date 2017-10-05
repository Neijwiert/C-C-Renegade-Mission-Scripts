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
#include "M01_HON_Chinook_Spawned_Soldier_03_GDI_JDG.h"

// After 401 cinematic frames in X1I_GDI_HON_TroopDrop01b.txt
void M01_HON_Chinook_Spawned_Soldier_03_GDI_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->nodBuggyObjId, sizeof(this->nodBuggyObjId), 1);
}

void M01_HON_Chinook_Spawned_Soldier_03_GDI_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_BuggyNew_Controller_JDG when param 27 is received with param nodBuggyObjId
	if (type == 27)
	{
		this->nodBuggyObjId = param;

		GameObject *paramObj = Commands->Find_Object(param);
		if (paramObj)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 17);
			params.Set_Attack(paramObj, 20.0f, 3.0f, true);

			Commands->Action_Attack(obj, params);
		}
	}
}

void M01_HON_Chinook_Spawned_Soldier_03_GDI_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered when movement complete, see animation_complete
	if (action_id == 38)
	{
		GameObject *M01BuggyNewControllerJDGObj = Commands->Find_Object(103367);
		if (M01BuggyNewControllerJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01BuggyNewControllerJDGObj, 0, 27, 0.0f);
		}
	}
}

void M01_HON_Chinook_Spawned_Soldier_03_GDI_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "s_a_human.h_a_troopdrop"))
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 38);
		params.Set_Movement(Vector3(-209.853f, 505.53201f, 4.0170002f), 0.8f, 3.0f);

		Commands->Action_Goto(obj, params);
	}
}

ScriptRegistrant<M01_HON_Chinook_Spawned_Soldier_03_GDI_JDG> M01_HON_Chinook_Spawned_Soldier_03_GDI_JDGRegistrant("M01_HON_Chinook_Spawned_Soldier_03_GDI_JDG", "");