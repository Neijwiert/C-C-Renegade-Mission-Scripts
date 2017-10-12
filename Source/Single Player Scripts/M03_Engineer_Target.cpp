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
#include "M03_Engineer_Target.h"

/*
M03 -> 1145881
M06 -> 100157 100330 100423 100445 101003 101004 101005 101006
M07 -> 100788 100780 100722 100721 100796 100798 100779
*/
void M03_Engineer_Target::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_20, sizeof(this->field_20), 1);
	Auto_Save_Variable(&this->initialHealth, sizeof(this->initialHealth), 2);
}

void M03_Engineer_Target::Created(GameObject *obj)
{
	this->initialHealth = Commands->Get_Health(obj);
}

void M03_Engineer_Target::Destroyed(GameObject *obj)
{
	Vector3 pos = Commands->Get_Position(obj);
	Commands->Create_Logical_Sound(obj, 40014, pos, 150.0f);
}

void M03_Engineer_Target::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	Vector3 pos = Commands->Get_Position(obj);
	Commands->Create_Logical_Sound(obj, 40012, pos, 150.0f);

	Commands->Start_Timer(obj, this, 3.0f, 40013);
}

// TODO
void M03_Engineer_Target::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 40011)
	{
		this->field_20 = param;
	}
}

// TODO
void M03_Engineer_Target::Timer_Expired(GameObject *obj, int number)
{
	if (number == 40013)
	{
		float health = Commands->Get_Health(obj);
		Vector3 pos = Commands->Get_Position(obj);

		if(health == this->initialHealth)
		{
			Commands->Create_Logical_Sound(obj, 40014, pos, 150.0f);
		}
		else
		{
			Commands->Create_Logical_Sound(obj, 40012, pos, 150.0f);

			Commands->Start_Timer(obj, this, 3.0f, 40013);
		}
	}
}

ScriptRegistrant<M03_Engineer_Target> M03_Engineer_TargetRegistrant("M03_Engineer_Target", "");