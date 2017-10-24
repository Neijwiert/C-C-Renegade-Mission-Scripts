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
#include "M03_Move_Commando_To_Start.h"

// This script is never used
void M03_Move_Commando_To_Start::Created(GameObject *obj)
{
	Commands->Start_Timer(obj, this, 1.0f, 0);
}

void M03_Move_Commando_To_Start::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 100 && param == 100)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Set_Position(starObj, Vector3(-118.12f, -120.12f, -0.72f));
	}
}

void M03_Move_Commando_To_Start::Timer_Expired(GameObject *obj, int number)
{
	if (!number)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Set_Position(starObj, Vector3(-328.09f, 75.65f, 4.25f));
	}
}

ScriptRegistrant<M03_Move_Commando_To_Start> M03_Move_Commando_To_StartRegistrant("M03_Move_Commando_To_Start", "");