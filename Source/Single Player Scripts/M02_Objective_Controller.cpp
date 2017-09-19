#include "General.h"
#include "M02_Objective_Controller.h"

void M02_Objective_Controller::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->nodConvoyTruckCount, sizeof(this->nodConvoyTruckCount), 1);
	Auto_Save_Variable(&this->area0GDIDeathsSinceLastDrop, sizeof(this->area0GDIDeathsSinceLastDrop), 2);
	Auto_Save_Variable(&this->area9GDIDeathsSinceLastDrop, sizeof(this->area9GDIDeathsSinceLastDrop), 3);
}

// On level start
void M02_Objective_Controller::Created(GameObject *obj)
{
	Commands->Reveal_Encyclopedia_Character(37); // Nod_FlameThrower_3Boss
	Commands->Reveal_Encyclopedia_Character(4); // GDI_Ignatio_Mobius
	Commands->Reveal_Encyclopedia_Character(10); // GDI_Sydney
	Commands->Reveal_Encyclopedia_Character(8); // Civilian_Petrova

	Commands->Set_Background_Music("02-Packing Iron.mp3");

	GameObject *damControlPanelObj = Commands->Find_Object(1111116);
	if (damControlPanelObj)
	{
		Commands->Set_Animation_Frame(damControlPanelObj, "MCT_NOD.MCT_NOD", 0);
	}

	this->area0GDIDeathsSinceLastDrop = 0;
	this->area9GDIDeathsSinceLastDrop = 0;
	this->havocCalledForReinforcementsArea0 = false;
	this->nodConvoyTruckCount = 3;

	Commands->Enable_Hibernation(obj, false);

	Commands->Set_HUD_Help_Text(7665, Vector3(0.196f, 0.882f, 0.196f)); // New Primary Objective\n

	// Locate Missing Scientists
	// The civilian scientists are most likely being held in the nearby Nod base. Locate the missing scientists.
	Commands->Add_Objective(201, 1, 0, 6222, 0, 6043);

	Commands->Set_Objective_HUD_Info_Position(201, 50.0f, "POG_M08_1_04.tga", 7384, Vector3(1208.31f, 562.49f, 55.02f)); // Locate

	// Disable Airstrip
	// Disabling of the Nod Airstrip has hampered the ability to fly in new vehicle reinforcements to the base area.
	Commands->Add_Objective(223, 3, 3, 6201, 0, 6013);

	// Destroy Tiberium Silos
	// Disabling the Nod Tiberium Silos has cut into Nod production capabilities, by reducing their funding.
	Commands->Add_Objective(222, 3, 3, 6202, 0, 6014);

	Commands->Start_Timer(obj, this, 2.0f, 201);
}

