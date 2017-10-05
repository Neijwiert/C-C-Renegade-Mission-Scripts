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
#include "M01_Objective_Pog_Controller_JDG.h"

/*
M01 -> 105828
*/
void M01_Objective_Pog_Controller_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->objectiveConversationId, sizeof(this->objectiveConversationId), 1);
	Auto_Save_Variable(&this->tutorialConversationId, sizeof(this->tutorialConversationId), 2);
}

// TODO
void M01_Objective_Pog_Controller_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		// Received from M01_Announce_First_ObjectiveZone_JDG when entered
		if (param == 102)
		{
			this->objectiveConversationId = Commands->Create_Conversation("M01_Start_Mission_Conversation", 100, 1000.0f, false); // Now, locate the Nod Detention Center - the civilians are being held there.
			Commands->Join_Conversation(NULL, this->objectiveConversationId, false, false, true);
			Commands->Start_Conversation(this->objectiveConversationId, this->objectiveConversationId);
			Commands->Monitor_Conversation(obj, this->objectiveConversationId);
		}

		// Received from ourselves after 2 seconds when custom type 6 with param 114 is received
		else if (param == 27)
		{
			this->tutorialConversationId = Commands->Create_Conversation("M01_Press_ESC_For_Objectives_Conversation", 100, 1000.0f, false); // Press 'O' for Mission Objectives
			Commands->Join_Conversation(NULL, this->tutorialConversationId, false, false, true);
			Commands->Start_Conversation(this->tutorialConversationId, this->tutorialConversationId);
			Commands->Monitor_Conversation(obj, this->tutorialConversationId);
		}
	}
	else if (type == 6)
	{
		// Received from M01_Base_GDI_Minigunner_JDG when custom type <= 0 and param 30 is received
		if (param == 114)
		{
			// Rescue Prisoners
			// Several GDI prisoners of war are being rounded up by Nod forces. Eliminate the Nod guards and help the prisoners evacuate the area.
			Commands->Add_Objective(114, OBJECTIVE_TYPE_SECONDARY, OBJECTIVE_STATUS_PENDING, 6273, NULL, 6140);
			Commands->Set_Objective_Radar_Blip(114, Vector3(47.128f, 36.584f, -0.93f));
			Commands->Set_Objective_HUD_Info_Position(114, 85.0f, "POG_M01_2_08.tga", 7389, Vector3(47.128f, 36.584f, -0.93f)); // Rescue
			Commands->Set_HUD_Help_Text(7666, Vector3(0.196f, 0.882f, 0.196f)); // New Secondary Objective\n

			Commands->Send_Custom_Event(obj, obj, 0, 27, 2.0f);
		}

		// Received from M01_mission_Controller_JDG when param 93 is received
		else if (param == 104)
		{
			GameObject *entranceBarnAreaTowerNodOfficerObj = Commands->Find_Object(102360);
			if (entranceBarnAreaTowerNodOfficerObj)
			{
				Vector3 entranceBarnAreaTowerNodOfficerObjPos = Commands->Get_Position(entranceBarnAreaTowerNodOfficerObj);
				Commands->Set_Objective_HUD_Info_Position(104, 86.0f, "POG_M01_2_01.tga", 7378, entranceBarnAreaTowerNodOfficerObjPos); // Eliminate
				Commands->Set_HUD_Help_Text(6268, Vector3(0.196f, 0.882f, 0.196f)); // Eliminate Nod Officer
			}
		}

		// Received from M01_mission_Controller_JDG when param 92 is received
		else if (param == 109)
		{
			GameObject *beachNodTurrer1Obj = Commands->Find_Object(101434);
			GameObject *beachNodTurrer2Obj = Commands->Find_Object(101435);

			Vector3 objectivePos;
			if (beachNodTurrer1Obj)
			{
				objectivePos = Vector3(-83.916f, 91.667f, 2.807f);
			}
			else if(beachNodTurrer2Obj)
			{
				objectivePos = Vector3(-105.024f, 113.658f, 1.872f);
			}

			Commands->Set_Objective_HUD_Info_Position(109, 91.0f, "POG_M01_2_02.tga", 7376, objectivePos); // Destroy
			Commands->Set_HUD_Help_Text(6269, Vector3(0.196f, 0.882f, 0.196f)); // Destroy Nod Turrets
		}
		else if (param == 110)
		{
			Commands->Set_Objective_HUD_Info_Position(110, 92.0f, "POG_M01_2_03.tga", 7389, Vector3(-49.91f, 183.86f, 0.774f)); // Rescue
			Commands->Set_HUD_Help_Text(6270, Vector3(0.196f, 0.882f, 0.196f)); // Rescue Civilians
		}
		else if (param == 112)
		{
			Commands->Set_Objective_HUD_Info_Position(112, 93.0f, "POG_M01_2_04.tga", 7381, Vector3(-54.786f, 213.816f, 6.453f)); // Evacuate
			Commands->Set_HUD_Help_Text(6271, Vector3(0.196f, 0.882f, 0.196f)); // Evacuate Civilians
		}

		// Received from M01_mission_Controller_JDG when param 83 is received
		else if (param == 100)
		{
			GameObject *churchPriestObj = Commands->Find_Object(101315);
			Vector3 churchPriestObjPos = Commands->Get_Position(churchPriestObj);

			Commands->Set_Objective_HUD_Info_Position(100, 94.0f, "POG_M01_2_06.tga", 7389, churchPriestObjPos); // Rescue
			Commands->Set_HUD_Help_Text(6272, Vector3(0.196f, 0.882f, 0.196f)); // Rescue Clergy
		}

		// Received from M01_mission_Controller_JDG when param 91 is received
		else if (param == 103)
		{
			Commands->Set_Objective_HUD_Info_Position(103, 95.0f, "POG_M02_2_01.tga", 7376, Vector3(-179.3f, 541.9f, 3.518f)); // Destroy
			Commands->Set_HUD_Help_Text(6274, Vector3(0.196f, 0.882f, 0.196f)); // Disable Hand of Nod
		}

		// Received from M01_mission_Controller_JDG when param 89 is received
		else if (param == 108)
		{
			Commands->Set_Objective_HUD_Info_Position(108, 96.0f, "POG_M01_1_04.tga", 7376, Vector3(-337.169f, 549.232f, 31.949f)); // Destroy
			Commands->Set_HUD_Help_Text(6220, Vector3(0.196f, 0.882f, 0.196f)); // Disable SAM Site
		}
	}
	else if (type == 7)
	{
		// Received from M01_GDIBase_POW_Conversation_Controller_JDG when the conversation ended
		if (param == 102)
		{
			if (Commands->Find_Object(106050)) // Captain Duncan in the shed
			{
				// Contact GDI Commander
				// The GDI base commander is carrying a new weapon prototype. We cannot allow it to fall into Nod hands. Locate the GDI base commander and contact him for details.
				Commands->Add_Objective(102, OBJECTIVE_TYPE_SECONDARY, OBJECTIVE_STATUS_PENDING, 7502, NULL, 7501);
				Commands->Set_Objective_HUD_Info_Position(102, 91.0f, "POG_M01_2_05.tga", 7374, Vector3(109.959f, 97.34f, 8.923f)); // Contact
				Commands->Set_Objective_Radar_Blip(102, Vector3(109.959f, 97.34f, 8.923f));
				
				GameObject *nonExistingObj = Commands->Find_Object(106052);
				if (nonExistingObj)
				{
					Commands->Send_Custom_Event(obj, nonExistingObj, 0, 27, 0.0f);
				}

				Commands->Set_HUD_Help_Text(7502, Vector3(0.196f, 0.882f, 0.196f)); // Contact GDI Commander
			}
		}

		// Received from M01_TurretBeach_Turret_01_Script_JDG when killed
		else if (param == 109)
		{
			Vector3 objectivePos;
			if (Commands->Find_Object(101435)) // Beach Nod Turret 2
			{
				objectivePos = Vector3(-105.024f, 113.658f, 1.872f);
			}

			Commands->Set_Objective_HUD_Info_Position(109, 91.0f, "POG_M01_2_02.tga", 7376, objectivePos); // Destroy
		}	
	}
}

