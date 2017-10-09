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
#include "M03_Objective_Controller.h"

/*
M03 -> 1100004
*/
void M03_Objective_Controller::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 2);
	Auto_Save_Variable(&this->field_24, sizeof(this->field_24), 3);
	Auto_Save_Variable(&this->field_25, sizeof(this->field_25), 4);
	Auto_Save_Variable(&this->field_34, sizeof(this->field_34), 5);
	Auto_Save_Variable(&this->field_26, sizeof(this->field_26), 6);
	Auto_Save_Variable(&this->field_27, sizeof(this->field_27), 7);
	Auto_Save_Variable(&this->field_28, sizeof(this->field_28), 8);
}

void M03_Objective_Controller::Created(GameObject *obj)
{
	Commands->Set_Num_Tertiary_Objectives(4);
	Commands->Set_Wind(90.0f, 5.0f, 2.0f, 0.0f);

	float randInterval = Commands->Get_Random(20.0f, 40.0f);
	Commands->Start_Timer(obj, this, randInterval, 0);

	this->field_20 = 0;
	this->field_1C = 0;
	this->field_28 = false;
	this->field_27 = false;
	this->field_26 = false;
	this->field_24 = false;
	this->field_25 = true;
}

// TODO
void M03_Objective_Controller::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type >= 300 && type <= 312)
	{
		if (type == 301)
		{
			this->field_34 = Commands->Get_ID(sender);
		}
		else if (type == 302 || type == 304)
		{
			if (!Commands->Find_Object(1100003)) // GDI Gunboat
			{
				return;
			}
		}

		if (param == 1)
		{
			Complete_Mission_Objective(type + 700);
		}
		else if (param == 2)
		{
			Commands->Set_Objective_Status(type + 700, OBJECTIVE_STATUS_FAILED);
		}
		else if (param == 3)
		{
			Add_Mission_Objective(type + 700);
		}
		else if (param == 4)
		{
			if (type == 307)
			{
				Commands->Start_Timer(obj, this, 3.0f, 1007);
			}
			else if (type == 310)
			{
				this->field_25 = false;

				Commands->Set_Objective_Status(1010, OBJECTIVE_STATUS_PENDING);
			}
			else
			{
				Commands->Set_Objective_Status(type + 700, OBJECTIVE_STATUS_PENDING);
			}
		}
	}
	else if (type == 40026)
	{
		if (!this->field_26)
		{
			GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);
			Commands->Send_Custom_Event(obj, M03ObjectiveControllerObj, 301, 2, 0.0f);
		}
		if (!this->field_27)
		{
			GameObject *M03ObjectiveControllerObj_1 = Commands->Find_Object(1100004);
			Commands->Send_Custom_Event(obj, M03ObjectiveControllerObj_1, 302, 2, 0.0f);
		}
		if (!this->field_28)
		{
			GameObject *M03ObjectiveControllerObj_2 = Commands->Find_Object(1100004);
			Commands->Send_Custom_Event(obj, M03ObjectiveControllerObj_2, 304, 2, 0.0f);
		}
	}
	else if (type == 5000)
	{
		if (param == 5000)
		{
			this->field_24 = true;
		}
	}
	else if (type == 40000)
	{
		Commands->Set_Background_Music("Sakura_Dogfight.mp3");
	}
	else if (type == 40001)
	{
		Commands->Set_Background_Music("03-ammoclip.mp3");
	}
	else if (type == 100063)
	{
		GameObject *M03IntroSubsitute = Commands->Find_Object(1100012);
		GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);
		Commands->Send_Custom_Event(M03ObjectiveControllerObj, M03IntroSubsitute, 999, 0, 0.0f);
	}
}

