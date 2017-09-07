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
#include "MX0_AmbientBattle.h"

// This script is never used
void MX0_AmbientBattle::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->soundId, sizeof(this->soundId), 1);
}

void MX0_AmbientBattle::Created(GameObject *obj)
{
	Timer_Expired(obj, 107);
}

void MX0_AmbientBattle::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Triggered after sound is done, see timer_expired
	if (type == CUSTOM_EVENT_SOUND_ENDED && param == this->soundId)
	{
		Commands->Start_Timer(obj, this, 3.0f, 107);
	}
}

void MX0_AmbientBattle::Timer_Expired(GameObject *obj, int number)
{
	// Triggered on creation or after 3 seconds in custom
	if (number == 107)
	{
		Vector3 pos = Commands->Get_Position(obj);
		this->soundId = Commands->Create_Sound("MX0_AmbientBattle_DME", pos, obj);

		Commands->Monitor_Sound(obj, soundId);
	}
}

ScriptRegistrant<MX0_AmbientBattle> MX0_AmbientBattleRegistrant("MX0_AmbientBattle", "");