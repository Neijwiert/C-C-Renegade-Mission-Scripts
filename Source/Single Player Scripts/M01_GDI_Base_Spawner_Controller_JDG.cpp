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
#include "M01_GDI_Base_Spawner_Controller_JDG.h"

/*
M01 -> 102335
*/
void M01_GDI_Base_Spawner_Controller_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->soldierCount, sizeof(this->soldierCount), 1);
}

void M01_GDI_Base_Spawner_Controller_JDG::Created(GameObject *obj)
{
	this->soldierCount = 0;
}

void M01_GDI_Base_Spawner_Controller_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_GDI_Base_Spawner_Guy_JDG when killed
	if (param == 69)
	{
		this->soldierCount--;

		Commands->Send_Custom_Event(obj, obj, 0, 68, 0.0f);

		Commands->Debug_Message("***************************GDI Base spawner has recieved custom that someone is dead\n");
	}

	// Received from M01_Player_Is_Entering_Tailgun_Alley_Backway_JDG or M01_Player_Is_Entering_Tailgun_Alley_JDG when entered
	else if (param == 70)
	{
		GameObject *backWayAlleySpawner1Obj = Commands->Find_Object(102332);
		if (backWayAlleySpawner1Obj)
		{
			Commands->Destroy_Object(backWayAlleySpawner1Obj);
		}

		GameObject *tunnelExitSpawnerObj = Commands->Find_Object(102333);
		if (tunnelExitSpawnerObj)
		{
			Commands->Destroy_Object(tunnelExitSpawnerObj);
		}

		GameObject *backWayAlleySpawner2Obj = Commands->Find_Object(102334);
		if (backWayAlleySpawner2Obj)
		{
			Commands->Destroy_Object(backWayAlleySpawner2Obj);
		}

		Commands->Destroy_Object(obj);
	}

	// Received from ourselves when param 69 is received or after 10 seconds after this block
	// Received from M01_Player_is_Leaving_GDI_Base_Zone when entered
	else if (param == 68)
	{
		Commands->Enable_Hibernation(obj, false);

		int difficultyLevel = Commands->Get_Difficulty_Level();
		if (!difficultyLevel)
		{
			if (!soldierCount)
			{
				Commands->Trigger_Spawner(102332);

				this->soldierCount++;

				Commands->Debug_Message("***************************GDI Base spawner has spawned an easy guy\n");
			}
		}
		else if (difficultyLevel == 1)
		{
			if (this->soldierCount <= 1)
			{
				float rand = Commands->Get_Random(0.5f, 2.5f);
				if (rand >= 0.5f && rand < 1.5f)
				{
					Commands->Trigger_Spawner(102332);
				}
				else
				{
					Commands->Trigger_Spawner(102333);
				}

				this->soldierCount++;

				Commands->Debug_Message("***************************GDI Base spawner has spawned a medium guy\n");
			}
		}
		else if (difficultyLevel == 2)
		{
			if (this->soldierCount <= 2)
			{
				float rand = Commands->Get_Random(0.5f, 3.5f);
				if (rand >= 0.5f &&rand < 1.5f)
				{
					Commands->Trigger_Spawner(102332);
				}
				else if (rand >= 1.5f && rand < 2.5f)
				{
					Commands->Trigger_Spawner(102333);
				}
				else
				{
					Commands->Trigger_Spawner(102334);
				}

				this->soldierCount++;

				Commands->Debug_Message("***************************GDI Base spawner has spawned a hard guy\n");
			}
		}

		Commands->Send_Custom_Event(obj, obj, 0, 68, 10.0f);
	}
}

ScriptRegistrant<M01_GDI_Base_Spawner_Controller_JDG> M01_GDI_Base_Spawner_Controller_JDGRegistrant("M01_GDI_Base_Spawner_Controller_JDG", "");