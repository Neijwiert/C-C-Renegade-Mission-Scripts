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

/*
M02 -> 401195 401194 401193 401192 401191 401190 401189 401122 401121 401120 401119 401118 401117 401116 401115 401097 401096 401095 401094 401089 401088 401087 401086 401085 401069 401068 401067 401053 401052 401051 401050 401049 401047 401046 400510 400505 400503 400453 400451 400415 400413 400411 400410 400345 400344 400319 400318 400317 400293 400292 400287 400286 400285 400213 400212 400211 400210 400209 400208 400207 400199 400200 400206 400275 400276 400277 400372 400373 400374 400553 400554 400555 400556 400557 400558 400559 400560 400977 400978 400979 400980 400981 400982 400983 400984 401002 401003 401045 401056 401057 401058 401060 401063 401064 401065 401071 401072 401073 401074 401075 401076 401077 401078 401103 401104 401105 401106 401107 401108 401109 401110 401112 401124 401125 401126 401127 401129 401132 401133 401134 401135 401136 401137 401138 401139 401141 401142 401143 401144 401145
The following cineamtic files: x2i_para03_area00/01/02/03/04/06/09/21.txt, x2i_drop03_area00/01/02/03/04/06/09/10/11/21.txt, x2i_drop02_area00/01/02/03/04/06/09/10/11/21.txt
When M02_Respawn_Controller::Use_Spawners is called
*/
void M02_Nod_Soldier::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->gotoLocation, sizeof(this->gotoLocation), 1);
	Auto_Save_Variable(&this->delayedInitialized, sizeof(this->delayedInitialized), 2);
	Auto_Save_Variable(&this->animationIndex, sizeof(this->animationIndex), 3);
	Auto_Save_Variable(&this->damaged, sizeof(this->damaged), 4);
	Auto_Save_Variable(&this->health, sizeof(this->health), 5);
	Auto_Save_Variable(&this->enemySeen, sizeof(this->enemySeen), 6);
}

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

	// Received from M02_Destroy_Objective when damaged (param = 214)
	// Received from M02_Obelisk when damaged (param = 202)
	// Received from M02_Power_Plant when damaged (param = 217)
	else if (type == 99)
	{
		if (!Commands->Is_Performing_Pathfind_Action(obj))
		{
			// M02_Obelisk
			if (param == 202)
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 50.0f, 214);
				params.Set_Movement(Vector3(1113.61f, 877.4f, 17.15f), 0.8f, 10.0f);
				params.Set_Attack(Vector3(1113.61f, 877.4f, 17.15f), 10.0f, 0.0f, false);
				
				Commands->Action_Attack(obj, params);
			}

			// M02_Destroy_Objective
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

			// M02_Power_Plant
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

	// Received from M02_Objective_Zone with objId = 
	// (401113, 401079, 401101, 401102, 401080, 401070, 401066, 401196, 401130, 401131, 401123, 401114, 400270, 
	// 400267, 400269, 400268, 400501, 400316, 400502, 400272, 400273, 400271, 400187, 400184, 400186, 400185, 400189, 400188)
	// Received from M02_Objective_Zone when timer number 9 triggered
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
	// Received from M02_Objective_Zone
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

	// After 15 seconds after timer number 1 triggered or 5 seconds after this block
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

	// Triggerd 1 second after custom type 0 param 0 received or 15 seconds after this block
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

	// Triggered 15 seconds after timer number 1 or 15 seconds after this block
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

	// Triggered 10 seconds after custom type 0 param 0 or 10 seconds after this block
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

	// Triggerd 5 seconds after timer number 1 triggered
	else if (number == 6)
	{
		Commands->Apply_Damage(obj, 10000.0f, "Blamokiller", NULL);
	}

	// Triggerd 1 second after custom type 0 param 0 or 30 seconds after this block
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

	// Triggerd 2 seconds after receiving any custom when we are the nod power plant officer or 2 seconds after this block
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

	// Triggered 3 seconds after timer number 10 triggered
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