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
#include "MX0_A03_NOD_BUGGIE.h"

void MX0_A03_NOD_BUGGIE::Created(GameObject *obj)
{

}

void MX0_A03_NOD_BUGGIE::Killed(GameObject *obj, GameObject *killer)
{
	Vector3 pos = Commands->Get_Position(obj);

	GameObject *nodMinigunnerObj = Commands->Create_Object("Nod_Minigunner_0", pos);
	Commands->Attach_Script(nodMinigunnerObj, "MX0_A03_NOD_TROOPER_TIB_DEATH", "");
}

void MX0_A03_NOD_BUGGIE::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 1)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 0.0f, 0);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.25f, 1.0f);
		params.WaypathID = 1400096;

		Commands->Action_Goto(obj, params);
	}
}

void MX0_A03_NOD_BUGGIE::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (!action_id)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 0.0f, 0);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.33f, 1.0f);
		params.WaypathID = 1400096;

		Commands->Action_Goto(obj, params);
	}
}

ScriptRegistrant<MX0_A03_NOD_BUGGIE> MX0_A03_NOD_BUGGIERegistrant("MX0_A03_NOD_BUGGIE", "");