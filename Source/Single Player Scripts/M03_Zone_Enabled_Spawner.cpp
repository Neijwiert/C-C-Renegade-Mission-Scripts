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
#include "M03_Zone_Enabled_Spawner.h"

/*
M03 -> 2001125 1144632 1144622 2001113
M10 -> 2008172 1100155
*/
void M03_Zone_Enabled_Spawner::Entered(GameObject *obj, GameObject *enterer)
{
	int controlNum = Get_Int_Parameter("Control_num");
	GameObject *controlNumObj = Commands->Find_Object(controlNum);
	
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos) && (controlNumObj || !controlNum))
	{
		int spawnerNum = Get_Int_Parameter("Spawner_num");
		Commands->Enable_Spawner(spawnerNum, true);
	}
}

ScriptRegistrant<M03_Zone_Enabled_Spawner> M03_Zone_Enabled_SpawnerRegistrant("M03_Zone_Enabled_Spawner", "Spawner_num:int, Control_num:int");