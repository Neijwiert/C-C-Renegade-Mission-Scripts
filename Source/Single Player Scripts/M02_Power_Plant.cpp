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
#include "M02_Power_Plant.h"

// On level start
void M02_Power_Plant::Created(GameObject *obj)
{
	this->notifiedForInternalRepairs = false;
}

void M02_Power_Plant::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *M02ObjectiveControllerObj = Commands->Find_Object(1111112);
	if (M02ObjectiveControllerObj)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 217, 1, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 202, 1, 0.0f);
	}

	GameObject *nodObeliskBuildingObj = Commands->Find_Object(1153845);
	if (nodObeliskBuildingObj)
	{
		Commands->Set_Building_Power(nodObeliskBuildingObj, false);
	}

	GameObject *damScriptZone1Obj = Commands->Find_Object(405116);
	if (damScriptZone1Obj)
	{
		Commands->Destroy_Object(damScriptZone1Obj);
	}

	GameObject *damScriptZone2Obj = Commands->Find_Object(405117);
	if (damScriptZone2Obj)
	{
		Commands->Destroy_Object(damScriptZone2Obj);
	}

	GameObject *honBuildingObj = Commands->Find_Object(1157690);
	if (honBuildingObj)
	{
		Commands->Set_Building_Power(honBuildingObj, false);
	}
}

void M02_Power_Plant::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	GameObject *nodPowerPlantNodEngineerObj = Commands->Find_Object(400199);
	if (nodPowerPlantNodEngineerObj)
	{
		Commands->Send_Custom_Event(obj, nodPowerPlantNodEngineerObj, 99, 217, 0.0f);
		
		float maxHealth = Commands->Get_Max_Health(obj);
		Commands->Set_Health(obj, maxHealth);

		if (!this->notifiedForInternalRepairs)
		{
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			if (damager == starObj)
			{
				this->notifiedForInternalRepairs = true;

				Commands->Stop_All_Conversations();

				int conversationId = Commands->Create_Conversation("M02_EVA_POWER_REPAIR", 100, 300.0f, true); // The Nod Power Plant is being repaired internally, external destruction is not possible.
				Commands->Join_Conversation(NULL, conversationId, true, true, true);
				Commands->Join_Conversation(starObj, conversationId, true, false, false);
				Commands->Start_Conversation(conversationId, 0);
			}
		}
	}
}

void M02_Power_Plant::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M02_Objective_Zone with objId = 400274
	if (type == 1)
	{
		this->notifiedForInternalRepairs = true;
	}
}

ScriptRegistrant<M02_Power_Plant> M02_Power_PlantRegistrant("M02_Power_Plant", "");