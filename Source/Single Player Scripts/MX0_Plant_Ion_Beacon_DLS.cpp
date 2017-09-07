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
#include "MX0_Plant_Ion_Beacon_DLS.h"

void MX0_Plant_Ion_Beacon_DLS::Register_Auto_Save_Variables()
{

}

// When MX0_Area4_Controller_DLS triggered timer with number 6
void MX0_Plant_Ion_Beacon_DLS::Created(GameObject *obj)
{
	Commands->Give_PowerUp(obj, "POW_IonCannonBeacon_Ai", false);
	Commands->Select_Weapon(obj, "Weapon_IonCannonBeacon_Ai");

	ActionParamsStruct params;
	params.Set_Basic(this, 95.0f, 10);
	
	Vector3 pos = Commands->Get_Position(obj);

	params.Set_Attack(pos, 5.0f, 0.0f, true);
	params.AttackForceFire = true;

	Commands->Action_Attack(obj, params);
}

ScriptRegistrant<MX0_Plant_Ion_Beacon_DLS> MX0_Plant_Ion_Beacon_DLSRegistrant("MX0_Plant_Ion_Beacon_DLS", "");