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
#include "MX0_A02_Controller.h"

/*
M13 -> 1100000
*/
void MX0_A02_Controller::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(this->soldierActorIds, sizeof(this->soldierActorIds), 1);
	Auto_Save_Variable(&this->starAtSniperArea, sizeof(this->starAtSniperArea), 2);
	Auto_Save_Variable(&this->soldierActorWaitForHavocTauntActionId, sizeof(this->soldierActorWaitForHavocTauntActionId), 3);
	Auto_Save_Variable(&this->destroyedHumveeObjId, sizeof(this->destroyedHumveeObjId), 4);
	Auto_Save_Variable(&this->destroyedMedTankObjId, sizeof(this->destroyedMedTankObjId), 5);
	Auto_Save_Variable(&this->apcObjId, sizeof(this->apcObjId), 6);
	Auto_Save_Variable(&this->sniper1ObjId, sizeof(this->sniper1ObjId), 7);
	Auto_Save_Variable(&this->sniper2ObjId, sizeof(this->sniper2ObjId), 8);
	Auto_Save_Variable(&this->canDoYeahTaunt, sizeof(this->canDoYeahTaunt), 9);
	Auto_Save_Variable(&this->progressCounter, sizeof(this->progressCounter), 10);
	Auto_Save_Variable(&this->doingEvent, sizeof(this->doingEvent), 11);
	Auto_Save_Variable(&this->nodRocketSoldierActorIndex, sizeof(this->nodRocketSoldierActorIndex), 12);
	Auto_Save_Variable(&this->shouldWaitForFreeSlots, sizeof(this->shouldWaitForFreeSlots), 13);
	Auto_Save_Variable(&this->blockageObjId, sizeof(this->blockageObjId), 14);
	Auto_Save_Variable(&this->shouldDestroySpawners, sizeof(this->shouldDestroySpawners), 15);
	Auto_Save_Variable(&this->helicopter0Killed, sizeof(this->helicopter0Killed), 16);
	Auto_Save_Variable(&this->soldierActorTankReadyTauntNumber, sizeof(this->soldierActorTankReadyTauntNumber), 17);
	Auto_Save_Variable(&this->reinforcementsEngineer1ObjId, sizeof(this->reinforcementsEngineer1ObjId), 18);
	Auto_Save_Variable(&this->reinforcementsEngineer2ObjId, sizeof(this->reinforcementsEngineer2ObjId), 19);
	Auto_Save_Variable(&this->soldierActorCallingHavocTauntActionId, sizeof(this->soldierActorCallingHavocTauntActionId), 20);
	Auto_Save_Variable(&this->soldierActorEnemyKilledTauntActionId, sizeof(this->soldierActorEnemyKilledTauntActionId), 21);
	Auto_Save_Variable(&this->medTankEntered, sizeof(this->medTankEntered), 22);
	Auto_Save_Variable(&this->starKillCount, sizeof(this->starKillCount), 23);
	Auto_Save_Variable(&this->soldierActorWrongWayTauntActorIndex, sizeof(this->soldierActorWrongWayTauntActorIndex), 24);
	Auto_Save_Variable(&this->spawnedA02Objects, sizeof(this->spawnedA02Objects), 25);
}

void MX0_A02_Controller::Created(GameObject *obj)
{
	this->spawnedA02Objects = false;
	this->starAtSniperArea = false;
	this->canDoYeahTaunt = false;
	this->doingEvent = true;
	this->shouldWaitForFreeSlots = false;
	this->shouldDestroySpawners = false;
	this->helicopter0Killed = false;
	this->medTankEntered = false;
	this->soldierActorTankReadyTauntNumber = 0;
	this->soldierActorWaitForHavocTauntActionId = 0;
	this->destroyedHumveeObjId = 0;
	this->destroyedMedTankObjId = 0;
	this->apcObjId = 0;
	this->sniper1ObjId = 0;
	this->sniper2ObjId = 0;
	this->blockageObjId = 0;
	this->progressCounter = 0;
	this->nodRocketSoldierActorIndex = 0;
	this->reinforcementsEngineer1ObjId = 0;
	this->reinforcementsEngineer2ObjId = 0;
	this->soldierActorCallingHavocTauntActionId = 0;
	this->soldierActorEnemyKilledTauntActionId = 0;
	this->starKillCount = 0;
	this->soldierActorWrongWayTauntActorIndex = 1;
}

