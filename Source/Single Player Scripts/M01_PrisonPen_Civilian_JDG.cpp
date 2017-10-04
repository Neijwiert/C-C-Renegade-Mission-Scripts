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
#include "M01_PrisonPen_Civilian_JDG.h"

/*
M01 -> 101929 101930 101931
*/
void M01_PrisonPen_Civilian_JDG::Register_Auto_Save_Variables()
{
	// randMoveLocations is not stored, could be a problem when save/loading the game

	Auto_Save_Variable(&this->detentionSoldier2ObjId, sizeof(this->detentionSoldier2ObjId), 1);
	Auto_Save_Variable(&this->objId, sizeof(this->objId), 2);
	Auto_Save_Variable(&this->prisonerCivillianMate1ObjId, sizeof(this->prisonerCivillianMate1ObjId), 3);
	Auto_Save_Variable(&this->prisonerCivillianMate2ObjId, sizeof(this->prisonerCivillianMate2ObjId), 4);
	Auto_Save_Variable(&this->movementActionIndex, sizeof(this->movementActionIndex), 5);
	Auto_Save_Variable(&this->minMovementInterval, sizeof(this->minMovementInterval), 6);
	Auto_Save_Variable(&this->maxMovementInterval, sizeof(this->maxMovementInterval), 7);
	Auto_Save_Variable(&this->field_B0, sizeof(this->field_B0), 8);
	Auto_Save_Variable(&this->heardStarShoot, sizeof(this->heardStarShoot), 9);
	Auto_Save_Variable(&this->field_B2, sizeof(this->field_B2), 10);
	Auto_Save_Variable(&this->field_B3, sizeof(this->field_B3), 11);
}

void M01_PrisonPen_Civilian_JDG::Created(GameObject *obj)
{
	this->field_B0 = false;
	this->heardStarShoot = false;
	this->field_B2 = false;
	this->field_B3 = false;

	Commands->Innate_Soldier_Enable_Enemy_Seen(obj, false);
	Commands->Innate_Soldier_Enable_Gunshot_Heard(obj, false);
	Commands->Innate_Soldier_Enable_Bullet_Heard(obj, false);

	this->randMoveLocations[0] = Vector3(-313.881f, 589.854f, 27.457f);
	this->randMoveLocations[1] = Vector3(-303.287f, 587.881f, 27.457f);
	this->randMoveLocations[2] = Vector3(-296.29f, 595.483f, 27.458f);
	this->randMoveLocations[3] = Vector3(-295.055f, 589.244f, 27.458f);
	this->randMoveLocations[4] = Vector3(-309.122f, 578.526f, 27.741f);
	this->randMoveLocations[5] = Vector3(-299.833f, 578.027f, 27.735f);
	this->randMoveLocations[6] = Vector3(-297.657f, 571.844f, 27.458f);
	this->randMoveLocations[7] = Vector3(-289.746f, 578.258f, 27.781f);
	this->randMoveLocations[8] = Vector3(-280.325f, 579.701f, 27.458f);
	this->randMoveLocations[9] = Vector3(-286.094f, 585.007f, 27.72f);

	this->minMovementInterval = 5.0f;
	this->maxMovementInterval = 30.0f;
	this->movementActionIndex = 20;
	this->detentionSoldier2ObjId = 101927;

	this->objId = Commands->Get_ID(obj);
	if (this->objId == 101929)
	{
		this->prisonerCivillianMate1ObjId = 101930;
		this->prisonerCivillianMate2ObjId = 101931;
	}
	else if (this->objId == 101930)
	{
		this->prisonerCivillianMate1ObjId = 101929;
		this->prisonerCivillianMate2ObjId = 101931;
	}
	else if (this->objId == 101931)
	{
		this->prisonerCivillianMate1ObjId = 101929;
		this->prisonerCivillianMate2ObjId = 101930;
	}

	Commands->Start_Timer(obj, this, 5.0f, 67);
}