// TODO
void M03_Objective_Controller::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED)
	{
		if (action_id == 1)
		{
			// Disable Lower SAM Sites
			// Nod has SAM Sites located in two positions in this destroyed hamlet. Find and disable them to allow for additional GDI reinforcements.
			Commands->Add_Objective(1002, OBJECTIVE_TYPE_SECONDARY, OBJECTIVE_STATUS_PENDING, 6293, NULL, 6160);
			
			GameObject *destroyedHamletSAMSite1Obj = Commands->Find_Object(300056);
			if (destroyedHamletSAMSite1Obj)
			{
				Commands->Set_Objective_Radar_Blip_Object(1002, destroyedHamletSAMSite1Obj);
				Commands->Set_Objective_HUD_Info(1002, 94.0f, "POG_M03_2_02.tga", 7376); // Destroy
			}
		}
		else if (action_id == 2)
		{
			// Disable Upper SAM Sites
			// Two Nod SAM Sites are protecting a heavy Shore Defense Cannon from aerial assault. Disable these two SAM Sites to facilitate a GDI airstrike on the Shore Defense Cannon.
			Commands->Add_Objective(1004, OBJECTIVE_TYPE_SECONDARY, OBJECTIVE_STATUS_PENDING, 6294, NULL, 6161);

			GameObject *bigGunDavesArrowObj = Commands->Find_Object(1205777);
			if (bigGunDavesArrowObj)
			{
				Commands->Set_Objective_Radar_Blip_Object(1004, bigGunDavesArrowObj);
				Commands->Set_Objective_HUD_Info(1004, 97.0f, "POG_M03_2_02.tga", 7376); // Destroy

				Commands->Send_Custom_Event(obj, obj, 40006, 1004, 15.0f);
			}
		}
		else if (action_id == 100020)
		{
			Commands->Set_Objective_Status(1002, OBJECTIVE_STATUS_ACCOMPLISHED);
		}
	}
}

// TODO
void M03_Objective_Controller::Timer_Expired(GameObject *obj, int number)
{
	if (number == 1004)
	{
		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(47.7f, 78.07f, 20.26f));
		Commands->Set_Facing(invisObj, 0.0f);
		Commands->Attach_Script(invisObj, "Test_Cinematic", "X3I_GDI_Drop_PowerUp.txt");

		Commands->Set_Objective_Status(1004, OBJECTIVE_STATUS_ACCOMPLISHED);
	}
	else if (number == 2004)
	{
		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(47.7f, 78.07f, 20.26f));
		Commands->Set_Facing(invisObj, 0.0f);
		Commands->Attach_Script(invisObj, "Test_Cinematic", "X3I_GDI_Drop_PowerUp.txt");
	}
	else if (number == 1007)
	{
		Commands->Set_Objective_Status(1007, OBJECTIVE_STATUS_PENDING);
	}
	else if (number == 911)
	{
		// Commando, I can't get any reinforcements into the village inlet! Did you take care of those SAMs?
		// Consider it done.
		int conversationId = Commands->Create_Conversation("M03CON024", 99, 2000.0f, true);
		Commands->Join_Conversation(NULL, conversationId, true, true, true);
		
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, true, true);
		Commands->Start_Conversation(conversationId, 100024);
		Commands->Monitor_Conversation(obj, conversationId);
	}
	else if (number == 912)
	{
		if (Commands->Find_Object(1100003)) // GDI Gunboat
		{
			// Commando, I see you've taken the liberty of removing the shore defense cannon for us. Nice going.
			// Wasn't about to let you have all the fun.
			int conversationId = Commands->Create_Conversation("M03CON032", 99, 2000.0f, true);
			Commands->Join_Conversation(NULL, conversationId, true, true, true);
			
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			Commands->Join_Conversation(starObj, conversationId, true, true, true);
			Commands->Start_Conversation(conversationId, 100032);
			Commands->Monitor_Conversation(obj, conversationId);
		}
	}
}

