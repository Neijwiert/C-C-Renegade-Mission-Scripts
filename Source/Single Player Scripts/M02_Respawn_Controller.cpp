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
#include "M02_Respawn_Controller.h"

void M02_Respawn_Controller::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(this->activeAreas, sizeof(this->activeAreas), 1);
	Auto_Save_Variable(this->heliDrop01Active, sizeof(this->heliDrop01Active), 2);
	Auto_Save_Variable(this->heliDrop02Active, sizeof(this->heliDrop02Active), 3);
	Auto_Save_Variable(this->paraDropActive, sizeof(this->paraDropActive), 4);
	Auto_Save_Variable(this->nodSoldierCount, sizeof(this->nodSoldierCount), 5);
	Auto_Save_Variable(this->maxNodSoldierCount, sizeof(this->maxNodSoldierCount), 6);
	Auto_Save_Variable(this->nodSoldierObjIds, sizeof(this->nodSoldierObjIds), 7);
	Auto_Save_Variable(&this->spawnerTwiddler, sizeof(this->spawnerTwiddler), 8);
	Auto_Save_Variable(&this->field_124, sizeof(this->field_124), 9);
	Auto_Save_Variable(&this->convoyBridgeNodSAMSiteDestroyedCount, sizeof(this->convoyBridgeNodSAMSiteDestroyedCount), 10);
	Auto_Save_Variable(&this->nodTiberiumSiloDestroyedCount, sizeof(this->nodTiberiumSiloDestroyedCount), 11);
}

// On level start
void M02_Respawn_Controller::Created(GameObject *obj)
{
	this->spawnerTwiddler = 0;
	this->field_124 = -1;
	this->convoyBridgeNodSAMSiteDestroyedCount = 0;
	this->nodTiberiumSiloDestroyedCount = 0;

	for (int x = 0; x < 26; x++)
	{
		this->activeAreas[x] = false;
		this->nodSoldierCount[x] = 0;
		this->nodSoldierObjIds[x] = 0;
		this->maxNodSoldierCount[x] = 8;
		this->heliDrop01Active[x] = false;
		this->heliDrop02Active[x] = false;
		this->paraDropActive[x] = false;

		switch (x)
		{
			case 6:
				this->maxNodSoldierCount[x] = 6;
				break;
			case 1:
			case 2:
			case 9:
				this->maxNodSoldierCount[x] = 7;
				break;
			case 10:
				this->paraDropActive[x] = true;
				break;
			case 3:
			case 21:
				this->maxNodSoldierCount[x] = 5;
				break;
			case 8:
			case 11:
			case 14:
			case 15:
			case 24:
				this->heliDrop01Active[x] = true;
				this->heliDrop02Active[x] = true;
				this->paraDropActive[x] = true;
				this->maxNodSoldierCount[x] = 3;
				break;
			default:
				break;
		}
	}

	GameObject *gdiTransportHelicopterObj = Commands->Create_Object("GDI_Transport_Helicopter", Vector3(-26.87f, 37.42f, 20.75f));
	if (gdiTransportHelicopterObj)
	{
		Commands->Set_Facing(gdiTransportHelicopterObj, -155.0f);
		Commands->Attach_Script(gdiTransportHelicopterObj, "M02_GDI_Helicopter", "");
	}

	GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-87.2f, -13.1f, -40.1f));
	if (invisObj)
	{
		Commands->Set_Facing(invisObj, 102.9f);
		Commands->Attach_Script(invisObj, "Test_Cinematic", "X2I_GDI_Drop_HummVee.txt");
	}

	GameObject *nodBuggyObj = Commands->Find_Object(400985);
	Commands->Send_Custom_Event(obj, nodBuggyObj, 0, 0, 0.0f);

	GameObject *invisObj2 = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
	if (invisObj2)
	{
		Commands->Attach_Script(invisObj2, "Test_Cinematic", "X2A_OrcaStrike.txt");
	}

	Commands->Start_Timer(obj, this, 3.0f, 2);
}

