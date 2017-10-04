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
#include "M01_GDIBase_BackPath_NodGuy_JDG.h"

// This script is never used
void M01_GDIBase_BackPath_NodGuy_JDG::Created(GameObject *obj)
{
	Commands->Set_Innate_Is_Stationary(obj, true);
}

// Never used, so it never receives any customs
void M01_GDIBase_BackPath_NodGuy_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type && param == 16)
	{
		Commands->Set_Innate_Is_Stationary(obj, false);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 38);
		params.Set_Movement(Vector3(57.651f, 54.752f, -0.449f), 0.8f, 1.0f);

		Commands->Action_Goto(obj, params);
	}
}

void M01_GDIBase_BackPath_NodGuy_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered after movement complete when custom type <= 0 and param 16 is received
	if (action_id == 38)
	{
		Commands->Set_Innate_Is_Stationary(obj, true);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 20);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		params.Set_Attack(starObj, 50.0f, 3.0f, true);
		params.AttackCheckBlocked = true;

		Commands->Action_Attack(obj, params);
	}
}

ScriptRegistrant<M01_GDIBase_BackPath_NodGuy_JDG> M01_GDIBase_BackPath_NodGuy_JDGRegistrant("M01_GDIBase_BackPath_NodGuy_JDG", "");