void MX0_A02_Controller::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// TODO (No custom)
	if (type == 203)
	{
		for (int x = 1; x < 9; x++)
		{
			GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[x]);

			if (soldierActorObj)
			{
				Commands->Send_Custom_Event(obj, soldierActorObj, 203, 0, 0.0f);
			}
		}

		Commands->Destroy_Object(obj);
	}

	// TODO (No custom)
	else if (type == 225)
	{
		if (sender)
		{
			Commands->Send_Custom_Event(obj, sender, 213, this->sniper1ObjId, 0.0f);
		}
	}

	// Received from MX0_A02_ACTOR when when custom 213 has been received
	else if (type == 226)
	{
		if (sender)
		{
			Commands->Send_Custom_Event(obj, sender, 214, this->sniper2ObjId, 0.0f);
		}
	}

	// Received from MX0_MissionStart_DME when custom type 223 is received
	// (Unused script) Received from MX0_A02_ZONE_DEFAULT_ON when entered
	else if (type == 223)
	{
		this->sniper1ObjId = param;
	}

	// Received from MX0_MissionStart_DME when custom type 224 is received
	// (Unused script) Received from MX0_A02_ZONE_DEFAULT_ON when entered
	else if (type == 224)
	{
		this->sniper2ObjId = param;
	}

	// Received from MX0_MissionStart_DME when timer number 134 triggers
	// (Unused script) Received from MX0_A02_ZONE_DEFAULT_ON when entered
	else if (type == 202)
	{
		if (!this->spawnedA02Objects)
		{
			this->spawnedA02Objects = true;

			GameObject *davesArrowObj = Commands->Find_Object(1100006);
			if (davesArrowObj)
			{
				Vector3 davesArrowObjPos = Commands->Get_Position(davesArrowObj);
				GameObject *soldierActorObj = Commands->Create_Object("MX0_GDI_MiniGunner_0_B", davesArrowObjPos);

				if (soldierActorObj)
				{
					Commands->Attach_Script(soldierActorObj, "MX0_A02_ACTOR", "0");
					this->soldierActorIds[1] = Commands->Get_ID(soldierActorObj);
					Commands->Send_Custom_Event(soldierActorObj, soldierActorObj, 204, 1, 0.1f);
				}

				Commands->Attach_Script(soldierActorObj, "M00_Send_Object_ID", "1400041,11,0.0f");
			}

			GameObject *davesArrowObj2 = Commands->Find_Object(1100010);
			if (davesArrowObj2)
			{
				Vector3 davesArrowObj2Pos = Commands->Get_Position(davesArrowObj2);
				GameObject *soldierActorObj = Commands->Create_Object("MX0_GDI_MiniGunner_1Off", davesArrowObj2Pos);

				if (soldierActorObj)
				{
					Commands->Attach_Script(soldierActorObj, "MX0_A02_ACTOR", "0");
					this->soldierActorIds[2] = Commands->Get_ID(soldierActorObj);
					Commands->Send_Custom_Event(soldierActorObj, soldierActorObj, 204, 2, 0.1f);
				}
			}

			GameObject *davesArrowObj3 = Commands->Find_Object(1100008);
			if (davesArrowObj3)
			{
				Vector3 davesArrowObj3Pos = Commands->Get_Position(davesArrowObj3);
				GameObject *soldierActorObj = Commands->Create_Object("MX0_GDI_RocketSoldier_1Off", davesArrowObj3Pos);

				if (soldierActorObj)
				{
					Commands->Attach_Script(soldierActorObj, "MX0_A02_ACTOR", "0");
					this->soldierActorIds[3] = Commands->Get_ID(soldierActorObj);
					Commands->Send_Custom_Event(soldierActorObj, soldierActorObj, 204, 3, 0.1f);
				}
			}

			GameObject *davesArrowObj4 = Commands->Find_Object(1100012);
			if (davesArrowObj4)
			{
				Vector3 davesArrowObj4Pos = Commands->Get_Position(davesArrowObj4);
				GameObject *soldierActorObj = Commands->Create_Object("MX0_GDI_MiniGunner_0", davesArrowObj4Pos);

				if (soldierActorObj)
				{
					Commands->Attach_Script(soldierActorObj, "MX0_A02_ACTOR", "0");
					this->soldierActorIds[4] = Commands->Get_ID(soldierActorObj);
					Commands->Send_Custom_Event(soldierActorObj, soldierActorObj, 204, 4, 0.1f);
				}
			}

			GameObject *davesArrowObj5 = Commands->Find_Object(1100014);
			if (davesArrowObj5)
			{
				Vector3 davesArrowObj5Pos = Commands->Get_Position(davesArrowObj5);
				GameObject *soldierActorObj = Commands->Create_Object("MX0_Nod_Minigunner_0", davesArrowObj5Pos);

				if (soldierActorObj)
				{
					Commands->Attach_Script(soldierActorObj, "MX0_A02_ACTOR", "0");
					this->soldierActorIds[5] = Commands->Get_ID(soldierActorObj);
					Commands->Send_Custom_Event(soldierActorObj, soldierActorObj, 204, 5, 0.1f);
				}
			}

			GameObject *davesArrowObj6 = Commands->Find_Object(1100016);
			if (davesArrowObj6)
			{
				Vector3 davesArrowObj6Pos = Commands->Get_Position(davesArrowObj6);
				GameObject *soldierActorObj = Commands->Create_Object("MX0_Nod_Minigunner_0", davesArrowObj6Pos);

				if (soldierActorObj)
				{
					Commands->Attach_Script(soldierActorObj, "MX0_A02_ACTOR", "0");
					this->soldierActorIds[6] = Commands->Get_ID(soldierActorObj);
					Commands->Send_Custom_Event(soldierActorObj, soldierActorObj, 204, 6, 0.1f);
				}
			}

			GameObject *davesArrowObj7 = Commands->Find_Object(1100018);
			if (davesArrowObj7)
			{
				Vector3 davesArrowObj7Pos = Commands->Get_Position(davesArrowObj7);
				GameObject *soldierActorObj = Commands->Create_Object("MX0_Nod_Minigunner_0", davesArrowObj7Pos);

				if (soldierActorObj)
				{
					Commands->Attach_Script(soldierActorObj, "MX0_A02_ACTOR", "0");
					this->soldierActorIds[7] = Commands->Get_ID(soldierActorObj);
					Commands->Send_Custom_Event(soldierActorObj, soldierActorObj, 204, 7, 0.1f);
				}
			}

			GameObject *davesArrowObj8 = Commands->Find_Object(1100020);
			if (davesArrowObj8)
			{
				Vector3 davesArrowObj8Pos = Commands->Get_Position(davesArrowObj8);
				GameObject *soldierActorObj = Commands->Create_Object("MX0_Nod_Minigunner_0", davesArrowObj8Pos);

				if (soldierActorObj)
				{
					Commands->Attach_Script(soldierActorObj, "MX0_A02_ACTOR", "0");
					this->soldierActorIds[8] = Commands->Get_ID(soldierActorObj);
					Commands->Send_Custom_Event(soldierActorObj, soldierActorObj, 204, 8, 0.1f);
				}
			}

			this->starAtSniperArea = true;
			this->soldierActorWaitForHavocTauntActionId = 1;

			Commands->Start_Timer(obj, this, 1.0f, 204);

			GameObject *destroyedHumveeObj = Commands->Create_Object("MX0_GDI_Humm-Vee_destroyed", Vector3(-84.582f, -60.035f, 1.05f));
			if (destroyedHumveeObj)
			{
				Commands->Attach_Script(destroyedHumveeObj, "MX0_A02_GDI_VEHICLE", "");
				Commands->Set_Facing(destroyedHumveeObj, 50.0f);

				this->destroyedHumveeObjId = Commands->Get_ID(destroyedHumveeObj);
			}

			GameObject *destroyedMedTankObj = Commands->Create_Object("MX0_GDI_Medium_Tank_Dec", Vector3(-90.183f, -51.691f, 1.216f));
			if (destroyedMedTankObj)
			{
				Commands->Attach_Script(destroyedMedTankObj, "MX0_A02_GDI_VEHICLE", "");
				Commands->Set_Facing(destroyedMedTankObj, -20.0f);

				this->destroyedMedTankObjId = Commands->Get_ID(destroyedMedTankObj);
			}

			GameObject *apcObj = Commands->Create_Object("MX0_GDI_APC", Vector3(-101.576f, -54.945f, 1.05f));
			if (apcObj)
			{
				Commands->Attach_Script(apcObj, "MX0_A02_GDI_APC", "");
				Commands->Set_Facing(apcObj, -45.0f);

				this->apcObjId = Commands->Get_ID(apcObj);
			}

			GameObject *blockageObj = Commands->Create_Object("Simple_Level_x0_A02_Blockage", Vector3(-71.233f, -41.517f, 0.181f));
			if (blockageObj)
			{
				this->blockageObjId = Commands->Get_ID(blockageObj);

				Commands->Set_Facing(blockageObj, -40.0f);
			}

			GameObject *MX0MissionStartDMEObj = Commands->Find_Object(1200001);
			if (MX0MissionStartDMEObj)
			{
				Commands->Send_Custom_Event(obj, MX0MissionStartDMEObj, 223, 0, 0.0f);
				Commands->Send_Custom_Event(obj, MX0MissionStartDMEObj, 224, 0, 0.0f);
			}

			Commands->Start_Timer(obj, this, 1.0f, 220);
		}
	}

	// Received from MX0_A02_ACTOR when movements with action ids 206/207 ended
	else if (type == 209)
	{
		if (sender)
		{
			float randSoldierActorIndex;
			if (param > 4)
			{
				randSoldierActorIndex = Commands->Get_Random(1.0f, 4.9998999f);
				if (randSoldierActorIndex > 4.0f)
				{
					randSoldierActorIndex = 4.0f;
				}
			}
			else
			{
				randSoldierActorIndex = Commands->Get_Random(5.0f, 8.9998999f);
				if (randSoldierActorIndex > 8.0f)
				{
					randSoldierActorIndex = 8.0f;
				}
			}

			Commands->Send_Custom_Event(obj, sender, 209, this->soldierActorIds[static_cast<size_t>(randSoldierActorIndex)], 0.0f);
		}
	}

	// Received from MX0_A02_ZONE_STARTUP after it is entered
	else if (type == 215)
	{
		if (this->canDoYeahTaunt)
		{
			this->starAtSniperArea = false;
			
			Commands->Start_Timer(obj, this, 1.0f, 207);

			return;
		}

		this->canDoYeahTaunt = true;

		if (Commands->Find_Object(this->sniper1ObjId))
		{
			GameObject *soldierActorObj1 = Commands->Find_Object(this->soldierActorIds[3]);
			if (soldierActorObj1)
			{
				Commands->Send_Custom_Event(obj, soldierActorObj1, 213, this->sniper1ObjId, 0.0f);
			}
		}
		else
		{
			GameObject *soldierActorObj1 = Commands->Find_Object(this->soldierActorIds[3]);
			if (soldierActorObj1 && Commands->Find_Object(this->sniper2ObjId))
			{
				Commands->Send_Custom_Event(obj, soldierActorObj1, 214, this->sniper2ObjId, 0.0f);
			}
		}

		GameObject *soldierActorObj2 = Commands->Find_Object(this->soldierActorIds[1]);
		if (soldierActorObj2)
		{
			Commands->Send_Custom_Event(obj, soldierActorObj2, 217, 0, 0.0f);
		}

		GameObject *apcObj = Commands->Find_Object(this->apcObjId);
		if (apcObj)
		{
			Commands->Send_Custom_Event(obj, apcObj, 215, 0, 0.0f);
		}

		this->starAtSniperArea = false;

		Commands->Start_Timer(obj, this, 1.0f, 207);
	}

	// TODO (No custom)
	else if (type == 212)
	{
		this->starAtSniperArea = false;

		Commands->Start_Timer(obj, this, 1.0f, 207);
	}

	// Received from MX0_A02_ACTOR when conversation with action id 206 has ended
	else if (type == 222)
	{
		for (int x = 5; x < 9; x++)
		{
			GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[x]);
			if (soldierActorObj)
			{
				Commands->Send_Custom_Event(obj, soldierActorObj, 205, 0, 0.0f);
			}
		}

		this->doingEvent = false;
		this->progressCounter = 0;

		Commands->Create_2D_Sound("MX0_NODOFFICER_044");
	}

	// Received from MX0_A02_ACTOR when conversation with action id 202 has ended
	else if (type == 218)
	{
		GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[2]);
		if (!soldierActorObj)
		{
			return;
		}

		Commands->Send_Custom_Event(obj, soldierActorObj, 218, 0, 0.0f);
	}

	// Received from MX0_A02_ACTOR when conversation with action id 205 has ended
	else if (type == 219)
	{
		GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[1]);
		if (!soldierActorObj)
		{
			return;
		}

		Commands->Send_Custom_Event(obj, soldierActorObj, 219, 0, 0.0f);
	}

	// Received from MX0_A02_ACTOR when conversation with action id 203 has ended
	else if (type == 220)
	{
		GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[2]);
		if (!soldierActorObj)
		{
			return;
		}

		Commands->Send_Custom_Event(obj, soldierActorObj, 220, 0, 0.0f);
	}

	// Received from MX0_A02_ACTOR when conversation with action id 204 has ended
	else if (type == 221)
	{
		GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[3]);
		if (!soldierActorObj)
		{
			return;
		}

		Commands->Send_Custom_Event(obj, soldierActorObj, 221, 0, 0.0f);
	}

	// Received from MX0_A02_ACTOR when killed by the star
	else if (type == 227)
	{
		float randSoldierActorObjIndex = Commands->Get_Random(1.0f, 4.9998999f);
		if (randSoldierActorObjIndex > 4.0)
		{
			randSoldierActorObjIndex = 4.0f;
		}

		GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[static_cast<size_t>(randSoldierActorObjIndex)]);
		if (soldierActorObj)
		{
			if (this->soldierActorCallingHavocTauntActionId > 9)
			{
				this->soldierActorCallingHavocTauntActionId = 0;
			}

			Commands->Send_Custom_Event(obj, soldierActorObj, 229, this->soldierActorCallingHavocTauntActionId, 0.0f);

			this->soldierActorCallingHavocTauntActionId++;
		}
	}

	// Received from MX0_A02_ACTOR when not killed by the star
	else if (type == 228)
	{
		GameObject *soldierActorObjKiller = Commands->Find_Object(param);
		if (soldierActorObjKiller)
		{
			if (this->soldierActorEnemyKilledTauntActionId > 3)
			{
				this->soldierActorEnemyKilledTauntActionId = 0;
			}

			Commands->Send_Custom_Event(obj, soldierActorObjKiller, 230, this->soldierActorEnemyKilledTauntActionId, 0.0f);

			this->soldierActorEnemyKilledTauntActionId++;
		}
	}

	// Received from ourself when timer number 209 ended or MX0_A02_ACTOR is done with movement action id 215
	else if (type == 235)
	{
		this->nodRocketSoldierActorIndex = 0;
		this->doingEvent = false;

		this->progressCounter++;
	}

	// Received from MX0_A02_ACTOR when killed or every 3 seconds if no free slot is available
	else if (type == 232)
	{
		int freeSlotsAvailable = 0;
		for (int x = 6; x < 9; x++)
		{
			GameObject *soldierActorObj = Commands->Find_Object(x);
			if (x == param || !soldierActorObj)
			{
				this->soldierActorIds[x] = 0;
			}

			if (!this->soldierActorIds[x])
			{
				this->starKillCount++;
				freeSlotsAvailable++;
			}
		}

		if (this->starKillCount > 2 && this->starKillCount < 99)
		{
			this->starKillCount = 99;

			Commands->Fade_Background_Music("Level 0 Hero.mp3", 2, 2);
		}

		if (this->shouldDestroySpawners)
		{
			GameObject *nodSoldierSpawner1 = Commands->Find_Object(1100026);
			if (nodSoldierSpawner1)
			{
				Commands->Destroy_Object(nodSoldierSpawner1);
			}

			GameObject *nodSoldierSpawner2 = Commands->Find_Object(1100027);
			if (nodSoldierSpawner2)
			{
				Commands->Destroy_Object(nodSoldierSpawner2);
			}

			GameObject *nodSoldierSpawner3 = Commands->Find_Object(1100028);
			if (nodSoldierSpawner3)
			{
				Commands->Destroy_Object(nodSoldierSpawner3);
			}
		}
		else
		{
			if (this->doingEvent && this->nodRocketSoldierActorIndex == param && this->nodRocketSoldierActorIndex)
			{
				this->nodRocketSoldierActorIndex = 0;
				this->doingEvent = false;
				this->progressCounter++;
			}
			if (!this->shouldWaitForFreeSlots)
			{
				this->shouldWaitForFreeSlots = true;

				if (!this->doingEvent)
				{
					if (!freeSlotsAvailable)
					{
						Commands->Start_Timer(obj, this, 3.0f, 208);

						return;
					}

					if (this->progressCounter == 0)
					{
						GameObject *nodSoldierObj = Commands->Create_Object("MX0_Nod_Minigunner_0", Vector3(-78.442f, -78.044f, 6.404f));
						if (nodSoldierObj)
						{
							freeSlotsAvailable--;
							this->doingEvent = true;

							Commands->Attach_Script(nodSoldierObj, "MX0_A02_ACTOR", "1");

							MX0_A02_Fill_Empty_Slot(nodSoldierObj, true);
						}
					}
					else if (this->progressCounter == 1)
					{
						GameObject *nodSoldierObj = Commands->Create_Object("MX0_Nod_Minigunner_0", Vector3(-102.985f, -73.331f, 4.415f));
						if (nodSoldierObj)
						{
							freeSlotsAvailable--;
							this->doingEvent = true;

							Commands->Attach_Script(nodSoldierObj, "MX0_A02_ACTOR", "2");

							MX0_A02_Fill_Empty_Slot(nodSoldierObj, true);
						}
					}
					else if (this->progressCounter == 2)
					{
						GameObject *nodSoldierObj = Commands->Create_Object("Nod_RocketSoldier_0", Vector3(-78.442f, -78.044f, 6.404f));
						if (nodSoldierObj)
						{
							freeSlotsAvailable--;
							this->doingEvent = true;

							Commands->Attach_Script(nodSoldierObj, "MX0_A02_ACTOR", "3");

							MX0_A02_Fill_Empty_Slot(nodSoldierObj, false);
						}
					}
					else if (this->progressCounter == 3)
					{
						GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-71.497f, -47.387f, 9.0f));
						if (invisObj)
						{
							this->doingEvent = true;

							Commands->Set_Facing(invisObj, -100.0f);
							Commands->Attach_Script(invisObj, "MX0_A02_HELICOPTER", "0");
							Commands->Attach_Script(invisObj, "Test_Cinematic", "X0I_Drop02_A02_E01.txt");

							Commands->Start_Timer(obj, this, 20.0f, 209);
						}

						GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[2]);
						if (soldierActorObj)
						{
							Commands->Send_Custom_Event(obj, soldierActorObj, 242, 0, 10.0f);
						}
					}
					else if (this->progressCounter == 4)
					{
						this->progressCounter = 5;

						GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-83.454f, -70.323f, 9.6f));
						if (invisObj)
						{
							this->doingEvent = true;

							Commands->Set_Facing(invisObj, 90.0f);
							Commands->Attach_Script(invisObj, "MX0_A02_HELICOPTER", "1");
							Commands->Attach_Script(invisObj, "Test_Cinematic", "X0I_Drop02_A02_E02.txt");
						}

						GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[2]);
						if (soldierActorObj)
						{
							Commands->Send_Custom_Event(obj, soldierActorObj, 243, 0, 10.0f);
						}
					}
				}

				if (freeSlotsAvailable)
				{
					for (int x = 5; x < 9; x++)
					{
						if (!this->soldierActorIds[x])
						{
							int nodSoldierSpawnerId;
							switch (x)
							{
								case 6:
									nodSoldierSpawnerId = 1100026;
									break;
								case 7:
									nodSoldierSpawnerId = 1100027;
									break;
								case 8:
									nodSoldierSpawnerId = 1100028;
									break;
								default:
									nodSoldierSpawnerId = 0;
									break;
							}

							GameObject *nodSoldierObj = Commands->Trigger_Spawner(nodSoldierSpawnerId);
							if (nodSoldierObj)
							{
								Commands->Attach_Script(nodSoldierObj, "MX0_A02_ACTOR", "0");
								this->soldierActorIds[x] = Commands->Get_ID(nodSoldierObj);
								Commands->Send_Custom_Event(nodSoldierObj, nodSoldierObj, 204, x, 0.1f);
								Commands->Send_Custom_Event(obj, nodSoldierObj, 205, 0, 0.1f);

								break;
							}
						}
					}
				}

				Commands->Start_Timer(obj, this, 3.0f, 208);
			}
		}
	}

	// Received from MX0_A02_ACTOR when done with movement with action id 209
	else if (type == 233)
	{
		GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[1]);
		if (!soldierActorObj || !Commands->Find_Object(this->soldierActorIds[this->nodRocketSoldierActorIndex]))
		{
			return;
		}

		Commands->Send_Custom_Event(obj, soldierActorObj, 233, 0, 0.0f);
	}

	// Received from MX0_A02_ACTOR when conversation with action id 225 has ended
	else if (type == 234)
	{
		GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[2]);
		if (!soldierActorObj || !Commands->Find_Object(this->soldierActorIds[this->nodRocketSoldierActorIndex]))
		{
			return;
		}

		Commands->Send_Custom_Event(obj, soldierActorObj, 234, this->soldierActorIds[this->nodRocketSoldierActorIndex], 0.0f);
	}

	// Received from MX0_A02_ACTOR when damaged
	else if (type == 236)
	{
		GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[1]);
		if (!soldierActorObj)
		{
			return;
		}

		Commands->Send_Custom_Event(obj, soldierActorObj, 236, 0, 0.0f);
	}

	// Received from MX0_A02_ACTOR when done with movement with action id 211
	else if (type == 237)
	{
		GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[4]);
		if (!soldierActorObj || !Commands->Find_Object(this->soldierActorIds[this->nodRocketSoldierActorIndex]))
		{
			return;
		}

		Commands->Send_Custom_Event(obj, soldierActorObj, 237, this->soldierActorIds[this->nodRocketSoldierActorIndex], 0.0f);
	}

	// Received from MX0_A02_ACTOR when it 'killed' the apc (custom type 240)
	else if (type == 239)
	{
		GameObject *apcObj = Commands->Find_Object(this->apcObjId);
		if (!apcObj)
		{
			return;
		}

		Commands->Send_Custom_Event(obj, apcObj, 210, 0, 2.0f);
	}

	// Received from MX0_A02_ACTOR when done with movement with action id 213
	else if (type == 240)
	{
		if (!sender)
		{
			return;
		}

		if (Commands->Find_Object(this->apcObjId))
		{
			Commands->Send_Custom_Event(obj, sender, 240, this->apcObjId, 0.0f);
		}
		else
		{
			Commands->Send_Custom_Event(obj, sender, 240, 0, 0.0f);
		}
	}

	// Received from MX0_A02_HELICOPTER when killed with ActorID > 0
	else if (type == 241)
	{
		GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[2]);
		if (!soldierActorObj)
		{
			return;
		}

		Commands->Send_Custom_Event(obj, soldierActorObj, 241, 0, 0.0f);
	}

	// Received from MX0_A02_ACTOR on create with ActorID == 5 (flamethrower)
	else if (type == 244)
	{
		GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[3]);
		if (!soldierActorObj)
		{
			return;
		}

		Commands->Send_Custom_Event(obj, soldierActorObj, 244, 0, 0.0f);
	}

	// Received from MX0_A02_ACTOR with timer number 210
	else if (type == 245)
	{
		GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[4]);
		if (!soldierActorObj)
		{
			return;
		}

		Commands->Send_Custom_Event(obj, soldierActorObj, 245, 0, 0.0f);
	}

	// Received from MX0_A02_HELICOPTER when killed with ActorID <= 0
	else if (type == 246)
	{
		if (this->helicopter0Killed)
		{
			return;
		}

		this->canDoYeahTaunt = false;
		this->helicopter0Killed = true;

		GameObject *soldierActorObj1 = Commands->Find_Object(this->soldierActorIds[2]);
		if (soldierActorObj1)
		{
			Commands->Send_Custom_Event(obj, soldierActorObj1, 246, 0, 0.5f);
		}

		GameObject *soldierActorObj2 = Commands->Find_Object(this->soldierActorIds[1]);
		if (soldierActorObj2)
		{
			Commands->Send_Custom_Event(obj, soldierActorObj2, 246, 0, 1.5f);
		}

		Commands->Start_Timer(obj, this, 1.0f, 212);
	}

	// Received from MX0_A02_ACTOR when conversation with action id 234 has ended
	else if (type == 250)
	{
		GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[4]);
		if (!soldierActorObj)
		{
			return;
		}

		Commands->Send_Custom_Event(obj, soldierActorObj, 250, 0, 0.0f);
	}

	// Received from MX0_A02_ACTOR when conversation with action id 235 has ended
	else if (type == 251)
	{
		GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[1]);
		if (!soldierActorObj)
		{
			return;
		}

		Commands->Send_Custom_Event(obj, soldierActorObj, 251, 0, 0.0f);
	}

	// Received from MX0_A02_ACTOR when conversation with action id 236 has ended
	else if (type == 252)
	{
		GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[2]);
		if (!soldierActorObj)
		{
			return;
		}

		Commands->Send_Custom_Event(obj, soldierActorObj, 252, 0, 0.0f);
	}

	// Received from MX0_A02_ACTOR when conversation with action id 237 has ended
	else if (type == 253)
	{
		GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[3]);
		if (!soldierActorObj)
		{
			return;
		}

		Commands->Send_Custom_Event(obj, soldierActorObj, 253, 0, 0.0f);
	}

	// Received from MX0_A02_ACTOR when conversation with action id 238 has ended
	else if (type == 254)
	{
		Commands->Create_2D_Sound("MX0_GDIEAGLEBASE_110");

		Commands->Start_Timer(obj, this, 5.0f, 218);
	}

	// Received from MX0_A02_ACTOR with timer number 216
	else if (type == 255)
	{
		if (!sender)
		{
			return;
		}

		this->reinforcementsEngineer1ObjId = Commands->Get_ID(sender);

		Commands->Send_Custom_Event(obj, sender, 255, this->destroyedMedTankObjId, 0.0f);
	}

	// Received from MX0_A02_ACTOR when conversation with action id 240 has ended
	else if (type == 256)
	{
		GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[1]);
		if (soldierActorObj)
		{
			Commands->Send_Custom_Event(obj, soldierActorObj, 257, 0, 0.0f);

			Commands->Start_Timer(obj, this, 5.0f, 219);
		}
	}

	// Received from MX0_A02_ACTOR with timer number 217
	else if (type == 258)
	{
		this->reinforcementsEngineer2ObjId = Commands->Get_ID(sender);
	}

	// Received from MX0_A02_GDI_MEDTANK when entered
	else if (type == 262)
	{
		this->medTankEntered = true;
		GameObject *MX0A03ControllerObj = Commands->Find_Object(1400041);
		if (MX0A03ControllerObj)
		{
			Commands->Send_Custom_Event(obj, MX0A03ControllerObj, 401, 0, 2.0f);
		}

		GameObject *reinforcementsEngineer2Obj = Commands->Find_Object(this->reinforcementsEngineer2ObjId);
		if (!reinforcementsEngineer2Obj)
		{
			return;
		}

		Commands->Send_Custom_Event(obj, reinforcementsEngineer2Obj, 263, 0, 0.0f);
	}

	// Received from MX0_A02_ACTOR with timer number 245
	else if (type == 264)
	{
		GameObject *blockageObj = Commands->Find_Object(this->blockageObjId);
		if (blockageObj)
		{
			Commands->Create_Explosion("Ground Explosion 01 - Harmless", Vector3(-71.233f, -41.517f, 0.181f), NULL);
			Commands->Set_Animation(blockageObj, "AG_L0_BLOCKAGE1.AG_L0_BLOCKAGE1", false, NULL, 0.0f, -1.0f, false);

			Commands->Start_Timer(obj, this, 2.0f, 221);
		}

		GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[1]);
		if (soldierActorObj)
		{
			Commands->Send_Custom_Event(obj, soldierActorObj, 265, 0, 0.0f);
		}

		Commands->Start_Timer(obj, this, 1.0f, 222);
	}

	// Received from MX0_A02_ACTOR when attack action with action_id 214 is complete
	else if (type == 267)
	{
		GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[3]);
		if (soldierActorObj)
		{
			Commands->Send_Custom_Event(obj, soldierActorObj, 267, 0, 0.0f);
		}
	}

	// Received from MX0_A02_ACTOR when conversation with action id 229 has ended
	else if (type == 268)
	{
		for (int x = 1; x < 5; x++)
		{
			GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[x]);
			if (soldierActorObj)
			{
				Commands->Send_Custom_Event(obj, soldierActorObj, 268, 0, 0.0f);
			}
		}
	}
}

