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
#include "M01_DetentionCiv_Air_Evac_Waypath_JDG.h"

// This script is never used
void M01_DetentionCiv_Air_Evac_Waypath_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Never received
	if (param == 27)
	{
		Commands->Set_Animation(obj, "XG_TransprtBone.XG_EV1_PathZ", false, NULL, 0.0f, -1.0f, false);
	}
}

void M01_DetentionCiv_Air_Evac_Waypath_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "XG_TransprtBone.XG_EV1_PathA"))
	{
		Commands->Set_Animation(obj, "XG_TransprtBone.XG_EV1_Pathloop", true, NULL, 0.0f, -1.0f, false);
	}
	else if (!_stricmp(animation_name, "XG_TransprtBone.XG_EV1_PathZ"))
	{
		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_DetentionCiv_Air_Evac_Waypath_JDG> M01_DetentionCiv_Air_Evac_Waypath_JDGRegistrant("M01_DetentionCiv_Air_Evac_Waypath_JDG", "");