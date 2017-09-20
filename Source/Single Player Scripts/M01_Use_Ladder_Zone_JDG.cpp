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
#include "M01_Use_Ladder_Zone_JDG.h"

/*
M01 -> 1200204 1200203 1200202 1200201 1200200 1200199 1200198 1200197 117834 117637 116381
*/
void M01_Use_Ladder_Zone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		int conversationId = Commands->Create_Conversation("M01_Press_E_For_Ladders_Conversation", 100, 1000.0f, true); // Press 'E' to Use Ladders
		Commands->Join_Conversation(NULL, conversationId, false, false, true);
		Commands->Start_Conversation(conversationId, conversationId);

		Commands->Set_HUD_Help_Text(8271, Vector3(0.196f, 0.882f, 0.196f)); // Press 'E' to Use Ladders\n
	}
}

ScriptRegistrant<M01_Use_Ladder_Zone_JDG> M01_Use_Ladder_Zone_JDGRegistrant("M01_Use_Ladder_Zone_JDG", "");