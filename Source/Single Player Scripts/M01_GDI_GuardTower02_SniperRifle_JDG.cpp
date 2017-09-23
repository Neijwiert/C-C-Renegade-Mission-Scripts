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
#include "M01_GDI_GuardTower02_SniperRifle_JDG.h"

/*
M01 -> 123253
*/
void M01_GDI_GuardTower02_SniperRifle_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		GameObject *M01DataDiscTextControllerJDGObj = Commands->Find_Object(117188);
		if (M01DataDiscTextControllerJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01DataDiscTextControllerJDGObj, 2, 1, 0.0f);
		}

		GameObject *sniperTargetNodMinigunnerObj = Commands->Create_Object("Nod_MiniGunner_0", Vector3(-79.182f, 141.927f, 1.763f));
		Commands->Attach_Script(sniperTargetNodMinigunnerObj, "M01_GuardTower02_Sniper_Target01_JDG", "");
	}
}

ScriptRegistrant<M01_GDI_GuardTower02_SniperRifle_JDG> M01_GDI_GuardTower02_SniperRifle_JDGRegistrant("M01_GDI_GuardTower02_SniperRifle_JDG", "");