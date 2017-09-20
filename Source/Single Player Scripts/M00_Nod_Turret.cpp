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
#include "M00_Nod_Turret.h"

/*
M13 -> 1400146 1400144
*/
void M00_Nod_Turret::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->attackObjId1, sizeof(this->attackObjId1), 1);
	Auto_Save_Variable(&this->attackObjId2, sizeof(this->attackObjId2), 2);
	Auto_Save_Variable(&this->attackObjId3, sizeof(this->attackObjId3), 3);
}

void M00_Nod_Turret::Created(GameObject *obj)
{
	Commands->Enable_Hibernation(obj, false);
	Commands->Innate_Enable(obj);
	Commands->Enable_Enemy_Seen(obj, true);
	
	Vector3 pos = Commands->Get_Position(obj);

	Vector3 attackObj1Pos(pos.X - 10.0f, pos.Y - 10.0f, pos.Z + 2.0f);
	Vector3 attackObj2Pos(pos.X + 10.0f, pos.Y, pos.Z + 2.0f);
	Vector3 attackObj3Pos(pos.X + 10.0f, pos.Y - 10.0f, pos.Z + 2.0f);

	GameObject *attackObj1 = Commands->Create_Object("Invisible_Object", attackObj1Pos);
	if (attackObj1)
	{
		this->attackObjId1 = Commands->Get_ID(attackObj1);
	}

	GameObject *attackObj2 = Commands->Create_Object("Invisible_Object", attackObj2Pos);
	if (attackObj2)
	{
		this->attackObjId2 = Commands->Get_ID(attackObj2);
	}

	GameObject *attackObj3 = Commands->Create_Object("Invisible_Object", attackObj3Pos);
	if (attackObj3)
	{
		this->attackObjId3 = Commands->Get_ID(attackObj3);
	}

	Commands->Start_Timer(obj, this, 10.0f, 1);
}

void M00_Nod_Turret::Killed(GameObject *obj, GameObject *killer)
{
	Vector3 pos = Commands->Get_Position(obj);
	float facing = Commands->Get_Facing(obj);

	GameObject *nodTurretDestroyed = Commands->Create_Object("Nod_Turret_Destroyed", pos);
	Commands->Set_Facing(nodTurretDestroyed, facing);
}

void M00_Nod_Turret::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	Vector3 enemyPos = Commands->Get_Position(enemy);
	enemyPos.Z -= 0.5f;

	ActionParamsStruct params;
	params.Set_Basic(this, 100.0f, 2);
	params.Set_Attack(enemyPos, 300.0f, 0.0f, true);
	params.AttackCheckBlocked = false;

	Commands->Action_Attack(obj, params);

	Commands->Start_Timer(obj, this, 2.0f, 2);
}

void M00_Nod_Turret::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (action_id == 2)
	{
		Commands->Action_Reset(obj, 100.0f);
	}
}

void M00_Nod_Turret::Timer_Expired(GameObject *obj, int number)
{
	// Triggered 10 seconds after creation or 10 seconds after this block
	if (number == 1)
	{
		int randAttackIndex = static_cast<int>(Commands->Get_Random(0.0f, 2.9999001f));
		if (randAttackIndex > 2)
		{
			randAttackIndex = 2;
		}

		GameObject *attackObj;
		switch (randAttackIndex)
		{
			case 0:
				attackObj = Commands->Find_Object(this->attackObjId1);
				break;
			case 1:
				attackObj = Commands->Find_Object(this->attackObjId2);
				break;
			default:
				attackObj = Commands->Find_Object(this->attackObjId3);
				break;
		}

		if (attackObj)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 70.0f, 1);
			params.Set_Attack(attackObj, 0.0f, 0.0f, true);

			Commands->Action_Attack(obj, params);
		}

		Commands->Start_Timer(obj, this, 10.0f, 1);
	}

	// Triggered 2 seconds after an enemy has been seen
	else if (number == 2)
	{
		Commands->Action_Reset(obj, 100.0f);
	}
}

ScriptRegistrant<M00_Nod_Turret> M00_Nod_TurretRegistrant("M00_Nod_Turret", "");