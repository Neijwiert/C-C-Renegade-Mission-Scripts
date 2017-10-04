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
#include "M01_GDI_Toolshed_PatrolGuy_JDG.h"

// This script is never used
void M01_GDI_Toolshed_PatrolGuy_JDG::Created(GameObject *obj)
{
	ActionParamsStruct params;
	params.Set_Basic(this, 30.0f, 38);
	params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.3f, 0.05f);
	params.WaypathID = 102153;
	params.WaypointStartID = 102154;
	params.WaypointEndID = 102157;

	Commands->Action_Goto(obj, params);
}

void M01_GDI_Toolshed_PatrolGuy_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	static const char *animations[14] = 
	{
		"H_A_A0A0_L01",
		"H_A_A0A0_L02",
		"H_A_A0A0_L03",
		"H_A_A0A0_L04",
		"H_A_A0A0_L05",
		"H_A_A0A0_L13",
		"H_A_X33C",
		"H_A_J33C",
		"H_A_J24C",
		"H_A_J22C",
		"H_A_J17C",
		"H_A_J15C",
		"H_A_J14C",
		"H_A_J11C"
	};
	if (complete_reason == ACTION_COMPLETE_NORMAL)
	{
		// When movement on create is done
		if (action_id == 38)
		{
			int randAnimationIndex = Commands->Get_Random_Int(0, 14);

			ActionParamsStruct params;
			params.Set_Basic(this, 30.0f, 46);
			params.Set_Animation(animations[randAnimationIndex], false);

			Commands->Action_Play_Animation(obj, params);
		}

		// When movement with action_id 38 is done
		else if (action_id == 46)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 30.0f, 39);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.3f, 0.05f);
			params.WaypathID = 102159;
			params.WaypointStartID = 102160;
			params.WaypointEndID = 102168;

			Commands->Action_Goto(obj, params);
		}

		// When movement with action_id 39 is done
		else if (action_id == 39)
		{
			int randAnimationIndex = Commands->Get_Random_Int(0, 14);

			ActionParamsStruct params;
			params.Set_Basic(this, 30.0f, 47);
			params.Set_Animation(animations[randAnimationIndex], false);

			Commands->Action_Play_Animation(obj, params);
		}

		// When movement with action_id 39 is done
		else if (action_id == 47)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 30.0f, 40);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.3f, 0.05f);
			params.WaypathID = 102176;
			params.WaypointStartID = 102177;
			params.WaypointEndID = 102183;

			Commands->Action_Goto(obj, params);
		}

		// When movement with action_id 47 is done
		else if (action_id == 40)
		{
			int randAnimationIndex = Commands->Get_Random_Int(0, 14);

			ActionParamsStruct params;
			params.Set_Basic(this, 30.0f, 48);
			params.Set_Animation(animations[randAnimationIndex], false);

			Commands->Action_Play_Animation(obj, params);
		}

		// When movement with action_id 40 is done
		else if (action_id == 48)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 30.0f, 41);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.3f, 0.05f);
			params.WaypathID = 102176;
			params.WaypointStartID = 102183;
			params.WaypointEndID = 102177;

			Commands->Action_Goto(obj, params);
		}

		// When movement with action_id 48 is done
		else if (action_id == 41)
		{
			int randAnimationIndex = Commands->Get_Random_Int(0, 14);

			ActionParamsStruct params;
			params.Set_Basic(this, 30.0f, 49);
			params.Set_Animation(animations[randAnimationIndex], false);

			Commands->Action_Play_Animation(obj, params);
		}

		// When movement with action_id 41 is done
		else if (action_id == 49)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 30.0f, 42);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.3f, 0.05f);
			params.WaypathID = 102159;
			params.WaypointStartID = 102168;
			params.WaypointEndID = 102160;

			Commands->Action_Goto(obj, params);
		}

		// When movement with action_id 49 is done
		else if (action_id == 42)
		{
			int randAnimationIndex = Commands->Get_Random_Int(0, 14);

			ActionParamsStruct params;
			params.Set_Basic(this, 30.0f, 50);
			params.Set_Animation(animations[randAnimationIndex], false);

			Commands->Action_Play_Animation(obj, params);
		}

		// When movement with action_id 42 is done
		else if (action_id == 50)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 30.0f, 43);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.3f, 0.05f);
			params.WaypathID = 102153;
			params.WaypointStartID = 102157;
			params.WaypointEndID = 102154;

			Commands->Action_Goto(obj, params);
		}

		// When movement with action_id 50 is done
		else if (action_id == 43)
		{
			int randAnimationIndex = Commands->Get_Random_Int(0, 14);

			ActionParamsStruct params;
			params.Set_Basic(this, 30.0f, 51);
			params.Set_Animation(animations[randAnimationIndex], false);

			Commands->Action_Play_Animation(obj, params);
		}

		// When movement with action_id 43 is done
		else if (action_id == 51)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 30.0f, 38);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.3f, 0.05f);
			params.WaypathID = 102153;
			params.WaypointStartID = 102154;
			params.WaypointEndID = 102157;

			Commands->Action_Goto(obj, params);
		}
	}
}

ScriptRegistrant<M01_GDI_Toolshed_PatrolGuy_JDG> M01_GDI_Toolshed_PatrolGuy_JDGRegistrant("M01_GDI_Toolshed_PatrolGuy_JDG", "");