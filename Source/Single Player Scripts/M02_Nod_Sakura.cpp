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
#include "M02_Nod_Sakura.h"

void M02_Nod_Sakura::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
}

// When Create_Sakura is called at M02_Objective_Zone
void M02_Nod_Sakura::Created(GameObject *obj)
{
	this->field_1C = false;

	Commands->Enable_Enemy_Seen(obj, true);
	Commands->Enable_Engine(obj, true);
	Commands->Disable_Physical_Collisions(obj);

	ActionParamsStruct params;
	params.Set_Basic(this, 91.0f, 0);
	params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 2.5f, 1.0f);
	params.WaypathID = 400397;
	params.WaypathSplined = true;

	Vector3 pos = Commands->Get_Position(obj);
	GameObject *starObj = Commands->Get_A_Star(pos);

	params.Set_Attack(starObj, 200.0f, 0.0f, true);

	Commands->Action_Goto(obj, params);
	Commands->Start_Timer(obj, this, 15.0f, 1);
	Commands->Start_Timer(obj, this, 10.0f, 3);
}

void M02_Nod_Sakura::Sound_Heard(GameObject *obj, const CombatSound & sound)
{
	// Received from M02_Objective_Zone
	if (sound.sound == 1000)
	{
		if (sound.Creator)
		{
			Commands->Send_Custom_Event(obj, sound.Creator, 100, 1, 0.0f);
		}
	}
}

void M02_Nod_Sakura::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	ActionParamsStruct params;
	params.Set_Basic(this, 92.0f, 2);
	params.AttackCheckBlocked = false;
	params.Set_Attack(enemy, 300.0f, 0.0f, true);

	Commands->Action_Attack(obj, params);
}

void M02_Nod_Sakura::Timer_Expired(GameObject *obj, int number)
{
	// Triggered 15 seconds after creation or 15 seconds after this blocpk
	if (number == 1)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		if (starObj)
		{
			Vector3 starPos = Commands->Get_Position(starObj);
			if (Commands->Get_Distance(pos, starPos) > 300.0f)
			{
				Commands->Apply_Damage(obj, 10000.0f, "Blamokiller", NULL);
			}
		}

		Commands->Start_Timer(obj, this, 15.0f, 1);
	}

	// Never triggered
	else if (number == 2)
	{
		Commands->Destroy_Object(obj);
	}

	// Triggered 10 seconds after creation
	else if (number == 3)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 93.0f, 1);

		Vector3 pos = Commands->Get_Position(obj);

		params.Set_Movement(pos, 0.0f, 5.0f);

		GameObject *starObj = Commands->Get_A_Star(pos);

		params.Set_Attack(starObj, 200.0f, 0.0f, true);
		params.AttackCheckBlocked = true;

		Commands->Action_Attack(obj, params);
	}
}

ScriptRegistrant<M02_Nod_Sakura> M02_Nod_SakuraaRegistrant("M02_Nod_Sakura", "Area_ID:int"); // Area_ID is not used, but it is there.