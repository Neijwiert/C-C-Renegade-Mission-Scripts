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
#include "M01_Visceroid03_JDG.h"

/*
M01 -> 106907
*/
void M01_Visceroid03_JDG::Created(GameObject *obj)
{
	Commands->Innate_Disable(obj);
}

void M01_Visceroid03_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_TibCave_StartZone_JDG when entered
	if (!type && param == 16)
	{
		Commands->Innate_Enable(obj);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 16);
		
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		params.Set_Attack(starObj, 10.0f, 0.0f, true);
		params.Set_Movement(starObj, 0.8f, 2.0f);
		params.AttackCheckBlocked = false;

		Commands->Action_Attack(obj, params);
	}
}

ScriptRegistrant<M01_Visceroid03_JDG> M01_Visceroid03_JDGRegistrant("M01_Visceroid03_JDG", "");