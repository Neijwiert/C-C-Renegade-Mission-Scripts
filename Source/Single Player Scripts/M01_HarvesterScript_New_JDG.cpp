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
#include "M01_HarvesterScript_New_JDG.h"

void M01_HarvesterScript_New_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->harvestAnimationCount, sizeof(this->harvestAnimationCount), 1);
	Auto_Save_Variable(&this->wasDamaged, sizeof(this->wasDamaged), 2);
	Auto_Save_Variable(&this->killed, sizeof(this->killed), 3);
}

// When M01_MissioN_Controller_JDG receives custom type 0 with param 148 or 153
void M01_HarvesterScript_New_JDG::Created(GameObject *obj)
{
	this->harvestAnimationCount = false;
	this->wasDamaged = false;
	this->killed = false;

	ActionParamsStruct params;
	params.Set_Basic(this, 100.0f, 38);

	float randXOffset = Commands->Get_Random(-1.0f, 1.0f);
	float randYOffset = Commands->Get_Random(-1.0f, 1.0f);

	params.Set_Movement(Vector3(-103.5f + randXOffset, 375.60001f + randYOffset, 19.0f), 0.25f, 1.0f);
	Commands->Action_Goto(obj, params);
}

void M01_HarvesterScript_New_JDG::Killed(GameObject *obj, GameObject *killer)
{
	this->killed = true;

	Commands->Create_Sound("EVA_Nod_Unit_Destroyed", Vector3(0.0f, 0.0f, 0.0f), obj);

	GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-93.9f, 426.6f, 21.0f));
	Commands->Attach_Script(invisObj, "Test_Cinematic", "X1D_GDIPowerUpDrop.txt");
}

void M01_HarvesterScript_New_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (damager)
	{
		Vector3 pos = Commands->Get_Position(obj);
		if (damager == Commands->Get_A_Star(pos) && !this->wasDamaged)
		{
			if (!this->killed)
			{
				Commands->Send_Custom_Event(obj, obj, 0, 16, 0.25f);
			}

			this->wasDamaged = true;
		}
	}
}

void M01_HarvesterScript_New_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from ourselves, see damaged
	if (!type && param == 16 && !this->killed)
	{
		int conversationId = Commands->Create_Conversation("M01_Add_Harvester_Objective", 100, 1000.0f, true); // The Nod Harvester is collecting Tiberium for the Refinery. Destroy it to slow production.
		Commands->Join_Conversation(NULL, conversationId, false, false, true);
		Commands->Start_Conversation(conversationId, conversationId);
	}
}

void M01_HarvesterScript_New_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// Triggered when we complete move action on created or see animation_complete
	if (action_id == 38)
	{
		this->harvestAnimationCount = 0;

		Commands->Set_Animation(obj, "V_NOD_HRVSTR.V_NOD_HRVSTR", false, NULL, 0.0f, -1.0f, false);
	}
}

void M01_HarvesterScript_New_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (this->harvestAnimationCount > 2)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 38);

		float randXOffset = Commands->Get_Random(-3.0, 3.0);
		float randYOffset = Commands->Get_Random(-3.0, 3.0);

		params.Set_Movement(Vector3(-103.5f + randXOffset, 375.60001f + randYOffset, 19.0f), 0.25f, 2.0f);

		Commands->Action_Goto(obj, params);
	}
	else
	{
		Commands->Set_Animation(obj, "V_NOD_HRVSTR.V_NOD_HRVSTR", false, NULL, 0.0f, -1.0f, false);

		this->harvestAnimationCount++;
	}
}

ScriptRegistrant<M01_HarvesterScript_New_JDG> M01_HarvesterScript_New_JDGRegistrant("M01_HarvesterScript_New_JDG", "");