void M02_Respawn_Controller::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M02_Nod_Soldier when killed with param = areaNumber
	if (type == 101)
	{
		if (--this->nodSoldierCount[param] < 0)
		{
			this->nodSoldierCount[param] = 0;
		}
	}

	// Received from M02_Objective_Zone when entered with objId = 
	// (401054 + param = 8,
	//	400274 + param = 10,
	//	400273 + param = 11)
	else if (type == 102)
	{
		Check_Respawns(param);
	}

	// Received from M02_Nod_Soldier when timer number 1 triggered with param = areaNumber
	else if (type == 103)
	{
		this->nodSoldierCount[param]++;
	}

	// Received from M02_Objective_Zone when entered with objId = 
	// (401079 + param = 15, 
	//	401080 + param = 15, 
	//	401070 + param = 14, 
	//	401196 + param = 14, 
	//	400270 + param = 8,
	//	400267 + param = 4,
	//	400269 + param = 9,
	//	400501 + param = 24,
	//	400502 + param = 24,
	//	400272 + param = 10,
	//	400273 + param = 11,
	//	400271 + param = 8,
	//	400187 + param = 4,
	//	400184 + param = 0,
	//	400186 + param = 2,
	//	400185 + param = 1,
	//	400189 + param = 6,
	//	400188 + param = 3)
	// Received from M02_Objective_Zone when timer number 9 triggered with param = 21
	else if (type == 104)
	{
		this->activeAreas[param] = true;
	}

	// Received from M02_Destroy_Objective when killed (HON) param = 21
	// Received from M02_Objective_Zone when entered with objId = 
	// (401079 + param = 14, 
	//	401080 + param = 14, 
	//	401070 + param = 11, 
	//	401196 + param = 11, 
	//	400270 + param = 9,
	//	400267 + param = 3,
	//	400269 + param = 6,
	//	400268 + param = 4,
	//	400268 + param = 24,
	//	400274 + param = 11,
	//	400316 + param = 0,
	//	400272 + param = 8,
	//	400273 + parma = 10,
	//	400271 + param = 9,
	//	400187 + param = 3,
	//	400186 + param = 1,
	//	400192 + param = 11,
	//	400188 + param = 2,
	//	400194 + param = 21)
	else if (type == 105)
	{
		this->activeAreas[param] = false;
	}

	// Received from M02_Nod_Soldier when timer number 1 triggered with param = areaNumber (Only when its an area officer)
	else if (type == 106)
	{
		if (sender)
		{
			this->nodSoldierObjIds[param] = Commands->Get_ID(sender);
		}
	}

	// Received from M02_Reset_Spawn when destroyed (Heli drop 01) with param being areaId
	else if (type == 107)
	{
		this->heliDrop01Active[param] = false;
	}

	// Received from M02_Reset_Spawn when destroyed (Heli drop 02) with param being areaId
	else if (type == 108)
	{
		this->heliDrop02Active[param] = false;
	}

	// Received from M02_Reset_Spawn when destroyed (Para drop) with param being areaId
	else if (type == 109)
	{
		this->paraDropActive[param] = false;
	}

	// Received from M02_Objective_Zone when entered with objId = 
	// (400267 + param = 4,
	//	400187 + param = 4,
	//	400185 + param = 1)
	// Received from M02_Objective_Zone when M02_Objective_Zone::Send_and_Destroy is called with type = 207
	// Received from M02_Nod_Soldier when killed with param = 2
	else if (type == 111)
	{
		Replacement_Vehicle(obj, param);
	}

	// Received from M02_Player_Vehicle  when killed param = areaId
	// Received from M02_Objective_Zone when entered with objId = 
	// (400267 + param = 4,
	//	400187 + param = 4,
	//	400185 + param = 1)
	else if (type == 112)
	{
		this->field_124 = param;
	}

	// Received from M02_Destroy_Objective when killed
	else if (type == 113)
	{
		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(511.3f, 680.3f, -21.2f));
		if (invisObj)
		{
			Commands->Set_Facing(invisObj, 146.60001f);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X2I_GDI_Drop_PowerUp.txt");
		}
	}

	// Received from M02_Objective_Zone when entered with objId = 
	// (401054 + param = 8,
	//	400274 + param = 10,
	//	400273 + param = 11)
	else if (type == 114)
	{
		this->nodSoldierCount[param] = 0;
	}

	// Received from M02_Destroy_Objective when killed (2 samsites at the bridge before the dam)
	else if (type == 115)
	{
		if (++this->convoyBridgeNodSAMSiteDestroyedCount > 1)
		{
			this->field_124 = 6;

			Replacement_Vehicle(obj, 6);
		}
	}

	// Received from M02_Destroy_Objective when killed (tiberium silos at the end of the level)
	else if (type == 116)
	{
		if (++this->nodTiberiumSiloDestroyedCount > 1)
		{
			Commands->Send_Custom_Event(obj, obj, 222, 1, 0.0f);
			Commands->Stop_All_Conversations();

			int conversationId = Commands->Create_Conversation("M02_HIDDEN_02_FINISH", 100, 300.0f, true); // This conversation does not exist
			Commands->Join_Conversation(NULL, conversationId, true, true, true);

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			Commands->Join_Conversation(starObj, conversationId, true, true, false);
			Commands->Start_Conversation(conversationId, 0);
		}
	}
}

