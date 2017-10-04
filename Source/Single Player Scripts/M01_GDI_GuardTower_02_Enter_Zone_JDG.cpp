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
#include "M01_GDI_GuardTower_02_Enter_Zone_JDG.h"

// This script is never used
void M01_GDI_GuardTower_02_Enter_Zone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		GameObject *sniperTarget1Obj = Commands->Create_Object("Nod_MiniGunner_0", pos);
		Commands->Attach_Script(sniperTarget1Obj, "M01_GuardTower_Sniper_Target_JDG", "-90.385 124.308 1.610, -79.477 139.201 0.947");
		
		GameObject *sniperTarget2Obj = Commands->Create_Object("Nod_MiniGunner_0", pos);
		Commands->Attach_Script(sniperTarget2Obj, "M01_GuardTower_Sniper_Target_JDG", "-85.499 117.221 1.915, -79.477 139.201 0.947");
		
		GameObject *sniperTarget3Obj = Commands->Create_Object("Nod_MiniGunner_0", pos);
		Commands->Attach_Script(sniperTarget3Obj, "M01_GuardTower_Sniper_Target_JDG", "-93.364 113.202 1.701, -79.477 139.201 0.947");

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_GDI_GuardTower_02_Enter_Zone_JDG> M01_GDI_GuardTower_02_Enter_Zone_JDGRegistrant("M01_GDI_GuardTower_02_Enter_Zone_JDG", "");