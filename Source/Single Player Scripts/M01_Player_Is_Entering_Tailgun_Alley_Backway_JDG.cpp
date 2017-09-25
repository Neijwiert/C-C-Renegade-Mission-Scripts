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
#include "M01_Player_Is_Entering_Tailgun_Alley_Backway_JDG.h"

/*
M01 -> 101718
*/
void M01_Player_Is_Entering_Tailgun_Alley_Backway_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		if (M01MissionControllerJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 134, 0.0f);
		}

		GameObject *tailGun1Obj = Commands->Find_Object(108707);
		if (tailGun1Obj)
		{
			Commands->Send_Custom_Event(obj, tailGun1Obj, 0, 16, 0.0f);
		}

		GameObject *tailGun2Obj = Commands->Find_Object(108708);
		if (tailGun2Obj)
		{
			Commands->Send_Custom_Event(obj, tailGun2Obj, 0, 16, 0.0f);
		}

		GameObject *tailGun3Obj = Commands->Find_Object(108709);
		if (tailGun3Obj)
		{
			Commands->Send_Custom_Event(obj, tailGun3Obj, 0, 16, 0.0f);
		}

		GameObject *M01GDIBaseSpawnerControllerJDGObj = Commands->Find_Object(102335);
		if (M01GDIBaseSpawnerControllerJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01GDIBaseSpawnerControllerJDGObj, 0, 70, 0.0f);
		}

		GameObject *M01TailGunRunSpawnerControllerJDGObj = Commands->Find_Object(102346);
		if (M01TailGunRunSpawnerControllerJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01TailGunRunSpawnerControllerJDGObj, 0, 68, 0.0f);
		}

		GameObject *M01PlayerIsEnteringTailGunAlleyJDGObj = Commands->Find_Object(101692);
		if (M01PlayerIsEnteringTailGunAlleyJDGObj)
		{
			Commands->Destroy_Object(M01PlayerIsEnteringTailGunAlleyJDGObj);
		}

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_Player_Is_Entering_Tailgun_Alley_Backway_JDG> M01_Player_Is_Entering_Tailgun_Alley_Backway_JDGRegistrant("M01_Player_Is_Entering_Tailgun_Alley_Backway_JDG", "");