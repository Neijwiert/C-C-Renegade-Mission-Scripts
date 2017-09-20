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
#include "M00_No_Falling_Damage_DME.h"

void M00_No_Falling_Damage_DME::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->fell, sizeof(this->fell), 1);
	Auto_Save_Variable(&this->healthBeforeFallingDmgApplied, sizeof(this->healthBeforeFallingDmgApplied), 2);
}

void M00_No_Falling_Damage_DME::Created(GameObject *obj)
{
	this->fell = false;
}

void M00_No_Falling_Damage_DME::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (this->fell && !damager)
	{
		Commands->Set_Health(obj, this->healthBeforeFallingDmgApplied);
	}

	this->fell = false;
}

void M00_No_Falling_Damage_DME::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_FALLING_DAMAGE)
	{
		this->fell = true;
		this->healthBeforeFallingDmgApplied = Commands->Get_Health(obj);
	}
}

ScriptRegistrant<M00_No_Falling_Damage_DME> M00_No_Falling_Damage_DMERegistrant("M00_No_Falling_Damage_DME", "");