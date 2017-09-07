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
#include "MX0_Nod_RocketSoldier_DLS.h"

void MX0_Nod_RocketSoldier_DLS::Register_Auto_Save_Variables()
{

}

// When spawned at the spawner (On the wall at the Nod base)
void MX0_Nod_RocketSoldier_DLS::Created(GameObject *obj)
{
	ActionParamsStruct params;
	params.Set_Basic(this, 95.0f, 0);

	int stationaryPoint = Get_Int_Parameter("Stationary_Point");
	GameObject *stationaryPointObj = Commands->Find_Object(stationaryPoint);

	params.Set_Movement(stationaryPointObj, 0.8f, 0.0f);

	Commands->Action_Goto(obj, params);
}

void MX0_Nod_RocketSoldier_DLS::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from MX0_Area4_Controller_DLS when timer number 4 triggers
	if (type == 445002)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 95.0f, 10);
		
		GameObject *paramObj = Commands->Find_Object(param);

		params.Set_Attack(paramObj, 150.0f, 0.0f, true);
		params.AttackCheckBlocked = false;
		params.AttackForceFire = true;

		Commands->Action_Attack(obj, params);
	}
}

void MX0_Nod_RocketSoldier_DLS::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	ActionParamsStruct params;
	params.Set_Basic(this, 95.0f, 10);
	params.Set_Attack(enemy, 150.0f, 0.0f, true);
	params.AttackCheckBlocked = false;
	params.AttackForceFire = true;

	Commands->Action_Attack(obj, params);
}

void MX0_Nod_RocketSoldier_DLS::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered when we moved to the stationary point on creation
	if (!action_id && complete_reason == ACTION_COMPLETE_NORMAL)
	{
		Commands->Set_Innate_Is_Stationary(obj, true);
	}
}

ScriptRegistrant<MX0_Nod_RocketSoldier_DLS> MX0_Nod_RocketSoldier_DLSRegistrant("MX0_Nod_RocketSoldier_DLS", "Stationary_Point=0:int");