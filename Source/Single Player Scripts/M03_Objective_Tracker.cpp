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
#include "M03_Objective_Tracker.h"

/*
M03 -> 1100004
*/
void M03_Objective_Tracker::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
}

void M03_Objective_Tracker::Created(GameObject *obj)
{
	this->field_1C = 0;
}

// TODO
void M03_Objective_Tracker::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 300 || type == 308 || type == 309)
	{
		if (param == 1)
		{
			this->field_1C++;
		}
	}
	if (type == 8000)
	{
		if (param == 8000 && sender)
		{
			Commands->Send_Custom_Event(obj, sender, 8000, this->field_1C, 0.0f);
		}
	}
}

ScriptRegistrant<M03_Objective_Tracker> M03_Objective_TrackerRegistrant("M03_Objective_Tracker", "");