void M02_Objective_Controller::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type > 199)
	{
		// Received from M02_Objective_Zone with objId = 
		// (401079 + type = 218/219/220, 
		//	401101 + type = 204, 
		//	401102 + type = 204, 
		//	401080 + type = 218/219/220, 
		//	400267 + type = 211,
		//	400269 + type = 202,
		//	400269 + type = 203,
		//	400269 + type = 217,
		//	400501 + type = 214,
		//	400502 + type = 214,
		//	400187 + type = 211,
		//	400184 + type = 206,
		//	400184 + type = 207,
		//	400186 + type = 210,
		//	400186 + type = 221,
		//	400185 + type = 208,
		//	400185 + type = 209,
		//	400189 + type = 215,
		//	400189 + type = 216,
		//	400188 + type = 212,
		//	400188 + type = 213)
		// Received from M02_Objective_Zone when timer number 9 triggered with type = 205
		if (!param)
		{
			Add_An_Objective(obj, type);

			Commands->Start_Timer(obj, this, 2.0f, type);
		}

		// Received from M02_Dam_MCT when 'destroyed' (type = 203)
		// Received from M02_Destroy_Objective (type = 204, 212, 214, 215, 216, 218, 220, 223)
		// Received from M02_Helipad when killed (type = 206)
		// Received from M02_Obelisk when killed (type = 202)
		// Received from M02_Player_Vehicle when entered (type = 221)
		// Received from M02_Power_Plant when killed (type = 217, 202)
		// Received from M02_Objective_Zone with objId = 400194 (type = 205)
		// Received when M02_Objective_Zone::Send_and_Destroy is called with types 207,208,209,210,211
		// Received from M02_Objective_Zone when timer number 9 triggered with type = 201
		// Received from M02_Respawn_Controller when type 116 has been received with type = 222
		else if (param == 1)
		{
			Commands->Set_HUD_Help_Text(7667, Vector3(0.196f, 0.882f, 0.196f)); // Objective Accomplished\n
			Commands->Set_Objective_Status(type, 1);
			Commands->Start_Timer(obj, this, 2.0f, type + 200);
		}

		// Received from M02_Player_Vehicle when destroyed type = 221
		else if (param == 2)
		{
			Commands->Set_Objective_Status(type, 2);
		}

		// Received from M02_Nod_Convoy_Truck when killed (type = 900)
		else if (param == 3)
		{
			if (--this->nodConvoyTruckCount)
			{
				return;
			}

			Commands->Set_HUD_Help_Text(7667, Vector3(0.196f, 0.882f, 0.196f)); // Objective Accomplished\n
			Commands->Set_Objective_Status(213, 1);
			Commands->Stop_All_Conversations();

			int conversationId = Commands->Create_Conversation("M02_SECONDARY_09_FINISH", 100, 300.0f, true); // One less convoy to worry about. Good going, Havoc.
			Commands->Join_Conversation(NULL, conversationId, true, true, true);

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			Commands->Join_Conversation(starObj, conversationId, true, false, false);
			
			Commands->Start_Conversation(conversationId, 0);
		}
		else
		{
			// Received from M02_Objective_Zone with objId = 400193 and param = 1002
			// Recieved from M02_GDI_Soldier when killed with param = 1000 (areaId <= 0) or 1001 (areaId = 9)
			if (type == 1000)
			{
				// M02_GDI_Soldier
				if (param == 1000)
				{
					if (++this->area0GDIDeathsSinceLastDrop > 1)
					{
						this->area0GDIDeathsSinceLastDrop = 0;

						GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-10.6f, -25.1f, -38.2f));
						if (invisObj)
						{
							Commands->Set_Facing(invisObj, 70.5f);
							Commands->Attach_Script(invisObj, "Test_Cinematic", "X2I_GDI_Drop02_Rocket.txt");
						}

						if (!this->havocCalledForReinforcementsArea0)
						{
							this->havocCalledForReinforcementsArea0 = true;

							Commands->Stop_All_Conversations();

							int conversationId = Commands->Create_Conversation("M02_MORE_ROCKET_SOLDIERS", 100, 300.0f, true); // You got any more of those rocket soldiers for me? I'm running low.
							Commands->Join_Conversation(NULL, conversationId, true, true, true);

							Vector3 pos = Commands->Get_Position(obj);
							GameObject *starObj = Commands->Get_A_Star(pos);

							Commands->Join_Conversation(starObj, conversationId, true, false, false);

							Commands->Start_Conversation(conversationId, 0);
						}
					}

					// M02_GDI_Soldier
					else if (param == 1001)
					{
						if (++this->area9GDIDeathsSinceLastDrop > 2)
						{
							this->area9GDIDeathsSinceLastDrop = 0;

							GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(819.38f, 884.05f, 24.5f));
							if (invisObj)
							{
								Commands->Set_Facing(invisObj, 110.0f);
								Commands->Attach_Script(invisObj, "Test_Cinematic", "X2I_GDI_Drop03_Minigunner.txt");
							}
						}
					}

					// M02_Objective_Zone
					else if (param == 1002)
					{
						Vector3 pos = Commands->Get_Position(obj);
						GameObject *starObj = Commands->Get_A_Star(pos);

						Commands->Give_PowerUp(starObj, "Level_01_Keycard", true);
					}
				}
			}
		}
	}
}

