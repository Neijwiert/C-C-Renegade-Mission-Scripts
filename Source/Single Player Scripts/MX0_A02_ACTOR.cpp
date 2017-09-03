/*custom
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
#include "MX0_A02_ACTOR.h"

void MX0_A02_ACTOR::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->health, sizeof(this->health), 2);
	Auto_Save_Variable(&this->field_28, sizeof(this->field_28), 3);
	Auto_Save_Variable(&this->field_1D, sizeof(this->field_1D), 4);
	Auto_Save_Variable(&this->field_2C, sizeof(this->field_2C), 5);
	Auto_Save_Variable(&this->shield, sizeof(this->shield), 6);
	Auto_Save_Variable(&this->field_30, sizeof(this->field_30), 7);
	Auto_Save_Variable(&this->field_34, sizeof(this->field_34), 8);
	Auto_Save_Variable(&this->field_1E, sizeof(this->field_1E), 9);
	Auto_Save_Variable(&this->field_1F, sizeof(this->field_1F), 10);
	Auto_Save_Variable(&this->field_38, sizeof(this->field_38), 11);
}

void MX0_A02_ACTOR::Created(GameObject *obj)
{
	Commands->Attach_Script(obj, "M00_Soldier_Powerup_Disable", "");

	this->field_1E = true;
	this->health = Commands->Get_Health(obj);
	this->shield = Commands->Get_Shield_Strength(obj);

	Commands->Innate_Disable(obj);

	this->field_28 = 0;
	this->field_2C = 0;
	this->field_1C = false;
	this->field_1D = false;
	this->field_1F = false;
	this->field_30 = 0;
	this->field_34 = 0;
	this->field_38 = 0;

	Commands->Enable_Enemy_Seen(obj, false);

	int actorId = Get_Int_Parameter("ActorID");
	if (actorId)
	{
		if (actorId == 1)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 209);
			params.Set_Movement(Vector3(-79.172f, -72.536f, 6.0f), 0.8f, 0.5f);

			Commands->Action_Goto(obj, params);
			Commands->Enable_Enemy_Seen(obj, true);
		}
		else if (actorId == 2)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 211);
			params.Set_Movement(Vector3(-97.415f, -68.822f, 3.944f), 0.8f, 0.5f);

			Commands->Action_Goto(obj, params);
			Commands->Enable_Enemy_Seen(obj, true);
		}
		else if (actorId == 3)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 213);
			params.Set_Movement(Vector3(-79.172f, -72.536f, 6.0f), 0.8f, 0.5f);

			Commands->Action_Goto(obj, params);
		}
		else if (actorId == 4)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 216);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.8f, 0.5f);
			params.WaypathID = 1100029;

			Commands->Action_Goto(obj, params);

			this->field_1C = true;

			Commands->Start_Timer(obj, this, 20.0f, 214);
		}
		else if (actorId == 5)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 217);

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			params.Set_Attack(starObj, 300.0f, 0.0f, true);
			params.Set_Movement(Vector3(0.0f, 0.0f, 0.0f), 0.8f, 0.5f);
			params.WaypathID = 1100034;
			params.AttackCheckBlocked = true;

			Commands->Action_Attack(obj, params);

			this->field_1C = true;

			Commands->Start_Timer(obj, this, 10.0f, 210);

			Commands->Trigger_Weapon(obj, true, Vector3(-90.941f, -60.47f, 0.068999998f), true);

			GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
			if (!MX0A02ControllerObj)
			{
				return;
			}

			Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 244, 0, 0.0f);
		}
		else if (actorId == 6)
		{
			this->field_1C = true;

			Commands->Start_Timer(obj, this, 2.0f, 210);
		}
		else if (actorId == 7)
		{
			Commands->Start_Timer(obj, this, 4.0f, 216);
		}
		else if (actorId == 8)
		{
			Commands->Start_Timer(obj, this, 4.0f, 217);
		}
	}
	else
	{
		Commands->Send_Custom_Event(obj, obj, 207, 0, 0.0f);
	}
}

void MX0_A02_ACTOR::Killed(GameObject *obj, GameObject *killer)
{
	if (this->field_1E)
	{
		GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
		if (MX0A02ControllerObj)
		{
			Vector3 pos = Commands->Get_Position(obj);
			if (killer == Commands->Get_A_Star(pos))
			{
				Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 227, 0, 0.0f);
			}
			else
			{
				if (killer)
				{
					Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 228, Commands->Get_ID(killer), 0.0f);
				}
			}

			if (this->field_2C > 4)
			{
				Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 232, this->field_2C, 0.1f);
			}
		}
	}
}

void MX0_A02_ACTOR::Damaged(GameObject *obj, GameObject *damager, float amount)
{
	if (this->field_1E)
	{
		if (this->field_1C)
		{
			this->shield = Commands->Get_Shield_Strength(obj);
		}
		else
		{
			if (this->field_1D)
			{
				this->shield = this->shield - 1.0f;
			}

			Commands->Set_Shield_Strength(obj, this->shield);
			Commands->Set_Health(obj, this->health);
		}

		if (this->field_1D)
		{
			if (damager)
			{
				Vector3 pos = Commands->Get_Position(obj);
				if (this->field_2C > 4 || damager != Commands->Get_A_Star(pos) && Commands->Get_Player_Type(obj) != Commands->Get_Player_Type(damager))
				{
					ActionParamsStruct params;
					params.Set_Basic(this, 90.0f, 201);
					params.Set_Attack(damager, 300.0f, 0.0f, true);

					if (this->field_2C > 4)
					{
						params.AttackCheckBlocked = false;
					}

					Commands->Action_Attack(obj, params);
				}
			}
		}
		else if (!this->field_1F && this->field_1C)
		{
			this->field_1F = true;

			int actorId = Get_Int_Parameter("ActorID");
			if (actorId == 1)
			{
				Commands->Debug_Message("XXXX INSERT FALLING DEATH ANIMATION HERE.\n");
				Commands->Apply_Damage(obj, 10000.0f, "Blamokiller", obj);

				GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
				if (MX0A02ControllerObj)
				{
					Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 236, 0, 0.0f);
				}
			}
			else if (actorId == 2)
			{
				Commands->Debug_Message("XXXX INSERT FALLING DEATH ANIMATION HERE.\n");
				Commands->Apply_Damage(obj, 10000.0f, "Blamokiller", obj);
			}
		}
	}
}

void MX0_A02_ACTOR::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (this->field_1E)
	{
		if (type == 203)
		{
			this->field_1E = false;
			this->field_1D = false;
		}
		else if (type == 204)
		{
			this->field_2C = param;
		}
		else if (type == 205)
		{
			this->field_1C = true;
		}
		else if (type == 206)
		{
			this->field_1C = false;
		}
		else if (type == 207)
		{
			Commands->Start_Timer(obj, this, 0.1f, 203);

			this->field_1D = true;
		}
		else if (type == 208)
		{
			this->field_1D = false;
		}
		else if (type == 209)
		{
			if (this->field_1D)
			{
				GameObject *paramObj = Commands->Find_Object(param);
				if (paramObj)
				{
					ActionParamsStruct params;
					params.Set_Basic(this, 90.0f, 201);
					params.Set_Attack(paramObj, 300.0f, 0.0f, true);
					params.MoveCrouched = true;
					params.AttackCrouched = true;

					if (this->field_2C > 4)
					{
						params.AttackCheckBlocked = false;
					}

					Commands->Action_Attack(obj, params);
				}
			}
		}
		else if (type == 211)
		{
			MX0_A02_Say_Something(obj, param, false);
		}
		else if (type == 213)
		{
			this->field_1D = false;
			GameObject *paramObj = Commands->Find_Object(param);
			if (paramObj)
			{
				this->field_30 = Commands->Get_ID(paramObj);
				GameObject *davesArrow = Commands->Find_Object(1100024);
				if (davesArrow)
				{
					ActionParamsStruct params;
					params.Set_Basic(this, 100.0f, 202);

					Vector3 davesArrowPos = Commands->Get_Position(davesArrow);

					params.Set_Movement(davesArrowPos, 0.8f, 0.5f);

					Commands->Action_Goto(obj, params);

					return;
				}
			}
			
			GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
			if (!MX0A02ControllerObj)
			{
				return;
			}

			Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 226, 0, 0.0f);
		}
		else if (type == 214)
		{
			this->field_1D = false;
			GameObject *paramObj = Commands->Find_Object(param);
			if (paramObj)
			{
				this->field_34 = Commands->Get_ID(paramObj);
				GameObject *davesArrow = Commands->Find_Object(1100024);
				if (davesArrow)
				{
					ActionParamsStruct params;
					params.Set_Basic(this, 100.0f, 203);

					Vector3 davesArrowPos = Commands->Get_Position(davesArrow);

					params.Set_Movement(davesArrow, 0.8f, 0.5f);
	
					Commands->Action_Goto(obj, params);

					return;
				}
			}

			Commands->Send_Custom_Event(obj, obj, 207, 0, 0.0f);
		}
		else if (type == 217)
		{
			Commands->Start_Timer(obj, this, 1.0f, 206);
		}
		else if (type == 219)
		{
			MX0_A02_Say_Something(obj, 204, true);
		}
		else if (type == 220)
		{
			this->field_1D = false;

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);
			Vector3 starPos = Commands->Get_Position(starObj);

			if (Commands->Get_Distance(pos, starPos) <= 50.0f)
			{
				MX0_A02_Say_Something(obj, 205, false);
			}
			else
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 208);
				params.Set_Movement(starObj, 0.8f, 3.0f);

				Commands->Action_Goto(obj, params);
			}
		}
		else if (type == 221)
		{
			MX0_A02_Say_Something(obj, 206, false);
		}
		else if (type == 229)
		{
			if (!this->field_1D)
			{
				return;
			}

			MX0_A02_Say_Something(obj, param + 221, false);
		}
		else if (type == 230)
		{
			if (!this->field_1D)
			{
				return;
			}

			MX0_A02_Say_Something(obj, param + 207, false);
		}
		else if (type == 231)
		{
			MX0_A02_Say_Something(obj, this->field_2C + 220, false);
		}
		else if (type == 233)
		{
			MX0_A02_Say_Something(obj, 225, false);
		}
		else if (type == 234)
		{
			GameObject *paramObj = Commands->Find_Object(param);
			if (!paramObj)
			{
				return;
			}

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 210);
			params.Set_Attack(paramObj, 300.0f, 0.0f, true);

			Commands->Action_Attack(obj, params);
		}
		else if (type == 236)
		{
			MX0_A02_Say_Something(obj, 226, false);
		}
		else if (type == 237)
		{
			GameObject *paramObj = Commands->Find_Object(param);
			if (!paramObj)
			{
				return;
			}

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 212);
			params.Set_Attack(paramObj, 300.0f, 0.0f, true);

			Commands->Action_Attack(obj, params);
		}
		else if (type == 240)
		{
			GameObject *paramObj = Commands->Find_Object(param);
			if (!paramObj)
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 215);
				params.Set_Movement(Vector3(-78.442f, -78.044f, 6.404f), 0.8f, 0.5f);

				Commands->Action_Goto(obj, params);

				return;
			}

			GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
			if (MX0A02ControllerObj)
			{
				Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 239, 0, 1.0f);
			}

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 214);
			params.Set_Attack(paramObj, 300.0f, 0.0f, true);

			Commands->Action_Attack(obj, params);
		}
		else if (type == 242)
		{
			MX0_A02_Say_Something(obj, 228, false);
		}
		else if (type == 243)
		{
			MX0_A02_Say_Something(obj, 229, false);
		}
		else if (type == 244)
		{
			MX0_A02_Say_Something(obj, 230, false);
		}
		else if (type == 245)
		{
			MX0_A02_Say_Something(obj, 231, false);
		}
		else if (type == 246)
		{
			if (this->field_2C == 1)
			{
				MX0_A02_Say_Something(obj, 227, false);
			}
			else
			{
				MX0_A02_Say_Something(obj, 233, false);
			}
		}
		else if (type == 241)
		{
			MX0_A02_Say_Something(obj, 227, false);
		}
		else if (type == 247)
		{
			GameObject *gotoPositionObj = Commands->Find_Object(1100021);
			if (this->field_2C == 6)
			{
				gotoPositionObj = Commands->Find_Object(1100016);
			}
			else if (this->field_2C == 7)
			{
				gotoPositionObj = Commands->Find_Object(1100018);
			}
			else if (this->field_2C == 8)
			{
				gotoPositionObj = Commands->Find_Object(1100020);
			}
			else if (this->field_2C == 5)
			{
				gotoPositionObj = Commands->Find_Object(1100014);
			}

			if (!gotoPositionObj)
			{
				return;
			}

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 218);

			Vector3 gotoPositionObjPos = Commands->Get_Position(gotoPositionObj);

			params.Set_Movement(gotoPositionObjPos, 0.8f, 0.5f);

			Commands->Action_Goto(obj, params);
		}
		else if (type == 248)
		{
			float movePosX = -105.302f;
			float movePosY = -55.383f;

			if (this->field_2C == 2)
			{
				movePosX = -90.786f;
				movePosY = -65.492f;
			}
			else if (this->field_2C == 3)
			{
				movePosX = -94.756f;
				movePosY = -59.51f;
			}
			else if (this->field_2C == 4)
			{
				movePosX = -104.989f;
				movePosY = -49.282f;
			}
			else if (this->field_2C == 1)
			{
				movePosX = -78.74f;
				movePosY = -60.34f;
			}

			this->field_1D = false;

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 201);
			params.Set_Movement(Vector3(movePosX, movePosY, 0.0f), 0.2f, 3.0f);

			Commands->Action_Goto(obj, params);
		}
		else if (type == 249)
		{
			MX0_A02_Say_Something(obj, 234, false);
		}
		else if (type == 250)
		{
			MX0_A02_Say_Something(obj, 235, false);
		}
		else if (type == 251)
		{
			MX0_A02_Say_Something(obj, 236, false);
		}
		else if (type == 252)
		{
			MX0_A02_Say_Something(obj, 237, true);
		}
		else if (type == 253)
		{
			MX0_A02_Say_Something(obj, 238, false);
		}
		else if (type == 255)
		{
			MX0_A02_Say_Something(obj, 239, false);

			if (!Commands->Find_Object(param))
			{
				return;
			}

			this->field_38 = param;
			GameObject *davesArrowObj = Commands->Find_Object(1100009);
			if (!davesArrowObj)
			{
				return;
			}

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 219);

			Vector3 davesArrowObjPos = Commands->Get_Position(davesArrowObj);

			params.Set_Movement(davesArrowObjPos, 0.8f, 3.0f);

			Commands->Action_Goto(obj, params);
		}
		else if (type == 257)
		{
			MX0_A02_Say_Something(obj, 241, false);
		}
		else if (type == 259)
		{
			MX0_A02_Say_Something(obj, 242, false);
		}
		else if (type == 260)
		{
			MX0_A02_Say_Something(obj, 243, false);
		}
		else if (type == 261)
		{
			MX0_A02_Say_Something(obj, 244, false);
		}
		else if (type == 263)
		{
			MX0_A02_Say_Something(obj, 245, false);
		}
		else if (type == 265)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 201);
			
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			params.Set_Movement(starObj, 0.8f, 10.0f);

			Commands->Action_Goto(obj, params);
		}
		else if (type == 267)
		{
			MX0_A02_Say_Something(obj, 246, false);
		}
		else if (type == 268)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 201);
			params.Set_Attack(Vector3(-85.66f, -73.31f, 22.81f), 300.0f, 0.0f, true);
			params.AttackCheckBlocked = false;

			Commands->Action_Attack(obj, params);
		}
		else if (type == 269)
		{
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);
			Vector3 starObjPos = Commands->Get_Position(starObj);

			if (Commands->Get_Distance(pos, starObjPos) < 10.0f)
			{
				if (this->field_2C == 3)
				{
					MX0_A02_Say_Something(obj, 248, true);
				}
				else if (this->field_2C == 4)
				{
					MX0_A02_Say_Something(obj, 249, true);
				}
				else if (this->field_2C == 2)
				{
					MX0_A02_Say_Something(obj, 247, true);
				}
			}
		}
	}
}

void MX0_A02_ACTOR::Enemy_Seen(GameObject *obj, GameObject *enemy)
{
	ActionParamsStruct params;
	params.Set_Basic(this, 90.0f, 201);
	params.Set_Attack(enemy, 300.0f, 0.0f, true);
	params.MoveCrouched = true;
	params.AttackCrouched = true;

	Commands->Action_Attack(obj, params);
}

void MX0_A02_ACTOR::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	if (this->field_1E)
	{
		if (complete_reason == ACTION_COMPLETE_CONVERSATION_ENDED || complete_reason == ACTION_COMPLETE_CONVERSATION_INTERRUPTED || complete_reason == ACTION_COMPLETE_CONVERSATION_UNABLE_TO_INIT)
		{
			if (action_id == 202)
			{
				GameObject *field30Obj = Commands->Find_Object(this->field_30);
				if (!field30Obj)
				{
					GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
					if (!MX0A02ControllerObj)
					{
						return;
					}

					Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 226, 0, 0.0f);

					return;
				}

				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 204);
				params.Set_Attack(field30Obj, 300.0f, 0.0f, true);
				params.AttackCheckBlocked = false;

				Commands->Action_Attack(obj, params);
			}
			else if (action_id == 203)
			{
				GameObject *field34Obj = Commands->Find_Object(this->field_34);
				if (!field34Obj)
				{
					Commands->Send_Custom_Event(obj, obj, 207, 0, 0.0f);

					return;
				}

				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 205);
				params.Set_Attack(field34Obj, 300.0f, 0.0f, true);
				params.AttackCheckBlocked = false;

				Commands->Action_Attack(obj, params);
			}
			else if (action_id == 204)
			{
				GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
				if (!MX0A02ControllerObj)
				{
					return;
				}

				Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 226, 0, 0.0f);
			}
			else if (action_id == 205)
			{
				Commands->Send_Custom_Event(obj, obj, 207, 0, 0.0f);
			}
			else if (action_id == 206 || action_id == 207)
			{
				if (!this->field_1D)
				{
					return;
				}

				GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
				if (!MX0A02ControllerObj)
				{
					return;
				}

				Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 209, this->field_2C, 0.0f);
			}
			else if (action_id == 208)
			{
				MX0_A02_Say_Something(obj, 205, true);
			}
			else if (action_id == 209)
			{
				this->field_1C = true;

				GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
				if (!MX0A02ControllerObj)
				{
					return;
				}

				Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 233, 0, 0.0f);
			}
			else if (action_id == 211)
			{
				this->field_1C = true;

				GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
				if (!MX0A02ControllerObj)
				{
					return;
				}

				Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 237, 0, 0.0f);
			}
			else if (action_id == 213)
			{
				GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
				if (!MX0A02ControllerObj)
				{
					return;
				}

				Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 240, 0, 0.0f);
			}
			else if (action_id == 214)
			{
				GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
				if (MX0A02ControllerObj)
				{
					Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 267, 0, 0.0f);
				}

				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 215);
				params.Set_Movement(Vector3(-78.442f, -78.044f, 6.404f), 0.8f, 0.5f);

				Commands->Action_Goto(obj, params);
			}
			else if (action_id == 215)
			{
				Commands->Destroy_Object(obj);

				GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
				if (MX0A02ControllerObj)
				{
					Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 235, 0, 0.0f);
				}
			}
			else if (action_id == 216)
			{
				Commands->Enable_Enemy_Seen(obj, true);
			}
			else if (action_id == 217)
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 90.0f, 201);

				Vector3 pos = Commands->Get_Position(obj);
				GameObject *starObj = Commands->Get_A_Star(pos);

				params.Set_Attack(starObj, 30.0f, 0.0f, true);
				params.Set_Movement(starObj, 0.8f, 1.0f);

				Commands->Action_Attack(obj, params);
			}
			else if (action_id == 218)
			{
				Commands->Apply_Damage(obj, 10000.0f, "Blamokiller", NULL);
			}
			else if (action_id == 219)
			{
				GameObject *field38Obj = Commands->Find_Object(this->field_38);
				if (field38Obj)
				{
					ActionParamsStruct params;
					params.Set_Basic(this, 100.0f, 201);
					params.Set_Attack(field38Obj, 300.0f, 0.0f, false);
					params.AttackCheckBlocked = false;

					Commands->Action_Attack(obj, params);

					Commands->Start_Timer(obj, this, 5.0f, 215);
				}
			}
			else if (action_id == 220)
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 201);
				params.Set_Attack(Vector3(-75.0f, -43.4f, 0.1f), 0.0f, 0.0f, true);
				params.MoveCrouched = true;
				params.AttackCrouched = true;

				Commands->Action_Attack(obj, params);
			}
		}
		else
		{
			if (action_id == 202)
			{
				this->field_1D = true;

				Commands->Start_Timer(obj, this, 0.1f, 203);

				GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
				if (MX0A02ControllerObj)
				{
					Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 218, 0, 0.0f);
				}
			}
			else if (action_id == 203)
			{
				this->field_1D = true;

				Commands->Start_Timer(obj, this, 0.1f, 203);

				GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
				if (MX0A02ControllerObj)
				{
					Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 220, 0, 0.0f);
				}
			}
			else if (action_id == 204)
			{
				this->field_1D = true;

				Commands->Start_Timer(obj, this, 0.1f, 203);

				GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
				if (MX0A02ControllerObj)
				{
					Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 221, 0, 0.0f);
				}
			}
			else if (action_id == 205)
			{
				this->field_1D = true;

				Commands->Start_Timer(obj, this, 0.1f, 203);

				GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
				if (MX0A02ControllerObj)
				{
					Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 219, 0, 0.0f);
				}
			}
			else if (action_id == 206)
			{
				this->field_1D = true;

				Commands->Start_Timer(obj, this, 0.1f, 203);
				
				GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
				if (MX0A02ControllerObj)
				{
					Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 222, 0, 0.0f);
				}
			}
			else if (action_id == 225)
			{
				GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
				if (MX0A02ControllerObj)
				{
					Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 234, 0, 0.0f);
				}
			}
			else if (action_id == 234)
			{
				GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
				if (MX0A02ControllerObj)
				{
					Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 250, 0, 0.0f);
				}
			}
			else if (action_id == 235)
			{
				GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
				if (MX0A02ControllerObj)
				{
					Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 251, 0, 0.0f);
				}
			}
			else if (action_id == 236)
			{
				GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
				if (MX0A02ControllerObj)
				{
					Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 252, 0, 0.0f);
				}
			}
			else if (action_id == 237)
			{
				GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
				if (MX0A02ControllerObj)
				{
					Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 253, 0, 0.0f);
				}
			}
			else if (action_id == 238)
			{
				GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
				if (MX0A02ControllerObj)
				{
					Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 254, 0, 0.0f);
				}
			}
			else if (action_id == 240)
			{
				Commands->Create_2D_Sound("MX0_GDIEAGLEBASE_117");

				Commands->Set_HUD_Help_Text(8387, Vector3(0.196f, 0.882f, 0.196f)); // Press 'E' to enter the Medium Tank\n
				
				GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
				if (MX0A02ControllerObj)
				{
					Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 256, 0, 0.0f);
				}
			}
			else if (action_id == 245)
			{
				GameObject *davesArrowObj = Commands->Find_Object(1100024);
				if (davesArrowObj)
				{
					ActionParamsStruct params;
					params.Set_Basic(this, 100.0f, 221);

					Vector3 davesArrowObjPos = Commands->Get_Position(davesArrowObj);

					params.Set_Movement(davesArrowObjPos, 0.8f, 5.0f);

					Commands->Action_Goto(obj, params);
				}

				GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
				if (MX0A02ControllerObj)
				{
					Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 264, 0, 0.0f);
				}
			}
			else if (action_id == 229)
			{
				GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
				if (MX0A02ControllerObj)
				{
					Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 268, 0, 0.0f);
				}
			}
		}
	}
}

void MX0_A02_ACTOR::Timer_Expired(GameObject *obj, int number)
{
	if (this->field_1E)
	{
		if (number == 202)
		{
			if (this->field_1D)
			{
				GameObject *gotoDavesArrowObj = Commands->Find_Object(1100021);
				switch (this->field_2C)
				{
					case 1:
						gotoDavesArrowObj = Commands->Find_Object(1100007);
						break;
					case 2:
						gotoDavesArrowObj = Commands->Find_Object(1100011);
						break;
					case 3:
						gotoDavesArrowObj = Commands->Find_Object(1100009);
						break;
					case 4:
						gotoDavesArrowObj = Commands->Find_Object(1100013);
						break;
					case 5:
						gotoDavesArrowObj = Commands->Find_Object(1100015);
						break;
					case 6:
						gotoDavesArrowObj = Commands->Find_Object(1100017);
						break;
					case 7:
						gotoDavesArrowObj = Commands->Find_Object(1100019);
						break;
					case 8:
						gotoDavesArrowObj = Commands->Find_Object(1100021);
						break;
					default:
						break;
				}

				if (gotoDavesArrowObj)
				{
					ActionParamsStruct params;
					params.Set_Basic(this, 100.0f, 206);

					Vector3 gotoDavesArrowObjPos = Commands->Get_Position(gotoDavesArrowObj);

					params.Set_Movement(gotoDavesArrowObjPos, 0.8f, 0.5f);

					Commands->Action_Goto(obj, params);
				}

				float randInterval = Commands->Get_Random(0.0f, 4.9998999f);
				if (randInterval > 4.0f)
				{
					randInterval = 4.0f;
				}

				randInterval += 4.0f;

				Commands->Start_Timer(obj, this, randInterval, 203);
			}
		}
		else if (number == 203)
		{
			if (this->field_1D)
			{
				GameObject *gotoDavesArrowObj = Commands->Find_Object(1100021);
				switch (this->field_2C)
				{
					case 1:
						gotoDavesArrowObj = Commands->Find_Object(1100006);
						break;
					case 2:
						gotoDavesArrowObj = Commands->Find_Object(1100010);
						break;
					case 3:
						gotoDavesArrowObj = Commands->Find_Object(1100008);
						break;
					case 4:
						gotoDavesArrowObj = Commands->Find_Object(1100012);
						break;
					case 5:
						gotoDavesArrowObj = Commands->Find_Object(1100014);
						break;
					case 6:
						gotoDavesArrowObj = Commands->Find_Object(1100016);
						break;
					case 7:
						gotoDavesArrowObj = Commands->Find_Object(1100018);
						break;
					case 8:
						gotoDavesArrowObj = Commands->Find_Object(1100020);
						break;
					default:
						break;
				}

				if (gotoDavesArrowObj)
				{
					ActionParamsStruct params;
					params.Set_Basic(this, 100.0f, 207);

					Vector3 gotoDavesArrowObjPos = Commands->Get_Position(gotoDavesArrowObj);

					params.Set_Movement(gotoDavesArrowObjPos, 0.8f, 0.5f);

					Commands->Action_Goto(obj, params);
				}

				float randInterval = Commands->Get_Random(0.0f, 4.9998999f);
				if (randInterval > 4.0f)
				{
					randInterval = 4.0f;
				}

				randInterval += 4.0f;

				Commands->Start_Timer(obj, this, randInterval, 202);
			}
		}
		else if (number == 205)
		{
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);
			Vector3 starObjPos = Commands->Get_Position(starObj);

			if (Commands->Get_Distance(pos, starObjPos) >= 50.0f)
			{
				Commands->Start_Timer(obj, this, 1.0f, 205);
			}
			else
			{
				this->field_1D = false;

				MX0_A02_Say_Something(obj, 202, true);
			}
		}
		else if (number == 206)
		{
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);
			Vector3 starObjPos = Commands->Get_Position(starObj);

			if (Commands->Get_Distance(pos, starObjPos) >= 50.0f)
			{
				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 201);
				params.Set_Movement(starObjPos, 0.8f, 0.5f);

				Commands->Action_Goto(obj, params);

				Commands->Start_Timer(obj, this, 1.0f, 206);
			}
			else
			{
				this->field_1D = false;

				MX0_A02_Say_Something(obj, 203, true);
			}
		}
		else if (number == 210)
		{
			GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
			if (MX0A02ControllerObj)
			{
				Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 245, 0, 1.0f);
			}

			Commands->Apply_Damage(obj, 10000.0f, "Blamokiller", obj);
		}
		else if (number == 214)
		{
			Commands->Apply_Damage(obj, 10000.0f, "Blamokiller", obj);
		}
		else if (number == 215)
		{
			GameObject *field38Obj = Commands->Find_Object(this->field_38);
			if (field38Obj)
			{
				Vector3 field38ObjPos = Commands->Get_Position(field38Obj);
				float field38ObjFacing = Commands->Get_Facing(field38Obj);
				Commands->Destroy_Object(field38Obj);

				GameObject *medTank = Commands->Create_Object("GDI_Medium_Tank_Player", field38ObjPos);
				if (medTank)
				{
					Commands->Attach_Script(obj, "M00_Send_Object_ID", "1400041,13,0.0f"); // Is obj here correct?
					Commands->Set_Facing(medTank, field38ObjFacing);

					MX0_A02_Say_Something(obj, 240, true);

					Commands->Attach_Script(medTank, "MX0_A02_GDI_MEDTANK", "");
				}
			}
		}
		else if (number == 216)
		{
			GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
			if (MX0A02ControllerObj)
			{
				Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 255, 0, 0.0f);
			}
		}
		else if (number == 217)
		{
			GameObject *MX0A02ControllerObj = Commands->Find_Object(1100000);
			if (MX0A02ControllerObj)
			{
				Commands->Send_Custom_Event(obj, MX0A02ControllerObj, 258, 0, 0.0f);
			}

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 220);
			params.Set_Movement(Vector3(-76.3f, -45.0f, 0.1f), 0.8f, 1.0f);

			Commands->Action_Goto(obj, params);
		}
	}
}

void MX0_A02_ACTOR::MX0_A02_Say_Something(GameObject *obj, int actionId, bool unknown)
{
	if (this->field_1E)
	{
		int priority = 100;
		char *conversation = "MX0_A02_PREAMB_01"; // This conversation does not exist
		switch (actionId)
		{
			case 1:
				conversation = "MX0_GDITROOPER2_051"; // Incoming!
				break;
			case 2:
				conversation = "MX0_GDITROOPER3_053"; // Stay low!
				break;
			case 3:
				conversation = "MX0_GDITROOPER4_054"; // I’m hit!
				break;
			case 4:
				conversation = "MX0_GDITROOPER2_056"; // Up there – snipers!
				break;
			case 5:
				conversation = "MX0_GDITROOPER1_057"; // Where is he?  I can’t see him!
				break;
			case 6:
				conversation = "MX0_GDITROOPER4_058"; // Where's that medic?
				break;
			case 7:
				conversation = "MX0_GDITROOPER4_060"; // They're everywhere!
				break;
			case 8:
				conversation = "MX0_GDITROOPER2_062"; // Go, go, go!
				break;
			case 9:
				conversation = "MX0_GDITROOPER1_063"; // We’re pinned.  There's nowhere to move!
				break;
			case 10:
				conversation = "MX0_GDITROOPER3_064"; // Where’re those  reinforcements?
				break;
			case 11:
				conversation = "MX0_GDITROOPER4_065"; // I’m running low on ammo!
				break;
			case 205:
				conversation = "MX0_GDITROOPER2_069"; // Captain Havoc!  This way, sir!
				break;
			case 202:
				conversation = "MX0_GDITROOPER1_066"; // Reinforcements?  There’s just one guy!
				break;
			case 203:
				conversation = "MX0_GDITROOPER2_067"; // Hey, it’s Havoc!
				break;
			case 204:
				conversation = "MX0_GDITROOPER1_068"; // Are we glad to see you!
				break;
			case 206:
				priority = 99;
				conversation = "MX0_GDITROOPER3_070"; // Payback time!
				break;
			case 207:
				conversation = "MX0_GDITROOPER1_071"; // Hey, I got one!
				break;
			case 208:
				conversation = "MX0_GDITROOPER4_072"; // Nice shot!
				break;
			case 209:
				conversation = "MX0_GDITROOPER3_073"; // There’s one!
				break;
			case 210:
				conversation = "MX0_GDITROOPER2_074"; // Come and get it!
				break;
			case 211:
				conversation = "MX0_GDITROOPER1_075"; // Great Shot, Sir!
				break;
			case 212:
				conversation = "MX0_GDITROOPER4_076"; // You got him, Havoc!
				break;
			case 213:
				conversation = "MX0_GDITROOPER2_078"; // Outstanding, Havoc!
				break;
			case 215:
				conversation = "MX0_GDITROOPER1_ALT09"; // Yeah!
				break;
			case 216: 
				conversation = "MX0_GDITROOPER4_ALT07"; // Go!
				break;
			case 214:
			case 217:
				conversation = "MX0_GDITROOPER3_ALT13"; // Good work!
				break;
			case 218:
				conversation = "MX0_GDITROOPER1_ALT10"; // Alright!
				break;
			case 219:
				conversation = "MX0_GDITROOPER2_ALT01"; // Nice shot!
				break;
			case 220:
				conversation = "MX0_GDITROOPER4_ALT06"; // Yeah!
				break;
			case 221:
				conversation = "MX0_GDITROOPER1_079"; // Don’t leave us, Sir!
				break;
			case 222:
				conversation = "MX0_GDITROOPER2_082"; // Hey! We need you’re help!
				break;
			case 223:
				conversation = "MX0_GDITROOPER3_081"; // Havoc! Come back!
				break;
			case 224:
				conversation = "MX0_GDITROOPER4_080"; // Wait! Where are you going, Havoc?
				break;
			case 225:
				conversation = "MX0_GDITROOPER1_086"; // Up there on the rocks!
				break;
			case 226:
				conversation = "MX0_GDITROOPER1_088"; // Ha!  Look at that!
				break;
			case 227:
				conversation = "MX0_GDITROOPER2_093"; // Chopper down and OUT!
				break;
			case 228:
				conversation = "MX0_GDITROOPER2_092"; // They’re dropping more troops!
				break;
			case 229:
				conversation = "MX0_GDITROOPER2_095"; // Another chopper – Take it out!
				break;
			case 230:
				conversation = "MX0_GDITROOPER3_096"; // Flame Troopers!  Watch your back!
				break;
			case 231:
				conversation = "MX0_GDITROOPER4_097"; // Roasted!
				break;
			case 232:
				conversation = "MX0_GDITROOPER1_099"; // All-RIGHT!
				break;
			case 233:
				conversation = "MX0_GDITROOPER2_098"; // Woo-hoo!
				break;
			case 234:
				conversation = "MX0_GDITROOPER2_105"; // We pushed 'em back!
				break;
			case 235:
				conversation = "MX0_GDITROOPER4_106"; // Look!  They’re on the run!
				break;
			case 236:
				conversation = "MX0_GDITROOPER1_107"; // We did it!
				break;
			case 237:
				conversation = "MX0_GDITROOPER2_108"; // Thanks Havoc.  We owe you, big time.
				break;
			case 238:
				conversation = "MX0_GDITROOPER3_109"; // Recon 1 to Eagle Base – We’ve taken casualties, but we’re okay
				break;
			case 239:
				conversation = "MX0_ENGINEER1_115"; // Stand by for repairs.
				break;
			case 240:
				conversation = "MX0_ENGINEER1_118"; // Your tank is repaired and ready to roll, Sir.
				break;
			case 241:
				conversation = "MX0_GDITROOPER1_119"; // You’ll be unstoppable in that tank, sir!
				break;
			case 242:
				conversation = "MX0_ENGINEER2_120"; // I’ll clear the road once you're inside the tank, Havoc.
				break;
			case 243:
				conversation = "MX0_GDITROOPER1_121"; // Your Tank's ready and waiting, sir!
				break;
			case 244:
				conversation = "MX0_GDITROOPER2_122"; // The Tank's yours, Havoc!   Rock and roll!
				break;
			case 245:
				conversation = "MX0_ENGINEER2_123"; // C4’s set... Fire in the hole!
				break;
			case 246:
				conversation = "MX0_GDITROOPER3_090"; // Aagh! We lost our APC!
				break;
			case 247:
				conversation = "MX0_A02_SPEECH_WRONGWAY_01"; // This conversation does not exist
				break;
			case 248:
				conversation = "MX0_A02_SPEECH_WRONGWAY_02"; // This conversation does not exist
				break;
			case 249:
				conversation = "MX0_A02_SPEECH_WRONGWAY_03"; // This conversation does not exist
				break;
			default:
				break;
		}

		int conversationId = Commands->Create_Conversation(conversation, priority, 300.0f, false);
		Commands->Join_Conversation(obj, conversationId, false, true, true);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, false, false, false);
		Commands->Start_Conversation(conversationId, actionId);
		Commands->Monitor_Conversation(obj, conversationId);
	}
}

ScriptRegistrant<MX0_A02_ACTOR> MX0_A02_ACTORRegistrant("MX0_A02_ACTOR", "ActorID=0:int");