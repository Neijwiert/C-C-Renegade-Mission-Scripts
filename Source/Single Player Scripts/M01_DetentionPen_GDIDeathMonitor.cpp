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
#include "M01_DetentionPen_GDIDeathMonitor.h"

/*
M01 -> 101928 101927 101926
*/
void M01_DetentionPen_GDIDeathMonitor::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->damaged, sizeof(this->damaged), 1);
}

void M01_DetentionPen_GDIDeathMonitor::Created(GameObject *obj)
{
	this->damaged = false;
}

void M01_DetentionPen_GDIDeathMonitor::Killed(GameObject *obj, GameObject *killer)
{
	Commands->Set_HUD_Help_Text(8243, Vector3(0.196f, 0.882f, 0.196f)); // Prisoner Killed\n
	Commands->Mission_Complete(false);

	GameObject *nonExistingObj = Commands->Find_Object(103380);
	Commands->Send_Custom_Event(obj, nonExistingObj, 0, 229, 0.0f);
}

void M01_DetentionPen_GDIDeathMonitor::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (obj)
	{
		if (!this->damaged)
		{
			this->damaged = true;

			Commands->Set_HUD_Help_Text(8242, Vector3(0.196f, 0.882f, 0.196f)); // Rescue the Prisoners\n
		}
	}
}

// TODO
void M01_DetentionPen_GDIDeathMonitor::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (param == 4002)
	{
		GameObject *nonExistingObj = Commands->Find_Object(103380);
		Vector3 nonExistingObjPos = Commands->Get_Position(nonExistingObj);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 4002);
		params.Set_Movement(nonExistingObjPos, 0.8f, 0.25f);

		Commands->Action_Goto(obj, params);
	}
}

ScriptRegistrant<M01_DetentionPen_GDIDeathMonitor> M01_DetentionPen_GDIDeathMonitorRegistrant("M01_DetentionPen_GDIDeathMonitor", "");