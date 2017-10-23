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
#include "RMV_Trigger_Poked.h"

/*
M03 -> 1100009
*/
void RMV_Trigger_Poked::Poked(GameObject *obj, GameObject *poker)
{
	int type = Get_Int_Parameter("Type");
	int param = Get_Int_Parameter("Param");
	int targetId = Get_Int_Parameter("Target_ID");
	
	GameObject *to;
	if (targetId)
	{
		to = Commands->Find_Object(targetId);
	}
	else
	{
		to = poker;
	}

	if (to)
	{
		Commands->Send_Custom_Event(obj, to, type, param, 0.0f);
	}
}

ScriptRegistrant<RMV_Trigger_Poked> RMV_Trigger_PokedRegistrant("RMV_Trigger_Poked", "Target_ID:int, Type:int, Param:int");