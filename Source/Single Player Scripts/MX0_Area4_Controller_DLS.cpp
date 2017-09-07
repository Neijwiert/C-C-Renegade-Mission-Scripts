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
#include "MX0_Area4_Controller_DLS.h"

void MX0_Area4_Controller_DLS::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->starAtAreaNumber, sizeof(this->starAtAreaNumber), 1);
	Auto_Save_Variable(&this->isA04Active, sizeof(this->isA04Active), 2);
	Auto_Save_Variable(&this->humveeObjId, sizeof(this->humveeObjId), 3);
	Auto_Save_Variable(&this->MX0GDIMiniGunner0BObjId, sizeof(this->MX0GDIMiniGunner0BObjId), 4);
	Auto_Save_Variable(&this->field_2C, sizeof(this->field_2C), 5);
	Auto_Save_Variable(&this->nodLightTankObjId, sizeof(this->nodLightTankObjId), 6);
	Auto_Save_Variable(&this->nodBuggyObjId, sizeof(this->nodBuggyObjId), 7);
	Auto_Save_Variable(&this->rightWallRocketTrooperObjId, sizeof(this->rightWallRocketTrooperObjId), 8);
	Auto_Save_Variable(&this->leftWallRocketTrooperObjId, sizeof(this->leftWallRocketTrooperObjId), 9);
	Auto_Save_Variable(&this->reinforcementsEngineer1ObjId, sizeof(this->reinforcementsEngineer1ObjId), 10);
	Auto_Save_Variable(&this->field_44, sizeof(this->field_44), 11);
	Auto_Save_Variable(&this->gdiRocketSoldier0ObjId, sizeof(this->gdiRocketSoldier0ObjId), 12);
	Auto_Save_Variable(&this->gdiMiniGunner0ObjId, sizeof(this->gdiMiniGunner0ObjId), 13);
	Auto_Save_Variable(&this->MX0ObeliskObjId, sizeof(this->MX0ObeliskObjId), 14);
	Auto_Save_Variable(&this->MX0NodObeliskObjId, sizeof(this->MX0NodObeliskObjId), 15);
	Auto_Save_Variable(&this->gdiOrcaFLyoverObjId, sizeof(this->gdiOrcaFLyoverObjId), 16);
	Auto_Save_Variable(&this->nodMobileArtilleryObjId, sizeof(this->nodMobileArtilleryObjId), 17);
	Auto_Save_Variable(&this->canDoFlyoverCinematic, sizeof(this->canDoFlyoverCinematic), 18);
	Auto_Save_Variable(&this->field_64, sizeof(this->field_64), 19);
	Auto_Save_Variable(&this->isObeliskSighted, sizeof(this->isObeliskSighted), 20);
	Auto_Save_Variable(&this->gdiReinforcementsDeathCount, sizeof(this->gdiReinforcementsDeathCount), 21);
	Auto_Save_Variable(&this->field_70, sizeof(this->field_70), 22);
	Auto_Save_Variable(&this->field_74, sizeof(this->field_74), 23);
	Auto_Save_Variable(&this->wrongWayConvTableIndex, sizeof(this->wrongWayConvTableIndex), 24);
	Auto_Save_Variable(&this->calledForStar, sizeof(this->calledForStar), 25);
}

void MX0_Area4_Controller_DLS::Created(GameObject *obj)
{
	this->starAtAreaNumber = 0;
	this->isA04Active = false;

	Commands->Scale_AI_Awareness(3.0f, 1.0f);

	this->isObeliskSighted = false;
	this->canDoFlyoverCinematic = false;
	this->gdiReinforcementsDeathCount = 0;
	this->wrongWayConvTableIndex = 0;
	this->calledForStar = 0;
}

