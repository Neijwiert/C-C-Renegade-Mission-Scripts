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
#include "M01_Announce_Hand_of_Nod_Zone.h"

/*
M01 -> 100978
*/
void M01_Announce_Hand_of_Nod_Zone::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
		Commands->Attach_Script(invisObj, "Test_Cinematic", "X1A_Apache_Orca_Chase_01.txt");

		GameObject *M01BuggyNewControllerJDGObj = Commands->Find_Object(103367);
		if (M01BuggyNewControllerJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01BuggyNewControllerJDGObj, 0, 16, 0.0f);
		}

		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 103, 0.0f);

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_Announce_Hand_of_Nod_Zone> M01_Announce_Hand_of_Nod_ZoneRegistrant("M01_Announce_Hand_of_Nod_Zone", "");