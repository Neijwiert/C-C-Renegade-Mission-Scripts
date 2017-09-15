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
#include "M02_Player_Vehicle.h"

void M02_Player_Vehicle::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->entered, sizeof(this->entered), 1);
}

// TODO
void M02_Player_Vehicle::Created(GameObject *obj)
{
	this->entered = false;

	Commands->Enable_Vehicle_Transitions(obj, true);
	Commands->Set_Player_Type(obj, SCRIPT_PLAYERTYPE_NEUTRAL);

	Commands->Start_Timer(obj, this, 15.0f, 1);

	int objId = Commands->Get_ID(obj);
	Send_Custom_To_SAM_Sites(obj, 100008, objId);
}

void M02_Player_Vehicle::Destroyed(GameObject *obj)
{
	int areaId = Get_Int_Parameter("Area_ID");

	GameObject *M02ObjectiveControllerObj = Commands->Find_Object(1111112);
	if (M02ObjectiveControllerObj)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 112, areaId, 0.0f);

		if (areaId == 2 && !this->entered)
		{
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 221, 2, 0.0f);
		}
	}
}

void M02_Player_Vehicle::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_VEHICLE_ENTERED && Get_Int_Parameter("Area_ID") == 2 && !this->entered)
	{
		this->entered = true;

		GameObject *M02ObjectiveControllerObj = Commands->Find_Object(1111112);
		if (M02ObjectiveControllerObj)
		{
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 221, 1, 0.0f);
		}
	}
}

void M02_Player_Vehicle::Timer_Expired(GameObject *obj, int number)
{
	// Triggered 15 seconds after creation or 15 second after this block
	if (number == 1)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		if (starObj)
		{
			Vector3 starPos = Commands->Get_Position(starObj);
			if (Commands->Get_Distance(pos, starPos) > 300.0f)
			{
				Commands->Apply_Damage(obj, 10000.0f, "Blamokiller", NULL);
			}
		}

		Commands->Start_Timer(obj, this, 15.0f, 1);
	}
}

void M02_Player_Vehicle::Send_Custom_To_SAM_Sites(GameObject *from, int type, int param)
{
	GameObject *smallBridgeNodSamsiteObj = Commands->Find_Object(1100085);
	if (smallBridgeNodSamsiteObj)
	{
		Commands->Send_Custom_Event(from, smallBridgeNodSamsiteObj, type, param, 0.5f);
	}

	GameObject *entranceSkiResortNodSamSiteObj = Commands->Find_Object(1100094);
	if (entranceSkiResortNodSamSiteObj)
	{
		Commands->Send_Custom_Event(from, entranceSkiResortNodSamSiteObj, type, param, 0.5f);
	}

	GameObject *bigBridgeNodSamSite1Obj = Commands->Find_Object(1100120);
	if (bigBridgeNodSamSite1Obj)
	{
		Commands->Send_Custom_Event(from, bigBridgeNodSamSite1Obj, type, param, 0.5f);
	}

	GameObject *bigBridgeNodSamSite2Obj = Commands->Find_Object(1100130);
	if (bigBridgeNodSamSite2Obj)
	{
		Commands->Send_Custom_Event(from, bigBridgeNodSamSite2Obj, type, param, 0.5f);
	}
}

ScriptRegistrant<M02_Player_Vehicle> M02_Player_VehicleRegistrant("M02_Player_Vehicle", "Area_ID:int");