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
#include "M01_HON_Paintball_Team_01_JDG.h"

/*
M01 -> 101999 102000 101998
*/
void M01_HON_Paintball_Team_01_JDG::Register_Auto_Save_Variables()
{
	
}

void M01_HON_Paintball_Team_01_JDG::Created(GameObject *obj)
{
	this->honPaintballTeam02OfficerObjId = 102001;
	this->actingOnPaintballer = false;
	this->awareOfStar = false;

	Commands->Set_Player_Type(obj, SCRIPT_PLAYERTYPE_RENEGADE);
	Commands->Set_Obj_Radar_Blip_Color(obj, RADAR_BLIP_COLOR_NOD);

	if (Commands->Get_ID(obj) != 101998) // HON Paintball team 01 Officer
	{
		this->homeLocation = Commands->Get_Position(obj);
		this->minChangeInnateInterval = 5.0f;
		this->maxChangeInnateInterval = 10.0f;
		this->innateAggressiveness = Commands->Get_Random(0.0f, 0.5f);
		this->innateTakeCoverProbability = Commands->Get_Random(0.5f, 1.0f);
		this->homeRadius = Commands->Get_Random(0.0f, 20.0f);

		Commands->Set_Innate_Aggressiveness(obj, this->innateAggressiveness);
		Commands->Set_Innate_Take_Cover_Probability(obj, this->innateTakeCoverProbability);
		Commands->Set_Innate_Soldier_Home_Location(obj, this->homeLocation, this->homeRadius);

		float randDuration = Commands->Get_Random(this->minChangeInnateInterval, this->maxChangeInnateInterval);
		Commands->Start_Timer(obj, this, randDuration, 27);
	}
}

void M01_HON_Paintball_Team_01_JDG::Killed(GameObject *obj, GameObject *killer)
{
	GameObject *M01PaintballRoomChatterControllerJDGObj = Commands->Find_Object(107554);
	if (M01PaintballRoomChatterControllerJDGObj)
	{
		Commands->Send_Custom_Event(obj, M01PaintballRoomChatterControllerJDGObj, 0, 22, 0.0f);
	}
}

void M01_HON_Paintball_Team_01_JDG::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (damager == Commands->Get_A_Star(pos))
	{
		this->awareOfStar = true;

		GameObject *M01PaintballRoomChatterControllerJDGObj = Commands->Find_Object(107554);
		if (M01PaintballRoomChatterControllerJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01PaintballRoomChatterControllerJDGObj, 0, 16, 0.0f);
		}
	}
	else
	{
		if (damager == Commands->Find_Object(this->honPaintballTeam02OfficerObjId))
		{
			this->actingOnPaintballer = true;

			Commands->Set_Innate_Aggressiveness(obj, 0.2f);
			Commands->Set_Innate_Take_Cover_Probability(obj, 1.0f);

			float randDuration = Commands->Get_Random(0.0f, 2.0f);
			Commands->Start_Timer(obj, this, randDuration, 28);
		}

		if (!this->awareOfStar)
		{
			float maxHealth = Commands->Get_Max_Health(obj);
			Commands->Set_Health(obj, maxHealth);
		}
	}
}

void M01_HON_Paintball_Team_01_JDG::Timer_Expired(GameObject *obj, int number)
{
	if (number == 27)
	{
		if (!this->actingOnPaintballer)
		{
			this->innateAggressiveness = Commands->Get_Random(0.0f, 0.5f);
			this->innateTakeCoverProbability = Commands->Get_Random(0.5f, 1.0f);
			this->homeRadius = Commands->Get_Random(0.0f, 20.0f);

			Commands->Set_Innate_Aggressiveness(obj, this->innateAggressiveness);
			Commands->Set_Innate_Take_Cover_Probability(obj, this->innateTakeCoverProbability);
			Commands->Set_Innate_Soldier_Home_Location(obj, this->homeLocation, this->homeRadius);

			float randDuration = Commands->Get_Random(this->minChangeInnateInterval, this->maxChangeInnateInterval);
			Commands->Start_Timer(obj, this, randDuration, 27);
		}
	}
	else if (number == 28)
	{
		this->actingOnPaintballer = false;

		float randDuration = Commands->Get_Random(this->minChangeInnateInterval, this->maxChangeInnateInterval);
		Commands->Start_Timer(obj, this, randDuration, 27);
	}
}

ScriptRegistrant<M01_HON_Paintball_Team_01_JDG> M01_HON_Paintball_Team_01_JDGRegistrant("M01_HON_Paintball_Team_01_JDG", "");