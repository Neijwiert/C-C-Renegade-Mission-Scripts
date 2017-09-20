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
M02 -> 401195 401194 401193 401192 401191 401190 401189 401122 401121 401120 401119 401118 401117 401116 401115 401097 401096 401095 401094 401089 401088 401087 401086 401085 401069 401068 401067 401053 401052 401051 401050 401049 401047 401046 400510 400505 400503 400453 400451 400415 400413 400411 400410 400345 400344 400319 400318 400317 400293 400292 400287 400286 400285 400213 400212 400211 400210 400209 400208 400207 400199 400200 400206 400275 400276 400277 400372 400373 400374 400553 400554 400555 400556 400557 400558 400559 400560 400977 400978 400979 400980 400981 400982 400983 400984 401002 401003 401045 401056 401057 401058 401060 401063 401064 401065 401071 401072 401073 401074 401075 401076 401077 401078 401103 401104 401105 401106 401107 401108 401109 401110 401112 401124 401125 401126 401127 401129 401132 401133 401134 401135 401136 401137 401138 401139 401141 401142 401143 401144 401145
The following cineamtic files: x2i_para03_area00/01/02/03/04/06/09/21.txt, x2i_drop03_area00/01/02/03/04/06/09/10/11/21.txt, x2i_drop02_area00/01/02/03/04/06/09/10/11/21.txt
When M02_Respawn_Controller::Use_Spawners is called
*/
class M02_Nod_Soldier : public ScriptImpClass
{
	public:
		virtual	void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Killed(GameObject *obj, GameObject *killer);
		virtual void Damaged(GameObject *obj, GameObject *damager, float amount);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);
		virtual void Sound_Heard(GameObject *obj, const CombatSound & sound);
		virtual void Enemy_Seen(GameObject *obj, GameObject *enemy);
		virtual void Timer_Expired(GameObject *obj, int number);

		Vector3 gotoLocation; // Never assigned
		bool delayedInitialized;
		int animationIndex;
		bool damaged;
		float health;
		bool enemySeen;
};