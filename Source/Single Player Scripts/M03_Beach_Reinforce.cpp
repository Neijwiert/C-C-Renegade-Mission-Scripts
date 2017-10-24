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
#include "M03_Beach_Reinforce.h"

/*
M03 -> 2018061
*/
void M03_Beach_Reinforce::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->deathsLeftBeforeReinforce, sizeof(this->deathsLeftBeforeReinforce), 1);
	Auto_Save_Variable(&this->reinforceLocationIndex, sizeof(this->reinforceLocationIndex), 2);
	Auto_Save_Variable(&this->area0TrooperKilledCount, sizeof(this->area0TrooperKilledCount), 3);
	Auto_Save_Variable(&this->area1TrooperKilledCount, sizeof(this->area1TrooperKilledCount), 4);
	Auto_Save_Variable(&this->area2TrooperKilledCount, sizeof(this->area2TrooperKilledCount), 5);
	Auto_Save_Variable(&this->beachReinforcementsLeft, sizeof(this->beachReinforcementsLeft), 6);
	Auto_Save_Variable(&this->inletReinforcementsLeft, sizeof(this->inletReinforcementsLeft), 7);
	Auto_Save_Variable(&this->baseReinforcementsLeft, sizeof(this->baseReinforcementsLeft), 8);
	Auto_Save_Variable(&this->lastTrooperKilledArea, sizeof(this->lastTrooperKilledArea), 9);
}

void M03_Beach_Reinforce::Created(GameObject *obj)
{
	this->lastTrooperKilledArea = 0;
	this->deathsLeftBeforeReinforce = 2;
	this->reinforceLocationIndex = 0;
	this->area0TrooperKilledCount = 0;
	this->area1TrooperKilledCount = 0;
	this->area2TrooperKilledCount = 0;
	this->beachReinforcementsLeft = Commands->Get_Difficulty_Level() + 2;
	this->inletReinforcementsLeft = Commands->Get_Difficulty_Level() + 3;
	this->baseReinforcementsLeft = Commands->Get_Difficulty_Level() + 4;
}

void M03_Beach_Reinforce::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from M03_Killed_Sound after 0 seconds when killed. param = location
	if (type == 40021)
	{
		this->lastTrooperKilledArea = param;
	}

	// Received from M03_Reinforce_Area after 0 seconds when custom type 40017 is received
	else if (type == 40018)
	{
		if (param == 1)
		{
			Reinforce_Base();
		}
	}

	// Received from M03_Reinforce_Area after 0 seconds when custom type 40017 is received
	else if (type == 40020)
	{
		if (param == 1)
		{
			Reinforce_Beach();
		}
	}

	// Received from M03_Reinforce_Area after 0 seconds when custom type 40017 is received
	else if (type == 40019)
	{
		if (param == 1)
		{
			Reinforce_Inlet();
		}
	}

	// Received from M03_Killed_Sound after 0 seconds when killed. param = officer
	else if (type == 40008)
	{
		if (param == 1)
		{
			if (!this->lastTrooperKilledArea)
			{
				this->area0TrooperKilledCount++;
			}
			else if (this->lastTrooperKilledArea == 1)
			{
				this->area1TrooperKilledCount++;
			}
			else if (this->lastTrooperKilledArea == 2)
			{
				this->area2TrooperKilledCount++;
			}
		}

		if (--this->deathsLeftBeforeReinforce <= 0)
		{
			this->deathsLeftBeforeReinforce = 5 - Commands->Get_Difficulty_Level();

			GameObject *powerPlantNodChinookReinforcementsObj = Commands->Find_Object(1144444);
			Commands->Send_Custom_Event(obj, powerPlantNodChinookReinforcementsObj, 40017, 0, 0.0f);
		}
	}

	// Received from M03_Past_Pillbox after 0 seconds when entered
	else if (type == 40009)
	{
		this->reinforceLocationIndex = 10;
	}
}

void M03_Beach_Reinforce::Reinforce_Beach()
{
	if (this->area0TrooperKilledCount <= 2 && this->beachReinforcementsLeft > 0)
	{
		this->beachReinforcementsLeft--;

		if (!this->reinforceLocationIndex )
		{
			this->reinforceLocationIndex++;

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-95.656f, -68.236f, 1.433f));
			Commands->Set_Facing(invisObj, -180.0f);
			Commands->Attach_Script(invisObj, "M03_Chinook_ParaDrop", "M03_Paratrooper");
		}
		else if (this->reinforceLocationIndex == 1)
		{
			this->reinforceLocationIndex = 0;

			Commands->Trigger_Spawner(2018881);
			Commands->Trigger_Spawner(2018880);
		}
		else if (this->reinforceLocationIndex > 1)
		{
			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-95.656f, -68.236f, 1.433f));
			Commands->Set_Facing(invisObj, -180.0f);
			Commands->Attach_Script(invisObj, "M03_Chinook_ParaDrop", "M03_Paratrooper");
		}
	}
}

void M03_Beach_Reinforce::Reinforce_Inlet()
{
	if (this->area1TrooperKilledCount <= 2 && this->inletReinforcementsLeft > 0)
	{
		this->inletReinforcementsLeft--;

		if (this->reinforceLocationIndex <= 0)
		{
			this->reinforceLocationIndex = 1;

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(51.0f, -91.0f, 2.0f));
			Commands->Set_Facing(invisObj, 75.0f);
			Commands->Attach_Script(invisObj, "M03_Chinook_ParaDrop", "M03_Paratrooper2");
		}
		else
		{
			this->reinforceLocationIndex--;

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(99.85f, -49.51f, 2.0f));
			Commands->Set_Facing(invisObj, 170.0f);
			Commands->Attach_Script(invisObj, "M03_Chinook_ParaDrop", "M03_Paratrooper2");
		}
	}
}

void M03_Beach_Reinforce::Reinforce_Base()
{
	if (this->area2TrooperKilledCount <= 2 && this->baseReinforcementsLeft > 0)
	{
		this->baseReinforcementsLeft--;

		if (this->reinforceLocationIndex > 0)
		{
			this->reinforceLocationIndex = 0;

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-79.0f, 59.0f, 9.5f));
			Commands->Set_Facing(invisObj, -120.0f);
			Commands->Attach_Script(invisObj, "M03_Chinook_ParaDrop", "M03_Paratrooper");
		}
		else
		{
			this->reinforceLocationIndex++;

			GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(-138.0f, 50.0f, 9.5f));
			Commands->Set_Facing(invisObj, -30.0f);
			Commands->Attach_Script(invisObj, "M03_Chinook_ParaDrop", "M03_Paratrooper");
		}
	}
}

ScriptRegistrant<M03_Beach_Reinforce> M03_Beach_ReinforceRegistrant("M03_Beach_Reinforce", "");