void MX0_A02_Controller::Timer_Expired(GameObject *obj, int number)
{
	// Triggered 1 second after custom 202 has been received, after that a random interval
	if (number == 204)
	{
		if (this->starAtSniperArea)
		{
			GameObject *soldierActorObj;
			switch (this->soldierActorWaitForHavocTauntActionId)
			{
				case 1:
				case 4:
				case 8:
					soldierActorObj = Commands->Find_Object(this->soldierActorIds[2]);
					break;
				case 5:
				case 9:
					soldierActorObj = Commands->Find_Object(this->soldierActorIds[1]);
					break;
				case 2:
				case 10:
					soldierActorObj = Commands->Find_Object(this->soldierActorIds[3]);
					break;
				case 3:
				case 6:
				case 7:
				case 11:
					soldierActorObj = Commands->Find_Object(this->soldierActorIds[4]);
					break;
				default:
					soldierActorObj = NULL;
					break;
			}

			if (soldierActorObj)
			{
				Commands->Send_Custom_Event(obj, soldierActorObj, 211, this->soldierActorWaitForHavocTauntActionId, 0.0f);
			}

			if (++this->soldierActorWaitForHavocTauntActionId > 11)
			{
				this->soldierActorWaitForHavocTauntActionId = 1;
			}

			float randInterval = Commands->Get_Random(2.0f, 4.9998999f);
			if (randInterval > 4.0f)
			{
				randInterval = 4.0f;
			}

			Commands->Start_Timer(obj, this, randInterval, 204);
		}
	}

	// Triggered 1 second after custom type 215 has been received
	else if (number == 207)
	{
		if (this->canDoYeahTaunt)
		{
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);
			Vector3 starObjPos = Commands->Get_Position(starObj);

			if (Commands->Get_Distance(starObjPos, pos) > 30.0f)
			{
				float randSoldierActorIndex = Commands->Get_Random(1.0f, 4.9998999f);
				if (randSoldierActorIndex > 4.0f)
				{
					randSoldierActorIndex = 4.0f;
				}

				GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[static_cast<size_t>(randSoldierActorIndex)]);
				if (soldierActorObj)
				{
					Commands->Send_Custom_Event(obj, soldierActorObj, 231, 0, 0.0f);
				}
			}
		}
	}

	// Triggered 3 seconds after custom type 232 received (soldier killed)
	else if (number == 208)
	{ 
		if (!this->shouldDestroySpawners)
		{
			this->shouldWaitForFreeSlots = false;

			Commands->Send_Custom_Event(obj, obj, 232, 0, 0.1f);
		}
	}

	// Triggered 20 seconds after custom type 232 received (when helicopter 0 is spawned)
	else if (number == 209)
	{
		Commands->Send_Custom_Event(obj, obj, 235, 0, 0.0f);
	}

	// Triggered 1 second after custom type 246 (helicopter 0 killed)
	else if (number == 212)
	{
		this->doingEvent = false;
		this->shouldWaitForFreeSlots = true;
		this->shouldDestroySpawners = true;
		this->canDoYeahTaunt = false;

		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-106.497f, -37.154f, 1.093f));
		if (invisObj)
		{
			Commands->Set_Facing(invisObj, -55.0f);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X0I_GDI_Drop02_Engineer.txt");
		}

		GameObject *buildingStateSoundControllerObj = Commands->Find_Object(1700006);
		if (buildingStateSoundControllerObj)
		{
			Commands->Destroy_Object(buildingStateSoundControllerObj);
		}

		for (int x = 5; x < 9; x++)
		{
			if (this->soldierActorIds[x])
			{
				GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[x]);
				if (soldierActorObj)
				{
					Commands->Send_Custom_Event(obj, soldierActorObj, 247, 0, 0.0f);
				}
			}
		}

		Commands->Create_2D_Sound("MX0_NODSOLDIER1_101");

		Commands->Start_Timer(obj, this, 3.0f, 213);
	}

	// Triggered 3 seconds after timer number 212
	else if (number == 213)
	{
		for (int x = 1; x < 5; x++)
		{
			GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[x]);
			if (soldierActorObj)
			{
				Commands->Send_Custom_Event(obj, soldierActorObj, 248, 0, 0.0f);

				if (x == 2)
				{
					Commands->Send_Custom_Event(obj, soldierActorObj, 249, 0, 0.0f);
				}
			}
		}
	}

	// Triggered 5 seconds after custom type 254
	else if (number == 218)
	{
		Commands->Create_2D_Sound("MX0_GDIEAGLEBASE_116");
	}

	// Triggered 5 seconds after custom type 256 or in this block (when the tank is repaired)
	else if (number == 219)
	{
		if (this->soldierActorTankReadyTauntNumber > 2 || this->medTankEntered)
		{
			return;
		}

		if (!this->soldierActorTankReadyTauntNumber)
		{
			GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[2]);
			if (soldierActorObj)
			{
				Commands->Send_Custom_Event(obj, soldierActorObj, 261, 0, 0.0f);
			}
		}
		else if (this->soldierActorTankReadyTauntNumber == 1)
		{
			GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[1]);
			if (soldierActorObj)
			{
				Commands->Send_Custom_Event(obj, soldierActorObj, 260, 0, 0.0f);
			}
		}
		else if (this->soldierActorTankReadyTauntNumber == 2)
		{
			GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[2]);
			if (soldierActorObj)
			{
				Commands->Send_Custom_Event(obj, soldierActorObj, 261, 0, 0.0f);
			}
		}

		this->soldierActorTankReadyTauntNumber++;

		Commands->Start_Timer(obj, this, 5.0f, 219);
	}

	// Triggered 1 second after custom 202 has been received, after that a random interval
	else if (number == 220)
	{
		if (!this->shouldDestroySpawners)
		{
			float random = Commands->Get_Random(1.0f, 16.999901f);
			if (random > 16.0f)
			{
				random = 16.0f;
			}

			GameObject *randomDavesArrowObj = Commands->Find_Object(1100023);
			switch (static_cast<int>(random))
			{
				case 1:
					randomDavesArrowObj = Commands->Find_Object(1100006);
					break;
				case 2:
					randomDavesArrowObj = Commands->Find_Object(1100007);
					break;
				case 3:
					randomDavesArrowObj = Commands->Find_Object(1100010);
					break;
				case 4:
					randomDavesArrowObj = Commands->Find_Object(1100011);
					break;
				case 5:
					randomDavesArrowObj = Commands->Find_Object(1100008);
					break;
				case 6:
					randomDavesArrowObj = Commands->Find_Object(1100009);
					break;
				case 7:
					randomDavesArrowObj = Commands->Find_Object(1100012);
					break;
				case 8:
					randomDavesArrowObj = Commands->Find_Object(1100013);
					break;
				case 9:
					randomDavesArrowObj = Commands->Find_Object(1100014);
					break;
				case 10:
					randomDavesArrowObj = Commands->Find_Object(1100015);
					break;
				case 11:
					randomDavesArrowObj = Commands->Find_Object(1100016);
					break;
				case 12:
					randomDavesArrowObj = Commands->Find_Object(1100017);
					break;
				case 13:
					randomDavesArrowObj = Commands->Find_Object(1100018);
					break;
				case 14:
					randomDavesArrowObj = Commands->Find_Object(1100019);
					break;
				case 15:
					randomDavesArrowObj = Commands->Find_Object(1100020);
					break;
				case 16:
					randomDavesArrowObj = Commands->Find_Object(1100021);
					break;
				default:
					randomDavesArrowObj = Commands->Find_Object(1100023);
					break;
			}

			if (randomDavesArrowObj)
			{
				Vector3 randomDavesArrowObjPos = Commands->Get_Position(randomDavesArrowObj);

				float randXOffset = Commands->Get_Random(-2.0f, 2.9999001f);
				if (randXOffset > 2.0f)
				{
					randXOffset = 2.0f;
				}

				randomDavesArrowObjPos.X += randXOffset;

				float randYOffset = Commands->Get_Random(-2.0f, 2.9999001f);
				if (randYOffset > 2.0f)
				{
					randYOffset = 2.0f;
				}

				randomDavesArrowObjPos.Y += randYOffset;

				Commands->Create_Explosion("Explosion_MX0", randomDavesArrowObjPos, obj);

				float randInterval = Commands->Get_Random(3.0f, 6.9998999f);
				if (randInterval > 6.0f)
				{
					randInterval = 6.0f;
				}

				Commands->Start_Timer(obj, this, randInterval, 220);
			}
		}
	}

	// Triggered 2 seconds after custom type 264
	else if (number == 221)
	{
		GameObject *blockageObj = Commands->Find_Object(this->blockageObjId);
		if (blockageObj)
		{
			Commands->Destroy_Object(blockageObj);
		}
	}

	// Triggered 1 second after custom type 264 or 5 seconds in this block
	else if (number == 222)
	{
		if (++this->soldierActorWrongWayTauntActorIndex > 4)
		{
			this->soldierActorWrongWayTauntActorIndex = 2;
		}

		GameObject *soldierActorObj = Commands->Find_Object(this->soldierActorIds[this->soldierActorWrongWayTauntActorIndex]);
		if (soldierActorObj)
		{
			Commands->Send_Custom_Event(obj, soldierActorObj, 269, 0, 0.0f);
		}

		Commands->Start_Timer(obj, this, 5.0, 222);
	}
}

void MX0_A02_Controller::MX0_A02_Fill_Empty_Slot(GameObject *nodSoldierObj, bool isMinigunner)
{
	for (int x = 5; x < 9; x++)
	{
		if (!this->soldierActorIds[x])
		{
			this->soldierActorIds[x] = Commands->Get_ID(nodSoldierObj);
			this->nodRocketSoldierActorIndex = (isMinigunner ? 0 : x);

			Commands->Send_Custom_Event(nodSoldierObj, nodSoldierObj, 204, x, 0.1f);
			
			return;
		}
	}
}

ScriptRegistrant<MX0_A02_Controller> MX0_A02_ControllerRegistrant("MX0_A02_Controller", "");
