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
#include "M01_DetentionPen_Evac_Controller01_JDG.h"

// This script is never used
void M01_DetentionPen_Evac_Controller01_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 2);
	Auto_Save_Variable(&this->field_24, sizeof(this->field_24), 3);
}

void M01_DetentionPen_Evac_Controller01_JDG::Created(GameObject *obj)
{
	this->field_24 = 0;
}

// Never used, so never receives any customs
void M01_DetentionPen_Evac_Controller01_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (param == 16)
	{
		// Just these calls
		Commands->Find_Object(101929); // detentionCiv1Obj
		Commands->Find_Object(101930); // detentionCiv2Obj
		Commands->Find_Object(101931); // detentionCiv3Obj
	}
	else if (param == 229)
	{
		if (++this->field_24 == 1)
		{
			Commands->Mission_Complete(false);
		}
	}
	else if (param == 27)
	{
		GameObject *field20Obj = Commands->Find_Object(this->field_20);
		GameObject *field1CObj = Commands->Find_Object(this->field_1C);

		if (field20Obj)
		{
			Commands->Send_Custom_Event(obj, field20Obj, 0, 27, 0.0f);
		}

		if (field1CObj)
		{
			Commands->Send_Custom_Event(obj, field1CObj, 0, 27, 0.0f);
		}
	}
}

ScriptRegistrant<M01_DetentionPen_Evac_Controller01_JDG> M01_DetentionPen_Evac_Controller01_JDGRegistrant("M01_DetentionPen_Evac_Controller01_JDG", "");