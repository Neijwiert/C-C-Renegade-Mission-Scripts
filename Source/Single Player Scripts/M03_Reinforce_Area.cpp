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
#include "M03_Reinforce_Area.h"

/*
M03 -> 1144444
*/
void M03_Reinforce_Area::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->starAtInlet, sizeof(this->starAtInlet), 1);
	Auto_Save_Variable(&this->starAtBase, sizeof(this->starAtBase), 2);
	Auto_Save_Variable(&this->troopKilledCount, sizeof(this->troopKilledCount), 3);
	Auto_Save_Variable(&this->dropAfterBridge, sizeof(this->dropAfterBridge), 4);
	Auto_Save_Variable(&this->starAtBeach, sizeof(this->starAtBeach), 5);
	Auto_Save_Variable(this->areasTroopKilledCounter, sizeof(this->areasTroopKilledCounter), 6);
	Auto_Save_Variable(this->areaTroopKilledCount, sizeof(this->areaTroopKilledCount), 7);
	Auto_Save_Variable(&this->areaActive, sizeof(this->areaActive), 8);
	Auto_Save_Variable(this->field_40, sizeof(this->field_40), 9);
}

void M03_Reinforce_Area::Created(GameObject *obj)
{
	this->dropAfterBridge = false;
	this->starAtBase = false;
	this->starAtInlet = false;
	this->starAtBeach = true;
	this->troopKilledCount = 0;
	this->areasTroopKilledCounter[2] = 3;
	this->areasTroopKilledCounter[1] = 3;
	this->areasTroopKilledCounter[0] = 3;
	this->areaTroopKilledCount[1] = 1;
	this->areaTroopKilledCount[0] = 1;
	this->areaTroopKilledCount[2] = 0;
	this->field_40[2] = 2;
	this->field_40[1] = 2;
	this->field_40[0] = 2;
	this->areaActive = false;
}

