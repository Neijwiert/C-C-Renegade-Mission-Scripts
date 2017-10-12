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
#include "RMV_Trigger_Zone_2.h"

/*
M03 -> 1144728 1144727
M10 -> 1110055 1110054 1110022 1110021 1110019 1110018 1110017 1110016 1110014 1110013 1110012 1110011 1100158 1100159
*/
void RMV_Trigger_Zone_2::Entered(GameObject *obj, GameObject *enterer)
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
	}
}

ScriptRegistrant<RMV_Trigger_Zone_2> RMV_Trigger_Zone_2Registrant("RMV_Trigger_Zone_2", "TargetID:int, Type:int, Param:int");