void M03_Objective_Controller::Add_Mission_Objective(int objectiveId)
{
	if (objectiveId == 1000)
	{
		// Locate Communications Center
		// Satellite imagery indicates the Nod Communications Center is located centrally on the island. Make your way to the Nod base and acquire information about the scientists' whereabouts.
		Commands->Add_Objective(objectiveId, OBJECTIVE_TYPE_PRIMARY, OBJECTIVE_STATUS_PENDING, 6226, NULL, 6047);

		GameObject *M03MainframeObj = Commands->Find_Object(1100009);
		if (M03MainframeObj)
		{
			Commands->Set_Objective_Radar_Blip_Object(objectiveId, M03MainframeObj);

			Vector3 M03MainframeObjPos = Commands->Get_Position(M03MainframeObj);
			Commands->Set_Objective_HUD_Info_Position(1000, 96.0f, "POG_M03_1_01.tga", 7384, M03MainframeObjPos); // Locate
		}

		GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);
		Commands->Send_Custom_Event(M03ObjectiveControllerObj, M03ObjectiveControllerObj, 100063, 0, 0.0f);
	}
	else if (objectiveId == 1001)
	{
		// Secure Beachhead
		// GDI forces are landing on the beachhead and encountering heavy resistance. Assist their landing operation by eliminating all Nod resistance on the beachhead.
		Commands->Add_Objective(objectiveId, OBJECTIVE_TYPE_SECONDARY, OBJECTIVE_STATUS_PENDING, 6292, NULL, 6159);

		GameObject *M03CineExplosion1Obj = Commands->Find_Object(1212283);
		if (M03CineExplosion1Obj)
		{
			Commands->Debug_Message("object found");

			Commands->Set_Objective_Radar_Blip_Object(objectiveId, M03CineExplosion1Obj);

			Vector3 M03CineExplosion1ObjPos = Commands->Get_Position(M03CineExplosion1Obj);
			Commands->Set_Objective_HUD_Info_Position(1001, 99.0f, "POG_M03_2_01.tga", 7375, M03CineExplosion1ObjPos); // Defeat

			GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);
			Commands->Send_Custom_Event(M03ObjectiveControllerObj, M03ObjectiveControllerObj, 40006, objectiveId, 15.0f);
		}
	}
	else if (objectiveId == 1002)
	{
		GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);

		// Disable Lower SAM Sites
		// Nod has SAM Sites located in two positions in this destroyed hamlet. Find and disable them to allow for additional GDI reinforcements.
		Commands->Add_Objective(1002, OBJECTIVE_TYPE_SECONDARY, OBJECTIVE_STATUS_PENDING, 6293, NULL, 6160);
		
		GameObject *destroyedHamletSAMSite1Obj = Commands->Find_Object(300056);
		if (destroyedHamletSAMSite1Obj)
		{
			Commands->Set_Objective_Radar_Blip_Object(1002, destroyedHamletSAMSite1Obj);
			
			Vector3 destroyedHamletSAMSite1ObjPos = Commands->Get_Position(destroyedHamletSAMSite1Obj);
			Commands->Set_Objective_HUD_Info_Position(1002, 98.0f, "POG_M03_2_02.tga", 7376, destroyedHamletSAMSite1ObjPos); // Destroy

			Commands->Send_Custom_Event(M03ObjectiveControllerObj, M03ObjectiveControllerObj, 40006, objectiveId, 15.0f);
		}

		Commands->Start_Timer(M03ObjectiveControllerObj, this, 15.0f, 911);
	}
	else if (objectiveId == 1004)
	{
		if (Commands->Find_Object(1100003) && Commands->Find_Object(1100002))// GDI Gunboat & Big Gun
		{
			// Commando, you still around? We've got more SAM Sites behind that Shore Defense Cannon. That thing has to be removed, or I'm going down!
			// Bring it on!
			int conversationId = Commands->Create_Conversation("M03CON026", 0, 2000.0f, false);
			Commands->Join_Conversation(NULL, conversationId, true, true, true);
			
			Vector3 pos = Commands->Get_Position(Owner());
			GameObject *starObj = Commands->Get_A_Star(pos);

			Commands->Join_Conversation(starObj, conversationId, true, true, true);
			Commands->Start_Conversation(conversationId, 2);
			Commands->Monitor_Conversation(Owner(), conversationId);
	
		}
	}
	else if (objectiveId == 1006)
	{
		// Destroy Defense Cannon
		// Destroying the Shore Defense Cannon before the SAM Sites was a quick solution to an otherwise complicated problem.
		Commands->Add_Objective(objectiveId, OBJECTIVE_TYPE_TERTIARY, OBJECTIVE_STATUS_PENDING, 6203, NULL, 6015);

		GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);
		Commands->Start_Timer(M03ObjectiveControllerObj, this, 4.0f, 912);
	}
	else if (objectiveId == 1007)
	{
		// Acquire Security Card
		// Access to the Communications Center mainframe is locked behind a secure door. Nod officers usually carry security cards, and you will need one to pass through.
		Commands->Add_Objective(objectiveId, OBJECTIVE_TYPE_PRIMARY, OBJECTIVE_STATUS_PENDING, 6227, NULL, 6048);
		
		GameObject *refineryGratingDavesArrowObj = Commands->Find_Object(1215546);
		if (refineryGratingDavesArrowObj)
		{
			Commands->Set_Objective_Radar_Blip_Object(1007, refineryGratingDavesArrowObj);

			Vector3 refineryGratingDavesArrowObjPos = Commands->Get_Position(refineryGratingDavesArrowObj);
			Commands->Set_Objective_HUD_Info_Position(1007, 95.0f, "POG_M03_1_02.tga", 7384, refineryGratingDavesArrowObjPos); // Locate
		}
	}
	else if (objectiveId == 1008)
	{
		// Hack Communications Mainframe
		// The Nod Communications Center Mainframe holds recent flight data for passing Nod aircraft. Approach the mainframe and hack it to find the destination of the scientists.
		Commands->Add_Objective(objectiveId, OBJECTIVE_TYPE_PRIMARY, OBJECTIVE_STATUS_PENDING, 6228, NULL, 6049);
		
		GameObject *M03MainframeObj = Commands->Find_Object(1100009);
		if (M03MainframeObj)
		{
			Commands->Set_Objective_Radar_Blip_Object(1008, M03MainframeObj);

			Vector3 M03MainframeObjPos = Commands->Get_Position(M03MainframeObj);
			Commands->Set_Objective_HUD_Info_Position(1008, 94.0f, "POG_M03_1_03.tga", 7370, M03MainframeObjPos); // Access
		}
	}
	else if (objectiveId == 1009)
	{
		// Disable Power Plant
		// Disabling the Nod Power Plant has shut down power for the base area, reducing the capabilities of Nod forces in this locale.
		Commands->Add_Objective(objectiveId, OBJECTIVE_TYPE_PRIMARY, OBJECTIVE_STATUS_PENDING, 6204, NULL, 6016);
	}
	else if (objectiveId == 1010)
	{
		// Escape via Submarine
		// Looks like the dock is your only way off the island. A small submarine is located nearby. Make your way on board to escape the destruction.
		Commands->Add_Objective(objectiveId, OBJECTIVE_TYPE_PRIMARY, OBJECTIVE_STATUS_PENDING, 6229, NULL, 6050);
		
		GameObject *subDavesArrowObj = Commands->Find_Object(1213908);
		if (subDavesArrowObj)
		{
			Commands->Set_Objective_Radar_Blip_Object(1010, subDavesArrowObj);

			Vector3 subDavesArrowObjPos = Commands->Get_Position(subDavesArrowObj);
			Commands->Set_Objective_HUD_Info_Position(1010, 93.0f, "POG_M03_1_04.tga", 7379, subDavesArrowObjPos); // Escape
		}
	}
	else if (objectiveId == 1011)
	{
		// Disable Tiberium Refinery
		// Disabling the Tiberium Refinery has hampered Nod production in this base facility.
		Commands->Add_Objective(objectiveId, OBJECTIVE_TYPE_TERTIARY, OBJECTIVE_STATUS_PENDING, 6205, NULL, 6017);
	}
	else if (objectiveId == 1012)
	{
		// Disable Communications Center
		// Disabling the Nod Communications Center has disrupted Nod communications throughout the area.
		Commands->Add_Objective(objectiveId, OBJECTIVE_TYPE_TERTIARY, OBJECTIVE_STATUS_PENDING, 6206, NULL, 6018);
	}
}

