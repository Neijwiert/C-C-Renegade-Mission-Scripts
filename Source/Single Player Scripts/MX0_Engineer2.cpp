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
#include "MX0_Engineer2.h"

void MX0_Engineer2::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->lastHealthAmount, sizeof(this->lastHealthAmount), 2);
	Auto_Save_Variable(&this->field_24, sizeof(this->field_24), 3);
	Auto_Save_Variable(&this->field_28, sizeof(this->field_28), 4);
	Auto_Save_Variable(&this->field_30, sizeof(this->field_30), 5);
	Auto_Save_Variable(&this->field_2C, sizeof(this->field_2C), 6);
}

void MX0_Engineer2::Created(GameObject *obj)
{
	this->field_2C = 0;
	this->field_24 = 0;
	this->field_28 = 0;
	this->lastHealthAmount = Commands->Get_Health(obj);
	this->field_30 = false;

	Commands->Start_Timer(obj, this, 4.0f, 121);
	Commands->Start_Timer(obj, this, 3.5f, 123);

	int objId = Commands->Get_ID(obj);
	GameObject *MX0MissionStartDMEObj = Commands->Find_Object(1200001);
	Commands->Send_Custom_Event(obj, MX0MissionStartDMEObj, 103, objId, 0.0f);
}

void MX0_Engineer2::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (action_id == 113)
	{
		ActionParamsStruct actionParamsStruct;
		actionParamsStruct.Set_Basic(this, 96.0f, 101);

		GameObject *davesArrowWaitingForHavocObj = Commands->Find_Object(1200026);
		Vector3 davesArrowWaitingForHavocObjPos = Commands->Get_Position(davesArrowWaitingForHavocObj);
		actionParamsStruct.Set_Movement(davesArrowWaitingForHavocObjPos, 0.8f, 0.8f);

		Commands->Action_Goto(obj, actionParamsStruct);

		Commands->Send_Custom_Event(obj, obj, 100005, 0, 0.60000002f);
	}
	else if (action_id == 135)
	{
		this->field_30 = false;
	}
	else if (action_id == 101)
	{
		if (Commands->Get_Action_ID(obj) != 101 && !Commands->Is_Performing_Pathfind_Action(obj) && (complete_reason == ACTION_COMPLETE_LOW_PRIORITY || complete_reason == ACTION_COMPLETE_PATH_BAD_DEST))
		{
			Commands->Start_Timer(obj, this, 5.0f, 116);
		}

		if (this->field_24 == 4 && complete_reason == ACTION_COMPLETE_NORMAL)
		{
			GameObject *MX0MissionStartDMEObj = Commands->Find_Object(1200001);
			Commands->Send_Custom_Event(obj, MX0MissionStartDMEObj, 132, 0, 0.0f);

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);
			Commands->Lock_Soldier_Facing(obj, starObj, true);

			ActionParamsStruct actionParamsStruct;
			actionParamsStruct.Set_Basic(this, 97.0f, 113);

			actionParamsStruct.Set_Animation("h_a_b0c0", true);

			Commands->Action_Play_Animation(obj, actionParamsStruct);
		}
	}
}

void MX0_Engineer2::Timer_Expired(GameObject *obj, int number)
{
	if (number == 137)
	{
		GameObject *field2CObj = Commands->Find_Object(this->field_2C);

		static int ENGINEER_WAIT_FOR_SNIPER_DEATH_OBJECT_IDS[] =
		{
			1200011,
			1200021,
			1200040
		};

		int randomInt = Commands->Get_Random_Int(0, 3);
		int waitObjId = ENGINEER_WAIT_FOR_SNIPER_DEATH_OBJECT_IDS[randomInt];

		ActionParamsStruct actionParamsStruct;
		actionParamsStruct.Set_Basic(this, 97.0f, 137);

		GameObject *waitObj = Commands->Find_Object(waitObjId);
		actionParamsStruct.Set_Movement(waitObj, 0.3f, 0.2f, true);
		actionParamsStruct.AttackCrouched = true;

		Commands->Action_Goto(obj, actionParamsStruct);
	}
	else if (number == 116)
	{
		ActionParamsStruct actionParamsStruct;
		actionParamsStruct.Set_Basic(this, 96.0f, 101);

		GameObject *field28Obj = Commands->Find_Object(this->field_28);
		Vector3 field28ObjPos = Commands->Get_Position(field28Obj);
		actionParamsStruct.Set_Movement(field28ObjPos, 0.80000001f, 1.0f);

		Commands->Action_Goto(obj, actionParamsStruct);
	}
	else if (number == 121)
	{
		Commands->Set_Innate_Is_Stationary(obj, true);
	}
	else if (number == 123)
	{
		ActionParamsStruct actionParamsStruct;
		actionParamsStruct.Set_Basic(this, 98.0f, 113);

		actionParamsStruct.Set_Animation("H_A_A0A0_L32", false);

		Commands->Action_Play_Animation(obj, actionParamsStruct);
	}
}

