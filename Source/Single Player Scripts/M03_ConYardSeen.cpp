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
#include "M03_ConYardSeen.h"

/*
M03 -> 1144636
*/
void M03_ConYardSeen::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
}

void M03_ConYardSeen::Created(GameObject *obj)
{
	this->field_1C = false;
}

// TODO
void M03_ConYardSeen::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 900)
	{
		this->field_1C = true;
	}
}

void M03_ConYardSeen::Entered(GameObject *obj, GameObject *enterer)
{
	if (!this->field_1C)
	{
		this->field_1C = true;

		GameObject *M03MissionControllerObj = Commands->Find_Object(1100004);
		Commands->Send_Custom_Event(obj, M03MissionControllerObj, 300, 1, 0.0f);
	}
}

ScriptRegistrant<M03_ConYardSeen> M03_ConYardSeenRegistrant("M03_ConYardSeen", "");