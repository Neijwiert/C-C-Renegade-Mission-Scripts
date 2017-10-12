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
#include "M08_Nod_Turret.h"

/*
M03 -> 1145881
M08 -> 109628 109627
*/
void M08_Nod_Turret::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->attackingTarget, sizeof(this->attackingTarget), 1);
}

void M08_Nod_Turret::Created(GameObject *obj)
{
	Commands->Set_Player_Type(obj, SCRIPT_PLAYERTYPE_NOD);
	Commands->Enable_Enemy_Seen(obj, true);

	this->attackingTarget = false;
}

void M08_Nod_Turret::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (!this->attackingTarget)
	{
		if (Commands->Get_Player_Type(damager))
		{
			this->attackingTarget = true;

			ActionParamsStruct params;
			params.Set_Basic(this, 90.0f, 1);
			params.Set_Attack(damager, 250.0f, 0.0f, true);
			params.AttackCheckBlocked = false;

			Commands->Action_Attack(obj, params);

			Commands->Start_Timer(obj, this, 6.0f, 0);
		}
	}
}

void M08_Nod_Turret::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	if (!this->attackingTarget)
	{
		this->attackingTarget = true;

		ActionParamsStruct params;
		params.Set_Basic(this, 90.0f, 1);
		params.Set_Attack(enemy, 250.0f, 0.0f, true);
		params.AttackCheckBlocked = false;

		Commands->Action_Attack(obj, params);

		Commands->Start_Timer(obj, this, 6.0f, 0);
	}
}

void M08_Nod_Turret::Timer_Expired(GameObject *obj, int number)
{
	// After 6 seconds when damaged or enemy seen
	if (!number)
	{
		this->attackingTarget = false;
	}
}

ScriptRegistrant<M08_Nod_Turret> M08_Nod_TurretRegistrant("M08_Nod_Turret", "");