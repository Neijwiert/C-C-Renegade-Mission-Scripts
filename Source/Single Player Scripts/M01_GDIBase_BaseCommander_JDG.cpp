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
#include "M01_GDIBase_BaseCommander_JDG.h"

/*
M01 -> 106050
*/
void M01_GDIBase_BaseCommander_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->conversation1Id, sizeof(this->conversation1Id), 1);
	Auto_Save_Variable(&this->gaveIonCannonBeacon, sizeof(this->gaveIonCannonBeacon), 2);
	Auto_Save_Variable(&this->conversation2Id, sizeof(this->conversation2Id), 3);
	Auto_Save_Variable(&this->field_28, sizeof(this->field_28), 4);
}

void M01_GDIBase_BaseCommander_JDG::Created(GameObject *obj)
{
	Commands->Set_Obj_Radar_Blip_Color(obj, RADAR_BLIP_COLOR_GDI);

	this->field_28 = false;
	this->gaveIonCannonBeacon = false;

	Commands->Set_Innate_Is_Stationary(obj, true);
	Commands->Set_Player_Type(obj, SCRIPT_PLAYERTYPE_NEUTRAL);
}

void M01_GDIBase_BaseCommander_JDG::Killed(GameObject *obj, GameObject *killer)
{
	if (!this->gaveIonCannonBeacon)
	{
		Commands->Set_Objective_Status(102, OBJECTIVE_STATUS_FAILED);
	}

	GameObject *M01CommanderShackZoneJDGObj = Commands->Find_Object(115985);
	if (M01CommanderShackZoneJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01CommanderShackZoneJDGObj, 0, 22, 0.0f);
	}

	GameObject *gdiPOW1Obj = Commands->Find_Object(116386);
	if (gdiPOW1Obj)
	{
		Commands->Apply_Damage(gdiPOW1Obj, 10000.0f, "Steel", NULL);
	}

	GameObject *gdiPOW2Obj = Commands->Find_Object(116385);
	if (gdiPOW2Obj)
	{
		Commands->Apply_Damage(gdiPOW2Obj, 10000.0f, "Steel", NULL);
	}

	GameObject *gdiMinigunnerObj = Commands->Find_Object(116382);
	if (gdiMinigunnerObj)
	{
		Commands->Send_Custom_Event(obj, gdiMinigunnerObj, 0, 22, 0.0f);
	}

	GameObject *gdiGrenadierObj = Commands->Find_Object(116383);
	if (gdiGrenadierObj)
	{
		Commands->Send_Custom_Event(obj, gdiGrenadierObj, 0, 22, 0.0f);
	}

	GameObject *nodMinigunner1Obj = Commands->Create_Object("Nod_MiniGunner_0", Vector3(28.566f, 29.79f, 1.792f));
	GameObject *nodMinigunner2Obj = Commands->Create_Object("Nod_MiniGunner_0", Vector3(65.899f, 46.347f, -0.294f));
	GameObject *nodMinigunner3Obj = Commands->Create_Object("Nod_MiniGunner_0", Vector3(24.776f, 44.863f, 0.708f));
	Commands->Set_Innate_Soldier_Home_Location(nodMinigunner1Obj, Vector3(28.566f, 29.79f, 1.792f), 10.0f);
	Commands->Set_Innate_Soldier_Home_Location(nodMinigunner2Obj, Vector3(65.899f, 46.347f, -0.294f), 10.0f);
	Commands->Set_Innate_Soldier_Home_Location(nodMinigunner3Obj, Vector3(24.776f, 44.863f, 0.708f), 10.0f);
}

void M01_GDIBase_BaseCommander_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (Commands->Get_Health(obj) <= 10.0f)
	{
		Commands->Set_Health(obj, 10.0f);
	}
}

