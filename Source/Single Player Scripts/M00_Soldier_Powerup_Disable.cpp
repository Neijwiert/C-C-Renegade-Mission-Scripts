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
#include "M00_Soldier_Powerup_Disable.h"

void M00_Soldier_Powerup_Disable::Created(GameObject *obj)
{
	Commands->Start_Timer(obj, this, 1.0f, 1);
}

void M00_Soldier_Powerup_Disable::Timer_Expired(GameObject *obj, int number) 
{
	// Triggered 1 Second after creation
	if (number == 1)
	{
		Commands->Send_Custom_Event(obj, obj, 9024, 0, 0.0f);
	}
}

ScriptRegistrant<M00_Soldier_Powerup_Disable> M00_Soldier_Powerup_DisableRegistrant("M00_Soldier_Powerup_Disable", ""); 