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
#include "M01_HON_Easy_Spawned_Guy_01_JDG.h"

void M01_HON_Easy_Spawned_Guy_01_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->movingToStar, sizeof(this->movingToStar), 1);
	Auto_Save_Variable(&this->noticedStar, sizeof(this->noticedStar), 2);
}

// When M01_Mission_Controller_JDG receives custom type 0 with param 205
void M01_HON_Easy_Spawned_Guy_01_JDG::Created(GameObject *obj)
{
	this->movingToStar = false;
	this->noticedStar = false;

	Commands->Enable_Hibernation(obj, false);

	ActionParamsStruct params;
	params.Set_Basic(this, 85.0f, 38);
	params.Set_Movement(Vector3(-218.772f, 510.932f, 4.017f), 0.8f, 0.05f);

	Commands->Action_Goto(obj, params);
}

void M01_HON_Easy_Spawned_Guy_01_JDG::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *M01MissionControllerObj = Commands->Find_Object(100376);

	if (!this->movingToStar)
	{
		Commands->Send_Custom_Event(obj, M01MissionControllerObj, 0, 208, 0.0f);
	}

	Commands->Send_Custom_Event(obj, M01MissionControllerObj, 0, 207, 0.0f);
}

void M01_HON_Easy_Spawned_Guy_01_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *starObj = Commands->Get_A_Star(pos);

	if (damager && damager == starObj)
	{
		GameObject *M01MissionControllerObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerObj, 0, 189, 0.0f);

		if (!this->noticedStar)
		{
			Commands->Action_Reset(obj, 100.0f);

			Commands->Innate_Force_State_Enemy_Seen(obj, starObj);

			this->noticedStar = true;
		}
	}
}

// TODO
void M01_HON_Easy_Spawned_Guy_01_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (param == 65)
	{
		if (obj)
		{
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			if (starObj)
			{
				this->movingToStar = true;

				ActionParamsStruct params;
				params.Set_Basic(this, 45.0f, 65);

				Vector3 starPos = Commands->Get_Position(starObj);

				params.Set_Movement(starPos, 0.8f, 5.0f);

				Commands->Action_Goto(obj, params);
			}
		}

		Commands->Send_Custom_Event(obj, obj, 0, 65, 30.0f);
	}
}

// TODO
void M01_HON_Easy_Spawned_Guy_01_JDG::Sound_Heard(GameObject *obj, const CombatSound & sound)
{
	Vector3 pos = Commands->Get_Position(obj);
	GameObject *starObj = Commands->Get_A_Star(pos);

	if (sound.Creator == starObj && !this->noticedStar)
	{
		Vector3 starPos = Commands->Get_Position(starObj);

		if (sound.sound == SOUND_TYPE_GUNSHOT)
		{
			this->noticedStar = true;

			Commands->Action_Reset(obj, 100.0f);
			Commands->Innate_Force_State_Gunshots_Heard(obj, starPos);
		}
		else if (sound.sound == SOUND_TYPE_BULLET_HIT)
		{
			this->noticedStar = true;

			Commands->Action_Reset(obj, 100.0f);
			Commands->Innate_Force_State_Bullet_Heard(obj, starPos);
		}
	}
}

void M01_HON_Easy_Spawned_Guy_01_JDG::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enemy == Commands->Get_A_Star(pos) && !this->noticedStar)
	{
		this->noticedStar = true;

		Commands->Action_Reset(obj, 100.0f);
		Commands->Set_Innate_Aggressiveness(obj, 0.25f);
		Commands->Set_Innate_Take_Cover_Probability(obj, 0.75f);
		Commands->Enable_Hibernation(obj, true);
	}
}

void M01_HON_Easy_Spawned_Guy_01_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason) 
{
	// Triggered when we failed to move to the star, see custom
	if (action_id == 65 && (complete_reason == ACTION_COMPLETE_PATH_BAD_START || complete_reason == ACTION_COMPLETE_PATH_BAD_DEST || complete_reason == ACTION_COMPLETE_MOVE_NO_PROGRESS_MADE))
	{
		Commands->Apply_Damage(obj, 10000.0f, "BlamoKiller", NULL);
	}
}

ScriptRegistrant<M01_HON_Easy_Spawned_Guy_01_JDG> M01_HON_Easy_Spawned_Guy_01_JDGRegistrant("M01_HON_Easy_Spawned_Guy_01_JDG", "");