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
#include "M01_SniperRifle_01_Target_JDG.h"

// When M01_SniperRifle_01_JDG is picked up, see custom
void M01_SniperRifle_01_Target_JDG::Created(GameObject *obj)
{
	Commands->Attach_Script(obj, "M00_Soldier_Powerup_Disable", "");
	Commands->Enable_Hibernation(obj, false);

	ActionParamsStruct params;
	params.Set_Basic(this, 90.0f, 38);
	params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.3f, 0.25f);
	params.WaypathID = 118434;

	Commands->Action_Goto(obj, params);
}

void M01_SniperRifle_01_Target_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered when done following waypath, see on create
	if (complete_reason == ACTION_COMPLETE_NORMAL && action_id == 38)
	{
		Commands->Enable_Hibernation(obj, true);
		Commands->Set_Innate_Is_Stationary(obj, true);
	}
}

ScriptRegistrant<M01_SniperRifle_01_Target_JDG> M01_SniperRifle_01_Target_JDGRegistrant("M01_SniperRifle_01_Target_JDG", "");