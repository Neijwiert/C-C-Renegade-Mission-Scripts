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
#include "M03_Chinook_Reinforcements.h"

/*
M03 -> 1141141
*/
void M03_Chinook_Reinforcements::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(this->areaTrooperDeathCount, sizeof(this->areaTrooperDeathCount), 1);
	Auto_Save_Variable(this->areaActive, sizeof(this->areaActive), 2);
	Auto_Save_Variable(&this->triggerCount, sizeof(this->triggerCount), 3);
	Auto_Save_Variable(&this->field_30, sizeof(this->field_30), 4);
	Auto_Save_Variable(&this->field_34, sizeof(this->field_34), 5);
	Auto_Save_Variable(&this->targetKillCount, sizeof(this->targetKillCount), 6);
}

void M03_Chinook_Reinforcements::Created(GameObject *obj)
{
	this->triggerCount = Get_Int_Parameter("Trigger_Count");
	this->field_30 = 0;
	this->targetKillCount = 0;
	this->field_34 = false;

	for (int x = 0; x < 3; x++)
	{
		this->areaTrooperDeathCount[x] = 0;
		this->areaActive[x] = true;
	}
}

void M03_Chinook_Reinforcements::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from ourselves after 0 seconds when custom type 4000 and param 4000 is received
	// Received from M00_Trigger_When_Killed_RMV after 0 seconds when killed. type = 2000, param = 1 (id = 300056, 1100020)
	if (type == 2000)
	{
		if (param >= 0 && param <= 2)
		{
			this->areaTrooperDeathCount[param]++;

			if (!param)
			{
				if (this->areaTrooperDeathCount[0] <= 2 && this->field_30 >= 3 - this->areaTrooperDeathCount[0])
				{
					Commands->Find_Object(1100004); // Yep, just a Find_Object here
				}
			}

			if (this->areaTrooperDeathCount[param] >= this->triggerCount && this->areaActive[param] && Commands->Find_Object(1100003)) // GDI Gunboat
			{
				this->areaTrooperDeathCount[param] = 0;

				Commands->Create_Sound("00-N180E", Vector3(0.0f, 0.0f, 0.0f), obj);

				if (param == 1)
				{
					const char *inletPreset = Get_Parameter("Inlet_Preset");
					GameObject *inletPresetObj = Commands->Create_Object(inletPreset, Vector3(84.398f, -45.703f, 1.0f));
					Commands->Set_Facing(inletPresetObj, 100.0f);
				}
				else if (param == 2)
				{
					const char *basePreset = Get_Parameter("Base_Preset");
					GameObject *basePresetObj = Commands->Create_Object(basePreset, Vector3(-94.822f, 15.727f, 10.0f));
					Commands->Set_Facing(basePresetObj, -155.0f);
				}
				else if (!param)
				{
					GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-84.609f, -91.188f, 1.0f));
					Commands->Set_Facing(invisObj, 190.0f);
					Commands->Attach_Script(invisObj, "Test_Cinematic", "X3I_TroopDrop1.txt");
				}
			}
		}
	}

	// Received from RMV_Trigger_Zone after 0 seconds when entered. type = 3000, param = 1 (id = 1100007)
	// Received from RMV_Trigger_Zone after 0 seconds when entered. type = 3000, param = 0 (id = 1100013)
	// Received from RMV_Trigger_Zone after 0 seconds when entered. type = 3000, param = 2 (id = 1141149)
	else if (type == 3000)
	{
		if (param >= 0 && param <= 2)
		{
			this->areaActive[param] = false;
		}
	}

	// Received from RMV_Trigger_Zone after 0 seconds when entered. type = 4000, param = 4000 (id = 1100005, 1144502)
	// Received from M00_Trigger_When_Killed_RMV after 10 seconds when killed. (id = 300059)
	// Received from M00_Trigger_When_Killed_RMV after 0 seconds when killed. (id = 300058)
	else if (type == 4000)
	{
		if (param == 4000 && ++this->targetKillCount >= 3)
		{
			this->targetKillCount = 0;
			this->areaTrooperDeathCount[2] = 2;

			Commands->Send_Custom_Event(obj, obj, 2000, 2, 0.0f);
		}
	}

	// Never received
	else if (type == 12000)
	{
		if (param == 12000 && ++this->field_30 >= 3 - this->areaTrooperDeathCount[0] && !this->field_34)
		{
			Commands->Find_Object(1100004); // Yep, just a Find_Object here
		}
	}
}

ScriptRegistrant<M03_Chinook_Reinforcements> M03_Chinook_ReinforcementsRegistrant("M03_Chinook_Reinforcements", "Beach_Preset:string, Inlet_Preset:string, Base_Preset:string, Trigger_Count:int");