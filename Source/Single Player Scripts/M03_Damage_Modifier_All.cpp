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
#include "M03_Damage_Modifier_All.h"

void M03_Damage_Modifier_All::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->modifiedCurrentHealth, sizeof(this->modifiedCurrentHealth), 1);
	Auto_Save_Variable(&this->unmodifiedCurrentHealth, sizeof(this->unmodifiedCurrentHealth), 2);
	Auto_Save_Variable(&this->calculatedDamage, sizeof(this->calculatedDamage), 3);
}

// After 1 frame in X3B_HoverXplode.txt attached to M03_Cinematic_Boss
void M03_Damage_Modifier_All::Created(GameObject *obj)
{
	this->unmodifiedCurrentHealth = Commands->Get_Health(obj);
	this->calculatedDamage = 0.0f;
}

void M03_Damage_Modifier_All::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	this->modifiedCurrentHealth = Commands->Get_Health(obj);

	float damage;
	if (this->modifiedCurrentHealth == 0.0f)
	{
		damage = this->unmodifiedCurrentHealth - this->modifiedCurrentHealth + this->calculatedDamage;
		this->calculatedDamage = 0.0;
	}
	else
	{
		damage = this->unmodifiedCurrentHealth - this->modifiedCurrentHealth;
		this->calculatedDamage = 0.0f;
	}
	
	float multipliedDamage = Get_Float_Parameter("Damage_multiplier") * damage;
	this->calculatedDamage += multipliedDamage;

	Commands->Set_Health(obj, this->unmodifiedCurrentHealth - this->calculatedDamage);

	this->unmodifiedCurrentHealth = Commands->Get_Health(obj);
	this->modifiedCurrentHealth = Commands->Get_Health(obj);
}

ScriptRegistrant<M03_Damage_Modifier_All> M03_Damage_Modifier_AllRegistrant("M03_Damage_Modifier_All", "Damage_multiplier:float");