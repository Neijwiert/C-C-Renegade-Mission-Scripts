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
#include "M01_Tailgun_Run_Spawner_Controller_JDG.h"

/*
M01 -> 102346
*/
void M01_Tailgun_Run_Spawner_Controller_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->soldierCount, sizeof(this->soldierCount), 1);
	Auto_Save_Variable(&this->starFirstTimeEnteredTailgunAlley, sizeof(this->starFirstTimeEnteredTailgunAlley), 2);
}

void M01_Tailgun_Run_Spawner_Controller_JDG::Created(GameObject *obj)
{
	Commands->Enable_Hibernation(obj, false);

	this->soldierCount = 0;
	this->starFirstTimeEnteredTailgunAlley = true;
}

void M01_Tailgun_Run_Spawner_Controller_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_TailgunRun_Spawner_Guy_JDG when killed
	if (param == 69)
	{
		this->soldierCount--;

		Commands->Send_Custom_Event(obj, obj, 0, 68, 0.0f);
	}

	// Received from M01_Player_Is_Crossing_Bridge_Via_Cave_Zone or M01_Player_Is_Crossing_Bridge_Via_Church_Zone or M01_Player_Is_Crossing_Bridge_Zone when entered
	else if (param == 70)
	{
		GameObject *tailGunRunSpawner1Obj = Commands->Find_Object(102343);
		if (tailGunRunSpawner1Obj)
		{
			Commands->Destroy_Object(tailGunRunSpawner1Obj);
		}

		GameObject *tailGunRunSpawner2Obj = Commands->Find_Object(102344);
		if (tailGunRunSpawner2Obj)
		{
			Commands->Destroy_Object(tailGunRunSpawner2Obj);
		}

		GameObject *tailGunRunSpawner3Obj = Commands->Find_Object(102345);
		if (tailGunRunSpawner3Obj)
		{
			Commands->Destroy_Object(tailGunRunSpawner3Obj);
		}

		Commands->Destroy_Object(obj);
	}

	// Received from M01_Player_Is_Entering_Tailgun_Alley_Backway_JDG or M01_Player_Is_Entering_Tailgun_Alley_JDG when entered
	// Received from ourselves when param 69 is received
	else if(param == 68)
	{
		int difficultyLevel = Commands->Get_Difficulty_Level();
		if (!difficultyLevel)
		{
			if (this->starFirstTimeEnteredTailgunAlley)
			{
				this->starFirstTimeEnteredTailgunAlley = false;

				Commands->Trigger_Spawner(102343);

				this->soldierCount++;
			}

			if (!this->soldierCount)
			{
				Commands->Trigger_Spawner(102343);

				this->soldierCount++;
			}
		}
		else if (difficultyLevel == 1)
		{
			if (this->starFirstTimeEnteredTailgunAlley)
			{
				this->starFirstTimeEnteredTailgunAlley = false;

				Commands->Trigger_Spawner(102343);
				Commands->Trigger_Spawner(102344);

				this->soldierCount += 2;
			}

			if (this->soldierCount <= 1)
			{
				float rand = Commands->Get_Random(0.5f, 2.5f);
				if (rand >= 0.5f && rand < 1.5f)
				{
					Commands->Trigger_Spawner(102343);
				}
				else
				{
					Commands->Trigger_Spawner(102344);
				}

				this->soldierCount++;
			}
		}
		else if (difficultyLevel == 2)
		{
			if (this->starFirstTimeEnteredTailgunAlley)
			{
				this->starFirstTimeEnteredTailgunAlley = false;

				Commands->Trigger_Spawner(102343);
				Commands->Trigger_Spawner(102344);
				Commands->Trigger_Spawner(102345);

				this->soldierCount += 3;
			}

			if (this->soldierCount <= 2)
			{
				float rand = Commands->Get_Random(0.5f, 3.5f);
				if (rand >= 0.5f && rand < 1.5f)
				{
					Commands->Trigger_Spawner(102343);
				}
				else if (rand >= 1.5f && rand < 2.5f)
				{
					Commands->Trigger_Spawner(102344);
				}
				else
				{
					Commands->Trigger_Spawner(102345);
				}

				this->soldierCount++;
			}
		}
	}
}

ScriptRegistrant<M01_Tailgun_Run_Spawner_Controller_JDG> M01_Tailgun_Run_Spawner_Controller_JDGRegistrant("M01_Tailgun_Run_Spawner_Controller_JDG", "");