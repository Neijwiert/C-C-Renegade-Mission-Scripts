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
#include "M01_PlayerLeaving_BarnArea_Zone_JDG.h"

/*
M01 -> 113323 113322 101466
*/
void M01_PlayerLeaving_BarnArea_Zone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		GameObject *M01GunboatActionControllerJDGObj = Commands->Find_Object(113325);
		if (M01GunboatActionControllerJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01GunboatActionControllerJDGObj, 0, 24, 0.0f);
		}
	}
}

ScriptRegistrant<M01_PlayerLeaving_BarnArea_Zone_JDG> M01_PlayerLeaving_BarnArea_Zone_JDGRegistrant("M01_PlayerLeaving_BarnArea_Zone_JDG", "");