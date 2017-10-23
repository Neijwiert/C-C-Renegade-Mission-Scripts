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
#include "M03_Officer_With_Key_Card.h"

/*
M03 -> 1144682
*/
void M03_Officer_With_Key_Card::Killed(GameObject *obj, GameObject *killer)
{
	Vector3 pos = Commands->Get_Position(obj);

	Vector3 keyCardPos = pos;
	keyCardPos.Z += 0.5f;

	GameObject *keyCardObj = Commands->Create_Object("Level_01_Keycard", keyCardPos);
	Commands->Attach_Script(keyCardObj, "M03_Key_Card", "");

	// Looks like a security card. Get to the mainframe in the Communications Center.
	// Lock and load!
	int conversationId = Commands->Create_Conversation("M03CON005", 99, 2000.0f, true);
	Commands->Join_Conversation(NULL, conversationId, true, true, true );

	GameObject *starObj = Commands->Get_A_Star(pos);

	Commands->Join_Conversation(starObj, conversationId, true, true, true);
	Commands->Start_Conversation(conversationId, 100005);
	Commands->Monitor_Conversation(obj, conversationId);
}

ScriptRegistrant<M03_Officer_With_Key_Card> M03_Officer_With_Key_CardRegistrant("M03_Officer_With_Key_Card", "");