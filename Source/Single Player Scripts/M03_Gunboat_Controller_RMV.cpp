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
#include "M03_Gunboat_Controller_RMV.h"

/*
M03 -> 1100003
*/
void M03_Gunboat_Controller_RMV::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->areaIndex, sizeof(this->areaIndex), 1);
	Auto_Save_Variable(&this->currentArea, sizeof(this->currentArea), 2);
	Auto_Save_Variable(&this->beachDestination, sizeof(this->beachDestination), 3);
	Auto_Save_Variable(&this->villageStart, sizeof(this->villageStart), 4);
	Auto_Save_Variable(&this->villageDestination, sizeof(this->villageDestination), 5);
	Auto_Save_Variable(&this->cannonStart, sizeof(this->cannonStart), 6);
	Auto_Save_Variable(&this->cannonDestination, sizeof(this->cannonDestination), 7);
	Auto_Save_Variable(this->beachAttackLocations, sizeof(this->beachAttackLocations), 8);
	Auto_Save_Variable(this->villageAttackLocations, sizeof(this->villageAttackLocations), 9);
	Auto_Save_Variable(this->cannonAttackLocations, sizeof(this->cannonAttackLocations), 10);
	Auto_Save_Variable(&this->beachAttackError, sizeof(this->beachAttackError), 11);
	Auto_Save_Variable(&this->villageAttackError, sizeof(this->villageAttackError), 12);
	Auto_Save_Variable(&this->cannonAttackError, sizeof(this->cannonAttackError), 13);
	Auto_Save_Variable(&this->attackLocationIndex, sizeof(this->attackLocationIndex), 14);
	Auto_Save_Variable(&this->damagedByStar, sizeof(this->damagedByStar), 15);
	Auto_Save_Variable(&this->currentHealth, sizeof(this->currentHealth), 16);
	Auto_Save_Variable(&this->downToHalfHealth, sizeof(this->downToHalfHealth), 17);
	Auto_Save_Variable(&this->downToQuarterHealth, sizeof(this->downToQuarterHealth), 18);
	Auto_Save_Variable(&this->field_DF, sizeof(this->field_DF), 19);
	Auto_Save_Variable(&this->inletTargetKilledCount, sizeof(this->inletTargetKilledCount), 20);
	Auto_Save_Variable(&this->inletTargetsAlive, sizeof(this->inletTargetsAlive), 21);
	Auto_Save_Variable(&this->damagedByBigGunCount, sizeof(this->damagedByBigGunCount), 22);
	Auto_Save_Variable(&this->bigGunKilled, sizeof(this->bigGunKilled), 23);
}

