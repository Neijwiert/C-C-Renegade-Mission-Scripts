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
#include "M03_Tailgun_Fodder.h"

// This script is never used
void M03_Tailgun_Fodder::Created(GameObject *obj)
{
	Commands->Action_Reset(obj, 99.0f);

	Vector3 pos = Commands->Get_Position(obj);
	GameObject *starObj = Commands->Get_A_Star(pos);

	if (starObj)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 99.0f, 0);
		params.Set_Attack(starObj, 200.0f, static_cast<float>(Commands->Get_Difficulty_Level()), true);
		params.Set_Movement(starObj, 0.8f, 5.0f);

		Commands->Action_Attack(obj, params);
	}
}

ScriptRegistrant<M03_Tailgun_Fodder> M03_Tailgun_FodderRegistrant("M03_Tailgun_Fodder", "");