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
M03 -> 1100003
*/
class M03_Gunboat_Controller_RMV : public ScriptImpClass
{
	public:
		virtual	void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Killed(GameObject *obj, GameObject *killer);
		virtual void Damaged(GameObject *obj, GameObject *damager, float amount);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);
		virtual void Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason);
		virtual void Timer_Expired(GameObject *obj, int number);

		int areaIndex;
		int field_20;
		Vector3 beachDestination;
		Vector3 villageStart;
		Vector3 villageDestination;
		Vector3 cannonStart;
		Vector3 cannonDestination;
		Vector3 beachAttackLocations[3];
		Vector3 villageAttackLocations[3];
		Vector3 cannonAttackLocations[3];
		float beachAttackError;
		float villageAttackError;
		float cannonAttackError;
		float attackLocationIndex;
		bool damagedByStar;
		bool downToHalfHealth;
		bool downToQuarterHealth;
		bool field_DF;
		bool field_E0;
		bool field_E1;
		float currentHealth;
		int field_E8;
		int damagedByBigGunCount;
};