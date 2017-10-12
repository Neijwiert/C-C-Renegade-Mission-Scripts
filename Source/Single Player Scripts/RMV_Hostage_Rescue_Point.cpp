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
#include "RMV_Hostage_Rescue_Point.h"

/*
M03 -> 1144671 1144670 1144669
*/
void RMV_Hostage_Rescue_Point::Entered(GameObject *obj, GameObject *enterer)
{
	if (!Commands->Is_A_Star(enterer))
	{
		Commands->Send_Custom_Event(obj, enterer, 999, 999, 0.0f);
	}
}

ScriptRegistrant<RMV_Hostage_Rescue_Point> RMV_Hostage_Rescue_PointRegistrant("RMV_Hostage_Rescue_Point", "");