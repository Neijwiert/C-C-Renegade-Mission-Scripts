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
#include "M02_Objective_Zone.h"

void M02_Objective_Zone::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->canSendCustomAndDestroy, sizeof(this->canSendCustomAndDestroy), 1);
	Auto_Save_Variable(&this->mendozaObjId, sizeof(this->mendozaObjId), 2);
	Auto_Save_Variable(&this->isZoneTriggered, sizeof(this->isZoneTriggered), 3);
	Auto_Save_Variable(&this->starWarnedForLeavingPrematurely, sizeof(this->starWarnedForLeavingPrematurely), 4);
}

// On level start
void M02_Objective_Zone::Created(GameObject *obj)
{
	this->starWarnedForLeavingPrematurely = false;
	this->mendozaObjId = 0;
	this->isZoneTriggered = false;
	this->canSendCustomAndDestroy = false;
}

// TODO
void M02_Objective_Zone::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 100)
	{
		if (param == 1)
		{
			this->canSendCustomAndDestroy = false;
		}
	}

	// Received from ourselves when timer number 9 triggers
	// TODO
	else if (type == 999)
	{
		if (param == 999)
		{
			GameObject *mendozaObj = Commands->Find_Object(this->mendozaObjId);
			if (mendozaObj)
			{
				this->mendozaObjId = 0;

				Commands->Destroy_Object(mendozaObj);
			}
		}
		else
		{
			this->mendozaObjId = param;
		}
	}
}

// TODO
void M02_Objective_Zone::Timer_Expired(GameObject *obj, int number)
{
	if (number == 3)
	{
		Send_and_Destroy(obj, 207);
	}
	else if (number == 4)
	{
		Send_and_Destroy(obj, 208);
	}
	else if (number == 5)
	{
		Send_and_Destroy(obj, 209);
	}
	else if (number == 6)
	{
		Send_and_Destroy(obj, 210);
	}
	else if (number == 7)
	{
		Send_and_Destroy(obj, 211);
	}
	else if (number == 9)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Set_Position(starObj, Vector3(1190.27f, 558.538f, 32.5f));

		GameObject *nodJetObj = Commands->Create_Object("Nod_Jet", Vector3(1209.88f, 528.647f, 21.313f));
		if (nodJetObj)
		{
			Commands->Attach_Script(nodJetObj, "M02_Nod_Jet_Waypath", "");
		}

		GameObject *M02ObjectiveControllerObj = Commands->Find_Object(1111112);
		if (M02ObjectiveControllerObj)
		{
			Commands->Control_Enable(starObj, true);

			ActionParamsStruct params;
			params.Set_Basic(this, 100.0f, 0);

			Commands->Action_Follow_Input(starObj, params);

			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 201, 1, 0.0f);
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 205, 0, 0.0f);
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 21, 0.0f);

			GameObject *airStripOfficerObj = Commands->Find_Object(400277);
			Commands->Send_Custom_Event(obj, airStripOfficerObj, 0, 0, 0.0f);

			GameObject *honRocketSoldierObj = Commands->Find_Object(401141);
			Commands->Send_Custom_Event(obj, honRocketSoldierObj, 0, 0, 0.0f);

			GameObject *airStripMinigunner1Obj = Commands->Find_Object(401142);
			Commands->Send_Custom_Event(obj, airStripMinigunner1Obj, 0, 0, 0.0f);

			GameObject *airStripMinigunner2Obj = Commands->Find_Object(401143);
			Commands->Send_Custom_Event(obj, airStripMinigunner2Obj, 0, 0, 0.0f);

			GameObject *airStripMinigunner3Obj = Commands->Find_Object(401144);
			Commands->Send_Custom_Event(obj, airStripMinigunner3Obj, 0, 0, 0.0f);

			GameObject *airStripMinigunner4Obj = Commands->Find_Object(401145);
			Commands->Send_Custom_Event(obj, airStripMinigunner4Obj, 0, 0, 0.0f);

			GameObject *nonExistingObj = Commands->Find_Object(401146); // Does not exist
			Commands->Send_Custom_Event(obj, nonExistingObj, 0, 0, 0.0f);

			GameObject *mendozaObj = Commands->Create_Object("Nod_FlameThrower_3Boss", Vector3(1260.01f, 535.46f, 18.428f));
			if (mendozaObj)
			{
				Commands->Attach_Script(mendozaObj, "M02_Mendoza", "");

				this->mendozaObjId = Commands->Get_ID(mendozaObj);

				GameObject *airStripStartScriptZoneObj = Commands->Find_Object(400194);
				if (airStripStartScriptZoneObj)
				{
					Commands->Send_Custom_Event(obj, airStripStartScriptZoneObj, 999, this->mendozaObjId, 0.0f);
				}
			}

			Commands->Destroy_Object(obj);
		}
	}
	else if (number == 10)
	{
		GameObject *skiResortEntranceScriptZoneObj = Commands->Find_Object(401028);
		if (!skiResortEntranceScriptZoneObj)
		{
			return;
		}

		Vector3 skiResortEntranceScriptZoneObjPos = Commands->Get_Position(skiResortEntranceScriptZoneObj);
		Vector3 pos = Commands->Get_Position(obj);

		GameObject *starObj = Commands->Get_A_Star(pos);
		Vector3 starPos = Commands->Get_Position(starObj);

		if (Commands->Get_Distance(skiResortEntranceScriptZoneObjPos, starPos) >= 20.0f)
		{
			Commands->Start_Timer(obj, this, 2.0f, 10);
		}
		else
		{
			int conversationId = Commands->Create_Conversation("MX2DSGN_DSGN0009", 100, 300.0f, true); // This is Bulldog Five - glad you could join us, sir <radioized>
			Commands->Join_Conversation(NULL, conversationId, true, true, true);
			Commands->Join_Conversation(starObj, conversationId, true, false, false);
			Commands->Start_Conversation(conversationId, 0);
		}
	}
	else if (number == 11)
	{
		if (!Commands->Find_Object(401028))// Ski resort entrance script zone
		{
			return;
		}

		int conversationId = Commands->Create_Conversation("MX2DSGN_DSGN0010", 100, 300.0f, true); // Thanks for the fireworks, sir. Bulldog five out <when you clear the objective>
		Commands->Join_Conversation(NULL, conversationId, true, true, true);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, false, false);
		Commands->Start_Conversation(conversationId, 0);
	}
	else if (number == 12)
	{
		if (!Commands->Find_Object(401036)) // GDI med tank at bridge
		{
			return;
		}

		// They're swarming like flies around the Bridge, sir <radioized>
		// My engine's siezed up, but I can stay here and guard the road. <radioized>
		// Go get 'em, sir! <radioized> 
		int conversationId = Commands->Create_Conversation("MX2DSGN_DSGN0011", 100, 300.0f, true);
		Commands->Join_Conversation(NULL, conversationId, true, true, true);
	 
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, false, false);
		Commands->Start_Conversation(conversationId, 0);
	}
}

