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
#include "MX0_MissionStart_DME.h"

void MX0_MissionStart_DME::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->engineer1ObjId, sizeof(this->engineer1ObjId), 1);
	Auto_Save_Variable(&this->engineer2ObjId, sizeof(this->engineer2ObjId), 2);
	Auto_Save_Variable(&this->hasEngineer1ObjId, sizeof(this->hasEngineer1ObjId), 3);
	Auto_Save_Variable(&this->engineerGotoZoneCount, sizeof(this->engineerGotoZoneCount), 4);
	Auto_Save_Variable(&this->sniper1ObjId, sizeof(this->sniper1ObjId), 5);
	Auto_Save_Variable(&this->sniper2ObjId, sizeof(this->sniper2ObjId), 6);
	Auto_Save_Variable(&this->engineerWaitForSniperConversationIndex, sizeof(this->engineerWaitForSniperConversationIndex), 7);
	Auto_Save_Variable(&this->starWarnedForSnipers, sizeof(this->starWarnedForSnipers), 8);
}

void MX0_MissionStart_DME::Created(GameObject *obj)
{
	// Deselect weapon of the star
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *starObj = Commands->Get_A_Star(pos);
	Commands->Select_Weapon(starObj, NULL);

	// Initialize member variables
	this->engineerWaitForSniperConversationIndex = 0;
	this->sniper2ObjId = 0;
	this->sniper1ObjId = 0;
	this->engineerGotoZoneCount = 0;
	this->hasEngineer1ObjId = false;
	this->engineer2ObjId = 0;
	this->engineer1ObjId = 0;
	this->starWarnedForSnipers = false;

	// Fade into intro music
	Commands->Fade_Background_Music("renegade_intro_no_vox.mp3", 0, 1);

	// Start intro cinematic
	GameObject *cinObj = Commands->Create_Object("Invisible_Object", Vector3(0, 0, 0));
	Commands->Attach_Script(cinObj, "Test_Cinematic", "X00_Intro.txt");
}

void MX0_MissionStart_DME::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered when the engineer finished the conversation: 
	// - Clear!
	if (action_id == 100001) 
	{
		// Sends custom 100002, 0 to itself which starts the conversation:
		// - It sounds like they're still in trouble, Sir!
		// - Saddle up, let's move.
		Commands->Start_Timer(obj, this, 2.0f, 119);
	}

	// Triggered when the engineer finished the conversation:
	// - It sounds like they're still in trouble, Sir!
	// - Saddle up, let's move.
	else if (action_id == 100002)
	{
		// Engineer calls for the star in 5 seconds when no MX0_Engineer_Goto1/2 zone has been entered yet
		Commands->Start_Timer(obj, this, 5.0f, 120);
	}

	// TODO
	else if (action_id == 100004)
	{
		GameObject *engineerGoto2ZoneCount4Obj = Commands->Find_Object(1200015);

		// TODO
		Commands->Send_Custom_Event(obj, engineerGoto2ZoneCount4Obj, 110, 0, 0.0f);
	}
}

