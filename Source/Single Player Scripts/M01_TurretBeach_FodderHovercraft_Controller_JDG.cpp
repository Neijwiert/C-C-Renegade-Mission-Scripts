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
#include "M01_TurretBeach_FodderHovercraft_Controller_JDG.h"

/*
M01 -> 121630
*/
void M01_TurretBeach_FodderHovercraft_Controller_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 1);
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 2);
}

void M01_TurretBeach_FodderHovercraft_Controller_JDG::Created(GameObject *obj)
{
	this->field_20 = false;
}

// TODO
void M01_TurretBeach_FodderHovercraft_Controller_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		if (param == 16)
		{
			if (!this->field_20)
			{
				if (!Commands->Find_Object(this->field_1C))
				{
					this->field_20 = true;

					GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
					Commands->Attach_Script(invisObj, "Test_Cinematic", "X1D_FodderHover_MTank.txt");
				}

				Commands->Send_Custom_Event(obj, obj, 0, 16, 5.0f);
			}
		}

		// Received from M01_FodderHovercraft_Script_JDG when killed
		else if (param == 22)
		{
			this->field_20 = false;

			Commands->Send_Custom_Event(obj, obj, 0, 16, 15.0f);
		}

		// Received from M01_FodderHovercraft_Script_JDG when created
		else if (!param)
		{
			this->field_1C = Commands->Get_ID(sender);
			this->field_20 = true;

			GameObject *beachNodTurret1Obj = Commands->Find_Object(101434);
			if (beachNodTurret1Obj)
			{
				Commands->Send_Custom_Event(obj, beachNodTurret1Obj, 218, this->field_1C, 1.0f);
			}

			GameObject *beachNodTurret2Obj = Commands->Find_Object(101435);
			if (beachNodTurret2Obj)
			{
				Commands->Send_Custom_Event(obj, beachNodTurret2Obj, 218, this->field_1C, 1.0f);
			}
		}
	}
}

ScriptRegistrant<M01_TurretBeach_FodderHovercraft_Controller_JDG> M01_TurretBeach_FodderHovercraft_Controller_JDGRegistrant("M01_TurretBeach_FodderHovercraft_Controller_JDG", "");