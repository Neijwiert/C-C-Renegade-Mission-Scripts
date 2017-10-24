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
#include "M03_Chinook_Drop_Soldiers_GDI.h"

// This script is never used
void M03_Chinook_Drop_Soldiers_GDI::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->controllerId, sizeof(this->controllerId), 1);
	Auto_Save_Variable(&this->inletNumber, sizeof(this->inletNumber), 2);
	Auto_Save_Variable(&this->beachNumber, sizeof(this->beachNumber), 2);
}

void M03_Chinook_Drop_Soldiers_GDI::Created(GameObject *obj)
{
	this->controllerId = Get_Int_Parameter("Controller_ID");
	this->beachNumber = 0;
	this->inletNumber = 0;
}

void M03_Chinook_Drop_Soldiers_GDI::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	GameObject *paramObj = Commands->Find_Object(param);
	if (type > 4000 && paramObj)
	{
		char buffer[80];
		sprintf(buffer, "%d,2000,%d", type - 4001, this->controllerId);
		Commands->Attach_Script(paramObj, "M03_Chinook_Spawned_Soldier_GDI", buffer);

		if (type == 4002)
		{
			char buffer[12];
			sprintf(buffer, "%d", this->inletNumber);

			this->inletNumber = (this->inletNumber + 1) % 3;

			Commands->Attach_Script(paramObj, "M03_Inlet_Soldier_GDI", buffer);
		}
		else if (type == 4001)
		{
			char buffer[12];
			sprintf(buffer, "%d", this->beachNumber);

			this->beachNumber = (this->beachNumber + 1) % 3;

			Commands->Attach_Script(paramObj, "M03_Beach_Soldier_GDI", buffer);
		}
	}
}

ScriptRegistrant<M03_Chinook_Drop_Soldiers_GDI> M03_Chinook_Drop_Soldiers_GDIRegistrant("M03_Chinook_Drop_Soldiers_GDI", "Controller_ID:int");