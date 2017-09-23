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
#include "M01_GuardTower02_NewSniperTarget_JDG.h"

void M01_GuardTower02_NewSniperTarget_JDG::Created(GameObject *obj)
{
	Commands->Enable_Hibernation(obj, false);

	ActionParamsStruct params;
	params.Set_Basic(this, 100.0f, 38);
	params.Set_Movement(Vector3(-67.347f, 118.048f, 2.641f), 0.8f, 1.0f);

	Commands->Action_Goto(obj, params);
}

void M01_GuardTower02_NewSniperTarget_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received by ourselves after 1 second, see Action_Complete with action_id = 38
	if (!type && param == 63)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 47);
		params.Set_Animation("H_A_A0A0_L34", false);

		Commands->Action_Play_Animation(obj, params);
	}
}

void M01_GuardTower02_NewSniperTarget_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_NORMAL)
	{
		// Triggered after we are done moving at create
		if (action_id == 38)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 63);

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			params.Set_Attack(starObj, 0.0f, 0.0f, true);

			Commands->Action_Attack(obj, params);

			Commands->Send_Custom_Event(obj, obj, 0, 63, 1.0f);
		}

		// Triggered after we are done with the animation, see custom
		else if (action_id == 47)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 48);
			params.Set_Animation("H_A_A0A0_L56", false);

			Commands->Action_Play_Animation(obj, params);
		}

		// Triggered after we are done with the animation, see action_id = 47
		else if (action_id == 48)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 50.0f, 63);

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			params.Set_Attack(starObj, 20.0f, 0.0f, true);
			params.Set_Movement(starObj, 0.8f, 5.0f);

			Commands->Action_Attack(obj, params);
		}
	}
}

ScriptRegistrant<M01_GuardTower02_NewSniperTarget_JDG> M01_GuardTower02_NewSniperTarget_JDGRegistrant("M01_GuardTower02_NewSniperTarget_JDG", "");