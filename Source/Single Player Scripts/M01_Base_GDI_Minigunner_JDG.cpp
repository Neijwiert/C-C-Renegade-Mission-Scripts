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
#include "M01_Base_GDI_Minigunner_JDG.h"

void M01_Base_GDI_Minigunner_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->itsHavocConversationId, sizeof(this->itsHavocConversationId), 1);
	Auto_Save_Variable(&this->baseDestroyedConversationId, sizeof(this->baseDestroyedConversationId), 2);
	Auto_Save_Variable(&this->health, sizeof(this->health), 3);
	Auto_Save_Variable(&this->preventDeath, sizeof(this->preventDeath), 4);
	Auto_Save_Variable(&this->pathConversationId, sizeof(this->pathConversationId), 5);
	Auto_Save_Variable(&this->field_2D, sizeof(this->field_2D), 6);
	Auto_Save_Variable(&this->field_2E, sizeof(this->field_2E), 7);
	Auto_Save_Variable(&this->field_2F, sizeof(this->field_2F), 8);
	Auto_Save_Variable(&this->attackedByNodMinigunners, sizeof(this->attackedByNodMinigunners), 9);
	Auto_Save_Variable(&this->field_31, sizeof(this->field_31), 10);
}

// On level start
void M01_Base_GDI_Minigunner_JDG::Created(GameObject *obj)
{
	this->field_31 = false;
	this->attackedByNodMinigunners = false;
	this->preventDeath = true;
	this->field_2D = false;
	this->field_2F = true;
	this->field_2E = false;
	this->health = Commands->Get_Health(obj);

	GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
	Commands->Attach_Script(invisObj, "Test_Cinematic", "X1C_Intro.txt");

	Commands->Set_Innate_Is_Stationary(obj, true);

	ActionParamsStruct params;
	params.Set_Basic(this, 100.0f, 16);
	params.Set_Attack(Vector3(72.223f, -24.750999f, -0.296f), 1000.0f, 0.0f, true);
	params.AttackCheckBlocked = false;

	Commands->Action_Attack(obj, params);

	Commands->Send_Custom_Event(obj, obj, 0, 27, 2.0f);
}

void M01_Base_GDI_Minigunner_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	GameObject *gdiGrenadierObj = Commands->Find_Object(116383);
	GameObject *nodMinigunner1Obj = Commands->Find_Object(116388);
	GameObject *nodMinigunner2Obj = Commands->Find_Object(116387);

	if ((damager == nodMinigunner1Obj || damager == nodMinigunner2Obj) && !this->attackedByNodMinigunners)
	{
		this->attackedByNodMinigunners = true;

		Commands->Send_Custom_Event(obj, obj, 0, 41, 0.0f);
	}

	if (this->preventDeath)
	{
		if (Commands->Get_Health(obj) <= 10.0f)
		{
			Commands->Set_Health(obj, 10.0);
		}
	}
	else
	{
		if (damager == gdiGrenadierObj)
		{
			Commands->Set_Health(obj, this->health);
		}
		else
		{
			this->health = Commands->Get_Health(obj);
		}
	}
}

