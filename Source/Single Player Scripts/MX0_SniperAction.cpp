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
#include "MX0_SniperAction.h"

void MX0_SniperAction::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->health, sizeof(this->health), 2);
}

void MX0_SniperAction::Created(GameObject *obj)
{
	this->health = Commands->Get_Health(obj);

	Commands->Innate_Disable(obj);
}

void MX0_SniperAction::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 114)
	{
		Commands->Start_Timer(obj, this, 5.0f, 108);
	}
}

void MX0_SniperAction::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	Commands->Apply_Damage(obj, 10000.0f, "Blamokiller", NULL);
	
	Vector3 pos = Commands->Get_Position(obj);
	if (damager == Commands->Get_A_Star(pos))
	{
		GameObject *starObj = Commands->Get_A_Star(pos);
		Commands->Apply_Damage(obj, 10000.0f, "Blamokiller", starObj);

		if (Commands->Reveal_Encyclopedia_Character(41)) // Nod_Minigunner_2SF's encyclopedia id
		{
			Commands->Display_Encyclopedia_Event_UI();
		}
	}
	else
	{
		Commands->Apply_Damage(obj, 10000.0f, "Blamokiller", NULL);
	}
}

void MX0_SniperAction::Timer_Expired(GameObject *obj, int number)
{
	if (number == 108)
	{
		ActionParamsStruct actionParamsStruct;
		actionParamsStruct.Set_Basic(this, 99.0f, 1);

		actionParamsStruct.Set_Movement(obj, 0.0f, 5.0f, true);

		int faceObjId = Get_Int_Parameter("FaceObj");
		GameObject *faceObj = Commands->Find_Object(faceObjId);

		actionParamsStruct.Set_Attack(faceObj, 250.0f, 0.0f, true);
		actionParamsStruct.AttackCheckBlocked = false;

		Commands->Action_Attack(obj, actionParamsStruct);
	}
}

ScriptRegistrant<MX0_SniperAction> MX0_SniperActionRegistrant("MX0_SniperAction", "FaceObj:int");