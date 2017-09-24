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
#include "M01_Medium_Tank_JDG.h"

void M01_Medium_Tank_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->canCreateSquishGuys, sizeof(this->canCreateSquishGuys), 1);
}

// After 419 cinematic frames in X1D_FodderHover_MTank.txt
void M01_Medium_Tank_JDG::Created(GameObject *obj)
{
	Commands->Create_Sound("m00evag_dsgn0009i1evag_snd", Vector3(0.0f, 0.0f, 0.0f), obj);

	this->canCreateSquishGuys = true;

	int objId = Commands->Get_ID(obj);

	GameObject *M01TankEnteringTunnelZoneJDGObj = Commands->Find_Object(101679);
	if (M01TankEnteringTunnelZoneJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01TankEnteringTunnelZoneJDGObj, 228, objId, 0.0f);
	}

	GameObject *honNodSAMSiteObj = Commands->Find_Object(100031);
	if (honNodSAMSiteObj)
	{
		Commands->Send_Custom_Event(obj, honNodSAMSiteObj, 100008, objId, 0.0f);
	}

	GameObject *M01TurretBeachFodderHovercraftControllerJDGObj = Commands->Find_Object(121630);
	if (M01TurretBeachFodderHovercraftControllerJDGObj)
	{
		Commands->Destroy_Object(M01TurretBeachFodderHovercraftControllerJDGObj);
	}

	GameObject *M01MediumTankReminderZoneJDGObj = Commands->Find_Object(122848);
	if (M01MediumTankReminderZoneJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01MediumTankReminderZoneJDGObj, 0, 16, 0.0f);
	}

	GameObject *M01CantBringMediumTankThroughHereZoneJDGObj = Commands->Find_Object(125661);
	if (M01CantBringMediumTankThroughHereZoneJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01CantBringMediumTankThroughHereZoneJDGObj, 0, 29, 0.0f);
	}
}

void M01_Medium_Tank_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED)
	{
		if (this->canCreateSquishGuys)
		{
			this->canCreateSquishGuys = false;

			GameObject *tunnelSquishGuy1Obj = Commands->Create_Object("Nod_Minigunner_0", Vector3(40.286f, 151.346f, 2.107f));
			GameObject *tunnelSquishGuy2Obj = Commands->Create_Object("Nod_Minigunner_0", Vector3(47.476f, 163.355f, 2.458f));
			GameObject *tunnelSquishGuy3Obj = Commands->Create_Object("Nod_Minigunner_0", Vector3(59.446f, 175.107f, -0.453f));
			Commands->Attach_Script(tunnelSquishGuy1Obj, "M01_Medium_Tank_Tunnel_Squish_Guy_JDG", "");
			Commands->Attach_Script(tunnelSquishGuy2Obj, "M01_Medium_Tank_Tunnel_Squish_Guy_JDG", "");
			Commands->Attach_Script(tunnelSquishGuy3Obj, "M01_Medium_Tank_Tunnel_Squish_Guy_JDG", "");

			Commands->Action_Reset(obj, 100.0f);
		}

		GameObject *M01MediumTankReminderZoneJDGObj = Commands->Find_Object(122848);
		if (M01MediumTankReminderZoneJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01MediumTankReminderZoneJDGObj, 0, 27, 0.0f);
		}

		GameObject *M01CantBringMediumTankThroughHereZoneJDGObj = Commands->Find_Object(125661);
		if (M01CantBringMediumTankThroughHereZoneJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01CantBringMediumTankThroughHereZoneJDGObj, 0, 27, 0.0f);
		}
	}
	else if (type == CUSTOM_EVENT_VEHICLE_EXITED)
	{
		GameObject *M01MediumTankReminderZoneJDGObj = Commands->Find_Object(122848);
		if (M01MediumTankReminderZoneJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01MediumTankReminderZoneJDGObj, 0, 28, 0.0f);
		}

		GameObject *M01CantBringMediumTankThroughHereZoneJDGObj = Commands->Find_Object(125661);
		if (M01CantBringMediumTankThroughHereZoneJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01CantBringMediumTankThroughHereZoneJDGObj, 0, 28, 0.0f);
		}
	}
}

ScriptRegistrant<M01_Medium_Tank_JDG> M01_Medium_Tank_JDGRegistrant("M01_Medium_Tank_JDG", "");