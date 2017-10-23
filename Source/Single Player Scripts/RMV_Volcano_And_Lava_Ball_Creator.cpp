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
#include "RMV_Volcano_And_Lava_Ball_Creator.h"

/*
M03 -> 1001001
*/
void RMV_Volcano_And_Lava_Ball_Creator::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->lastLavaBallIndex, sizeof(this->lastLavaBallIndex), 1);
}

void RMV_Volcano_And_Lava_Ball_Creator::Created(GameObject *obj)
{
	this->lastLavaBallIndex = -1;

	Commands->Enable_Cinematic_Freeze(obj, false);
}

void RMV_Volcano_And_Lava_Ball_Creator::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received after 356 frames in x3d_sakuracrash.txt
	if (type == 500 && param == 500)
	{
		Commands->Reveal_Encyclopedia_Character(39); // Nod_Minigunner_3Boss
		Commands->Reveal_Encyclopedia_Vehicle(31); // Nod_Comanche

		Commands->Enable_Cinematic_Freeze(obj, false);

		int conversationId = Commands->Create_Conversation("M03CON010", 99, 2000.0f, true); // Havoc - I'm picking up a submarine adjacent to the dockyards beyond this base facility. Find a way to get on board, get off that island before it blows!
		Commands->Join_Conversation(NULL, conversationId, true, true, true);
		Commands->Start_Conversation(conversationId, 100010);
		Commands->Monitor_Conversation(obj, conversationId);

		Commands->Set_Ash(0.15000001f, 3.0f, false);

		GameObject *M03MissionControllerObj = Commands->Find_Object(1100004);
		Commands->Send_Custom_Event(obj, M03MissionControllerObj, 40001, 0, 0.0f);

		Commands->Set_Clouds(1.0f, 1.0f, 20.0f);

		Commands->Start_Timer(obj, this, 55.0f, 40005);
		Commands->Start_Timer(obj, this, 10.0f, 40007);
		Commands->Start_Timer(obj, this, 0.1f, 1001);

		GameObject *M03EvacuationControllerObj = Commands->Find_Object(1144977);
		if (M03EvacuationControllerObj)
		{
			Commands->Send_Custom_Event(obj, M03EvacuationControllerObj, 100, 100, 0.0f);
		}

		Commands->Create_Object("Volcano Controller", Vector3(0.0f, 0.0f, 0.0f));
	}
}

void RMV_Volcano_And_Lava_Ball_Creator::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered when done with conversation, see custom
	if (action_id == 100010)
	{
		GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);
		Commands->Send_Custom_Event(obj, M03ObjectiveControllerObj, 310, 3, 0.0f);
	}
}

void RMV_Volcano_And_Lava_Ball_Creator::Timer_Expired(GameObject *obj, int number)
{
	// Triggered after 0.1 seconds or 5 to 7 seconds, see action complete or this block
	if (number == 1001)
	{
		int rand = static_cast<int>(Commands->Get_Random(0.0f, 1.0f) * 20.0f);
		if (rand == this->lastLavaBallIndex)
		{
			rand = (rand + 1) % 20;
		}

		if (rand > 19)
		{
			rand = 19;
		}

		switch (rand)
		{
			case 0:
				Commands->Create_Object("LavaBall01", Vector3(0.0f, 0.0f, 0.0f));

				break;
			case 1:
				Commands->Create_Object("LavaBall02", Vector3(0.0f, 0.0f, 0.0f));

				break;
			case 2:
				Commands->Create_Object("LavaBall03", Vector3(0.0f, 0.0f, 0.0f));

				break;
			case 3:
				Commands->Create_Object("LavaBall04", Vector3(0.0f, 0.0f, 0.0f));

				break;
			case 4:
				Commands->Create_Object("LavaBall05", Vector3(0.0f, 0.0f, 0.0f));

				break;
			case 5:
				Commands->Create_Object("LavaBall06", Vector3(0.0f, 0.0f, 0.0f));

				break;
			case 6:
				Commands->Create_Object("LavaBall07", Vector3(0.0f, 0.0f, 0.0f));

				break;
			case 7:
				Commands->Create_Object("LavaBall08", Vector3(0.0f, 0.0f, 0.0f));

				break;
			case 8:
				Commands->Create_Object("LavaBall09", Vector3(0.0f, 0.0f, 0.0f));

				break;
			case 9:
				Commands->Create_Object("LavaBall10", Vector3(0.0f, 0.0f, 0.0f));

				break;
			case 10:
				Commands->Create_Object("LavaBall11", Vector3(0.0f, 0.0f, 0.0f));

				break;
			case 11:
				Commands->Create_Object("LavaBall12", Vector3(0.0f, 0.0f, 0.0f));

				break;
			case 12:
				Commands->Create_Object("LavaBall13", Vector3(0.0f, 0.0f, 0.0f));

				break;
			case 13:
				Commands->Create_Object("LavaBall14", Vector3(0.0f, 0.0f, 0.0f));

				break;
			case 14:
				Commands->Create_Object("LavaBall15", Vector3(0.0f, 0.0f, 0.0f));

				break;
			case 15:
				Commands->Create_Object("LavaBall16", Vector3(0.0f, 0.0f, 0.0f));

				break;
			case 16:
				Commands->Create_Object("LavaBall17", Vector3(0.0f, 0.0f, 0.0f));

				break;
			case 17:
				Commands->Create_Object("LavaBall18", Vector3(0.0f, 0.0f, 0.0f));

				break;
			case 18:
				Commands->Create_Object("LavaBall19", Vector3(0.0f, 0.0f, 0.0f));

				break;
			case 19:
				Commands->Create_Object("LavaBall20", Vector3(0.0f, 0.0f, 0.0f));

				break;
		}

		this->lastLavaBallIndex = rand;

		float randInterval = Commands->Get_Random(3.0f, 5.0f) + 2.0f;
		Commands->Start_Timer(obj, this, randInterval, 1001);
	}
}

ScriptRegistrant<RMV_Volcano_And_Lava_Ball_Creator> RMV_Volcano_And_Lava_Ball_CreatorRegistrant("RMV_Volcano_And_Lava_Ball_Creator", "");