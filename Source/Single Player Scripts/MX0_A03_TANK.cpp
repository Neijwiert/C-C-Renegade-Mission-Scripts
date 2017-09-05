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
#include "MX0_A03_TANK.h"

void MX0_A03_TANK::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(this->targetObjIds, sizeof(this->targetObjIds), 1);
	Auto_Save_Variable(&this->targetObjIndex, sizeof(this->targetObjIndex), 2);
	Auto_Save_Variable(&this->currentTargetObjId, sizeof(this->currentTargetObjId), 3);
}

// After 148 cinematic frames in xg_a03_tank_drop
void MX0_A03_TANK::Created(GameObject *obj)
{
	this->targetObjIndex = 1;

	Commands->Attach_Script(obj, "M00_Send_Object_ID", "1400041,9,0");
	Commands->Attach_Script(obj, "M00_Send_Object_ID", "1500017,3,1.0f");
	Commands->Attach_Script(obj, "M00_Send_Object_ID", "1500020,3,1.0f");

	this->targetObjIds[1] = 1400144; // left turret
	this->targetObjIds[2] = 1400146; // right turret
	this->targetObjIds[4] = 1400001; // Nod harvy
	this->targetObjIds[3] = 1400081; // Nod buggy

	Commands->Send_Custom_Event(obj, obj, 0, 0, 1.0f);
}

void MX0_A03_TANK::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received by ourselves. On created and action_complete
	if (!type)
	{
		this->currentTargetObjId = this->targetObjIds[this->targetObjIndex];
		GameObject *targetObj = Commands->Find_Object(this->currentTargetObjId);
		if (targetObj)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 90.0f, 0);
			params.Set_Attack(targetObj, 60.0f, 0.25f, true);

			Commands->Action_Attack(obj, params);
		}
	}
}

void MX0_A03_TANK::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered when a target kill action is completed
	if (!action_id)
	{
		this->targetObjIndex++;

		Commands->Send_Custom_Event(obj, obj, 0, 0, 1.0f);
	}
}

ScriptRegistrant<MX0_A03_TANK> MX0_A03_TANKRegistrant("MX0_A03_TANK", "");