void MX0_MissionStart_DME::Timer_Expired(GameObject *obj, int number)
{
	// The following events occur when the star is done with his animation in the intro cinematic. 
	if (number == 117) // Triggered after 0.5 seconds
	{
		// Start the background music
		Commands->Set_Background_Music("Level0_pt1_music.mp3");

		// Get the star
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		//Give weapons
		Commands->Give_PowerUp(starObj, "POW_AutoRifle_Player", false);
		Commands->Give_PowerUp(starObj, "POW_AutoRifle_Player", false);

		// Select autorifle
		Commands->Select_Weapon(starObj, "Weapon_AutoRifle_Player");

		// Fails the mission when owner gets killed
		Commands->Attach_Script(starObj, "Havoc_Script", "");
	}
	else if (number == 133) // Triggered after 1.0 seconds
	{
		// Get the daves arrow position for sniper 1 (the one on the small rock)
		GameObject *sniper1PosObj = Commands->Find_Object(1200043);
		Vector3 sniper1Pos = Commands->Get_Position(sniper1PosObj);

		// Get the daves arrow position for sniper 2 (the one on the cliff side)
		GameObject *sniper2PosObj = Commands->Find_Object(1200044);
		Vector3 sniper2Pos = Commands->Get_Position(sniper2PosObj);

		// Create sniper 1, attach scripts and store its id
		// TODO
		GameObject *sniper1Obj = Commands->Create_Object("NOD_Minigunner_2SF", sniper1Pos);
		Commands->Attach_Script(sniper1Obj, "M00_Soldier_Powerup_Disable", "");
		Commands->Attach_Script(sniper1Obj, "M00_Send_Object_ID", "1200001, 1, 0.0f");
		Commands->Attach_Script(sniper1Obj, "MX0_SniperAction", "1200045");
		Commands->Attach_Script(sniper1Obj, "MX0_KillNotify", "");
		this->sniper1ObjId = Commands->Get_ID(sniper1Obj);

		// Create sniper 2, attach scripts and store its id
		// TODO
		GameObject *sniper2Obj = Commands->Create_Object("NOD_Minigunner_2SF", sniper2Pos);
		Commands->Attach_Script(sniper2Obj, "M00_Soldier_Powerup_Disable", "");
		Commands->Attach_Script(sniper2Obj, "M00_Send_Object_ID", "1200001, 2, 0.0f");
		Commands->Attach_Script(sniper2Obj, "MX0_SniperAction", "1200045");
		this->sniper2ObjId = Commands->Get_ID(sniper2Obj);
	}
	else if (number == 127) // Triggered after 3.0 seconds
	{
		// MX0_HELPTEXT_01: Move the Mouse to Look Around
		Commands->Set_HUD_Help_Text(8576, Vector3(0.196f, 0.882f, 0.196f));
	}
	else if (number == 128) // Triggered after 6.0 seconds 
	{
		// MX0_HELPTEXT_02: Use W, S, A and D keys to Move (Default)
		Commands->Set_HUD_Help_Text(8577, Vector3(0.196f, 0.882f, 0.196f));
	}
	else if (number == 118) // Triggered after 9.0 seconds
	{
		// MX0_HELPTEXT_03: Press F1 for Help
		Commands->Set_HUD_Help_Text(8578, Vector3(0.196f, 0.882f, 0.196f));
	}

	// Triggered 2.0 seconds after the engineer finished his conversation: 
	// - !Clear 
	else if (number == 119) 
	{
		// Engineer starts conversation:
		// - It sounds like they're still in trouble, Sir!
		// - Saddle up, let's move.
		Commands->Send_Custom_Event(obj, obj, 100002, 0, 0.0f);
	}

	// Triggered 5.0 seconds after the engineer finished his conversation:
	// - It sounds like they're still in trouble, Sir!
	// - Saddle up, let's move.
	else if (number == 120)
	{
		// When no MX0_Engineer_Goto1/2 zone has been entered yet
		if (this->engineerGotoZoneCount <= 1) 
		{
			// Engineer calls for star conversation 
			Commands->Send_Custom_Event(obj, obj, 100003, 0, 0.0f);

			// Check again after 5 seconds
			Commands->Start_Timer(obj, this, 5.0f, 120);
		}
	}

	// Triggered 2.0 seconds after MX0_Engineer_Goto2 zone is entered with count 5
	else if (number == 136)
	{
		if (Commands->Find_Object(this->sniper1ObjId) != NULL) // Is sniper 1 still alive?
		{
			// TODO
			GameObject *engineer1Obj = Commands->Find_Object(this->engineer1ObjId);
			Commands->Send_Custom_Event(obj, engineer1Obj, 136, this->sniper1ObjId, 0.0f);

			// TODO
			GameObject *engineer2Obj = Commands->Find_Object(this->engineer2ObjId);
			Commands->Send_Custom_Event(obj, engineer2Obj, 136, sniper1ObjId, 0.0f);

			// Check again in 3 seconds
			Commands->Start_Timer(obj, this, 3.0f, 136);
		}
	}

	// Triggered 5.0 seconds after MX0_Engineer_Goto2 zone is entered with count 5
	else if (number == 124)
	{
		if (Commands->Find_Object(this->sniper1ObjId) != NULL) // Is sniper 1 still alive?
		{
			// Engineer asks the star to take out the sniper conversation
			Commands->Send_Custom_Event(obj, obj, 100007, 0, 0.0f);
		}
	}

	// TODO
	else if (number == 142)
	{
		// Create conversation MX0_ENGINEER1_048: 
		// - We’ll hang tight until the coast is clear
		int conversationId = Commands->Create_Conversation("MX0_ENGINEER1_048", 97, 2000.0f, false);

		// Let engineer 1 join the conversation
		GameObject *engineer1Obj = Commands->Find_Object(this->engineer1ObjId);
		Commands->Join_Conversation(engineer1Obj, conversationId, false, true, true);

		// Let the star join the conversation
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);
		Commands->Join_Conversation(starObj, conversationId, true, true, true);

		// Start and monitor the conversation
		Commands->Start_Conversation(conversationId, 100048);
		Commands->Monitor_Conversation(obj, conversationId);
	}

	// TODO
	else if (number == 134)
	{
		// TODO
		GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
		Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 202, 0, 0.0f);
	}
}