void M03_Gunboat_Controller_RMV::Created(GameObject *obj)
{
	Commands->Enable_Hibernation(obj, false);
	Commands->Enable_Engine(obj, true);

	this->field_DF = false;
	this->damagedByStar = false;
	this->downToQuarterHealth = false;
	this->downToHalfHealth = false;
	this->currentHealth = Commands->Get_Max_Health(obj);
	this->beachDestination = Get_Vector3_Parameter("Beach_Destination");
	this->villageStart = Get_Vector3_Parameter("Village_Start");
	this->villageDestination = Get_Vector3_Parameter("Village_Destination");
	this->cannonStart = Get_Vector3_Parameter("Cannon_Start");
	this->cannonDestination = Get_Vector3_Parameter("Cannon_Destination");
	this->beachAttackError = 3.0f;
	this->villageAttackError = 3.0f;
	this->cannonAttackError = 3.0f;
	this->beachAttackLocations[0] = Vector3(-116.0f, -44.0f, 25.0f);
	this->beachAttackLocations[1] = Vector3(-96.0f, -36.0f, 25.0f);
	this->beachAttackLocations[2] = Vector3(-74.0f, -35.0f, 29.0f);
	this->villageAttackLocations[0] = Vector3(24.0f, -25.0f, 16.0f);
	this->villageAttackLocations[1] = Vector3(45.0f, -30.0f, 16.0f);
	this->villageAttackLocations[2] = Vector3(80.0f, -12.0f, 16.0f);
	this->cannonAttackLocations[0] = Vector3(91.0f, 42.0f, 27.0f);
	this->cannonAttackLocations[1] = Vector3(66.0f, 71.0f, 17.0f);
	this->cannonAttackLocations[2] = Vector3(52.558f, 51.739f, 28.88f);
	this->inletTargetKilledCount = 0;
	this->inletTargetsAlive = true;
	this->areaIndex = 0;

	ActionParamsStruct params;
	params.Set_Basic(this, 90.0f, 0);
	params.Set_Movement(this->beachDestination, 0.25f, 3.0f);

	this->attackLocationIndex = Commands->Get_Random(0.0f, 3.0f);
	if (this->attackLocationIndex >= 0.0f)
	{
		if (this->attackLocationIndex > 2.0f)
		{
			this->attackLocationIndex = 2.0f;
		}
	}
	else
	{
		this->attackLocationIndex = 0.0f;
	}

	params.Set_Attack(this->beachAttackLocations[static_cast<int>(this->attackLocationIndex)], 500.0f, this->beachAttackError, true);
	params.AttackCheckBlocked = false;
	params.MovePathfind = false;
	params.AttackFaceTarget = false;

	Commands->Action_Attack(obj, params);

	Commands->Start_Timer(obj, this, 3.5f, 1);

	this->bigGunKilled = false;
	this->damagedByBigGunCount = 0;
}

void M03_Gunboat_Controller_RMV::Killed(GameObject *obj, GameObject *killer)
{
	Commands->Create_Explosion_At_Bone("Ground Explosions Twiddler", obj, "TURRET", NULL);

	if (killer == Commands->Find_Object(1100002)) // Big Gun
	{
		int conversationId = Commands->Create_Conversation("M03CON031", 0, 0.0f, true); // This is Captain Soanso sending a general S.O.S. alert, we've taken serious damage from shore defense and - <static>
		Commands->Join_Conversation(NULL, conversationId, true, true, true);
		Commands->Start_Conversation(conversationId, 100031);
		Commands->Monitor_Conversation(obj, conversationId);

		GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);
		Commands->Send_Custom_Event(obj, M03ObjectiveControllerObj, 40026, 0, 0.0f);

		GameObject *gdiGunBoatObj = Commands->Find_Object(1100003);
		Commands->Send_Custom_Event(obj, gdiGunBoatObj, 40026, 0, 0.0f);
	}
	else
	{
		// This is GDI gunboat captain Soanso, I'm taking heavy fire, need - <static and screams>
		// Guess I don't have to worry about the gunboat.
		// Sir, the gunboat's down!
		int conversationId = Commands->Create_Conversation("M03CON018", 0, 0.0f, true);
		Commands->Join_Conversation(NULL, conversationId, true, true, true);
		Commands->Join_Conversation(NULL, conversationId, true, true, true);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, true, true);
		Commands->Start_Conversation(conversationId, 100018);
		Commands->Monitor_Conversation(obj, conversationId);
	}
}

