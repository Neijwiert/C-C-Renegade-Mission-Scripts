/*
 * A Command & Conquer: Renegade SSGM Plugin, containing all the M13 (or M00) mission scripts
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
#include "MX0_MissionStart_DME.h"

void MX0_MissionStart_DME::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 2);
	Auto_Save_Variable(&this->field_24, sizeof(this->field_24), 3);
	Auto_Save_Variable(&this->field_28, sizeof(this->field_28), 4);
	Auto_Save_Variable(&this->sniper1ObjId, sizeof(this->sniper1ObjId), 5);
	Auto_Save_Variable(&this->sniper2ObjId, sizeof(this->sniper2ObjId), 6);
	Auto_Save_Variable(&this->field_34, sizeof(this->field_34), 7);
	Auto_Save_Variable(&this->field_38, sizeof(this->field_38), 8);
}

void MX0_MissionStart_DME::Created(GameObject *obj)
{
	// Deselect weapon of the star
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *starObj = Commands->Get_A_Star(pos);
	Commands->Select_Weapon(starObj, NULL);

	// Initialize member variables
	this->field_34 = 0;
	this->sniper2ObjId = 0;
	this->sniper1ObjId = 0;
	this->field_28 = 0;
	this->field_24 = 0;
	this->field_20 = 0;
	this->field_1C = 0;
	this->field_38 = 0;

	// Fade into intro music
	Commands->Fade_Background_Music("renegade_intro_no_vox.mp3", 0, 1);

	// Start intro cinematic
	GameObject *cinObj = Commands->Create_Object("Invisible_Object", Vector3(0, 0, 0));
	Commands->Attach_Script(cinObj, "Test_Cinematic", "X00_Intro.txt");
}

void MX0_MissionStart_DME::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (action_id == 100001)
	{
		Commands->Start_Timer(obj, this, 2.0f, 119);
	}
	else if (action_id == 100002)
	{
		Commands->Start_Timer(obj, this, 5.0f, 120);
	}
	else if (action_id == 100004)
	{
		GameObject *scriptZoneObj = Commands->Find_Object(1200015);
		Commands->Send_Custom_Event(obj, scriptZoneObj, 110, 0, 0.0f);
	}
}

void MX0_MissionStart_DME::Timer_Expired(GameObject *obj, int number)
{
	if (number == 142)
	{
		// Create conversation MX0_ENGINEER1_048: 
		// - We�ll hang tight until the coast is clear
		int conversationId = Commands->Create_Conversation("MX0_ENGINEER1_048", 97, 2000.0f, false);
		GameObject *field_1CObject = Commands->Find_Object(this->field_1C);
		Commands->Join_Conversation(field_1CObject, conversationId, false, true, true);

		// Let the star join the conversation
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);
		Commands->Join_Conversation(starObj, conversationId, true, true, true);

		// Start and monitor the conversation
		Commands->Start_Conversation(conversationId, 100048);
		Commands->Monitor_Conversation(obj, conversationId);
	}
	else if (number == 136)
	{
		if (Commands->Find_Object(this->sniper1ObjId) != NULL) // Is sniper 1 still alive?
		{
			GameObject *field_1CObj = Commands->Find_Object(this->field_1C);
			Commands->Send_Custom_Event(obj, field_1CObj, 136, this->sniper1ObjId, 0.0f);

			GameObject *field_20Obj = Commands->Find_Object(this->field_20);
			Commands->Send_Custom_Event(obj, field_20Obj, 136, sniper1ObjId, 0.0f);

			Commands->Start_Timer(obj, this, 3.0f, 136);
		}
	}
	else if (number == 134)
	{
		GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
		Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 202, 0, 0.0f);
	}
	else if (number == 133)
	{	
		// Get the daves arrow position for sniper 1 (the one on the small rock)
		GameObject *sniper1PosObj = Commands->Find_Object(1200043);
		Vector3 sniper1Pos = Commands->Get_Position(sniper1PosObj);

		// Get the daves arrow position for sniper 2 (the one on the cliff side)
		GameObject *sniper2PosObj = Commands->Find_Object(1200044);
		Vector3 sniper2Pos = Commands->Get_Position(sniper2PosObj);

		// Create sniper 1, attach scripts and store its id
		GameObject *sniper1Obj = Commands->Create_Object("NOD_Minigunner_2SF", sniper1Pos);
		Commands->Attach_Script(sniper1Obj, "M00_Soldier_Powerup_Disable", NULL);
		Commands->Attach_Script(sniper1Obj, "M00_Send_Object_ID", "1200001, 1, 0.0f");
		Commands->Attach_Script(sniper1Obj, "MX0_SniperAction", "1200045");
		Commands->Attach_Script(sniper1Obj, "MX0_KillNotify", NULL);
		this->sniper1ObjId = Commands->Get_ID(sniper1Obj);

		// Create sniper 2, attach scripts and store its id
		GameObject *sniper2Obj = Commands->Create_Object("NOD_Minigunner_2SF", sniper2Pos);
		Commands->Attach_Script(sniper2Obj, "M00_Soldier_Powerup_Disable", NULL);
		Commands->Attach_Script(sniper2Obj, "M00_Send_Object_ID", "1200001, 2, 0.0f");
		Commands->Attach_Script(sniper2Obj, "MX0_SniperAction", "1200045");
		this->sniper2ObjId = Commands->Get_ID(sniper2Obj);
	}
	else if (number == 124)
	{
		if (Commands->Find_Object(this->sniper1ObjId) != NULL) // Is sniper 1 still alive?
		{
			Commands->Send_Custom_Event(obj, obj, 100007, 0, 0.0f);
		}
	}
	else if (number == 119)
	{
		Commands->Send_Custom_Event(obj, obj, 100002, 0, 0.0f);
	}
	else if (number == 120)
	{
		if (this->field_28 <= 1)
		{
			Commands->Send_Custom_Event(obj, obj, 100003, 0, 0.0f);
			Commands->Start_Timer(obj, this, 5.0f, 120);
		}
	}
	else if (number == 127)
	{	
		Commands->Set_HUD_Help_Text(8576, Vector3(0.196f, 0.882f, 0.196f)); // MX0_HELPTEXT_01: Move the Mouse to Look Around
	}
	else if (number == 128)
	{
		Commands->Set_HUD_Help_Text(8577, Vector3(0.196f, 0.882f, 0.196f)); // MX0_HELPTEXT_02: Use W, S, A and D keys to Move (Default)
	}
	else if (number == 118)
	{
		Commands->Set_HUD_Help_Text(8578, Vector3(0.196f, 0.882f, 0.196f)); // MX0_HELPTEXT_03: Press F1 for Help
	}
	else if (number == 117)
	{
		// Start the background music
		Commands->Set_Background_Music("Level0_pt1_music.mp3");

		// Get the star
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		//Give weapons, select autorifle and attach script 
		Commands->Give_PowerUp(starObj, "POW_AutoRifle_Player", false);
		Commands->Give_PowerUp(starObj, "POW_AutoRifle_Player", false);
		Commands->Select_Weapon(starObj, "Weapon_AutoRifle_Player");
		Commands->Attach_Script(starObj, "Havoc_Script", NULL);
	}
}

void MX0_MissionStart_DME::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 138)
	{
		if (!this->field_38)
		{
			// Create conversation MX0CON018:
			// - There we go!
			int conversationId = ::Commands->Create_Conversation("MX0CON018", 97, 2000.0f, 0);

			GameObject *field_1CObj = Commands->Find_Object(this->field_1C);
			Commands->Join_Conversation(field_1CObj, conversationId, false, true, true);

			// Find the star and join to conversation
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);
			Commands->Join_Conversation(starObj, conversationId, true, true, true);

			// Start conversation and monitor it
			Commands->Start_Conversation(conversationId, 100018);
			Commands->Monitor_Conversation(obj, conversationId);

			Commands->Start_Timer(obj, this, 3.0f, 142);
		}
	}
	else if (type == 136)
	{
		Commands->Start_Timer(obj, this, 2.0f, 136);
	}
	else if (type == 114)
	{
		GameObject *sniper1Obj = Commands->Find_Object(this->sniper1ObjId);
		Commands->Send_Custom_Event(obj, sniper1Obj, 114, 0, 0.0f);

		GameObject *sniper2Obj = Commands->Find_Object(this->sniper2ObjId);
		Commands->Send_Custom_Event(obj, sniper2Obj, 114, 0, 0.0f);
	}
	else if (type == 223)
	{
		Commands->Send_Custom_Event(obj, sender, 223, this->sniper1ObjId, 0.0f);
	}
	else if (type == 224)
	{
		Commands->Send_Custom_Event(obj, sender, 224, this->sniper2ObjId, 0.0f);
	}
	else if (type == 9035)
	{
		if (param == 1)
		{
			this->sniper1ObjId = Commands->Get_ID(sender);
		}
		else if (param == 2)
		{
			this->sniper2ObjId = Commands->Get_ID(sender);
			Commands->Start_Timer(obj, this, 2.0f, 134);
		}
	}
	else if (type == 99)
	{
		this->field_28 = 0;

		// Create blocker1 at the rock pile in the beginning and disable rendering
		GameObject *blocker1Obj = Commands->Create_Object("Large_Blocker", Vector3(-148.071f, -31.267f, -0.306f));
		Commands->Set_Facing(blocker1Obj, -30.0f);
		Commands->Set_Is_Rendered(blocker1Obj, false);

		// Create blocker2 at the rock pile in the beginning and disable rendering
		GameObject *blocker2Obj = Commands->Create_Object("Large_Blocker", Vector3(-148.234f, -31.542f, 1.357f));
		Commands->Set_Facing(blocker2Obj, -30.0f);
		Commands->Set_Is_Rendered(blocker2Obj, false);

		Commands->Start_Timer(obj, this, 0.5f, 117);
		Commands->Start_Timer(obj, this, 3.0f, 127);
		Commands->Start_Timer(obj, this, 6.0f, 128);
		Commands->Start_Timer(obj, this, 9.0f, 118);
		Commands->Start_Timer(obj, this, 1.0f, 133);
	}
	else if (type == 130)
	{
		GameObject *field_1CObj_1 = Commands->Find_Object(this->field_1C);
		Commands->Send_Custom_Event(obj, field_1CObj_1, 131, 0, 0.0f);

		GameObject *field_20Obj = Commands->Find_Object(this->field_20);
		Commands->Send_Custom_Event(obj, field_20Obj, 131, 0, 0.0f);
	}
	else if (type == 132)
	{
		GameObject *field_1CObj_2 = Commands->Find_Object(this->field_1C);
		Commands->Send_Custom_Event(obj, field_1CObj_2, 132, 0, 0.0f);
	}
	else if (type == 111)
	{
		GameObject *field_1CObj_3 = Commands->Find_Object(this->field_1C);
		Commands->Send_Custom_Event(obj, field_1CObj_3, 112, 0, 0.0f);

		GameObject *field_20Obj_1 = Commands->Find_Object(this->field_20);
		Commands->Send_Custom_Event(obj, field_20Obj_1, 112, 0, 0.0f);
	}
	else if (type == 102)
	{
		this->field_28 = param;

		GameObject *field_1CObj_4 = Commands->Find_Object(this->field_1C);
		Commands->Send_Custom_Event(obj, field_1CObj_4, 108, this->field_28, 0.0f);

		GameObject *field_20Obj_2 = ::Commands->Find_Object(this->field_20);
		Commands->Send_Custom_Event(obj, field_20Obj_2, 108, this->field_28, 0.0f);
	}
	else if (type == 103)
	{
		if (this->field_24)
		{
			this->field_20 = param;
		}
		else
		{
			this->field_24 = 1;
			this->field_1C = param;
		}
	}
	else if (type == 104)
	{
		if (param == 1)
		{
			Commands->Send_Custom_Event(obj, sender, 105, this->field_1C, 0.0f);
		}
		else if (param == 2)
		{
			Commands->Send_Custom_Event(obj, sender, 106, this->field_20, 0.0f);
		}
	}
	else if (type == 100001)
	{
		// Create conversation MX0CON001:
		// - Clear!
		int conversationId = Commands->Create_Conversation("MX0CON001", 99, 2000.0f, false);

		GameObject *field_20Obj_3 = Commands->Find_Object(this->field_20);
		Commands->Join_Conversation(field_20Obj_3, conversationId, false, true, true);

		// Start conversation and monitor it
		Commands->Start_Conversation(conversationId, 100001);
		Commands->Monitor_Conversation(obj, conversationId);
	}
	else if (type == 100002)
	{
		// Create conversation MX0CON002:
		// - It sounds like they're still in trouble, Sir!
		// - Saddle up, let's move.
		int conversationId = Commands->Create_Conversation("MX0CON002", 97, 2000.0f, false);

		GameObject *field1CObj = Commands->Find_Object(this->field_1C);
		Commands->Join_Conversation(field1CObj, conversationId, false, true, true);

		// Find the star and its id
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);
		int starId = Commands->Get_ID(starObj);

		Commands->Join_Conversation_Facing(field1CObj, conversationId, starId);

		GameObject *field20Obj = Commands->Find_Object(this->field_20);
		Commands->Join_Conversation(field20Obj, conversationId, false, true, true);
		Commands->Join_Conversation_Facing(field20Obj, conversationId, starId);
	
		// Join the star with the conversation
		Commands->Join_Conversation(starObj, conversationId, true, true, true);

		// Start conversation and monitor it
		Commands->Start_Conversation(conversationId, 100002);
		Commands->Monitor_Conversation(obj, conversationId);
	}
	else if (type == 100003)
	{
		static const char * const ENGINEER_WAIT_FOR_HAVOC_CONVERSATIONS[] =
		{
			// - Let's go, Havoc!
			"MX0CON003",

			// - Let's go!
			"MX0CON003alt1",

			// - This way, Havoc!
			"MX0CON003alt2",

			// - Over here, Sir!
			"MX0CON003alt3"
		};

		// Get random int and start the conversation
		int randomInt = Commands->Get_Random_Int(0, 4);
		int conversationId = Commands->Create_Conversation(ENGINEER_WAIT_FOR_HAVOC_CONVERSATIONS[randomInt], 97, 2000.0f, false);
	
		GameObject *field20Obj = Commands->Find_Object(this->field_20);
		Commands->Join_Conversation(field20Obj, conversationId, false, true, true);
		
		// Find the star and its id
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);
		int starId = Commands->Get_ID(starObj);

		Commands->Join_Conversation_Facing(field20Obj, conversationId, starId);
		
		// Join the star with the conversation
		Commands->Join_Conversation(starObj, conversationId, true, true, true);
		
		// Start conversation and monitor it
		Commands->Start_Conversation(conversationId, 100003);
		Commands->Monitor_Conversation(obj, conversationId);
	}
	else if (type == 100006)
	{
		if (Commands->Find_Object(this->sniper1ObjId) != NULL)
		{
			GameObject *field20Obj = Commands->Find_Object(this->field_20);
			Commands->Send_Custom_Event(obj, field20Obj, 125, 0, 0.0f);

			Commands->Start_Timer(obj, this, 5.0f, 124);

			// Create conversation MX0CON006:
			// - Up there - snipers!
			int conversationId = Commands->Create_Conversation("MX0CON006", 97, 2000.0f, false);
			
			Commands->Join_Conversation(field20Obj, conversationId, false, true, true);
			
			// Find the star and its id
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);
			int starId = Commands->Get_ID(starObj);

			Commands->Join_Conversation_Facing(field20Obj, conversationId, starId);
		
			// Join the star with the conversation
			Commands->Join_Conversation(starObj, conversationId, true, true, true);

			// Start conversation and monitor it
			Commands->Start_Conversation(conversationId, 100006);
			Commands->Monitor_Conversation(obj, conversationId);

			this->field_38 = 1;
		}
	}
	else if (type == 100007)
	{
		if (Commands->Find_Object(this->sniper1ObjId) != NULL)
		{
			static const char * const ENGINEER_WAIT_FOR_SNIPER_CONVERSATIONS[] =
			{
				// - Let's give 'em a little surprise.
				"MX0CON012",

				// - I can't take them out with this.
				"MX0CON013",

				// - Havoc will take care of 'em.
				"MX0CON014",

				// - Snipers, Sir! Take them out!
				"MX0CON015",

				// - Get 'em, Sir!
				"MX0CON016",

				// Lock & Load, Havoc. Drop that sniper!
				"MX0CON017"
			};

			if (this->field_34 == 0 || this->field_34 == 2 || this->field_34 == 4)
			{
				int conversationId = Commands->Create_Conversation(ENGINEER_WAIT_FOR_SNIPER_CONVERSATIONS[this->field_34], 97, 2000.0f, false);

				GameObject *field1CObj = Commands->Find_Object(this->field_1C);
				Commands->Join_Conversation(field1CObj, conversationId, false, true, true);

				// Join the star with the conversation
				Vector3 pos = Commands->Get_Position(obj);
				GameObject *starObj = Commands->Get_A_Star(pos);
				Commands->Join_Conversation(starObj, conversationId, true, true, true);

				// Start conversation and monitor it
				Commands->Start_Conversation(conversationId, 100007);
				Commands->Monitor_Conversation(obj, conversationId);
			}

			if (this->field_34 == 1 || this->field_34 == 3 || this->field_34 == 5)
			{
				int conversationId = Commands->Create_Conversation(ENGINEER_WAIT_FOR_SNIPER_CONVERSATIONS[this->field_34], 97, 2000.0f, false);

				GameObject *field20Obj = Commands->Find_Object(this->field_20);
				Commands->Join_Conversation(field20Obj, conversationId, false, true, true);

				// Join the star with the conversation
				Vector3 pos = Commands->Get_Position(obj);
				GameObject *starObj = Commands->Get_A_Star(pos);
				Commands->Join_Conversation(starObj, conversationId, true, true, true);

				// Start conversation and monitor it
				Commands->Start_Conversation(conversationId, 100007);
				Commands->Monitor_Conversation(obj, conversationId);
			}

			if (this->field_34 < 5)
			{
				this->field_34++;
			}

			if (this->field_34 >= 5) // Bause of this line, MX0CON017 will never be used
			{
				this->field_34 = 0;
			}

			Commands->Start_Timer(obj, this, 5.0f, 124);
		}
	}
	else if (type == 100005)
	{
		// Create conversation MX0CON005:
		// - You hear that? It's Recon 1. Let's go!
		int conversationId = Commands->Create_Conversation("MX0CON005", 97, 2000.0f, false);
		
		GameObject *field20Obj = Commands->Find_Object(this->field_20);
		Commands->Join_Conversation(field20Obj, conversationId, false, true, true);
		
		// Find the star and its id
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);
		int starId = Commands->Get_ID(starObj);

		Commands->Join_Conversation_Facing(field20Obj, conversationId, starId);
		
		// Join the star with the conversation
		Commands->Join_Conversation(starObj, conversationId, true, true, true);

		// Start conversation and monitor it
		Commands->Start_Conversation(conversationId, 100005);
		Commands->Monitor_Conversation(obj, conversationId);
	}
}

ScriptRegistrant<MX0_MissionStart_DME> MX0_MissionStart_DMERegistrant("MX0_MissionStart_DME", NULL);