void M01_Objective_Pog_Controller_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED)
	{
		if (action_id == this->objectiveConversationId)
		{
			// Locate Detention Center
			// Satellite imagery indicates the Nod Detention Center is adjacent to the Nod Communications Center. Locate the Nod Detention Center.
			Commands->Add_Objective(106, OBJECTIVE_TYPE_PRIMARY, OBJECTIVE_STATUS_PENDING, 6217, NULL, 6040);
			Commands->Set_Objective_Radar_Blip(106, Vector3(-312.725f, 575.887f, 27.221f));
			Commands->Set_Objective_HUD_Info_Position(106, 80.0f, "POG_M01_1_01.tga", 7384, Vector3(-312.725f, 575.887f, 27.221f)); // Locate
			Commands->Set_HUD_Help_Text(7665, Vector3(0.196f, 0.882f, 0.196f)); // New Primary Objective\n

			this->tutorialConversationId = Commands->Create_Conversation("M01_Press_ESC_For_Objectives_Conversation", 100, 1000.0f, false);
			Commands->Join_Conversation(NULL, this->tutorialConversationId, false, false, true);
			Commands->Start_Conversation(this->tutorialConversationId, this->tutorialConversationId);
			Commands->Monitor_Conversation(obj, this->tutorialConversationId);
		}
		else if (action_id == this->tutorialConversationId)
		{
			Commands->Set_HUD_Help_Text(8273, Vector3(0.196f, 0.882f, 0.196f)); // Press 'O' for Mission Objectives\n
		}
	}
}

ScriptRegistrant<M01_Objective_Pog_Controller_JDG> M01_Objective_Pog_Controller_JDGRegistrant("M01_Objective_Pog_Controller_JDG", "");