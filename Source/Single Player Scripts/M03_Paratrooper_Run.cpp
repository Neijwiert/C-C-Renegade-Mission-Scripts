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
#include "M03_Paratrooper_Run.h"

void M03_Paratrooper_Run::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->receivedFallingDamage, sizeof(this->receivedFallingDamage), 1);
	Auto_Save_Variable(&this->healthBeforeFallingDamage, sizeof(this->healthBeforeFallingDamage), 2);
}

// When timer number 4/5/6 in M03_Chinook_ParaDrop is triggered
void M03_Paratrooper_Run::Created(GameObject *obj)
{
	this->receivedFallingDamage = false;
}

void M03_Paratrooper_Run::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (!this->receivedFallingDamage && !damager)
	{
		this->receivedFallingDamage = true;

		Commands->Set_Health(obj, this->healthBeforeFallingDamage);

		float randMoveOffset = Commands->Get_Random(-12.0f, 12.0f);
		Vector3 pos = Commands->Get_Position(obj);

		float facing = static_cast<float>(DEG_TO_RAD(Commands->Get_Facing(obj)));
		pos.X += WWMath::Cos(facing) * randMoveOffset;
		pos.Y += WWMath::Sin(facing) * randMoveOffset;

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 40);
		params.Set_Movement(pos, 0.8f, 1.0f);

		Commands->Action_Goto(obj, params);
	}
}

void M03_Paratrooper_Run::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!this->receivedFallingDamage && type == CUSTOM_EVENT_FALLING_DAMAGE)
	{
		this->healthBeforeFallingDamage = Commands->Get_Health(obj);
	}
}

ScriptRegistrant<M03_Paratrooper_Run> M03_Paratrooper_RunRegistrant("M03_Paratrooper_Run", "");