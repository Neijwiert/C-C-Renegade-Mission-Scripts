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
#include "M01_FodderHovercraft_Script_JDG.h"

// After 1 cinematic frame in X1D_FodderHover_MTank.txt
void M01_FodderHovercraft_Script_JDG::Created(GameObject *obj)
{
	GameObject *M01TurretBeachFodderHovercraftControllerJDGObj = Commands->Find_Object(121630);
	if (M01TurretBeachFodderHovercraftControllerJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01TurretBeachFodderHovercraftControllerJDGObj, 0, 0, 0.0f);
	}

	float randDelay = Commands->Get_Random(5.0f, 8.0f);
	Commands->Send_Custom_Event(obj, obj, 0, 16, randDelay);
}

void M01_FodderHovercraft_Script_JDG::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *M01TurretBeachFodderHovercraftControllerJDGObj = Commands->Find_Object(121630);
	if (M01TurretBeachFodderHovercraftControllerJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01TurretBeachFodderHovercraftControllerJDGObj, 0, 22, 0.0f);
	}

	GameObject *M01HovercraftExplosionControllerJDGObj = Commands->Find_Object(121839);
	if (M01HovercraftExplosionControllerJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01HovercraftExplosionControllerJDGObj, 0, 22, 0.0f);
	}
}

void M01_FodderHovercraft_Script_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{

}

void M01_FodderHovercraft_Script_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received by ourselves after 5 to 8 seconds after create
	if (!type && param == 16)
	{
		GameObject *beachNodTurret1Obj = Commands->Find_Object(101434);
		GameObject *beachNodTurret2Obj = Commands->Find_Object(101435);
		if (beachNodTurret1Obj || beachNodTurret2Obj)
		{
			Commands->Apply_Damage(obj, 100000.0f, "STEEL", NULL);
		}
	}
}

ScriptRegistrant<M01_FodderHovercraft_Script_JDG> M01_FodderHovercraft_Script_JDGRegistrant("M01_FodderHovercraft_Script_JDG", "");