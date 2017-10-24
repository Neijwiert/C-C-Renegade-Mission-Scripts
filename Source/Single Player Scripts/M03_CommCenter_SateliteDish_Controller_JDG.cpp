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
#include "M03_CommCenter_SateliteDish_Controller_JDG.h"

// This script is never used
void M03_CommCenter_SateliteDish_Controller_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->startupParam, sizeof(this->startupParam), 1);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 2);
	Auto_Save_Variable(&this->soundParam, sizeof(this->soundParam), 3);
}

void M03_CommCenter_SateliteDish_Controller_JDG::Created(GameObject *obj)
{
	this->startupParam = 100;
	this->field_20 = 101;
	this->soundParam = 102;

	if (obj)
	{
		Commands->Send_Custom_Event(obj, obj, 0, this->startupParam, 0.0f);
	}
}

void M03_CommCenter_SateliteDish_Controller_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (param == this->startupParam)
	{
		if (obj)
		{
			Commands->Send_Custom_Event(obj, obj, 0, this->soundParam, 0.0f);
		}
	}
	else if (param == this->soundParam)
	{
		Commands->Create_Sound("Satelite Dish Moving Twiddler", Vector3(-110.26f, 41.3f, 19.37f), obj);

		float randDelay = Commands->Get_Random(0.0f, 3.0f);
		if (obj)
		{
			Commands->Send_Custom_Event(obj, obj, 0, this->soundParam, randDelay);
		}
	}
}

ScriptRegistrant<M03_CommCenter_SateliteDish_Controller_JDG> M03_CommCenter_SateliteDish_Controller_JDGRegistrant("M03_CommCenter_SateliteDish_Controller_JDG", "");