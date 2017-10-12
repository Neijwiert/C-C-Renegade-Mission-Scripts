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
#include "M00_Cinematic_Attack_Command_DLS.h"

void M00_Cinematic_Attack_Command_DLS::Created(GameObject *obj)
{
	float facing = Commands->Get_Facing(obj);
	Vector3 pos = Commands->Get_Position(obj);
	
	pos.X += WWMath::Cos(DEG_TO_RADF(facing)) * 10.0f;
	pos.Y += WWMath::Sin(DEG_TO_RADF(facing)) * 10.0f;
	pos.Z += 2.0f;

	ActionParamsStruct params;
	params.Set_Basic(this, 99.0f, 1);
	params.Set_Attack(pos, 100.0f, 0.0f, true);
	params.AttackForceFire = true;

	Commands->Action_Attack(obj, params);

	float attackDuration = Get_Float_Parameter("AttackDuration");
	Commands->Send_Custom_Event(obj, obj, 1, 1, attackDuration);
}

void M00_Cinematic_Attack_Command_DLS::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	Commands->Action_Reset(obj, 100.0f);
}

ScriptRegistrant<M00_Cinematic_Attack_Command_DLS> M00_Cinematic_Attack_Command_DLSRegistrant("M00_Cinematic_Attack_Command_DLS", "AttackDuration=1.0:float");