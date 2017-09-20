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
#include "MX0_GDI_ORCA.h"

/*
M13 -> 1400004
*/
void MX0_GDI_ORCA::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->canDropTank, sizeof(this->canDropTank), 1);
}

void MX0_GDI_ORCA::Created(GameObject *obj)
{
	this->canDropTank = true;
}

void MX0_GDI_ORCA::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received by ourselves 1.5 seconds after action_complete
	if (type == 1)
	{
		// Orca 6 to Eagle base.  I have visual on the harvester.
		// Starting my run, now.
		int conversationId = Commands->Create_Conversation("MX0_A03_03", 0, 0.0f, true);
		Commands->Join_Conversation(NULL, conversationId, true, true, true);
		Commands->Start_Conversation(conversationId, 1);
	}

	// Received by ourselves 9.5 seconds after action_complete
	else if (type == 2)
	{
		GameObject *MX0A03ControllerObj = Commands->Find_Object(1400041);
		if (MX0A03ControllerObj)
		{
			Commands->Send_Custom_Event(obj, MX0A03ControllerObj, 9035, 12, 0.0f);
		}

		GameObject *MX0GDIMiniGunner0BObj = Commands->Find_Object(this->MX0GDIMiniGunner0BObjId);

		// This is Orca 6 – Bingo fuel.  Returning to base.
		// Copy, Orca Six.  Your ball, Havoc.
		// Finish off that Harvester!
		int conversationId = Commands->Create_Conversation("MX0_A03_04", 0, 0.0f, true);
		Commands->Join_Conversation(MX0GDIMiniGunner0BObj, conversationId, false, false, true);
		Commands->Join_Conversation(NULL, conversationId, true, true, true);
		Commands->Start_Conversation(conversationId, 2);

		if (MX0GDIMiniGunner0BObj)
		{
			Commands->Send_Custom_Event(obj, MX0GDIMiniGunner0BObj, 5, 0, 0.0f);
			Commands->Send_Custom_Event(obj, MX0GDIMiniGunner0BObj, 3, 0, 10.0f);
		}
	}

	// Received MX0_A03_Controller_DAK at type 9035 when we sent it custom 9035 in custom type 2
	else if (type == 9035)
	{
		this->MX0GDIMiniGunner0BObjId = param;
	}
}

void MX0_GDI_ORCA::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered after conversion in entered complete
	if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED && action_id == 1)
	{
		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
		Commands->Attach_Script(invisObj, "Test_Cinematic", "X0F_Harvester.txt");

		Commands->Send_Custom_Event(obj, obj, 1, 0, 1.5f);

		GameObject *nodHarvesterObj = Commands->Find_Object(1400001);
		Commands->Send_Custom_Event(obj, nodHarvesterObj, 413, 4, 7.0f);
		Commands->Send_Custom_Event(obj, nodHarvesterObj, 413, 3, 9.0f);
		Commands->Send_Custom_Event(obj, nodHarvesterObj, 413, 3, 10.0f);
		Commands->Send_Custom_Event(obj, obj, 2, 0, 9.5f);
	}
}

void MX0_GDI_ORCA::Entered(GameObject *obj, GameObject *enterer)
{
	if (this->canDropTank)
	{
		Commands->Fade_Background_Music("Level 0 Tiberium.mp3", 2, 2);

		// Eagle Base, we must be close.
		// There’s a NOD Harvester here.
		// Good Work. I’m sending in more fire power.
		int conversationId = Commands->Create_Conversation("MX0_A03_02", 0, 0.0f, true);
		Commands->Join_Conversation(NULL, conversationId, true, true, true);
		Commands->Start_Conversation(conversationId, 1);
		Commands->Monitor_Conversation(obj, conversationId);

		GameObject *tankDropObj = Commands->Find_Object(1400057);
		if (tankDropObj)
		{
			Commands->Attach_Script(tankDropObj, "Test_Cinematic", "XG_A03_Tank_Drop.txt");
		}

		this->canDropTank = false;
	}
}

ScriptRegistrant<MX0_GDI_ORCA> MX0_GDI_ORCARegistrant("MX0_GDI_ORCA", "");