// TODO
void M01_PrisonPen_Civilian_JDG::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (param == 24)
	{
		Commands->Innate_Soldier_Enable_Gunshot_Heard(obj, false);
		Commands->Innate_Soldier_Enable_Bullet_Heard(obj, false);

		this->field_B0 = false;
	}
	else if (param == 46)
	{
		static const char *animations[4] =
		{
			"H_A_J01C",
			"H_A_J18C",
			"H_A_J24C",
			"H_A_J26C"
		};

		int randAnimationIndex = Commands->Get_Random_Int(0, 4);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 46);
		params.Set_Animation(animations[randAnimationIndex], false);

		Commands->Action_Play_Animation(obj, params);
	}
	else if (param == 63)
	{
		Commands->Innate_Disable(obj);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 63);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		params.Set_Attack(starObj, 0.0f, 0.0f, true);

		Commands->Action_Attack(obj, params);

		float randDuration = Commands->Get_Random(1.25f, 3.5f);
		Commands->Send_Custom_Event(obj, obj, 0, 46, randDuration);
	}
	else if (param == 16)
	{
		Commands->Innate_Soldier_Enable_Gunshot_Heard(obj, true);
		Commands->Innate_Soldier_Enable_Bullet_Heard(obj, true);

		this->field_B0 = true;
	}
}

// TODO
void M01_PrisonPen_Civilian_JDG::Sound_Heard(GameObject *obj, const CombatSound & sound)
{
	if (sound.sound == 400004)
	{
		if (!this->field_B2)
		{
			this->field_B2 = true;

			Commands->Action_Reset(obj, 100.0f);

			if (Commands->Find_Object(100034))  // Comm center sam site
			{
				Vector3 moveLocation;

				objId = Commands->Get_ID(obj);
				switch (objId)
				{
					case 101929:
						moveLocation = Vector3(-303.867f, 579.217f, 27.604f);

						break;
					case 101930:
						moveLocation = Vector3(-304.901f, 575.936f, 27.724f);

						break;
					default:
						moveLocation = Vector3(-304.887f, 582.889f, 27.457f);

						break;
				}

				ActionParamsStruct params;
				params.Set_Basic(this, 100.0f, 42);
				params.Set_Movement(moveLocation, 0.8f, 0.5f);

				Commands->Action_Goto(obj, params);
			}
		}
	}
	else if (sound.sound == 400005)
	{
		this->field_B3 = true;

		Commands->Set_Innate_Is_Stationary(obj, false);

		Commands->Action_Reset(obj, 100.0f);

		Vector3 moveLocation;

		objId = Commands->Get_ID(obj);
		switch (objId)
		{
			case 101929:
				moveLocation = Vector3(-303.867f, 579.217f, 27.604f);

				break;
			case 101930:
				moveLocation = Vector3(-304.901f, 575.936f, 27.724f);

				break;
			default:
				moveLocation = Vector3(-304.887f, 582.889f, 27.457f);

				break;
		}

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 42);
		params.Set_Movement(moveLocation, 0.8f, 0.5f);

		Commands->Action_Goto(obj, params);
	}
	else if (sound.sound == SOUND_TYPE_GUNSHOT || sound.sound == SOUND_TYPE_BULLET_HIT)
	{
		Vector3 pos = Commands->Get_Position(obj);
		if (sound.Creator == Commands->Get_A_Star(pos) && this->field_B0 && !this->heardStarShoot)
		{
			Commands->Send_Custom_Event(obj, obj, 0, 63, 0.0f);

			this->heardStarShoot = true;
		}
	}
}

// TODO
void M01_PrisonPen_Civilian_JDG::Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason)
{
	static const char *animations[4] =
	{
		"H_A_J01C",
		"H_A_J18C",
		"H_A_J24C",
		"H_A_J26C"
	};

	if (action_id == 38)
	{
		if (complete_reason == ACTION_COMPLETE_NORMAL)
		{
			float randDuration = Commands->Get_Random(this->minMovementInterval, this->maxMovementInterval);
			Commands->Start_Timer(obj, this, randDuration, 0);
		}
	}
	else if (action_id == 39)
	{
		int randAnimationIndex = Commands->Get_Random_Int(0, 4);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 47);
		params.Set_Animation(animations[randAnimationIndex], false);

		Commands->Action_Play_Animation(obj, params);
	}
	else if (action_id == 40)
	{
		int randAnimationIndex = Commands->Get_Random_Int(0, 4);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 48);
		params.Set_Animation(animations[randAnimationIndex], false);

		Commands->Action_Play_Animation(obj, params);
	}
	else if (action_id == 48)
	{
		Commands->Set_Innate_Is_Stationary(obj, true);
	}
	else if (action_id == 41)
	{
		int randAnimationIndex = Commands->Get_Random_Int(0, 4);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 49);
		params.Set_Animation(animations[randAnimationIndex], false);

		Commands->Action_Play_Animation(obj, params);

		if (obj == Commands->Find_Object(101929)) // detentionCiv1Obj
		{
			int conversationId = Commands->Create_Conversation("M01_Detention_Babushka_Evac_Conversation01", 100, 1000.0f, false); // Thank you GDI! I knew they were lying!
			Commands->Join_Conversation(obj, conversationId, true, true, true);

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			Commands->Join_Conversation(starObj, conversationId, false, false, false);
			Commands->Start_Conversation(conversationId, conversationId);
		}
	}
	else if (action_id == 49)
	{
		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 4, 5.0f);

		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 42);
		params.Set_Movement(Vector3(-328.136f, 548.518f, 30.044f), 0.8f, 3.0f);

		Commands->Action_Goto(obj, params);
	}
	else if (action_id == 42)
	{
		Commands->Set_Innate_Is_Stationary(obj, true);

		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		if (M01MissionControllerJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 4, 0.0f);
		}
	}
}

