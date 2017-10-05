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
#include "M01_Whack_A_Mole_Exit_Zone_JDG.h"

/*
M01 -> 101207
*/
void M01_Whack_A_Mole_Exit_Zone_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->starInWhackAMoleArea, sizeof(this->starInWhackAMoleArea), 1);
}

void M01_Whack_A_Mole_Exit_Zone_JDG::Created(GameObject *obj)
{
	this->starInWhackAMoleArea = false;
}

void M01_Whack_A_Mole_Exit_Zone_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_Whack_A_Mole_Enter_Zone_JDG when entered
	if (param == 203)
	{
		this->starInWhackAMoleArea = true;
	}
}

void M01_Whack_A_Mole_Exit_Zone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos) && this->starInWhackAMoleArea)
	{
		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 204, 0.0f);

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_Whack_A_Mole_Exit_Zone_JDG> M01_Whack_A_Mole_Exit_Zone_JDGRegistrant("M01_Whack_A_Mole_Exit_Zone_JDG", "");