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
#include "MX0_A03_GDI_TANK_DROP_ZONE_DAK.h"

// This script is never used
void MX0_A03_GDI_TANK_DROP_ZONE_DAK::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->canDoEnteredEvents, sizeof(this->canDoEnteredEvents), 1);
}

void MX0_A03_GDI_TANK_DROP_ZONE_DAK::Created(GameObject *obj)
{
	this->canDoEnteredEvents = true;
}

void MX0_A03_GDI_TANK_DROP_ZONE_DAK::Entered(GameObject *obj, GameObject *enterer)
{
	if (this->canDoEnteredEvents)
	{
		GameObject *tankDropObj = Commands->Find_Object(1400057);
		if (tankDropObj)
		{
			Commands->Attach_Script(tankDropObj, "Test_Cinematic", "XG_A03_Tank_Drop.txt");
		}

		this->canDoEnteredEvents = false;
	}
}

ScriptRegistrant<MX0_A03_GDI_TANK_DROP_ZONE_DAK> MX0_A03_GDI_TANK_DROP_ZONE_DAKRegistrant("MX0_A03_GDI_TANK_DROP_ZONE_DAK", "");