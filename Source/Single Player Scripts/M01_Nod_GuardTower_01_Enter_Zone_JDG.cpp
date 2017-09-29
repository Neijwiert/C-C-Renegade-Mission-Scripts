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
#include "M01_Nod_GuardTower_01_Enter_Zone_JDG.h"

/*
M01 -> 102998
*/
void M01_Nod_GuardTower_01_Enter_Zone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		GameObject *guardTowerSniperTarget1Obj = Commands->Create_Object("Nod_MiniGunner_0", Vector3(-148.379f, 342.954f, 9.656f));
		Commands->Attach_Script(guardTowerSniperTarget1Obj, "M01_GuardTower_Sniper_Target_JDG", "-157.390 363.504 10.695, 148.379 342.954 9.656");

		GameObject *guardTowerSniperTarget2Obj = Commands->Create_Object("Nod_MiniGunner_0", Vector3(-149.93f, 338.9f, 9.17f));
		Commands->Attach_Script(guardTowerSniperTarget2Obj, "M01_GuardTower_Sniper_Target_JDG", "-158.760 357.590 10.695, -149.930 338.900 9.170");

		GameObject *guardTowerSniperTarget3Obj = Commands->Create_Object("Nod_MiniGunner_0", Vector3(-146.812f, 339.745f, 9.345f));
		Commands->Attach_Script(guardTowerSniperTarget3Obj, "M01_GuardTower_Sniper_Target_JDG", "-162.892 357.941 10.655, -146.812 339.745 9.345");

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_Nod_GuardTower_01_Enter_Zone_JDG> M01_Nod_GuardTower_01_Enter_Zone_JDGRegistrant("M01_Nod_GuardTower_01_Enter_Zone_JDG", "");