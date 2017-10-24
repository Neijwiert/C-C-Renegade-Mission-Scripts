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
#include "RMV_Test_Big_Gun_Turning.h"

/*
M03 -> 1100002
*/
void RMV_Test_Big_Gun_Turning::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->bigGunSamKilledCount, sizeof(this->bigGunSamKilledCount), 1);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 2);
	Auto_Save_Variable(&this->deathCinematicStarted, sizeof(this->deathCinematicStarted), 3);
}

void RMV_Test_Big_Gun_Turning::Created(GameObject *obj)
{
	this->deathCinematicStarted = false;
	this->bigGunSamKilledCount = 0;
	this->field_20 = false;
}

void RMV_Test_Big_Gun_Turning::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
	Commands->Attach_Script(invisObj, "Test_Cinematic", "X3C_Bigguns2.txt");

	GameObject *M03MissionControllerObj = Commands->Find_Object(1100004);
	Commands->Send_Custom_Event(obj, M03MissionControllerObj, 306, 3, 0.0f);
	Commands->Send_Custom_Event(obj, M03MissionControllerObj, 306, 1, 0.0f);

	GameObject *gdiGunBoatObj = Commands->Find_Object(1100003);
	Commands->Send_Custom_Event(obj, gdiGunBoatObj, 40027, 0, 0.0f);
}

void RMV_Test_Big_Gun_Turning::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (this->deathCinematicStarted)
	{
		float health = Commands->Get_Health(obj);
		if (health < 1.0f)
		{
			Commands->Set_Health(obj, health + 1.0f);
		}
	}
}

void RMV_Test_Big_Gun_Turning::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Never received
	if (type == 40026)
	{
		Commands->Action_Reset(obj, 100.0f);

		Commands->Enable_Enemy_Seen(obj, false);
	}

	// Received from RMV_Trigger_Zone after 0 seconds when entered. (id = 1100007, 1100015)
	else if (type == 1000)
	{
		if (param == 1000)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 99.0f, 2);

			GameObject *gdiGunBoatObj = Commands->Find_Object(1100003);

			params.Set_Attack(gdiGunBoatObj, 1000.0f, 2.0f, true);
			params.AttackCheckBlocked = false;

			Commands->Action_Attack(obj, params);
		}
	}

	// Received from M00_Trigger_When_Killed_RMV after 0 seconds when killed. (id = 300059, 300058)
	else if (type == 2000)
	{
		if (param == 2000 && ++this->bigGunSamKilledCount == 2)
		{
			if (Commands->Find_Object(1100003)) // GDI Gunboat
			{
				Commands->Start_Timer(obj, this, 2.0f, 40024);
			}
		}
	}

	// Received after 500 cinematic frames in X3C_Bigguns.txt
	// Received after 0 cinematic frames in X3C_Bigguns2.txt
	else if (type == 2450)
	{
		if (param == 2450)
		{
			GameObject *gdiGunBoatObj = Commands->Find_Object(1100003);
			if (gdiGunBoatObj)
			{
				Commands->Send_Custom_Event(obj, gdiGunBoatObj, 8000, 8000, 0.0f);
			}

			Commands->Destroy_Object(obj);
		}
	}
}

void RMV_Test_Big_Gun_Turning::Sound_Heard(GameObject *obj, const CombatSound & sound)
{
	if (sound.sound == 994)
	{
		Commands->Shake_Camera(sound.Position, 40.0f, 0.30000001f, 1.0f);

		if (!this->field_20 && Commands->Get_Random(1.0f, 5.0f) < 2.0f)
		{
			this->field_20 = true;
		}
	}
}

void RMV_Test_Big_Gun_Turning::Timer_Expired(GameObject *obj, int number)
{
	// Triggered after 2 seconds when custom type 2000 is received
	if (number == 40024)
	{
		if (Commands->Find_Object(1100002)) // Big Gun
		{
			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X3C_Bigguns.txt");

			this->deathCinematicStarted = true;
		}
	}
}

ScriptRegistrant<RMV_Test_Big_Gun_Turning> RMV_Test_Big_Gun_TurningRegistrant("RMV_Test_Big_Gun_Turning", "");