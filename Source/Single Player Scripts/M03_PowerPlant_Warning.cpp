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
#include "M03_PowerPlant_Warning.h"

/*
M03 -> 1208220 1208219 1208218
*/
void M03_PowerPlant_Warning::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->powerPlantEntered, sizeof(this->powerPlantEntered), 1);
	Auto_Save_Variable(&this->zoneEntered, sizeof(this->zoneEntered), 2);
}

void M03_PowerPlant_Warning::Created(GameObject *obj)
{
	this->powerPlantEntered = false;
	this->zoneEntered = false;
}

void M03_PowerPlant_Warning::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M03_CommCenter_Arrow after 0 seconds when custom type 40002 is received
	// Received from ourselves after 0 seconds when entered
	if (type == 40002)
	{
		this->powerPlantEntered = true;
	}
}

void M03_PowerPlant_Warning::Entered(GameObject *obj, GameObject *enterer)
{
	if (!this->powerPlantEntered && !this->zoneEntered)
	{
		this->zoneEntered = true;

		GameObject *M03PowerPlantWarningScriptZone1Obj = Commands->Find_Object(1208218);
		Commands->Send_Custom_Event(obj, M03PowerPlantWarningScriptZone1Obj, 40002, 0, 0.0f);

		GameObject *M03PowerPlantWarningScriptZone2Obj = Commands->Find_Object(1208219);
		Commands->Send_Custom_Event(obj, M03PowerPlantWarningScriptZone2Obj, 40002, 0, 0.0f);

		GameObject *M03PowerPlantWarningScriptZone3Obj = Commands->Find_Object(1208220);
		Commands->Send_Custom_Event(obj, M03PowerPlantWarningScriptZone3Obj, 40002, 0, 0.0f);

		Commands->Set_HUD_Help_Text(8258, Vector3(0.196f, 0.882f, 0.196f)); // Maintain Power Plant Integrity\n
	}
}

ScriptRegistrant<M03_PowerPlant_Warning> M03_PowerPlant_WarningRegistrant("M03_PowerPlant_Warning", "");