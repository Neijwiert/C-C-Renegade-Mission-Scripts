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
#include "M01_KeyCard01_Script_JDG.h"

void M01_KeyCard01_Script_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		int conversationId = Commands->Create_Conversation("M01_First_Keycard_Conversation", 100, 1000.0f, true); // That's a keycard. It unlocks doors of the same color.
		Commands->Join_Conversation(NULL, conversationId, false, false, true);
		Commands->Start_Conversation(conversationId, conversationId);
	}
}

ScriptRegistrant<M01_KeyCard01_Script_JDG> M01_KeyCard01_Script_JDGRegistrant("M01_KeyCard01_Script_JDG", "");