// TODO
void M01_GDIBase_BaseCommander_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		if (param == 16)
		{
			Commands->Set_Player_Type(obj, SCRIPT_PLAYERTYPE_GDI);

			Commands->Send_Custom_Event(obj, obj, 0, 4, 1.0f);
		}
		else if (param == 1)
		{
			this->field_28 = true;
		}
		else if (param == 4)
		{
			GameObject *nodAssailant1Obj = Commands->Find_Object(101399);
			GameObject *nodAssailant2Obj = Commands->Find_Object(103335);
			GameObject *nodAssailant3Obj = Commands->Find_Object(103340);
			GameObject *nodAssailant4Obj = Commands->Find_Object(102185);
			if (nodAssailant1Obj || nodAssailant2Obj || nodAssailant3Obj || nodAssailant4Obj)
			{
				Commands->Send_Custom_Event(obj, obj, 0, 4, 0.25f);
			}
			else
			{
				GameObject *nonExistingObj = Commands->Find_Object(105615);
				if (nonExistingObj)
				{
					Commands->Destroy_Object(nonExistingObj);
				}

				GameObject *M01CommanderShackZoneJDGObj = Commands->Find_Object(115985);
				if (M01CommanderShackZoneJDGObj)
				{
					Commands->Send_Custom_Event(obj, M01CommanderShackZoneJDGObj, 0, 16, 0.0f);
				}
			}
		}
		else if (param == 9)
		{
			Commands->Enable_Hibernation(obj, false);

			Commands->Action_Reset(obj, 100.0f);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 9);
			params.Set_Movement(Vector3(55.9f, 40.3f, -0.2f), 0.8f, 1.0f);
			
			Commands->Action_Goto(obj, params);
		}
		else if (param == 4001)
		{
			Commands->Action_Reset(obj, 100.0f);

			GameObject *M01GDIBaseCommanderEvacControllerJDGObj = Commands->Find_Object(106694);
			if (M01GDIBaseCommanderEvacControllerJDGObj)
			{
				Commands->Set_Innate_Is_Stationary(obj, false);

				Vector3 M01GDIBaseCommanderEvacControllerJDGObjPos = Commands->Get_Position(M01GDIBaseCommanderEvacControllerJDGObj);

				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 4001);
				params.Set_Movement(M01GDIBaseCommanderEvacControllerJDGObjPos, 0.8f, 1.0f);
				
				Commands->Action_Goto(obj, params);
			}
		}
		else if (param == 27)
		{
			// Good timing.  Nod was closing in.
			// I've been protecting something special.
			this->conversation1Id = Commands->Create_Conversation("M01_GDI04_Conversation_01", 100, 1000.0f, false);
			Commands->Join_Conversation(obj, this->conversation1Id, false, true, true);
			
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			Commands->Join_Conversation(starObj, this->conversation1Id, false, false, false);
			Commands->Start_Conversation(this->conversation1Id, this->conversation1Id);
			Commands->Monitor_Conversation(obj, this->conversation1Id);
		}
	}
}

// TODO
void M01_GDIBase_BaseCommander_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED)
	{
		if (action_id == this->conversation1Id)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 46);
			params.Set_Animation("H_A_J12C", false);

			Commands->Action_Play_Animation(obj, params);

			Commands->Set_Objective_Status(102, OBJECTIVE_STATUS_ACCOMPLISHED);
		}
		else if (action_id == this->conversation2Id)
		{
			GameObject *M01GDIBaseCommanderEvacControllerJDGObj = Commands->Find_Object(106694);
			if (M01GDIBaseCommanderEvacControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01GDIBaseCommanderEvacControllerJDGObj, 0, 16, 0.0f);
			}
		}
	}
	else if (complete_reason == ACTION_COMPLETE_NORMAL && action_id != 39)
	{
		if (action_id == 46)
		{
			Commands->Create_Object("POW_IonCannonBeacon_Player", Vector3(114.848f, 95.401f, 8.923f));

			this->gaveIonCannonBeacon = true;

			// It's a prototype homing beacon for GDI's new space-based ion cannon.
			// Great for taking out large structures.
			// Once you place the beacon, you have ten seconds to clear the area.
			// I got to go see to my men.  Good luck.
			this->conversation2Id = Commands->Create_Conversation("M01_GDI04_Conversation_02", 100, 1000.0f, false);
			Commands->Join_Conversation(obj, this->conversation2Id, false, true, true);
		
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			Commands->Join_Conversation(starObj, this->conversation2Id, false, false, false);
			Commands->Start_Conversation(this->conversation2Id, this->conversation2Id);
			Commands->Monitor_Conversation(obj, this->conversation2Id);
		}
		else if (action_id == 9)
		{
			Commands->Set_Innate_Is_Stationary(obj, true);
			
			GameObject *M01GDIBaseCommanderEvacControllerJDGObj = Commands->Find_Object(106694);
			if (M01GDIBaseCommanderEvacControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01GDIBaseCommanderEvacControllerJDGObj, 0, 27, 0.0f);
			}
		}
		else if (action_id == 4001)
		{
			Vector3 dropPosition = Commands->Get_Position(obj);
			dropPosition.Z += 0.75f;

			static const char *drops[2] = 
			{
				"POW_Health_100",
				"POW_Armor_100"
			};

			int randDropindex = Commands->Get_Random_Int(0, 2);

			Commands->Create_Object(drops[randDropindex], dropPosition);
			
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			Commands->Give_Points(starObj, 2000.0f, false);

			GameObject *M01GDIBaseCommanderEvacControllerJDGObj = Commands->Find_Object(106694);
			if (M01GDIBaseCommanderEvacControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01GDIBaseCommanderEvacControllerJDGObj, 0, 10, 0.0f);
			}
		}
	}
}

void M01_GDIBase_BaseCommander_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "S_A_Human.XG_EV5_troop"))
	{
		if (!this->gaveIonCannonBeacon)
		{
			Commands->Set_Objective_Status(102, OBJECTIVE_STATUS_FAILED);
		}

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_GDIBase_BaseCommander_JDG> M01_GDIBase_BaseCommander_JDGRegistrant("M01_GDIBase_BaseCommander_JDG", "");