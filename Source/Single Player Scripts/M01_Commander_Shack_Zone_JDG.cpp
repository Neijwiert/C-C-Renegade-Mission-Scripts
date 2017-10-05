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
#include "M01_Commander_Shack_Zone_JDG.h"

/*
M01 -> 115985
*/
void M01_Commander_Shack_Zone_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->starInsideShack, sizeof(this->starInsideShack), 2);
	Auto_Save_Variable(&this->field_1E, sizeof(this->field_1E), 3);
	Auto_Save_Variable(&this->field_1F, sizeof(this->field_1F), 4);
}

void M01_Commander_Shack_Zone_JDG::Created(GameObject *obj)
{
	this->field_1C = true;
	this->starInsideShack = false;
	this->field_1E = false;
	this->field_1F = false;
}

// TODO
void M01_Commander_Shack_Zone_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		// Received from M01_GDIBase_BaseCommander_JDG when param 4 is received
		if (param == 16)
		{
			this->field_1E = true;

			Commands->Send_Custom_Event(obj, obj, 0, 27, 0.0f);
		}

		// Received from M01_GDIBase_BaseCommander_JDG when killed
		else if (param == 22)
		{
			this->field_1C = false;
		}

		// Received from ourselves after param 16 or after this block or entered
		else if (param == 27)
		{
			if (!this->field_1F)
			{
				if (this->starInsideShack && this->field_1C && this->field_1E)
				{
					GameObject *captainDuncanObj = Commands->Find_Object(106050);
					if (captainDuncanObj)
					{
						this->field_1F = true;

						Commands->Send_Custom_Event(obj, captainDuncanObj, 0, 27, 1.0f);
					}
				}
				else
				{
					Commands->Send_Custom_Event(obj, obj, 0, 27, 0.25f);
				}
			}
		}
	}
}

void M01_Commander_Shack_Zone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		this->starInsideShack = true;

		Commands->Send_Custom_Event(obj, obj, 0, 27, 0.0f);

		GameObject *M01DuncanInHereConvControllerJDGObj = Commands->Find_Object(125857);
		if (M01DuncanInHereConvControllerJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01DuncanInHereConvControllerJDGObj, 0, 27, 0.0f);
		}
	}
}

void M01_Commander_Shack_Zone_JDG::Exited(GameObject *obj, GameObject *exiter)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (exiter == Commands->Get_A_Star(pos))
	{
		this->starInsideShack = false;
	}
}

ScriptRegistrant<M01_Commander_Shack_Zone_JDG> M01_Commander_Shack_Zone_JDGRegistrant("M01_Commander_Shack_Zone_JDG", "");