void M02_Objective_Controller::Timer_Expired(GameObject *obj, int number)
{
	const char *conversationName;

	Commands->Stop_All_Conversations();

	switch (number)
	{
		case 403: // Triggered after custom type 203 is received with param = 1
			conversationName = "M02_PRIMARY_03_FINISH"; // That's got it! The bay door should be open now.

			break;
		case 410: // Triggered after custom type 210 is received with param = 1
			conversationName = "M02_SECONDARY_06_FINISH"; // Bravo! Havoc, you're a miracle in action.

			break;
		case 414: // Triggered after custom type 214 is received with param = 1
			conversationName = "M02_SECONDARY_10_FINISH"; // One SAM down, EVA shows two to go. Good job.

			break;
		case 416: // Triggered after custom type 216 is received with param = 1
			conversationName = "M02_SECONDARY_12_FINISH"; // It's a pleasure to see you at work, Havoc.

			break;
		case 417: // Triggered after custom type 217 is received with param = 1
			// That's enough, Havoc. Good work.
			// With the power plant down, that obelisk is no longer a threat
			conversationName = "M02_SECONDARY_13_FINISH";

			break;
		case 411: // Triggered after custom type 211 is received with param = 1
			// Checkpoint secure, Havoc. Good show.
			// Next!
			conversationName = "M02_SECONDARY_07_FINISH";

			Commands->Start_Timer(obj, this, 5.0f, 11); // No timer for this one
			break;
		case 412: // Triggered after custom type 212 is received with param = 1
			conversationName = "M02_SECONDARY_08_FINISH"; // Rocket Launcher ammunition, coming down.

			break;
		case 407: // Triggered after custom type 207 is received with param = 1
			conversationName = "M02_SECONDARY_03_FINISH"; // Objective secure, nice one Havoc. Keep an eye out for additional reinforcements.

			break;
		case 408: // Triggered after custom type 208 is received with param = 1
			// Excellent pace, Havoc. Make sure both buildings are secure.
			// Your boys aren't helping much. You're gonna need more.
			conversationName = "M02_SECONDARY_04_FINISH";

			break;
		case 409: // Triggered after custom type 209 is received with param = 1
			// Checkpoint secure. Keep up the good work, Havoc.
			// Makin' me feel special, Locke.
			conversationName = "M02_SECONDARY_05_FINISH";

			break;
		case 404: // Triggered after custom type 204 is received with param = 1
			conversationName = "M02_SECONDARY_01_FINISH"; // Good show Havoc! That should slow them down some.

			break;
		case 406: // Triggered after custom type 206 is received with param = 1
			conversationName = "M02_SECONDARY_02_FINISH"; // The Helipad is down. Nice work Havoc.

			break;
		case 208: // Triggered after custom type 208 is received with param = 0
			// Your next two checkpoints are in the houses across the bridge.Eliminate all resistance and occupy the houses.
			conversationName = "M02_SECONDARY_04_START";

			break;
		case 212: // Triggered after custom type 212 is received with param = 0
			// Havoc, take out that SAM Site and I'll airdrop some supplies for you.
			// Can you drop me some rocket salvos? I get the feeling I'm going to need more of those.

			conversationName = "M02_SECONDARY_08_START";

			break;
		case 214: // Triggered after custom type 214 is received with param = 0
			conversationName = "M02_SECONDARY_10_START"; // SATCOM shows another SAM Site nearby.Watch for engineers repairing it.

			break;
		case 218: // Triggered after custom type 218 is received with param = 0
			// Havoc, EVA shows several machines in the area emitting a unique radiation signature. Eliminate the three machines to hamper Nod Tiberium research.
			// If the scientists are anywhere, they've got to be here.
			conversationName = "M02_SECONDARY_14_START";

			break;
		case 210: // Triggered after custom type 210 is received with param = 0
			// This is Bulldog three - Engaging NOD armor - they got one of our orcas, Havoc <radioized>
			// You can take 'em with the Mammoth.<radioized>
			// We lost her crew to NOD Chem Troopers, so be careful! <radioized>
			conversationName = "MX2DSGN_DSGN0004";

			break;
		case 211: // Triggered after custom type 211 is received with param = 0
			// Nod's next major checkpoint is on the other side of this hill. Take the dirt road past the guard towers.
			conversationName = "M02_SECONDARY_07_START";

			break;
		case 204: // Triggered after custom type 204 is received with param = 0
			// You'll encounter less enemy resistance if you destroy the Hand of Nod. You remember where the Master Control Terminal is?
			// I know this drill all too well.
			conversationName = "M02_SECONDARY_01_START";

			break;
		case 10: // Triggered 120 seconds after timer number 205
			// Don't forget about securing those houses, Havoc.
			// I've got better things to do. Isn't that why we call them secondary objectives?
			conversationName = "M02_PRIMARY_04_REMIND";

			break;
		case 201: // Triggered after custom type 201 is received with param = 0
			// Havoc, locate the Nod base facility and recover the scientists. Here's a Humm-Vee to get you started.
			conversationName = "M02_PRIMARY_01_START";

			break;
		case 205: // Triggered after custom type 205 is received with param = 0
			// Well, we know where the scientists are. SATCOM indicates that Nod Cargo Plane is your only ticket out of here. Get on board and follow the scientists.
			conversationName = "M02_PRIMARY_04_START";

			Commands->Start_Timer(obj, this, 120.0f, 10);
			break;
		case 206: // Triggered after custom type 206 is received with param = 0
			// Havoc, Take out that Helipad and you'll encounter less Apaches along the route, as it takes them longer to arrive.
			conversationName = "M02_SECONDARY_02_START";

			break;
		default:
			conversationName = NULL;

			break;
	}

	int conversationId = 0;
	if (conversationName != NULL)
	{
		conversationId = Commands->Create_Conversation(conversationName, 100, 300.0f, true);
	}

	if (conversationId)
	{
		Commands->Join_Conversation(NULL, conversationId, true, true, true);
		
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, false, false);
		Commands->Start_Conversation(conversationId, 0);
	}
}

