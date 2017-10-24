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
#include "M03_Area_Troop_Counter.h"

/*
M03 -> 1144444
*/
void M03_Area_Troop_Counter::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(this->areasTroopCounter, sizeof(this->areasTroopCounter), 1);
	Auto_Save_Variable(this->areasTroopKilledCounter, sizeof(this->areasTroopKilledCounter), 2);
}

void M03_Area_Troop_Counter::Created(GameObject *obj)
{
	this->areasTroopCounter[0] = 2;
	this->areasTroopCounter[1] = 4;
	this->areasTroopCounter[2] = 6;

	this->areasTroopKilledCounter[2] = 0;
	this->areasTroopKilledCounter[1] = 0;
	this->areasTroopKilledCounter[0] = 0;
}

void M03_Area_Troop_Counter::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from RMV_Trigger_Killed after 0 seconds when killed (id = 1146701, 1146700, 300009, 300028, 300032, 1141162, 1144451, 300010 or soldier from paradrop in M03_Chinook_ParaDrop)
	if (type == 1000)
	{
		if (param == 1000)
		{
			int currentActiveArea = -1;
			int reinforcedTroopKilledCounter = -1;
			Commands->Send_Custom_Event(obj, obj, 5000, reinterpret_cast<int>(&currentActiveArea), 0.0f); // Yes, get reference
			Commands->Send_Custom_Event(obj, obj, 6300, reinterpret_cast<int>(&reinforcedTroopKilledCounter), 0.0f); // Yes, get reference

			if (currentActiveArea >= 0 && currentActiveArea <= 2)
			{
				this->areasTroopCounter[currentActiveArea]--;

				if (++this->areasTroopKilledCounter[currentActiveArea] % reinforcedTroopKilledCounter)
				{
					if (this->areasTroopCounter[currentActiveArea] <= 0)
					{
						if (this->areasTroopCounter[currentActiveArea] < 0)
						{
							this->areasTroopCounter[currentActiveArea] = 0;
						}

						Commands->Send_Custom_Event(obj, obj, 6000, 6000, 0.0f);
						
						this->areasTroopCounter[currentActiveArea] += Commands->Get_Difficulty_Level();
					}
				}
			}
		}
	}

	// Received from M03_Reinforce_Area after 0 seconds when timer number 60000 expired. param = 1
	else if (type == 7000)
	{
		this->areasTroopCounter[param] += Commands->Get_Difficulty_Level() + 1;
		this->areasTroopKilledCounter[param] = 0;
	}
}

ScriptRegistrant<M03_Area_Troop_Counter> M03_Area_Troop_CounterRegistrant("M03_Area_Troop_Counter", "");