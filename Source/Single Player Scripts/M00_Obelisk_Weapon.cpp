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
#include "M00_Obelisk_Weapon.h"

void M00_Obelisk_Weapon::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->canAttack, sizeof(this->canAttack), 1);
	Auto_Save_Variable(&this->enemyObjId, sizeof(this->enemyObjId), 2);
	Auto_Save_Variable(&this->obeliskEffectObjId, sizeof(this->obeliskEffectObjId), 3);
}

// When M00_Nod_Obelisk is created
void M00_Obelisk_Weapon::Created(GameObject *obj)
{
	this->obeliskEffectObjId = 0;
	this->enemyObjId = 0;
	this->canAttack = true;

	Commands->Set_Player_Type(obj, SCRIPT_PLAYERTYPE_NOD);
	Commands->Set_Is_Rendered(obj, false);
	Commands->Enable_Enemy_Seen(obj, true);
	Commands->Enable_Hibernation(obj, false);
	Commands->Innate_Enable(obj);

	Vector3 pos = Commands->Get_Position(obj);
	GameObject *obeliskEffectObj = Commands->Create_Object("Obelisk Effect", pos);
	if (obeliskEffectObj)
	{
		this->obeliskEffectObjId = Commands->Get_ID(obeliskEffectObj);
		Commands->Set_Animation_Frame(obeliskEffectObj, "OBL_POWERUP.OBL_POWERUP", 0);
	}
}

void M00_Obelisk_Weapon::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M00_Nod_Obelisk, see timer. param = 0 means power is down or building is dead. 1 is opposite.
	if (type == 1)
	{
		if (param)
		{
			Commands->Enable_Enemy_Seen(obj, true);
			this->canAttack = true;
		}
		else
		{
			Commands->Enable_Enemy_Seen(obj, false);

			this->canAttack = false;

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 0);
			params.Set_Attack(obj, 0.0f, 0.0f, true);

			Commands->Action_Attack(obj, params);
			Commands->Action_Reset(obj, 100.0f);

			if (this->obeliskEffectObjId)
			{
				GameObject *obeliskEffectObj = Commands->Find_Object(this->obeliskEffectObjId);
				if (obeliskEffectObj)
				{
					Commands->Set_Animation_Frame(obeliskEffectObj, "OBL_POWERUP.OBL_POWERUP", 0);
				}
			}
		}
	}

	// Received from ourselves with param being the enemy obj id
	else if (type == 2)
	{
		if (this->canAttack)
		{
			GameObject *enemyObj = Commands->Find_Object(param);
			if (enemyObj)
			{
				Vector3 enemyObjPos = Commands->Get_Position(enemyObj);
				Vector3 pos = Commands->Get_Position(obj);

				enemyObjPos.Z = 0.0f;
				pos.Z = 0.0f;

				float distance = Commands->Get_Distance(pos, enemyObjPos);
				if (distance <= 15.0f || distance >= 70.0f)
				{
					if (this->obeliskEffectObjId)
					{
						GameObject *obeliskEffectObj = Commands->Find_Object(this->obeliskEffectObjId);
						if (obeliskEffectObj)
						{
							Commands->Set_Animation_Frame(obeliskEffectObj, "OBL_POWERUP.OBL_POWERUP", 0);
						}
					}

					Commands->Action_Reset(obj, 100.0f);
				}
				else
				{
					this->enemyObjId = param;
					this->canAttack = false;

					Commands->Start_Timer(obj, this, 2.5f, 1);

					GameObject *obeliskEffectObj = Commands->Find_Object(this->obeliskEffectObjId);
					if (obeliskEffectObj)
					{
						Commands->Set_Animation_Frame(obeliskEffectObj, "OBL_POWERUP.OBL_POWERUP", 1);
					}

					pos.Z -= 20.0f;
					Commands->Create_Sound("Obelisk_Warm_Up", pos, obj);
				}
			}
		}
	}
}

void M00_Obelisk_Weapon::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	int enemyObjId = Commands->Get_ID(enemy);
	Commands->Send_Custom_Event(obj, obj, 2, enemyObjId, 0.0f);
}

void M00_Obelisk_Weapon::Timer_Expired(GameObject *obj, int number)
{
	// Triggered after 2.5 seconds see custom type 2
	if (number == 1)
	{
		GameObject *enemyObj = Commands->Find_Object(this->enemyObjId);
		Vector3 pos = Commands->Get_Position(obj);
		Vector3 enemyObjPos = Commands->Get_Position(enemyObj);

		pos.Z = 0.0f;
		enemyObjPos.Z = 0.0f;

		float distance = Commands->Get_Distance(pos, enemyObjPos);

		if (enemyObj && distance > 15.0f && distance < 70.0f)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 0);
			params.Set_Attack(enemyObj, 150.0f, 0.0f, true);

			Commands->Action_Attack(obj, params);

			this->enemyObjId = 0;

			Commands->Start_Timer(obj, this, 2.0f, 2);
		}
		else
		{
			if (this->obeliskEffectObjId)
			{
				GameObject *obeliskEffectObj = Commands->Find_Object(this->obeliskEffectObjId);
				if (obeliskEffectObj)
				{
					Commands->Set_Animation_Frame(obeliskEffectObj, "OBL_POWERUP.OBL_POWERUP", 0);
				}
			}

			Commands->Action_Reset(obj, 100.0f);

			this->canAttack = true;
		}
	}

	// Triggered after 2 seconds see timer number 1
	else if (number == 2)
	{
		if (this->obeliskEffectObjId)
		{
			GameObject *obeliskEffectObj = Commands->Find_Object(this->obeliskEffectObjId);
			if (obeliskEffectObj)
			{
				Commands->Set_Animation_Frame(obeliskEffectObj, "OBL_POWERUP.OBL_POWERUP", 0);
			}
		}

		this->canAttack = true;
	}
}

ScriptRegistrant<M00_Obelisk_Weapon> M00_Obelisk_WeaponRegistrant("M00_Obelisk_Weapon", "");