// TODO
void M02_Objective_Zone::Entered(GameObject *obj, GameObject *enterer)
{
	GameObject *M02ObjectiveControllerObj = Commands->Find_Object(1111112);
	if (!M02ObjectiveControllerObj)
	{
		return;
	}

	int objId = Commands->Get_ID(obj);
	if (objId == 400551)
	{
		Create_Apache(24);

		Commands->Destroy_Object(obj);
	}
	else if (objId == 401113)
	{
		GameObject *gymNodMinigunner1Obj = Commands->Find_Object(401115);
		Commands->Send_Custom_Event(obj, gymNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *gymNodMinigunner2Obj = Commands->Find_Object(401116);
		Commands->Send_Custom_Event(obj, gymNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *gymNodMinigunner3Obj = Commands->Find_Object(401117);
		Commands->Send_Custom_Event(obj, gymNodMinigunner3Obj, 0, 0, 0.0f);

		GameObject *forrestTowerMinigunner1Obj = Commands->Find_Object(401118);
		Commands->Send_Custom_Event(obj, forrestTowerMinigunner1Obj, 0, 0, 0.0f);

		GameObject *forrestMinigunner1Obj = Commands->Find_Object(401119);
		Commands->Send_Custom_Event(obj, forrestMinigunner1Obj, 0, 0, 0.0f);

		GameObject *forrestMinigunner2Obj = Commands->Find_Object(401120);
		Commands->Send_Custom_Event(obj, forrestMinigunner2Obj, 0, 0, 0.0f);

		GameObject *forrestMinigunner3Obj = Commands->Find_Object(401121);
		Commands->Send_Custom_Event(obj, forrestMinigunner3Obj, 0, 0, 0.0f);

		GameObject *forrestTowerMinigunner2Obj = Commands->Find_Object(401122);
		Commands->Send_Custom_Event(obj, forrestTowerMinigunner2Obj, 0, 0, 0.0f);

		GameObject *stairsScriptZoneObj = Commands->Find_Object(401114);
		if (stairsScriptZoneObj)
		{
				Commands->Destroy_Object(stairsScriptZoneObj);
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 401054)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 114, 8, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 102, 8, 0.0f);

		Commands->Destroy_Object(obj);
	}
	else if (objId == 401079)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 14, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 15, 0.0f);

		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1064, pos, 1000.0f);

		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 218, 0, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 219, 0, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 220, 0, 0.0f);

		GameObject *tibCavernNodOfficerObj = Commands->Find_Object(401089);
		Commands->Send_Custom_Event(obj, tibCavernNodOfficerObj, 0, 0, 0.0f);

		GameObject *tibCavernNodChemtrooper1Obj = Commands->Find_Object(401087);
		Commands->Send_Custom_Event(obj, tibCavernNodChemtrooper1Obj, 0, 0, 0.0f);

		GameObject *tibCavernNodChemtrooper2Obj = Commands->Find_Object(401086);
		Commands->Send_Custom_Event(obj, tibCavernNodChemtrooper2Obj, 0, 0, 0.0f);

		GameObject *tibCavernNodChemtrooper3Obj = Commands->Find_Object(401088);
		Commands->Send_Custom_Event(obj, tibCavernNodChemtrooper3Obj, 0, 0, 0.0f);

		GameObject *tibCavernNodChemtrooper4Obj = Commands->Find_Object(401085);
		Commands->Send_Custom_Event(obj, tibCavernNodChemtrooper4Obj, 0, 0, 0.0f);

		GameObject *tibCavernNodGunEmplacement1Obj = Commands->Find_Object(401090);
		Commands->Send_Custom_Event(obj, tibCavernNodGunEmplacement1Obj, 0, 0, 0.0f);

		GameObject *tibCavernNodGunEmplacement2Obj = Commands->Find_Object(401091);
		Commands->Send_Custom_Event(obj, tibCavernNodGunEmplacement2Obj, 0, 0, 0.0f);

		GameObject *caveEndNodGunEmplacementObj = Commands->Find_Object(401092);
		Commands->Send_Custom_Event(obj, caveEndNodGunEmplacementObj, 0, 0, 0.0f);

		GameObject *caveEndNodMinigunner1Obj = Commands->Find_Object(401094);
		Commands->Send_Custom_Event(obj, caveEndNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *caveEndNodMinigunner2Obj = Commands->Find_Object(401095);
		Commands->Send_Custom_Event(obj, caveEndNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *caveEndNodMinigunner3Obj = Commands->Find_Object(401096);
		Commands->Send_Custom_Event(obj, caveEndNodMinigunner3Obj, 0, 0, 0.0f);

		GameObject *caveEndNodRocketTrooperObj = Commands->Find_Object(401097);
		Commands->Send_Custom_Event(obj, caveEndNodRocketTrooperObj, 0, 0, 0.0f);

		GameObject *tibCavernNodRocketTrooperObj = Commands->Find_Object(401189);
		Commands->Send_Custom_Event(obj, tibCavernNodRocketTrooperObj, 0, 0, 0.0f);

		GameObject *tibCavernNodChemtrooper5Obj = Commands->Find_Object(401190);
		Commands->Send_Custom_Event(obj, tibCavernNodChemtrooper5Obj, 0, 0, 0.0f);

		GameObject *tibCavernNodChemtrooper6Obj = Commands->Find_Object(401191);
		Commands->Send_Custom_Event(obj, tibCavernNodChemtrooper6Obj, 0, 0, 0.0f);

		GameObject *tibCavernEntranceScriptZoneObj = Commands->Find_Object(401080);
		if (tibCavernEntranceScriptZoneObj)
		{
			Commands->Destroy_Object(tibCavernEntranceScriptZoneObj);
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 401101)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 204, 0, 0.0f);

		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1065, pos, 1000.0f);

		GameObject *honTowerNodMinigunnerObj = Commands->Find_Object(401103);
		Commands->Send_Custom_Event(obj, honTowerNodMinigunnerObj, 0, 0, 0.0f);

		GameObject *caveExitNodMinigunnerObj = Commands->Find_Object(401104);
		Commands->Send_Custom_Event(obj, caveExitNodMinigunnerObj, 0, 0, 0.0f);

		GameObject *bunkBedsNodMinigunner1Obj = Commands->Find_Object(401105);
		Commands->Send_Custom_Event(obj, bunkBedsNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *bunkBedsNodMinigunner2Obj = Commands->Find_Object(401106);
		Commands->Send_Custom_Event(obj, bunkBedsNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *messHallNodMinigunner1Obj = Commands->Find_Object(401107);
		Commands->Send_Custom_Event(obj, messHallNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *messHallNodMinigunner2Obj = Commands->Find_Object(401108);
		Commands->Send_Custom_Event(obj, messHallNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *messHallNodMinigunner3Obj = Commands->Find_Object(401109);
		Commands->Send_Custom_Event(obj, messHallNodMinigunner3Obj, 0, 0, 0.0f);

		GameObject *messHallNodCookObj = Commands->Find_Object(401110);
		Commands->Send_Custom_Event(obj, messHallNodCookObj, 0, 0, 0.0f);

		GameObject *honTowerNodRocketTrooperObj = Commands->Find_Object(401112);
		Commands->Send_Custom_Event(obj, honTowerNodRocketTrooperObj, 0, 0, 0.0f);

		GameObject *caveExitScriptZoneObj = Commands->Find_Object(401102);
		if (caveExitScriptZoneObj)
		{
			Commands->Destroy_Object(caveExitScriptZoneObj);
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 401102)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 204, 0, 0.0f);
		
		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1065, pos, 1000.0f);

		GameObject *honTowerNodMinigunnerObj = Commands->Find_Object(401103);
		Commands->Send_Custom_Event(obj, honTowerNodMinigunnerObj, 0, 0, 0.0f);

		GameObject *caveExitNodMinigunnerObj = Commands->Find_Object(401104);
		Commands->Send_Custom_Event(obj, caveExitNodMinigunnerObj, 0, 0, 0.0f);

		GameObject *bunkBedsNodMinigunner1Obj = Commands->Find_Object(401105);
		Commands->Send_Custom_Event(obj, bunkBedsNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *bunkBedsNodMinigunner2Obj = Commands->Find_Object(401106);
		Commands->Send_Custom_Event(obj, bunkBedsNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *messHallNodMinigunner1Obj = Commands->Find_Object(401107);
		Commands->Send_Custom_Event(obj, messHallNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *messHallNodMinigunner2Obj = Commands->Find_Object(401108);
		Commands->Send_Custom_Event(obj, messHallNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *messHallNodMinigunner3Obj = Commands->Find_Object(401109);
		Commands->Send_Custom_Event(obj, messHallNodMinigunner3Obj, 0, 0, 0.0f);

		GameObject *messHallNodCookObj = Commands->Find_Object(401110);
		Commands->Send_Custom_Event(obj, messHallNodCookObj, 0, 0, 0.0f);

		GameObject *honTowerNodRocketTrooperObj = Commands->Find_Object(401112);
		Commands->Send_Custom_Event(obj, honTowerNodRocketTrooperObj, 0, 0, 0.0f);

		GameObject *caveExitScriptZoneObj = Commands->Find_Object(401101);
		if (caveExitScriptZoneObj)
		{
			Commands->Destroy_Object(caveExitScriptZoneObj);
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 401080)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 14, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 15, 0.0f);

		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1064, pos, 1000.0f);

		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 218, 0, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 219, 0, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 220, 0, 0.0f);

		GameObject *tibCavernNodOfficerObj = Commands->Find_Object(401089);
		Commands->Send_Custom_Event(obj, tibCavernNodOfficerObj, 0, 0, 0.0f);

		GameObject *tibCavernNodChemtrooper1Obj = Commands->Find_Object(401087);
		Commands->Send_Custom_Event(obj, tibCavernNodChemtrooper1Obj, 0, 0, 0.0f);

		GameObject *tibCavernNodChemtrooper2Obj = Commands->Find_Object(401086);
		Commands->Send_Custom_Event(obj, tibCavernNodChemtrooper2Obj, 0, 0, 0.0f);

		GameObject *tibCavernNodChemtrooper3Obj = Commands->Find_Object(401088);
		Commands->Send_Custom_Event(obj, tibCavernNodChemtrooper3Obj, 0, 0, 0.0f);

		GameObject *tibCavernNodChemtrooper4Obj = Commands->Find_Object(401085);
		Commands->Send_Custom_Event(obj, tibCavernNodChemtrooper4Obj, 0, 0, 0.0f);

		GameObject *tibCavernNodGunEmplacement1Obj = Commands->Find_Object(401090);
		Commands->Send_Custom_Event(obj, tibCavernNodGunEmplacement1Obj, 0, 0, 0.0f);

		GameObject *tibCavernNodGunEmplacement2Obj = Commands->Find_Object(401091);
		Commands->Send_Custom_Event(obj, tibCavernNodGunEmplacement2Obj, 0, 0, 0.0f);

		GameObject *caveEndNodGunEmplacementObj = Commands->Find_Object(401092);
		Commands->Send_Custom_Event(obj, caveEndNodGunEmplacementObj, 0, 0, 0.0f);

		GameObject *caveEndNodMinigunner1Obj = Commands->Find_Object(401094);
		Commands->Send_Custom_Event(obj, caveEndNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *caveEndNodMinigunner2Obj = Commands->Find_Object(401095);
		Commands->Send_Custom_Event(obj, caveEndNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *caveEndNodMinigunner3Obj = Commands->Find_Object(401096);
		Commands->Send_Custom_Event(obj, caveEndNodMinigunner3Obj, 0, 0, 0.0f);

		GameObject *caveEndNodRocketTrooperObj = Commands->Find_Object(401097);
		Commands->Send_Custom_Event(obj, caveEndNodRocketTrooperObj, 0, 0, 0.0f);

		GameObject *tibCavernNodRocketTrooperObj = Commands->Find_Object(401189);
		Commands->Send_Custom_Event(obj, tibCavernNodRocketTrooperObj, 0, 0, 0.0f);

		GameObject *tibCavernNodChemtrooper5Obj = Commands->Find_Object(401190);
		Commands->Send_Custom_Event(obj, tibCavernNodChemtrooper5Obj, 0, 0, 0.0f);

		GameObject *tibCavernNodChemtrooper6Obj = Commands->Find_Object(401191);
		Commands->Send_Custom_Event(obj, tibCavernNodChemtrooper6Obj, 0, 0, 0.0f);

		GameObject *caveExitScriptZoneObj = Commands->Find_Object(401079);
		if (caveExitScriptZoneObj)
		{
			Commands->Destroy_Object(caveExitScriptZoneObj);
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 401070)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 11, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 14, 0.0f);
		
		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1061, pos, 1000.0f);

		GameObject *caveNodMinigunner1Obj = Commands->Find_Object(401071);
		Commands->Send_Custom_Event(obj, caveNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *caveNodMinigunner2Obj = Commands->Find_Object(401072);
		Commands->Send_Custom_Event(obj, caveNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *caveNodMinigunner3Obj = Commands->Find_Object(401073);
		Commands->Send_Custom_Event(obj, caveNodMinigunner3Obj, 0, 0, 0.0f);

		GameObject *caveNodMinigunner4Obj = Commands->Find_Object(401074);
		Commands->Send_Custom_Event(obj, caveNodMinigunner4Obj, 0, 0, 0.0f);

		GameObject *caveNodOfficerObj = Commands->Find_Object(401075);
		Commands->Send_Custom_Event(obj, caveNodOfficerObj, 0, 0, 0.0f);

		GameObject *caveNodRocketTrooperObj = Commands->Find_Object(401076);
		Commands->Send_Custom_Event(obj, caveNodRocketTrooperObj, 0, 0, 0.0f);

		GameObject *caveNodMinigunner5Obj = Commands->Find_Object(401077);
		Commands->Send_Custom_Event(obj, caveNodMinigunner5Obj, 0, 0, 0.0f);

		GameObject *caveNodMinigunner6Obj = Commands->Find_Object(401078);
		Commands->Send_Custom_Event(obj, caveNodMinigunner6Obj, 0, 0, 0.0f);

		GameObject *caveNodMinigunner7Obj = Commands->Find_Object(401192);
		Commands->Send_Custom_Event(obj, caveNodMinigunner7Obj, 0, 0, 0.0f);

		GameObject *caveNodChemTrooperObj = Commands->Find_Object(401193);
		Commands->Send_Custom_Event(obj, caveNodChemTrooperObj, 0, 0, 0.0f);

		GameObject *caveNodMinigunner8Obj = Commands->Find_Object(401194);
		Commands->Send_Custom_Event(obj, caveNodMinigunner8Obj, 0, 0, 0.0f);

		GameObject *caveNodMinigunner9Obj = Commands->Find_Object(401195);
		Commands->Send_Custom_Event(obj, caveNodMinigunner9Obj, 0, 0, 0.0f);
		
		GameObject *caveEntranceScriptZoneObj = Commands->Find_Object(401196);
		if (caveEntranceScriptZoneObj)
		{
			Commands->Destroy_Object(caveEntranceScriptZoneObj);
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 401066)
	{
		GameObject *obbyMCTNodEngineerObj = Commands->Find_Object(400200);
		Commands->Send_Custom_Event(obj, obbyMCTNodEngineerObj, 0, 0, 0.0f);

		GameObject *obbyNodMinigunner1Obj = Commands->Find_Object(401067);
		Commands->Send_Custom_Event(obj, obbyNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *obbyNodMinigunner2Obj = Commands->Find_Object(401068);
		Commands->Send_Custom_Event(obj, obbyNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *obbyNodMinigunner3Obj = Commands->Find_Object(401069);
		Commands->Send_Custom_Event(obj, obbyNodMinigunner3Obj, 0, 0, 0.0f);

		GameObject *nodObbyBuildingObj = Commands->Find_Object(1153845);
		if (nodObbyBuildingObj)
		{
			Commands->Destroy_Object(nodObbyBuildingObj);
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 400991)
	{
		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
		if (invisObj)
		{
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X2F_OrcaSam.txt");
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 401001)
	{
		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
		if (invisObj)
		{
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X2H_ApacheSpray.txt");
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 401029)
	{
		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
		if (invisObj)
		{
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X2D_MTank_Slip.txt");
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 400992)
	{
		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
		if (invisObj)
		{
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X2G_AirCrash.txt");
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 400989)
	{
		Commands->Destroy_Object(obj);
	}
	else if (objId == 400990)
	{
		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
		if (invisObj)
		{
			Commands->Attach_Script(invisObj, "Test_Cinematic", "X2E_C130Drop.txt");
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 401196)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 11, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 14, 0.0f);
		
		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1061, pos, 1000.0f);

		GameObject *caveNodMinigunner1Obj = Commands->Find_Object(401071);
		Commands->Send_Custom_Event(obj, caveNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *caveNodMinigunner2Obj = Commands->Find_Object(401072);
		Commands->Send_Custom_Event(obj, caveNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *caveNodMinigunner3Obj = Commands->Find_Object(401073);
		Commands->Send_Custom_Event(obj, caveNodMinigunner3Obj, 0, 0, 0.0f);

		GameObject *caveNodMinigunner4Obj = Commands->Find_Object(401074);
		Commands->Send_Custom_Event(obj, caveNodMinigunner4Obj, 0, 0, 0.0f);

		GameObject *caveNodOfficerObj = Commands->Find_Object(401075);
		Commands->Send_Custom_Event(obj, caveNodOfficerObj, 0, 0, 0.0f);

		GameObject *caveNodRocketTrooperObj = Commands->Find_Object(401076);
		Commands->Send_Custom_Event(obj, caveNodRocketTrooperObj, 0, 0, 0.0f);

		GameObject *caveNodMinigunner5Obj = Commands->Find_Object(401077);
		Commands->Send_Custom_Event(obj, caveNodMinigunner5Obj, 0, 0, 0.0f);

		GameObject *caveNodMinigunner6Obj = Commands->Find_Object(401078);
		Commands->Send_Custom_Event(obj, caveNodMinigunner6Obj, 0, 0, 0.0f);

		GameObject *caveNodMinigunner7Obj = Commands->Find_Object(401192);
		Commands->Send_Custom_Event(obj, caveNodMinigunner7Obj, 0, 0, 0.0f);

		GameObject *caveNodChemTrooperObj = Commands->Find_Object(401193);
		Commands->Send_Custom_Event(obj, caveNodChemTrooperObj, 0, 0, 0.0f);

		GameObject *caveNodMinigunner8Obj = Commands->Find_Object(401194);
		Commands->Send_Custom_Event(obj, caveNodMinigunner8Obj, 0, 0, 0.0f);

		GameObject *caveNodMinigunner9Obj = Commands->Find_Object(401195);
		Commands->Send_Custom_Event(obj, caveNodMinigunner9Obj, 0, 0, 0.0f);

		GameObject *caveEntranceScriptZoneObj = Commands->Find_Object(401070);
		if (caveEntranceScriptZoneObj)
		{
			Commands->Destroy_Object(caveEntranceScriptZoneObj);
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 401130)
	{
		GameObject *nodHONBuildingObj = Commands->Find_Object(1157690);
		if (nodHONBuildingObj)
		{
			if (Commands->Get_Health(nodHONBuildingObj) != 0.0f)
			{
				Commands->Stop_All_Conversations();

				int conversationId = Commands->Create_Conversation("M02_SECONDARY_01_GUIDE", 100, 300.0f, true); // The easiest way to destroy the Hand of Nod is to eliminate its Master Control Terminal.
				Commands->Join_Conversation(NULL, conversationId, true, true, true);

				Vector3 pos = Commands->Get_Position(obj);
				GameObject *starObj = Commands->Get_A_Star(pos);

				Commands->Join_Conversation(starObj, conversationId, true, false, false);
				Commands->Start_Conversation(conversationId, 0);
			}
		}

		GameObject *windowedHallNodTechnician1Obj = Commands->Find_Object(401132);
		Commands->Send_Custom_Event(obj, windowedHallNodTechnician1Obj, 0, 0, 0.0f);

		GameObject *windowedHallNodTechnician2Obj = Commands->Find_Object(401133);
		Commands->Send_Custom_Event(obj, windowedHallNodTechnician2Obj, 0, 0, 0.0f);

		GameObject *windowedHallNodMinigunner1Obj = Commands->Find_Object(401134);
		Commands->Send_Custom_Event(obj, windowedHallNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *windowedHallNodMinigunner2Obj = Commands->Find_Object(401135);
		Commands->Send_Custom_Event(obj, windowedHallNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *windowedHallNodMinigunner3Obj = Commands->Find_Object(401136);
		Commands->Send_Custom_Event(obj, windowedHallNodMinigunner3Obj, 0, 0, 0.0f);

		GameObject *controlRoomNodMinigunner1Obj = Commands->Find_Object(401137);
		Commands->Send_Custom_Event(obj, controlRoomNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *controlRoomNodMinigunner2Obj = Commands->Find_Object(401138);
		Commands->Send_Custom_Event(obj, controlRoomNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *controlRoomNodOfficerObj = Commands->Find_Object(401139);
		Commands->Send_Custom_Event(obj, controlRoomNodOfficerObj, 0, 0, 0.0f);

		GameObject *honWindowedHallEntranceScriptZoneObj = Commands->Find_Object(401131);
		if (honWindowedHallEntranceScriptZoneObj)
		{
			Commands->Destroy_Object(honWindowedHallEntranceScriptZoneObj);
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 401186)
	{
		Create_Apache(15);

		Commands->Stop_All_Conversations();

		// Looks like some sort of meteor. Made one heck of a hole in the ceiling.
		// EVA indicates it has been here for some time, and was only recently unearthed. Better stay back, it's loaded with Tiberium.
		int conversationId = Commands->Create_Conversation("M02_SECONDARY_14_GUIDE", 100, 300.0f, true);
		Commands->Join_Conversation(NULL, conversationId, true, true, true);
		
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, false, false);
		Commands->Start_Conversation(conversationId, 0);

		GameObject *tibCaveEntranceScriptZoneObj = Commands->Find_Object(401187);
		if (tibCaveEntranceScriptZoneObj)
		{
			Commands->Destroy_Object(tibCaveEntranceScriptZoneObj);
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 401187)
	{
		Create_Apache(15);

		Commands->Stop_All_Conversations();

		// Looks like some sort of meteor. Made one heck of a hole in the ceiling.
		// EVA indicates it has been here for some time, and was only recently unearthed. Better stay back, it's loaded with Tiberium.
		int conversationId = Commands->Create_Conversation("M02_SECONDARY_14_GUIDE", 100, 300.0f, true);
		Commands->Join_Conversation(NULL, conversationId, true, true, true);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, false, false);
		Commands->Start_Conversation(conversationId, 0);

		GameObject *tibCaveEntranceScriptZoneObj = Commands->Find_Object(401186);
		if (tibCaveEntranceScriptZoneObj)
		{
			Commands->Destroy_Object(tibCaveEntranceScriptZoneObj);
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 401131)
	{
		GameObject *nodHONBuildingObj = Commands->Find_Object(1157690);
		if (nodHONBuildingObj)
		{
			if (Commands->Get_Health(nodHONBuildingObj) != 0.0f)
			{
				Commands->Stop_All_Conversations();

				int conversationId = Commands->Create_Conversation("M02_SECONDARY_01_GUIDE", 100, 300.0f, true); // The easiest way to destroy the Hand of Nod is to eliminate its Master Control Terminal.
				Commands->Join_Conversation(NULL, conversationId, true, true, true);

				Vector3 pos = Commands->Get_Position(obj);
				GameObject *starObj = Commands->Get_A_Star(pos);

				Commands->Join_Conversation(starObj, conversationId, true, false, false);
				Commands->Start_Conversation(conversationId, 0);
			}
		}

		GameObject *windowedHallNodTechnician1Obj = Commands->Find_Object(401132);
		Commands->Send_Custom_Event(obj, windowedHallNodTechnician1Obj, 0, 0, 0.0f);

		GameObject *windowedHallNodTechnician2Obj = Commands->Find_Object(401133);
		Commands->Send_Custom_Event(obj, windowedHallNodTechnician2Obj, 0, 0, 0.0f);

		GameObject *windowedHallNodMinigunner1Obj = Commands->Find_Object(401134);
		Commands->Send_Custom_Event(obj, windowedHallNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *windowedHallNodMinigunner2Obj = Commands->Find_Object(401135);
		Commands->Send_Custom_Event(obj, windowedHallNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *windowedHallNodMinigunner3Obj = Commands->Find_Object(401136);
		Commands->Send_Custom_Event(obj, windowedHallNodMinigunner3Obj, 0, 0, 0.0f);

		GameObject *controlRoomNodMinigunner1Obj = Commands->Find_Object(401137);
		Commands->Send_Custom_Event(obj, controlRoomNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *controlRoomNodMinigunner2Obj = Commands->Find_Object(401138);
		Commands->Send_Custom_Event(obj, controlRoomNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *controlRoomNodOfficerObj = Commands->Find_Object(401139);
		Commands->Send_Custom_Event(obj, controlRoomNodOfficerObj, 0, 0, 0.0f);

		GameObject *honWindowedHallEntranceScriptZoneObj = Commands->Find_Object(401130);
		if (honWindowedHallEntranceScriptZoneObj)
		{
			Commands->Destroy_Object(honWindowedHallEntranceScriptZoneObj);
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 401123)
	{
		GameObject *honOfficeNodMinigunner1Obj = Commands->Find_Object(401124);
		Commands->Send_Custom_Event(obj, honOfficeNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *honOfficeNodMinigunner2Obj = Commands->Find_Object(401125);
		Commands->Send_Custom_Event(obj, honOfficeNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *honOfficeNodMinigunner3Obj = Commands->Find_Object(401126);
		Commands->Send_Custom_Event(obj, honOfficeNodMinigunner3Obj, 0, 0, 0.0f);

		GameObject *honOfficeNodMinigunner4Obj = Commands->Find_Object(401127);
		Commands->Send_Custom_Event(obj, honOfficeNodMinigunner4Obj, 0, 0, 0.0f);

		GameObject *nonExistingObj = Commands->Find_Object(401128); // Does not exist
		Commands->Send_Custom_Event(obj, nonExistingObj, 0, 0, 0.0f);

		GameObject *honOfficeNodTechnicianObj = Commands->Find_Object(401129);
		Commands->Send_Custom_Event(obj, honOfficeNodTechnicianObj, 0, 0, 0.0f);

		Commands->Destroy_Object(obj);
	}
	else if (objId == 401114)
	{
		GameObject *gymNodMinigunner1Obj = Commands->Find_Object(401115);
		Commands->Send_Custom_Event(obj, gymNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *gymNodMinigunner2Obj = Commands->Find_Object(401116);
		Commands->Send_Custom_Event(obj, gymNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *gymNodMinigunner3Obj = Commands->Find_Object(401117);
		Commands->Send_Custom_Event(obj, gymNodMinigunner3Obj, 0, 0, 0.0f);

		GameObject *forrestTowerMinigunner1Obj = Commands->Find_Object(401118);
		Commands->Send_Custom_Event(obj, forrestTowerMinigunner1Obj, 0, 0, 0.0f);

		GameObject *forrestMinigunner1Obj = Commands->Find_Object(401119);
		Commands->Send_Custom_Event(obj, forrestMinigunner1Obj, 0, 0, 0.0f);

		GameObject *forrestMinigunner2Obj = Commands->Find_Object(401120);
		Commands->Send_Custom_Event(obj, forrestMinigunner2Obj, 0, 0, 0.0f);

		GameObject *forrestMinigunner3Obj = Commands->Find_Object(401121);
		Commands->Send_Custom_Event(obj, forrestMinigunner3Obj, 0, 0, 0.0f);

		GameObject *forrestTowerMinigunner2Obj = Commands->Find_Object(401122);
		Commands->Send_Custom_Event(obj, forrestTowerMinigunner2Obj, 0, 0, 0.0f);

		GameObject *stairsScriptZoneObj = Commands->Find_Object(401113);
		if (stairsScriptZoneObj)
		{
			Commands->Destroy_Object(stairsScriptZoneObj);
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 405118)
	{
		Commands->Destroy_Object(obj);
	}
	else if (objId == 405120)
	{
		GameObject *mammothTankObj = Commands->Create_Object("GDI_Mammoth_Tank_Player", Vector3(570.06f, 261.47f, -55.0f));
		if (mammothTankObj)
		{
			Commands->Set_Facing(mammothTankObj, 45.0);
			Commands->Attach_Script(mammothTankObj, "M02_Player_Vehicle", "2");
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 405122)
	{
		Commands->Stop_All_Conversations();
		
		int conversationId = Commands->Create_Conversation("M02_SECONDARY_11_START", 100, 300.0f, true); // This bridge is flanked by SAM Sites. We're not getting anywhere near that area until they're gone.
		Commands->Join_Conversation(NULL, conversationId, true, true, true);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, false, false);
		Commands->Start_Conversation(conversationId, 0);

		Commands->Start_Timer(obj, this, 6.0f, 12);

		Commands->Destroy_Object(obj);
	}
	else if (objId == 405119)
	{
		Commands->Stop_All_Conversations();

		int conversationId = Commands->Create_Conversation("M02_SECONDARY_03_START", 100, 300.0f, true); // Looks like Nod has an officer calling in reinforcements in that guard tower. Eliminate all resistance and occupy the tower.
		Commands->Join_Conversation(NULL, conversationId, true, true, true);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, false, false);
		Commands->Start_Conversation(conversationId, 0);

		Commands->Destroy_Object(obj);
	}
	else if (objId == 405116)
	{
		Commands->Stop_All_Conversations();

		// Warning, Approaching Nod Obelisk. Proceed with caution.
		// You heard her right, Havoc, stay as far away from that tower as you can. It's called an Obelisk of Light, and it's deadly against ground units. If you can't destroy it from the outside, try shutting off its power supply.
		int conversationId = Commands->Create_Conversation("M02_OBELISK_APPROACH", 100, 300.0f, true); 

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, false, false);
		Commands->Start_Conversation(conversationId, 0);

		Commands->Destroy_Object(obj);
	}
	else if (objId == 401982)
	{
		Commands->Stop_All_Conversations();

		// That building across from the dam is a Nod Power Plant, which supplies power to the other buildings in the area. Your work will be a lot easier if you destroy it.
		// How am I supposed to get over there?
		// Havoc - EVA SATCOM indicates the Power Plant has elevator access from the base of the dam.
		// That's the info I needed, Locke. Thanks.
		int conversationId = Commands->Create_Conversation("M02_SECONDARY_13_START", 100, 300.0f, true);
		Commands->Join_Conversation(NULL, conversationId, true, true, true);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, false, false);
		Commands->Start_Conversation(conversationId, 0);

		GameObject *nodJetObj = Commands->Create_Object("Nod_Jet", Vector3(929.91f, 1132.34f, 70.5f));
		if (nodJetObj)
		{
			Commands->Attach_Script(nodJetObj, "M02_Nod_Jet", "");
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 405117)
	{
		Commands->Stop_All_Conversations();

		// Don't forget about that Nod Helipad. If it's intact the helicopters can refuel there.
		// You make a good back seat driver, Locke.
		int conversationId = Commands->Create_Conversation("M02_PRIMARY_02_REMIND", 100, 300.0f, true);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, false, false);
		Commands->Start_Conversation(conversationId, 0);

		Commands->Destroy_Object(obj);
	}
	else if (objId == 400196)
	{
		this->canSendCustomAndDestroy = true;

		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1000, pos, 70.0f);

		Commands->Start_Timer(obj, this, 1.0f, 4);

		Commands->Destroy_Object(obj);
	}
	else if (objId == 400270)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 9, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 8, 0.0f);

		GameObject *damNodOfficerObj = Commands->Find_Object(400275);
		Commands->Send_Custom_Event(obj, damNodOfficerObj, 0, 0, 0.0f);

		GameObject *damNodMinigunner1Obj = Commands->Find_Object(401046);
		Commands->Send_Custom_Event(obj, damNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *damNodMinigunner2Obj = Commands->Find_Object(401047);
		Commands->Send_Custom_Event(obj, damNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *nonExistingObj = Commands->Find_Object(401048); // Does not exist
		Commands->Send_Custom_Event(obj, nonExistingObj, 0, 0, 0.0f);

		GameObject *damNodMinigunner3Obj = Commands->Find_Object(401049);
		Commands->Send_Custom_Event(obj, damNodMinigunner3Obj, 0, 0, 0.0f);

		GameObject *damNodMinigunner4Obj = Commands->Find_Object(401050);
		Commands->Send_Custom_Event(obj, damNodMinigunner4Obj, 0, 0, 0.0f);

		GameObject *damNodMinigunner5Obj = Commands->Find_Object(401051);
		Commands->Send_Custom_Event(obj, damNodMinigunner5Obj, 0, 0, 0.0f);

		GameObject *damNodMinigunner6Obj = Commands->Find_Object(401052);
		Commands->Send_Custom_Event(obj, damNodMinigunner6Obj, 0, 0, 0.0f);

		GameObject *damNodMinigunner7Obj = Commands->Find_Object(401053);
		Commands->Send_Custom_Event(obj, damNodMinigunner7Obj, 0, 0, 0.0f);

		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1059, pos, 1000.0f);

		GameObject *damElevatorScriptZoneObj = Commands->Find_Object(400271);
		if (damElevatorScriptZoneObj)
		{
			Commands->Destroy_Object(damElevatorScriptZoneObj);
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 400201)
	{
		this->canSendCustomAndDestroy = true;

		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1000, pos, 70.0f);

		Commands->Start_Timer(obj, this, 1.0f, 7);
		
		Commands->Destroy_Object(obj);
	}
	else if (objId == 400197)
	{
		this->canSendCustomAndDestroy = true;

		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1000, pos, 70.0f);

		Commands->Start_Timer(obj, this, 1.0f, 5);

		Commands->Destroy_Object(obj);
	}
	else if (objId == 400198)
	{
		this->canSendCustomAndDestroy = true;
		
		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1000, pos, 70.0f);

		Commands->Start_Timer(obj, this, 1.0f, 6);

		Commands->Destroy_Object(obj);
	}
	else if (objId == 400267)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 211, 0, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 3, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 4, 0.0f);

		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1053, pos, 1000.0f);
		Commands->Create_Logical_Sound(obj, 1102, pos, 1000.0f);

		GameObject *skiResortNodOfficerObj = Commands->Find_Object(400210);
		Commands->Send_Custom_Event(obj, skiResortNodOfficerObj, 0, 0, 0.0f);

		GameObject *skiResortNodMinigunner1Obj = Commands->Find_Object(400451);
		Commands->Send_Custom_Event(obj, skiResortNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *nonExisting1Obj = Commands->Find_Object(400452); // Does not exist
		Commands->Send_Custom_Event(obj, nonExisting1Obj, 0, 0, 0.0f);

		GameObject *skiResortNodMinigunner2Obj = Commands->Find_Object(400453);
		Commands->Send_Custom_Event(obj, skiResortNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *skiResortNodGunEmplacementObj = Commands->Find_Object(400499);
		Commands->Send_Custom_Event(obj, skiResortNodGunEmplacementObj, 0, 0, 0.0f);

		GameObject *nonExisting2Obj = Commands->Find_Object(400500); // Does not exist
		Commands->Send_Custom_Event(obj, nonExisting2Obj, 0, 0, 0.0f);

		GameObject *nonExisting3Obj = Commands->Find_Object(401025); // Does not exist
		Commands->Send_Custom_Event(obj, nonExisting3Obj, 0, 0, 0.0f);

		GameObject *skiResortNodLightTank1Obj = Commands->Find_Object(401026);
		Commands->Send_Custom_Event(obj, skiResortNodLightTank1Obj, 0, 0, 0.0f);

		GameObject *skiResortNodLightTank2Obj = Commands->Find_Object(401027);
		Commands->Send_Custom_Event(obj, skiResortNodLightTank2Obj, 0, 0, 0.0f);

		GameObject *skiResortGDIMammothTankObj = Commands->Find_Object(401028);
		Commands->Send_Custom_Event(obj, skiResortGDIMammothTankObj, 0, 0, 0.0f);

		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 112, 4, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 111, 4, 0.0f);

		Create_Buggy(4);

		GameObject *skiResortGDIMedTankObj = Commands->Find_Object(401018);
		Commands->Send_Custom_Event(obj, skiResortGDIMedTankObj, 0, 0, 0.0f);

		Commands->Start_Timer(obj, this, 0.1f, 10);

		GameObject *skiResortEntranceScriptZoneObj = Commands->Find_Object(400187);
		if (skiResortEntranceScriptZoneObj)
		{
			Commands->Destroy_Object(skiResortEntranceScriptZoneObj);
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 400269)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 202, 0, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 203, 0, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 217, 0, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 6, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 9, 0.0f);

		GameObject *outsideObbyNodOfficerObj = Commands->Find_Object(400212);
		Commands->Send_Custom_Event(obj, outsideObbyNodOfficerObj, 0, 0, 0.0f);

		GameObject *outsideObbyNodMinigunner1Obj = Commands->Find_Object(400981);
		Commands->Send_Custom_Event(obj, outsideObbyNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *outsideObbyNodMinigunner2Obj = Commands->Find_Object(400982);
		Commands->Send_Custom_Event(obj, outsideObbyNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *damElevatorNodMinigunnerObj = Commands->Find_Object(400983);
		Commands->Send_Custom_Event(obj, damElevatorNodMinigunnerObj, 0, 0, 0.0f);

		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1056, pos, 1000.0f);

		Call_GDI_Soldiers(9);

		Commands->Stop_All_Conversations();

		// SATCOM shows you are approaching the base facility. You'll need to find an alternate way inside, I don't see a main entrance.
		// Goes against my instinct, Locke, but I'll try it your way.
		int conversationId = Commands->Create_Conversation("M02_PRIMARY_01_GUIDE", 100, 300.0f, true);
		Commands->Join_Conversation(NULL, conversationId, true, true, true);

		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, false, false);
		Commands->Start_Conversation(conversationId, 0);

		Commands->Destroy_Object(obj);
	}
	else if (objId == 400268)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 4, 0.0f);
		
		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1054, pos, 1000.0f);

		GameObject *caveBridgeNodMinigunner1Obj = Commands->Find_Object(400552);
		Commands->Send_Custom_Event(obj, caveBridgeNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *caveBridgeNodMinigunner2Obj = Commands->Find_Object(400553);
		Commands->Send_Custom_Event(obj, caveBridgeNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *caveBridgeNodRocketTrooperObj = Commands->Find_Object(400560);
		Commands->Send_Custom_Event(obj, caveBridgeNodRocketTrooperObj, 0, 0, 0.0f);

		GameObject *caveBridgeNodMinigunner3Obj = Commands->Find_Object(400555);
		Commands->Send_Custom_Event(obj, caveBridgeNodMinigunner3Obj, 0, 0, 0.0f);

		GameObject *caveBridgeNodMinigunner4Obj = Commands->Find_Object(400558);
		Commands->Send_Custom_Event(obj, caveBridgeNodMinigunner4Obj, 0, 0, 0.0f);

		GameObject *caveBridgeNodMinigunner5Obj = Commands->Find_Object(400559);
		Commands->Send_Custom_Event(obj, caveBridgeNodMinigunner5Obj, 0, 0, 0.0f);

		GameObject *caveBridgeNodMinigunner6Obj = Commands->Find_Object(400557);
		Commands->Send_Custom_Event(obj, caveBridgeNodMinigunner6Obj, 0, 0, 0.0f);

		GameObject *caveBridgeNodMinigunner7Obj = Commands->Find_Object(400554);
		Commands->Send_Custom_Event(obj, caveBridgeNodMinigunner7Obj, 0, 0, 0.0f);

		GameObject *caveBridgeNodMinigunner8Obj = Commands->Find_Object(400556);
		Commands->Send_Custom_Event(obj, caveBridgeNodMinigunner8Obj, 0, 0, 0.0f);

		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 24, 0.0f);

		Commands->Destroy_Object(obj);
	}
	else if (objId == 400274)
	{
		GameObject *nodPowerPlantBuildingObj = Commands->Find_Object(1153846);
		if (nodPowerPlantBuildingObj)
		{
			if (Commands->Get_Health(nodPowerPlantBuildingObj) <= 0.0f)
			{
				Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 11, 0.0f);

				Vector3 pos = Commands->Get_Position(obj);
				Commands->Create_Logical_Sound(obj, 1061, pos, 1000.0f);

				Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 114, 10, 0.0f);
				Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 102, 10, 0.0f);
			}

			Commands->Send_Custom_Event(obj, nodPowerPlantBuildingObj, 1, 0, 0.0f);
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 400501)
	{
		GameObject *pathToSkiResortTowerNodOfficerObj = Commands->Find_Object(400503);
		Commands->Send_Custom_Event(obj, pathToSkiResortTowerNodOfficerObj, 0, 0, 0.0f);

		GameObject *nonExisting1Obj = Commands->Find_Object(400504); // Does not exist
		Commands->Send_Custom_Event(obj, nonExisting1Obj, 0, 0, 0.0f);

		GameObject *pathToSkiResortNodRocketTrooperObj = Commands->Find_Object(400505);
		Commands->Send_Custom_Event(obj, pathToSkiResortNodRocketTrooperObj, 0, 0, 0.0f);

		GameObject *pathToSkiResortNodLightTankObj = Commands->Find_Object(400506);
		Commands->Send_Custom_Event(obj, pathToSkiResortNodLightTankObj, 0, 0, 0.0f);

		GameObject *nonExisting2Obj = Commands->Find_Object(400507);
		Commands->Send_Custom_Event(obj, nonExisting2Obj, 0, 0, 0.0f); // Does not exist

		GameObject *nonExisting3Obj = Commands->Find_Object(400508); // Does not exist
		Commands->Send_Custom_Event(obj, nonExisting3Obj, 0, 0, 0.0f);

		GameObject *nonExisting4Obj = Commands->Find_Object(400509); // Does not exist
		Commands->Send_Custom_Event(obj, nonExisting4Obj, 0, 0, 0.0f);

		GameObject *pathToSkiResortTowerNodEngineerObj = Commands->Find_Object(400510);
		Commands->Send_Custom_Event(obj, pathToSkiResortTowerNodEngineerObj, 0, 0, 0.0f);

		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 24, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 214, 0, 0.0f);

		GameObject *aroundSkiResortPathScriptZoneObj = Commands->Find_Object(400502);
		if (aroundSkiResortPathScriptZoneObj)
		{
			Commands->Destroy_Object(aroundSkiResortPathScriptZoneObj);
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 400316)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 0, 0.0f);

		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1050, pos, 1000.0f);

		GameObject *firstTunnelNodMinigunner1Obj = Commands->Find_Object(400317);
		Commands->Send_Custom_Event(obj, firstTunnelNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *firstTunnelNodMinigunner2Obj = Commands->Find_Object(400318);
		Commands->Send_Custom_Event(obj, firstTunnelNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *firstTunnelNodMinigunner3Obj = Commands->Find_Object(400319);
		Commands->Send_Custom_Event(obj, firstTunnelNodMinigunner3Obj, 0, 0, 0.0f);

		Commands->Destroy_Object(obj);
	}
	else if (objId == 400502)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 24, 0.0f);

		GameObject *pathToSkiResortTowerNodOfficerObj = Commands->Find_Object(400503);
		Commands->Send_Custom_Event(obj, pathToSkiResortTowerNodOfficerObj, 0, 0, 0.0f);

		GameObject *nonExisting1Obj = Commands->Find_Object(400504); // Does not exist
		Commands->Send_Custom_Event(obj, nonExisting1Obj, 0, 0, 0.0f);

		GameObject *pathToSkiResortNodRocketTrooperObj = Commands->Find_Object(400505);
		Commands->Send_Custom_Event(obj, pathToSkiResortNodRocketTrooperObj, 0, 0, 0.0f);

		GameObject *pathToSkiResortNodLightTankObj = Commands->Find_Object(400506);
		Commands->Send_Custom_Event(obj, pathToSkiResortNodLightTankObj, 0, 0, 0.0f);

		GameObject *nonExisting2Obj = Commands->Find_Object(400507);
		Commands->Send_Custom_Event(obj, nonExisting2Obj, 0, 0, 0.0f); // Does not exist

		GameObject *nonExisting3Obj = Commands->Find_Object(400508); // Does not exist
		Commands->Send_Custom_Event(obj, nonExisting3Obj, 0, 0, 0.0f);

		GameObject *nonExisting4Obj = Commands->Find_Object(400509); // Does not exist
		Commands->Send_Custom_Event(obj, nonExisting4Obj, 0, 0, 0.0f);

		GameObject *pathToSkiResortTowerNodEngineerObj = Commands->Find_Object(400510);
		Commands->Send_Custom_Event(obj, pathToSkiResortTowerNodEngineerObj, 0, 0, 0.0f);

		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 214, 0, 0.0f);

		GameObject *exitSkiResortScriptZoneObj = Commands->Find_Object(400501);
		if (exitSkiResortScriptZoneObj)
		{
			Commands->Destroy_Object(exitSkiResortScriptZoneObj);
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 400272)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 8, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 10, 0.0f);

		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1058, pos, 1000.0f);

		GameObject *damLowerPathNodOfficerObj = Commands->Find_Object(400213);
		Commands->Send_Custom_Event(obj, damLowerPathNodOfficerObj, 0, 0, 0.0f);

		GameObject *damLowerPathNodRocketTrooperObj = Commands->Find_Object(400984);
		Commands->Send_Custom_Event(obj, damLowerPathNodRocketTrooperObj, 0, 0, 0.0f);

		GameObject *damMCTRoomScriptZoneObj = Commands->Find_Object(401054);
		Commands->Send_Custom_Event(obj, damMCTRoomScriptZoneObj, 0, 0, 0.0f);

		GameObject *nonExistingObj = Commands->Find_Object(401055); // Does not exist
		Commands->Send_Custom_Event(obj, nonExistingObj, 0, 0, 0.0f);

		GameObject *powerPlantBasementNodMinigunner1Obj = Commands->Find_Object(401056);
		Commands->Send_Custom_Event(obj, powerPlantBasementNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *powerPlantBasementNodMinigunner2Obj = Commands->Find_Object(401057);
		Commands->Send_Custom_Event(obj, powerPlantBasementNodMinigunner2Obj, 0, 0, 0.0f);

		Commands->Destroy_Object(obj);
	}
	else if (objId == 400273)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 10, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 11, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 114, 11, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 102, 11, 0.0f);

		GameObject *powerPlantMainRoomNodOfficerObj = Commands->Find_Object(400276);
		Commands->Send_Custom_Event(obj, powerPlantMainRoomNodOfficerObj, 0, 0, 0.0f);

		GameObject *powerPlantMCTNodEngineerObj = Commands->Find_Object(400199);
		Commands->Send_Custom_Event(obj, powerPlantMCTNodEngineerObj, 0, 0, 0.0f);

		GameObject *powerPlantMainRoomNodMinigunner1Obj = Commands->Find_Object(401065);
		Commands->Send_Custom_Event(obj, powerPlantMainRoomNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *powerPlantCoreNodTechnicianObj = Commands->Find_Object(401058);
		Commands->Send_Custom_Event(obj, powerPlantCoreNodTechnicianObj, 0, 0, 0.0f);

		GameObject *nonExisting1Obj = Commands->Find_Object(401059); // Does not exist
		Commands->Send_Custom_Event(obj, nonExisting1Obj, 0, 0, 0.0f);

		GameObject *powerPlantBasementNodMinigunnerObj = Commands->Find_Object(401060);
		Commands->Send_Custom_Event(obj, powerPlantBasementNodMinigunnerObj, 0, 0, 0.0f);

		GameObject *nonExisting2Obj = Commands->Find_Object(401061); // Does not exist
		Commands->Send_Custom_Event(obj, nonExisting2Obj, 0, 0, 0.0f);

		GameObject *powerPlantMainRoomNodTechnicianObj = Commands->Find_Object(401063);
		Commands->Send_Custom_Event(obj, powerPlantMainRoomNodTechnicianObj, 0, 0, 0.0f);

		GameObject *powerPlantMainRoomNodMinigunner2Obj = Commands->Find_Object(401064);
		Commands->Send_Custom_Event(obj, powerPlantMainRoomNodMinigunner2Obj, 0, 0, 0.0f);

		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1060, pos, 1000.0f);

		Commands->Destroy_Object(obj);
	}
	else if (objId == 400271)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 9, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 8, 0.0f);

		GameObject *damNodOfficerObj = Commands->Find_Object(400275);
		Commands->Send_Custom_Event(obj, damNodOfficerObj, 0, 0, 0.0f);

		GameObject *damNodMinigunner1Obj = Commands->Find_Object(401046);
		Commands->Send_Custom_Event(obj, damNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *damNodMinigunner2Obj = Commands->Find_Object(401047);
		Commands->Send_Custom_Event(obj, damNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *nonExistingObj = Commands->Find_Object(401048); // Does not exist
		Commands->Send_Custom_Event(obj, nonExistingObj, 0, 0, 0.0f);

		GameObject *damNodMinigunner3Obj = Commands->Find_Object(401049);
		Commands->Send_Custom_Event(obj, damNodMinigunner3Obj, 0, 0, 0.0f);

		GameObject *damNodMinigunner4Obj = Commands->Find_Object(401050);
		Commands->Send_Custom_Event(obj, damNodMinigunner4Obj, 0, 0, 0.0f);

		GameObject *damNodMinigunner5Obj = Commands->Find_Object(401051);
		Commands->Send_Custom_Event(obj, damNodMinigunner5Obj, 0, 0, 0.0f);

		GameObject *damNodMinigunner6Obj = Commands->Find_Object(401052);
		Commands->Send_Custom_Event(obj, damNodMinigunner6Obj, 0, 0, 0.0f);

		GameObject *damNodMinigunner7Obj = Commands->Find_Object(401053);
		Commands->Send_Custom_Event(obj, damNodMinigunner7Obj, 0, 0, 0.0f);

		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1059, pos, 1000.0f);

		GameObject *damWalkwayScriptZoneObj = Commands->Find_Object(303203);
		if (damWalkwayScriptZoneObj)
		{
			Commands->Destroy_Object(damWalkwayScriptZoneObj);
		}

		GameObject *damElevatorScriptZoneObj = Commands->Find_Object(400270);
		if (damElevatorScriptZoneObj)
		{
			Commands->Destroy_Object(damElevatorScriptZoneObj);
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 400187)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 211, 0, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 3, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 4, 0.0f);
		
		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1102, pos, 1000.0f);
		Commands->Create_Logical_Sound(obj, 1053, pos, 1000.0f);

		GameObject *skiResortNodOfficerObj = Commands->Find_Object(400210);
		Commands->Send_Custom_Event(obj, skiResortNodOfficerObj, 0, 0, 0.0f);

		GameObject *skiResortNodMinigunner1Obj = Commands->Find_Object(400451);
		Commands->Send_Custom_Event(obj, skiResortNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *nonExisting1Obj = Commands->Find_Object(400452); // Does not exist
		Commands->Send_Custom_Event(obj, nonExisting1Obj, 0, 0, 0.0f);

		GameObject *skiResortNodMinigunner2Obj = Commands->Find_Object(400453);
		Commands->Send_Custom_Event(obj, skiResortNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *skiResortNodGunEmplacementObj = Commands->Find_Object(400499);
		Commands->Send_Custom_Event(obj, skiResortNodGunEmplacementObj, 0, 0, 0.0f);

		GameObject *nonExisting2Obj = Commands->Find_Object(400500); // Does not exist
		Commands->Send_Custom_Event(obj, nonExisting2Obj, 0, 0, 0.0f);

		GameObject *nonExisting3Obj = Commands->Find_Object(401025); // Does not exist
		Commands->Send_Custom_Event(obj, nonExisting3Obj, 0, 0, 0.0f);

		GameObject *skiResortNodLightTank1Obj = Commands->Find_Object(401026);
		Commands->Send_Custom_Event(obj, skiResortNodLightTank1Obj, 0, 0, 0.0f);

		GameObject *skiResortNodLightTank2Obj = Commands->Find_Object(401027);
		Commands->Send_Custom_Event(obj, skiResortNodLightTank2Obj, 0, 0, 0.0f);

		GameObject *skiResortGDIMammothTankObj = Commands->Find_Object(401028);
		Commands->Send_Custom_Event(obj, skiResortGDIMammothTankObj, 0, 0, 0.0f);

		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 112, 4, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 111, 4, 0.0f);

		Create_Buggy(4);

		GameObject *skiResortMedTankObj = Commands->Find_Object(401018);
		Commands->Send_Custom_Event(obj, skiResortMedTankObj, 0, 0, 0.0f);

		Commands->Start_Timer(obj, this, 0.1f, 10);

		GameObject *aroundSkiResortScriptZoneObj = Commands->Find_Object(400267);
		if (aroundSkiResortScriptZoneObj)
		{
			Commands->Destroy_Object(aroundSkiResortScriptZoneObj);
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 304006)
	{
		if (!this->isZoneTriggered)
		{
			this->isZoneTriggered = true;

			int conversationId = Commands->Create_Conversation("MX2DSGN_DSGN0001", 100, 300.0f, true); // Take the Hum-vee, Havoc, you have a lot of open road to cover
			Commands->Join_Conversation(NULL, conversationId, true, true, true);

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			Commands->Join_Conversation(starObj, conversationId, true, false, false);
			Commands->Start_Conversation(conversationId, 0);

			Commands->Set_HUD_Help_Text(8287, Vector3(0.196f, 0.882f, 0.196f)); // Press 'E' to enter the Humm-Vee\n
		}

		Commands->Destroy_Object(obj);
	}
	else if (objId == 301601)
	{
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		if (!Commands->Has_Key(starObj, 6))
		{
			Commands->Stop_All_Conversations();

			int conversationId = Commands->Create_Conversation("M02_BAY_DOOR_WARNING", 100, 300.0f, true); // I'm still showing the dam's Master Control Terminal as active. It should be somewhere inside, find it and destroy it.
			Commands->Join_Conversation(NULL, conversationId, true, true, true);

			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			Commands->Join_Conversation(starObj, conversationId, true, false, false);
			Commands->Start_Conversation(conversationId, 0);
		}
	
		Commands->Destroy_Object(obj);
	}
	else if (objId == 303203)
	{
		Commands->Stop_All_Conversations();

		int conversationId = Commands->Create_Conversation("M02_PRIMARY_03_GUIDE", 100, 300.0f, true); // Good, you made it inside. EVA indicates the dam's Master Control Terminal controls the large bay door on top. Find the Master Control Terminal and destroy it.
		Commands->Join_Conversation(NULL, conversationId, true, true, true);

		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, false, false);
		Commands->Start_Conversation(conversationId, 0);

		Commands->Destroy_Object(obj);
	}
	else if (objId == 400184)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 206, 0, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 207, 0, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 0, 0.0f);

		Call_GDI_Soldiers(0);
		Create_Apache(0);

		GameObject *firstAreaNodMinigunner1Obj = Commands->Find_Object(400286);
		Commands->Send_Custom_Event(obj, firstAreaNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *firstAreaNodMinigunner2Obj = Commands->Find_Object(400287);
		Commands->Send_Custom_Event(obj, firstAreaNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *nonExistingObj = Commands->Find_Object(400288); // Does not exist
		Commands->Send_Custom_Event(obj, nonExistingObj, 0, 0, 0.0f);

		GameObject *firstAreaNodMinigunner3Obj = Commands->Find_Object(400292);
		Commands->Send_Custom_Event(obj, firstAreaNodMinigunner3Obj, 0, 0, 0.0f);

		GameObject *firstAreaNodMinigunner4Obj = Commands->Find_Object(400293);
		Commands->Send_Custom_Event(obj, firstAreaNodMinigunner4Obj, 0, 0, 0.0f);

		GameObject *firstAreaNodRocketTrooperObj = Commands->Find_Object(400285);
		Commands->Send_Custom_Event(obj, firstAreaNodRocketTrooperObj, 0, 0, 0.0f);

		GameObject *firstAreaNodOfficerObj = Commands->Find_Object(400206);
		Commands->Send_Custom_Event(obj, firstAreaNodOfficerObj, 0, 0, 0.0f);

		GameObject *firstAreaGDIMedTank1Obj = Commands->Find_Object(401004);
		Commands->Send_Custom_Event(obj, firstAreaGDIMedTank1Obj, 0, 0, 0.0f);

		GameObject *firstAreaNodLightTankObj = Commands->Find_Object(401005);
		Commands->Send_Custom_Event(obj, firstAreaNodLightTankObj, 0, 0, 0.0f);

		GameObject *firstAreaGDIMedTank2Obj = Commands->Find_Object(401006);
		Commands->Send_Custom_Event(obj, firstAreaGDIMedTank2Obj, 0, 0, 0.0f);

		Commands->Destroy_Object(obj);
	}
	else if (objId == 400186)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 210, 0, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 221, 0, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 1, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 2, 0.0f);

		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1051, pos, 1000.0f);

		GameObject *thirdAreaNodMinigunner1Obj = Commands->Find_Object(400372);
		Commands->Send_Custom_Event(obj, thirdAreaNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *thirdAreaNodMinigunner2Obj = Commands->Find_Object(400373);
		Commands->Send_Custom_Event(obj, thirdAreaNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *thirdAreaNodMinigunner3Obj = Commands->Find_Object(400374);
		Commands->Send_Custom_Event(obj, thirdAreaNodMinigunner3Obj, 0, 0, 0.0f);

		GameObject *thirdAreaNodMobileArtilleryObj = Commands->Find_Object(401167);
		Commands->Send_Custom_Event(obj, thirdAreaNodMobileArtilleryObj, 0, 0, 0.0f);

		GameObject *thirdAreaNodOfficerObj = Commands->Find_Object(400208);
		Commands->Send_Custom_Event(obj, thirdAreaNodOfficerObj, 0, 0, 0.0f);

		GameObject *thirdAreaGDIMedTank1Obj = Commands->Find_Object(401014);
		Commands->Send_Custom_Event(obj, thirdAreaGDIMedTank1Obj, 0, 0, 0.0f);

		GameObject *thirdAreaNodLightTank1Obj = Commands->Find_Object(401015);
		Commands->Send_Custom_Event(obj, thirdAreaNodLightTank1Obj, 0, 0, 0.0f);

		GameObject *thirdAreaGDIMedTank2Obj = Commands->Find_Object(401016);
		Commands->Send_Custom_Event(obj, thirdAreaGDIMedTank2Obj, 0, 0, 0.0f);

		GameObject *thirdAreaNodLightTank2Obj = Commands->Find_Object(401017);
		Commands->Send_Custom_Event(obj, thirdAreaNodLightTank2Obj, 0, 0, 0.0f);

		Create_Buggy(2);
		Create_Sakura(2);
	
		Commands->Destroy_Object(obj);
	}
	else if (objId == 400185)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 208, 0, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 209, 0, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 1, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 112, 1, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 111, 1, 0.0f);

		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1100, pos, 1000.0f);
		Commands->Create_Logical_Sound(obj, 1073, pos, 1000.0f);

		GameObject *secondAreaGDIMinigunner1Obj = Commands->Find_Object(400342);
		Commands->Send_Custom_Event(obj, secondAreaGDIMinigunner1Obj, 0, 0, 0.0f);

		GameObject *secondAreaGDIMinigunner2Obj = Commands->Find_Object(400343);
		Commands->Send_Custom_Event(obj, secondAreaGDIMinigunner2Obj, 0, 0, 0.0f);

		GameObject *secondAreaGDIMinigunner3Obj = Commands->Find_Object(400347);
		Commands->Send_Custom_Event(obj, secondAreaGDIMinigunner3Obj, 0, 0, 0.0f);

		GameObject *secondAreaGDIOfficerObj = Commands->Find_Object(400332);
		Commands->Send_Custom_Event(obj, secondAreaGDIOfficerObj, 0, 0, 0.0f);

		GameObject *secondAreaGDIMammothTank = Commands->Find_Object(400340);
		Commands->Send_Custom_Event(obj, secondAreaGDIMammothTank, 0, 0, 0.0f);

		GameObject *nonExistingObj = Commands->Find_Object(400348); // Does not exist
		Commands->Send_Custom_Event(obj, nonExistingObj, 0, 0, 0.0f);

		GameObject *secondAreaNodMinigunner1Obj = Commands->Find_Object(400344);
		Commands->Send_Custom_Event(obj, secondAreaNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *secondAreaNodMinigunner2Obj = Commands->Find_Object(400345);
		Commands->Send_Custom_Event(obj, secondAreaNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *secondAreaNodLightTank1Obj = Commands->Find_Object(400333);
		Commands->Send_Custom_Event(obj, secondAreaNodLightTank1Obj, 0, 0, 0.0f);

		GameObject *secondAreaNodOfficerObj = Commands->Find_Object(400207);
		Commands->Send_Custom_Event(obj, secondAreaNodOfficerObj, 0, 0, 0.0f);

		GameObject *secondAreaNodLightTank2Obj = Commands->Find_Object(400371);
		Commands->Send_Custom_Event(obj, secondAreaNodLightTank2Obj, 0, 0, 0.0f);

		GameObject *secondAreaGDIMedTankObj = Commands->Find_Object(401008);
		Commands->Send_Custom_Event(obj, secondAreaGDIMedTankObj, 0, 0, 0.0f);
		
		Commands->Destroy_Object(obj);
	}
	else if (objId == 400192)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 11, 0.0f);

		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1061, pos, 1000.0f);

		Commands->Destroy_Object(obj);
	}
	else if (objId == 400189)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 215, 0, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 216, 0, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 6, 0.0f);

		GameObject *bridgeNodOfficerObj = Commands->Find_Object(400211);
		Commands->Send_Custom_Event(obj, bridgeNodOfficerObj, 0, 0, 0.0f);

		GameObject *bridgeNodGunEmplacement1Obj = Commands->Find_Object(400976);
		Commands->Send_Custom_Event(obj, bridgeNodGunEmplacement1Obj, 0, 0, 0.0f);

		GameObject *bridgeNodGunEmplacement2Obj = Commands->Find_Object(400975);
		Commands->Send_Custom_Event(obj, bridgeNodGunEmplacement2Obj, 0, 0, 0.0f);

		GameObject *bridgeNodMinigunner1Obj = Commands->Find_Object(400977);
		Commands->Send_Custom_Event(obj, bridgeNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *bridgeNodMinigunner2Obj = Commands->Find_Object(400978);
		Commands->Send_Custom_Event(obj, bridgeNodMinigunner2Obj, 0, 0, 0.0f);

		GameObject *bridgeNodMinigunner3Obj = Commands->Find_Object(400979);
		Commands->Send_Custom_Event(obj, bridgeNodMinigunner3Obj, 0, 0, 0.0f);

		GameObject *bridgeNodMinigunner4Obj = Commands->Find_Object(400980);
		Commands->Send_Custom_Event(obj, bridgeNodMinigunner4Obj, 0, 0, 0.0f);

		GameObject *bridgeGDIMedTank1Obj = Commands->Find_Object(401036);
		Commands->Send_Custom_Event(obj, bridgeGDIMedTank1Obj, 0, 0, 0.0f);

		GameObject *bridgeNodLightTankObj = Commands->Find_Object(401037);
		Commands->Send_Custom_Event(obj, bridgeNodLightTankObj, 0, 0, 0.0f);

		GameObject *bridgeGDIMedTank2Obj = Commands->Find_Object(401038);
		Commands->Send_Custom_Event(obj, bridgeGDIMedTank2Obj, 0, 0, 0.0f);

		GameObject *bridgeNodRocketTrooperObj = Commands->Find_Object(401045);
		Commands->Send_Custom_Event(obj, bridgeNodRocketTrooperObj, 0, 0, 0.0f);

		Commands->Destroy_Object(obj);
	}
	else if (objId == 400188)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 212, 0, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 213, 0, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 2, 0.0f);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 3, 0.0f);

		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1052, pos, 1000.0f);

		GameObject *nodLightTank = Commands->Create_Object("Nod_Light_Tank", Vector3(465.645f, 715.86f, -14.935f));
		if (nodLightTank)
		{
			Commands->Set_Facing(nodLightTank, -10.0f);
			Commands->Attach_Script(nodLightTank, "M02_Nod_Vehicle", "3");
		}

		GameObject *pathToSkiResortNodRocketTrooper1Obj = Commands->Find_Object(400410);
		Commands->Send_Custom_Event(obj, pathToSkiResortNodRocketTrooper1Obj, 0, 0, 0.0f);

		GameObject *pathToSkiResortNodOfficerObj = Commands->Find_Object(400209);
		Commands->Send_Custom_Event(obj, pathToSkiResortNodOfficerObj, 0, 0, 0.0f);

		GameObject *pathToSkiResortNodRocketTrooper2Obj = Commands->Find_Object(400411);
		Commands->Send_Custom_Event(obj, pathToSkiResortNodRocketTrooper2Obj, 0, 0, 0.0f);

		GameObject *nonExisting1Obj = Commands->Find_Object(400412); // Does not exist
		Commands->Send_Custom_Event(obj, nonExisting1Obj, 0, 0, 0.0f);

		GameObject *pathToSkiResortNodMinigunner1Obj = Commands->Find_Object(400413);
		Commands->Send_Custom_Event(obj, pathToSkiResortNodMinigunner1Obj, 0, 0, 0.0f);

		GameObject *nonExisting2Obj = Commands->Find_Object(400414); // Does not exist
		Commands->Send_Custom_Event(obj, nonExisting2Obj, 0, 0, 0.0f);

		GameObject *pathToSkiResortNodMinigunner2Obj = Commands->Find_Object(400415);
		Commands->Send_Custom_Event(obj, pathToSkiResortNodMinigunner2Obj, 0, 0, 0.0f);
		
		Commands->Destroy_Object(obj);
	}
	else if (objId == 400195)
	{
		this->canSendCustomAndDestroy = true;

		Vector3 pos = Commands->Get_Position(obj);
		Commands->Create_Logical_Sound(obj, 1000, pos, 70.0f);

		Commands->Start_Timer(obj, this, 1.0f, 3);
	}
	else if (objId == 400193)
	{
		GameObject *invisObj = Commands->Create_Object("Invisible_Object", Vector3(0.0f, 0.0f, 0.0f));
		if (invisObj)
		{
			Vector3 pos = Commands->Get_Position(obj);
			GameObject *starObj = Commands->Get_A_Star(pos);

			Commands->Control_Enable(starObj, false);

			Commands->Start_Timer(obj, this, 1.0f, 9);

			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 1000, 1002, 25.0f);

			Commands->Attach_Script(invisObj, "Test_Cinematic", "X2K_Midtro.txt");
		}
	}
	else if (objId == 400194)
	{
		if (!this->isZoneTriggered)
		{
			this->isZoneTriggered = true;

			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 205, 1, 0.0f);
			Commands->Mission_Complete(true);

			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 21, 0.0f);
		}
	}
}

