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
#include "M01_Comm_Stationary_Tech_JDG.h"

/*
M01 -> 101171 101170 101168
*/
void M01_Comm_Stationary_Tech_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->spawnLocation, sizeof(this->spawnLocation), 1);
}

void M01_Comm_Stationary_Tech_JDG::Created(GameObject *obj)
{
	this->spawnLocation = Commands->Get_Position(obj);

	Commands->Innate_Disable(obj);
}

// TODO
void M01_Comm_Stationary_Tech_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (param == 141)
	{
		Commands->Innate_Enable(obj);
		Commands->Set_Innate_Aggressiveness(obj, 0.15000001f);
		Commands->Set_Innate_Take_Cover_Probability(obj, 1.0f);

		ActionParamsStruct params;
		params.Set_Basic(this, 30.0f, 38);
		params.Set_Movement(this->spawnLocation, 0.3f, 0.0f);

		Commands->Action_Goto(obj, params);
	}
}

// TODO
void M01_Comm_Stationary_Tech_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_NORMAL)
	{
		if (action_id == 38)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 30.0f, 46);
			params.Set_Animation("H_A_CON2", false);

			Commands->Action_Play_Animation(obj, params);
		}
		else if (action_id == 46)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 30.0f, 38);
			params.Set_Movement(this->spawnLocation, 0.3f, 0.0f);

			Commands->Action_Goto(obj, params);
		}
	}
}

ScriptRegistrant<M01_Comm_Stationary_Tech_JDG> M01_Comm_Stationary_Tech_JDGRegistrant("M01_Comm_Stationary_Tech_JDG", "");