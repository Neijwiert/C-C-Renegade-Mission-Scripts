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
#include "M03_No_More_Parachute.h"

// When timer 1/2/3 triggers in M03_Chinook_ParaDrop
void M03_No_More_Parachute::Destroyed(GameObject *obj)
{
	Commands->Create_3D_Sound_At_Bone("parachute_away", obj, "ROOTTRANSFORM");
}

ScriptRegistrant<M03_No_More_Parachute> M03_No_More_ParachuteRegistrant("M03_No_More_Parachute", "");