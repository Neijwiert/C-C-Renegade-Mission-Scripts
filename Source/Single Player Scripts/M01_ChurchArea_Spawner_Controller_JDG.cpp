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
#include "M01_ChurchArea_Spawner_Controller_JDG.h"

/*
M01 -> 102350
*/
void M01_ChurchArea_Spawner_Controller_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->soldierCount, sizeof(this->soldierCount), 1);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 2);
}

void M01_ChurchArea_Spawner_Controller_JDG::Created(GameObject *obj)
{
	Commands->Enable_Hibernation(obj, false);

	this->soldierCount = 0;
	this->field_20 = true;
}

// TODO
void M01_ChurchArea_Spawner_Controller_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (param == 69)
	{
		this->soldierCount--;

		Commands->Send_Custom_Event(obj, obj, 0, 68, 0.0f);
	}
	else if (param == 70)
	{
		GameObject *churchAreaMinigunnerSpawner1Obj = Commands->Find_Object(102347);
		if (churchAreaMinigunnerSpawner1Obj)
		{
			Commands->Destroy_Object(churchAreaMinigunnerSpawner1Obj);
		}

		GameObject *churchAreaMinigunnerSpawner2Obj = Commands->Find_Object(102348);
		if (churchAreaMinigunnerSpawner2Obj)
		{
			Commands->Destroy_Object(churchAreaMinigunnerSpawner2Obj);
		}

		GameObject *churchAreaFlamethrowerSpawnerObj = Commands->Find_Object(102349);
		if (churchAreaFlamethrowerSpawnerObj)
		{
			Commands->Destroy_Object(churchAreaFlamethrowerSpawnerObj);
		}

		Commands->Destroy_Object(obj);
	}
	else if(param == 68)
	{
		int difficultyLevel = Commands->Get_Difficulty_Level();
		if (this->field_20)
		{
			this->field_20 = false;

			if (!difficultyLevel)
			{
				Commands->Trigger_Spawner(102347);

				this->soldierCount++;
			}
			else if (difficultyLevel == 1)
			{
				Commands->Trigger_Spawner(102347);
				Commands->Trigger_Spawner(102348);

				this->soldierCount += 2;
			}
			else if (difficultyLevel == 2)
			{
				Commands->Trigger_Spawner(102347);
				Commands->Trigger_Spawner(102348);
				Commands->Trigger_Spawner(102349);

				this->soldierCount += 3;
			}
		}
		else
		{
			if (!difficultyLevel)
			{
				if (!this->soldierCount)
				{
					Commands->Trigger_Spawner(102347);

					this->soldierCount++;
				}
			}
			else if (difficultyLevel == 1)
			{
				if (this->soldierCount <= 1)
				{
					float rand = Commands->Get_Random(0.5f, 2.5f);
					if (rand >= 0.5f && rand < 1.5f)
					{
						Commands->Trigger_Spawner(102347);
					}
					else
					{
						Commands->Trigger_Spawner(102348);
					}
					
					this->soldierCount++;
				}
			}
			else if (difficultyLevel == 2)
			{
				if (this->soldierCount <= 2)
				{
					float rand = Commands->Get_Random(0.5f, 3.5f);
					if (rand >= 0.5f && rand < 1.5f)
					{
						Commands->Trigger_Spawner(102347);
					}
					else if (rand >= 1.5f && rand < 2.5f)
					{
						Commands->Trigger_Spawner(102348);
					}
					else
					{
						Commands->Trigger_Spawner(102349);
					}

					this->soldierCount++;
				}
			}
		}
	}
}

ScriptRegistrant<M01_ChurchArea_Spawner_Controller_JDG> M01_ChurchArea_Spawner_Controller_JDGRegistrant("M01_ChurchArea_Spawner_Controller_JDG", "");