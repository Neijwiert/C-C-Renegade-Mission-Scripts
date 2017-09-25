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
#include "M01_TibCave_StartZone_JDG.h"

/*
M01 -> 108922
*/
void M01_TibCave_StartZone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		GameObject *visceroid1Obj = Commands->Find_Object(106908);
		GameObject *visceroid2Obj = Commands->Find_Object(106909);
		GameObject *visceroid3Obj = Commands->Find_Object(106907);
		GameObject *visceroidNodGuy1Obj = Commands->Find_Object(106910);
		GameObject *visceroidNodGuy2Obj = Commands->Find_Object(106911);

		if (visceroid1Obj)
		{
			Commands->Send_Custom_Event(obj, visceroid1Obj, 0, 16, 0.0f);
		}

		if (visceroid2Obj)
		{
			Commands->Send_Custom_Event(obj, visceroid2Obj, 0, 16, 0.0f);
		}

		if (visceroid3Obj)
		{
			Commands->Send_Custom_Event(obj, visceroid3Obj, 0, 16, 0.0f);
		}

		if (visceroidNodGuy1Obj)
		{
			Commands->Send_Custom_Event(obj, visceroidNodGuy1Obj, 0, 16, 0.0f);
		}

		if (visceroidNodGuy2Obj)
		{
			Commands->Send_Custom_Event(obj, visceroidNodGuy2Obj, 0, 16, 1.0f);
		}

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_TibCave_StartZone_JDG> M01_TibCave_StartZone_JDGRegistrant("M01_TibCave_StartZone_JDG", "");