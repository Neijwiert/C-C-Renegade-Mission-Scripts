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
#include "M01_Destroyed_SAMSITE_JDG.h"

// When M03_SAM_Site_Logic is killed
void M01_Destroyed_SAMSITE_JDG::Created(GameObject *obj)
{
	ActionParamsStruct params;

	Vector3 pos = Commands->Get_Position(obj);

	params.Set_Basic(this, 100.0f, 20);
	params.Set_Attack(Vector3(pos.X, pos.Y, pos.Z + 5.0f), 0.0f, 0.0f, true);
	params.AttackCheckBlocked = true;

	Commands->Action_Attack(obj, params);
}

ScriptRegistrant<M01_Destroyed_SAMSITE_JDG> M01_Destroyed_SAMSITE_JDGRegistrant("M01_Destroyed_SAMSITE_JDG", "");