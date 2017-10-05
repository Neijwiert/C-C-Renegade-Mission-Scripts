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
#include "M01_Church_Balcony_MiniGunner_JDG.h"

/*
M01 -> 101312
*/
void M01_Church_Balcony_MiniGunner_JDG::Created(GameObject *obj)
{
	Commands->Innate_Disable(obj);
}

void M01_Church_Balcony_MiniGunner_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_mission_Controller_JDG when param 148 is received
	if (param == 16)
	{
		if (obj)
		{
			Commands->Innate_Enable(obj);

			ActionParamsStruct params;
			params.Set_Basic(this, 45.0f, 38);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.3f, 0.25f);
			params.WaypathID = 100784;
			params.WaypointStartID = 100785;
			params.WaypointEndID = 100786;

			Commands->Action_Goto(obj, params);
		}
	}
}

void M01_Church_Balcony_MiniGunner_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
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
		// When done following waypath, see param 16 or action id 49
		if (action_id == 38)
		{
			int randAnimationIndex = Commands->Get_Random_Int(0, 14);

			ActionParamsStruct params;
			params.Set_Basic(this, 45.0f, 46);
			params.Set_Animation(animations[randAnimationIndex], false);

			Commands->Action_Play_Animation(obj, params);
		}

		// When done with animation, see action id 38
		else if (action_id == 46)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 45.0f, 39);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.3f, 0.25f);
			params.WaypathID = 100788;
			params.WaypointStartID = 100789;
			params.WaypointEndID = 100790;

			Commands->Action_Goto(obj, params);
		}

		// When done following waypath, see action id 46
		else if (action_id == 39)
		{
			int randAnimationIndex = Commands->Get_Random_Int(0, 14);

			ActionParamsStruct params;
			params.Set_Basic(this, 45.0f, 47);
			params.Set_Animation(animations[randAnimationIndex], false);

			Commands->Action_Play_Animation(obj, params);
		}

		// When done doing animation, see action id 39
		else if (action_id == 47 )
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 45.0f, 40);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.3f, 0.25f);
			params.WaypathID = 100792;
			params.WaypointStartID = 100793;
			params.WaypointEndID = 100794;

			Commands->Action_Goto(obj, params);
		}

		// When done following waypath, see action id 47
		else if (action_id == 40)
		{
			int randAnimationIndex = Commands->Get_Random_Int(0, 14);

			ActionParamsStruct params;
			params.Set_Basic(this, 45.0f, 48);
			params.Set_Animation(animations[randAnimationIndex], false);

			Commands->Action_Play_Animation(obj, params);
		}

		// When done doing animation, see action id 40
		else if (action_id == 48)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 45.0f, 41);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.3f, 0.25f);
			params.WaypathID = 100796;
			params.WaypointStartID = 100797;
			params.WaypointEndID = 100799;
			
			Commands->Action_Goto(obj, params);
		}

		// When done following waypath, see action id 48
		else if (action_id == 41)
		{
			int randAnimationIndex = Commands->Get_Random_Int(0, 14);

			ActionParamsStruct params;
			params.Set_Basic(this, 45.0f, 49);
			params.Set_Animation(animations[randAnimationIndex], false);

			Commands->Action_Play_Animation(obj, params);
		}

		// When done doing animation, see action id 41
		else if (action_id == 49)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 45.0f, 38);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.3f, 0.25f);
			params.WaypathID = 100784;
			params.WaypointStartID = 100785;
			params.WaypointEndID = 100786;
			
			Commands->Action_Goto(obj, params);
		}
	}
}

ScriptRegistrant<M01_Church_Balcony_MiniGunner_JDG> M01_Church_Balcony_MiniGunner_JDGRegistrant("M01_Church_Balcony_MiniGunner_JDG", "");