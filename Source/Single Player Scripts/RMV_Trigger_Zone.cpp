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
#include "RMV_Trigger_Zone.h"

/*
M03 -> 1144636 1100000 1100001 1100005 1100006 1100007 1100013 1100014 1100015 1141149 1144502 1144675
M10 -> 1100155
*/
void RMV_Trigger_Zone::Entered(GameObject *obj, GameObject *enterer)
{
	if (Commands->Is_A_Star(enterer))
	{
		int type = Get_Int_Parameter("Type");
		int param = Get_Int_Parameter("Param");

		int targetId = Get_Int_Parameter("TargetID");
		GameObject *targetObj = Commands->Find_Object(targetId);
		if (targetObj)
		{
			Commands->Send_Custom_Event(obj, targetObj, type, param, 0.0f);
		}

		if (obj)
		{
			Commands->Destroy_Object(obj);
		}
	}
}

ScriptRegistrant<RMV_Trigger_Zone> RMV_Trigger_ZoneRegistrant("RMV_Trigger_Zone", "TargetID:int, Type:int, Param:int");