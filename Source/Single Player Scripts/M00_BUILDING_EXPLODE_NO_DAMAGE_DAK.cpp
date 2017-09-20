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
#include "M00_BUILDING_EXPLODE_NO_DAMAGE_DAK.h"

/*
M01 -> 153909 153910 157976 157977
M02 -> 474463 467781 1153845 1153846 1157687 1157688 1157690
M03 -> 1150001 1150002 1150003
M04 -> 150001
M06 -> 163894
M07 -> 168926
M08 -> 162588
M10 -> 1153940 1153939 1153938 1153935 1153933 1153932 1153931 1154051 1154061 1154083 1154084
MTU -> 450938 450937 450936 450935
*/
void M00_BUILDING_EXPLODE_NO_DAMAGE_DAK::Killed(GameObject *obj, GameObject *killer)
{
	Vector3 pos = Commands->Get_Position(obj);
	Commands->Shake_Camera(pos, 25.0f, 0.1f, 4.0f);
}

ScriptRegistrant<M00_BUILDING_EXPLODE_NO_DAMAGE_DAK> M00_BUILDING_EXPLODE_NO_DAMAGE_DAKRegistrant("M00_BUILDING_EXPLODE_NO_DAMAGE_DAK", "");