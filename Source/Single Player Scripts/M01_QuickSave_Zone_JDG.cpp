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
#include "M01_QuickSave_Zone_JDG.h"

/*
M01 -> 118635 118636
*/
void M01_QuickSave_Zone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		Commands->Set_HUD_Help_Text(8285, Vector3(0.196f, 0.882f, 0.196f)); // Press F6 to make a Quick Save\n

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_QuickSave_Zone_JDG> M01_QuickSave_Zone_JDGRegistrant("M01_QuickSave_Zone_JDG", "");