void M02_Objective_Zone::Send_and_Destroy(GameObject *obj, int type)
{
	if (this->canSendCustomAndDestroy)
	{
		GameObject *M02ObjectiveControllerObj = Commands->Find_Object(1111112);
		if (M02ObjectiveControllerObj)
		{
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, type, 1, 0.0f);

			this->canSendCustomAndDestroy = false;

			Commands->Destroy_Object(obj);

			if (type == 207)
			{
				Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 111, 0, 0.0f);
				
				Vector3 pos = Commands->Get_Position(obj);
				Commands->Create_Logical_Sound(obj, 1100, pos, 1000.0f);
			}
			else if (type == 210)
			{
				Call_GDI_Soldiers(2);
			}
		}
	}
	else if (!this->starWarnedForLeavingPrematurely)
	{
		this->starWarnedForLeavingPrematurely = true;

		Commands->Stop_All_Conversations();

		int conversationId = Commands->Create_Conversation("M02_EVAG_SECURE_WARNING", 100, 300.0f, true); // All enemy resistance in the area must be eliminated to secure this objective.
		Commands->Join_Conversation(NULL, conversationId, true, true, true);
		
		Vector3 pos = Commands->Get_Position(obj);
		GameObject *starObj = Commands->Get_A_Star(pos);

		Commands->Join_Conversation(starObj, conversationId, true, false, false);
		Commands->Start_Conversation(conversationId, 0);
	}
}