void MX0_MissionStart_DME::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from MX0_Engineer1 and MX0_Engineer2 when they are created
	if (type == 103)
	{
		if (this->hasEngineer1ObjId)
		{
			this->engineer2ObjId = param;
		}
		else
		{
			this->hasEngineer1ObjId = true;
			this->engineer1ObjId = param;
		}
	}

	// Triggered when the star is done with his animation in the intro cinematic
	else if (type == 99) 
	{
		this->engineerGotoZoneCount = 0;

		// Create blocker1 at the rock pile in the beginning and disables rendering
		GameObject *blocker1Obj = Commands->Create_Object("Large_Blocker", Vector3(-148.071f, -31.267f, -0.306f));
		Commands->Set_Facing(blocker1Obj, -30.0f);
		Commands->Set_Is_Rendered(blocker1Obj, false);

		// Create blocker2 at the rock pile in the beginning and disables rendering
		GameObject *blocker2Obj = Commands->Create_Object("Large_Blocker", Vector3(-148.234f, -31.542f, 1.357f));
		Commands->Set_Facing(blocker2Obj, -30.0f);
		Commands->Set_Is_Rendered(blocker2Obj, false);

		// Starts Level0_pt1_music.mp3 music, gives weapon to the star and attaches its script
		Commands->Start_Timer(obj, this, 0.5f, 117);

		// Shows HUD helptext: 
		// - Move the Mouse to Look Around
		Commands->Start_Timer(obj, this, 3.0f, 127);

		// Shows HUD helptext: 
		// - Use W, S, A and D keys to Move (Default)
		Commands->Start_Timer(obj, this, 6.0f, 128);

		// Shows HUD helptext: 
		// - Press F1 for Help
		Commands->Start_Timer(obj, this, 9.0f, 118);

		// Spawns the snipers on the small rock and cliff side
		Commands->Start_Timer(obj, this, 1.0f, 133);
	}

	// Triggered when intro cinematic is done
	else if (type == 100001) 
	{
		// Create conversation MX0CON001:
		// - Clear!
		int conversationId = Commands->Create_Conversation("MX0CON001", 99, 2000.0f, false);

		// Let engineer 2 join the conversation
		GameObject *engineer2Obj = Commands->Find_Object(this->engineer2ObjId);
		Commands->Join_Conversation(engineer2Obj, conversationId, false, true, true);

		// Start conversation and monitor it
		Commands->Start_Conversation(conversationId, 100001);
		Commands->Monitor_Conversation(obj, conversationId);
	}

	// Triggered 2.0 seconds after the engineer finished his conversation: 
	// - !Clear 
	else if (type == 100002) 
	{
		// Create conversation MX0CON002:
		// - It sounds like they're still in trouble, Sir!
		// - Saddle up, let's move.
		int conversationId = Commands->Create_Conversation("MX0CON002", 97, 2000.0f, false);

		// Let engineer 1 join the conversation
		GameObject *engineer1Obj = Commands->Find_Object(this->engineer1ObjId);
		Commands->Join_Conversation(engineer1Obj, conversationId, false, true, true);

		// Find the star and its id
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);
		int starId = Commands->Get_ID(starObj);

		// Let engineer 1 join the conversation again and this time face the star
		Commands->Join_Conversation_Facing(engineer1Obj, conversationId, starId);

		// Let engineer 2 join the conversation twice
		GameObject *engineer2Obj = Commands->Find_Object(this->engineer2ObjId);
		Commands->Join_Conversation(engineer2Obj, conversationId, false, true, true);
		Commands->Join_Conversation_Facing(engineer2Obj, conversationId, starId);

		// Join the star with the conversation
		Commands->Join_Conversation(starObj, conversationId, true, true, true);

		// Start conversation and monitor it
		Commands->Start_Conversation(conversationId, 100002);
		Commands->Monitor_Conversation(obj, conversationId);
	}

	// Received from MX0_Engineer_Goto when entered, with param being its 'Count' parameter
	// Received from MX0_Engineer_Goto2 TODO
	else if (type == 102)
	{
		this->engineerGotoZoneCount = param;

		// TODO
		GameObject *engineer1Obj = Commands->Find_Object(this->engineer1ObjId);
		Commands->Send_Custom_Event(obj, engineer1Obj, 108, this->engineerGotoZoneCount, 0.0f);

		// TODO
		GameObject *engineer2Obj = ::Commands->Find_Object(this->engineer2ObjId);
		Commands->Send_Custom_Event(obj, engineer2Obj, 108, this->engineerGotoZoneCount, 0.0f);
	}

	// Received from MX0_Engineer_Goto when entered, with param being 1 or 2
	// Received from MX0_Engineer_Goto2 TODO
	else if (type == 104)
	{
		if (param == 1)
		{
			// TODO
			Commands->Send_Custom_Event(obj, sender, 105, this->engineer1ObjId, 0.0f);
		}
		else if (param == 2)
		{
			// TODO
			Commands->Send_Custom_Event(obj, sender, 106, this->engineer2ObjId, 0.0f);
		}
	}

	// Received from MX0_NOD_TroopDrop when entered with param 0
	else if (type == 114)
	{
		// TODO
		GameObject *sniper1Obj = Commands->Find_Object(this->sniper1ObjId);
		Commands->Send_Custom_Event(obj, sniper1Obj, 114, 0, 0.0f);

		// TODO
		GameObject *sniper2Obj = Commands->Find_Object(this->sniper2ObjId);
		Commands->Send_Custom_Event(obj, sniper2Obj, 114, 0, 0.0f);
	}

	// Received from MX0_Engineer_Goto2 when entered and count 5 with param 0
	else if (type == 136)
	{
		// TODO
		Commands->Start_Timer(obj, this, 2.0f, 136);
	}

	// Received from MX0_KillNotify when killed by a star with param 0
	else if (type == 138)
	{
		if (!this->starWarnedForSnipers)
		{
			// Create conversation MX0CON018:
			// - There we go!
			int conversationId = ::Commands->Create_Conversation("MX0CON018", 97, 2000.0f, 0);

			// Let engineer 1 join the conversation
			GameObject *engineer1Obj = Commands->Find_Object(this->engineer1ObjId);
			Commands->Join_Conversation(engineer1Obj, conversationId, false, true, true);

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

	// Received from MX0_Engineer2 TODO
	else if (type == 132)
	{
		// TODO
		GameObject *engineer1Obj = Commands->Find_Object(this->engineer1ObjId);
		Commands->Send_Custom_Event(obj, engineer1Obj, 132, 0, 0.0f);
	}

	// Received from MX0_Engineer_Goto2 when entered and count 5 with param 1
	else if (type == 100006)
	{
		if (Commands->Find_Object(this->sniper1ObjId) != NULL)
		{
			// TODO
			GameObject *engineer2Obj = Commands->Find_Object(this->engineer2ObjId);
			Commands->Send_Custom_Event(obj, engineer2Obj, 125, 0, 0.0f);

			// Engineer asks star to take out snipers conversation if sniper 1 is still alive
			Commands->Start_Timer(obj, this, 5.0f, 124);

			// Create conversation MX0CON006:
			// - Up there - snipers!
			int conversationId = Commands->Create_Conversation("MX0CON006", 97, 2000.0f, false);

			// Let engineer 2 join the conversation
			Commands->Join_Conversation(engineer2Obj, conversationId, false, true, true);

			// Find the star and its id
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);
			int starId = Commands->Get_ID(starObj);

			// Let engineer 2 join the conversation again and face the star
			Commands->Join_Conversation_Facing(engineer2Obj, conversationId, starId);

			// Join the star with the conversation
			Commands->Join_Conversation(starObj, conversationId, true, true, true);

			// Start conversation and monitor it
			Commands->Start_Conversation(conversationId, 100006);
			Commands->Monitor_Conversation(obj, conversationId);

			this->starWarnedForSnipers = true;
		}
	}
	
	// TODO
	else if (type == 223)
	{
		// TODO
		Commands->Send_Custom_Event(obj, sender, 223, this->sniper1ObjId, 0.0f);
	}

	// TODO
	else if (type == 224)
	{
		// TODO
		Commands->Send_Custom_Event(obj, sender, 224, this->sniper2ObjId, 0.0f);
	}

	// TODO
	else if (type == 9035)
	{
		if (param == 1)
		{
			this->sniper1ObjId = Commands->Get_ID(sender);
		}
		else if (param == 2)
		{
			this->sniper2ObjId = Commands->Get_ID(sender);

			// TODO
			Commands->Start_Timer(obj, this, 2.0f, 134);
		}
	}

	// TODO
	else if (type == 130)
	{
		// TODO
		GameObject *engineer1Obj = Commands->Find_Object(this->engineer1ObjId);
		Commands->Send_Custom_Event(obj, engineer1Obj, 131, 0, 0.0f);

		// TODO
		GameObject *engineer2Obj = Commands->Find_Object(this->engineer2ObjId);
		Commands->Send_Custom_Event(obj, engineer2Obj, 131, 0, 0.0f);
	}
	
	// TODO
	else if (type == 111)
	{
		// TODO
		GameObject *engineer1Obj = Commands->Find_Object(this->engineer1ObjId);
		Commands->Send_Custom_Event(obj, engineer1Obj, 112, 0, 0.0f);

		// TODO
		GameObject *engineer2Obj = Commands->Find_Object(this->engineer2ObjId);
		Commands->Send_Custom_Event(obj, engineer2Obj, 112, 0, 0.0f);
	}

	// TODO
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
	
		// Let engineer 2 join the conversation
		GameObject *engineer2Obj = Commands->Find_Object(this->engineer2ObjId);
		Commands->Join_Conversation(engineer2Obj, conversationId, false, true, true);
		
		// Find the star and its id
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);
		int starId = Commands->Get_ID(starObj);

		// Let engineer 2 join the conversation again and face the star
		Commands->Join_Conversation_Facing(engineer2Obj, conversationId, starId);
		
		// Join the star with the conversation
		Commands->Join_Conversation(starObj, conversationId, true, true, true);
		
		// Start conversation and monitor it
		Commands->Start_Conversation(conversationId, 100003);
		Commands->Monitor_Conversation(obj, conversationId);
	}
	
	// TODO
	else if (type == 100007)
	{
		if (Commands->Find_Object(this->sniper1ObjId) != NULL) // Is sniper 1 still alive?
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

			if (this->engineerWaitForSniperConversationIndex == 0 || this->engineerWaitForSniperConversationIndex == 2 || this->engineerWaitForSniperConversationIndex == 4)
			{
				int conversationId = Commands->Create_Conversation(ENGINEER_WAIT_FOR_SNIPER_CONVERSATIONS[this->engineerWaitForSniperConversationIndex], 97, 2000.0f, false);

				// Let engineer 1 join the conversation
				GameObject *engineer1Obj = Commands->Find_Object(this->engineer1ObjId);
				Commands->Join_Conversation(engineer1Obj, conversationId, false, true, true);

				// Join the star with the conversation
				Vector3 pos = Commands->Get_Position(obj);
				GameObject *starObj = Commands->Get_A_Star(pos);
				Commands->Join_Conversation(starObj, conversationId, true, true, true);

				// Start conversation and monitor it
				Commands->Start_Conversation(conversationId, 100007);
				Commands->Monitor_Conversation(obj, conversationId);
			}

			if (this->engineerWaitForSniperConversationIndex == 1 || this->engineerWaitForSniperConversationIndex == 3 || this->engineerWaitForSniperConversationIndex == 5)
			{
				int conversationId = Commands->Create_Conversation(ENGINEER_WAIT_FOR_SNIPER_CONVERSATIONS[this->engineerWaitForSniperConversationIndex], 97, 2000.0f, false);

				// Let engineer 2 join the conversation
				GameObject *engineer2Obj = Commands->Find_Object(this->engineer2ObjId);
				Commands->Join_Conversation(engineer2Obj, conversationId, false, true, true);

				// Join the star with the conversation
				Vector3 pos = Commands->Get_Position(obj);
				GameObject *starObj = Commands->Get_A_Star(pos);
				Commands->Join_Conversation(starObj, conversationId, true, true, true);

				// Start conversation and monitor it
				Commands->Start_Conversation(conversationId, 100007);
				Commands->Monitor_Conversation(obj, conversationId);
			}
			
			// Check and increment index field
			if (this->engineerWaitForSniperConversationIndex < 5)
			{
				this->engineerWaitForSniperConversationIndex++;
			}

			if (this->engineerWaitForSniperConversationIndex >= 5) // Bause of this line, MX0CON017 will never be used
			{
				this->engineerWaitForSniperConversationIndex = 0;
			}

			// TODO
			Commands->Start_Timer(obj, this, 5.0f, 124);
		}
	}

	// TODO
	else if (type == 100005)
	{
		// Create conversation MX0CON005:
		// - You hear that? It's Recon 1. Let's go!
		int conversationId = Commands->Create_Conversation("MX0CON005", 97, 2000.0f, false);
		
		// Let engineer 2 join the conversation
		GameObject *engineer2Obj = Commands->Find_Object(this->engineer2ObjId);
		Commands->Join_Conversation(engineer2Obj, conversationId, false, true, true);
		
		// Find the star and its id
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);
		int starId = Commands->Get_ID(starObj);

		// Let engineer 2 join the conversation again and face the star
		Commands->Join_Conversation_Facing(engineer2Obj, conversationId, starId);
		
		// Join the star with the conversation
		Commands->Join_Conversation(starObj, conversationId, true, true, true);

		// Start conversation and monitor it
		Commands->Start_Conversation(conversationId, 100005);
		Commands->Monitor_Conversation(obj, conversationId);
	}
}

ScriptRegistrant<MX0_MissionStart_DME> MX0_MissionStart_DMERegistrant("MX0_MissionStart_DME", "");
