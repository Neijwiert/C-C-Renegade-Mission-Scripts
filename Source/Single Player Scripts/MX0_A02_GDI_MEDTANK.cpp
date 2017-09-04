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
#include "MX0_A02_GDI_MEDTANK.h"

void MX0_A02_GDI_MEDTANK::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->vehicleEntered, sizeof(this->vehicleEntered), 1);
}

// After MX0_A02_ACTOR triggers the timer with number 215
void MX0_A02_GDI_MEDTANK::Created(GameObject *obj)
{
	this->vehicleEntered = false;
}

void MX0_A02_GDI_MEDTANK::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	float maxHealth = Commands->Get_Max_Health(obj);
	Commands->Set_Health(obj, maxHealth);
}

void MX0_A02_GDI_MEDTANK::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if(type == CUSTOM_EVENT_VEHICLE_ENTERED && !this->vehicleEntered)
	{
		this->vehicleEntered = true;

		Commands->Fade_Background_Music("Level 0 Tiberium.mp3", 2, 2);
		
		GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
		if (MX0A02ControllerObj)
		{
			Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 262, 0, 0.0f);
		}
	}
}

ScriptRegistrant<MX0_A02_GDI_MEDTANK> MX0_A02_GDI_MEDTANKRegistrant("MX0_A02_GDI_MEDTANK", "");