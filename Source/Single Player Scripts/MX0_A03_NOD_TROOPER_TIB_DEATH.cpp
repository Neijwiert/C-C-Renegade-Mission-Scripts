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
#include "MX0_A03_NOD_TROOPER_TIB_DEATH.h"

void MX0_A03_NOD_TROOPER_TIB_DEATH::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *MX0A03ControllerObj = Commands->Find_Object(1400041);
	if (MX0A03ControllerObj)
	{
		Commands->Send_Custom_Event(obj, MX0A03ControllerObj, 415, 0, 2.0f);
	}
}

void MX0_A03_NOD_TROOPER_TIB_DEATH::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 9035)
	{
		this->field_1C = param;
	}
}

ScriptRegistrant<MX0_A03_NOD_TROOPER_TIB_DEATH> MX0_A03_NOD_TROOPER_TIB_DEATHRegistrant("MX0_A03_NOD_TROOPER_TIB_DEATH", "");