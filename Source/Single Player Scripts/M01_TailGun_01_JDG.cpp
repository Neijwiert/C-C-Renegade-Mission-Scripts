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
#include "M01_TailGun_01_JDG.h"

/*
M01 -> 108707
*/
void M01_TailGun_01_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_Player_Is_Entering_Tailgun_Alley_Backway_JDG or M01_Player_Is_Entering_Tailgun_Alley_JDG when entered
	if (param == 16)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 65);
		
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		params.Set_Attack(starObj, 50.0f, 0.0f, true);
		params.AttackCheckBlocked = false;

		Commands->Action_Attack(obj, params);
	}
}

ScriptRegistrant<M01_TailGun_01_JDG> M01_TailGun_01_JDGRegistrant("M01_TailGun_01_JDG", "");