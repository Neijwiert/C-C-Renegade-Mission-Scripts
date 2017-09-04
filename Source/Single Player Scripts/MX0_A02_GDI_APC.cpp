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
#include "MX0_A02_GDI_APC.h"

// After MX0_A02_Controller receives custom type 202
void MX0_A02_GDI_APC::Created(GameObject *obj)
{
	this->canBeDamaged = false;
}

void MX0_A02_GDI_APC::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (!this->canBeDamaged)
	{
		float maxHealth = Commands->Get_Max_Health(obj);
		Commands->Set_Health(obj, maxHealth);
	}
}

void MX0_A02_GDI_APC::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from MX0_A02_Controller when custom type 240 has been received
	if (type == 210)
	{
		this->canBeDamaged = true;

		Commands->Apply_Damage(obj, 10000.0f, "Blamokiller", sender);
	}

	// Received from MX0_A02_Controller when custom 215 has been received
	else if (type == 215)
	{
		Commands->Innate_Enable(obj);
		Commands->Enable_Enemy_Seen(obj, true);
	}
}

void MX0_A02_GDI_APC::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	ActionParamsStruct params;
	params.Set_Basic(this, 90.0f, 201);
	params.Set_Attack(enemy, 300.0f, 0.0f, true);
	params.AttackCheckBlocked = false;

	Commands->Action_Attack(obj, params);
}

ScriptRegistrant<MX0_A02_GDI_APC> MX0_A02_GDI_APCRegistrant("MX0_A02_GDI_APC", "");