void M02_Respawn_Controller::Timer_Expired(GameObject *obj, int number)
{
	// Triggerd 3 seconds after creation or after a calculated interval in this block
	if (number == 2)
	{
		for (int x = 0; x < 26; x++)
		{
			if (this->activeAreas[x] && this->nodSoldierObjIds[x])
			{
				if (Commands->Find_Object(this->nodSoldierObjIds[x]))
				{
					Check_Respawns(x);
				}
			}
		}

		float interval = static_cast<float>(25 - 10 * Commands->Get_Difficulty_Level());

		Commands->Start_Timer(obj, this, interval, 2);
	}
}

void M02_Respawn_Controller::Check_Respawns(int areaId)
{
	int originalRand = static_cast<int>(Commands->Get_Random(0.0f, 2.9999001f));
	if (originalRand > 2)
	{
		originalRand = 2;
	}

	int rand = originalRand;

	bool cycledThroughAllRands = false;
	int extraSpawnerNodSoldierCount = 0;

	int nodSoldierCountTmp = this->nodSoldierCount[areaId];
	while (nodSoldierCountTmp < this->maxNodSoldierCount[areaId])
	{
		if (cycledThroughAllRands)
		{
			if (++extraSpawnerNodSoldierCount > 2)
			{
				nodSoldierCountTmp = this->maxNodSoldierCount[areaId];
			}
			else
			{
				nodSoldierCountTmp++;

				Use_Spawners(areaId);
			}
		}
		else if (rand == 1)
		{
			if (!this->heliDrop02Active[areaId] && this->maxNodSoldierCount[areaId] - nodSoldierCountTmp > 1)
			{
				this->heliDrop02Active[areaId] = true;

				nodSoldierCountTmp += 2;

				Use_Heli_Drop_02(areaId);
			}
		}
		else if (rand == 2)
		{
			if (!this->paraDropActive[areaId] && this->maxNodSoldierCount[areaId] - nodSoldierCountTmp > 2)
			{
				this->paraDropActive[areaId] = true;

				Use_Parachute_Drop(areaId);

				nodSoldierCountTmp += 3;
			}
		}
		else if (rand == 0)
		{
			if (!this->heliDrop01Active[areaId] && this->maxNodSoldierCount[areaId] - nodSoldierCountTmp > 2)
			{
				this->heliDrop01Active[areaId] = true;

				nodSoldierCountTmp += 3;

				Use_Heli_Drop_01(areaId);
			}
		}

		if (!cycledThroughAllRands)
		{
			if (++rand > 2)
			{
				rand = 0;
			}
			
			if (rand == originalRand)
			{
				cycledThroughAllRands = true;
			}
		}
	}
}

