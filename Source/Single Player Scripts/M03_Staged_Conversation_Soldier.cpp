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
#include "M03_Staged_Conversation_Soldier.h"

// This script is never used
void M03_Staged_Conversation_Soldier::Created(GameObject *obj)
{
	Commands->Start_Timer(obj, this, 0.5f, 0);
}

void M03_Staged_Conversation_Soldier::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	Commands->Set_Innate_Is_Stationary(obj, false);
	Commands->Innate_Soldier_Enable_Footsteps_Heard(obj, true);
	Commands->Innate_Soldier_Enable_Gunshot_Heard(obj, true);
	Commands->Innate_Soldier_Enable_Bullet_Heard(obj, true);
}

void M03_Staged_Conversation_Soldier::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	Commands->Set_Innate_Is_Stationary(obj, false);
	Commands->Innate_Soldier_Enable_Footsteps_Heard(obj, true);
	Commands->Innate_Soldier_Enable_Gunshot_Heard(obj, true);
	Commands->Innate_Soldier_Enable_Bullet_Heard(obj, true);
}

void M03_Staged_Conversation_Soldier::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	Commands->Set_Innate_Is_Stationary(obj, false);
	Commands->Innate_Soldier_Enable_Footsteps_Heard(obj, true);
	Commands->Innate_Soldier_Enable_Gunshot_Heard(obj, true);
	Commands->Innate_Soldier_Enable_Bullet_Heard(obj, true);
}

void M03_Staged_Conversation_Soldier::Timer_Expired(GameObject *obj, int number)
{
	Commands->Set_Innate_Is_Stationary(obj, true);
	Commands->Innate_Soldier_Enable_Footsteps_Heard(obj, false);
	Commands->Innate_Soldier_Enable_Gunshot_Heard(obj, false);
	Commands->Innate_Soldier_Enable_Bullet_Heard(obj, false);
}

ScriptRegistrant<M03_Staged_Conversation_Soldier> M03_Staged_Conversation_SoldierRegistrant("M03_Staged_Conversation_Soldier", "");