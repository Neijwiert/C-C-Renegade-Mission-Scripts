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
#include "M01_Player_Is_Entering_Tailgun_Alley_JDG.h"

/*
M01 -> 101692
*/
void M01_Player_Is_Entering_Tailgun_Alley_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		// Warning - Nod Light Tank detected.
		// She's right, Nick. I'm also detecting Machine gun emplacements.
		// You'll need explosives or heavy firepower to take out those gun emplacements.
		int conversationId = Commands->Create_Conversation("M01_Announce_LightTank_Conversation", 100, 1000.0f, false);
		Commands->Join_Conversation(NULL, conversationId, false, false, true);
		Commands->Join_Conversation(NULL, conversationId, false, false, true);
		Commands->Start_Conversation(conversationId, conversationId);

		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		if (Commands->Find_Object(100376))
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

		GameObject *M01PlayerIsEnteringTailGunAlleyBackwayJDGObj = Commands->Find_Object(101718);
		if (M01PlayerIsEnteringTailGunAlleyBackwayJDGObj)
		{
			Commands->Destroy_Object(M01PlayerIsEnteringTailGunAlleyBackwayJDGObj);
		}

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_Player_Is_Entering_Tailgun_Alley_JDG> M01_Player_Is_Entering_Tailgun_Alley_JDGRegistrant("M01_Player_Is_Entering_Tailgun_Alley_JDG", "");