void MX0_Area4_Controller_DLS::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received after 228 cinematic frames from x0e_obelisk
	if (type == 445018)
	{
		GameObject *MX0ObeliskObj = Commands->Find_Object(this->MX0ObeliskObjId);
		Commands->Destroy_Object(MX0ObeliskObj);
	}

	// Received from MX0_Area4_Zone_DLS when entered by a star, param being area
	else if (type == 445000)
	{
		this->starAtAreaNumber = param;

		if (param == 1 && !this->isA04Active)
		{
			this->isA04Active = true;

			Commands->Start_Timer(obj, this, 3.0f, 0);

			GameObject *humveeObj = Commands->Find_Object(this->humveeObjId);
			Commands->Attach_Script(humveeObj, "MX0_Vehicle_DLS", "1500018, 1500019, 0, 0, 0.5f");
			Commands->Attach_Script(humveeObj, "MX0_GDI_Killed_DLS", "1");

			GameObject *MX0GDIMiniGunner0BObj = Commands->Find_Object(this->MX0GDIMiniGunner0BObjId);
			Commands->Attach_Script(MX0GDIMiniGunner0BObj, "MX0_Vehicle_DLS", "1500022, 1500023, 0, 0, 1.0f");
			Commands->Attach_Script(MX0GDIMiniGunner0BObj, "MX0_GDI_Killed_DLS", "2");
			Commands->Attach_Script(MX0GDIMiniGunner0BObj, "M00_Damage_Modifier_DME", ".10f,1,1,1,1");

			GameObject *field2CObj = Commands->Find_Object(this->field_2C);
			Commands->Attach_Script(field2CObj, "MX0_Vehicle_DLS", "1500027, 1500028, 1500029, 0, 1.0f");

			GameObject *field44Obj = Commands->Find_Object(this->field_44);
			Commands->Attach_Script(field44Obj, "MX0_GDI_Soldier_DLS", "1500049, 1500050, 0, 0, 0.8f");

			GameObject *gdiRocketSoldierObj = Commands->Create_Object("GDI_RocketSoldier_0", Vector3(51.644f, 27.306f, 4.85f));
			Commands->Attach_Script(gdiRocketSoldierObj, "M00_Send_Object_ID", "1500017, 12, 0.0f");
			Commands->Attach_Script(gdiRocketSoldierObj, "MX0_GDI_Soldier_DLS", "1500053, 1500070, 0, 0, 0.8f");

			Commands->Send_Custom_Event(obj, humveeObj, 445002, 445010, 0.0f);
		}
	}

	// Received from various M00_Send_Object_ID
	else if (type == 9035)
	{
		switch (param)
		{
			case 1:
				this->humveeObjId = Commands->Get_ID(sender);
				break;
			case 3:
				this->MX0GDIMiniGunner0BObjId = Commands->Get_ID(sender);
				break;
			case 4: // TODO (No custom)
				this->field_2C = Commands->Get_ID(sender);
				break;
			case 5:
				this->nodLightTankObjId = Commands->Get_ID(sender);
				break;
			case 6:
				this->nodBuggyObjId = Commands->Get_ID(sender);
				break;
			case 8:
				this->rightWallRocketTrooperObjId = Commands->Get_ID(sender);
				break;
			case 9:
				this->leftWallRocketTrooperObjId = Commands->Get_ID(sender);
				break;
			case 10:
				this->reinforcementsEngineer1ObjId = Commands->Get_ID(sender);
				break;
			case 11: // TODO (No custom)
				this->field_44 = Commands->Get_ID(sender);
				break;
			case 12:
				this->gdiRocketSoldier0ObjId = Commands->Get_ID(sender);
				break;
			case 13:
				this->gdiMiniGunner0ObjId = Commands->Get_ID(sender);
				break;
			case 14:
				this->MX0ObeliskObjId = Commands->Get_ID(sender);
				break;
			case 15:
				this->MX0NodObeliskObjId = Commands->Get_ID(sender);
				break;
			case 16:
				this->gdiOrcaFLyoverObjId = Commands->Get_ID(sender);
				break;
			case 17:
				this->nodMobileArtilleryObjId = Commands->Get_ID(sender);
				break;
			case 18: // TODO (No custom)
				this->field_64 = Commands->Get_ID(sender);
				break; 
			case 19: // TODO (No custom)
				this->field_70 = Commands->Get_ID(sender);
				break;
			case 20: // TODO (No custom)
				this->field_74 = Commands->Get_ID(sender);
				break;
		}
	}

	// Received after 100 cinematic frames from x0e_obelisk
	else if (type == 445007)
	{
		GameObject *rightSamSiteObj = Commands->Find_Object(1500015);
		Commands->Send_Custom_Event(obj, rightSamSiteObj, 445007, this->gdiOrcaFLyoverObjId, 0.0f);

		GameObject *leftSamSiteObj = Commands->Find_Object(1500016);
		Commands->Send_Custom_Event(obj, leftSamSiteObj, 445007, this->gdiOrcaFLyoverObjId, 0.0f);
	}

	// Received after 90 cinematic frames from x0d_a10_crash
	else if (type == 445008)
	{
		GameObject *nodMobileArtilleryObj = Commands->Find_Object(this->nodMobileArtilleryObjId);
		Commands->Apply_Damage(nodMobileArtilleryObj, 50000.0f, "STEEL", NULL);
	}

	// Received after 440 cinematic frames from x0z_finale
	else if (type == 445009)
	{
		Commands->Mission_Complete(true);
	}

	// Received from MX0_GDI_Killed_DLS when killed
	else if (type == 445019)
	{
		if (++this->gdiReinforcementsDeathCount == 2)
		{
			this->gdiReinforcementsDeathCount = 0;

			GameObject *MX0GDIReinforceArea4Obj = Commands->Find_Object(1500102);
			Vector3 MX0GDIReinforceArea4ObjPos = Commands->Get_Position(MX0GDIReinforceArea4Obj);

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", MX0GDIReinforceArea4ObjPos);
		
			float MX0GDIReinforceArea4ObjFacing = Commands->Get_Facing(MX0GDIReinforceArea4Obj);

			Commands->Set_Facing(invisObj, MX0GDIReinforceArea4ObjFacing);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "MX0_GDI_Reinforce_Area4.txt");
		}
	}
}

