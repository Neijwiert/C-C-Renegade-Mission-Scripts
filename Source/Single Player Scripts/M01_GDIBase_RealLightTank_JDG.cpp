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
#include "M01_GDIBase_RealLightTank_JDG.h"

/*
M01 -> 120022
*/
void M01_GDIBase_RealLightTank_JDG::Killed(GameObject *obj, GameObject *killer)
{

}

void M01_GDIBase_RealLightTank_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		// Received from M01_RealLightTank_TriggerZone_JDG when entered
		if (param == 16)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 38);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.7f, 10.0f);
			params.WaypathID = 120218;

			Commands->Action_Goto(obj, params);

			GameObject *M01AirstrikeControllerJDGObj = Commands->Find_Object(121041);
			if (M01AirstrikeControllerJDGObj)
			{
				Commands->Send_Custom_Event(obj, M01AirstrikeControllerJDGObj, 0, 16, 0.0f);
				Commands->Send_Custom_Event(obj, M01AirstrikeControllerJDGObj, 0, 27, 15.0f);
			}
		}

		// Received from M01_Airstrike_Controller_JDG when it receives custom type <= 0 and param <= 0
		else if (param == 17)
		{
			if (obj)
			{
				Commands->Apply_Damage(obj, 10000.0f, "BlamoKiller", NULL);
			}
		}
	}
}

void M01_GDIBase_RealLightTank_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	// When done following waypath see param 16
	if (complete_reason == ACTION_COMPLETE_NORMAL && action_id == 38)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 17);
		
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		params.Set_Attack(starObj, 100.0f, 10.0f, true);
		params.AttackCheckBlocked = false;

		Commands->Action_Attack(obj, params);
	}
}

ScriptRegistrant<M01_GDIBase_RealLightTank_JDG> M01_GDIBase_RealLightTank_JDGRegistrant("M01_GDIBase_RealLightTank_JDG", "");