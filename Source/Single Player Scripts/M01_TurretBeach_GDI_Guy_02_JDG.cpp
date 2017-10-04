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
#include "M01_TurretBeach_GDI_Guy_02_JDG.h"

/*
x1e_hover_troop.txt at frame(s): 1
*/
void M01_TurretBeach_GDI_Guy_02_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered when we moved to location, see animation_complete
	if (action_id == 38)
	{
		Vector3 pos = Commands->Get_Position(obj);
		Commands->Set_Innate_Soldier_Home_Location(obj, pos, 10.0f);
	}
}

void M01_TurretBeach_GDI_Guy_02_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "S_A_Human.H_A_X1E_Run"))
	{
		GameObject *nonExistingObj = Commands->Find_Object(101448);
		if (nonExistingObj)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 38);
			params.Set_Movement(nonExistingObj, 0.8f, 0.5f);
			
			Commands->Action_Goto(obj, params);
		}
		else
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 80.0f, 38);
			params.Set_Movement(Vector3(-65.4f, 204.6f, 3.7f), 0.8f, 0.5f);

			Commands->Action_Goto(obj, params);
		}
	}
}

ScriptRegistrant<M01_TurretBeach_GDI_Guy_02_JDG> M01_TurretBeach_GDI_Guy_02_JDGRegistrant("M01_TurretBeach_GDI_Guy_02_JDG", "");