void M02_Objective_Controller::Add_An_Objective(GameObject *obj, int objectiveId)
{
	static const Vector3 objectivePositions[20] =
	{
		Vector3(1113.53f, 877.16f, 17.47f),
		Vector3(1001.69f, 903.86f, 0.82f),
		Vector3(1209.6f, 571.97f, 18.15f),
		Vector3(1163.07f, 513.01f, 20.52f),
		Vector3(139.51f, -5.62f, -12.83f),
		Vector3(130.88f, 21.1f, -8.49f),
		Vector3(556.17f, 23.18f, -53.88f),
		Vector3(495.01f, 114.7f, -55.84f),
		Vector3(648.77f, 300.74f, -59.99f),
		Vector3(425.43f, 840.52f, 8.09f),
		Vector3(468.59f, 582.74f, -43.48f),
		Vector3(493.76f, 619.03f, -36.62f),
		Vector3(572.8f, 873.57f, -2.83f),
		Vector3(801.57f, 1054.95f, 22.0f),
		Vector3(783.25f, 945.26f, 23.04f),
		Vector3(1062.0f, 978.7f, -18.3f),
		Vector3(1300.2f, 614.1f, 19.2f),
		Vector3(1310.1f, 696.5f, 19.5f),
		Vector3(1342.2f, 655.1f, 19.3f),
		Vector3(570.06f, 261.47f, -60.0f)
	};

	switch (objectiveId)
	{
		case 202:
			Commands->Set_HUD_Help_Text(7665, Vector3(0.196f, 0.882f, 0.196f)); // New Primary Objective\n

			// Disable Obelisk
			// The Obelisk is a deadly defense weapon, but it requires power to operate. If you cannot get inside to disable it, consider removing the power source.
			Commands->Add_Objective(objectiveId, 2, 0, 6223, 0, 6044);
			Commands->Set_Objective_Radar_Blip(objectiveId, objectivePositions[0]);
			Commands->Set_Objective_HUD_Info_Position(objectiveId, 66.0f, "POG_M07_2_05.tga", 7377, objectivePositions[0]); // Disable

			break;
		case 203:
			Commands->Set_HUD_Help_Text(7665, Vector3(0.196f, 0.882f, 0.196f)); // New Primary Objective\n

			// Disable Dam Master Control Terminal
			// The Master Control Terminal inside the dam controls the large bay door on top. Disabling this terminal will not only open the door, it will prevent Nod from using this dam as a secondary power source.
			Commands->Add_Objective(objectiveId, 1, 0, 6224, 0, 6045);
			Commands->Set_Objective_Radar_Blip(objectiveId, objectivePositions[1]);
			Commands->Set_Objective_HUD_Info_Position(objectiveId, 67.0f, "POG_M02_1_03.tga", 7376, objectivePositions[1]); // Destroy

			break;
		case 204:
			Commands->Set_HUD_Help_Text(7666, Vector3(0.196f, 0.882f, 0.196f)); // New Secondary Objective\n

			// Disable Hand of Nod
			// Disabling the Hand of Nod will dramatically hamper reinforcements for this area. Consider using an Ion Cannon Beacon if you have one.
			Commands->Add_Objective(objectiveId, 2, 0, 6275, 0, 6142);
			Commands->Set_Objective_Radar_Blip(objectiveId, objectivePositions[2]);
			Commands->Set_Objective_HUD_Info_Position(objectiveId, 80.0f, "POG_M02_2_01.tga", 7376, objectivePositions[2]); // Destroy

			break;
		case 205:
			Commands->Set_HUD_Help_Text(7665, Vector3(0.196f, 0.882f, 0.196f)); // New Primary Objective\n

			// Infiltrate Cargo Plane
			// Approach the Cargo Plane from the rear to pursue the scientists.
			Commands->Add_Objective(objectiveId, 1, 0, 6225, 0, 6046);
			Commands->Set_Objective_Radar_Blip(objectiveId, objectivePositions[3]);
			Commands->Set_Objective_HUD_Info_Position(objectiveId, 80.0f, "POG_M02_1_04.tga", 7373, objectivePositions[3]); // Capture

			break;
		case 206:
			Commands->Set_HUD_Help_Text(7666, Vector3(0.196f, 0.882f, 0.196f)); // New Secondary Objective\n

			// Disable Helipad
			// Disable the Nod Helipad to reduce Apache encounters throughout this area. This will force them to refuel at another location.
			Commands->Add_Objective(objectiveId, 2, 0, 6276, 0, 6143);
			Commands->Set_Objective_Radar_Blip(objectiveId, objectivePositions[4]);
			Commands->Set_Objective_HUD_Info_Position(objectiveId, 53.0f, "POG_M02_2_02.tga", 7376, objectivePositions[4]); // Destroy

			break;
		case 207:
			Commands->Set_HUD_Help_Text(7666, Vector3(0.196f, 0.882f, 0.196f)); // New Secondary Objective\n

			// Secure Guard Tower
			// This checkpoint is held by the Nod Guard Tower on top of the hill. Eliminate all enemy resistance and then secure the tower to allow the GDI assault to progress.
			Commands->Add_Objective(objectiveId, 2, 0, 6277, 0, 6144);
			Commands->Set_Objective_Radar_Blip(objectiveId, objectivePositions[5]);
			Commands->Set_Objective_HUD_Info_Position(objectiveId, 54.0f, "POG_M02_2_03.tga", 7378, objectivePositions[5]); // Eliminate

			break;
		case 208:
			Commands->Set_HUD_Help_Text(7666, Vector3(0.196f, 0.882f, 0.196f)); // New Secondary Objective\n

			// Secure Second House
			// Another house occupied by Nod soldiers, potentially the location of the Nod Officer coordinating with Command for reinforcements. Eliminate all enemies in the surrounding area and then enter the house to secure this objective.
			Commands->Add_Objective(objectiveId, 2, 0, 6279, 0, 6146);
			Commands->Set_Objective_Radar_Blip(objectiveId, objectivePositions[6]);
			Commands->Set_Objective_HUD_Info_Position(objectiveId, 56.0f, "POG_M02_2_06.tga", 7378, objectivePositions[6]); // Eliminate

			break;
		case 209:
			Commands->Set_HUD_Help_Text(7666, Vector3(0.196f, 0.882f, 0.196f)); // New Secondary Objective\n

			// Secure First House
			// Nod occupies several of the houses in this area, and GDI needs to secure them all. Eliminate all enemies in the surrounding area and then enter the building to secure this objective.
			Commands->Add_Objective(objectiveId, 2, 0, 6278, 0, 6145);
			Commands->Set_Objective_Radar_Blip(objectiveId, objectivePositions[7]);
			Commands->Set_Objective_HUD_Info_Position(objectiveId, 55.0f, "POG_M02_2_05.tga", 7378, objectivePositions[7]); // Eliminate

			break;
		case 210:
			Commands->Set_HUD_Help_Text(7666, Vector3(0.196f, 0.882f, 0.196f)); // New Secondary Objective\n

			// Secure Third House
			// This house is being used by an officer who is coordinating with Command for reinforcements. Eliminate the Nod Officer and all nearby reinforcements, then enter the house to secure this objective.
			Commands->Add_Objective(objectiveId, 2, 0, 6280, 0, 6147);
			Commands->Set_Objective_Radar_Blip(objectiveId, objectivePositions[8]);
			Commands->Set_Objective_HUD_Info_Position(objectiveId, 57.0f, "POG_M02_2_04.tga", 7378, objectivePositions[8]); // Eliminate

			break;
		case 211:
			Commands->Set_HUD_Help_Text(7666, Vector3(0.196f, 0.882f, 0.196f)); // New Secondary Objective\n

			// Secure Ski Resort
			// Nod is using this checkpoint to refuel their defensive line. Eliminate all enemy resistance and then secure the building front.
			Commands->Add_Objective(objectiveId, 2, 0, 6281, 0, 6148);
			Commands->Set_Objective_Radar_Blip(objectiveId, objectivePositions[9]);
			Commands->Set_Objective_HUD_Info_Position(objectiveId, 61.0f, "POG_M02_2_07.tga", 7378, objectivePositions[9]); // Eliminate

			break;
		case 212:
			Commands->Set_HUD_Help_Text(7666, Vector3(0.196f, 0.882f, 0.196f)); // New Secondary Objective\n

			// Disable First SAM Site
			// Disabling this SAM Site will improve local air superiority for GDI. Consider using C4 to destroy this emplacement.
			Commands->Add_Objective(objectiveId, 2, 0, 6282, 0, 6149);
			Commands->Set_Objective_Radar_Blip(objectiveId, objectivePositions[10]);
			Commands->Set_Objective_HUD_Info_Position(objectiveId, 59.0f, "POG_M02_2_08.tga", 7376, objectivePositions[10]); // Destroy

			break;
		case 213:
			Commands->Set_HUD_Help_Text(7666, Vector3(0.196f, 0.882f, 0.196f)); // New Secondary Objective\n

			// Destroy Nod Convoy
			// The convoy trucks in this area are transporting supplies toward the battle front. Eliminate all three supply trucks to complete this objective.
			Commands->Add_Objective(objectiveId, 2, 0, 6283, 0, 6150);
			Commands->Set_Objective_Radar_Blip(objectiveId, objectivePositions[11]);
			Commands->Set_Objective_HUD_Info_Position(objectiveId, 60.0f, "POG_M02_2_09.tga", 7376, objectivePositions[11]); // Destroy

			break;
		case 214:
			Commands->Set_HUD_Help_Text(7666, Vector3(0.196f, 0.882f, 0.196f)); // New Secondary Objective\n

			// Disable Second SAM Site
			// Disabling this SAM Site will improve local air superiority for GDI. Consider using C4 to destroy this emplacement.
			Commands->Add_Objective(objectiveId, 2, 0, 6284, 0, 6151);
			Commands->Set_Objective_Radar_Blip(objectiveId, objectivePositions[12]);
			Commands->Set_Objective_HUD_Info_Position(objectiveId, 62.0f, "POG_M02_2_10.tga", 7376, objectivePositions[12]); // Destroy

			break;
		case 215:
			Commands->Set_HUD_Help_Text(7666, Vector3(0.196f, 0.882f, 0.196f)); // New Secondary Objective\n

			// Disable Third SAM Site
			// Disabling this SAM Site will improve local air superiority for GDI. Consider using C4 to destroy this emplacement.
			Commands->Add_Objective(objectiveId, 2, 0, 6285, 0, 6152);
			Commands->Set_Objective_Radar_Blip(objectiveId, objectivePositions[13]);
			Commands->Set_Objective_HUD_Info_Position(objectiveId, 63.0f, "POG_M02_2_11.tga", 7376, objectivePositions[13]); // Destroy

			break;
		case 216:
			Commands->Set_HUD_Help_Text(7666, Vector3(0.196f, 0.882f, 0.196f)); // New Secondary Objective\n

			// Disable Fourth SAM Site
			// Disabling this SAM Site will improve local air superiority for GDI. Consider using C4 to destroy this emplacement.
			Commands->Add_Objective(objectiveId, 2, 0, 6286, 0, 6153);
			Commands->Set_Objective_Radar_Blip(objectiveId, objectivePositions[14]);
			Commands->Set_Objective_HUD_Info_Position(objectiveId, 64.0f, "POG_M02_2_12.tga", 7376, objectivePositions[14]); // Destroy

			break;
		case 217:
			Commands->Set_HUD_Help_Text(7666, Vector3(0.196f, 0.882f, 0.196f)); // New Secondary Objective\n

			// Disable Power Plant
			// Power Plants provide energy for other Nod structures. Disabling this Power Plant will shut down the Obelisk. Assault the building from the interior, as it is being repaired by a Nod Engineer.
			Commands->Add_Objective(objectiveId, 2, 0, 6287, 0, 6154);
			Commands->Set_Objective_Radar_Blip(objectiveId, objectivePositions[15]);
			Commands->Set_Objective_HUD_Info_Position(objectiveId, 65.0f, "POG_M02_2_13.tga", 7376, objectivePositions[15]); // Destroy

			break;
		case 218:
			Commands->Set_HUD_Help_Text(7666, Vector3(0.196f, 0.882f, 0.196f)); // New Secondary Objective\n

			// Destroy Tiberium Machine Alpha
			// Nod has three Tiberium research machines located throughout this cavern area. Destroy this machine to hamper Nod Tiberium research.
			Commands->Add_Objective(objectiveId, 2, 0, 6288, 0, 6155);
			Commands->Set_Objective_Radar_Blip(objectiveId, objectivePositions[16]);
			Commands->Set_Objective_HUD_Info_Position(objectiveId, 70.0f, "POG_M02_2_14.tga", 7376, objectivePositions[16]); // Destroy

			break;
		case 219:
			Commands->Set_HUD_Help_Text(7666, Vector3(0.196f, 0.882f, 0.196f)); // New Secondary Objective\n

			// Destroy Tiberium Machine Beta
			// Nod has three Tiberium research machines located throughout this cavern area. Destroy this machine to hamper Nod Tiberium research.
			Commands->Add_Objective(objectiveId, 2, 0, 6289, 0, 6156);
			Commands->Set_Objective_Radar_Blip(objectiveId, objectivePositions[17]);
			Commands->Set_Objective_HUD_Info_Position(objectiveId, 71.0f, "POG_M02_2_14.tga", 7376, objectivePositions[17]); // Destroy

			break;
		case 220:
			Commands->Set_HUD_Help_Text(7666, Vector3(0.196f, 0.882f, 0.196f)); // New Secondary Objective\n

			// Destroy Tiberium Machine Gamma
			// Nod has three Tiberium research machines located throughout this cavern area. Destroy this machine to hamper Nod Tiberium research.
			Commands->Add_Objective(objectiveId, 2, 0, 6290, 0, 6157);
			Commands->Set_Objective_Radar_Blip(objectiveId, objectivePositions[18]);
			Commands->Set_Objective_HUD_Info_Position(objectiveId, 72.0f, "POG_M02_2_14.tga", 7376, objectivePositions[18]); // Destroy

			break;
		case 221:
			Commands->Set_HUD_Help_Text(7666, Vector3(0.196f, 0.882f, 0.196f)); // New Secondary Objective\n

			// Acquire Mammoth Tank
			// GDI has airdropped a Mammoth Tank for your use. Acquire the Mammoth Tank and use it to your advantage.
			Commands->Add_Objective(objectiveId, 2, 0, 6291, 0, 6158);
			Commands->Set_Objective_Radar_Blip(objectiveId, objectivePositions[19]);
			Commands->Set_Objective_HUD_Info_Position(objectiveId, 58.0f, "POG_M02_2_15.tga", 7371, objectivePositions[19]); // Acquire

			break;
		default:
			break;
	}
}

ScriptRegistrant<M02_Objective_Controller> M02_Objective_ControllerRegistrant("M02_Objective_Controller", "");