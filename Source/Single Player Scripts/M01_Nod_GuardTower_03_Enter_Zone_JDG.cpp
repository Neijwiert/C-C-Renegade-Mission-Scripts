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
#include "M01_Nod_GuardTower_03_Enter_Zone_JDG.h"

/*
M01 -> 103006
*/
void M01_Nod_GuardTower_03_Enter_Zone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		GameObject *guardTowerSniperTarget1Obj = Commands->Create_Object("Nod_MiniGunner_0", Vector3(-254.164f, 564.968f, 30.63f));
		Commands->Attach_Script(guardTowerSniperTarget1Obj, "M01_GuardTower_Sniper_Target_JDG", "-221.487 556.431 25.927, -254.164 564.968 30.630");

		GameObject *guardTowerSniperTarget2Obj = Commands->Create_Object("Nod_MiniGunner_0", Vector3(-253.935f, 561.105f, 30.922f));
		Commands->Attach_Script(guardTowerSniperTarget2Obj, "M01_GuardTower_Sniper_Target_JDG", "-221.169 561.729 25.662, -253.935 561.105 30.922");

		GameObject *guardTowerSniperTarget3Obj = Commands->Create_Object("Nod_MiniGunner_0", Vector3(-257.185f, 562.242f, 30.54f));
		Commands->Attach_Script(guardTowerSniperTarget3Obj, "M01_GuardTower_Sniper_Target_JDG", "-225.770 558.924 26.635, -257.185 562.242 30.540");
		
		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-200.897f, 533.21f, 19.5f));
		Commands->Attach_Script(invisObj, "Test_Cinematic", "X1I_HON_Medlab_Chinook.txt");

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_Nod_GuardTower_03_Enter_Zone_JDG> M01_Nod_GuardTower_03_Enter_Zone_JDGRegistrant("M01_Nod_GuardTower_03_Enter_Zone_JDG", "");