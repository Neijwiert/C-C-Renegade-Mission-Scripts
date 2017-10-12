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
#include "M03_Wheres_The_Star.h"

/*
M03 -> 1144638 1144636 1144634 1144632 1144631 1144628 1100013 1144622 1144624 1144625 1144626
*/
void M03_Wheres_The_Star::Entered(GameObject *obj, GameObject *enterer)
{
	int controllerId = Get_Int_Parameter("Controller_ID");
	GameObject *controllerObj = Commands->Find_Object(controllerId);
	int param = Get_Int_Parameter("Param");
	int type = Get_Int_Parameter("Type");

	Commands->Send_Custom_Event(obj, controllerObj, type, param, 0.0f);
}

ScriptRegistrant<M03_Wheres_The_Star> M03_Wheres_The_StarRegistrant("M03_Wheres_The_Star", "Controller_ID:int, Type:int, Param:int");