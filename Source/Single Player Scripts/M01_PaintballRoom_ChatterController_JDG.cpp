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
#include "M01_PaintballRoom_ChatterController_JDG.h"

/*
M01 -> 107554
*/
void M01_PaintballRoom_ChatterController_JDG::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->honPaintballTeam02OfficerObjId, sizeof(this->honPaintballTeam02OfficerObjId), 1);
	Auto_Save_Variable(&this->honPaintballTeam01OfficerObjId, sizeof(this->honPaintballTeam01OfficerObjId), 2);
	Auto_Save_Variable(&this->honPaintballTeam01Guy02ObjId, sizeof(this->honPaintballTeam01Guy02ObjId), 3);
	Auto_Save_Variable(&this->honPaintballTeam01Guy01ObjId, sizeof(this->honPaintballTeam01Guy01ObjId), 4);
	Auto_Save_Variable(&this->honPaintballTeam02Guy01ObjId, sizeof(this->honPaintballTeam02Guy01ObjId), 5);
	Auto_Save_Variable(&this->honPaintballTeam02Guy02ObjId, sizeof(this->honPaintballTeam02Guy02ObjId), 6);
	Auto_Save_Variable(&this->team2OfficerKilled, sizeof(this->team2OfficerKilled), 7);
	Auto_Save_Variable(&this->team1OfficerKilled, sizeof(this->team1OfficerKilled), 8);
	Auto_Save_Variable(&this->team1Guy2Killed, sizeof(this->team1Guy2Killed), 9);
	Auto_Save_Variable(&this->team1Guy1Killed, sizeof(this->team1Guy1Killed), 10);
	Auto_Save_Variable(&this->team2Guy1Killed, sizeof(this->team2Guy1Killed), 11);
	Auto_Save_Variable(&this->team2Guy2Killed, sizeof(this->team2Guy2Killed), 12);
	Auto_Save_Variable(&this->anyPaintballerDamagedByStar, sizeof(this->anyPaintballerDamagedByStar), 13);
}

void M01_PaintballRoom_ChatterController_JDG::Created(GameObject *obj)
{
	this->honPaintballTeam02OfficerObjId = 102001;
	this->honPaintballTeam01OfficerObjId = 101998;
	this->honPaintballTeam01Guy02ObjId = 102000;
	this->honPaintballTeam01Guy01ObjId = 101999;
	this->honPaintballTeam02Guy01ObjId = 102002;
	this->honPaintballTeam02Guy02ObjId = 102003;
	this->team2OfficerKilled = false;
	this->team1OfficerKilled = false;
	this->team1Guy2Killed = false;
	this->team1Guy1Killed = false;
	this->team2Guy1Killed = false;
	this->team2Guy2Killed = false;
	this->anyPaintballerDamagedByStar = false;
}

