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
#include "MX0_KillNotify.h"

void MX0_KillNotify::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
}

void MX0_KillNotify::Created(GameObject *obj)
{
	this->field_1C = false;
}

void MX0_KillNotify::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 114)
	{
		Commands->Start_Timer(obj, this, 3.0f, 139);
		Commands->Start_Timer(obj, this, 7.0f, 140);
	}
}

void MX0_KillNotify::Timer_Expired(GameObject *obj, int number)
{
	if (number == 139)
	{
		int conversationId = Commands->Create_Conversation("MX0CON020", 95, 2000.0f, false); //NOD control,  I count 5 GDI targets.

		Commands->Join_Conversation(obj, conversationId, true, true, true);

		Commands->Start_Conversation(conversationId, 100020);
		Commands->Monitor_Conversation(obj, conversationId);
	}
	else if (number == 140)
	{
		int conversationId = Commands->Create_Conversation("MX0CON021", 99, 2000.0f, false); //Correction.  Make that 4.

		Commands->Join_Conversation(obj, conversationId, true, true, true);

		Commands->Start_Conversation(conversationId, 100021);
		Commands->Monitor_Conversation(obj, conversationId);

		Commands->Start_Timer(obj, this, 5.0f, 141);
	}
	else if (number == 141)
	{
		if (this->field_1C)
		{
			static const char * const CONVERSATIONS[] =
			{
				"Mx0_NODSNIPER_Alt01", //Tracking targets…
				"Mx0_NODSNIPER_Alt02", //Stay right there, infidel.
				"Mx0_NODSNIPER_Alt03", //Don’t move, infidel.
				"Mx0_NODSNIPER_Alt04", //Easy…
				"Mx0_NODSNIPER_Alt05", //Heh!
				"Mx0_NODSNIPER_Alt06" //Steady…
			};

			int randomInt = Commands->Get_Random_Int(0, 6);
			int conversationId = Commands->Create_Conversation(CONVERSATIONS[randomInt], 95, 2000.0f, false);

			Commands->Join_Conversation(obj, conversationId, true, true, true);

			Commands->Start_Conversation(conversationId, 100021);
			Commands->Monitor_Conversation(obj, conversationId);

			Commands->Start_Timer(obj, this, 5.0f, 141);
		}
	}
}

void MX0_KillNotify::Killed(GameObject *obj, GameObject *killer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (killer == Commands->Get_A_Star(pos))
	{
		GameObject *MX0MissionStartDMEObj = Commands->Find_Object(1200001);
		Commands->Send_Custom_Event(obj, MX0MissionStartDMEObj, 138, 0, 0.0f);
	}
}

ScriptRegistrant<MX0_KillNotify> MX0_KillNotifyRegistrant("MX0_KillNotify", "");