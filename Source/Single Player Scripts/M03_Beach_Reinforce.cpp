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
#include "M03_Beach_Reinforce.h"

/*
M03 -> 2018061
*/
void M03_Beach_Reinforce::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 1);
	Auto_Save_Variable(&this->field_24, sizeof(this->field_24), 2);
	Auto_Save_Variable(&this->field_28, sizeof(this->field_28), 3);
	Auto_Save_Variable(&this->field_2C, sizeof(this->field_2C), 4);
	Auto_Save_Variable(&this->field_30, sizeof(this->field_30), 5);
	Auto_Save_Variable(&this->field_34, sizeof(this->field_34), 6);
	Auto_Save_Variable(&this->field_38, sizeof(this->field_38), 7);
	Auto_Save_Variable(&this->field_3C, sizeof(this->field_3C), 8);
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 9);
}

void M03_Beach_Reinforce::Created(GameObject *obj)
{
	this->field_1C = 0;
	this->field_20 = 2;
	this->field_24 = 0;
	this->field_28 = 0;
	this->field_2C = 0;
	this->field_30 = 0;
	this->field_34 = Commands->Get_Difficulty_Level() + 2;
	this->field_38 = Commands->Get_Difficulty_Level() + 3;
	this->field_3C = Commands->Get_Difficulty_Level() + 4;
}

// TODO
void M03_Beach_Reinforce::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 40021)
	{
		this->field_1C = param;
	}
	else if (type == 40018)
	{
		if (param == 1)
		{
			Reinforce_Base();
		}
	}
	else if (type == 40020)
	{
		if (param == 1)
		{
			Reinforce_Beach();
		}
	}
	else if (type == 40019)
	{
		if (param == 1)
		{
			Reinforce_Inlet();
		}
	}
	else if (type == 40008)
	{
		if (param == 1)
		{
			if (!this->field_1C)
			{
				this->field_28++;
			}
			else if (this->field_1C == 1)
			{
				this->field_2C++;
			}
			else if (this->field_1C == 2)
			{
				this->field_30++;
			}
		}

		if (--this->field_20 <= 0)
		{
			this->field_20 = 5 - Commands->Get_Difficulty_Level();

			GameObject *powerPlantNodChinookReinforcementsObj = Commands->Find_Object(1144444);
			Commands->Send_Custom_Event(obj, powerPlantNodChinookReinforcementsObj, 40017, 0, 0.0f);
		}
	}
	else if (type == 40009)
	{
		this->field_24 = 10;
	}
}

void M03_Beach_Reinforce::Reinforce_Beach()
{
	if (this->field_28 <= 2 && this->field_34 > 0)
	{
		this->field_34--;

		if (!this->field_24 )
		{
			this->field_24++;

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-95.656f, -68.236f, 1.433f));
			Commands->Set_Facing(invisObj, -180.0f);
			Commands->Attach_Script(invisObj, "M03_Chinook_ParaDrop", "M03_Paratrooper");
		}
		else if (this->field_24 == 1)
		{
			this->field_24 = 0;

			Commands->Trigger_Spawner(2018881);
			Commands->Trigger_Spawner(2018880);
		}
		else if (this->field_24 > 1)
		{
			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-95.656f, -68.236f, 1.433f));
			Commands->Set_Facing(invisObj, -180.0f);
			Commands->Attach_Script(invisObj, "M03_Chinook_ParaDrop", "M03_Paratrooper");
		}
	}
}

void M03_Beach_Reinforce::Reinforce_Inlet()
{
	if (this->field_2C <= 2 && this->field_38 > 0)
	{
		this->field_38--;

		if (this->field_24 <= 0)
		{
			this->field_24 = 1;

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(51.0f, -91.0f, 2.0f));
			Commands->Set_Facing(invisObj, 75.0f);
			Commands->Attach_Script(invisObj, "M03_Chinook_ParaDrop", "M03_Paratrooper2");
		}
		else
		{
			this->field_24--;

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(99.85f, -49.51f, 2.0f));
			Commands->Set_Facing(invisObj, 170.0f);
			Commands->Attach_Script(invisObj, "M03_Chinook_ParaDrop", "M03_Paratrooper2");
		}
	}
}

void M03_Beach_Reinforce::Reinforce_Base()
{
	if (this->field_30 <= 2 && this->field_3C > 0)
	{
		this->field_3C--;

		if (this->field_24 > 0)
		{
			this->field_24 = 0;

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-79.0f, 59.0f, 9.5f));
			Commands->Set_Facing(invisObj, -120.0f);
			Commands->Attach_Script(invisObj, "M03_Chinook_ParaDrop", "M03_Paratrooper");
		}
		else
		{
			this->field_24++;

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-138.0f, 50.0f, 9.5f));
			Commands->Set_Facing(invisObj, -30.0f);
			Commands->Attach_Script(invisObj, "M03_Chinook_ParaDrop", "M03_Paratrooper");
		}
	}
}

ScriptRegistrant<M03_Beach_Reinforce> M03_Beach_ReinforceRegistrant("M03_Beach_Reinforce", "");