void M02_Objective_Zone::Call_GDI_Soldiers(int a2)
{
	Vector3 cinematicPos(0.0f, 0.0f, 0.0f);
	float facing = 0.0f;
	const char *cinematicFileName = "X2I_GDI_Drop02_Rocket.txt";

	if (!a2)
	{
		cinematicPos = Vector3(-10.6f, -25.1f, -38.2f);
		facing = 70.5f;
		cinematicFileName = "X2I_GDI_Drop02_Rocket.txt";
	}
	else if (a2 == 2)
	{
		cinematicPos = Vector3(664.7f, 312.57f, -59.65f);
		facing = -150.0f;
		cinematicFileName = "X2I_GDI_Drop02_Rocket_2.txt";
	}
	else if (a2 == 9)
	{
		cinematicPos = Vector3(869.32f, 912.316f, 30.15f);
		facing = 110.0f;
		cinematicFileName = "X2I_GDI_Drop03_Minigunner.txt";
	}

	GameObject *invisObj = Commands->Create_Object("Invisible_Object", cinematicPos);
	if (invisObj)
	{
		Commands->Set_Facing(invisObj, facing);
		Commands->Attach_Script(invisObj, "Test_Cinematic", cinematicFileName);
	}
}

void M02_Objective_Zone::Create_Apache(int a2)
{
	GameObject *helipadBuildingObj = Commands->Find_Object(474463);
	if (helipadBuildingObj)
	{
		float helipadHealth = Commands->Get_Health(helipadBuildingObj);
		if (Commands->Get_Building_Power(helipadBuildingObj) && helipadHealth > 0.0f || a2 == 15)
		{
			Vector3 apachePos(0.0f, 0.0f, 0.0f);
			float facing = 0.0f;
			const char *scriptParams = "0";

			if (!a2)
			{
				apachePos = Vector3(138.36f, -5.61f, -12.2f);
				facing = -80.0f;
				scriptParams = "0";
			}
			else if (a2 == 15)
			{
				apachePos = Vector3(1300.9f, 657.717f, 67.364f);
				facing = 0.0f;
				scriptParams = "15";
			}
			else if (a2 == 24)
			{
				apachePos = Vector3(610.03f, 1036.35f, -15.676f);
				facing = 0.0f;
				scriptParams = "24";
			}

			GameObject *nodApacheObj = Commands->Create_Object("Nod_Apache", apachePos);
			if (nodApacheObj)
			{
				Commands->Set_Facing(nodApacheObj, facing);
				Commands->Attach_Script(nodApacheObj, "M02_Nod_Apache", scriptParams);
			}
		}
	}
}

