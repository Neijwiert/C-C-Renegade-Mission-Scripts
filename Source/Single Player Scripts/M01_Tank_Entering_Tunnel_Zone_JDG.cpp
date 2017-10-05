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
#include "M01_Tank_Entering_Tunnel_Zone_JDG.h"

/*
M01 -> 101679
*/
void M01_Tank_Entering_Tunnel_Zone_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
}

void M01_Tank_Entering_Tunnel_Zone_JDG::Created(GameObject *obj)
{
	this->field_1C = 0;
}

// TODO
void M01_Tank_Entering_Tunnel_Zone_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_Medium_Tank_JDG when created param = its object id
	if (type == 228)
	{
		this->field_1C = param;
	}
}

void M01_Tank_Entering_Tunnel_Zone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	if (enterer == Commands->Find_Object(this->field_1C))
	{
		GameObject *nonExisting1Obj = Commands->Find_Object(101682);
		if (nonExisting1Obj)
		{
			Commands->Send_Custom_Event(obj, nonExisting1Obj, 0, 228, 0.0f);
		}

		GameObject *nonExisting2Obj = Commands->Find_Object(101683);
		if (nonExisting2Obj)
		{
			Commands->Send_Custom_Event(obj, nonExisting2Obj, 0, 228, 0.0f);
		}

		GameObject *nonExisting3Obj = Commands->Find_Object(101684);
		if (nonExisting3Obj)
		{
			Commands->Send_Custom_Event(obj, nonExisting3Obj, 0, 228, 0.0f);
		}

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_Tank_Entering_Tunnel_Zone_JDG> M01_Tank_Entering_Tunnel_Zone_JDGRegistrant("M01_Tank_Entering_Tunnel_Zone_JDG", "");