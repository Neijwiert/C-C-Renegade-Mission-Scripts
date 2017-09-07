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
#include "M00_Powerup_Destroy.h"

void M00_Powerup_Destroy::Created(GameObject *obj)
{
	float delay = 30.0f - static_cast<float>(10 * Commands->Get_Difficulty_Level());
	Commands->Start_Timer(obj, this, delay, 10);
}

void M00_Powerup_Destroy::Timer_Expired(GameObject *obj, int number)
{
	// Triggered after creation
	if (number == 10)
	{
		Commands->Expire_Powerup(obj);
	}
}

ScriptRegistrant<M00_Powerup_Destroy> M00_Powerup_DestroyRegistrant("M00_Powerup_Destroy", "");