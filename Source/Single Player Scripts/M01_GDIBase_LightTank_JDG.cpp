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
#include "M01_GDIBase_LightTank_JDG.h"

/*
M01 -> 102435
*/
void M01_GDIBase_LightTank_JDG::Killed(GameObject *obj, GameObject *killer)
{
	int conversationId = Commands->Create_Conversation("M01_TankReward_Conversation", 100, 1000.0f, false); // Nice work. I'm air-dropping supplies, look for them.
	Commands->Join_Conversation(NULL, conversationId, false, false, true);
	Commands->Start_Conversation(conversationId, conversationId);

	GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(4.2f, 279.8f, 3.2f));
	Commands->Attach_Script(invisObj, "Test_Cinematic", "X1D_GDIPowerUpDrop.txt");

	Vector3 pos = Commands->Get_Position(obj);
	pos.Z += 0.25f;

	Commands->Create_Object("POW_Medal_Armor", pos);
}

// TODO
void M01_GDIBase_LightTank_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	// Received from ourselves after 3 seconds after this block
	// Received from M01_mission_Controller_JDG when param 134 is received
	if (param == 16)
	{
		Vector3 pos = Commands->Get_Position(obj);
		if (Commands->Get_A_Star(pos))
		{
			float moveSpeed;

			int difficultyLevel = Commands->Get_Difficulty_Level();
			switch (difficultyLevel)
			{
				case 2:
					moveSpeed = 0.6f;

					break;
				case 1:
					moveSpeed = 0.4f;

					break;
				default:
					moveSpeed = 0.2f;

					break;
			}

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 39);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), moveSpeed, 2.0f);
			params.WaypathID = 102233;
			params.AttackCheckBlocked = false;
			params.AttackActive = false;

			Commands->Action_Attack(obj, params);
			
			Commands->Send_Custom_Event(obj, obj, 0, 29, 3.0f);
		}
		else
		{
			Commands->Send_Custom_Event(obj, obj, 0, 16, 3.0f);
		}
	}

	// Received from ourselves after 3 seconds after param 16
	else if (param == 29)
	{
		float moveSpeed;
		float attackRange;
		float attackError;

		int difficultyLevel = Commands->Get_Difficulty_Level();
		switch (difficultyLevel)
		{
			case 2:
				moveSpeed = 0.4f;
				attackRange = 100.0f;
				attackError = 0.0f;

				break;
			case 1:
				moveSpeed = 0.3f;
				attackRange = 75.0f;
				attackError = 1.0f;

				break;
			default:
				moveSpeed = 0.1f;
				attackRange = 50.0f;
				attackError = 3.0f;

				break;
		}

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 39);
		params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), moveSpeed, 2.0f);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		params.Set_Attack(starObj, attackRange, attackError, true);
		params.AttackActive = true;

		Commands->Modify_Action(obj, 39, params, true, true);
	}
}

ScriptRegistrant<M01_GDIBase_LightTank_JDG> M01_GDIBase_LightTank_JDGRegistrant("M01_GDIBase_LightTank_JDG", "");