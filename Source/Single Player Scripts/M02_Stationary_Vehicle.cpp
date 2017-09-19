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
#include "M02_Stationary_Vehicle.h"

// On level start
void M02_Stationary_Vehicle::Created(GameObject *obj)
{
	int areaId = Get_Int_Parameter("Area_ID");

	Commands->Enable_Enemy_Seen(obj, false);

	if (areaId == 99)
	{
		Commands->Innate_Disable(obj);

		Commands->Start_Timer(obj, this, 5.3299999f, 1);
	}
}

void M02_Stationary_Vehicle::Killed(GameObject *obj, GameObject *killer)
{
	int areaId = Get_Int_Parameter("Area_ID");
	int objId = Commands->Get_ID(obj);

	if (areaId == 2)
	{
		if (objId == 401016) // Area 02 left GDI medium tank
		{
			int conversationId = Commands->Create_Conversation("MX2DSGN_DSGN0008", 100, 300.0f, true); // Waaaahhhhhhhhh! <death scream of being exploded in a tank>
			Commands->Join_Conversation(NULL, conversationId, true, true, true);
			
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			Commands->Join_Conversation(starObj, conversationId, true, false, false);
			Commands->Start_Conversation(conversationId, 0);
		}
		else if (objId == 401167) // Area 02 Nod mobile artillery
		{
			GameObject *area2GDIMedTankObj = Commands->Find_Object(401016);
			if (area2GDIMedTankObj)
			{
				int conversationId = Commands->Create_Conversation("MX2DSGN_DSGN0007", 100, 300.0f, true); // You make it look easy, Havoc
				Commands->Join_Conversation(NULL, conversationId, true, true, true);

				Vector3 pos = Commands->Get_Position(obj);
				GameObject *starObj = Commands->Get_A_Star(pos);

				Commands->Join_Conversation(starObj, conversationId, true, false, false);
				Commands->Start_Conversation(conversationId, 0);
			}
		}
	}
}

void M02_Stationary_Vehicle::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M02_Objective_Zone with objId = (400267, 400501, 400502, 400187, 400184, 400186, 400185, 400189, 401079, 401080)
	// Received from M02_Respawn_Controller when created
	if (!type && !param)
	{
		Commands->Enable_Enemy_Seen(obj, true);
	}
}

void M02_Stationary_Vehicle::Sound_Heard(GameObject *obj, const CombatSound & sound)
{
	// Received from M02_Objective_Zone
	if (sound.sound == 1000 && sound.Creator && !Commands->Get_Player_Type(obj))
	{
		Commands->Send_Custom_Event(obj, sound.Creator, 100, 1, 0.0f);
	}
}

void M02_Stationary_Vehicle::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	if (Get_Int_Parameter("Area_ID") != 99)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 90.0f, 0);
		params.Set_Attack(enemy, 300.0f, 0.0f, true);
		params.AttackCheckBlocked = false;

		Commands->Action_Attack(obj, params);

		Commands->Start_Timer(obj, this, 5.0f, 2);
	}
}

void M02_Stationary_Vehicle::Timer_Expired(GameObject *obj, int number)
{
	// Triggered 5.3299999 seconds after creation
	if (number == 1)
	{
		Commands->Apply_Damage(obj, 10000.0f, "Blamokiller", NULL);
	}

	// Trigered 5 seconds after enemy_seen
	else if (number == 2)
	{
		Commands->Action_Reset(obj, 100.0f);
	}
}

ScriptRegistrant<M02_Stationary_Vehicle> M02_Stationary_VehicleRegistrant("M02_Stationary_Vehicle", "Area_ID:int");