// TODO
void M01_Base_GDI_Minigunner_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		if (param == 27)
		{
			Commands->Set_Innate_Is_Stationary(obj, false);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 38);
			params.Set_Attack(Vector3(72.223f, -24.751f, -0.296f), 1000.0f, 0.0f, true);
			params.Set_Movement(Vector3(66.771f, -38.435f, -0.058f), 0.8f, 1.0f);

			Commands->Action_Goto(obj, params); // Yes, not Action_Attack

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-74.184f, -28.372f, 3.157f));
			Commands->Set_Facing(invisObj, 90.0f);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X01D_C130Troopdrop.txt");
		}
		else if (param == 28)
		{
			Commands->Set_Innate_Is_Stationary(obj, false);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 39);
			params.Set_Attack(Vector3(28.449f, -23.325f, 3.032f), 1000.0f, 0.0f, true);
			params.Set_Movement(Vector3(40.893f, -39.773f, -0.179f), 1.05f, 1.0f);

			Commands->Action_Goto(obj, params); // Yes, not Action_Attack
		}
		else if (param == 17)
		{
			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(34.165f, 41.909f, -10.0f));
			Commands->Attach_Script(invisObj, "Test_Cinematic", "XG_Ap_vs_Trn.txt");

			GameObject *nodMinigunner3Obj = Commands->Find_Object(116384);
			if (nodMinigunner3Obj)
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 17);
				params.Set_Movement(Vector3(40.893f, -39.773f, -0.179f), 1.05f, 1.0f);
				params.Set_Attack(nodMinigunner3Obj, 1000.0f, 0.0f, true);
				params.AttackCrouched = true;
				params.AttackCheckBlocked = false;

				Commands->Action_Attack(obj, params);
			}
			else
			{
				Commands->Action_Reset(obj, 100.0f);

				Commands->Send_Custom_Event(obj, obj, 0, 29, 0.0f);
			}
		}
		else if (param == 29)
		{
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);
			Vector3 starPos = Commands->Get_Position(starObj);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 16);
			params.Set_Face_Location(starPos, 2.0f);

			Commands->Action_Face_Location(obj, params);

			this->itsHavocConversationId = Commands->Create_Conversation("M01_GDI01_Conversation_01", 100, 1000.0f, false); // It's Havoc!
			Commands->Join_Conversation(obj, this->itsHavocConversationId, true, true, true);
			Commands->Join_Conversation(starObj, this->itsHavocConversationId, false, false, false);
			Commands->Start_Conversation(this->itsHavocConversationId, this->itsHavocConversationId);
			Commands->Monitor_Conversation(obj, this->itsHavocConversationId);

			GameObject *gdiGrenadierObj = Commands->Find_Object(116383);
			if (gdiGrenadierObj)
			{
				Commands->Send_Custom_Event(obj, gdiGrenadierObj, 0, 16, 0.0f);
			}
		}
		else if (param == 30)
		{
			GameObject *M01ObjectivePogControllerJDGObj = Commands->Find_Object(105828);
			if (M01ObjectivePogControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01ObjectivePogControllerJDGObj, 6, 114, 0.0f);
			}
		}
		else if (param == 9)
		{
			Commands->Enable_Hibernation(obj, false);

			this->field_2D = true;
			this->field_2E = false;

			Commands->Action_Reset(obj, 100.0f);

			Commands->Set_Innate_Soldier_Home_Location(obj, Vector3(56.0f, 28.2f, -0.2f), 10.0f);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 9);
			params.Set_Movement(Vector3(56.0f, 28.2f, -0.2f), 0.8f, 1.0f);

			Commands->Action_Goto(obj, params);
		}
		else if (param == 4001)
		{
			Commands->Action_Reset(obj, 100.0f);
			
			GameObject *M01GDIBaseCommanderEvacControllerJDGObj = Commands->Find_Object(106694);
			if (M01GDIBaseCommanderEvacControllerJDGObj)
			{
				Commands->Set_Innate_Is_Stationary(obj, false);

				Vector3 M01GDIBaseCommanderEvacControllerJDGObjPos = Commands->Get_Position(M01GDIBaseCommanderEvacControllerJDGObj);

				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 4001);
				params.Set_Movement(M01GDIBaseCommanderEvacControllerJDGObjPos, 0.8f, 1.0f);

				Commands->Action_Goto(obj, params);
			}
		}
		else if (param == 31)
		{
			if (!this->field_2E)
			{
				if (this->field_31)
				{
					this->field_2F = false;

					Commands->Action_Reset(obj, 100.0f);

					this->field_2E = true;

					ActionParamsStruct params;
					params.Set_Basic(this, 100.0f, 43);
					params.Set_Movement(Vector3(95.229f, 101.136f, 8.934f), 0.8f, 1.0f);
					params.AttackActive = false;

					Commands->Action_Attack(obj, params);
				}
				else
				{
					Vector3 pos = Commands->Get_Position(obj);
					GameObject *starObj = Commands->Get_A_Star(pos);
					Vector3 starPos = Commands->Get_Position(starObj);

					ActionParamsStruct params;
					params.Set_Basic(this, 100.0f, 16);
					params.Set_Face_Location(starPos, 2.0f);

					Commands->Action_Face_Location(obj, params);

					this->pathConversationId = Commands->Create_Conversation("M01_GDI01_Conversation_05", 100, 1000.0f, false); // This is the path.  Let's go!
					Commands->Join_Conversation(obj, this->pathConversationId, true, true, true);
					Commands->Join_Conversation(starObj, this->pathConversationId, false, false, false);
					Commands->Start_Conversation(this->pathConversationId, this->pathConversationId);
					Commands->Monitor_Conversation(obj, this->pathConversationId);
				}
			}
		}
		else if (param == 32)
		{
			if (!this->field_2D)
			{
				this->field_31 = true;
			}
		}
		else if (param == 22)
		{
			if (this->field_2F)
			{
				this->preventDeath = false;

				Commands->Apply_Damage(obj, 100000.0f, "Steel", NULL);
			}
		}
		else if (param == 41)
		{
			GameObject *nodMinigunner1Obj = Commands->Find_Object(116388);
			if (nodMinigunner1Obj)
			{
				Commands->Set_Innate_Is_Stationary(obj, false);

				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 41);
				params.Set_Movement(Vector3(58.784f, 55.223f, -0.551f), 0.8f, 1.0f);
				params.Set_Attack(nodMinigunner1Obj, 50.0f, 2.0f, true);
				params.AttackCheckBlocked = false;

				Commands->Action_Attack(obj, params);
			}
			else
			{
				Commands->Set_Innate_Is_Stationary(obj, false);

				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 41);
				params.Set_Movement(Vector3(58.784f, 55.223f, -0.551f), 0.8f, 1.0f);
				params.AttackCheckBlocked = false;

				Commands->Action_Attack(obj, params); // Yes, Action_Attack here
			}
		}
	}
}

