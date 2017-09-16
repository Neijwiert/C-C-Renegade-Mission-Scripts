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
#include "M02_Nod_Soldier.h"

void M02_Nod_Soldier::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->gotoLocation, sizeof(this->gotoLocation), 1);
	Auto_Save_Variable(&this->delayedInitialized, sizeof(this->delayedInitialized), 2);
	Auto_Save_Variable(&this->animationIndex, sizeof(this->animationIndex), 3);
	Auto_Save_Variable(&this->damaged, sizeof(this->damaged), 4);
	Auto_Save_Variable(&this->health, sizeof(this->health), 5);
	Auto_Save_Variable(&this->enemySeen, sizeof(this->enemySeen), 6);
}

// TODO
void M02_Nod_Soldier::Created(GameObject *obj)
{
	this->animationIndex = 0;
	this->delayedInitialized = false;
	this->enemySeen = false;
	this->damaged = false;
	this->health = Commands->Get_Health(obj);

	Vector3 pos = Commands->Get_Position(obj);
	Commands->Set_Innate_Soldier_Home_Location(obj, pos, 20.0f);
	Commands->Enable_Hibernation(obj, false);

	Commands->Start_Timer(obj, this, 1.0f, 1);

	int prePlaced = Get_Int_Parameter("Pre_Placed");
	if (prePlaced)
	{
		Commands->Innate_Disable(obj);
	}
	else
	{
		Commands->Grant_Key(obj, 1, true);

		GameObject *starObj = Commands->Get_A_Star(pos);
		if (starObj)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 71.0f, 0);
			params.Set_Movement(starObj, 0.8f, 5.0f);

			Commands->Action_Goto(obj, params);
		}
	}
}

void M02_Nod_Soldier::Killed(GameObject *obj, GameObject *killer)
{
	int areaNumber = Get_Int_Parameter("Area_Number");
	
	GameObject *M02ObjectiveControllerObj = Commands->Find_Object(1111112);
	if (M02ObjectiveControllerObj)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 101, areaNumber, 0.0f);
	}

	if (areaNumber == 3)
	{
		if (Get_Int_Parameter("Area_Officer"))
		{
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 111, 2, 0.0f);
		}
	}
}

void M02_Nod_Soldier::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (!this->damaged && !damager)
	{
		this->damaged = true;

		Commands->Set_Health(obj, this->health);
	}
}

// TODO
void M02_Nod_Soldier::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (Commands->Get_ID(obj) == 400276) // Power plant nod officer
	{
		Commands->Start_Timer(obj, this, 2.0f, 10);
	}

	if (type == CUSTOM_EVENT_FALLING_DAMAGE)
	{
		if (!this->damaged)
		{
			this->health = Commands->Get_Max_Health(obj);
		}
	}
	else if (type == 99)
	{
		if (!Commands->Is_Performing_Pathfind_Action(obj))
		{
			if (param == 202)
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 50.0f, 214);
				params.Set_Movement(Vector3(1113.61f, 877.4f, 17.15f), 0.8f, 10.0f);
				params.Set_Attack(Vector3(1113.61f, 877.4f, 17.15f), 10.0f, 0.0f, false);
				
				Commands->Action_Attack(obj, params);
			}
			else if (param == 214)
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 71.0f, 214);
				params.Set_Movement(sender, 0.8f, 10.0f);

				Vector3 senderPos = Commands->Get_Position(sender);
				senderPos.Z -= 1.0f;

				params.Set_Attack(senderPos, 10.0f, 0.0f, false);
				
				Commands->Action_Attack(obj, params);
			}
			else if (param == 217)
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 50.0f, 214);
				params.Set_Movement(Vector3(1062.24f, 978.38f, -16.85f), 0.8f, 10.0f);
				params.Set_Attack(Vector3(1062.24f, 978.38f, -16.85f), 10.0f, 0.0f, false);

				Commands->Action_Attack(obj, params);
			}
		}
	}
	else if (!type)
	{
		if (!param)
		{
			Commands->Innate_Enable(obj);

			int prePlaced = Get_Int_Parameter("Pre_Placed");
			if (prePlaced == 1)
			{
				Commands->Start_Timer(obj, this, 10.0f, 5);
			}
			else if (prePlaced == 2)
			{
				Commands->Get_ID(obj); // A random call to Get_ID here for some reason

				Commands->Start_Timer(obj, this, 1.0f, 3);
			}
			else if (prePlaced == 6)
			{
				Commands->Start_Timer(obj, this, 1.0f, 7);
			}
			else if(prePlaced == 5)
			{
				if (Get_Int_Parameter("Area_Number") == 19)
				{
					ActionParamsStruct params;
					params.Set_Basic(this, 31.0f, 100);
					params.Set_Animation("H_A_CON2", true); // TODO

					Commands->Action_Play_Animation(obj, params);
				}
			}
		}
	}
}

