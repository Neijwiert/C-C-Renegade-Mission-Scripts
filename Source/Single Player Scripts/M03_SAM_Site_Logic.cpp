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
#include "M03_SAM_Site_Logic.h"

void M03_SAM_Site_Logic::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->attackingEnemy, sizeof(this->attackingEnemy), 1);
	Auto_Save_Variable(this->ignoredObjIds, sizeof(this->ignoredObjIds), 2);
	Auto_Save_Variable(&this->currentIgnoredObjIndex, sizeof(this->currentIgnoredObjIndex), 3);
}

// On level start
void M03_SAM_Site_Logic::Created(GameObject *obj)
{
	Commands->Enable_Enemy_Seen(obj, true);

	this->attackingEnemy = false;
	this->currentIgnoredObjIndex = 0;

	for (int x = 0; x < 10; x++)
	{
		this->ignoredObjIds[x] = 0;
	}

	float randInterval = Commands->Get_Random(2.0f, 3.0f);
	Commands->Start_Timer(obj, this, randInterval, 1);
}

void M03_SAM_Site_Logic::Killed(GameObject *obj, GameObject *killer)
{
	Vector3 pos = Commands->Get_Position(obj);
	float facing = Commands->Get_Facing(obj);

	GameObject *M01DestroyedSAMObj = Commands->Create_Object("M01_Destroyed_SAM", pos);
	Commands->Set_Facing(M01DestroyedSAMObj, facing);
	Commands->Attach_Script(M01DestroyedSAMObj, "M01_Destroyed_SAMSITE_JDG", "");
	Commands->Attach_Script(M01DestroyedSAMObj, "M03_Destroyed_SAM_Site", "");
}

void M03_SAM_Site_Logic::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// TODO (No custom)
	if (type == 100008 && this->currentIgnoredObjIndex < 10 && this->currentIgnoredObjIndex >= 0)
	{
		this->ignoredObjIds[this->currentIgnoredObjIndex++] = param;
	}
}

void M03_SAM_Site_Logic::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	if (!this->attackingEnemy && !Commands->Is_A_Star(enemy))
	{
		int enemyId = Commands->Get_ID(enemy);

		bool isIgnoredEnemy = false;
		if (this->currentIgnoredObjIndex > 0)
		{
			for (int x = 0; x < this->currentIgnoredObjIndex; x++)
			{
				if (this->ignoredObjIds[x] == enemyId)
				{
					isIgnoredEnemy = true;

					break;
				}
			}
		}

		if (!isIgnoredEnemy)
		{
			Vector3 enemyPos = Commands->Get_Position(enemy);
			Vector3 pos = Commands->Get_Position(obj);

			if (enemyPos.Z - pos.Z >= 5.0f)
			{
				this->attackingEnemy = true;

				ActionParamsStruct params;
				params.Set_Basic(this, 90.0f, 0);
				params.Set_Attack(enemy, 500.0f, 0.0f, true);
				params.AttackCheckBlocked = false;

				Commands->Action_Attack(obj, params);

				Commands->Start_Timer(obj, this, 2.0f, 0);
			}
		}
	}
}

void M03_SAM_Site_Logic::Timer_Expired(GameObject *obj, int number)
{
	// Triggered after 2 seconds after attacking a real enemy in enemy_seen
	if (!number)
	{
		this->attackingEnemy = false;

		Commands->Action_Reset(obj, 95.0f);
	}

	// Triggered after 1 second after creation or timer number <= 0
	else if (number == 1)
	{
		Vector3 pos = Commands->Get_Position(obj);
		float randXOffset = Commands->Get_Random(-10.0f, 10.0f);
		float randYOffset = Commands->Get_Random(-10.0f, 10.0f);
		float randZOffset = Commands->Get_Random(2.0f, 6.0f);

		ActionParamsStruct params;
		params.Set_Basic(this, 80.0f, 20);
		params.Set_Attack(Vector3(pos.X + randXOffset, pos.Y + randYOffset, pos.Z + randZOffset), 0.0f, 0.0f, true);
		params.AttackCheckBlocked = 1;

		Commands->Action_Attack(obj, params);

		float randInterval = Commands->Get_Random(2.0f, 3.0f);
		Commands->Start_Timer(obj, this, randInterval, 1);
	}
}

ScriptRegistrant<M03_SAM_Site_Logic> M03_SAM_Site_LogicRegistrant("M03_SAM_Site_Logic", "");