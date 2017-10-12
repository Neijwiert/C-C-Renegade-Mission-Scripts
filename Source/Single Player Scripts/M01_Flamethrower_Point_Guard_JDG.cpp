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
#include "M01_Flamethrower_Point_Guard_JDG.h"

// This script is never used
void M01_Flamethrower_Point_Guard_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->homeLocation, sizeof(this->homeLocation), 1);
	Auto_Save_Variable(&this->facingFront, sizeof(this->facingFront), 2);
	Auto_Save_Variable(&this->facingLeft, sizeof(this->facingLeft), 3);
	Auto_Save_Variable(&this->facingRight, sizeof(this->facingRight), 4);
}

void M01_Flamethrower_Point_Guard_JDG::Created(GameObject *obj)
{
	this->homeLocation = Commands->Get_Position(obj);

	this->facingFront = Commands->Get_Facing(obj);
	this->facingLeft = this->facingFront - 45.0f;
	this->facingRight = this->facingFront + 45.0f;

	Commands->Set_Innate_Soldier_Home_Location(obj, this->homeLocation, 20.0f);
	Commands->Set_Innate_Aggressiveness(obj, 0.5f);
	Commands->Set_Innate_Take_Cover_Probability(obj, 0.5f);

	float randDelay = Commands->Get_Random(5.0f, 10.0f);
	Commands->Send_Custom_Event(obj, obj, 0, 67, randDelay);
}

void M01_Flamethrower_Point_Guard_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (damager == Commands->Get_A_Star(pos))
	{
		if (obj)
		{
			Commands->Set_Innate_Is_Stationary(obj, false);

			GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
			Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 189, 0.0f);
		}
	}
}

void M01_Flamethrower_Point_Guard_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received by ourselves after a random delay of 5 to 10 seconds in create or after this block
	if (param == 67)
	{
		float randFaceLocation = Commands->Get_Random(this->facingLeft, this->facingRight);
		float randFaceDuration = Commands->Get_Random(5.0f, 10.0f);

		float radiansFacing = DEG_TO_RADF(randFaceLocation);

		ActionParamsStruct params;
		params.Set_Basic(this, 45.0f, 20);
		params.Set_Face_Location(Vector3(this->homeLocation.X + WWMath::Cos(radiansFacing), this->homeLocation.Y + WWMath::Sin(radiansFacing), this->homeLocation.Z), randFaceDuration);

		Commands->Action_Face_Location(obj, params);

		Commands->Send_Custom_Event(obj, obj, 0, 67, randFaceDuration);
	}
}

void M01_Flamethrower_Point_Guard_JDG::Sound_Heard(GameObject *obj, const CombatSound & sound)
{
	if (sound.sound == SOUND_TYPE_GUNSHOT || sound.sound == SOUND_TYPE_BULLET_HIT)
	{
		Commands->Set_Innate_Is_Stationary(obj, false);
	}
}

void M01_Flamethrower_Point_Guard_JDG::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	Commands->Set_Innate_Is_Stationary(obj, false);
}

ScriptRegistrant<M01_Flamethrower_Point_Guard_JDG> M01_Flamethrower_Point_Guard_JDGRegistrant("M01_Flamethrower_Point_Guard_JDG", "");