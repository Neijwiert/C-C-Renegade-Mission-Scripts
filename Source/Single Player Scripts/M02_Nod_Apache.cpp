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
#include "M02_Nod_Apache.h"

void M02_Nod_Apache::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->wayPathId, sizeof(this->wayPathId), 1);
}

// TODO
void M02_Nod_Apache::Created(GameObject *obj)
{
	int areaId = Get_Int_Parameter("Area_ID");
	
	float interval = 1.0f;
	if (areaId == 15)
	{
		this->wayPathId = 1; // Yes, I know this is not a waypath id, but see the rest of the script...
		interval = 15.0f;
	}
	else if (areaId == 24)
	{
		this->wayPathId = 24;
	}
	else if (!areaId)
	{
		this->wayPathId = 0;
		interval = 8.0f;
	}

	Commands->Enable_Engine(obj, true);
	Commands->Disable_Physical_Collisions(obj);

	Commands->Start_Timer(obj, this, interval, 1);
	Commands->Start_Timer(obj, this, 15.0f, 3);
}

void M02_Nod_Apache::Sound_Heard(GameObject *obj, const CombatSound & sound)
{
	if (sound.sound == 1000)
	{
		if (sound.Creator)
		{
			Commands->Send_Custom_Event(obj, sound.Creator, 100, 1, 0.0f);
		}
	}
}

void M02_Nod_Apache::Timer_Expired(GameObject *obj, int number)
{
	// Triggered on creation or after 5 seconds when timer number 2 triggered
	if (number == 1)
	{
		if (this->wayPathId == 400304)
		{
			this->wayPathId = 400310;
		}
		else if (this->wayPathId == 400544)
		{
			this->wayPathId = -1;
		}
		else if (this->wayPathId == 400310)
		{
			this->wayPathId = 400304;
		}
		else if (this->wayPathId == 401173)
		{
			this->wayPathId = 401180;
		}
		else if (this->wayPathId == 401169)
		{
			this->wayPathId = 401173;
		}
		else if (this->wayPathId == 1)
		{
			this->wayPathId = 401169;
		}
		else if (!this->wayPathId)
		{
			this->wayPathId = 400296;
		}
		else if (this->wayPathId == 24)
		{
			this->wayPathId = 400544;
		}

		if (this->wayPathId != -1)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 90.0f, 0);

			int difficultyLevel = Commands->Get_Difficulty_Level();

			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), difficultyLevel * 0.1f + 0.2f, 5.0f);
			params.WaypathID = this->wayPathId;
			params.WaypathSplined = true;
			params.MovePathfind = false;

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			params.Set_Attack(starObj, 200.0f, 0.0f, true);
			params.AttackCheckBlocked = false;
			params.AttackActive = false;

			Commands->Action_Attack(obj, params);
		}

		Commands->Start_Timer(obj, this, 5.0f, 2);
	}

	// Triggered 5 seconds after timer number 1
	else if (number == 2)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 90.0f, 0);

		Vector3 pos = Commands->Get_Position(obj);

		params.Set_Movement(pos, 0.0f, 5.0f);
		params.MovePathfind = false;

		GameObject *starObj = Commands->Get_A_Star(pos);

		params.Set_Attack(starObj, 200.0f, 0.0f, true);
		params.AttackCheckBlocked = false;
		params.AttackActive = true;

		Commands->Action_Attack(obj, params);

		Commands->Start_Timer(obj, this, 5.0f, 1);
	}

	// Triggered after 15 seconds on creation
	else if (number == 3)
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
	}
}

ScriptRegistrant<M02_Nod_Apache> M02_Nod_ApacheRegistrant("M02_Nod_Apache", "Area_ID:int");