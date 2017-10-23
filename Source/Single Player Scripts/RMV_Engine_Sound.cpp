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
#include "RMV_Engine_Sound.h"

void RMV_Engine_Sound::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->engineSoundId, sizeof(this->engineSoundId), 1);
}

// When RMV_Volcano_And_Lava_Ball_Creator creates a fireball, see timer expired
void RMV_Engine_Sound::Created(GameObject *obj)
{
	const char *preset = Get_Parameter("Preset");
	const char *bone = Get_Parameter("Bone");

	this->engineSoundId = Commands->Create_3D_Sound_At_Bone(preset, obj, bone);
}

void RMV_Engine_Sound::Destroyed(GameObject *obj)
{
	Commands->Stop_Sound(this->engineSoundId, true);
}

ScriptRegistrant<RMV_Engine_Sound> RMV_Engine_SoundRegistrant("RMV_Engine_Sound", "Preset:string, Bone:string");