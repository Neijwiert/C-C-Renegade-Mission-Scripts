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
#include "M01_ChurchArea_Spawner_Guy_JDG.h"

/*
102347
*/
void M01_ChurchArea_Spawner_Guy_JDG::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *M01ChurchAreaSpawnerControllerJDGObj = Commands->Find_Object(102350);
	if (M01ChurchAreaSpawnerControllerJDGObj)
	{
		Commands->Debug_Message("***************************church area spawner has been killed--sending custom\n");

		Commands->Send_Custom_Event(obj, M01ChurchAreaSpawnerControllerJDGObj, 0, 69, 0.0f);
	}
}

ScriptRegistrant<M01_ChurchArea_Spawner_Guy_JDG> M01_ChurchArea_Spawner_Guy_JDGRegistrant("M01_ChurchArea_Spawner_Guy_JDG", "");