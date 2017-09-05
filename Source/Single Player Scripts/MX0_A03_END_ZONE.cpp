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
#include "MX0_A03_END_ZONE.h"

void MX0_A03_END_ZONE::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->canSendCustoms, sizeof(this->canSendCustoms), 1);
}

void MX0_A03_END_ZONE::Created(GameObject *obj)
{
	this->canSendCustoms = true;
}

void MX0_A03_END_ZONE::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 414)
	{
		this->canSendCustoms = false;
	}
	else if (type == 9035)
	{
		this->field_1C = param;
	}
}

void MX0_A03_END_ZONE::Entered(GameObject *obj, GameObject *enterer)
{
	if (this->canSendCustoms)
	{
		GameObject *MX0A03ControllerObj = Commands->Find_Object(1400041);
		if (MX0A03ControllerObj)
		{
			Commands->Send_Custom_Event(obj, MX0A03ControllerObj, 9035, 12, 0.0f);
		}

		GameObject *field1CObj = Commands->Find_Object(this->field_1C);
		if (field1CObj)
		{
			Commands->Send_Custom_Event(obj, field1CObj, 4, 0, 0.0f);
		}

		if (MX0A03ControllerObj)
		{
			Commands->Send_Custom_Event(obj, MX0A03ControllerObj, 405, 1, 0.0f);
		}

		this->canSendCustoms = 0;
	}
}

ScriptRegistrant<MX0_A03_END_ZONE> MX0_A03_END_ZONERegistrant("MX0_A03_END_ZONE", "");