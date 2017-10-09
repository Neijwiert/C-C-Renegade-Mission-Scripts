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
#include "M03_Commando_Script.h"

void M03_Commando_Script::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 2);
}

// When M03_Initial_Powerups timer's expires
void M03_Commando_Script::Created(GameObject *obj)
{
	this->field_1C = false;
	this->field_20 = 0;
}

void M03_Commando_Script::Destroyed(GameObject *obj)
{
	int controllerObjId = Get_Int_Parameter("Controller_ID");
	GameObject *controllerObj = Commands->Find_Object(controllerObjId);
	if (controllerObj)
	{
		Commands->Send_Custom_Event(obj, controllerObj, 12176, 12176, 0.0f);
	}
}

void M03_Commando_Script::Killed(GameObject *obj, GameObject *killer)
{
	if (this->field_1C)
	{
		GameObject *field20Obj = Commands->Find_Object(this->field_20);
		if (field20Obj)
		{
			Commands->Send_Custom_Event(obj, field20Obj, 1001, 1001, 0.0f);
		}
	}

	GameObject *nonExistingObj = Commands->Find_Object(1141168);
	if (nonExistingObj)
	{
		Commands->Send_Custom_Event(obj, nonExistingObj, 300, 300, 0.0f);
	}
}

// TODO
void M03_Commando_Script::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 3000)
	{
		if (this->field_1C)
		{
			if (Commands->Get_ID(sender) == this->field_20)
			{
				this->field_1C = false;
				this->field_20 = 0;
			}
		}
		else
		{
			this->field_1C = true;
			this->field_20 = Commands->Get_ID(sender);
		}
	}
	else if (type == 3100)
	{
		if (param == 3100 && Commands->Get_ID(sender) == this->field_20)
		{
			this->field_1C = false;
			this->field_20 = 0;
		}
	}
}

void M03_Commando_Script::Sound_Heard(GameObject *obj, const CombatSound & sound)
{

}

ScriptRegistrant<M03_Commando_Script> M03_Commando_ScriptRegistrant("M03_Commando_Script", "Controller_ID:int");