void M01_PaintballRoom_ChatterController_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (!type)
	{
		// Received from M01_HON_Paintball_Team_01_JDG or M01_HON_Paintball_Team_02_JDG when damaged by star
		if (param == 16)
		{
			if (!this->anyPaintballerDamagedByStar)
			{
				this->anyPaintballerDamagedByStar = true;

				if (this->team2OfficerKilled)
				{
					if (!this->team1OfficerKilled)
					{
						float randDelay = Commands->Get_Random(1.0f, 2.0f);
						Commands->Send_Custom_Event(obj, obj, 0, 28, randDelay);
					}
				}
				else
				{
					float randDelay = Commands->Get_Random(1.0f, 2.0f);
					Commands->Send_Custom_Event(obj, obj, 0, 27, randDelay);
				}

				if (this->team1Guy2Killed)
				{
					if (!this->team1Guy1Killed)
					{
						float randDelay = Commands->Get_Random(1.0f, 2.0f);
						Commands->Send_Custom_Event(obj, obj, 0, 30, randDelay);
					}
				}
				else
				{
					float randDelay = Commands->Get_Random(1.0f, 2.0f);
					Commands->Send_Custom_Event(obj, obj, 0, 29, randDelay);
				}

				if (this->team2Guy1Killed)
				{
					if (!this->team2Guy2Killed)
					{
						float randDelay = Commands->Get_Random(1.0f, 2.0f);
						Commands->Send_Custom_Event(obj, obj, 0, 32, randDelay);
					}
				}
				else
				{
					float randDelay = Commands->Get_Random(1.0f, 2.0f);
					Commands->Send_Custom_Event(obj, obj, 0, 31, randDelay);
				}
			}
		}

		// Received from M01_HON_Paintball_Team_01_JDG or M01_HON_Paintball_Team_02_JDG when killed
		else if (param == 22)
		{
			GameObject *honPaintballTeam02OfficerObjId = Commands->Find_Object(this->honPaintballTeam02OfficerObjId);
			GameObject *honPaintballTeam01OfficerObjId = Commands->Find_Object(this->honPaintballTeam01OfficerObjId);
			GameObject *honPaintballTeam01Guy02ObjId = Commands->Find_Object(this->honPaintballTeam01Guy02ObjId);
			GameObject *honPaintballTeam01Guy01ObjId = Commands->Find_Object(this->honPaintballTeam01Guy01ObjId);
			GameObject *honPaintballTeam02Guy01ObjId = Commands->Find_Object(this->honPaintballTeam02Guy01ObjId);
			GameObject *honPaintballTeam02Guy02ObjId = Commands->Find_Object(this->honPaintballTeam02Guy02ObjId);

			if (sender == honPaintballTeam02OfficerObjId)
			{
				this->team2OfficerKilled = true;
			}
			else if (sender == honPaintballTeam01OfficerObjId)
			{
				this->team1OfficerKilled = true;
			}
			else if (sender == honPaintballTeam01Guy02ObjId)
			{
				this->team1Guy2Killed = true;
			}
			else if (sender == honPaintballTeam01Guy01ObjId)
			{
				this->team1Guy1Killed = true;
			}
			else if (sender == honPaintballTeam02Guy01ObjId)
			{
				this->team2Guy1Killed = true;
			}
			else if (sender == honPaintballTeam02Guy02ObjId)
			{
				this->team2Guy2Killed = true;
			}
		}

		// Received from ourselves after 1 to 2 seconds when param 16 is received
		else if (param == 27)
		{
			GameObject *honPaintballTeam02OfficerObj = Commands->Find_Object(this->honPaintballTeam02OfficerObjId);
			if (honPaintballTeam02OfficerObj)
			{
				int conversationId = Commands->Create_Conversation("M01_Paintball_Intruder_Conversation_02", 95, 20.0f, true); // Cease fire! Cease fire!
				Commands->Join_Conversation(honPaintballTeam02OfficerObj, conversationId, true, true, true);
				Commands->Start_Conversation(conversationId, conversationId);
			}
		}

		// Received from ourselves after 1 to 2 seconds when param 16 is received
		else if (param == 28)
		{
			GameObject *honPaintballTeam01OfficerObj = Commands->Find_Object(this->honPaintballTeam01OfficerObjId);
			if (honPaintballTeam01OfficerObj)
			{
				int conversationId = Commands->Create_Conversation("M01_Paintball_Intruder_Conversation_02", 95, 20.0f, true); // Cease fire! Cease fire!
				Commands->Join_Conversation(honPaintballTeam01OfficerObj, conversationId, true, true, true);
				Commands->Start_Conversation(conversationId, conversationId);
			}
		}

		// Received from ourselves after 1 to 2 seconds when param 16 is received
		else if (param == 29)
		{
			GameObject *honPaintballTeam01Guy02Obj = Commands->Find_Object(this->honPaintballTeam01Guy02ObjId);
			if (honPaintballTeam01Guy02Obj)
			{
				int conversationId = Commands->Create_Conversation("M01_Paintball_Intruder_Conversation_01", 95, 20.0f, true); // Someone's using live ammo! Check your weapon settings!
				Commands->Join_Conversation(honPaintballTeam01Guy02Obj, conversationId, true, true, true);
				Commands->Start_Conversation(conversationId, conversationId);
			}
		}

		// Received from ourselves after 1 to 2 seconds when param 16 is received
		else if (param == 30)
		{
			GameObject *honPaintballTeam01Guy01Obj = Commands->Find_Object(this->honPaintballTeam01Guy01ObjId);
			if (honPaintballTeam01Guy01Obj)
			{
				int conversationId = Commands->Create_Conversation("M01_Paintball_Intruder_Conversation_01", 95, 20.0f, true); // Someone's using live ammo! Check your weapon settings!
				Commands->Join_Conversation(honPaintballTeam01Guy01Obj, conversationId, true, true, true);
				Commands->Start_Conversation(conversationId, conversationId);
			}
		}

		// Received from ourselves after 1 to 2 seconds when param 16 is received
		else if (param == 31)
		{
			GameObject *honPaintballTeam02Guy01Obj = Commands->Find_Object(this->honPaintballTeam02Guy01ObjId);
			if (honPaintballTeam02Guy01Obj)
			{
				int conversationId = Commands->Create_Conversation("M01_Paintball_Intruder_Conversation_03", 95, 20.0f, true); // Fox in the hole - get him!
				Commands->Join_Conversation(honPaintballTeam02Guy01Obj, conversationId, true, true, true);
				Commands->Start_Conversation(conversationId, conversationId);
			}
		}

		// Received from ourselves after 1 to 2 seconds when param 16 is received
		else if(param == 32)
		{
			GameObject *honPaintballTeam02Guy02Obj = Commands->Find_Object(this->honPaintballTeam02Guy02ObjId);
			if (honPaintballTeam02Guy02Obj)
			{
				int conversationId = Commands->Create_Conversation("M01_Paintball_Intruder_Conversation_03", 95, 20.0f, true); // Fox in the hole - get him!
				Commands->Join_Conversation(honPaintballTeam02Guy02Obj, conversationId, true, true, true);
				Commands->Start_Conversation(conversationId, conversationId);
			}
		}
	}
}

ScriptRegistrant<M01_PaintballRoom_ChatterController_JDG> M01_PaintballRoom_ChatterController_JDGRegistrant("M01_PaintballRoom_ChatterController_JDG", "");