void M03_Gunboat_Controller_RMV::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (damager && Commands->Is_A_Star(damager))
	{
		if (!this->damagedByStar)
		{
			this->damagedByStar = true;

			Commands->Create_2D_Sound("00-N062E");
		}
	}

	if (Commands->Get_Health(obj) > 0.0f)
	{
		float lostHealth = this->currentHealth - Commands->Get_Health(obj);
		float calculatedDamage = lostHealth * (Commands->Get_Difficulty_Level() + 1);

		Commands->Set_Health(obj, this->currentHealth - calculatedDamage);
		this->currentHealth = Commands->Get_Health(obj);

		if (!this->downToHalfHealth)
		{
			if (this->currentHealth < Commands->Get_Max_Health(obj) * 0.5f)
			{
				this->downToHalfHealth = true;
			}
		}
		else
		{
			if (!this->downToQuarterHealth)
			{
				if (this->currentHealth < Commands->Get_Max_Health(obj) * 0.25f)
				{
					this->downToQuarterHealth = true;
				}
			}
		}

		if (damager == Commands->Find_Object(1100002)) // Big Gun
		{
			this->damagedByBigGunCount++;
			if (this->damagedByBigGunCount > 1 && this->damagedByBigGunCount <= 3)
			{
				GameObject *bigGunSAMSiteObj = Commands->Find_Object(300058);
				if (Commands->Get_Health(bigGunSAMSiteObj) > 0.0f)
				{
					int conversationId = Commands->Create_Conversation("M03CON029", 99, 2000.0f, true); // I'm taking a pounding from that cannon, Commando! Any luck with the SAM Sites?
					Commands->Join_Conversation(NULL, conversationId, true, true, true);
					Commands->Start_Conversation(conversationId, 100029);
					Commands->Monitor_Conversation(obj, conversationId);
				}
			}
			else if (this->damagedByBigGunCount > 3 && this->damagedByBigGunCount <= 5)
			{
				int conversationId = Commands->Create_Conversation("M03CON030", 99, 2000.0f, true); // This is Captain Soanso calling for available assistance! We're going down if that shore defense cannon isn't taken care of!
				Commands->Join_Conversation(NULL, conversationId, true, true, true);
				Commands->Start_Conversation(conversationId, 100030);
				Commands->Monitor_Conversation(obj, conversationId);
			}
		}
	}
}

void M03_Gunboat_Controller_RMV::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Never received
	if (type == 8000 && param == 8000)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 40);
		params.Set_Movement(Vector3(-58.0f, 222.0f, -1.12f), 0.5f, 5.0f);

		Commands->Action_Goto(obj, params);
	}
	else
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 90.0f, 0);

		// Received from RMV_Test_Big_Gun_Turning after 0 seconds when killed.
		if (type == 40027)
		{
			this->bigGunKilled = true;

			Commands->Action_Reset(obj, 100.0f);
		}

		// Received from RMV_Trigger_Killed after 0 seconds when killed (id = 300056, 1100020, 1144678, 1144679))
		else if (type == 7000)
		{
			if (param == 7000)
			{
				if (++this->inletTargetKilledCount >= 4)
				{
					this->inletTargetsAlive = false;
				}
			}
		}

		else if (type == Get_Int_Parameter("Receive_Type"))
		{
			// Never received
			if (param == Get_Int_Parameter("Receive_Param_Destroy"))
			{
				Commands->Destroy_Object(obj);
			}

			// Received from RMV_Trigger_Zone after 0 seconds when entered. type = 2000, param = 1 (id = 1100000)
			// Received from M03_Objective_Controller after 0 seconds when objective completed with id 1001 type = 2000, param = 1
			else if (param == Get_Int_Parameter("Receive_Param_For_Village"))
			{
				params.Set_Movement(this->villageStart, 1.0f, 3.0f);

				this->currentArea = 0;
			}

			// Received from RMV_Trigger_Zone after 0 seconds when entered. type = 2000, param = 2 (id = 1100001, 1100014)
			else if (param == Get_Int_Parameter("Receive_Param_For_Cannon"))
			{
				params.Set_Movement(this->cannonStart, 1.0f, 3.0f);

				this->currentArea = 1;
			}
		}

		this->areaIndex = 3;

		Commands->Action_Goto(obj, params);
	}
}

void M03_Gunboat_Controller_RMV::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered when done moving to new area
	if (action_id != 40 && complete_reason == ACTION_COMPLETE_NORMAL && !action_id)
	{
		if (this->areaIndex == 3)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 90.0f, 0);

			if (!this->currentArea)
			{
				params.Set_Movement(this->villageDestination, 0.25f, 3.0f);

				this->areaIndex = 1;
			}
			else if (this->currentArea == 1)
			{
				params.Set_Movement(this->cannonDestination, 0.25f, 3.0f);

				this->areaIndex = 2;
			}

			Commands->Action_Goto(obj, params);
		}
		else if (this->areaIndex == 2)
		{
			Commands->Action_Reset(obj, 90.0f);
		}
	}
}

