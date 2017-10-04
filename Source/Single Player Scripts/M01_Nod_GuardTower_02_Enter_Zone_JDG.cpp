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
#include "M01_Nod_GuardTower_02_Enter_Zone_JDG.h"

// This script is never used
void M01_Nod_GuardTower_02_Enter_Zone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		GameObject *sniperTarget1Obj = Commands->Create_Object("Nod_MiniGunner_0", Vector3(-153.944f, 602.434f, 4.226f));
		Commands->Attach_Script(sniperTarget1Obj, "M01_GuardTower_Sniper_Target_JDG", "-141.665 573.039 4.409, -153.944 602.434 4.226");

		GameObject *sniperTarget2Obj = Commands->Create_Object("Nod_MiniGunner_0", Vector3(-156.672f, 605.082f, 4.163f));
		Commands->Attach_Script(sniperTarget2Obj, "M01_GuardTower_Sniper_Target_JDG", "-138.653 576.128 4.889, -156.672 605.082 4.163");

		GameObject *sniperTarget3Obj = Commands->Create_Object("Nod_MiniGunner_0", Vector3(-160.792f, 606.559f, 4.075f));
		Commands->Attach_Script(sniperTarget3Obj, "M01_GuardTower_Sniper_Target_JDG", "-143.313 580.514 4.337, -160.792 606.559 4.075");

		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_Nod_GuardTower_02_Enter_Zone_JDG> M01_Nod_GuardTower_02_Enter_Zone_JDGRegistrant("M01_Nod_GuardTower_02_Enter_Zone_JDG", "");