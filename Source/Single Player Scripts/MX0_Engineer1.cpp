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
#include "MX0_Engineer1.h"

void MX0_Engineer1::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->lastHealthAmount1, sizeof(this->lastHealthAmount1), 1);
	Auto_Save_Variable(&this->lastHealthAmount2, sizeof(this->lastHealthAmount2), 2);
	Auto_Save_Variable(&this->engineerGotoZoneCount, sizeof(this->engineerGotoZoneCount), 3);
	Auto_Save_Variable(&this->gotoDestObjId, sizeof(this->gotoDestObjId), 4);
	Auto_Save_Variable(&this->doingDamageAnimation, sizeof(this->doingDamageAnimation), 5);
}

void MX0_Engineer1::Created(GameObject *obj)
{
	// Initialize member variables
	this->doingDamageAnimation = false;
	this->engineerGotoZoneCount = 0;
	this->gotoDestObjId = 0;
	this->lastHealthAmount2 = Commands->Get_Health(obj);

	// Set innate is stationary
	Commands->Start_Timer(obj, this, 4.0f, 121);

	// Moves engineer to wait position for star to come
	Commands->Start_Timer(obj, this, 4.0f, 122);

	// Send our id to the start controller where it is stored for later use
	int objId = Commands->Get_ID(obj);
	GameObject *MX0MissionStartDMEObj = Commands->Find_Object(1200001);
	Commands->Send_Custom_Event(obj, MX0MissionStartDMEObj, 103, objId, 0.0f);
}

void MX0_Engineer1::Timer_Expired(GameObject *obj, int number)
{
	// Triggered 4.0 seconds after script creation 
	if (number == 121)
	{
		Commands->Set_Innate_Is_Stationary(obj, true);
	}

	// Triggered 4.0 seconds after script creation
	else if (number == 122)
	{
		// Initialize ActionParamsStruct
		ActionParamsStruct actionParamsStruct;
		actionParamsStruct.Set_Basic(this, 96.0f, 101);

		// Set movement to the wait position for the star to come
		GameObject *davesArrowWaitingForHavocObj = Commands->Find_Object(1200029);
		Vector3 davesArrowWaitingForHavocObjPos = Commands->Get_Position(davesArrowWaitingForHavocObj);
		actionParamsStruct.Set_Movement(davesArrowWaitingForHavocObjPos, 0.80000001f, 0.80000001f);

		// Execute movement
		Commands->Action_Goto(obj, actionParamsStruct);
	}

	// TODO
	else if (number == 116)
	{
		// Initialize ActionParamsStruct
		ActionParamsStruct actionParamsStruct;
		actionParamsStruct.Set_Basic(this, 96.0f, 101);

		// Find and set movement to the stored goto dest obj
		GameObject *gotoDestObj = Commands->Find_Object(this->gotoDestObjId);
		Vector3 gotoDestObjPos = Commands->Get_Position(gotoDestObj);
		actionParamsStruct.Set_Movement(gotoDestObjPos, 0.8f, 1.0f);

		// Execute movement
		Commands->Action_Goto(obj, actionParamsStruct);
	}
}

void MX0_Engineer1::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered when we are at the wait position for star and goto dest obj pos 
	if (action_id == 101)
	{
		// If we're not doing the same action now AND not doing a pathfind action AND we completed this one on low priority
		if (Commands->Get_Action_ID(obj) != 101 && !Commands->Is_Performing_Pathfind_Action(obj) && complete_reason == ACTION_COMPLETE_LOW_PRIORITY)
		{
			// Move to the goto dest obj
			Commands->Start_Timer(obj, this, 5.0f, 116);
		}

		// If we're in the small passageway AND we completed this one normally
		if (this->engineerGotoZoneCount == 3 && complete_reason == ACTION_COMPLETE_NORMAL)
		{
			// Starts conversation of engineer 1 MX0CON004:
			// - Looks like they were boxed in.
			// And it disables engineer 1 and 2's innate
			GameObject *MX0MissionStartDMEObj = Commands->Find_Object(1200001);
			Commands->Send_Custom_Event(obj, MX0MissionStartDMEObj, 100004, 0, 0.0f); 
		}
	}

	// TODO
	else if (action_id == 113)
	{
		// Initialize ActionParamsStruct
		ActionParamsStruct actionParamsStruct;
		actionParamsStruct.Set_Basic(this, 96.0f, 101);

		// Get the position of the dave's arrow where the engineer waits for Havoc to come
		GameObject *davesArrowWaitingForHavocObj = Commands->Find_Object(1200029);
		Vector3 davesArrowWaitingForHavocObjPos = Commands->Get_Position(davesArrowWaitingForHavocObj);

		// Set movement to that location
		actionParamsStruct.Set_Movement(davesArrowWaitingForHavocObjPos, 0.8f, 0.8f);

		// Execute movement
		Commands->Action_Goto(obj, actionParamsStruct);
	}
	else if (action_id == 135)
	{
		this->doingDamageAnimation = false;
	}
	
}

