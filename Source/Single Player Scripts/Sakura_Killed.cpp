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
#include "Sakura_Killed.h"

void Sakura_Killed::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
}

// When RMV_M03_Comm_Center_Terminal completed converstion, see action complete
void Sakura_Killed::Created(GameObject *obj)
{
	GameObject *nodHarvesterObj = Commands->Find_Object(1144518);
	if (nodHarvesterObj)
	{
		Commands->Send_Custom_Event(obj, nodHarvesterObj, 622, 622, 0.0f);
	}
}

void Sakura_Killed::Killed(GameObject *obj, GameObject *killer)
{
	Commands->Create_Object("Sakura Crash Controller", Vector3(0.0f, 0.0f, 0.0f));

	GameObject *dlsVolcanoActiveScriptZoneObj = Commands->Find_Object(1300001);
	if (dlsVolcanoActiveScriptZoneObj)
	{
		Commands->Send_Custom_Event(obj, dlsVolcanoActiveScriptZoneObj, 6000, 6600, 0.0f);
	}

	Vector3 pos = Commands->Get_Position(obj);
	GameObject *starObj = Commands->Get_A_Star(pos);

	Commands->Grant_Key(starObj, 6, true);
}

void Sakura_Killed::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	Vector3 pos = Commands->Get_Position(obj);
	float health = Commands->Get_Health(obj);

	if (damager != Commands->Get_A_Star(pos) && health < 5.0f)
	{
		Commands->Set_Health(obj, health + 1.0f);
	}
}

ScriptRegistrant<Sakura_Killed> Sakura_KilledRegistrant("Sakura_Killed", "");