/*
* A Command & Conquer: Renegade SSGM Plugin, containing all the single player mission scripts
* Copyright(C) 2017  Neijwiert
*
* This program is free software : you can redistribute it and / or modifyM01_Hon_WarroomC
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
#include "M01_HON_WarroomController_JDG.h"

/*
M01 -> 124044
*/
void M01_HON_WarroomController_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 2);
	Auto_Save_Variable(&this->field_24, sizeof(this->field_24), 3);
	Auto_Save_Variable(&this->field_25, sizeof(this->field_25), 4);
	Auto_Save_Variable(&this->field_28, sizeof(this->field_28), 5);
}

void M01_HON_WarroomController_JDG::Created(GameObject *obj)
{
	this->field_1C = 0;
	this->field_20 = 0;
	this->field_24 = false;
	this->field_25 = false;
	this->field_28 = 0;
}

// TODO
void M01_HON_WarroomController_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		if (obj)
		{
			if (param == 12)
			{
				this->field_1C = Commands->Get_ID(sender);
			}
			else if (param == 13)
			{
				this->field_20 = Commands->Get_ID(sender);
			}
			else if (param == 139)
			{
				this->field_24 = true;
			}
			else if (param == 140)
			{
				this->field_24 = false;
			}
			else if (param == 22)
			{
				if (++this->field_28 == 2)
				{
					this->field_28 = 0;

					GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-147.1f, 558.8f, 4.4f));
					Commands->Set_Facing(invisObj, -130.0f);
					Commands->Attach_Script(invisObj, "Test_Cinematic", "X1I_GDI_HON_TroopDrop01a.txt");
				}
			}
		}
	}
}

void M01_HON_WarroomController_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	GameObject *field1CObj = Commands->Find_Object(this->field_1C);
	GameObject *field20Obj = Commands->Find_Object(this->field_20);
	if (enterer == field1CObj)
	{
		if (!this->field_24)
		{
			Commands->Apply_Damage(field1CObj, 10000.0f, "STEEL", NULL);
		}
		else
		{
			if (field1CObj)
			{
				Commands->Send_Custom_Event(obj, field1CObj, 0, 16, 0.0f);
			}
		}
	}
	else if (enterer == field20Obj)
	{
		if (!this->field_24)
		{
			Commands->Apply_Damage(field20Obj, 10000.0f, "STEEL", NULL);
		}
		else
		{
			if (field20Obj)
			{
				Commands->Send_Custom_Event(obj, field20Obj, 0, 16, 0.0f);
			}
		}
	}
}

ScriptRegistrant<M01_HON_WarroomController_JDG> M01_HON_WarroomController_JDGRegistrant("M01_HON_WarroomController_JDG", "");