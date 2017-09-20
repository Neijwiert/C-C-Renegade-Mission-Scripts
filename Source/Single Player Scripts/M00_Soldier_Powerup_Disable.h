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

#pragma once

#include <scripts.h>

/*
M01 -> 103373 102051 101305 102360 102357 102358 102001 101998 101936
M05 -> 101358 100648 100170 101359 100158 100109 100100 100099 101363 101362 101361 101360 101357 100647 100264 100157 100057 100058 100064 100065 100108 100169
M06 -> 101105 101097 101098 101099 101153 101154
M07 -> 100672 100671
M08 -> 100649 100772 100771 100770 100171 100163 100162
M10 -> 2000800 2011893
M11 -> 101274 100394 101102 100387 100379 100345 100344 100343 100342 100432 100433 100434 100100 101402 101404 101770 101771
*/
class M00_Soldier_Powerup_Disable : public ScriptImpClass
{
	private:
		virtual void Created(GameObject *obj);
		virtual void Timer_Expired(GameObject *obj, int number);
};
