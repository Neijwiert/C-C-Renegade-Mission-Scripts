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
#include "DAK_MX0_Sec_3_Humvee.h"

// This script is never used
void DAK_MX0_Sec_3_Humvee::Entered(GameObject *obj, GameObject *enterer)
{
	ActionParamsStruct params;
	params.Set_Basic(this, 100.0f, 100);
	params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.5f, 5.0f);
	params.WaypathID = 1400037; // Does not exist
	params.WaypointStartID = 1400038; // Does not exist

	GameObject *nonExistingObj = Commands->Find_Object(1400035);  // Does not exist
	Commands->Action_Goto(nonExistingObj, params);

	Commands->Debug_Message("***** DAK_MX0_Sec_3_Humvee:Entered code exectued.\n");
}

ScriptRegistrant<DAK_MX0_Sec_3_Humvee> DAK_MX0_Sec_3_HumveeRegistrant("DAK_MX0_Sec_3_Humvee", "");