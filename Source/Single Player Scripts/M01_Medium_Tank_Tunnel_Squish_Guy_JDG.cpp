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
#include "M01_Medium_Tank_Tunnel_Squish_Guy_JDG.h"

// When entered in M01_Medium_Tank_JDG
void M01_Medium_Tank_Tunnel_Squish_Guy_JDG::Created(GameObject *obj)
{
	Vector3 pos = Commands->Get_Position(obj);
	Commands->Set_Innate_Soldier_Home_Location(obj, pos, 10.0f);
}

void M01_Medium_Tank_Tunnel_Squish_Guy_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Never received
	if (param == 228)
	{
		Commands->Set_Innate_Is_Stationary(obj, false);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 38);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 1.0f, 0.25f);
		params.WaypathID = 101554;
		params.WaypointStartID = 101561;

		Commands->Action_Goto(obj, params);

		Commands->Create_3D_Sound_At_Bone("M01_Nod_HupHup", obj, "ROOTTRANSFORM");
	}
}

ScriptRegistrant<M01_Medium_Tank_Tunnel_Squish_Guy_JDG> M01_Medium_Tank_Tunnel_Squish_Guy_JDGRegistrant("M01_Medium_Tank_Tunnel_Squish_Guy_JDG", "");