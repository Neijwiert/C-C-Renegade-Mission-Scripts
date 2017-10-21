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
#include "M03_Sakura_Explosion.h"

/*
x3d_sakuracrash.txt at frame(s): 1
*/
void M03_Sakura_Explosion::Destroyed(GameObject *obj)
{
	Commands->Create_Explosion_At_Bone("Air Explosions Twiddler", obj, "ROTOR00", NULL);
	Commands->Create_Explosion_At_Bone("Air Explosions Twiddler", obj, "TURRET", NULL);
	Commands->Create_Explosion_At_Bone("Air Explosions Twiddler", obj, "ROTOR01", NULL);
}

ScriptRegistrant<M03_Sakura_Explosion> M03_Sakura_ExplosionRegistrant("M03_Sakura_Explosion", "");