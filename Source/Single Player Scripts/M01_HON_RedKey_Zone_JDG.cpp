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
#include "M01_HON_RedKey_Zone_JDG.h"

/*
M01 -> 115383
*/
void M01_HON_RedKey_Zone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *starObj = Commands->Get_A_Star(pos);

	if (enterer == starObj)
	{
		if (!Commands->Has_Key(starObj, 3))
		{
			Commands->Create_Object("Level_03_Keycard", Vector3(-168.195f, 527.489f, -31.13f));

			Commands->Destroy_Object(obj);
		}
	}
}

ScriptRegistrant<M01_HON_RedKey_Zone_JDG> M01_HON_RedKey_Zone_JDGRegistrant("M01_HON_RedKey_Zone_JDG", "");