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
#include "M01_HON_Escorts_Warroom_MCT_ZoneController_JDG.h"

/*
M01 -> 103378
*/
void M01_HON_Escorts_Warroom_MCT_ZoneController_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		GameObject *honWarRoomNodOfficerObj = Commands->Find_Object(103373);
		GameObject *honWarRoomMCTProtector1Obj = Commands->Find_Object(103374);
		GameObject *honWarRoomMCTProtector2Obj = Commands->Find_Object(103375);
		if (honWarRoomNodOfficerObj)
		{
			Commands->Send_Custom_Event(obj, honWarRoomNodOfficerObj, 0, 16, 0.0f);

			if (honWarRoomMCTProtector1Obj && honWarRoomMCTProtector2Obj)
			{
				// Shred the docs! Our extraction team isn't coming!
				// Protect the master control terminal!
				int conversationId = Commands->Create_Conversation("M01_Protect_The_MCT", 100, 1000.0f, true);
				Commands->Join_Conversation(honWarRoomMCTProtector2Obj, conversationId, false, false, true);
				Commands->Join_Conversation(honWarRoomMCTProtector1Obj, conversationId, false, false, true);
				Commands->Start_Conversation(conversationId, conversationId);
			}
		}

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_HON_Escorts_Warroom_MCT_ZoneController_JDG> M01_HON_Escorts_Warroom_MCT_ZoneController_JDGRegistrant("M01_HON_Escorts_Warroom_MCT_ZoneController_JDG", "");