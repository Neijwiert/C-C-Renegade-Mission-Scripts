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
M01 -> 116383
*/
class M01_Base_GDI_Grenadier_JDG : public ScriptImpClass
{
	public:
		virtual	void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Damaged(GameObject *obj, GameObject *damager, float amount);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);
		virtual void Enemy_Seen(GameObject *obj, GameObject *enemy);
		virtual void Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason);
		virtual void Animation_Complete(GameObject *obj, const char *animation_name);

		float health;
		bool preventDeath;
		bool field_21;
		bool canSeeEnemies;
		bool field_23;
		bool attackedByNodMinigunners;
};