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
#include "MX0_A03_CONTROLLER_DAK.h"

void MX0_A03_CONTROLLER_DAK::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->humveeObjId, sizeof(this->humveeObjId), 1);
	Auto_Save_Variable(&this->field_2C, sizeof(this->field_2C), 2);
	Auto_Save_Variable(&this->field_30, sizeof(this->field_30), 3);
	Auto_Save_Variable(&this->field_34, sizeof(this->field_34), 4);
	Auto_Save_Variable(&this->MX0A03TankObjId, sizeof(this->MX0A03TankObjId), 5);
	Auto_Save_Variable(&this->MX0GDIMiniGunner0BObjId, sizeof(this->MX0GDIMiniGunner0BObjId), 6);
	Auto_Save_Variable(&this->reinforcementsEngineer1ObjId, sizeof(this->reinforcementsEngineer1ObjId), 7);
	Auto_Save_Variable(&this->field_38, sizeof(this->field_38), 8);
	Auto_Save_Variable(&this->field_3C, sizeof(this->field_3C), 9);
	Auto_Save_Variable(&this->field_40, sizeof(this->field_40), 10);
	Auto_Save_Variable(&this->field_44, sizeof(this->field_44), 11);
	Auto_Save_Variable(&this->field_48, sizeof(this->field_48), 12);
	Auto_Save_Variable(&this->field_4C, sizeof(this->field_4C), 14);
	Auto_Save_Variable(&this->field_50, sizeof(this->field_50), 15);
	Auto_Save_Variable(&this->field_4D, sizeof(this->field_4D), 16);
	Auto_Save_Variable(&this->field_4E, sizeof(this->field_4E), 17);
}

void MX0_A03_CONTROLLER_DAK::Created(GameObject *obj)
{
	this->field_50 = 1;
	this->field_4C = false;
	this->field_4D = false;
	this->field_38 = 0;
	this->field_3C = 0;
	this->field_4E = true;
}

