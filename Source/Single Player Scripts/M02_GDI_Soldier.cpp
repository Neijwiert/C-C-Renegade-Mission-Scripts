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
#include "M02_GDI_Soldier.h"

void M02_GDI_Soldier::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->isMyAreaActivated, sizeof(this->isMyAreaActivated), 1);
	Auto_Save_Variable(&this->didHaveConversation, sizeof(this->didHaveConversation), 2);
	Auto_Save_Variable(&this->backupConversationIndex, sizeof(this->backupConversationIndex), 3);
}

// On level start
// The following cineamtic files: x2i_gdi_drop02_rocket.txt, x2i_gdi_drop02_rocket_2.txt, x2i_gdi_drop02_rocket_24.txt and x2i_gdi_drop03_minigunner.txt
void M02_GDI_Soldier::Created(GameObject *obj)
{
	Commands->Enable_Hibernation(obj, false);

	this->isMyAreaActivated = false;
	this->didHaveConversation = false;
	this->backupConversationIndex = 0;

	if (Get_Int_Parameter("Area_ID") == 1)
	{
		Commands->Innate_Disable(obj);

		if (Get_Int_Parameter("Soldier_Type") == 2)
		{
			Commands->Start_Timer(obj, this, 10.0f, 2);
		}
	}
	else
	{
		Commands->Start_Timer(obj, this, 1.0f, 1);
	}
}

void M02_GDI_Soldier::Killed(GameObject *obj, GameObject *killer)
{
	int areaId = Get_Int_Parameter("Area_ID");
	if (!areaId)
	{
		GameObject *M02ObjectiveControllerObj = Commands->Find_Object(1111112);
		if (M02ObjectiveControllerObj)
		{
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 1000, 1000, 0.0f);
		}
	}
	else if (areaId == 9)
	{
		GameObject *M02ObjectiveControllerObj = Commands->Find_Object(1111112);
		if (M02ObjectiveControllerObj)
		{
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 1000, 1001, 0.0f);
		}
	}
}

void M02_GDI_Soldier::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M02_Objective_Zone with objId = 400185
	if (!type && !param)
	{
		Commands->Innate_Enable(obj);
	}
}

void M02_GDI_Soldier::Sound_Heard(GameObject *obj, const CombatSound & sound)
{
	// Received from M02_Objective_Zone
	if (sound.sound > 1099 && sound.sound < 1199)
	{
		int areaId = Get_Int_Parameter("Area_ID");
		int soundAreaId = sound.sound - 1100;

		if (soundAreaId == areaId)
		{
			this->isMyAreaActivated = true;

			if (soundAreaId == 2)
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 81.0f, 0);
				params.Set_Movement(Vector3(648.77002f, 300.73999f, -59.990002f), 0.3f, 1.0f);

				Commands->Action_Goto(obj, params);
			}
			else if(!soundAreaId)
			{
				Commands->Action_Reset(obj, 100.0f);
			}
		}
	}
}

void M02_GDI_Soldier::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggerd when we moved to the star, see 
	if (action_id == 1 && complete_reason == ACTION_COMPLETE_NORMAL && !this->didHaveConversation)
	{
		this->didHaveConversation = true;

		int soldierType = Get_Int_Parameter("Soldier_Type");
		const char *conversationName;
		switch (soldierType)
		{
			case 1:
				conversationName = "M02_GERS_COVER_01"; // I've got you covered, sir.

				break;
			case 4:
				conversationName = "M02_GEGR_COVER_01";  // I'm  right behind you sir.

				break;
			default:
				conversationName = NULL;

				break;
		}

		int conversationId = Commands->Create_Conversation(conversationName, 99, 50.0f, true);
		Commands->Join_Conversation(obj, conversationId, true, true, false);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, true, false);
		Commands->Start_Conversation(conversationId, 0);
	}
}

void M02_GDI_Soldier::Timer_Expired(GameObject *obj, int number)
{
	// Triggered after 1 second on creation or 10 seconds after this block
	if (number == 1)
	{
		if (Get_Int_Parameter("Area_ID") == 9)
		{
			GameObject *nodObeliskBuildingObj = Commands->Find_Object(1153845);
			if (nodObeliskBuildingObj)
			{
				if (Commands->Get_Building_Power(nodObeliskBuildingObj))
				{
					ActionParamsStruct params;
					params.Set_Basic(this, 71.0f, 0);
					params.Set_Movement(Vector3(1105.78f, 897.75f, 35.05f), 1.0f, 20.0f);

					Commands->Action_Goto(obj, params);
				}
			}
		}
		else 
		{
			if (!this->isMyAreaActivated)
			{
				Vector3 pos = Commands->Get_Position(obj);
				GameObject *starObj = Commands->Get_A_Star(pos);

				if (starObj)
				{
					ActionParamsStruct params;
					params.Set_Basic(this, 71.0f, 1);
					params.Set_Movement(starObj, 1.0f, 20.0f);

					Commands->Action_Goto(obj, params);
				}
				else
				{
					this->isMyAreaActivated = true;
				}
			}
		}

		Commands->Start_Timer(obj, this, 10.0f, 1);
	}

	// Triggered 10 seconds after creation or 10 seconds after this block
	else if (number == 2)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);
		Vector3 starPos = Commands->Get_Position(starObj);

		if (Commands->Get_Distance(pos, starPos) < 30.0f)
		{
			const char *conversationName = NULL;
			if (!this->backupConversationIndex)
			{
				conversationName = "M02_GOMG_RADIO_01"; // Attention all GDI forces! Requesting backup at my position immediately!
			}
			else if (this->backupConversationIndex == 1)
			{
				conversationName = "M02_GOMG_RADIO_02"; // Where's my backup! I need  armor out here, Nod's got us pinned!
			}
			else if (this->backupConversationIndex == 2)
			{
				conversationName = "M02_GOMG_RADIO_03"; // Any remaining GDI forces converge on my location!
			}

			int conversationId = Commands->Create_Conversation(conversationName, 99, 20.0f, true);
			Commands->Join_Conversation(obj, conversationId, true, true, true);
			Commands->Start_Conversation(conversationId, 0);

			this->backupConversationIndex++;
		}

		Commands->Start_Timer(obj, this, 10.0f, 2);
	}
}

void M02_GDI_Soldier::Poked(GameObject *obj, GameObject *poker)
{
	if (Get_Int_Parameter("Soldier_Type") == 2 && !this->didHaveConversation)
	{
		this->didHaveConversation = true;

		int conversationId = Commands->Create_Conversation("M02_GOMG_CONVERSATION", 99, 10.0f, true); // Good to have you with us. We've got hostiles across the river, and not enough resistance to go around. Take whatever supplies you need, anything you can do to help is appreciated.
		
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);
	
		Commands->Join_Conversation(starObj, conversationId, true, true, false);
		Commands->Start_Conversation(conversationId, 0);
	}
}

ScriptRegistrant<M02_GDI_Soldier> M02_GDI_SoldierRegistrant("M02_GDI_Soldier", "Area_ID:int, Soldier_Type=0:int");