void MX0_Engineer1::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	float currentHealth = Commands->Get_Health(obj);
	this->lastHealthAmount1 = currentHealth;
	float healthDifference = this->lastHealthAmount2 - currentHealth;
	float newHealth = this->lastHealthAmount2 - Get_Float_Parameter("Damage_multiplier") * healthDifference;
	Commands->Set_Health(obj, newHealth);
	this->lastHealthAmount2 = Commands->Get_Health(obj);
	this->lastHealthAmount1 = Commands->Get_Health(obj);

	Vector3 pos = Commands->Get_Position(obj);
	if (damager == Commands->Get_A_Star(pos) && !this->doingDamageAnimation)
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

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);
		Commands->Join_Conversation(starObj, conversationId, true, true, true);

		Commands->Start_Conversation(conversationId, 100008);
		Commands->Monitor_Conversation(obj, conversationId);

		this->doingDamageAnimation = true;

		ActionParamsStruct actionParamsStruct;
		actionParamsStruct.Set_Basic(this, 100.0f, 135);
	
		actionParamsStruct.Set_Animation("H_A_J21C", false);

		Commands->Action_Play_Animation(obj, actionParamsStruct);
	}
}

void MX0_Engineer1::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "H_A_J21C")) // Should be == 0, but it doesn't appear to be compiled like that
	{
		this->doingDamageAnimation = false;
	}
}

void MX0_Engineer1::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 136)
	{
		GameObject *paramObj = Commands->Find_Object(param);

		Commands->Unlock_Soldier_Facing(obj);

		if (paramObj)
		{
			Commands->Lock_Soldier_Facing(obj, paramObj, true);

			static int ENGINEER_WAIT_FOR_SNIPER_DEATH_OBJECT_IDS[] = 
			{
				1200010,
				1200022,
				1200039
			};

			int randomInt = Commands->Get_Random_Int(0, 3);
			
			ActionParamsStruct actionParamsStruct;
			actionParamsStruct.Set_Basic(this, 97.0f, 137);

			GameObject *waitObj = Commands->Find_Object(ENGINEER_WAIT_FOR_SNIPER_DEATH_OBJECT_IDS[randomInt]);
			actionParamsStruct.Set_Movement(waitObj, 0.3f, 0.2f, true);
			actionParamsStruct.AttackCrouched = true;

			Commands->Action_Goto(obj, actionParamsStruct);
		}
	}
	else if (type == 131)
	{
		Commands->Action_Reset(obj, 100.0f);

		ActionParamsStruct actionParamsStruct;
		actionParamsStruct.Set_Basic(this, 97.0f, 131);

		GameObject *engineerStartDavesArrow = Commands->Find_Object(1200006);
		actionParamsStruct.Set_Movement(engineerStartDavesArrow, 0.8f, 1.0f);

		Commands->Action_Goto(obj, actionParamsStruct);
	}
	else if (type == 132)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Lock_Soldier_Facing(obj, starObj, true);

		ActionParamsStruct actionParamsStruct;
		actionParamsStruct.Set_Basic(this, 97, 113);

		actionParamsStruct.Set_Animation("h_a_b0c0", true);

		Commands->Action_Play_Animation(obj, actionParamsStruct);
	}
	else if (type == 112)
	{
		ActionParamsStruct actionParamsStruct;
		actionParamsStruct.Set_Basic(this, 95, 1);

		GameObject *rightOfHidingRocksDavesArrow = Commands->Find_Object(1200021);
		actionParamsStruct.Set_Movement(rightOfHidingRocksDavesArrow, 1.2f, 1.0f, true);

		GameObject *nonExistingObject = Commands->Find_Object(1200017);
		actionParamsStruct.Set_Attack(nonExistingObject, 250.0f, 0.0f, true);
		actionParamsStruct.AttackCheckBlocked = true;

		Commands->Action_Attack(obj, actionParamsStruct);
	}
	else if (type == 108)
	{
		this->engineerGotoZoneCount = param;
	}
	else if (type == 100)
	{
		this->gotoDestObjId = param;

		ActionParamsStruct actionParamsStruct;
		actionParamsStruct.Set_Basic(this, 96.0f, 101);

		GameObject *field28Obj = Commands->Find_Object(this->gotoDestObjId);
		Vector3 field28ObjPos = Commands->Get_Position(field28Obj);
		actionParamsStruct.Set_Movement(field28ObjPos, 0.80000001f, 0.80000001f);

		Commands->Action_Goto(obj, actionParamsStruct);
	}
}

ScriptRegistrant<MX0_Engineer1> MX0_Engineer1Registrant("MX0_Engineer1", "Damage_multiplier:float");