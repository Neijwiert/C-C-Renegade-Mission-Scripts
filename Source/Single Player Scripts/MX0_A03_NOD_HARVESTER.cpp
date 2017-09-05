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
#include "MX0_A03_NOD_HARVESTER.h"

void MX0_A03_NOD_HARVESTER::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->atEndOfWaypath, sizeof(this->atEndOfWaypath), 1);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 2);
}

void MX0_A03_NOD_HARVESTER::Created(GameObject *obj)
{
	Commands->Set_Health(obj, 1200.0f);

	this->atEndOfWaypath = false;

	ActionParamsStruct params;
	params.Set_Basic(this, 0.0f, 0);
	params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.25f, 1.0f);
	params.WaypathID = 1400093;

	Commands->Action_Goto(obj, params);
}

void MX0_A03_NOD_HARVESTER::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *MX0A03ControllerObj = Commands->Find_Object(1400041);
	if (MX0A03ControllerObj)
	{
		Commands->Send_Custom_Event(obj, MX0A03ControllerObj, 408, 0, 0.0f);
	}
}

void MX0_A03_NOD_HARVESTER::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	
}

void MX0_A03_NOD_HARVESTER::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 413)
	{
		Commands->Apply_Damage(obj, 200.0f, "EXPLOSIVE", NULL);
	}
}

void MX0_A03_NOD_HARVESTER::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (obj)
	{
		Commands->Set_Animation(obj, "V_NOD_HRVSTR.V_NOD_HRVSTR", false, NULL, 0.0f, -1.0f, false);
	}
}

void MX0_A03_NOD_HARVESTER::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (this->atEndOfWaypath)
	{
		this->atEndOfWaypath = false;

		ActionParamsStruct params;
		params.Set_Basic(this, 0.0f, 0);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.25f, 1.0f);
		params.WaypathID = 1400093;

		Commands->Action_Goto(obj, params);
	}
	else
	{
		this->atEndOfWaypath = true;

		ActionParamsStruct params;
		params.Set_Basic(this, 0.0f, 0);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.25f, 1.0f);
		params.WaypathID = 1400093;
		params.WaypointStartID = 1400095;
		params.WaypointEndID = 1400094;

		Commands->Action_Goto(obj, params);
	}
}

ScriptRegistrant<MX0_A03_NOD_HARVESTER> MX0_A03_NOD_HARVESTERRegistrant("MX0_A03_NOD_HARVESTER", "");