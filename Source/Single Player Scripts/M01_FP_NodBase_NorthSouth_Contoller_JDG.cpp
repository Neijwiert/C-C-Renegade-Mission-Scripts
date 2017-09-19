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
#include "M01_FP_NodBase_NorthSouth_Contoller_JDG.h"

void M01_FP_NodBase_NorthSouth_Contoller_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->cinematicFileIndex, sizeof(this->cinematicFileIndex), 1);
}

// On level start
void M01_FP_NodBase_NorthSouth_Contoller_JDG::Created(GameObject *obj)
{
	Commands->Start_Timer(obj, this, 60.0f, 0);

	this->cinematicFileIndex = 21;
}

void M01_FP_NodBase_NorthSouth_Contoller_JDG::Timer_Expired(GameObject *obj, int number)
{
	static const char *flyoverCinematicFiles[48] = 
	{
		"X1A_Solo_Orca_14.txt",
		"X1A_Solo_Apache_14.txt",
		"X1A_Solo_Apache_14.txt",
		"X1A_Solo_Apache_14.txt",
		"X1A_Solo_Apache_14.txt",
		"X1A_Solo_Apache_14.txt",
		"X1A_Solo_Apache_14.txt",
		"X1A_Solo_Apache_14.txt",
		"X1A_Solo_Apache_14.txt",
		"X1A_Solo_Apache_14.txt",
		"X1A_Solo_Apache_14.txt",
		"X1A_Solo_Nod_Chinook_14.txt",
		"X1A_Solo_Nod_Chinook_14.txt",
		"X1A_Solo_Nod_Chinook_14.txt",
		"X1A_Solo_Nod_Chinook_14.txt",
		"X1A_Solo_Nod_Chinook_14.txt",
		"X1A_Solo_Nod_Chinook_14.txt",
		"X1A_Solo_Nod_Chinook_14.txt",
		"X1A_Solo_Nod_Chinook_14.txt",
		"X1A_Solo_Nod_Chinook_14.txt",
		"X1A_Solo_Nod_Chinook_14.txt",
		"X1A_Solo_GDI_Chinook_14.txt",
		"X1A_Solo_NodCargo_14.txt",
		"X1A_Solo_A10_14.txt",
		"X1A_Solo_Orca_15.txt",
		"X1A_Solo_Apache_15.txt",
		"X1A_Solo_Apache_15.txt",
		"X1A_Solo_Apache_15.txt",
		"X1A_Solo_Apache_15.txt",
		"X1A_Solo_Apache_15.txt",
		"X1A_Solo_Apache_15.txt",
		"X1A_Solo_Apache_15.txt",
		"X1A_Solo_Apache_15.txt",
		"X1A_Solo_Apache_15.txt",
		"X1A_Solo_Apache_15.txt",
		"X1A_Solo_Nod_Chinook_15.txt",
		"X1A_Solo_Nod_Chinook_15.txt",
		"X1A_Solo_Nod_Chinook_15.txt",
		"X1A_Solo_Nod_Chinook_15.txt",
		"X1A_Solo_Nod_Chinook_15.txt",
		"X1A_Solo_Nod_Chinook_15.txt",
		"X1A_Solo_Nod_Chinook_15.txt",
		"X1A_Solo_Nod_Chinook_15.txt",
		"X1A_Solo_Nod_Chinook_15.txt",
		"X1A_Solo_Nod_Chinook_15.txt",
		"X1A_Solo_GDI_Chinook_15.txt",
		"X1A_Solo_NodCargo_15.txt",
		"X1A_Solo_A10_15.txt"
	};

	int randomIndex = Commands->Get_Random_Int(0, 48);
	while (randomIndex == this->cinematicFileIndex)
	{
		randomIndex = Commands->Get_Random_Int(0, 48);
	}

	this->cinematicFileIndex = randomIndex;

	GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
	Commands->Attach_Script(invisObj, "Test_Cinematic", flyoverCinematicFiles[this->cinematicFileIndex]);
	
	float randInterval = Commands->Get_Random(30.0f, 60.0f);
	Commands->Start_Timer(obj, this, randInterval, 0);
}

ScriptRegistrant<M01_FP_NodBase_NorthSouth_Contoller_JDG> M01_FP_NodBase_NorthSouth_Contoller_JDGRegistrant("M01_FP_NodBase_NorthSouth_Contoller_JDG", "");