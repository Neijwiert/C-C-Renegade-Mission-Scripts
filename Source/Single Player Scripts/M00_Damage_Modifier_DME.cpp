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

void M00_Damage_Modifier_DME::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 2);
	Auto_Save_Variable(&this->field_24, sizeof(this->field_24), 3);
	Auto_Save_Variable(&this->killableByStar, sizeof(this->killableByStar), 4);
	Auto_Save_Variable(&this->killableByNotStar, sizeof(this->killableByNotStar), 5);
	Auto_Save_Variable(&this->starModifier, sizeof(this->starModifier), 6);
	Auto_Save_Variable(&this->notStarModifier, sizeof(this->notStarModifier), 7);
	Auto_Save_Variable(&this->enabled, sizeof(this->enabled), 8);
}

void M00_Damage_Modifier_DME::Created(GameObject *obj)
{
	this->enabled = true;
	this->field_20 = Commands->Get_Health(obj);
	this->field_24 = 0.0f;
	this->killableByStar = Get_Int_Parameter("Killable_By_Star");
	this->killableByNotStar = Get_Int_Parameter("Killable_ByNotStar"); // Yes, this is a typo on purpose
	this->starModifier = Get_Int_Parameter("Star_Modifier");
	this->notStarModifier = Get_Int_Parameter("NotStar_Modifier");
}

void M00_Damage_Modifier_DME::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	/*
	if (this->enabled)
	{
		v29 = 0;
		if (!this->starModifier
			&& (Commands2 = Commands,
				obj_1 = this->base.base.base.vPtr->base.Owner(&this->base.base),
				Commands2->Get_Position(&pos, obj_1),
				damager == Commands2->Get_A_Star(&pos))
			&& this->killableByStar
			|| !this->notStarModifier
			&& (Commands3 = Commands,
				obj_2 = this->base.base.base.vPtr->base.Owner(&this->base.base),
				Commands3->Get_Position(&pos_1, obj_2),
				damager != Commands3->Get_A_Star(&pos_1))
			&& this->killableByNotStar)
		{
			v29 = 1;
		}
		if (v29)
			this->field_20 = Commands->Get_Health(obj);
		v28 = 0;
		if (this->starModifier
			&& (Commands4 = Commands,
				obj_3 = this->base.base.base.vPtr->base.Owner(&this->base.base),
				Commands4->Get_Position(&pos_1, obj_3),
				damager == Commands4->Get_A_Star(&pos_1))
			&& this->killableByStar
			|| this->notStarModifier
			&& (Commands5 = Commands,
				obj_4 = this->base.base.base.vPtr->base.Owner(&this->base.base),
				Commands5->Get_Position(&pos, obj_4),
				damager != Commands5->Get_A_Star(&pos))
			&& this->killableByNotStar)
		{
			v28 = 1;
		}
		if (v28)
		{
			health_1 = Commands->Get_Health(obj);
			this->field_1C = health_1;
			v14 = health_1 < 0.0;
			v15 = 0;
			v16 = health_1 == 0.0;
			if ((HIBYTE(v13) & 0x45) == 64)
				v17 = this->field_20 - this->field_1C + this->field_24;
			else
				v17 = this->field_20 - this->field_1C;
			v18 = v17;
			v19 = ScriptImpClass::Get_Float_Parameter(&this->base, "Damage_multiplier") * v18;
			this->field_24 = v19 + this->field_24;
			v20 = this->field_20 - v19;
			Commands->Set_Health(obj, v20);
			this->field_20 = Commands->Get_Health(obj);
			this->field_1C = Commands->Get_Health(obj);
		}
		v27 = 0;
		if (this->starModifier
			&& (Commands6 = Commands,
				obj_5 = this->base.base.base.vPtr->base.Owner(&this->base.base),
				Commands6->Get_Position(&pos_1, obj_5),
				damager == Commands6->Get_A_Star(&pos_1))
			&& !this->killableByStar
			|| this->notStarModifier
			&& (Commands7 = Commands,
				obj_6 = this->base.base.base.vPtr->base.Owner(&this->base.base),
				Commands7->Get_Position(&pos, obj_6),
				damager != Commands7->Get_A_Star(&pos))
			&& !this->killableByNotStar)
		{
			v27 = 1;
		}
		if (v27)
		{
			this->field_1C = Commands->Get_Health(obj);
			v25 = this->field_20 - this->field_1C;
			v26 = this->field_20 - ScriptImpClass::Get_Float_Parameter(&this->base, "Damage_multiplier") * v25;
			Commands->Set_Health(obj, v26);
			this->field_20 = Commands->Get_Health(obj);
			this->field_1C = Commands->Get_Health(obj);
		}
	}
	*/
}

void M00_Damage_Modifier_DME::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
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