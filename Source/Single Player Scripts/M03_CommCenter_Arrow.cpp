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
#include "M03_CommCenter_Arrow.h"

/*
M03 -> 2009818
*/
void M03_CommCenter_Arrow::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->field_1D, sizeof(this->field_1D), 2);
}

void M03_CommCenter_Arrow::Created(GameObject *obj)
{
	this->field_1C = true;
	this->field_1D = false;
}

// TODO
void M03_CommCenter_Arrow::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 40002)
	{
		this->field_1D = true;

		GameObject *M03CommCenterWarningScriptZone1Obj = Commands->Find_Object(1208215);
		Commands->Send_Custom_Event(obj, M03CommCenterWarningScriptZone1Obj, 40002, 0, 0.0f);

		GameObject *M03CommCenterWarningScriptZone2Obj = Commands->Find_Object(1208216);
		Commands->Send_Custom_Event(obj, M03CommCenterWarningScriptZone2Obj, 40002, 0, 0.0f);

		GameObject *M03CommCenterWarningScriptZone3Obj = Commands->Find_Object(1208217);
		Commands->Send_Custom_Event(obj, M03CommCenterWarningScriptZone3Obj, 40002, 0, 0.0f);

		GameObject *M03CommCenterWarningScriptZone4Obj = Commands->Find_Object(1208218);
		Commands->Send_Custom_Event(obj, M03CommCenterWarningScriptZone4Obj, 40002, 0, 0.0f);

		GameObject *M03PowerPlantWarningScriptZone1Obj = Commands->Find_Object(1208219);
		Commands->Send_Custom_Event(obj, M03PowerPlantWarningScriptZone1Obj, 40002, 0, 0.0f);

		GameObject *M03PowerPlantWarningScriptZone2Obj = Commands->Find_Object(1208220);
		Commands->Send_Custom_Event(obj, M03PowerPlantWarningScriptZone2Obj, 40002, 0, 0.0f);
	}
	else if (type == 40003)
	{
		GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);
		Commands->Send_Custom_Event(obj, M03ObjectiveControllerObj, 312, 1, 0.0f);

		if (!this->field_1D)
		{
			Commands->Send_Custom_Event(obj, M03ObjectiveControllerObj, 308, 2, 0.0f);

			Commands->Set_HUD_Help_Text(8261, Vector3(0.196f, 0.882f, 0.196f)); // Communications Center Offline\n

			Commands->Start_Timer(obj, this, 4.0f, 40025);
		}
	}
	else if (type == 40010)
	{
		GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);
		Commands->Send_Custom_Event(obj, M03ObjectiveControllerObj, 309, 1, 0.0f);

		if (!this->field_1D)
		{
			Commands->Send_Custom_Event(obj, M03ObjectiveControllerObj, 308, 2, 0.0);

			Commands->Set_HUD_Help_Text(8259, Vector3(0.196f, 0.882f, 0.196f)); // Power Compromised\n

			Commands->Start_Timer(obj, this, 4.0f, 40025);
		}
	}
}

// TODO
void M03_CommCenter_Arrow::Timer_Expired(GameObject *obj, int number)
{
	if (number == 40025)
	{
		Commands->Mission_Complete(false);
	}
}

ScriptRegistrant<M03_CommCenter_Arrow> M03_CommCenter_ArrowRegistrant("M03_CommCenter_Arrow", "");