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
#include "M01_Evac_TroopBone_JDG.h"

// When M01_GDIBaseCommander_EvacController_JDG receives custom type 0 with param 27
void M01_Evac_TroopBone_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "XG_EV5_troopBN.XG_EV5_troopBN"))
	{
		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_Evac_TroopBone_JDG> M01_Evac_TroopBone_JDGRegistrant("M01_Evac_TroopBone_JDG", "");