void M03_Objective_Controller::Complete_Mission_Objective(int objectiveId)
{
	if (objectiveId == 1000)
	{
		GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);
		GameObject *commCenterBuildingObj = Commands->Find_Object(1150002);
		Commands->Send_Custom_Event(M03ObjectiveControllerObj, commCenterBuildingObj, 40023, 0, 0.0f);

		Commands->Set_Objective_Status(objectiveId, OBJECTIVE_STATUS_ACCOMPLISHED);
	}
	else if (objectiveId == 1009)
	{
		// Disable Power Plant
		// Disabling the Nod Power Plant has shut down power for the base area, reducing the capabilities of Nod forces in this locale.
		Commands->Add_Objective(objectiveId, OBJECTIVE_TYPE_TERTIARY, OBJECTIVE_STATUS_PENDING, 6204, NULL, 6016);
		Commands->Set_Objective_Status(objectiveId, OBJECTIVE_STATUS_ACCOMPLISHED);
		
		// I see you're keeping yourself busy.
		// Figured you'd want it destroyed. Just workin' on that promotion.
		int conversationId = Commands->Create_Conversation("M03CON033", 99, 2000.0f, false);
		Commands->Join_Conversation(NULL, conversationId, true, true, true);

		Vector3 pos = Commands->Get_Position(Owner());
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, true, true);
		Commands->Start_Conversation(conversationId, 100033);

		GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);
		Commands->Monitor_Conversation(M03ObjectiveControllerObj, conversationId);
	}
	else if (objectiveId == 1011)
	{
		// Disable Tiberium Refinery
		// Disabling the Tiberium Refinery has hampered Nod production in this base facility.
		Commands->Add_Objective(1011, OBJECTIVE_TYPE_TERTIARY, OBJECTIVE_STATUS_PENDING, 6205, NULL, 6017);
		Commands->Set_Objective_Status(1011, OBJECTIVE_STATUS_ACCOMPLISHED);
		
		// You're really going to town down there. You didn't forget about our scientists?
		int conversationId = Commands->Create_Conversation("M03CON035", 99, 2000.0, 0);
		Commands->Join_Conversation(NULL, conversationId, true, true, true);

		Vector3 pos = Commands->Get_Position(Owner());
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, true, true);
		Commands->Start_Conversation(conversationId, 100035);

		GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);
		Commands->Monitor_Conversation(M03ObjectiveControllerObj, conversationId);
	}
	else if (objectiveId == 1012)
	{
		// Disable Communications Center
		// Disabling the Nod Communications Center has disrupted Nod communications throughout the area.
		Commands->Add_Objective(1012, OBJECTIVE_TYPE_TERTIARY, OBJECTIVE_STATUS_PENDING, 6206, NULL, 6018);
		Commands->Set_Objective_Status(1012, OBJECTIVE_STATUS_ACCOMPLISHED);

		// I know it's your job, but do you absolutely have to blow up everything?
		// Ha ha!
		int conversationId = Commands->Create_Conversation("M03CON037", 99, 2000.0f, NULL);
		Commands->Join_Conversation(NULL, conversationId, true, true, true);
		
		Vector3 pos = Commands->Get_Position(Owner());
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, true, true);
		Commands->Start_Conversation(conversationId, 100037);

		GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);
		Commands->Monitor_Conversation(M03ObjectiveControllerObj, conversationId);
	}
	else if (objectiveId == 1001)
	{
		this->field_26 = true;

		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-96.2f, -75.8f, 1.4f));
		Commands->Set_Facing(invisObj, 0.0f);
		Commands->Attach_Script(invisObj, "Test_Cinematic", "X3I_GDI_Drop_PowerUp.txt");

		GameObject *gdiGunBoatObj = Commands->Find_Object(1100003);
		if (gdiGunBoatObj)
		{
			if (!this->field_24)
			{
				Commands->Send_Custom_Event(Owner(), gdiGunBoatObj, 2000, 1, 0.0f);

				GameObject *brokenAirplaneWingScriptZoneObj = Commands->Find_Object(1100000);
				Commands->Destroy_Object(brokenAirplaneWingScriptZoneObj);
			}
		}

		Commands->Set_Objective_Status(objectiveId, OBJECTIVE_STATUS_ACCOMPLISHED);
	}
	else if (objectiveId == 1002)
	{
		if (++this->field_1C >= 2)
		{
			this->field_27 = true;

			// Sure am glad you're here Commando. Good job on the SAMs.
			// Ha ha!
			int conversationId = Commands->Create_Conversation("M03CON020", 99, 2000.0f, false);
			Commands->Join_Conversation(NULL, conversationId, true, true, true);
			
			Vector3 pos = Commands->Get_Position(Owner());
			GameObject *starObj = Commands->Get_A_Star(pos);

			Commands->Join_Conversation(starObj, conversationId, true, true, true);
			Commands->Start_Conversation(conversationId, 100020);

			GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);
			Commands->Monitor_Conversation(M03ObjectiveControllerObj, conversationId);

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(73.0f, -80.04f, 0.2f));
			Commands->Set_Facing(invisObj, 0.0f);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X3I_GDI_Drop_PowerUp.txt");

			GameObject *pastDestroyedHamletObjectiveZoneObj = Commands->Find_Object(1100001);
			Commands->Send_Custom_Event(M03ObjectiveControllerObj, pastDestroyedHamletObjectiveZoneObj, 40004, 0, 0.0f);
		}
	}
	else if (objectiveId == 1004)
	{
		if (++this->field_20 >= 2)
		{
			this->field_28 = true;

			Commands->Set_Objective_Status(1004, OBJECTIVE_STATUS_ACCOMPLISHED);

			GameObject *gdiGunBoatObj = Commands->Find_Object(1100003);
			if (gdiGunBoatObj)
			{
				GameObject *M03ObjectiveControllerObj = Commands->Find_Object(1100004);
				if (Commands->Find_Object(1100002)) // Big Gun
				{
					int conversationId = Commands->Create_Conversation("M03CON027", 99, 2000.0f, false); // That takes care of 'em! I'm calling in an airstrike on the cannon now, stand clear!
					Commands->Join_Conversation(NULL, conversationId, true, true, true);
					Commands->Start_Conversation(conversationId, 100027);
					Commands->Monitor_Conversation(M03ObjectiveControllerObj, conversationId);

					Commands->Start_Timer(Owner(), this, 4.0f, 1004);
				}
				else
				{
					int conversationId = Commands->Create_Conversation("M03CON028", 99, 2000.0f, true); // Hey, you took care of everything. You want to raid the base for me while you're at it?
					Commands->Join_Conversation(NULL, conversationId, true, true, true);
					Commands->Start_Conversation(conversationId, 100028);
					Commands->Monitor_Conversation(M03ObjectiveControllerObj, conversationId);

					Commands->Start_Timer(Owner(), this, 4.0f, 2004);
				}
			}
		}
	}
}

ScriptRegistrant<M03_Objective_Controller> M03_Objective_ControllerRegistrant("M03_Objective_Controller", "");