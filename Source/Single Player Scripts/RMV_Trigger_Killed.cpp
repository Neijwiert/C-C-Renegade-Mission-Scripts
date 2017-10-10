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
#include "RMV_Trigger_Killed.h"

/*
M03 -> 1146701 1146700 300009 300028 300056 300032 1141162 1150001 1150002 1150003 1144451 1100020 1144677 1144678 1144679 1144448 1144731 1144732 300010
M10 -> 1153940 1153939 1153933 1153932 1153931 1100007 1100008 1100009 1100010 1100011 1100012 1100013 1100014 1154061 1154083 1154084
x10i_troopdrop.txt at frame(s): 401 581 761
*/
void RMV_Trigger_Killed::Killed(GameObject *obj, GameObject *killer)
{
	int id = Get_Int_Parameter("ID");
	GameObject *idObj = Commands->Find_Object(id);
	int type = Get_Int_Parameter("Type");
	int param = Get_Int_Parameter("Param");

	Commands->Send_Custom_Event(obj, idObj, type, param, 0.0f);
}

ScriptRegistrant<RMV_Trigger_Killed> RMV_Trigger_KilledRegistrant("RMV_Trigger_Killed", "ID:int, Type:int, Param:int");