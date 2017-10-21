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
#include "RMV_Camera_Behavior.h"

/*
M03 -> 1202444 1202445
M06 -> 101430 101431 101432 101433 101434 101435 101436 101437 101439 101440 101441 101442 101443 101444 101445 101446
M07 -> 123261 123260
M09 -> 1101566 1101565 1101564 1101563 2005502 2005503 1101299 1101827 1202590 1202723
M10 -> 1111350 1111349 1111348 1111347 1111346 1111345 1111344 1111307 1100262 1100261 1100260 1100251 1100249 1100248 1100247 1100246 1100230 1100229 1110061 1100031 1100033 1100034 1100035 1100036 1100037 1100038 1110060
*/
void RMV_Camera_Behavior::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->angleIndex, sizeof(this->angleIndex), 1);
	Auto_Save_Variable(&this->lookingAtEnemy, sizeof(this->lookingAtEnemy), 2);
	Auto_Save_Variable(&this->enemyId, sizeof(this->enemyId), 3);
	Auto_Save_Variable(&this->delayLookAtEnemyPassed, sizeof(this->delayLookAtEnemyPassed), 4);
	Auto_Save_Variable(&this->stillSeeingEnemyAfterDelay, sizeof(this->stillSeeingEnemyAfterDelay), 5);
	Auto_Save_Variable(&this->beepSoundId, sizeof(this->beepSoundId), 6);
	Auto_Save_Variable(&this->isGun, sizeof(this->isGun), 7);
	Auto_Save_Variable(&this->attackingEnemy, sizeof(this->attackingEnemy), 8);
}

void RMV_Camera_Behavior::Created(GameObject *obj)
{
	this->beepSoundId = 0;
	this->enemyId = 0;
	this->angleIndex = 0;
	this->isGun = (Get_Int_Parameter("Is_Gun") == 1);
	this->attackingEnemy = false;
	this->stillSeeingEnemyAfterDelay = false;
	this->delayLookAtEnemyPassed = false;
	this->lookingAtEnemy = false;

	Commands->Enable_Enemy_Seen(obj, true);

	ActionParamsStruct params;
	params.Set_Basic(this, 90.0f, 0);

	const char *strAngle = Get_Parameter(0);
	float angle = static_cast<float>(atof(strAngle));
	
	float facing = Commands->Get_Facing(obj);
	Vector3 pos = Commands->Get_Position(obj);

	Vector3 facingPos = pos;
	facingPos.X += WWMath::Cos(DEG_TO_RADF(facing)) * 10.0f;
	facingPos.Y += WWMath::Sin(DEG_TO_RADF(facing)) * 10.0f;
	facingPos.Z -= 1.0f;

	facingPos.X -= pos.X;
	facingPos.Y -= pos.Y;
	facingPos.Z -= pos.Z;

	float cosAngle = WWMath::Cos(DEG_TO_RADF(angle) * 0.5f); // Is this multiply by 0.5 correct?
	float sinAngle = WWMath::Sin(DEG_TO_RADF(angle) * 0.5f); // Is this multiply by 0.5 correct?

	Vector3 attackPos = pos;
	attackPos.X += cosAngle * facingPos.X - sinAngle * facingPos.Y;
	attackPos.Y += facingPos.X * sinAngle + cosAngle * facingPos.Y;
	attackPos.Z += facingPos.Z;

	params.Set_Attack(attackPos, 0.0f, 0.0f, true);

	Commands->Action_Attack(obj, params);

	float interval = atoi(strAngle) / 60.0f; // Yes, this uses angle
	Commands->Start_Timer(obj, this, interval, 0);
}

void RMV_Camera_Behavior::Killed(GameObject *obj, GameObject *killer)
{
	Commands->Stop_Sound(this->beepSoundId, true);
}

void RMV_Camera_Behavior::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	if (this->lookingAtEnemy)
	{
		if (this->delayLookAtEnemyPassed)
		{
			if (this->enemyId == Commands->Get_ID(enemy))
			{
				this->stillSeeingEnemyAfterDelay = true;
			}
		}
	}
	else if (!this->attackingEnemy)
	{
		Vector3 pos = Commands->Get_Position(obj);
		this->beepSoundId = Commands->Create_Sound("Beep", pos, obj);

		ActionParamsStruct params;
		params.Set_Basic(this, 99.0f, 1);
		params.Set_Attack(enemy, 0.0f, 0.0f, true);

		Commands->Action_Attack(obj, params);

		this->enemyId = Commands->Get_ID(enemy);
		this->lookingAtEnemy = true;

		float delay = Get_Float_Parameter("Delay");
		Commands->Start_Timer(obj, this, delay, 1);
	}
}

void RMV_Camera_Behavior::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// When done attacking enemy, see timer number 2
	if (complete_reason == ACTION_COMPLETE_NORMAL && action_id == 3)
	{
		this->enemyId = 0;
		this->attackingEnemy = false;
		this->stillSeeingEnemyAfterDelay = false;
		this->delayLookAtEnemyPassed = false;
		this->lookingAtEnemy = false;
		this->angleIndex = 0;

		Commands->Action_Reset(obj, 100.0f);
	}
}