void M02_Respawn_Controller::Replacement_Vehicle(GameObject *obj, int areaId)
{
	if (areaId == 0)
	{
		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(161.6f, 27.6f, -18.0f));
		if (invisObj)
		{
			Commands->Set_Facing(invisObj, 115.4f);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X2I_GDI_Drop_HummVee.txt");
		}
	}
	else if (areaId == 1)
	{
		GameObject *nodBuggyPlayerObj = Commands->Create_Object("Nod_Buggy_Player", Vector3(494.652f, 128.786f, -56.159f));
		if (nodBuggyPlayerObj)
		{
			Commands->Set_Facing(nodBuggyPlayerObj, 30.0f);
			Commands->Attach_Script(nodBuggyPlayerObj, "M02_Player_Vehicle", "3");
		}
	}
	else if (areaId == 2)
	{
		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(448.32f, 717.19f, -12.35f));
		if (invisObj)
		{
			Commands->Set_Facing(invisObj, -30.0f);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X2I_GDI_Drop_MediumTank.txt");
		}
	}
	else if (areaId == 4)
	{
		GameObject *nodLightTankPlayer = Commands->Create_Object("Nod_Light_Tank_Player", Vector3(429.672f, 894.045f, 4.354f));
		if (nodLightTankPlayer)
		{
			Commands->Set_Facing(nodLightTankPlayer, -25.0f);
			Commands->Attach_Script(nodLightTankPlayer, "M02_Player_Vehicle", "5");
		}
	}
	else if (areaId == 5)
	{
		GameObject *nodReconBikePlayerObj = Commands->Create_Object("Nod_Recon_Bike_Player", Vector3(611.1f, 1164.9f, 4.6f));
		if (nodReconBikePlayerObj)
		{
			Commands->Set_Facing(nodReconBikePlayerObj, -39.700001f);
			Commands->Attach_Script(nodReconBikePlayerObj, "M02_Player_Vehicle", "6");
		}
	}
	else if (areaId == 6)
	{
		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(785.3f, 893.9f, 21.8f));
		if (invisObj)
		{
			Commands->Set_Facing(invisObj, 138.0f);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X2I_GDI_Drop_MediumTank.txt");
		}
	}
	else if (areaId == 14)
	{
		GameObject *nodReconBikePlayer = Commands->Create_Object("Nod_Recon_Bike_Player", Vector3(1229.37f, 742.89f, 27.03f));
		if (nodReconBikePlayer)
		{
			Commands->Set_Facing(nodReconBikePlayer, 85.0f);
			Commands->Attach_Script(nodReconBikePlayer, "M02_Player_Vehicle", "14");
		}
	}
	else
	{
		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
		if (invisObj)
		{
			Commands->Set_Facing(invisObj, 0.0f);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X2I_GDI_Drop_HummVee.txt");
		}
	}
}

