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
#include "M01_GDI_BeachGuy01_JDG.h"

// After 321 cinematic frames in x1h_hover_troop.txt
void M01_GDI_BeachGuy01_JDG::Created(GameObject *obj)
{
	Commands->Innate_Disable(obj);
	Commands->Enable_Hibernation(obj, false);

	ActionParamsStruct params;
	params.Set_Basic(this, 100.0f, 38);
	params.Set_Movement(Vector3(36.231f, 28.471f, 0.131f), 0.8f, 2.0f);

	Commands->Action_Goto(obj, params);
}

void M01_GDI_BeachGuy01_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (Commands->Get_Health(obj) <= 10.0f)
	{
		Vector3 pos = Commands->Get_Position(obj);
		if (damager == Commands->Get_A_Star(pos))
		{
			Commands->Set_Health(obj, 10.0f);
		}
	}
}

void M01_GDI_BeachGuy01_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// See on create
	if (complete_reason == ACTION_COMPLETE_NORMAL && action_id == 38)
	{
		Commands->Apply_Damage(obj, 10000.0f, "STEEL", NULL);
	}
}

ScriptRegistrant<M01_GDI_BeachGuy01_JDG> M01_GDI_BeachGuy01_JDGRegistrant("M01_GDI_BeachGuy01_JDG", "");