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
M01 -> 101929 101930 101931
*/
class M01_PrisonPen_Civilian_JDG : public ScriptImpClass
{
	public:
		virtual	void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);
		virtual void Sound_Heard(GameObject *obj, const CombatSound & sound);
		virtual void Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason);
		virtual void Timer_Expired(GameObject *obj, int number);
		virtual void Poked(GameObject *obj, GameObject *poker);

		Vector3 randMoveLocations[10];
		int detentionSoldier2ObjId;
		int objId;
		int prisonerCivillianMate1ObjId;
		int prisonerCivillianMate2ObjId;
		int movementActionIndex;
		float minMovementInterval;
		float maxMovementInterval;
		bool starCanSeeMe;
		bool heardStarShoot;
		bool isGateSwitchPokedAndSamAlive;
		bool isGateSwitchPokedAndSamDead;
};