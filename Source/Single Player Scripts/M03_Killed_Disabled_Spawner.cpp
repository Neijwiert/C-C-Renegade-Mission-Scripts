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
#include "M03_Killed_Disabled_Spawner.h"

/*
M03 -> 1146700 300009 300010
*/
void M03_Killed_Disabled_Spawner::Killed(GameObject *obj, GameObject *killer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (killer == Commands->Get_A_Star(pos)) // Spawner never gets disabled if killed by a bot?
	{
		int spawnerNum = Get_Int_Parameter("Spawner_num");
		Commands->Enable_Spawner(spawnerNum, false);
	}
}

ScriptRegistrant<M03_Killed_Disabled_Spawner> M03_Killed_Disabled_SpawnerRegistrant("M03_Killed_Disabled_Spawner", "Spawner_num:int");