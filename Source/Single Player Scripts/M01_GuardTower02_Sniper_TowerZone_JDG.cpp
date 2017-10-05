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
#include "M01_GuardTower02_Sniper_TowerZone_JDG.h"

/*
M01 -> 123645
*/
void M01_GuardTower02_Sniper_TowerZone_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->starInTower, sizeof(this->starInTower), 1);
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 2);
}

void M01_GuardTower02_Sniper_TowerZone_JDG::Created(GameObject *obj)
{
	this->starInTower = false;
	this->field_20 = 0;
}

// TODO
void M01_GuardTower02_Sniper_TowerZone_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M01_GuardTower02_Sniper_Target01_JDG or M01_GuardTower02_Sniper_Target02_JDG when killed
	if (!type && param == 22 && this->starInTower && ++this->field_20 == 1)
	{
		GameObject *sniperTargetNodMinigunnerObj = Commands->Create_Object("Nod_MiniGunner_0", Vector3(-51.423f, 126.198f, 2.537f));
		Commands->Attach_Script(sniperTargetNodMinigunnerObj, "M01_GuardTower02_NewSniperTarget_JDG", "");
	}
}

void M01_GuardTower02_Sniper_TowerZone_JDG::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		this->starInTower = true;
	}
}

void M01_GuardTower02_Sniper_TowerZone_JDG::Exited(GameObject *obj, GameObject *exiter)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (exiter == Commands->Get_A_Star(pos))
	{
		this->starInTower = false;
	}
}

ScriptRegistrant<M01_GuardTower02_Sniper_TowerZone_JDG> M01_GuardTower02_Sniper_TowerZone_JDGRegistrant("M01_GuardTower02_Sniper_TowerZone_JDG", "");