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
#include "M01_Tiberium_Cave_Spawn_Helicopter_Zone_JDG.h"

/*
M01 -> 100955
*/
void M01_Tiberium_Cave_Spawn_Helicopter_Zone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *starObj = Commands->Get_A_Star(pos);

	if (enterer == starObj)
	{
		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 195, 0.0f);

		// Warning - Enemy aircraft detected.
		// Nice timing, sweetheart. 
		int conversationId = Commands->Create_Conversation("M01_Announce_Apache_Conversation", 100, 1000.0f, false);
		Commands->Join_Conversation(NULL, conversationId, false, false, true);
		Commands->Join_Conversation(starObj, conversationId, false, false, false);
		Commands->Start_Conversation(conversationId, conversationId);

		GameObject *tiberiumCaveUpThereNodGuyObj = Commands->Find_Object(103274);
		if (tiberiumCaveUpThereNodGuyObj)
		{
			Commands->Send_Custom_Event(obj, tiberiumCaveUpThereNodGuyObj, 0, 16, 0.0f);
		}

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_Tiberium_Cave_Spawn_Helicopter_Zone_JDG> M01_Tiberium_Cave_Spawn_Helicopter_Zone_JDGRegistrant("M01_Tiberium_Cave_Spawn_Helicopter_Zone_JDG", "");