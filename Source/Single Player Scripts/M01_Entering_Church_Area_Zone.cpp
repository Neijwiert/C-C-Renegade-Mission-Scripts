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
#include "M01_Entering_Church_Area_Zone.h"

/*
M01 -> 100801 100802 100803
*/
void M01_Entering_Church_Area_Zone::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 148, 0.0f);

		GameObject *M01EnteringChurchAreaZone1Obj = Commands->Find_Object(100801);
		if (M01EnteringChurchAreaZone1Obj)
		{
			Commands->Destroy_Object(M01EnteringChurchAreaZone1Obj);
		}

		GameObject *M01EnteringChurchAreaZone2Obj = Commands->Find_Object(100802);
		if (M01EnteringChurchAreaZone2Obj)
		{
			Commands->Destroy_Object(M01EnteringChurchAreaZone2Obj);
		}

		GameObject *M01EnteringChurchAreaZone3Obj = Commands->Find_Object(100803);
		if (M01EnteringChurchAreaZone3Obj)
		{
			Commands->Destroy_Object(M01EnteringChurchAreaZone3Obj);
		}
	}
}

ScriptRegistrant<M01_Entering_Church_Area_Zone> M01_Entering_Church_Area_ZoneRegistrant("M01_Entering_Church_Area_Zone", "");