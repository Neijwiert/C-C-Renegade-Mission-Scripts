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
#include "MX0_Nod_Bunker_DLS.h"

void MX0_Nod_Bunker_DLS::Register_Auto_Save_Variables()
{

}

// On level start
void MX0_Nod_Bunker_DLS::Created(GameObject *obj)
{
	Commands->Set_Innate_Is_Stationary(obj, true);
}

void MX0_Nod_Bunker_DLS::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	ActionParamsStruct params;
	params.Set_Basic(this, 95.0f, 10);
	params.Set_Attack(enemy, 0.0f, 0.0f, true);
	params.AttackCheckBlocked = false;
	params.AttackCrouched = true;
	params.AttackForceFire = true;

	Commands->Action_Attack(obj, params);
}

ScriptRegistrant<MX0_Nod_Bunker_DLS> MX0_Nod_Bunker_DLSRegistrant("MX0_Nod_Bunker_DLS", "");