void M02_Respawn_Controller::Use_Heli_Drop_01(int areaId)
{
	const char *cinematicFile;
	const char *scriptParams;
	Vector3 cinPos;
	float facing;

	switch (areaId)
	{
		case 0:
			scriptParams = "0,107";
			cinematicFile = "X2I_Drop03_Area00.txt";
			cinPos = Vector3(119.5f, 6.1f, -19.7f);
			facing = 140.0f;

			break;
		case 1:
			scriptParams = "1,107";
			cinematicFile = "X2I_Drop03_Area01.txt";
			cinPos = Vector3(515.5f, 17.1f, -58.9f);
			facing = 51.299999f;

			break;
		case 2:
			scriptParams = "2,107";
			cinematicFile = "X2I_Drop03_Area02.txt";
			cinPos = Vector3(619.9f, 310.4f, -59.9f);
			facing = 0.0f;

			break;
		case 3:
			scriptParams = "3,107";
			cinematicFile = "X2I_Drop03_Area03.txt";
			cinPos = Vector3(530.4f, 690.8f, -19.2f);
			facing = -22.799999f;

			break;
		case 4:
			scriptParams = "4,107";
			cinematicFile = "X2I_Drop03_Area04.txt";
			cinPos = Vector3(461.5f, 908.0f, 5.3f);
			facing = -102.4f;

			break;
		case 6:
			scriptParams = "6,107";
			cinematicFile = "X2I_Drop03_Area06.txt";
			cinPos = Vector3(772.1f, 969.6f, 21.4f);
			facing = 163.2f;

			break;
		case 9:
			scriptParams = "9,107";
			cinematicFile = "X2I_Drop03_Area09.txt";
			cinPos = Vector3(957.638f, 879.421f, 34.22f);
			facing = 55.0f;

			break;
		case 10:
			scriptParams = "10,107";
			cinematicFile = "X2I_Drop03_Area10.txt";
			cinPos = Vector3(1044.01f, 947.144f, -37.816f);
			facing = -83.5f;

			break;
		case 11:
			scriptParams = "11,107";
			cinematicFile = "X2I_Drop03_Area11.txt";
			cinPos = Vector3(1072.78f, 997.836f, 6.492f);
			facing = -35.0f;

			break;
		case 21:
			scriptParams = "21,107";
			cinematicFile = "X2I_Drop03_Area21.txt";
			cinPos = Vector3(1174.0f, 478.803f, 22.657f);
			facing = 55.0f;

			break;
		default:
			cinematicFile = "X2I_Drop03_Area00.txt";
			scriptParams = "0,107";
			cinPos = Vector3(0.0f, 0.0f, 0.0f);
			facing = 0.0f;

			break;
	}

	GameObject *invisObj = Commands->Create_Object("Invisible_Object", cinPos);
	if (invisObj)
	{
		Commands->Set_Facing(invisObj, facing);
		Commands->Attach_Script(invisObj, "M02_Reset_Spawn", scriptParams);
		Commands->Attach_Script(invisObj, "Test_Cinematic", cinematicFile);
	}
}

void M02_Respawn_Controller::Use_Heli_Drop_02(int areaId)
{
	const char *cinematicFile;
	const char *scriptParams;
	Vector3 cinPos;
	float facing;

	switch (areaId)
	{
		case 0:
			scriptParams = "0,108";
			cinematicFile = "X2I_Drop02_Area00.txt";
			cinPos = Vector3(99.4f, -13.8f, -28.4f);
			facing = 25.1f;

			break;
		case 1:
			scriptParams = "1,108";
			cinematicFile = "X2I_Drop02_Area01.txt";
			cinPos = Vector3(522.4f, 128.1f, -58.7f);
			facing = -133.89999f;

			break;
		case 2:
			scriptParams = "2,108";
			cinematicFile = "X2I_Drop02_Area02.txt";
			cinPos = Vector3(614.8f, 278.9f, -52.3f);
			facing = 22.799999f;

			break;
		case 3:
			scriptParams = "3,108";
			cinematicFile = "X2I_Drop02_Area03.txt";
			cinPos = Vector3(506.11f, 567.12f, -39.6f);
			facing = 66.900002f;

			break;
		case 4:
			scriptParams = "4,108";
			cinematicFile = "X2I_Drop02_Area04.txt";
			cinPos = Vector3(471.1f, 854.9f, 3.7f);
			facing = -171.2f;

			break;
		case 6:
			scriptParams = "6,108";
			cinematicFile = "X2I_Drop02_Area06.txt";
			cinPos = Vector3(786.0f, 1032.3f, 21.3f);
			facing = 168.7f;

			break;
		case 9:
			scriptParams = "9,108";
			cinematicFile = "X2I_Drop02_Area09.txt";
			cinPos = Vector3(1047.11f, 879.541f, 34.22f);
			facing = 140.0f;

			break;
		case 10:
			scriptParams = "10,108";
			cinematicFile = "X2I_Drop02_Area10.txt";
			cinPos = Vector3(970.504f, 941.132f, -37.812f);
			facing = -130.0f;

			break;
		case 11:
			scriptParams = "11,108";
			cinematicFile = "X2I_Drop02_Area11.txt";
			cinPos = Vector3(1092.03f, 975.856f, 4.14f);
			facing = 15.0f;

			break;
		case 21:
			scriptParams = "21,108";
			cinematicFile = "X2I_Drop02_Area21.txt";
			cinPos = Vector3(1173.37f, 562.608f, 32.394f);
			facing = 0.0f;

			break;
		default:
			cinematicFile = "X2I_Drop02_Area00.txt";
			scriptParams = "0,108";
			cinPos = Vector3(0.0f, 0.0f, 0.0f);
			facing = 0.0f;

			break;
	}

	GameObject *invisObj = Commands->Create_Object("Invisible_Object", cinPos);
	if (invisObj)
	{
		Commands->Set_Facing(invisObj, facing);
		Commands->Attach_Script(invisObj, "M02_Reset_Spawn", scriptParams);
		Commands->Attach_Script(invisObj, "Test_Cinematic", cinematicFile);
	}
}

