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
#include "M09_Attack_Blocked_False.h"

/*
M03 -> 300052
M09 -> 2005480
*/
void M09_Attack_Blocked_False::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	ActionParamsStruct params;
	params.Set_Basic(this, 90.0f, 1);
	params.Set_Attack(enemy, 200.0f, 5.0f, true);
	params.AttackCheckBlocked = false;

	Commands->Action_Attack(obj, params);
}

ScriptRegistrant<M09_Attack_Blocked_False> M09_Attack_Blocked_FalseRegistrant("M09_Attack_Blocked_False", "");