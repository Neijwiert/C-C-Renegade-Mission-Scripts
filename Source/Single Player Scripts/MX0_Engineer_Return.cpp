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
#include "MX0_Engineer_Return.h"

// This script is never used
void MX0_Engineer_Return::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->isZoneEntered, sizeof(this->isZoneEntered), 1);
}

void MX0_Engineer_Return::Created(GameObject *obj)
{
	this->isZoneEntered = false;
}

void MX0_Engineer_Return::Entered(GameObject *obj, GameObject *enterer)
{
	if (!this->isZoneEntered)
	{
		this->isZoneEntered = true;

		GameObject *MX0MissionStartDMEObj = Commands->Find_Object(1200001);
		Commands->Send_Custom_Event(obj, MX0MissionStartDMEObj, 130, 1, 0.0f);
	}
}

ScriptRegistrant<MX0_Engineer_Return> MX0_Engineer_ReturnRegistrant("MX0_Engineer_Return", "");