void MX0_Area4_Controller_DLS::Timer_Expired(GameObject *obj, int number)
{
	static const char *wrongWayConvTable[] =
	{
		"MX0_A04_CON015", // You’re going the wrong way, Havoc!
		"MX0_A04_CON016", // Stay with the Mission, Havoc!
		"MX0_A04_CON017", // This area is already secure.
		"MX0_A04_CON018" // Havoc, Did I say retreat? Now Turn Around!
	};

	if (!number)
	{
		if (this->starAtAreaNumber <= 0)
		{
			if (!this->calledForStar)
			{
				this->calledForStar = 1;

				GameObject *humveeObj = Commands->Find_Object(this->humveeObjId);
				Commands->Send_Custom_Event(obj, humveeObj, 445002, 445020, 0.0f);
			}

			Commands->Start_Timer(obj, this, 2.0f, 0);
		}
		else
		{
			GameObject *humveeObj = Commands->Find_Object(this->humveeObjId);
			Commands->Send_Custom_Event(obj, humveeObj, 445001, 1, 0.0f);
			
			GameObject *MX0GDIMiniGunner0BObj = Commands->Find_Object(this->MX0GDIMiniGunner0BObjId);
			Commands->Send_Custom_Event(obj, MX0GDIMiniGunner0BObj, 445001, 1, 0.0f);

			GameObject *field2CObj = Commands->Find_Object(this->field_2C);
			Commands->Send_Custom_Event(obj, field2CObj, 445001, 1, 0.0f);
			
			GameObject *field44Obj = Commands->Find_Object(this->field_44);
			Commands->Send_Custom_Event(obj, field44Obj, 445006, 1, 0.0f);

			GameObject *gdiRocketSoldier0Obj = Commands->Find_Object(this->gdiRocketSoldier0ObjId);
			Commands->Send_Custom_Event(obj, gdiRocketSoldier0Obj, 445006, 1, 0.0f);
			
			GameObject *gdiMiniGunner0Obj = Commands->Find_Object(this->gdiMiniGunner0ObjId);
			Commands->Send_Custom_Event(obj, gdiMiniGunner0Obj, 445006, 1, 0.0f);
			
			GameObject *nodLightTankObj = Commands->Find_Object(this->nodLightTankObjId);
			Commands->Attach_Script(nodLightTankObj, "MX0_Vehicle_DLS", "1500033, 1500034, 0, 0, 0.4f");

			GameObject *nodBuggyObj = Commands->Find_Object(this->nodBuggyObjId);
			Commands->Attach_Script(nodBuggyObj, "MX0_Vehicle_DLS", "0, 0, 0, 0, 0.0f");

			GameObject *nodMobileArtilleryObj = Commands->Find_Object(this->nodMobileArtilleryObjId);
			Commands->Attach_Script(nodMobileArtilleryObj, "MX0_Vehicle_DLS", "1500084, 1500085, 0, 0, 1.0f");

			Commands->Start_Timer(obj, this, 2.0f, 1);

			Commands->Send_Custom_Event(obj, humveeObj, 9037, 0, 0.0f);
			Commands->Send_Custom_Event(obj, field44Obj, 445002, 445012, 4.0f);
		}
	}
	else if (number == 1)
	{
		if (this->starAtAreaNumber <= 1)
		{
			GameObject *gdiRocketSoldier0Obj = Commands->Find_Object(this->gdiRocketSoldier0ObjId);
			Commands->Send_Custom_Event(obj, gdiRocketSoldier0Obj, 445002, 445021, 0.0f);

			Commands->Start_Timer(obj, this, 6.0f, 1);
		}
		else
		{
			GameObject *nodLightTankObj = Commands->Find_Object(this->nodLightTankObjId);
			Commands->Send_Custom_Event(obj, nodLightTankObj, 445001, 1, 0.0f);

			GameObject *MX0NodObeliskObj = Commands->Find_Object(1500020);
			Commands->Send_Custom_Event(obj, MX0NodObeliskObj, 445003, 1, 0.0f);

			Commands->Start_Timer(obj, this, 0.0f, 2);

			GameObject *field44Obj = Commands->Find_Object(this->field_44);
			Commands->Send_Custom_Event(obj, field44Obj, 445002, 445013, 4.0f);

			GameObject *gdiRocketSoldier0Obj = Commands->Find_Object(this->gdiRocketSoldier0ObjId);
			Commands->Send_Custom_Event(obj, gdiRocketSoldier0Obj, 445006, 1, 0.0f);

			GameObject *gdiMiniGunner0Obj = Commands->Find_Object(this->gdiMiniGunner0ObjId);
			Commands->Send_Custom_Event(obj, gdiMiniGunner0Obj, 445006, 1, 0.0f);

			int randInt = Commands->Get_Random_Int(0, 3);

			GameObject *field70Obj = Commands->Find_Object(this->field_70);
			Commands->Send_Custom_Event(obj, field70Obj, 445006, randInt, 0.0f);

			GameObject *field74Obj = Commands->Find_Object(this->field_74);
			Commands->Send_Custom_Event(obj, field74Obj, 445006, randInt, 0.0f);
		}
	}
	else if (number == 2)
	{
		if (this->starAtAreaNumber == 0 || this->starAtAreaNumber == 1)
		{
			if (this->wrongWayConvTableIndex == 4)
			{
				this->wrongWayConvTableIndex = 0;
			}

			int conversationId = Commands->Create_Conversation(wrongWayConvTable[this->wrongWayConvTableIndex], 100, 200.0f, false);
			Commands->Join_Conversation(obj, conversationId, false, true, true);
			Commands->Start_Conversation(conversationId, 10);

			this->wrongWayConvTableIndex++;

			Commands->Start_Timer(obj, this, 5.0f, 2);
		}
		else if (this->starAtAreaNumber == 2)
		{
			Commands->Start_Timer(obj, this, 5.0f, 2);
		}
		else if (this->starAtAreaNumber == 3)
		{
			Commands->Enable_Spawner(1500042, true);     // Left wall soldier
			Commands->Enable_Spawner(1500044, true);     // Right wall soldier

			GameObject *MX0NodGunEmplacementObj = Commands->Find_Object(1500039);
			Commands->Send_Custom_Event(obj, MX0NodGunEmplacementObj, 445002, 1, 0.0f);
			
			GameObject *field2CObj = Commands->Find_Object(this->field_2C);
			Commands->Send_Custom_Event(obj, field2CObj, 445001, 2, 0.0f);

			GameObject *nodMobileArtilleryObj = Commands->Find_Object(this->nodMobileArtilleryObjId);
			Commands->Send_Custom_Event(obj, nodMobileArtilleryObj, 445001, 1, 0.0f);

			Commands->Start_Timer(obj, this, 8.0f, 3);

			GameObject *MX0GDIReinforceArea4Obj = Commands->Find_Object(1500102);
			Vector3 MX0GDIReinforceArea4ObjPos = Commands->Get_Position(MX0GDIReinforceArea4Obj);
			
			GameObject *invisObj = Commands->Create_Object("Invisible_Object", MX0GDIReinforceArea4ObjPos);
			
			float MX0GDIReinforceArea4ObjFacing = Commands->Get_Facing(MX0GDIReinforceArea4Obj);

			Commands->Set_Facing(invisObj, MX0GDIReinforceArea4ObjFacing);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "MX0_GDI_Reinforce_Area4.txt");

			GameObject *gdiRocketSoldier0Obj = Commands->Find_Object(this->gdiRocketSoldier0ObjId);
			Commands->Send_Custom_Event(obj, gdiRocketSoldier0Obj, 445006, 1, 0.0f);

			GameObject *gdiMiniGunner0Obj = Commands->Find_Object(this->gdiMiniGunner0ObjId);
			Commands->Send_Custom_Event(obj, gdiMiniGunner0Obj, 445006, 1, 0.0f);

			int randInt = Commands->Get_Random_Int(0, 3);
			
			GameObject *field70Obj = Commands->Find_Object(this->field_70);
			Commands->Send_Custom_Event(obj, field70Obj, 445006, randInt, 0.0f);
			
			GameObject *field74Obj = Commands->Find_Object(this->field_74);
			Commands->Send_Custom_Event(obj, field74Obj, 445006, randInt, 0.0f);
		}
	}
	else if (number == 3)
	{
		if (this->starAtAreaNumber == 0 || this->starAtAreaNumber == 1)
		{
			if (this->wrongWayConvTableIndex == 4)
			{
				this->wrongWayConvTableIndex = 0;
			}

			int conversationId = Commands->Create_Conversation(wrongWayConvTable[this->wrongWayConvTableIndex], 100, 200.0f, false);
			Commands->Join_Conversation(obj, conversationId, false, true, true);
			Commands->Start_Conversation(conversationId, 10);

			this->wrongWayConvTableIndex++;

			Commands->Start_Timer(obj, this, 5.0f, 3);
		}
		else if (this->starAtAreaNumber == 2)
		{
			Commands->Start_Timer(obj, this, 5.0f, 3);
		}
		else if (this->starAtAreaNumber == 3)
		{
			Commands->Create_2D_Sound("MX0_A4_Orca_Mix");

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
			Commands->Set_Facing(invisObj, 0.0f);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X0E_Obelisk.txt");

			GameObject *MX0NodObeliskObj = Commands->Find_Object(this->MX0NodObeliskObjId);
			Commands->Destroy_Object(MX0NodObeliskObj);

			Commands->Start_Timer(obj, this, 5.0f, 7);
			Commands->Start_Timer(obj, this, 10.0f, 8);

			this->canDoFlyoverCinematic = true;

			Commands->Start_Timer(obj, this, 8.0f, 10);
			Commands->Start_Timer(obj, this, 2.0f, 4);
			
			GameObject *gdiRocketSoldier0Obj = Commands->Find_Object(this->gdiRocketSoldier0ObjId);
			Commands->Send_Custom_Event(obj, gdiRocketSoldier0Obj, 445006, 1, 0.0f);

			GameObject *gdiMiniGunner0Obj = Commands->Find_Object(this->gdiMiniGunner0ObjId);
			Commands->Send_Custom_Event(obj, gdiMiniGunner0Obj, 445006, 1, 0.0f);

			int randInt = Commands->Get_Random_Int(0, 3);
			
			GameObject *field70Obj = Commands->Find_Object(this->field_70);
			Commands->Send_Custom_Event(obj, field70Obj, 445006, randInt, 0.0f);
			
			GameObject *field74Obj = Commands->Find_Object(this->field_74);
			Commands->Send_Custom_Event(obj, field74Obj, 445006, randInt, 0.0f);
		}
	}
	else if (number == 10)
	{
		int conversationId = Commands->Create_Conversation("MX0_A04_CON005", 100, 200.0f, false); // Havoc, you’ve got  to clear out those SAM sites!
		Commands->Join_Conversation(NULL, conversationId, false, true, true);
		Commands->Start_Conversation(conversationId, 1);
		
		GameObject *gdiRocketSoldier0Obj = Commands->Find_Object(this->gdiRocketSoldier0ObjId);
		Commands->Send_Custom_Event(obj, gdiRocketSoldier0Obj, 445002, 445014, 3.0f);
	}
	else if (number == 4)
	{
		if (this->starAtAreaNumber == 0 || this->starAtAreaNumber == 1)
		{
			if (this->wrongWayConvTableIndex == 4)
			{
				this->wrongWayConvTableIndex = 0;
			}

			int conversationId = Commands->Create_Conversation(wrongWayConvTable[this->wrongWayConvTableIndex], 100, 200.0f, false);
			Commands->Join_Conversation(obj, conversationId, false, true, true);
			Commands->Start_Conversation(conversationId, 10);

			this->wrongWayConvTableIndex++;

			Commands->Start_Timer(obj, this, 5.0f, 4);
		}
		else if (this->starAtAreaNumber == 2)
		{
			Commands->Start_Timer(obj, this, 5.0f, 4);
		}
		else if (this->starAtAreaNumber == 3)
		{
			if (!Commands->Find_Object(1500015) && !Commands->Find_Object(1500016))// Right and left samsite
			{
				GameObject *gdiRocketSoldier0Obj = Commands->Find_Object(this->gdiRocketSoldier0ObjId);
				Commands->Send_Custom_Event(obj, gdiRocketSoldier0Obj, 445002, 445016, 0.0f);
				
				GameObject *field44Obj = Commands->Find_Object(this->field_44);
				Commands->Send_Custom_Event(obj, field44Obj, 445002, 445017, 2.0f);

				GameObject *rightWallRocketTrooperObj = Commands->Find_Object(this->rightWallRocketTrooperObjId);
				Commands->Send_Custom_Event(obj, rightWallRocketTrooperObj, 445002, this->reinforcementsEngineer1ObjId, 0.0f);
				
				GameObject *leftWallRocketTrooperObj = Commands->Find_Object(this->leftWallRocketTrooperObjId);
				Commands->Send_Custom_Event(obj, leftWallRocketTrooperObj, 445002, this->reinforcementsEngineer1ObjId, 0.0f);

				Commands->Start_Timer(obj, this, 5.0f, 5);
			}
			else
			{
				Commands->Start_Timer(obj, this, 5.0f, 7);
				Commands->Start_Timer(obj, this, 10.0f, 8);

				this->canDoFlyoverCinematic = false;

				if (!this->isObeliskSighted)
				{
					this->isObeliskSighted = true;

					GameObject *gdiRocketSoldier0Obj = Commands->Find_Object(this->gdiRocketSoldier0ObjId);
					Commands->Send_Custom_Event(obj, gdiRocketSoldier0Obj, 445002, 445015, 9.0f);
					
					GameObject *rightSamSiteObj = Commands->Find_Object(1500015);
					Commands->Send_Custom_Event(obj, rightSamSiteObj, 9037, 0, 0.0f);
					
					GameObject *leftSameSiteObj = Commands->Find_Object(1500016);
					Commands->Send_Custom_Event(obj, leftSameSiteObj, 9037, 0, 0.0f);

					Commands->Start_Timer(obj, this, 20.0f, 12);
					Commands->Start_Timer(obj, this, 24.0f, 13);
				}
				
				Commands->Start_Timer(obj, this, 5.0f, 4);
			}
		}
	}
	else if (number == 12)
	{
		GameObject *rightSamSiteObj = Commands->Find_Object(1500015);
		Commands->Apply_Damage(rightSamSiteObj, 50000.0f, "STEEL", NULL);
	}
	else if (number == 13)
	{
		GameObject *leftSameSiteObj = Commands->Find_Object(1500016);
		Commands->Apply_Damage(leftSameSiteObj, 50000.0f, "STEEL", NULL);
	}
	else if (number == 5)
	{
		if (this->starAtAreaNumber == 0 || this->starAtAreaNumber == 1)
		{
			if (this->wrongWayConvTableIndex == 4)
			{
				this->wrongWayConvTableIndex = 0;
			}

			int conversationId = Commands->Create_Conversation(wrongWayConvTable[this->wrongWayConvTableIndex], 100, 200.0f, false);
			Commands->Join_Conversation(obj, conversationId, false, true, true);
			Commands->Start_Conversation(conversationId, 10);

			this->wrongWayConvTableIndex++;

			Commands->Start_Timer(obj, this, 5.0f, 5);
		}
		else if (this->starAtAreaNumber == 2)
		{
			Commands->Start_Timer(obj, this, 5.0f, 5);
		}
		else if (this->starAtAreaNumber == 3)
		{
			Commands->Fade_Background_Music("Renegade_A10_Outro.mp3", 1, 1);

			int conversationId = Commands->Create_Conversation("MX0_A04_CON010", 100, 200.0f, false); // This is Eagle Claw 1 –Starting  attack run
			Commands->Join_Conversation(NULL, conversationId, false, true, true);
			Commands->Start_Conversation(conversationId, 1);

			Commands->Start_Timer(obj, this, 5.0f, 11);

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
			Commands->Set_Facing(invisObj, 0.0f);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X0D_A10_Crash.txt");

			Commands->Start_Timer(obj, this, 6.0f, 6);

			GameObject *gdiRocketSoldier0Obj = Commands->Find_Object(this->gdiRocketSoldier0ObjId);
			Commands->Send_Custom_Event(obj, gdiRocketSoldier0Obj, 445006, 1, 0.0f);

			GameObject *gdiMiniGunner0Obj = Commands->Find_Object(this->gdiMiniGunner0ObjId);
			Commands->Send_Custom_Event(obj, gdiMiniGunner0Obj, 445006, 1, 0.0f);

			int randInt = Commands->Get_Random_Int(0, 3);
			
			GameObject *field70Obj = Commands->Find_Object(this->field_70);
			Commands->Send_Custom_Event(obj, field70Obj, 445006, randInt, 0.0f);
			
			GameObject *field74Obj = Commands->Find_Object(this->field_74);
			Commands->Send_Custom_Event(obj, field74Obj, 445006, randInt, 0.0f);
		}
	}
	else if (number == 11)
	{
		int conversationId = Commands->Create_Conversation("MX0_A04_CON011", 100, 200.0f, false); // I’m hit! I’m hit!
		Commands->Join_Conversation(0, conversationId, false, true, true);
		Commands->Start_Conversation(conversationId, 1);
	}
	else if (number == 6)
	{
		// This is Eagle Base.  I’m not risking any more pilots.
		// We have a GPS lock on the NOD Installation.  Prepare for ION Cannon strike.
		int conversationId = Commands->Create_Conversation("MX0_A04_CON012", 100, 200.0f, false);
		Commands->Join_Conversation(0, conversationId, false, true, true);
		Commands->Start_Conversation(conversationId, 1);

		GameObject *ionCannonStrikeLocObj = Commands->Find_Object(1500087);
		Vector3 ionCannonStrikeLocObjPos = Commands->Get_Position(ionCannonStrikeLocObj);

		GameObject *nodRocketSoldierObj = Commands->Create_Object("Nod_RocketSoldier_1Off", ionCannonStrikeLocObjPos);
		Commands->Attach_Script(nodRocketSoldierObj, "MX0_Plant_Ion_Beacon_DLS", "");

		Commands->Start_Timer(obj, this, 22.0f, 14);
		Commands->Start_Timer(obj, this, 25.0f, 9);
		
		GameObject *gdiRocketSoldier0Obj = Commands->Find_Object(this->gdiRocketSoldier0ObjId);
		Commands->Send_Custom_Event(obj, gdiRocketSoldier0Obj, 445006, 1, 0.0f);
		
		GameObject *gdiMiniGunner0Obj = Commands->Find_Object(this->gdiMiniGunner0ObjId);
		Commands->Send_Custom_Event(obj, gdiMiniGunner0Obj, 445006, 1, 0.0f);

		int randInt = Commands->Get_Random_Int(0, 3);
		
		GameObject *field70Obj = Commands->Find_Object(this->field_70);
		Commands->Send_Custom_Event(obj, field70Obj, 445006, randInt, 0.0f);
		
		GameObject *field74Obj = Commands->Find_Object(this->field_74);
		Commands->Send_Custom_Event(obj, field74Obj, 445006, randInt, 0.0f);
	}
	else if (number == 14)
	{
		Commands->Set_Screen_Fade_Color(1.0f, 1.0f, 1.0f, 0.2f);
		Commands->Set_Screen_Fade_Opacity(1.0f, 0.2f);
	}
	else if (number == 9)
	{
		GameObject *field64Obj = Commands->Find_Object(this->field_64);
		Commands->Destroy_Object(field64Obj);
		
		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
		Commands->Set_Facing(invisObj, 0.0f);
		Commands->Attach_Script(invisObj, "Test_Cinematic", "X0Z_Finale.txt");
	}
	else if (number == 7)
	{
		if (this->canDoFlyoverCinematic)
		{
			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
			Commands->Set_Facing(invisObj, 0.0f);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X0C_Flyovers_01.txt");
		}
	}
	else if (number == 8)
	{
		if (this->canDoFlyoverCinematic)
		{
			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
			Commands->Set_Facing(invisObj, 0.0f);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X0C_Flyovers_02.txt");
		}
	}
}

ScriptRegistrant<MX0_Area4_Controller_DLS> MX0_Area4_Controller_DLSRegistrant("MX0_Area4_Controller_DLS", "");