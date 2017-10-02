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
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->starAtHONMCT, sizeof(this->starAtHONMCT), 2);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 3);
	Auto_Save_Variable(&this->field_24, sizeof(this->field_24), 4);
	Auto_Save_Variable(&this->didMCTConversation, sizeof(this->didMCTConversation), 5);
	Auto_Save_Variable(&this->gdiHONMCTConversationId, sizeof(this->gdiHONMCTConversationId), 6);
}

void M01_GiveMCTSpeech_Zone_JDG::Created(GameObject *obj)
{
	this->starAtHONMCT = false;
	this->field_1C = false;
	this->didMCTConversation = false;
	this->field_20 = 0;
	this->field_24 = 0;
}

// TODO
void M01_GiveMCTSpeech_Zone_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		if (obj)
		{
			if (param == 12)
			{
				this->field_20 = Commands->Get_ID(sender);
			}
			else if (param == 13)
			{
				this->field_24 = Commands->Get_ID(sender);
			}
			else if (param == 16)
			{
				this->field_1C = true;

				Commands->Send_Custom_Event(obj, obj, 0, 27, 0.0f);
			}
			else if (param == 27)
			{
				if (this->field_1C && this->starAtHONMCT)
				{
					GameObject *field20Obj = Commands->Find_Object(this->field_20);
					GameObject *field24Obj = Commands->Find_Object(this->field_24);

					if (field20Obj || field24Obj)
					{
						if (!this->didMCTConversation)
						{
							this->didMCTConversation = true;

							// This is the Master Control Terminal. Destroy it and we're done!
							this->gdiHONMCTConversationId = Commands->Create_Conversation("M01_GDI_HON_MCT", 100, 1000.0f, false);

							if (field20Obj)
							{
								Commands->Join_Conversation(field20Obj, this->gdiHONMCTConversationId, false, true, true);
							}
							else
							{
								Commands->Join_Conversation(field24Obj, this->gdiHONMCTConversationId, false, true, true);
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

// TODO
void M01_GiveMCTSpeech_Zone_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED && action_id == this->gdiHONMCTConversationId)
	{
		GameObject *field20Obj = Commands->Find_Object(this->field_20);
		GameObject *field24Obj = Commands->Find_Object(this->field_24);

		if (field20Obj)
		{
			Commands->Send_Custom_Event(obj, field20Obj, 0, 25, 0.0f);
		}

		if (field24Obj)
		{
			Commands->Send_Custom_Event(obj, field24Obj, 0, 25, 0.0f);
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