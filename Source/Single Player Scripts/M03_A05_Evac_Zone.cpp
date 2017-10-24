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
#include "M03_A05_Evac_Zone.h"

// This script is never used
void M03_A05_Evac_Zone::Created(GameObject *obj)
{
	this->canSpawnSoldiers = true;
	this->field_1D = false;
}

void M03_A05_Evac_Zone::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 1000)
	{
		Vector3 spawnLocation;
		if (this->field_1D)
		{
			spawnLocation = Vector3(-167.38f, 0.09f, 9.46f);
		}
		else
		{
			spawnLocation = Vector3(-99.5f, 80.53f, 9.0f);
		}

		GameObject *nodMinigunner = Commands->Create_Object("Nod MiniGunner", spawnLocation);
		if (nodMinigunner)
		{
			Commands->Attach_Script(nodMinigunner, "M00_Trigger_When_Destroyed_RMV", "1,2,1,0,1000004,1000,1,0.0,0.0,0");

			ActionParamsStruct params;
			if (Commands->Get_Random(0.0f, 1.0f) >= 0.75f)
			{
				params.Set_Basic(this, 85.0f, 1);
			}
			else
			{
				params.Set_Basic(this, 99.0f, 1);
			}
			
			params.Set_Movement(Vector3(-330.0f, 75.0f, 4.0f), 1.0f, 1.0f);

			Commands->Action_Goto(nodMinigunner, params);
		}
	}
	else if (type == 1001)
	{
		this->field_1D = true;
	}
	else if (type == 1002)
	{
		this->field_1D = false;
	}
}

void M03_A05_Evac_Zone::Entered(GameObject *obj, GameObject *enterer)
{
	if (Commands->Is_A_Star(enterer) && this->canSpawnSoldiers)
	{
		this->canSpawnSoldiers = false;

		for (int spawnLocationIndex = 0; spawnLocationIndex < 3; spawnLocationIndex++)
		{
			static const Vector3 M03A05SpawnLocations[3] =
			{
				Vector3(-53.33f, 37.58f, 8.87f),
				Vector3(-111.4f, 32.27f, 9.31f),
				Vector3(-166.93f, 8.75f, 9.43f)
			};

			GameObject *nodMinigunner = Commands->Create_Object("Nod MiniGunner", M03A05SpawnLocations[spawnLocationIndex]);
			if (nodMinigunner)
			{
				Commands->Attach_Script(nodMinigunner, "M00_Trigger_When_Destroyed_RMV", "1,2,1,0,1000004,1000,1,0.0,0.0,0");

				ActionParamsStruct params;
				if (Commands->Get_Random(0.0f, 1.0f) >= 0.75f)
				{
					params.Set_Basic(this, 85.0f, 1);
				}
				else
				{
					params.Set_Basic(this, 99.0f, 1);
				}

				params.Set_Movement(Vector3(-330.0f, 75.0f, 4.0f), 1.0f, 1.0f);

				Commands->Action_Goto(nodMinigunner, params);
			}
		}
	}
}

ScriptRegistrant<M03_A05_Evac_Zone> M03_A05_Evac_ZoneRegistrant("M03_A05_Evac_Zone", "");