void M03_Reinforce_Area::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M03_Area_Troop_Counter after 0 seconds when custom type 1000 and param 1000 is received. param = address to local variable
	if (type == 5000)
	{
		if (this->starAtBeach)
		{
			(*reinterpret_cast<int *>(param)) = 0; // Unsafe
		}
		else if (this->starAtInlet)
		{
			(*reinterpret_cast<int *>(param)) = 1; // Unsafe
		}
		else if (this->starAtBase)
		{
			(*reinterpret_cast<int *>(param)) = 2; // Unsafe
		}
		else
		{
			(*reinterpret_cast<int *>(param)) = -1; // Unsafe
		}
	}

	// Received from M03_Area_Troop_Counter after 0 seconds when custom type 1000 and param 1000 is received. param = address to local variable
	else if (type == 6300)
	{
		if (this->starAtBeach)
		{
			(*reinterpret_cast<int *>(param)) = this->areasTroopKilledCounter[0]; // Unsafe
		}
		else if (this->starAtInlet)
		{
			(*reinterpret_cast<int *>(param)) = this->areasTroopKilledCounter[1]; // Unsafe
		}
		else if(this->starAtBase)
		{
			(*reinterpret_cast<int *>(param)) = this->areasTroopKilledCounter[2]; // Unsafe
		}
	}

	else if (type == 6000)
	{
		// Received from M03_Area_Troop_Counter after 0 seconds when custom type 1000 and param 1000 is received.
		if (param == 6000)
		{
			if (this->starAtBeach && this->areasTroopKilledCounter[0] != 1000 || this->starAtInlet && this->areasTroopKilledCounter[1] != 1000 || this->starAtBase && this->areasTroopKilledCounter[2] != 1000)
			{
				this->areaActive = true;

				Commands->Start_Timer(obj, this, 3.0f, 0);
			}
		}
	}

	// Received from RMV_Trigger_Killed after 0 seconds when killed (id = 1144448, 1144731, 1144732) param = 0
	// Received from RMV_Trigger_Killed after 0 seconds when killed (id = 300056, 1100020, 1144678, 1144679) param = 1
	// Received from RMV_Trigger_Killed after 0 seconds when killed (id = 1150001, 1150002, 1150003) param = 2
	else if (type == 8000)
	{
		if (++this->areaTroopKilledCount[param] > 1) // Unsafe
		{
			this->areaTroopKilledCount[param] = (param != 2); // Unsafe
			if (++this->areasTroopKilledCounter[param] > 5) // Unsafe
			{
				this->areasTroopKilledCounter[param] = 1000; // Unsafe
			}
		}
	}

	// Received from M03_Wheres_The_Star after 0 seconds when entered (id = 1100013)
	else if (!type)
	{
		if (param == 2)
		{
			this->starAtBeach = false;
		}
	}

	else if (type == 1)
	{
		// Received from M03_Wheres_The_Star after 0 seconds when entered (id = 1144632, 1144624, 1144622)
		if (param == 1)
		{
			this->starAtInlet = true;
		}

		// Received from M03_Wheres_The_Star after 0 seconds when entered (id = 1144634, 1144631, 1144625)
		else if (param == 2)
		{
			this->starAtInlet = false;
		}
	}

	else if (type == 2)
	{
		// Received from M03_Wheres_The_Star after 0 seconds when entered (id = 1144638, 1144628)
		if (param == 1)
		{
			this->starAtBase = true;
		}

		// Received from M03_Wheres_The_Star after 0 seconds when entered (id = 1144636, 1144626)
		else if (param == 2)
		{
			this->starAtBase = false;
		}
	}

	// Received from RMV_Trigger_Killed after 0 seconds when killed (id = 1146701, 1146700, 300009, 300028, 300032, 1141162, 1144451, 300010 or soldier from paradrop in M03_Chinook_ParaDrop)
	else if (type == 1000)
	{
		if (param == 1000)
		{
			if (this->starAtInlet || this->starAtBase || this->starAtBeach)
			{
				this->troopKilledCount++;
				if(this->starAtBeach && this->troopKilledCount >= this->areasTroopKilledCounter[0] || this->starAtInlet && this->troopKilledCount >= this->areasTroopKilledCounter[1] || this->starAtBase && this->troopKilledCount >= this->areasTroopKilledCounter[2])
				{
					this->areaActive = false;

					Commands->Start_Timer(obj, this, 3.0f, 0);

					this->troopKilledCount = 0;
				}
			}
		}
	}

	// Received from M03_Beach_Reinforce after 0 seconds when custom type 40008 is received
	else if (type == 40017)
	{
		Commands->Send_Custom_Event(obj, sender, 40018, this->starAtBase, 0.0f);
		Commands->Send_Custom_Event(obj, sender, 40019, this->starAtInlet, 0.0f);
		Commands->Send_Custom_Event(obj, sender, 40020, this->starAtBeach, 0.0f);
	}
}

void M03_Reinforce_Area::Timer_Expired(GameObject *obj, int number)
{
	// Triggered after 3 seconds, see custom type 1000 or 6000
	if (!number)
	{
		this->dropAfterBridge = !this->dropAfterBridge;
	}

	// Never triggered
	else if (number == 60000)
	{
		if (this->field_40[1]-- > 0)
		{
			Vector3 paraDropLocation;
			float facing;

			if (this->dropAfterBridge)
			{
				paraDropLocation = Vector3(99.85f, -49.51f, 2.0f);
				facing = 170.0F;
			}
			else
			{
				paraDropLocation = Vector3(51.0f, -91.0f, 2.0f);
				facing = 75.0f;
			}

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", paraDropLocation);
			Commands->Set_Facing(invisObj, facing);
			Commands->Attach_Script(invisObj, "M03_Chinook_ParaDrop", "M03_Paratrooper");

			if (!this->areaActive)
			{
				Commands->Send_Custom_Event(obj, obj, 7000, 1, 0.0f);
			}
		}
	}	
}

ScriptRegistrant<M03_Reinforce_Area> M03_Reinforce_AreaRegistrant("M03_Reinforce_Area", "");