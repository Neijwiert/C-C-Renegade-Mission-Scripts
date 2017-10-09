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
#include "M00_Damaged_Warning.h"

void M00_Damaged_Warning::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->damagedSoundIntervalActive, sizeof(this->damagedSoundIntervalActive), 1);
}

// When timer expires in M00_Put_Script_On_Commando attached to star
void M00_Damaged_Warning::Created(GameObject *obj)
{
	this->damagedSoundIntervalActive = false;
}

void M00_Damaged_Warning::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (Commands->Get_Health(obj) <= 25.0f && !this->damagedSoundIntervalActive)
	{
		Commands->Create_Sound("00-N112E", Vector3(0.0f, 0.0f, 0.0f), obj);

		this->damagedSoundIntervalActive = true;

		Commands->Start_Timer(obj, this, 30.0f, 0);
	}
}

void M00_Damaged_Warning::Timer_Expired(GameObject *obj, int number)
{
	this->damagedSoundIntervalActive = false;
}

ScriptRegistrant<M00_Damaged_Warning> M00_Damaged_WarningRegistrant("M00_Damaged_Warning", "");