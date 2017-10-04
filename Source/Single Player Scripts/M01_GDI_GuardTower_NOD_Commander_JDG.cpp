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
#include "M01_GDI_GuardTower_NOD_Commander_JDG.h"

/*
M01 -> 102360
*/
void M01_GDI_GuardTower_NOD_Commander_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->conversationId, sizeof(this->conversationId), 1);
	Auto_Save_Variable(&this->killed, sizeof(this->killed), 2);
	Auto_Save_Variable(&this->doingConversation, sizeof(this->doingConversation), 3);
	Auto_Save_Variable(&this->notDamaged, sizeof(this->notDamaged), 4);
	Auto_Save_Variable(&this->enemySeen, sizeof(this->enemySeen), 5);
}

void M01_GDI_GuardTower_NOD_Commander_JDG::Created(GameObject *obj)
{
	this->doingConversation = false;
	this->notDamaged = true;
	this->enemySeen = false;
	this->killed = false;

	Commands->Set_Innate_Is_Stationary(obj, true);
}

void M01_GDI_GuardTower_NOD_Commander_JDG::Killed(GameObject *obj, GameObject *killer)
{
	if (this->doingConversation)
	{
		Commands->Stop_Conversation(this->conversationId);
	}

	GameObject *nonExistingObj = Commands->Find_Object(102361);
	GameObject *M01BarnAreaNODCommanderTriggerZone02JDGObj = Commands->Find_Object(103343);
	if (nonExistingObj)
	{
		Commands->Destroy_Object(nonExistingObj);
	}

	if (M01BarnAreaNODCommanderTriggerZone02JDGObj)
	{
		Commands->Destroy_Object(M01BarnAreaNODCommanderTriggerZone02JDGObj);
	}

	this->killed = true;

	GameObject *M01GDIBaseArtilleryControllerJDGObj = Commands->Find_Object(102294);
	if (M01GDIBaseArtilleryControllerJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01GDIBaseArtilleryControllerJDGObj, 0, 24, 0.0f);
	}

	GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
	if (M01MissionControllerJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 101, 5.0f);
	}

	Vector3 pos = Commands->Get_Position(obj);
	if (Commands->Get_A_Star(pos))
	{
		// Havoc, that tower offers a perfect vantage point for your sniper rifle.
		// Use it to your advantage.
		int conversationId = Commands->Create_Conversation("M01_GuardTower_Outro_Conversation", 100, 1000.0f, false);
		Commands->Join_Conversation(NULL, conversationId, false, false, true);
		Commands->Start_Conversation(conversationId, conversationId);
	}

	pos.Z += 0.75;

	static const char *drops[2] =
	{
		"POW_Health_100",
		"POW_Armor_100"
	};

	int randDropIndex = Commands->Get_Random_Int(0, 2);
	Commands->Create_Object(drops[randDropIndex], pos);
}

void M01_GDI_GuardTower_NOD_Commander_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (obj)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		if (damager == starObj && !this->killed && this->notDamaged)
		{
			Vector3 starPos = Commands->Get_Position(starObj);

			if (Commands->Get_Distance(pos, starPos) >= 15.0f)
			{
				static const char *animations[5] = 
				{
					"H_A_A0A0_L20",
					"H_A_A0A0_L21",
					"H_A_A0A0_L36",
					"H_A_A0A0_L52",
					"H_A_J21C"
				};

				int randAnimationIndex = Commands->Get_Random_Int(0, 5);

				this->notDamaged = false;

				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 47);
				params.Set_Animation(animations[randAnimationIndex], false);

				Commands->Action_Play_Animation(obj, params);
			}
		}
	}
}

