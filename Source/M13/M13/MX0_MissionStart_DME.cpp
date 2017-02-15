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
	// Initialize member variables
	this->field_34 = 0;
	this->sniper2ObjId = 0;
	this->sniper1ObjId = 0;
	this->field_28 = 0;
	this->field_24 = 0;
	this->field_20 = 0;
	this->field_1C = 0;
	this->field_38 = 0;

	// Deselect weapon of the star
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *starObj = Commands->Get_A_Star(pos);
	Commands->Select_Weapon(starObj, NULL);

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
		Commands->Start_Timer(obj, this, 5.0, 120);
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
		// Start conversation MX0_ENGINEER1_048: 
		// - We’ll hang tight until the coast is clear
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
			// Start conversation MX0CON018:
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
	if (type == 99)
	{
		this->field_28 = 0;
		LODWORD(pos.X) = 0xC314122D;                // -148.071f
		LODWORD(pos.Y) = 0xC1FA22D1;                // -31.267f
		LODWORD(pos.Z) = 0xBE9CAC08;                // -0.306f
		blocker1Obj = ::Commands->Create_Object("Large_Blocker", &pos);
		::Commands->Set_Facing(blocker1Obj, -30.0);
		::Commands->Set_Is_Rendered(blocker1Obj, 0);
		LODWORD(pos.X) = 0xC3143BE7;                // -148.234f
		LODWORD(pos.Y) = 0xC1FC5604;                // -31.542f
		LODWORD(pos.Z) = 0x3FADB22D;                // 1.357f
		blocker2Obj = ::Commands->Create_Object("Large_Blocker", &pos);
		::Commands->Set_Facing(blocker2Obj, -30.0);
		::Commands->Set_Is_Rendered(blocker2Obj, 0);
		::Commands->Start_Timer(obj, &this->base.base, 0.5, 117);
		::Commands->Start_Timer(obj, &this->base.base, 3.0, 127);
		::Commands->Start_Timer(obj, &this->base.base, 6.0, 128);
		::Commands->Start_Timer(obj, &this->base.base, 9.0, 118);
		::Commands->Start_Timer(obj, &this->base.base, 1.0, 133);
	}
	if (type == 130)
	{
		Commands_4 = ::Commands;
		field_1CObj_1 = ::Commands->Find_Object(this->field_1C);
		Commands_4->Send_Custom_Event(obj, field_1CObj_1, 131, 0, 0.0);
		Commands_5 = ::Commands;
		field_20Obj = ::Commands->Find_Object(this->field_20);
		Commands_5->Send_Custom_Event(obj, field_20Obj, 131, 0, 0.0);
	}
	if (type == 132)
	{
		Commands_6 = ::Commands;
		field_1CObj_2 = ::Commands->Find_Object(this->field_1C);
		Commands_6->Send_Custom_Event(obj, field_1CObj_2, 132, 0, 0.0);
	}
	if (type == 111)
	{
		Commands_7 = ::Commands;
		field_1CObj_3 = ::Commands->Find_Object(this->field_1C);
		Commands_7->Send_Custom_Event(obj, field_1CObj_3, 112, 0, 0.0);
		Commands_8 = ::Commands;
		field_20Obj_1 = ::Commands->Find_Object(this->field_20);
		Commands_8->Send_Custom_Event(obj, field_20Obj_1, 112, 0, 0.0);
	}
	if (type == 102)
	{
		this->field_28 = param;
		Commands_9 = ::Commands;
		field_28 = this->field_28;
		field_1CObj_4 = ::Commands->Find_Object(this->field_1C);
		Commands_9->Send_Custom_Event(obj, field_1CObj_4, 108, field_28, 0.0);
		Commands_10 = ::Commands;
		field_28_1 = this->field_28;
		field_20Obj_2 = ::Commands->Find_Object(this->field_20);
		Commands_10->Send_Custom_Event(obj, field_20Obj_2, 108, field_28_1, 0.0);
	}
	if (type == 103)
	{
		if (this->field_24)
		{
			this->field_20 = param;
		}
		else
		{
			this->field_1C = param;
			this->field_24 = 1;
		}
	}
	if (type == 104)
	{
		if (param == 1)
			::Commands->Send_Custom_Event(obj, sender, 105, this->field_1C, 0.0);
		if (param == 2)
			::Commands->Send_Custom_Event(obj, sender, 106, this->field_20, 0.0);
	}
	if (type == 100001)
	{
		conversationId_2 = ::Commands->Create_Conversation("MX0CON001", 99, 2000.0, 0);
		Commands_11 = ::Commands;
		field_20Obj_3 = ::Commands->Find_Object(this->field_20);
		Commands_11->Join_Conversation(field_20Obj_3, conversationId_2, 0, 1, 1);
		::Commands->Start_Conversation(conversationId_2, 100001);
		::Commands->Monitor_Conversation(obj, conversationId_2);
	}
	if (type == 100002)
	{
		conversationId_3 = ::Commands->Create_Conversation("MX0CON002", 97, 2000.0, 0);
		Commands_12 = ::Commands;
		v38 = ::Commands->Find_Object(*(_DWORD *)(param + 28));
		Commands_12->Join_Conversation(v38, conversationId_3, 0, 1, 1);
		v39 = ::Commands;
		v40 = (ScriptableGameObj *)(*(int(__cdecl **)(int))(*(_DWORD *)param + 72))(param);
		v39->Get_Position(&v124, v40);
		v41 = (int)v39->Get_A_Star(&v124);
		v120 = v39->Get_ID((ScriptableGameObj *)v41);
		v42 = ::Commands->Find_Object(v186BE);
		v39->Join_Conversation_Facing(v42, conversationId_3, v120);
		v43 = ::Commands;
		v44 = ::Commands->Find_Object(v186C2);
		v43->Join_Conversation(v44, conversationId_3, 0, 1, 1);
		v45 = ::Commands;
		v46 = (ScriptableGameObj *)(*(int(__cdecl **)(signed int))(v186A2 + 72))(100002);
		v45->Get_Position(&v124, v46);
		v47 = (int)v45->Get_A_Star(&v124);
		v48 = v45->Get_ID((ScriptableGameObj *)v47);
		v49 = ::Commands->Find_Object(*((_DWORD *)obj + 8));
		v45->Join_Conversation_Facing(v49, conversationId_3, v48);
		v50 = ::Commands;
		v51 = (ScriptableGameObj *)(*(int(__cdecl **)(ScriptableGameObj *))(*(_DWORD *)obj + 72))(obj);
		v50->Get_Position(&v124, v51);
		v52 = v50->Get_A_Star(&v124);
		v50->Join_Conversation(v52, v118, conversationId_3, 1, 1);
		::Commands->Start_Conversation(conversationId_3, 100002);
		::Commands->Monitor_Conversation(obj, conversationId_3);
	}
	if (type == 100003)
	{
		LODWORD(pos.X) = "MX0CON003";
		LODWORD(pos.Y) = "MX0CON003alt1";
		LODWORD(pos.Z) = "MX0CON003alt2";
		LODWORD(v124.X) = "MX0CON003alt3";
		v53 = ((int(__cdecl *)(_DWORD))::Commands->Get_Random_Int)(0);
		v54 = ::Commands->Create_Conversation(*((const char **)&pos.X + v53), 97, 2000.0, 0);
		v55 = ::Commands;
		v56 = ::Commands->Find_Object(this->field_20);
		v55->Join_Conversation(v56, 1, 1, 4, v119);
		v57 = ::Commands;
		v58 = (ScriptableGameObj *)(*(int(__cdecl **)(signed int))(v186A3 + 72))(100003);
		v57->Get_Position((Vector3 *)&v122, v58);
		v59 = (int)v57->Get_A_Star((Vector3 *)&v122);
		v60 = v57->Get_ID((ScriptableGameObj *)v59);
		v61 = ::Commands->Find_Object(*((_DWORD *)obj + 8));
		v57->Join_Conversation_Facing(v61, v54, v60);
		v62 = ::Commands;
		v63 = (ScriptableGameObj *)(*(int(__cdecl **)(ScriptableGameObj *))(*(_DWORD *)obj + 72))(obj);
		v62->Get_Position((Vector3 *)&v122, v63);
		v64 = v62->Get_A_Star((Vector3 *)&v122);
		v62->Join_Conversation(v64, v54, v54, 1, 1);
		::Commands->Start_Conversation(v54, 100003);
		::Commands->Monitor_Conversation(obj, v54);
	}
	if (type == 100004)
	{
		v65 = ::Commands->Create_Conversation("MX0CON004", 95, 2000.0, 0);
		v66 = ::Commands;
		v67 = ::Commands->Find_Object(v186C0);
		v66->Join_Conversation(v67, v65, 1, 1, 1);
		v68 = ::Commands;
		v69 = (ScriptableGameObj *)(*(int(__cdecl **)(signed int))(v186A4 + 72))(100004);
		v68->Get_Position((Vector3 *)&v122, v69);
		v70 = (int)v68->Get_A_Star((Vector3 *)&v122);
		v71 = v68->Get_ID((ScriptableGameObj *)v70);
		v72 = ::Commands->Find_Object(*((_DWORD *)obj + 7));
		v68->Join_Conversation_Facing(v72, v65, v71);
		v73 = ::Commands;
		v74 = (ScriptableGameObj *)(*(int(__cdecl **)(ScriptableGameObj *))(*(_DWORD *)obj + 72))(obj);
		v73->Get_Position((Vector3 *)&v122, v74);
		v75 = v73->Get_A_Star((Vector3 *)&v122);
		v73->Join_Conversation(v75, v118, v65, 1, 1);
		::Commands->Start_Conversation(v65, 100004);
		::Commands->Monitor_Conversation(obj, v65);
		v76 = ::Commands;
		v77 = ::Commands->Find_Object(this->field_1C);
		v76->Innate_Disable(v77);
		v78 = ::Commands;
		v79 = ::Commands->Find_Object(this->field_20);
		v78->Innate_Disable(v79);
	}
	if (type == 100005)
	{
		v80 = ::Commands->Create_Conversation("MX0CON005", 97, 2000.0, 0);
		v81 = ::Commands;
		v82 = ::Commands->Find_Object(v186C5);
		v81->Join_Conversation(v82, v80, 0, 1, 1);
		v83 = ::Commands;
		v84 = (ScriptableGameObj *)(*(int(__cdecl **)(signed int))(v186A5 + 72))(100005);
		v83->Get_Position((Vector3 *)&v122, v84);
		v85 = (int)v83->Get_A_Star((Vector3 *)&v122);
		v86 = v83->Get_ID((ScriptableGameObj *)v85);
		v87 = ::Commands->Find_Object(*((_DWORD *)obj + 8));
		v83->Join_Conversation_Facing(v87, v80, v86);
		v88 = ::Commands;
		v89 = (ScriptableGameObj *)(*(int(__cdecl **)(_DWORD))(*(_DWORD *)obj + 72))(obj);
		v88->Get_Position((Vector3 *)&v122, v89);
		v90 = v88->Get_A_Star((Vector3 *)&v122);
		v88->Join_Conversation(v90, v118, v80, 1, 1);
		::Commands->Start_Conversation(v80, 100005);
		::Commands->Monitor_Conversation(obj, v80);
	}
	if (type == 100006 && (int)::Commands->Find_Object(this->sniper1ObjId))
	{
		v91 = ::Commands;
		v92 = ::Commands->Find_Object(this->field_20);
		v91->Send_Custom_Event(obj, v92, 125, 0, 0.0);
		::Commands->Start_Timer(obj, &this->base.base, 5.0, 124);
		v93 = ::Commands->Create_Conversation("MX0CON006", 97, 2000.0, 0);
		v94 = ::Commands;
		v95 = ::Commands->Find_Object(this->field_20);
		v94->Join_Conversation(v95, 1, 1, v96, v97);
		v98 = ::Commands;
		v99 = (*(int(__cdecl **)(int))(*(_DWORD *)type + 72))(type);
		v98->Get_Position((Vector3 *)&v122, (ScriptableGameObj *)v99);
		v100 = (int)v98->Get_A_Star((Vector3 *)&v122);
		v101 = v98->Get_ID((ScriptableGameObj *)v100);
		v102 = ::Commands->Find_Object(*((_DWORD *)obj + 8));
		v98->Join_Conversation_Facing(v102, v93, v101);
		v103 = ::Commands;
		v104 = (*(int(__cdecl **)(_DWORD))(*(_DWORD *)obj + 72))(obj);
		v103->Get_Position((Vector3 *)&v122, (ScriptableGameObj *)v104);
		v105 = v103->Get_A_Star((Vector3 *)&v122);
		v103->Join_Conversation(v105, v93, v93, 1, 1);
		::Commands->Start_Conversation(v93, 100006);
		::Commands->Monitor_Conversation(obj, v93);
		this->field_38 = 1;
	}
	if (type == 100007 && (int)::Commands->Find_Object(this->sniper1ObjId))
	{
		if (!this->field_34 || this->field_34 == 2 || this->field_34 == 4)
		{
			qmemcpy(&v121, &off_8595F90, 24u);
			v106 = ::Commands->Create_Conversation((const char *)(&v121)[this->field_34], 97, 2000.0, 0);
			v107 = ::Commands;
			v108 = ::Commands->Find_Object(this->field_1C);
			v107->Join_Conversation(v108, v106, 0, 1, 1);
			v109 = ::Commands;
			v110 = this->base.base.base.vPtr->base.Owner((ScriptClass *)this);
			v109->Get_Position((Vector3 *)&v120, v110);
			v111 = ((int(__stdcall *)(int *))v109->Get_A_Star)(&v120);
			v109->Join_Conversation((ScriptableGameObj *)v111, v106, 1, 1, 1);
			::Commands->Start_Conversation(v106, 100007);
			::Commands->Monitor_Conversation(obj, v106);
		}
		if (this->field_34 == 1 || this->field_34 == 3 || this->field_34 == 5)
		{
			qmemcpy(&v120, off_8595FA8, 0x18u);
			v112 = ::Commands->Create_Conversation((const char *)*(&v120 + this->field_34), 97, 2000.0, 0);
			v113 = ::Commands;
			v114 = ::Commands->Find_Object(this->field_20);
			v113->Join_Conversation(v114, v112, 0, 1, 1);
			v115 = ::Commands;
			v116 = this->base.base.base.vPtr->base.Owner((ScriptClass *)this);
			v115->Get_Position(&pos, v116);
			v117 = v115->Get_A_Star(&pos);
			v115->Join_Conversation(v117, v112, 1, 1, 1);
			::Commands->Start_Conversation(v112, 100007);
			::Commands->Monitor_Conversation(obj, v112);
		}
		if (this->field_34 > 4 || (++this->field_34, this->field_34 > 4))
			this->field_34 = 0;
		::Commands->Start_Timer(obj, &this->base.base, 5.0, 124);
	}
}