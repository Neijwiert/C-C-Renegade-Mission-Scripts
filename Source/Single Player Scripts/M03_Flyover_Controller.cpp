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
#include "M03_Flyover_Controller.h"

/*
M03 -> 1141163
*/
void M03_Flyover_Controller::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->lastFlyoverCinematicIndex, sizeof(this->lastFlyoverCinematicIndex), 1);
	Auto_Save_Variable(&this->active, sizeof(this->active), 2);
}

void M03_Flyover_Controller::Created(GameObject *obj)
{
	Commands->Start_Timer(obj, this, 25.0f, 0);

	this->lastFlyoverCinematicIndex = 21;
	this->active = true;
}

void M03_Flyover_Controller::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Never received
	if (type == 511)
	{
		if (param == 511)
		{
			this->active = false;
		}
	}

	// Never received
	else if (type == 512)
	{
		if (param == 512)
		{
			this->active = true;
		}
	}
}

void M03_Flyover_Controller::Timer_Expired(GameObject *obj, int number)
{
	static const char *flyoverCinematicFiles[17] =
	{
		"A-10_1.txt",
		"A-10_2.txt",
		"A-10_3.txt",
		"A-10_4.txt",
		"A-10_5.txt",
		"A-10_6.txt",
		"Orca_1.txt",
		"Orca_2.txt",
		"Orca_3.txt",
		"Orca_4.txt",
		"Orca_5.txt",
		"Orca_1.txt",
		"Orca_2.txt",
		"Orca_3.txt",
		"Orca_4.txt",
		"Orca_5.txt",
		"Orca_6.txt"
	};

	int rand = static_cast<int>(Commands->Get_Random(0.0f, 16.999901f));
	if (this->active || rand <= 5 || rand > 10)
	{
		while (rand == this->lastFlyoverCinematicIndex)
		{
			rand = static_cast<int>(Commands->Get_Random(0.0f, 16.999901f));
		}

		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
		Commands->Attach_Script(invisObj, "Test_Cinematic", flyoverCinematicFiles[rand]);

		Commands->Start_Timer(obj, this, 25.0f, 0);

		this->lastFlyoverCinematicIndex = rand;
	}
	else
	{
		Commands->Start_Timer(obj, this, 25.0f, 0);
	}
}

ScriptRegistrant<M03_Flyover_Controller> M03_Flyover_ControllerRegistrant("M03_Flyover_Controller", "");