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
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->field_1D, sizeof(this->field_1D), 2);
	Auto_Save_Variable(&this->field_24, sizeof(this->field_24), 3);
	Auto_Save_Variable(&this->field_1F, sizeof(this->field_1F), 4);
	Auto_Save_Variable(&this->field_1E, sizeof(this->field_1E), 5);
	Auto_Save_Variable(this->field_28, sizeof(this->field_28), 6);
	Auto_Save_Variable(this->field_34, sizeof(this->field_34), 7);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 8);
	Auto_Save_Variable(this->field_40, sizeof(this->field_40), 9);
}

void M03_Reinforce_Area::Created(GameObject *obj)
{
	this->field_1F = false;
	this->field_1D = false;
	this->field_1C = false;
	this->field_1E = true;
	this->field_24 = 0;
	this->field_28[2] = 3;
	this->field_28[1] = 3;
	this->field_28[0] = 3;
	this->field_34[1] = 1;
	this->field_34[0] = 1;
	this->field_34[2] = 0;
	this->field_40[2] = 2;
	this->field_40[1] = 2;
	this->field_40[0] = 2;
	this->field_20 = false;
}

// TODO
void M03_Reinforce_Area::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 5000)
	{
		if (this->field_1E)
		{
			(*reinterpret_cast<int *>(param)) = 0; // Unsafe
		}
		else if (this->field_1C)
		{
			(*reinterpret_cast<int *>(param)) = 1; // Unsafe
		}
		else if (this->field_1D)
		{
			(*reinterpret_cast<int *>(param)) = 2; // Unsafe
		}
		else
		{
			(*reinterpret_cast<int *>(param)) = -1; // Unsafe
		}
	}
	else if (type == 6300)
	{
		if (this->field_1E)
		{
			(*reinterpret_cast<int *>(param)) = this->field_28[0]; // Unsafe
		}
		else if (this->field_1C)
		{
			(*reinterpret_cast<int *>(param)) = this->field_28[1]; // Unsafe
		}
		else if(this->field_1D)
		{
			(*reinterpret_cast<int *>(param)) = this->field_28[2]; // Unsafe
		}
	}
	else if (type == 6000)
	{
		if (param == 6000)
		{
			if (this->field_1E && this->field_28[0] != 1000 || this->field_1C && this->field_28[1] != 1000 || this->field_1D && this->field_28[2] != 1000)
			{
				this->field_20 = true;

				Commands->Start_Timer(obj, this, 3.0f, 0);
			}
		}
	}
	else if (type == 8000)
	{
		if (++this->field_34[param] > 1) // Unsafe
		{
			this->field_34[param] = (param != 2); // Unsafe
			if (++this->field_28[param] > 5) // Unsafe
			{
				this->field_28[param] = 1000; // Unsafe
			}
		}
	}
	else if (!type)
	{
		if (param == 2)
		{
			this->field_1E = false;
		}
	}
	else if (type == 1)
	{
		if (param == 1)
		{
			this->field_1C = true;
		}
		else if (param == 2)
		{
			this->field_1C = false;
		}
	}
	else if (type == 2)
	{
		if (param == 1)
		{
			this->field_1D = true;
		}
		else if (param == 2)
		{
			this->field_1D = false;
		}
	}
	else if (type == 1000)
	{
		if (param == 1000)
		{
			if (this->field_1C || this->field_1D || this->field_1E)
			{
				this->field_24++;
				if(this->field_1E && this->field_24 >= this->field_28[0] || this->field_1C && this->field_24 >= this->field_28[1] || this->field_1D && this->field_24 >= this->field_28[2])
				{
					this->field_20 = false;

					Commands->Start_Timer(obj, this, 3.0f, 0);

					this->field_24 = 0;
				}
			}
		}
	}
	else if (type == 40017)
	{
		Commands->Send_Custom_Event(obj, sender, 40018, this->field_1D, 0.0f);
		Commands->Send_Custom_Event(obj, sender, 40019, this->field_1C, 0.0f);
		Commands->Send_Custom_Event(obj, sender, 40020, this->field_1E, 0.0f);
	}
}

// TODO
void M03_Reinforce_Area::Timer_Expired(GameObject *obj, int number)
{
	if (!number)
	{
		this->field_1F = !this->field_1F;
	}
	else if (number == 60000)
	{
		if (this->field_40[1]-- > 0)
		{
			Vector3 paraDropLocation;
			float facing;

			if (this->field_1F)
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

			if (!this->field_20)
			{
				Commands->Send_Custom_Event(obj, obj, 7000, 1, 0.0f);
			}
		}
	}	
}

ScriptRegistrant<M03_Reinforce_Area> M03_Reinforce_AreaRegistrant("M03_Reinforce_Area", "");