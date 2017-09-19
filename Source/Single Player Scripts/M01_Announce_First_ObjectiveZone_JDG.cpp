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
#include "M01_Announce_First_ObjectiveZone_JDG.h"

// On level start
void M01_Announce_First_ObjectiveZone_JDG::Created(GameObject *obj)
{
	int conversationId = Commands->Create_Conversation("M01_Press_F1_Conversation", 100, 1000.0f, true); // Press F1 for the Help Screen
	Commands->Join_Conversation(NULL, conversationId, false, false, true);
	Commands->Start_Conversation(conversationId, conversationId);

	Commands->Set_HUD_Help_Text(8272, Vector3(0.196f, 0.882f, 0.196f)); // Press F1 for the Help Screen\n

	GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
	Commands->Attach_Script(invisObj, "Test_Cinematic", "X1H_Hover_Troop.txt");
}

void M01_Announce_First_ObjectiveZone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		GameObject *M01ObjectivePogControllerJDGObj = Commands->Find_Object(105828);
		if (M01ObjectivePogControllerJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01ObjectivePogControllerJDGObj, 0, 102, 0.0f);
		}

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_Announce_First_ObjectiveZone_JDG> M01_Announce_First_ObjectiveZone_JDGRegistrant("M01_Announce_First_ObjectiveZone_JDG", "");