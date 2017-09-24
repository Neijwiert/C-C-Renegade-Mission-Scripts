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
#include "M01_Sinking_Gunboat_JDG.h"

// When M01_Initial_Gunboat_Script_JDG is killed
void M01_Sinking_Gunboat_JDG::Created(GameObject *obj)
{
	Commands->Set_Animation(obj, "V_GDI_GBOAT.XG_GBOAT_SINK", false, NULL, 0.0f, 60.0f, false);
}

void M01_Sinking_Gunboat_JDG::Animation_Complete(GameObject *obj, const char *animation_name)
{
	if (!_stricmp(animation_name, "V_GDI_GBOAT.XG_GBOAT_SINK"))
	{
		Commands->Destroy_Object(obj);
	}
}

ScriptRegistrant<M01_Sinking_Gunboat_JDG> M01_Sinking_Gunboat_JDGRegistrant("M01_Sinking_Gunboat_JDG", "");