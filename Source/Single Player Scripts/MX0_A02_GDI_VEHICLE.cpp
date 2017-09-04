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
#include "MX0_A02_GDI_VEHICLE.h"

// After MX0_A02_Controller receives custom type 202
void MX0_A02_GDI_VEHICLE::Created(GameObject *obj)
{
	Commands->Set_Health(obj, 10.0f);
}

void MX0_A02_GDI_VEHICLE::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	Commands->Set_Health(obj, 10.0f);
}

ScriptRegistrant<MX0_A02_GDI_VEHICLE> MX0_A02_GDI_VEHICLERegistrant("MX0_A02_GDI_VEHICLE", "");