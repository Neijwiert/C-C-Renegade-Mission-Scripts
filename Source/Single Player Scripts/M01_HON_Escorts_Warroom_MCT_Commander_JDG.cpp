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
#include "M01_HON_Escorts_Warroom_MCT_Commander_JDG.h"

/*
M01 -> 103373
*/
void M01_HON_Escorts_Warroom_MCT_Commander_JDG::Created(GameObject *obj)
{
	Commands->Set_Innate_Is_Stationary(obj, true);
}

// TODO
void M01_HON_Escorts_Warroom_MCT_Commander_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_HON_Escorts_Warroom_MCT_ZoneController_JDG when entered
	if (param == 16)
	{
		Commands->Set_Innate_Is_Stationary(obj, false);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 38);
		params.Set_Movement(Vector3(-181.10001f, 541.40002f, 3.5f), 0.8f, 0.25f);

		Commands->Action_Goto(obj, params);
	}
}

// TODO
void M01_HON_Escorts_Warroom_MCT_Commander_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (action_id == 38)
	{
		Commands->Set_Innate_Is_Stationary(obj, true);

		ActionParamsStruct params;
		params.Set_Basic(this, 45.0f, 17);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		params.Set_Attack(starObj, 50.0f, 0.0f, false);

		Commands->Action_Attack(obj, params);
	}
}

ScriptRegistrant<M01_HON_Escorts_Warroom_MCT_Commander_JDG> M01_HON_Escorts_Warroom_MCT_Commander_JDGRegistrant("M01_HON_Escorts_Warroom_MCT_Commander_JDG", "");