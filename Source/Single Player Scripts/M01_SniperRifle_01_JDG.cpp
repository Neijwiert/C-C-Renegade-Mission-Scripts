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
#include "M01_SniperRifle_01_JDG.h"

/*
M01 -> 117636
*/
void M01_SniperRifle_01_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == CUSTOM_EVENT_POWERUP_GRANTED)
	{
		GameObject *M01DataDiscTextControllerJDGObj = Commands->Find_Object(117188);
		if (M01DataDiscTextControllerJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01DataDiscTextControllerJDGObj, 2, 1, 0.0f);
		}

		GameObject *nodMinigunnerObj = Commands->Create_Object("Nod_MiniGunner_0", Vector3(-53.217f, -68.049f, 13.701f));
		Commands->Attach_Script(nodMinigunnerObj, "M01_SniperRifle_01_Target_JDG", "");
	}
}

ScriptRegistrant<M01_SniperRifle_01_JDG> M01_SniperRifle_01_JDGRegistrant("M01_SniperRifle_01_JDG", "");