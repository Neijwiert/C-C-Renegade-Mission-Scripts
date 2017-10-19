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
	Auto_Save_Variable(this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(this->field_28, sizeof(this->field_28), 2);
}

void M03_Area_Troop_Counter::Created(GameObject *obj)
{
	this->field_1C[0] = 2;
	this->field_1C[1] = 4;
	this->field_1C[2] = 6;

	this->field_28[2] = 0;
	this->field_28[1] = 0;
	this->field_28[0] = 0;
}

// TODO
void M03_Area_Troop_Counter::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 1000)
	{
		if (param == 1000)
		{
			int index = -1;
			int intVar = -1;
			Commands->Send_Custom_Event(obj, obj, 5000, reinterpret_cast<int>(&index), 0.0f); // Yes, get reference
			Commands->Send_Custom_Event(obj, obj, 6300, reinterpret_cast<int>(&intVar), 0.0f); // Yes, get reference

			if (index >= 0 && index <= 2)
			{
				this->field_1C[index]--;

				if (++this->field_28[index] % intVar)
				{
					if (this->field_1C[index] <= 0)
					{
						if (this->field_1C[index] < 0)
						{
							this->field_1C[index] = 0;
						}

						Commands->Send_Custom_Event(obj, obj, 6000, 6000, 0.0f);
						
						this->field_1C[index] += Commands->Get_Difficulty_Level();
					}
				}
			}
		}
	}
	else if (type == 7000)
	{
		this->field_1C[param] += Commands->Get_Difficulty_Level() + 1;
		this->field_28[param] = 0;
	}
}

ScriptRegistrant<M03_Area_Troop_Counter> M03_Area_Troop_CounterRegistrant("M03_Area_Troop_Counter", "");