void M01_Base_GDI_Minigunner_JDG::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	if (this->field_2E)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 43);
		params.Set_Movement(enemy, 0.3f, 5.0f);
		params.Set_Attack(enemy, 1000.0f, 0.0f, true);
		params.AttackActive = true;

		Commands->Modify_Action(obj, 43, params, true, true);
	}
}

// TODO
void M01_Base_GDI_Minigunner_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED)
	{
		if (action_id == this->itsHavocConversationId)
		{
			this->baseDestroyedConversationId = Commands->Create_Conversation("M01_GDI01_Conversation_02", 100, 1000.0f, false); // Our base is destroyed and Nod is taking prisoners!  Follow me!
			Commands->Join_Conversation(obj, this->baseDestroyedConversationId, true, true, true);
			
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			Commands->Join_Conversation(starObj, this->baseDestroyedConversationId, false, false, false);
			Commands->Start_Conversation(this->baseDestroyedConversationId, this->baseDestroyedConversationId);
			Commands->Monitor_Conversation(obj, this->baseDestroyedConversationId);
		}
		else if (action_id == this->pathConversationId)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 31, 5.0f);
		}
		else if (action_id == this->baseDestroyedConversationId)
		{
			Commands->Set_HUD_Help_Text(7699, Vector3(0.196f, 0.88200003f, 0.196f)); // Let's Go!\n

			Commands->Send_Custom_Event(obj, obj, 0, 30, 2.0f);

			Commands->Set_Innate_Is_Stationary(obj, false);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 40);
			params.Set_Movement(Vector3(31.697f, 16.155f, 2.2f), 0.8f, 1.0f);

			Commands->Action_Goto(obj, params);

			GameObject *gdiGrenadierObj = Commands->Find_Object(116383);
			if (gdiGrenadierObj)
			{
				Commands->Send_Custom_Event(obj, gdiGrenadierObj, 0, 29, 0.0f);
			}
		}
	}
	else if (complete_reason == ACTION_COMPLETE_NORMAL)
	{
		if (action_id == 38)
		{
			Commands->Set_Innate_Is_Stationary(obj, true);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 16);
			params.Set_Attack(Vector3(72.223f, -24.751f, -0.296f), 1000.0f, 0.0f, true);
			params.AttackCrouched = true;
			params.AttackCheckBlocked = false;

			Commands->Action_Attack(obj, params);

			Commands->Send_Custom_Event(obj, obj, 0, 28, 2.0f);
		}
		else if (action_id == 39)
		{
			Commands->Set_Innate_Is_Stationary(obj, true);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 16);
			params.Set_Attack(Vector3(28.449f, -23.325f, 3.032f), 1000.0f, 0.0f, true);
			params.AttackCrouched = true;
			params.AttackCheckBlocked = false;

			Commands->Action_Attack(obj, params);
		}
		else if (action_id == 17)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 29, 0.0f);
		}
		else if (action_id == 9)
		{
			Commands->Set_Innate_Is_Stationary(obj, true);

			GameObject *M01GDIBaseCommanderEvacControllerJDGObj = Commands->Find_Object(106694);
			if (M01GDIBaseCommanderEvacControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01GDIBaseCommanderEvacControllerJDGObj, 0, 27, 0.0f);
			}
		}
		else if (action_id == 4001)
		{
			Vector3 pos = Commands->Get_Position(obj);
			pos.Z += 0.75f;

			static const char *drops[2] =
			{
				"POW_Health_100",
				"POW_Armor_100"
			};

			int randInt = Commands->Get_Random_Int(0, 2);
			Commands->Create_Object(drops[randInt], pos);

			GameObject *M01GDIBaseCommanderEvacControllerJDGObj = Commands->Find_Object(106694);
			if (M01GDIBaseCommanderEvacControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01GDIBaseCommanderEvacControllerJDGObj, 0, 10, 0.0f);
			}
		}
		else if (action_id == 40)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 41, 0.0f);
		}
		else if (action_id == 41)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 42);
			params.Set_Movement(Vector3(58.784f, 55.223f, -0.551f), 0.8f, 1.0f);

			Commands->Action_Goto(obj, params);
		}
		else if (action_id == 42)
		{
			Commands->Set_Innate_Is_Stationary(obj, true);
		}
		else if (action_id == 43)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 43);
			params.Set_Movement(Vector3(95.229f, 101.136f, 8.934f), 0.8f, 1.0f);
			params.AttackActive = false;

			Commands->Action_Attack(obj, params);
		}
	}
}

void M01_Base_GDI_Minigunner_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "S_A_Human.XG_EV5_troop"))
	{
		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_Base_GDI_Minigunner_JDG> M01_Base_GDI_Minigunner_JDGRegistrant("M01_Base_GDI_Minigunner_JDG", "");