void M02_Respawn_Controller::Use_Parachute_Drop(int areaId)
{
	const char *cinematicFile;
	const char *scriptParams;
	Vector3 cinPos;
	float facing;

	switch (areaId)
	{
		case 0:
			cinematicFile = "X2I_Para03_Area00.txt";
			scriptParams = "0,109";
			cinPos = Vector3(119.5f, 38.2f, -15.6f);
			facing = -144.89999f;

			break;
		case 1:
			cinematicFile = "X2I_Para03_Area01.txt";
			scriptParams = "1,109";
			cinPos = Vector3(509.7f, 78.3f, -55.2f);
			facing = -73.699997f;

			break;
		case 2:
			cinematicFile = "X2I_Para03_Area02.txt";
			scriptParams = "2,109";
			cinPos = Vector3(598.7f, 342.2f, -62.7f);
			facing = -119.1f;

			break;
		case 3:
			cinematicFile = "X2I_Para03_Area03.txt";
			scriptParams = "3,109";
			cinPos = Vector3(488.2f, 608.4f, -41.2f);
			facing = -158.2f;

			break;
		case 4:
			cinematicFile = "X2I_Para03_Area04.txt";
			scriptParams = "4,109";
			cinPos = Vector3(371.1f, 794.0f, 3.7f);
			facing = 90.0f;

			break;
		case 6:
			cinematicFile = "X2I_Para03_Area06.txt";
			scriptParams = "6,109";
			cinPos = Vector3(765.5f, 1004.0f, 21.4f);
			facing = -14.4f;

			break;
		case 9:
			cinematicFile = "X2I_Para03_Area09.txt";
			scriptParams = "9,109";
			cinPos = Vector3(1028.1f, 876.4f, 34.2f);
			facing = 87.199997f;

			break;
		case 21:
			cinematicFile = "X2I_Para03_Area21.txt";
			scriptParams = "21,109";
			cinPos = Vector3(1255.73f, 565.134f, 17.602f);
			facing = 150.0f;

			break;
		default:
			cinematicFile = "X2I_Para03_Area00.txt";
			scriptParams = "0,109";
			cinPos = Vector3(0.0f, 0.0f, 0.0f);
			facing = 0.0f;

			break;
	}

	GameObject *invisObj = Commands->Create_Object("Invisible_Object", cinPos);
	if (invisObj)
	{
		Commands->Set_Facing(invisObj, facing);
		Commands->Attach_Script(invisObj, "M02_Reset_Spawn", scriptParams);
		Commands->Attach_Script(invisObj, "Test_Cinematic", cinematicFile);
	}	
}