void M02_Nod_Soldier::Sound_Heard(GameObject *obj, const CombatSound & sound)
{
	if (sound.sound == 1000)
	{
		if (sound.Creator)
		{
			Commands->Send_Custom_Event(obj, sound.Creator, 100, 1, 0.0f);
		}
	}
	else
	{
		if (!this->delayedInitialized && sound.sound >= 1050 && sound.sound < 1100)
		{
			int areaNumberSound = sound.sound - 1050;
			if (Get_Int_Parameter("Area_Number") == areaNumberSound)
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 99.0f, 0);
				params.Set_Movement(this->gotoLocation, 0.8f, 20.0f);
				
				Commands->Modify_Action(obj, 0, params, true, false);
			}
		}
	}
}

void M02_Nod_Soldier::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	this->enemySeen = true;
}

void M02_Nod_Soldier::Timer_Expired(GameObject *obj, int number)
{
	// Triggered 1 second after creation
	if (number == 1)
	{
		int areaOfficer = Get_Int_Parameter("Area_Officer");
		int prePlaced = Get_Int_Parameter("Pre_Placed");
		int areaNumber = Get_Int_Parameter("Area_Number");

		GameObject *M02ObjectiveControllerObj = Commands->Find_Object(1111112);
		if (!M02ObjectiveControllerObj)
		{
			return;
		}

		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 103, areaNumber, 0.0f);
		if (areaOfficer)
		{
			Commands->Set_Innate_Take_Cover_Probability(obj, 100.0f);
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 106, areaNumber, 0.0f);

			Commands->Enable_Hibernation(obj, true);
		}
		else
		{
			if (prePlaced == 2)
			{
				if (areaNumber == 99)
				{
					Commands->Start_Timer(obj, this, 5.0f, 6);

					Commands->Enable_Hibernation(obj, true);
				}
			}
			else if (prePlaced == 4)
			{
				Commands->Attach_Script(obj, "M00_Soldier_Powerup_Disable", "");

				if (!areaNumber || areaNumber == 3 || areaNumber == 6 || areaNumber == 10 || areaNumber == 16 || areaNumber == 21)
				{
					this->delayedInitialized = true;

					Commands->Start_Timer(obj, this, 15.0f, 4);
				}

				Commands->Enable_Hibernation(obj, true);
			}
			else if (!prePlaced)
			{
				Commands->Start_Timer(obj, this, 15.0f, 2);
			}
		}
	}
	else if (number == 2)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		if (starObj)
		{
			Vector3 starPos = Commands->Get_Position(starObj);
			if (Commands->Get_Distance(pos, starPos) <= 70.0f)
			{
				if (!Commands->Is_Performing_Pathfind_Action(obj))
				{
					ActionParamsStruct params;
					params.Set_Basic(this, 90.0f, 0);
					params.Set_Movement(starObj, 0.8f, 10.0f);
					params.Set_Attack(starObj, 150.0f, 0.0f, true);

					Commands->Action_Attack(obj, params);
				}
			}
			else
			{
				if (!Commands->Is_Object_Visible(starObj, obj))
				{
					Commands->Apply_Damage(obj, 10000.0f, "Blamokiller", NULL);
				}
				else
				{
					if (!Commands->Is_Performing_Pathfind_Action(obj))
					{
						ActionParamsStruct params;
						params.Set_Basic(this, 90.0f, 0);
						params.Set_Movement(starObj, 0.8f, 10.0f);
						params.Set_Attack(starObj, 150.0f, 0.0f, true);

						Commands->Action_Attack(obj, params);
					}
				}
			}
		}

		Commands->Start_Timer(obj, this, 5.0f, 2);
	}
	else if (number == 3)
	{
		if (!Commands->Is_Performing_Pathfind_Action(obj))
		{
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			if (starObj)
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 90.0f, 0);
				params.Set_Movement(starObj, 0.8f, 10.0f);
				params.Set_Attack(starObj, 150.0f, 0.0f, true);

				Commands->Action_Attack(obj, params);
			}
		}

		Commands->Start_Timer(obj, this, 15.0, 3);
	}
	else if (number == 4)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		if (starObj)
		{
			Vector3 starPos = Commands->Get_Position(starObj);
			if (Commands->Get_Distance(pos, starPos) < 150.0f)
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 90.0f, 0);
				params.Set_Attack(starObj, 150.0f, 0.0f, true);

				Commands->Action_Attack(obj, params);
			}
		}

		Commands->Start_Timer(obj, this, 15.0, 4);
	}
	else if (number == 5)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);
		Vector3 starPos = Commands->Get_Position(starObj);

		if (Commands->Get_Distance(pos, starPos) > 10.0f && !Commands->Is_Performing_Pathfind_Action(obj))
		{
			if (++this->animationIndex > 2)
			{
				this->animationIndex = 0;
			}

			if (this->animationIndex == 0)
			{
				GameObject *closestSoldierObj = Commands->Find_Closest_Soldier(pos, 1.0f, 50.0f, true);
				if (closestSoldierObj && closestSoldierObj != starObj)
				{
					ActionParamsStruct params;
					params.Set_Basic(this, 99.0f, 0);
					params.Set_Animation("H_A_J18C", false); // TODO
					params.Set_Face_Location(starPos, 4.0f);

					Commands->Action_Play_Animation(obj, params);
				}
			}
			else if (this->animationIndex == 1)
			{
				if (starObj)
				{
					ActionParamsStruct params;
					params.Set_Basic(this, 99.0f, 0);
					params.Set_Animation("H_A_J23C", false); // TODO
					params.Set_Face_Location(starPos, 4.0f);

					Commands->Action_Play_Animation(obj, params);
				}
			}
			else if (this->animationIndex == 2) // TODO
			{
				if (starObj)
				{
					ActionParamsStruct params;
					params.Set_Basic(this, 99.0f, 0);
					params.Set_Animation("H_A_J27C", false);
					params.Set_Face_Location(starPos, 4.0f);

					Commands->Action_Play_Animation(obj, params);
				}
			}
		}

		Commands->Start_Timer(obj, this, 10.0f, 5);
	}
	else if (number == 6)
	{
		Commands->Apply_Damage(obj, 10000.0f, "Blamokiller", NULL);
	}
	else if (number == 7)
	{
		if (this->enemySeen)
		{
			if (!Commands->Is_Performing_Pathfind_Action(obj))
			{
				Vector3 pos = Commands->Get_Position(obj);
				GameObject *starObj = Commands->Get_A_Star(pos);

				if (starObj)
				{
					ActionParamsStruct params;
					params.Set_Basic(this, 90.0f, 0);
					params.Set_Movement(starObj, 0.8f, 10.0f);
					params.Set_Attack(starObj, 150.0f, 0.0f, true);

					Commands->Action_Attack(obj, params);
				}
			}
		}
		else
		{
			float rand = Commands->Get_Random(0.0f, 1.9999f);
			if (rand > 1.0f)
			{
				rand = 1.0f;
			}

			int multipliedRand = static_cast<int>(5.0f * rand); // TODO (Does this actually make sense? Is it cast to an int right here?)

			Vector3 moveLocation;
			if (multipliedRand == 1)
			{
				moveLocation = Vector3(1340.65f, 655.04f, 19.51f);
			}
			else if (multipliedRand == 2)
			{
				moveLocation = Vector3(1310.67f, 694.74f, 19.71f);
			}
			else if (multipliedRand > 2)
			{
				moveLocation = Vector3(1242.26f, 646.04f, 19.65f);
			}
			else if (!multipliedRand)
			{
				moveLocation = Vector3(1301.55f, 617.89f, 19.66f);
			}

			ActionParamsStruct params;
			params.Set_Basic(this, 31.0f, 0);
			params.Set_Movement(moveLocation, 0.3f, 10.0f);

			Commands->Action_Goto(obj, params);
		}

		Commands->Start_Timer(obj, this, 30.0f, 7);
	}
	else if (number == 10)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);
		Vector3 starPos = Commands->Get_Position(starObj);

		if (Commands->Get_Distance(pos, starPos) >= 20.0f)
		{
			Commands->Start_Timer(obj, this, 2.0f, 10);
		}
		else
		{
			int conversationId = Commands->Create_Conversation("MX2DSGN_DSGN0014", 100, 300.0f, true); // Intruder! Stop him!
			Commands->Join_Conversation(NULL, conversationId, true, true, true);
			Commands->Join_Conversation(starObj, conversationId, true, false, false);
			Commands->Start_Conversation(conversationId, 0);

			Commands->Start_Timer(obj, this, 3.0f, 11);
		}
	}
	else if (number == 11)
	{
		int conversationId = Commands->Create_Conversation("MX2DSGN_DSGN0015", 100, 300.0f, true); // This is Engineering - what's going on up there? <radioized>
		Commands->Join_Conversation(NULL, conversationId, true, true, true);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, false, false);
		Commands->Start_Conversation(conversationId, 0);
	}
}

ScriptRegistrant<M02_Nod_Soldier> M02_Nod_SoldierRegistrant("M02_Nod_Soldier", "Area_Number:int,Area_Officer:int,Pre_Placed:int");