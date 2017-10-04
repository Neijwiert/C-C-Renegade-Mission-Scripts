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
#include "M01_ConYard_Dropoff_Dude_JDG.h"

// This script is never used
void M01_ConYard_Dropoff_Dude_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "S_A_Human.H_A_TroopDrop"))
	{
		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_ConYard_Dropoff_Dude_JDG> M01_ConYard_Dropoff_Dude_JDGRegistrant("M01_ConYard_Dropoff_Dude_JDG", "");