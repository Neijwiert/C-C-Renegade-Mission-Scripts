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
#include "M01_GiveMCTSpeech_Zone_JDG.h"

/*
M01 -> 124260
*/
void M01_GiveMCTSpeech_Zone_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->chinookGDISpawnedSoldiersInHonWarRoom, sizeof(this->chinookGDISpawnedSoldiersInHonWarRoom), 1);
	Auto_Save_Variable(&this->starAtHONMCT, sizeof(this->starAtHONMCT), 2);
	Auto_Save_Variable(&this->honChinookSpawnedSoldier1GDIObjId, sizeof(this->honChinookSpawnedSoldier1GDIObjId), 3);
	Auto_Save_Variable(&this->honChinookSpawnedSoldier2GDIObjId, sizeof(this->honChinookSpawnedSoldier2GDIObjId), 4);
	Auto_Save_Variable(&this->didMCTConversation, sizeof(this->didMCTConversation), 5);
	Auto_Save_Variable(&this->gdiHONMCTConversationId, sizeof(this->gdiHONMCTConversationId), 6);
}

void M01_GiveMCTSpeech_Zone_JDG::Created(GameObject *obj)
{
	this->starAtHONMCT = false;
	this->chinookGDISpawnedSoldiersInHonWarRoom = false;
	this->didMCTConversation = false;
	this->honChinookSpawnedSoldier1GDIObjId = 0;
	this->honChinookSpawnedSoldier2GDIObjId = 0;
}

void M01_GiveMCTSpeech_Zone_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		if (obj)
		{
			// Received from M01_HON_Chinook_Spawned_Soldier_01_GDI_JDG when created
			if (param == 12)
			{
				this->honChinookSpawnedSoldier1GDIObjId = Commands->Get_ID(sender);
			}

			// Received from M01_HON_Chinook_Spawned_Soldier_02_GDI_JDG when created
			else if (param == 13)
			{
				this->honChinookSpawnedSoldier2GDIObjId = Commands->Get_ID(sender);
			}

			// Received from M01_HON_Chinook_Spawned_Soldier_01_GDI_JDG or M01_HON_Chinook_Spawned_Soldier_02_GDI_JDG when action with id 39 is complete
			else if (param == 16)
			{
				this->chinookGDISpawnedSoldiersInHonWarRoom = true;

				Commands->Send_Custom_Event(obj, obj, 0, 27, 0.0f);
			}

			// Received from ourselves when param 16 is received or entered
			else if (param == 27)
			{
				if (this->chinookGDISpawnedSoldiersInHonWarRoom && this->starAtHONMCT)
				{
					GameObject *honChinookSpawnedSoldier1GDIObj = Commands->Find_Object(this->honChinookSpawnedSoldier1GDIObjId);
					GameObject *honChinookSpawnedSoldier2GDIObj = Commands->Find_Object(this->honChinookSpawnedSoldier2GDIObjId);

					if (honChinookSpawnedSoldier1GDIObj || honChinookSpawnedSoldier2GDIObj)
					{
						if (!this->didMCTConversation)
						{
							this->didMCTConversation = true;

							// This is the Master Control Terminal. Destroy it and we're done!
							this->gdiHONMCTConversationId = Commands->Create_Conversation("M01_GDI_HON_MCT", 100, 1000.0f, false);

							if (honChinookSpawnedSoldier1GDIObj)
							{
								Commands->Join_Conversation(honChinookSpawnedSoldier1GDIObj, this->gdiHONMCTConversationId, false, true, true);
							}
							else
							{
								Commands->Join_Conversation(honChinookSpawnedSoldier2GDIObj, this->gdiHONMCTConversationId, false, true, true);
							}

							Vector3 pos = Commands->Get_Position(obj);
							GameObject *starObj = Commands->Get_A_Star(pos);

							Commands->Join_Conversation(starObj, this->gdiHONMCTConversationId, false, false, false);
							Commands->Start_Conversation(this->gdiHONMCTConversationId, this->gdiHONMCTConversationId);
							Commands->Monitor_Conversation(obj, this->gdiHONMCTConversationId);
						}
					}
				}
			}
		}
	}
}

void M01_GiveMCTSpeech_Zone_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED && action_id == this->gdiHONMCTConversationId)
	{
		GameObject *honChinookSpawnedSoldier1GDIObj = Commands->Find_Object(this->honChinookSpawnedSoldier1GDIObjId);
		GameObject *honChinookSpawnedSoldier2GDIObj = Commands->Find_Object(this->honChinookSpawnedSoldier2GDIObjId);

		if (honChinookSpawnedSoldier1GDIObj)
		{
			Commands->Send_Custom_Event(obj, honChinookSpawnedSoldier1GDIObj, 0, 25, 0.0f);
		}

		if (honChinookSpawnedSoldier2GDIObj)
		{
			Commands->Send_Custom_Event(obj, honChinookSpawnedSoldier2GDIObj, 0, 25, 0.0f);
		}

		Commands->Destroy_Object(obj);
	}
}

void M01_GiveMCTSpeech_Zone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		this->starAtHONMCT = true;

		Commands->Send_Custom_Event(obj, obj, 0, 27, 0.0f);
	}
}

void M01_GiveMCTSpeech_Zone_JDG::Exited(GameObject *obj, GameObject *exiter)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (exiter == Commands->Get_A_Star(pos))
	{
		this->starAtHONMCT = false;
	}
}

ScriptRegistrant<M01_GiveMCTSpeech_Zone_JDG> M01_GiveMCTSpeech_Zone_JDGRegistrant("M01_GiveMCTSpeech_Zone_JDG", "");