void MX0_Engineer2::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	float currentHealth = Commands->Get_Health(obj);
	this->field_1C = currentHealth;
	float healthDifference = this->lastHealthAmount - currentHealth;
	float newHealth = this->lastHealthAmount - Get_Float_Parameter("Damage_multiplier") * healthDifference;
	Commands->Set_Health(obj, newHealth);
	this->lastHealthAmount = Commands->Get_Health(obj);
	this->field_1C = Commands->Get_Health(obj);

	Vector3 pos = Commands->Get_Position(obj);
	if (damager == Commands->Get_A_Star(pos) && !this->field_30)
	{
		static const char * const ENGINEER_TAKING_DAMAGE_CONVERSATIONS[] =
		{
			// - Check your fire!
			"MX0CON008",

			// - Medic!
			"MX0CON009",

			// - Hey! Watch out!
			"MX0CON010",

			// - I'm taking fire!
			"MX0CON011"
		};

		int randomInt = Commands->Get_Random_Int(0, 4);
		int conversationId = Commands->Create_Conversation(ENGINEER_TAKING_DAMAGE_CONVERSATIONS[randomInt], 97, 2000.0f, false);

		Commands->Join_Conversation(obj, conversationId, false, true, true);

		GameObject *starObj = Commands->Get_A_Star(pos);
		Commands->Join_Conversation(starObj, conversationId, true, true, true);

		Commands->Start_Conversation(conversationId, 100008);
		Commands->Monitor_Conversation(obj, conversationId);

		this->field_30 = true;

		ActionParamsStruct actionParamsStruct;
		actionParamsStruct.Set_Basic(this, 100.0f, 135);

		actionParamsStruct.Set_Animation("H_A_J21C", false);

		Commands->Action_Play_Animation(obj, actionParamsStruct);
	}
}

void MX0_Engineer2::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_strcmpi(animation_name, "H_A_J21C"))   // Should be == 0, but it doesn't appear to be compiled like that
	{
		this->field_30 = false;
	}
}

void MX0_Engineer2::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 136)
	{
		this->field_2C = param;
		Commands->Start_Timer(obj, this, 5.0f, 137);
	}
	else if (type == 131)
	{
		Commands->Action_Reset(obj, 100.0f);

		ActionParamsStruct actionParamsStruct;
		actionParamsStruct.Set_Basic(this, 97.0f, 131);

		GameObject *engineerStartDavesArrow = Commands->Find_Object(1200007);
		actionParamsStruct.Set_Movement(engineerStartDavesArrow, 0.8f, 1.0f);

		Commands->Action_Goto(obj, actionParamsStruct);
	}
	else if (type == 125)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);
		Vector3 starPos = Commands->Get_Position(starObj);

		float faceAngle = atan2(starPos.X - pos.X, starPos.Y - pos.Y) * 57.295776f;
		Commands->Set_Facing(obj, faceAngle);

		Commands->Action_Reset(obj, 100.0f);

		Commands->Set_Animation(obj, "H_A_A0A0_L53", false, NULL, 0.0f, -1.0f, true);
	}
	else if (type == 112)
	{
		ActionParamsStruct actionParamsStruct;
		actionParamsStruct.Set_Basic(this, 95.0f, 1);

		GameObject *leftOfHidingRocksDavesArrow = Commands->Find_Object(1200022);
		actionParamsStruct.Set_Movement(leftOfHidingRocksDavesArrow, 1.2f, 1.0f, true);

		GameObject *nonExistingObject = Commands->Find_Object(1200017);
		actionParamsStruct.Set_Attack(nonExistingObject, 250.0f, 0.0f, true);
		actionParamsStruct.AttackCheckBlocked = true;

		Commands->Action_Attack(obj, actionParamsStruct);
	}
	else if (type == 108)
	{
		this->field_24 = param;
	}
	else if (type == 100)
	{
		this->field_28 = param;

		ActionParamsStruct actionParamsStruct;
		actionParamsStruct.Set_Basic(this, 96.0f, 101);

		GameObject *field28Obj = Commands->Find_Object(this->field_28);
		Vector3 field28ObjPos = Commands->Get_Position(field28Obj);
		actionParamsStruct.Set_Movement(field28ObjPos, 0.80000001f, 0.80000001f);

		Commands->Action_Goto(obj, actionParamsStruct);
	}
}

ScriptRegistrant<MX0_Engineer2> MX0_Engineer2Registrant("MX0_Engineer2", "Damage_multiplier:float");