void M02_Objective_Zone::Create_Sakura(int a2)
{
	GameObject *helipadBuildingObj = Commands->Find_Object(474463);
	if (helipadBuildingObj)
	{
		float helipadHealth = Commands->Get_Health(helipadBuildingObj);
		if (Commands->Get_Building_Power(helipadBuildingObj) && helipadHealth > 0.0f)
		{
			GameObject *nodApacheObj = Commands->Create_Object("Nod_Apache", Vector3(509.362f, 473.582f, -40.0f));
			if (nodApacheObj)
			{
				Commands->Set_Facing(nodApacheObj, 0.0f);
				Commands->Attach_Script(nodApacheObj, "M02_Nod_Sakura", "2");
			}
		}
	}
}

void M02_Objective_Zone::Create_Buggy(int a2)
{
	Vector3 buggyPos(0.0f, 0.0f, 0.0f);
	float facing = 0.0f;
	const char *scriptParams = "0";

	if (a2 == 2)
	{
		buggyPos = Vector3(674.622f, 308.296f, -61.674f);
		facing = -90.0f;
		scriptParams = "2";
	}
	else if (a2 == 4)
	{
		buggyPos = Vector3(366.209f, 861.401f, 6.488f);
		facing = -110.0f;
		scriptParams = "4";
	}

	GameObject *nodBuggyObj = Commands->Create_Object("Nod_Buggy", buggyPos);
	if (nodBuggyObj)
	{
		Commands->Set_Facing(nodBuggyObj, facing);
		Commands->Attach_Script(nodBuggyObj, "M02_Nod_Vehicle", scriptParams);
	}
}

ScriptRegistrant<M02_Objective_Zone> M02_Objective_ZoneRegistrant("M02_Objective_Zone", "");