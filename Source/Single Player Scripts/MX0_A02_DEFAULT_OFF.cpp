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
#include "MX0_A02_DEFAULT_OFF.h"

void MX0_A02_DEFAULT_OFF::Register_Auto_Save_Variables()
{

}

// This script is never used
void MX0_A02_DEFAULT_OFF::Created(GameObject *obj)
{
	Commands->Innate_Disable(obj);
}

void MX0_A02_DEFAULT_OFF::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (obj != Commands->Get_A_Star(pos))
	{
		Commands->Apply_Damage(obj, 10000.0f, "Blamokiller", obj);
	}
}

ScriptRegistrant<MX0_A02_DEFAULT_OFF> MX0_A02_DEFAULT_OFFRegistrant("MX0_A02_DEFAULT_OFF", "");