// TODO
void M01_GDI_GuardTower_NOD_Commander_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		if (param == 27)
		{
			if (obj)
			{
				int difficultyLevel = Commands->Get_Difficulty_Level();

				float delay;
				if (difficultyLevel == 2)
				{
					delay = 60.0f;
				}
				else if (difficultyLevel == 1)
				{
					delay = 90.0f;
				}
				else
				{
					delay = 120.0f;
				}

				Commands->Send_Custom_Event(obj, obj, 0, 71, delay);
				Commands->Send_Custom_Event(obj, obj, 0, 27, delay);
			}
		}
		else if (param == 66)
		{
			if (obj && !this->killed)
			{
				GameObject *M01NodCommanderConversationControllerGDIObj = Commands->Find_Object(103398);
				if (M01NodCommanderConversationControllerGDIObj)
				{
					Commands->Debug_Message("**********************Nod gdi tower commander is sending custom to dialog controller\n");

					Commands->Send_Custom_Event(obj, M01NodCommanderConversationControllerGDIObj, 0, 27, 0.0f);
				}

				float randDelay = Commands->Get_Random(15.0f, 30.0f);
				Commands->Send_Custom_Event(obj, obj, 0, 66, randDelay);
			}
		}
		else if (param == 71)
		{
			if (obj)
			{
				GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
				Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 71, 0.0f);
			}
		}
		else if (param == 16)
		{
			GameObject *nonExistingObj = Commands->Find_Object(102361);
			GameObject *M01BarnAreaNODCommanderTriggerZone02JDGObj = Commands->Find_Object(103343);

			if (sender == nonExistingObj)
			{
				Commands->Debug_Message("**********************Nod gdi tower commander has recieved command to start acting\n");

				Commands->Send_Custom_Event(obj, obj, 0, 27, 2.0f);
				Commands->Send_Custom_Event(obj, obj, 0, 66, 2.0f);
			}
			else if (sender == M01BarnAreaNODCommanderTriggerZone02JDGObj)
			{
				Commands->Debug_Message("**********************Nod gdi tower commander has recieved command to start acting\n");

				Commands->Send_Custom_Event(obj, obj, 0, 27, 20.0f);
				Commands->Send_Custom_Event(obj, obj, 0, 66, 20.0f);
			}

			Vector3 pos = Commands->Get_Position(obj);
			if (Commands->Get_A_Star(pos) && !this->killed)
			{
				this->doingConversation = true;

				this->conversationId = Commands->Create_Conversation("M01_GuardTower_Intro_Conversation", 95, 1000.0f, true); // Havoc, there's a Nod officer directing reinforcements from a guard tower up ahead. Take him out.
				Commands->Join_Conversation(NULL, this->conversationId, false, false, true);
				Commands->Start_Conversation(this->conversationId, this->conversationId);
				Commands->Monitor_Conversation(obj, this->conversationId);
			}
		}
	}
}

// TODO
void M01_GDI_GuardTower_NOD_Commander_JDG::Sound_Heard(GameObject *obj, const CombatSound & sound)
{
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *starObj = Commands->Get_A_Star(pos);

	if (sound.Creator == starObj && sound.sound == SOUND_TYPE_BULLET_HIT && obj && !this->killed && !this->enemySeen)
	{
		Vector3 starPos = Commands->Get_Position(starObj);

		if (Commands->Get_Distance(pos, starPos) >= 15.0f)
		{
			static const char *animations[5] =
			{
				"H_A_A0A0_L20",
				"H_A_A0A0_L21",
				"H_A_A0A0_L36",
				"H_A_A0A0_L52",
				"H_A_J21C"
			};

			int randAnimationIndex = Commands->Get_Random_Int(0, 5);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 47);
			params.Set_Animation(animations[randAnimationIndex], false);

			Commands->Action_Play_Animation(obj, params);
		}
	}
}

void M01_GDI_GuardTower_NOD_Commander_JDG::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (Commands->Get_A_Star(pos))
	{
		this->enemySeen = true;
	}
}

// TODO
void M01_GDI_GuardTower_NOD_Commander_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED && action_id == this->conversationId)
	{
		this->doingConversation = false;
	}
}

ScriptRegistrant<M01_GDI_GuardTower_NOD_Commander_JDG> M01_GDI_GuardTower_NOD_Commander_JDGRegistrant("M01_GDI_GuardTower_NOD_Commander_JDG", "");