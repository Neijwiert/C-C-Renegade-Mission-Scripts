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
M03 -> 1202444 1202445
M06 -> 101430 101431 101432 101433 101434 101435 101436 101437 101439 101440 101441 101442 101443 101444 101445 101446
M07 -> 123261 123260
M09 -> 1101566 1101565 1101564 1101563 2005502 2005503 1101299 1101827 1202590 1202723
M10 -> 1111350 1111349 1111348 1111347 1111346 1111345 1111344 1111307 1100262 1100261 1100260 1100251 1100249 1100248 1100247 1100246 1100230 1100229 1110061 1100031 1100033 1100034 1100035 1100036 1100037 1100038 1110060
*/
class RMV_Camera_Behavior : public ScriptImpClass
{
	public:
		virtual	void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Killed(GameObject *obj, GameObject *killer);
		virtual void Enemy_Seen(GameObject *obj, GameObject *enemy);
		virtual void Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason);
		virtual void Timer_Expired(GameObject *obj, int number);

		Vector3 Get_Target();

		int angleIndex;
		int enemyId;
		int beepSoundId;
		bool lookingAtEnemy;
		bool delayLookAtEnemyPassed;
		bool stillSeeingEnemyAfterDelay;
		bool isGun;
		bool attackingEnemy;
};