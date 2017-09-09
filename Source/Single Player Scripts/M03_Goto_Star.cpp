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
#include "M03_Goto_Star.h"

// TODO (When created?)
void M03_Goto_Star::Created(GameObject *obj)
{
	Commands->Start_Timer(obj, this, 3.5f, 20000);
}

void M03_Goto_Star::Timer_Expired(GameObject *obj, int number)
{
	if (number == 20000)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 20000);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		params.Set_Movement(starObj, 0.8f, 2.5f);
		params.Set_Attack(starObj, 50.0f, 1.0f, true);

		Commands->Action_Attack(obj, params);
		Commands->Start_Timer(obj, this, 3.0f, 20000);
	}
}

ScriptRegistrant<M03_Goto_Star> M03_Goto_StarRegistrant("M03_Goto_Star", "");