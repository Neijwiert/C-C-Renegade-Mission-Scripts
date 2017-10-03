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
#include "M01_Comm_Mainframe_PogZone_02_JDG.h"

/*
M01 -> 108026
*/
void M01_Comm_Mainframe_PogZone_02_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		Commands->Set_Objective_HUD_Info_Position(107, 93.0f, "POG_M01_1_02.tga", 7370, Vector3(-292.135f, 522.74f, 16.448f)); // Access

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_Comm_Mainframe_PogZone_02_JDG> M01_Comm_Mainframe_PogZone_02_JDGRegistrant("M01_Comm_Mainframe_PogZone_02_JDG", "");