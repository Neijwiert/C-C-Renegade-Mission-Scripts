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
#include "M03_Inlet_Nod_Reinforcements.h"

/*
M03 -> 1141180
*/
void M03_Inlet_Nod_Reinforcements::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->inletSamKilledCount, sizeof(this->inletSamKilledCount), 1);
}

void M03_Inlet_Nod_Reinforcements::Created(GameObject *obj)
{
	this->inletSamKilledCount = 0;
}

void M03_Inlet_Nod_Reinforcements::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M00_Trigger_When_Killed_RMV after 0 seconds when killed. (id = 300056, 1100020)
	if (type == 300 && param == 300)
	{
		this->inletSamKilledCount++;
	}

	if (this->inletSamKilledCount > 1)
	{
		Commands->Start_Timer(obj, this, 30 / 30.0f, 0);
		Commands->Start_Timer(obj, this, 129 / 30.0f, 1);
		Commands->Start_Timer(obj, this, 243 / 30.0f, 2);
	}
}

void M03_Inlet_Nod_Reinforcements::Timer_Expired(GameObject *obj, int number)
{
	// Triggered after 30 / 30.0f seconds, see custom
	if (!number)
	{
		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(51.0f, -91.0f, 2.0f));
		Commands->Set_Facing(invisObj, 75.0f);
		Commands->Attach_Script(invisObj, "M03_Chinook_ParaDrop", "M03_Paratrooper");
	}

	// Triggered after 129 / 30.0f seconds, see custom
	else if (number == 1)
	{
		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(97.0f, -46.0f, 2.0f));
		Commands->Set_Facing(invisObj, 170.0f);
		Commands->Attach_Script(invisObj, "M03_Chinook_ParaDrop", "M03_Paratrooper");
	}

	// Triggered after 243 / 30.0f seconds, see custom
	else if (number == 2)
	{
		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(64.0f, -32.0f, 2.0f));
		Commands->Set_Facing(invisObj, 70.0f);
		Commands->Attach_Script(invisObj, "M03_Chinook_ParaDrop", "M03_Paratrooper");
	}
}

ScriptRegistrant<M03_Inlet_Nod_Reinforcements> M03_Inlet_Nod_ReinforcementsRegistrant("M03_Inlet_Nod_Reinforcements", "");