void MX0_A03_CONTROLLER_DAK::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		if (this->field_4D && this->field_4C && this->field_4E)
		{
			Commands->Send_Custom_Event(obj, obj, 405, 0, 0.0f);

			GameObject *MX0A03ENDZONEObj = Commands->Find_Object(1400069);
			Commands->Send_Custom_Event(obj, MX0A03ENDZONEObj, 414, 0, 0.0f);
		}
	}
	
	// Received from MX0_A03_NOD_TURRET on create
	else if (type == 403)
	{
		this->field_38++;
	}

	// Received from MX0_A03_NOD_TURRET when killed
	else if (type == 404)
	{
		this->field_38--;
	}
	else if (type == 406)
	{
		this->field_3C++;
	}

	// Received from MX0_NOD_INFRANTRY when killed
	else if (type == 407)
	{
		if (!--this->field_3C && this->field_50 <= 4 && !this->field_4C)
		{
			GameObject *nodLedgeDropLocObj = Commands->Find_Object(1400152);
			Vector3 nodLedgeDropLocObjPos = Commands->Get_Position(nodLedgeDropLocObj);

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", nodLedgeDropLocObjPos);
			Commands->Attach_Script(invisObj, "Test_Cinematic", "MX0_A03_NOD_LedgeDrop.txt");

			this->field_50++;
		}
	}
	else if (type == 405)
	{
		this->field_4E = false;

		GameObject *gdiTroopDropLocObj = Commands->Find_Object(1500051);
		Vector3 gdiTroopDropLocObjPos = Commands->Get_Position(gdiTroopDropLocObj);

		GameObject *invisObj = Commands->Create_Object("Invisible_Object", gdiTroopDropLocObjPos);
		Commands->Set_Facing(invisObj, 0.0f);
		Commands->Attach_Script(invisObj, "Test_Cinematic", "MX0_GDI_TroopDrop_Area4.txt");

		GameObject *MX0A03TankObj = Commands->Find_Object(this->MX0A03TankObjId);
		if (MX0A03TankObj)
		{
			Commands->Action_Reset(MX0A03TankObj, 100.0f);

			ActionParamsStruct params;
			params.Set_Basic(this, 91.0f, 1);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 1.0f, 1.0f);
			params.WaypathID = 1400058;

			Commands->Action_Goto(MX0A03TankObj, params);
		}

		GameObject *humveeObj = Commands->Find_Object(this->humveeObjId);
		if (humveeObj)
		{
			Commands->Send_Custom_Event(obj, humveeObj, 3, 0, 1.25f);
		}

		GameObject *MX0GDIMiniGunner0BObj = Commands->Find_Object(this->MX0GDIMiniGunner0BObjId);
		if (MX0GDIMiniGunner0BObj)
		{
			Commands->Send_Custom_Event(obj, MX0GDIMiniGunner0BObj, 2, 0, 0.0f);
		}
	}

	// Received from MX0_A02_Controller 2 seconds after custom type 262 is triggered
	else if (type == 401)
	{
		Commands->Scale_AI_Awareness(2.0f, 1.0f);

		GameObject *humveeDropLocationObj = Commands->Find_Object(1400042);
		if (humveeDropLocationObj)
		{
			Commands->Debug_Message("***** sending custom, dropping humvee, attaching script\n");
			Commands->Attach_Script(humveeDropLocationObj, "Test_Cinematic", "XG_A03_HumveeDrop_B.txt");
		}

		GameObject *nodBuggyObj = Commands->Find_Object(1400081);
		if (nodBuggyObj)
		{
			Commands->Send_Custom_Event(obj, nodBuggyObj, 1, 0, 0.0f);
		}

		GameObject *MX0GDIMiniGunner0BObj = Commands->Find_Object(this->MX0GDIMiniGunner0BObjId);
		if (MX0GDIMiniGunner0BObj)
		{
			Commands->Attach_Script(MX0GDIMiniGunner0BObj, "MX0_A03_GDI_TROOPER_ONE", "");
			Commands->Send_Custom_Event(obj, MX0GDIMiniGunner0BObj, 1, 0, 2.0f);
		}

		GameObject *nodPlacedMinigunner1 = Commands->Find_Object(1400150);
		if (nodPlacedMinigunner1)
		{
			Commands->Send_Custom_Event(obj, nodPlacedMinigunner1, 417, 0, 0.0f);
		}

		GameObject *nodPlacedMinigunner2 = Commands->Find_Object(1400149);
		if (nodPlacedMinigunner2)
		{
			Commands->Send_Custom_Event(obj, nodPlacedMinigunner2, 417, 0, 0.0f);
		}
	}

	// Received from MX0_A03_NOD_TROOPER_TIB_DEATH when killed
	else if (type == 415)
	{
		this->field_4D = true;

		Commands->Send_Custom_Event(obj, obj, 0, 0, 0.0f);
	}

	// Received from MX0_A03_NOD_HARVESTER when killed (408)

	// Received from MX0_A02_ACTOR after timer number 215 triggered
	// Received from MX0_A02_Controller after custom type 202
	// Received from MX0_A03_HUMVEE when created
	// Received from MX0_A03_TANK when created
	else if (type == 408 || type == 9035)
	{
		if (type == 408)
		{
			this->field_4C = true;

			GameObject *MX0GDIMiniGunner0BObj = Commands->Find_Object(this->MX0GDIMiniGunner0BObjId);
			if (MX0GDIMiniGunner0BObj)
			{
				if (this->field_4E)
				{
					int conversationId = Commands->Create_Conversation("MX0_A03_08", 0, 0.0f, true); // Nice!   That’ll cost 'em!
					Commands->Join_Conversation(MX0GDIMiniGunner0BObj, conversationId, false, false, true);
					Commands->Start_Conversation(conversationId, 0);
				}

				Commands->Send_Custom_Event(obj, MX0GDIMiniGunner0BObj, 4, 0, 0.0f);
			}

			Commands->Send_Custom_Event(obj, obj, 0, 0, 0.0f);
		}

		switch (param)
		{
			case 1:
				this->humveeObjId = Commands->Get_ID(sender);
				break;
			case 2:
				Commands->Send_Custom_Event(obj, sender, 9035, this->humveeObjId, 0.0f);
				break;
			case 3:
				this->field_2C = Commands->Get_ID(sender);
				break;
			case 4:
				Commands->Send_Custom_Event(obj, sender, 9035, this->field_2C, 0.0f);
				break;
			case 5:
				this->field_30 = Commands->Get_ID(sender);
				break;
			case 6:
				Commands->Send_Custom_Event(obj, sender, 9035, this->field_30, 0.0f);
				break;
			case 7:
				this->field_34 = Commands->Get_ID(sender);
				break;
			case 8:
				Commands->Send_Custom_Event(obj, sender, 9035, this->field_34, 0.0f);
				break;
			case 9:
				this->MX0A03TankObjId = Commands->Get_ID(sender);
				break;
			case 10:
				Commands->Send_Custom_Event(obj, sender, 9035, this->MX0A03TankObjId, 0.0f);
				break;
			case 11:
				this->MX0GDIMiniGunner0BObjId = Commands->Get_ID(sender);
				Commands->Debug_Message("***** DAK ***** A03: recieved trooper1 ID.\n");
				break;
			case 12:
				Commands->Send_Custom_Event(obj, sender, 9035, this->MX0GDIMiniGunner0BObjId, 0.0f);
				break;
			case 13:
				this->reinforcementsEngineer1ObjId = Commands->Get_ID(sender);
				Commands->Attach_Script(obj, "M00_Send_Object_ID", "1500017,10,1.0f");
				break;
			case 14:
				Commands->Send_Custom_Event(obj, sender, 9035, this->reinforcementsEngineer1ObjId, 0.0f);
				break;
			case 15:
				this->field_40 = Commands->Get_ID(sender);
				break;
			case 16:
				Commands->Send_Custom_Event(obj, sender, 9035, this->field_40, 0.0f);
				break;
			case 17:
				this->field_44 = Commands->Get_ID(sender);
				break;
			case 18:
				Commands->Send_Custom_Event(obj, sender, 9035, this->field_44, 0.0f);
				break;
			default:
				break;
		}
	}
}

ScriptRegistrant<MX0_A03_CONTROLLER_DAK> MX0_A03_CONTROLLER_DAKRegistrant("MX0_A03_CONTROLLER_DAK", "");
