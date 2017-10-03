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
#include "M11_Temple_Hologram_01_JDG.h"

/*
M01 -> 157978
M11 -> 100252
*/
void M11_Temple_Hologram_01_JDG::Created(GameObject *obj)
{
	Commands->Set_Animation(obj, "DSP_HOLO_BIG.DSP_HOLO_BIG", true, NULL, 0.0f, -1.0f, false);
}

ScriptRegistrant<M11_Temple_Hologram_01_JDG> M11_Temple_Hologram_01_JDGRegistrant("M11_Temple_Hologram_01_JDG", "");