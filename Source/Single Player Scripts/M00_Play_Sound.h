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
M01 -> 300299 300298 300301 300300 300302 300296 300295 300305 300304 300303 300306 300297
M02 -> 100555
M04 -> 300504 300503 300506 300505 300502 300507
M05 -> 500092 500091 500089 500090 500088 500096 500097 500094 500086 500087 500095 500093 109455 109456 109457 109458 109459 109460 109461 109462 109463 109464 109465 109466
M06 -> 500025 112955 112971
M08 -> 100495 100492 100489 100488 100486 100485 100484 100482 100481 100480 100479 100478 100475 100474 100470 100467 100465 100464 100458 100456 100455 100454 100453 100449 100448 100447 100382 100357 100355 100354 100398
M10 -> 7000015 7000014 7000013 7000012 7000011 7000010 7000009 7000005 7000004 7000003 7000002 7000001 7000007 5000012 5000006 5000007 5000008 5000009 5000010 5000013 5000014 5000015 5000016 5000017 5000018 5000019
M13 -> 100017 100016 100012 100034 100037 100044
*/
class M00_Play_Sound : public ScriptImpClass
{
	private:
		virtual void Created(GameObject *obj);
		virtual void Timer_Expired(GameObject *obj, int number);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);
};
