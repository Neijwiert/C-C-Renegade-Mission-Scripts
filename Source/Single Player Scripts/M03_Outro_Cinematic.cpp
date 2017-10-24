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
#include "M03_Outro_Cinematic.h"

// This script is never used
void M03_Outro_Cinematic::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->entererId, sizeof(this->entererId), 1);
}

void M03_Outro_Cinematic::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 8000 && param > 2)
	{
		GameObject *entererObj = Commands->Find_Object(this->entererId);
		Commands->Set_Position(entererObj, Vector3(-324.49f, 71.6f, 4.26f));

		Commands->Create_Object("Finale Controller", Vector3(0.0f, 0.0f, 0.0f));

		Commands->Start_Timer(obj, this, 625.0f / 30.0f, 0);
	}
}

void M03_Outro_Cinematic::Timer_Expired(GameObject *obj, int number)
{
	GameObject *M03MissionControllerObj = Commands->Find_Object(1100004);
	if (M03MissionControllerObj)
	{
		Commands->Send_Custom_Event(obj, M03MissionControllerObj, 310, 1, 0.0f);
	}
}

void M03_Outro_Cinematic::Entered(GameObject *obj, GameObject *enterer)
{
	this->entererId = Commands->Get_ID(enterer);

	GameObject *M03MissionControllerObj = Commands->Find_Object(1100004);
	if (M03MissionControllerObj)
	{
		Commands->Send_Custom_Event(obj, M03MissionControllerObj, 8000, 8000, 0.0f);
	}
}

ScriptRegistrant<M03_Outro_Cinematic> M03_Outro_CinematicRegistrant("M03_Outro_Cinematic", "");