// TODO
void M01_PrisonPen_Civilian_JDG::Timer_Expired(GameObject *obj, int number)
{
	int x;
	do
	{
		x = static_cast<int>(Commands->Get_Random(0.0f, 12.9999f));
	} while (x == this->movementActionIndex);

	this->movementActionIndex = x;

	if (this->movementActionIndex <= 9)
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 38);
		params.Set_Movement(this->randMoveLocations[this->movementActionIndex], 0.3f, 1.0f);

		Commands->Action_Goto(obj, params);
	}
	else if (this->movementActionIndex == 10)
	{
		GameObject *prisonerCivillianMate1Obj = Commands->Find_Object(this->prisonerCivillianMate1ObjId);
		if (prisonerCivillianMate1Obj)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 38);
			params.Set_Movement(prisonerCivillianMate1Obj, 0.3f, 2.0f);

			Commands->Action_Goto(obj, params);
		}
	}
	else if (this->movementActionIndex == 11)
	{
		GameObject *prisonerCivillianMate2Obj = Commands->Find_Object(this->prisonerCivillianMate2ObjId);
		if (prisonerCivillianMate2Obj)
		{
			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 38);
			params.Set_Movement(prisonerCivillianMate2Obj, 0.3f, 2.0f);

			Commands->Action_Goto(obj, params);
		}
	}
	else
	{
		ActionParamsStruct params;
		params.Set_Basic(this, 100.0f, 38);
		
		GameObject *detentionSoldier2Obj = Commands->Find_Object(this->detentionSoldier2ObjId);

		params.Set_Movement(detentionSoldier2Obj, 0.3f, 2.0f);

		Commands->Action_Goto(obj, params);
	}

	this->minMovementInterval = 5.0f;
	this->maxMovementInterval = 30.0f;
}

void M01_PrisonPen_Civilian_JDG::Poked(GameObject *obj, GameObject *poker)
{
	if (this->field_B2 && !this->field_B3)
	{
		GameObject *detentionCiv1Obj = Commands->Find_Object(101929);
		GameObject *detentionCiv2Obj = Commands->Find_Object(101930);
		GameObject *detentionCiv3Obj = Commands->Find_Object(101931);
		
		const char *conversationName = NULL;
		if (obj == detentionCiv1Obj)
		{
			conversationName = "M01_Detention_Babushka_Poke_Conversation01"; // Where is the transport helicopter?
		}
		else if (obj == detentionCiv2Obj)
		{
			conversationName = "M01_Detention_Pierre_Poke_Conversation01"; // I'm no soldier, but I think the SAM Site needs to go.
		}
		else if (obj == detentionCiv3Obj)
		{
			conversationName = "M01_Detention_FarmerJohn_Poke_Conversation01"; // I'm not going anywhere until it's safe.
		}

		if (conversationName == NULL)
		{
			return;
		}

		int conversationId = Commands->Create_Conversation(conversationName, 100, 1000.0f, false);
		Commands->Join_Conversation(obj, conversationId, false, true, true);
		Commands->Start_Conversation(conversationId, conversationId);
	}
}

ScriptRegistrant<M01_PrisonPen_Civilian_JDG> M01_PrisonPen_Civilian_JDGRegistrant("M01_PrisonPen_Civilian_JDG", "");