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
#include "M01_ConDropZone_JDG.h"

/*
M01 -> 119825
*/
void M01_ConDropZone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(54.835f, -3.011f, 13.105f));
		Commands->Set_Facing(invisObj, 45.0f);
		Commands->Attach_Script(invisObj, "Test_Cinematic", "X01_ConYardDrop.txt"); // This cinematic file does not exist

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_ConDropZone_JDG> M01_ConDropZone_JDGRegistrant("M01_ConDropZone_JDG", "");