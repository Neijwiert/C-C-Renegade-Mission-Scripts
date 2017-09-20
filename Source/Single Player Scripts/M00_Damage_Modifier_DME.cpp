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
#include "M00_Damage_Modifier_DME.h"

/*
M03 -> 1100003
M13 -> 1400001 1500016 1500015
*/
void M00_Damage_Modifier_DME::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->modifiedCurrentHealth, sizeof(this->modifiedCurrentHealth), 1);
	Auto_Save_Variable(&this->unmodifiedCurrentHealth, sizeof(this->unmodifiedCurrentHealth), 2);
	Auto_Save_Variable(&this->calculatedDamage, sizeof(this->calculatedDamage), 3);
	Auto_Save_Variable(&this->killableByStar, sizeof(this->killableByStar), 4);
	Auto_Save_Variable(&this->killableByNotStar, sizeof(this->killableByNotStar), 5);
	Auto_Save_Variable(&this->starModifier, sizeof(this->starModifier), 6);
	Auto_Save_Variable(&this->notStarModifier, sizeof(this->notStarModifier), 7);
	Auto_Save_Variable(&this->enabled, sizeof(this->enabled), 8);
}

void M00_Damage_Modifier_DME::Created(GameObject *obj)
{
	this->enabled = true;
	this->unmodifiedCurrentHealth = Commands->Get_Health(obj);
	this->calculatedDamage = 0.0f;
	this->killableByStar = Get_Int_Parameter("Killable_By_Star");
	this->killableByNotStar = Get_Int_Parameter("Killable_ByNotStar"); // Yes, this is a typo on purpose
	this->starModifier = Get_Int_Parameter("Star_Modifier");
	this->notStarModifier = Get_Int_Parameter("NotStar_Modifier");
}

void M00_Damage_Modifier_DME::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (this->enabled)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *star = Commands->Get_A_Star(pos);

		if (!this->starModifier && damager == star && this->killableByStar || !this->notStarModifier && damager != star && this->killableByNotStar)
		{
			this->unmodifiedCurrentHealth = Commands->Get_Health(obj);
		}

		if (this->starModifier && damager == star && this->killableByStar || this->notStarModifier && damager != star && this->killableByNotStar)
		{
			this->modifiedCurrentHealth = Commands->Get_Health(obj);

			float damage;
			if (this->modifiedCurrentHealth == 0.0f)
			{
				damage = this->unmodifiedCurrentHealth - this->modifiedCurrentHealth + this->calculatedDamage;
			}
			else
			{
				damage = this->unmodifiedCurrentHealth - this->modifiedCurrentHealth;
			}

			float multipliedDamage = Get_Float_Parameter("Damage_multiplier") * damage;

			this->calculatedDamage = multipliedDamage + this->calculatedDamage;

			Commands->Set_Health(obj, this->unmodifiedCurrentHealth - multipliedDamage);

			this->unmodifiedCurrentHealth = Commands->Get_Health(obj);
			this->modifiedCurrentHealth = Commands->Get_Health(obj);
		}
			
		if (this->starModifier && damager == star && !this->killableByStar || this->notStarModifier && damager != star && !this->killableByNotStar)
		{
			this->modifiedCurrentHealth = Commands->Get_Health(obj);

			float damage = this->unmodifiedCurrentHealth - this->modifiedCurrentHealth;
			float multipliedDamage = this->unmodifiedCurrentHealth - Get_Float_Parameter("Damage_multiplier") * damage;

			Commands->Set_Health(obj, multipliedDamage);

			this->unmodifiedCurrentHealth = Commands->Get_Health(obj);
			this->modifiedCurrentHealth = Commands->Get_Health(obj);
		}
	}
}

void M00_Damage_Modifier_DME::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Used to enable/disable the damage modifier
	if (type == 9037)
	{
		switch (param)
		{
			case 1:
				this->enabled = true;
				break;
			default:
				this->enabled = false;
				break;
		}
	}
}

ScriptRegistrant<M00_Damage_Modifier_DME> M00_Damage_Modifier_DMERegistrant("M00_Damage_Modifier_DME", "Damage_multiplier:float, Star_Modifier=1:int, NotStar_Modifier=1:int, Killable_By_Star=1:int, Killable_by_NotStar=1:int");