void M03_Gunboat_Controller_RMV::Timer_Expired(GameObject *obj, int number)
{
	// Triggered after 3.5 seconds or 1.5 seconds, see create or this block
	if (number == 1 && !this->bigGunKilled)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 90.0f, 0);
		params.AttackCheckBlocked = false;
		params.MovePathfind = false;
		params.AttackFaceTarget = false;

		if (this->areaIndex == 0)
		{
			this->attackLocationIndex = Commands->Get_Random(0.0f, 3.0f);
			if (this->attackLocationIndex >= 0.0f)
			{
				if (this->attackLocationIndex > 2.0f)
				{
					this->attackLocationIndex = 2.0f;
				}
			}
			else
			{
				this->attackLocationIndex = 0.0f;
			}

			params.Set_Movement(this->beachDestination, 0.25f, 3.0f);
			params.Set_Attack(this->beachAttackLocations[static_cast<int>(this->attackLocationIndex)], 500.0f, this->beachAttackError, true);

			Commands->Action_Attack(obj, params);
		}
		else if (this->areaIndex == 1)
		{
			this->attackLocationIndex = Commands->Get_Random(0.0f, 3.0f);
			if (this->attackLocationIndex >= 0.0f)
			{
				if (this->attackLocationIndex > 2.0f)
				{
					this->attackLocationIndex = 2.0f;
				}
			}
			else
			{
				this->attackLocationIndex = 0.0f;
			}

			params.Set_Movement(this->villageDestination, 0.25f, 3.0f);
			params.Set_Attack(this->villageAttackLocations[static_cast<int>(this->attackLocationIndex)], 500.0f, this->villageAttackError, true);


			if (!this->inletTargetsAlive)
			{
				params.AttackActive = false;
			}

			Commands->Action_Attack(obj, params);
		}
		else if (this->areaIndex == 2)
		{
			this->attackLocationIndex = Commands->Get_Random(0.0f, 3.0f);
			if (this->attackLocationIndex >= 0.0f)
			{
				if (this->attackLocationIndex > 2.0f)
				{
					this->attackLocationIndex = 2.0f;
				}
			}
			else
			{
				this->attackLocationIndex = 0.0f;
			}

			params.Set_Movement(this->cannonDestination, 0.25f, 3.0f);
			params.Set_Attack(this->cannonAttackLocations[static_cast<int>(this->attackLocationIndex)], 500.0f, this->cannonAttackError, true);

			Commands->Action_Attack(obj, params);
		}
		else if (this->areaIndex == 3)
		{
			if (this->currentArea == 1)
			{
				params.Set_Movement(this->cannonStart, 0.5f, 3.0f);
			}
			else if (!this->currentArea)
			{
				params.Set_Movement(this->villageStart, 0.5f, 3.0f);

				if (!this->field_DF)
				{
					this->field_DF = true;
				}
			}

			Commands->Action_Goto(obj, params);
		}
		else if (this->areaIndex == 4)
		{
			Commands->Action_Reset(obj, 100.0f);

			if (this->currentArea == 1)
			{
				params.Set_Movement(this->cannonStart, 0.5f, 3.0f);
			}
			else if(!this->currentArea)
			{
				params.Set_Movement(this->villageStart, 0.5f, 3.0f);

				if (!this->field_DF)
				{
					this->field_DF = true;
				}
			}

			Commands->Action_Goto(obj, params);
		}

		Commands->Start_Timer(obj, this, 1.5f, 1);
	}
}

ScriptRegistrant<M03_Gunboat_Controller_RMV> M03_Gunboat_Controller_RMVRegistrant("M03_Gunboat_Controller_RMV", "Receive_Type:int, Receive_Param_For_Village:int, Receive_Param_For_Cannon:int, Beach_Destination:vector3, Village_Start:vector3, Village_Destination:vector3, Cannon_Start:vector3, Cannon_Destination:Vector3, Receive_Param_Destroy:int");