void RMV_Camera_Behavior::Timer_Expired(GameObject *obj, int number)
{
	// Triggered after 3 seconds, see this block or timer number 2
	if (number == 10)
	{
		GameObject *enemyObj = Commands->Find_Object(this->enemyId);
		if (Commands->Is_Object_Visible(obj, enemyObj))
		{
			Commands->Start_Timer(obj, this, 3.0f, 10);
		}
		else
		{
			this->enemyId = 0;
			this->attackingEnemy = false;
			this->stillSeeingEnemyAfterDelay = false;
			this->delayLookAtEnemyPassed = false;
			this->lookingAtEnemy = false;
			this->angleIndex = 0;

			Commands->Action_Reset(obj, 100.0f);
		}
	}

	// Triggered after delay seconds, see enemy seen
	else if (number == 1)
	{
		this->delayLookAtEnemyPassed = true;

		Commands->Start_Timer(obj, this, 2.0f, 2);
	}

	// Triggered after 2 seconds, see timer number 1
	else if (number == 2)
	{
		if (this->stillSeeingEnemyAfterDelay)
		{
			if (this->isGun)
			{
				Commands->Action_Reset(obj, 100.0f);

				Commands->Stop_Sound(this->beepSoundId, true);

				this->attackingEnemy = true;

				ActionParamsStruct params;
				params.Set_Basic(this, 99.0f, 3);

				GameObject *enemyObj = Commands->Find_Object(this->enemyId);

				params.Set_Attack(enemyObj, 200.0f, 0.0f, true);

				Commands->Action_Attack(obj, params);

				Commands->Start_Timer(obj, this, 3.0f, 10);
			}
			else
			{
				Commands->Stop_Sound(this->beepSoundId, true);

				int alarmId = Get_Int_Parameter("Alarm_ID");
				if (alarmId)
				{
					GameObject *alarmObj = Commands->Find_Object(alarmId);
					if (alarmObj)
					{
						Commands->Send_Custom_Event(obj, alarmObj, 1007, this->enemyId, 0.0f);
					}
				}
				this->enemyId = 0;
				this->stillSeeingEnemyAfterDelay = false;
				this->delayLookAtEnemyPassed = false;
				this->lookingAtEnemy = false;
				this->angleIndex = 0;

				Commands->Action_Reset(obj, 100.0f);
			}
		}
		else
		{
			this->enemyId = 0;
			this->attackingEnemy = false;
			this->stillSeeingEnemyAfterDelay = false;
			this->delayLookAtEnemyPassed = false;
			this->lookingAtEnemy = false;

			Commands->Action_Reset(obj, 100.0f);

			this->angleIndex = 0;
		}
	}

	// Triggered after angle / 60 seconds or angle / 120 seconds, see create or this block
	else
	{
		this->angleIndex = (this->angleIndex + 1) % 8;

		ActionParamsStruct params;
		params.Set_Basic(this, 90.0f, 0);
		params.Set_Attack(Get_Target(), 0.0f, 0.0f, true);

		Commands->Action_Attack(obj, params);

		const char *strAngle = Get_Parameter(0); // Yes, this uses angle
		float interval = static_cast<float>(atof(strAngle)) / 120.0f;
		Commands->Start_Timer(obj, this, interval, 0);
	}
}

Vector3 RMV_Camera_Behavior::Get_Target()
{
	float facing = Commands->Get_Facing(Owner());
	Vector3 pos = Commands->Get_Position(Owner());

	Vector3 facingPos = pos;
	facingPos.X += WWMath::Cos(DEG_TO_RADF(facing)) * 10.0f;
	facingPos.Y += WWMath::Sin(DEG_TO_RADF(facing)) * 10.0f;
	facingPos.Z -= 0.89999998f;

	const char *strAngle = Get_Parameter(0);
	float angle = static_cast<float>(atof(strAngle));
	if (angle >= 0.0f)
	{
		if (angle > 360.0f)
		{
			angle = 360.0f;
		}
	}
	else
	{
		angle = 0.0f;
	}

	switch (this->angleIndex)
	{
		case 0:
			angle *= 0.5f;

			break;
		case 4:
			angle *= -0.5f;

			break;
		case 3:
		case 5:
			angle *= -0.25f;
			
			break;
		case 2:
		case 6:
			angle = 0.0f;

			break;
		case 7:
			angle *= 0.25f;

			break;
	}

	facingPos.X -= pos.X;
	facingPos.Y -= pos.Y;
	facingPos.Z -= pos.Z;

	float sinAngle = WWMath::Sin(DEG_TO_RADF(angle));
	float cosAngle = WWMath::Cos(DEG_TO_RADF(angle));

	Vector3 attackPos = pos;
	attackPos.X += cosAngle * facingPos.X - sinAngle * facingPos.Y;
	attackPos.Y += facingPos.X * sinAngle + cosAngle * facingPos.Y;
	attackPos.Z += facingPos.Z;

	return attackPos;
}

ScriptRegistrant<RMV_Camera_Behavior> RMV_Camera_BehaviorRegistrant("RMV_Camera_Behavior", "Angle:float, Alarm_ID=0:int, Is_Gun=0:int, Delay=0.0:float");