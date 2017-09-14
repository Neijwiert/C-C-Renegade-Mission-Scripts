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
#include "M02_Mendoza.h"

void M02_Mendoza::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->killed, sizeof(this->killed), 1);
	Auto_Save_Variable(&this->health, sizeof(this->health), 2);
	Auto_Save_Variable(&this->shieldStrength, sizeof(this->shieldStrength), 3);
	Auto_Save_Variable(&this->tauntIndex, sizeof(this->tauntIndex), 4);
	Auto_Save_Variable(&this->didMoveToRandomLocation, sizeof(this->didMoveToRandomLocation), 5);
}

// When timer number 9 triggers on M02_Objective_Zone
void M02_Mendoza::Created(GameObject *obj)
{
	this->health = Commands->Get_Health(obj);
	this->shieldStrength = Commands->Get_Shield_Strength(obj);
	this->killed = false;
	this->didMoveToRandomLocation = false;
	this->tauntIndex = 0;

	Commands->Innate_Disable(obj);

	ActionParamsStruct params;
	params.Set_Basic(this, 98.0f, 3);

	Vector3 pos = Commands->Get_Position(obj);
	GameObject *starObj = Commands->Get_A_Star(pos);

	params.Set_Attack(starObj, 300.0f, 0.0f, true);

	Commands->Action_Attack(obj, params);

	Commands->Start_Timer(obj, this, 2.0f, 1);
}

void M02_Mendoza::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	Commands->Set_Health(obj, this->health);

	this->shieldStrength -= 2.0f;
	Commands->Set_Shield_Strength(obj, this->shieldStrength);

	if (this->shieldStrength < 1.0f && !this->killed)
	{
		this->killed = true;

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 1);
		params.Set_Movement(Vector3(1260.01f, 535.46f, 18.428f), 0.8f, 5.0f);

		Commands->Action_Goto(obj, params);
	}
}

void M02_Mendoza::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered when we moved to the flee position, see damaged event
	if (action_id == 1)
	{
		Commands->Innate_Disable(obj);
		
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *invisObj = Commands->Create_Object("Invisible_Object", pos);

		if (invisObj)
		{
			Commands->Attach_Script(invisObj, "Test_Cinematic", "XG_ROPE_EVAC_F.txt");

			// I will finish you later, Maggot! <yelling from the helicopter>
			// Forget him, Havoc - you'll have your chance. 
			// You can finish off what's left of the base, but you still need to get on that cargo plane!
			int conversationId = Commands->Create_Conversation("MX2DSGN_DSGN0025", 100, 300.0f, true);
			Commands->Join_Conversation(obj, conversationId, true, false, false);

			GameObject *starObj = Commands->Get_A_Star(pos);

			Commands->Join_Conversation(starObj, conversationId, true, false, false);
			Commands->Join_Conversation(NULL, conversationId, true, true, true);
			Commands->Start_Conversation(conversationId, 0);
		}
	}

	// Triggered when we moved to a random location, see timer_expired with number 1
	else if (action_id == 2)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 98.0f, 3);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		params.Set_Attack(starObj, 300.0f, 0, true);

		Commands->Action_Attack(obj, params);
	}
}

void M02_Mendoza::Timer_Expired(GameObject *obj, int number)
{
	// Triggered after 2 seconds on creation or 7 seconds after this block
	if (number == 1 && !this->killed)
	{
		// You there! I will teach you a lesson!
		Commands->Create_Conversation("MX2DSGN_DSGN0019", 100, 300.0f, true); // Does not seem to be used, just a call to Create_Conversation

		if (++this->tauntIndex > 6)
		{
			this->tauntIndex = 1;
		}

		const char *conversationName;
		switch (this->tauntIndex)
		{
			case 1: 
				conversationName = "MX2DSGN_DSGN0019"; // You there! I will teach you a lesson!

				break;
			case 2:
				conversationName = "MX2DSGN_DSGN0020"; // You will pay for your intrusion, infidel!

				break;
			case 3:
				conversationName = "MX2DSGN_DSGN0021"; // Ha-Ha-Ha-Ha - you can't face me, can you!?

				break;
			case 4:
				conversationName = "MX2DSGN_DSGN0022"; // I stand before you now - kill me if you can!

				break;
			case 5:
				conversationName = "MX2DSGN_DSGN0023"; // I don't think you have what it takes, soldier boy

				break;
			default:
				conversationName = "MX2DSGN_DSGN0024"; // Ha-Ha-Ha-Ha

				break;
		}

		int conversationId = Commands->Create_Conversation(conversationName, 100, 300.0f, true);
		Commands->Join_Conversation(obj, conversationId, true, false, false);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, false, false);
		Commands->Start_Conversation(conversationId, 0);

		Commands->Start_Timer(obj, this, 7.0f, 1);

		if (this->didMoveToRandomLocation)
		{
			this->didMoveToRandomLocation = false;
		}
		else
		{
			this->didMoveToRandomLocation = true;

			Commands->Action_Reset(obj, 100.0f);

			ActionParamsStruct params;
			params.Set_Basic(this, 99.0f, 2);

			int rand = static_cast<int>(Commands->Get_Random(0.0f, 3.9999001f));
			if (rand > 3)
			{
				rand = 3;
			}

			Vector3 moveLocation;
			switch (rand)
			{
			case 0:
				moveLocation = Vector3(1248.59f, 568.431f, 17.396f);

				break;
			case 1:
				moveLocation = Vector3(1264.68f, 564.459f, 17.816f);

				break;
			case 2:
				moveLocation = Vector3(1270.22f, 541.406f, 17.978f);

				break;
			case 3:
				moveLocation = Vector3(1251.21f, 544.658f, 17.854f);

				break;
			default:
				moveLocation = Vector3(1260.01f, 535.46f, 18.428f);

				break;
			}

			params.Set_Movement(moveLocation, 0.8f, 5.0f);

			Commands->Action_Goto(obj, params);
		}
	}
}

ScriptRegistrant<M02_Mendoza> M02_MendozaRegistrant("M02_Mendoza", "");