bool M02_Respawn_Controller::Use_Spawners(int areaId)
{
	int spawnerId;
	const char *scriptParams;

	if (++this->spawnerTwiddler > 2)
	{
		this->spawnerTwiddler = 0;
	}

	switch (areaId)
	{
		case 0:
			spawnerId = 400233;

			if (this->spawnerTwiddler)
			{
				spawnerId = 400236;
				if (this->spawnerTwiddler != 1)
				{
					spawnerId = 400237;
				}
			}

			scriptParams = "0,0,-1";

			break;
		case 1:
			spawnerId = 400234;

			if (this->spawnerTwiddler)
			{
				spawnerId = 400238;
				if (this->spawnerTwiddler != 1)
				{
					spawnerId = 400239;
				}
			}

			scriptParams = "1,0,-1";

			break;
		case 2:
			spawnerId = 400235;

			if (this->spawnerTwiddler)
			{
				spawnerId = 400240;
				if (this->spawnerTwiddler != 1)
				{
					spawnerId = 400241;
				}
			}

			scriptParams = "2,0,-1";

			break;
		case 3:
			spawnerId = 400242;

			if (this->spawnerTwiddler)
			{
				spawnerId = 400243;
				if (this->spawnerTwiddler != 1)
				{
					spawnerId = 400244;
				}
			}

			scriptParams = "3,0,-1";

			break;
		case 4:
			spawnerId = 400245;

			if (this->spawnerTwiddler)
			{
				spawnerId = 400246;
				if (this->spawnerTwiddler != 1)
				{
					spawnerId = 400247;
				}
			}

			scriptParams = "4,0,-1";

			break;
		case 6:
			spawnerId = 400248;
			if (this->spawnerTwiddler)
			{
				spawnerId = 400249;
				if (this->spawnerTwiddler != 1)
				{
					spawnerId = 400250;
				}
			}

			scriptParams = "6,0,-1";

			break;
		case 8:
			spawnerId = 400261;

			if (this->spawnerTwiddler)
			{
				spawnerId = 400262;
				if (this->spawnerTwiddler != 1)
				{
					spawnerId = 400263;
				}
			}

			scriptParams = "8,0,-1";

			break;
		case 9:
			spawnerId = 400251;

			if (this->spawnerTwiddler)
			{
				spawnerId = 400252;
				if (this->spawnerTwiddler != 1)
				{
					spawnerId = 400253;
				}
			}

			scriptParams = "9,0,-1";

			break;
		case 10:
			spawnerId = 400254;

			if (this->spawnerTwiddler)
			{
				spawnerId = 400255;
				if (this->spawnerTwiddler != 1)
				{
					spawnerId = 400256;
				}
			}

			scriptParams = "10,0,-1";

			break;
		case 11:
			spawnerId = 400257;

			if (this->spawnerTwiddler)
			{
				spawnerId = 400258;
				if (this->spawnerTwiddler != 1)
				{
					spawnerId = 400259;
				}
			}

			scriptParams = "11,0,-1";

			break;
		case 14:
			spawnerId = 401081;

			if (this->spawnerTwiddler)
			{
				spawnerId = 401082;
				if (this->spawnerTwiddler != 1)
				{
					spawnerId = 401083;
				}
			}

			scriptParams = "14,0,-1";

			break;
		case 15:
			spawnerId = 401098;

			if (this->spawnerTwiddler)
			{
				spawnerId = 401099;
				if (this->spawnerTwiddler != 1)
				{
					spawnerId = 401100;
				}
			}

			scriptParams = "15,0,-1";

			break;
		case 21:
			spawnerId = 400264;

			if (this->spawnerTwiddler)
			{
				spawnerId = 400265;
				if (this->spawnerTwiddler != 1)
				{
					spawnerId = 400266;
				}
			}

			scriptParams = "21,0,-1";

			break;
		case 24:
			spawnerId = 400540;

			if (this->spawnerTwiddler)
			{
				spawnerId = 400541;
				if (this->spawnerTwiddler != 1)
				{
					spawnerId = 400542;
				}
			}

			scriptParams = "24,0,-1";

			break;
		default:
			spawnerId = 0;
			scriptParams = "0,0,-1";

			break;
	}

	GameObject *spawnedObj = Commands->Trigger_Spawner(spawnerId);
	if (spawnedObj)
	{
		Commands->Attach_Script(spawnedObj, "M02_Nod_Soldier", scriptParams);
		
		return true;
	}
	else
	{
		return false;
	}
}

ScriptRegistrant<M02_Respawn_Controller> M02_Respawn_ControllerRegistrant("M02_Respawn_Controller", "");