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
#include "M01_Player_Is_Crossing_Bridge_Via_Cave_Zone.h"

/*
M01 -> 100934
*/
void M01_Player_Is_Crossing_Bridge_Via_Cave_Zone::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 154, 0.0f);

		GameObject *M01TailGunRunSpawnerControllerJDGObj = Commands->Find_Object(102346);
		if (M01TailGunRunSpawnerControllerJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01TailGunRunSpawnerControllerJDGObj, 0, 70, 0.0f);
		}

		GameObject *M01ChurchAreaSpawnerControllerJDGObj = Commands->Find_Object(102350);
		if (M01ChurchAreaSpawnerControllerJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01ChurchAreaSpawnerControllerJDGObj, 0, 68, 0.0f);
		}

		GameObject *M01PlayerIsCrossingBridgeZoneObj = Commands->Find_Object(100922);
		if (M01PlayerIsCrossingBridgeZoneObj)
		{
			Commands->Destroy_Object(M01PlayerIsCrossingBridgeZoneObj);
		}

		GameObject *M01PlayerIsCrossingBridgeViaCaveZoneObj = Commands->Find_Object(100934);
		if (M01PlayerIsCrossingBridgeViaCaveZoneObj)
		{
			Commands->Destroy_Object(M01PlayerIsCrossingBridgeViaCaveZoneObj);
		}

		GameObject *M01PlayerIsCrossingBridgeViaChurchZoneObj = Commands->Find_Object(100972);
		if (M01PlayerIsCrossingBridgeViaChurchZoneObj)
		{
			Commands->Destroy_Object(M01PlayerIsCrossingBridgeViaChurchZoneObj);
		}
	}
}

ScriptRegistrant<M01_Player_Is_Crossing_Bridge_Via_Cave_Zone> M01_Player_Is_Crossing_Bridge_Via_Cave_ZoneRegistrant("M01_Player_Is_Crossing_Bridge_Via_Cave_Zone", "");