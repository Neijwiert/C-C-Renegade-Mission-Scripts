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
#include "M03_Tailgun_Fodder_Zone.h"

// This script is never used
void M03_Tailgun_Fodder_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
}

void M03_Tailgun_Fodder_Zone::Created(GameObject *obj)
{
	this->field_1C = true;

	int spawner1Id = Get_Int_Parameter("Spawner_ID_1");
	Commands->Enable_Spawner(spawner1Id, false);

	int spawner2Id = Get_Int_Parameter("Spawner_ID_2");
	Commands->Enable_Spawner(spawner2Id, false);

	int spawner3Id = Get_Int_Parameter("Spawner_ID_3");
	Commands->Enable_Spawner(spawner3Id, false);
}

void M03_Tailgun_Fodder_Zone::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 5000)
	{
		if (param == 1)
		{
			this->field_1C = true;
		}
		else if (param == 2)
		{
			this->field_1C = false;

			Entered(obj, NULL); // Is the second argument NULL?

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			Exited(obj, starObj);
		}
	}
	else if (type == 200)
	{
		if (param == 200)
		{
			Commands->Destroy_Object(obj);
		}
	}
	else if (type == 300)
	{
		if (param == 300)
		{
			Exited(obj, sender);
		}
	}
}

void M03_Tailgun_Fodder_Zone::Entered(GameObject *obj, GameObject *enterer)
{
	if (this->field_1C)
	{
		int spawner1Id = Get_Int_Parameter("Spawner_ID_1");
		Commands->Enable_Spawner(spawner1Id, true);

		int spawner2Id = Get_Int_Parameter("Spawner_ID_2");
		Commands->Enable_Spawner(spawner2Id, true);

		int spawner3Id = Get_Int_Parameter("Spawner_ID_3");
		Commands->Enable_Spawner(spawner3Id, true);
	}
}

void M03_Tailgun_Fodder_Zone::Exited(GameObject *obj, GameObject *exiter)
{
	int spawner1Id = Get_Int_Parameter("Spawner_ID_1");
	Commands->Enable_Spawner(spawner1Id, false);

	int spawner2Id = Get_Int_Parameter("Spawner_ID_2");
	Commands->Enable_Spawner(spawner2Id, false);

	int spawner3Id = Get_Int_Parameter("Spawner_ID_3");
	Commands->Enable_Spawner(spawner3Id, false);
}

ScriptRegistrant<M03_Tailgun_Fodder_Zone> M03_Tailgun_Fodder_ZoneRegistrant("M03_Tailgun_Fodder_Zone", "Spawner_ID_1:int, Spawner_ID_2:int, Spawner_ID_3:int");