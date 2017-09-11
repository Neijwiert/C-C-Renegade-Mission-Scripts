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
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
	Auto_Save_Variable(&this->mendozaObjId, sizeof(this->mendozaObjId), 2);
	Auto_Save_Variable(&this->field_1D, sizeof(this->field_1D), 3);
	Auto_Save_Variable(&this->field_1E, sizeof(this->field_1E), 4);
}

// On level start
void M02_Objective_Zone::Created(GameObject *obj)
{
	this->field_1E = false;
	this->mendozaObjId = 0;
	this->field_1D = false;
	this->field_1C = false;
}

// TODO
void M02_Objective_Zone::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == 100)
	{
		if (param == 1)
		{
			this->field_1C = false;
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

	/*
	if (objId > 400551)
	{
		if (objId <= 401113)
		{
			if (objId > 401054)
			{
				if (objId != 401066)
				{
					if (objId != 401070)
						goto LABEL_193;
					Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 11, 0.0);
					Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 14, 0.0);
					Commands->Get_Position((Vector3 *)&v589, obj);
					Commands->Create_Logical_Sound(obj, 1061, (const Vector3 *)&v589, 1000.0);
					v301 = Commands;
					v302 = Commands->Find_Object(401071);
					v301->Send_Custom_Event(obj, v302, 0, 0, 0.0);
					v303 = Commands;
					v304 = Commands->Find_Object(401072);
					v303->Send_Custom_Event(obj, v304, 0, 0, 0.0);
					v305 = Commands;
					v306 = Commands->Find_Object(401073);
					v305->Send_Custom_Event(obj, v306, 0, 0, 0.0);
					v307 = Commands;
					v308 = Commands->Find_Object(401074);
					v307->Send_Custom_Event(obj, v308, 0, 0, 0.0);
					v309 = Commands;
					v310 = Commands->Find_Object(401075);
					v309->Send_Custom_Event(obj, v310, 0, 0, 0.0);
					v311 = Commands;
					v312 = Commands->Find_Object(401076);
					v311->Send_Custom_Event(obj, v312, 0, 0, 0.0);
					v313 = Commands;
					v314 = Commands->Find_Object(401077);
					v313->Send_Custom_Event(obj, v314, 0, 0, 0.0);
					v315 = Commands;
					v316 = Commands->Find_Object(401078);
					v315->Send_Custom_Event(obj, v316, 0, 0, 0.0);
					v317 = Commands;
					v318 = Commands->Find_Object(401192);
					v317->Send_Custom_Event(obj, v318, 0, 0, 0.0);
					v319 = Commands;
					v320 = Commands->Find_Object(401193);
					v319->Send_Custom_Event(obj, v320, 0, 0, 0.0);
					v321 = Commands;
					v322 = Commands->Find_Object(401194);
					v321->Send_Custom_Event(obj, v322, 0, 0, 0.0);
					v323 = Commands;
					v324 = Commands->Find_Object(401195);
					v323->Send_Custom_Event(obj, v324, 0, 0, 0.0);
					v587 = 401196;
					goto LABEL_192;
				}
				v292 = Commands;
				v293 = Commands->Find_Object(400200);
				v292->Send_Custom_Event(obj, v293, 0, 0, 0.0);
				v294 = Commands;
				v295 = Commands->Find_Object(401067);
				v294->Send_Custom_Event(obj, v295, 0, 0, 0.0);
				v296 = Commands;
				v297 = Commands->Find_Object(401068);
				v296->Send_Custom_Event(obj, v297, 0, 0, 0.0);
				v298 = Commands;
				v299 = Commands->Find_Object(401069);
				v298->Send_Custom_Event(obj, v299, 0, 0, 0.0);
				v300 = Commands->Find_Object(1153845);
				if (v300)
					Commands->Send_Custom_Event(obj, v300, 1, 0, 0.0);
				goto LABEL_190;
			}
			if (objId == 400991)
			{
				v589 = 0;
				v590 = 0;
				v591 = 0;
				if (!(int)Commands->Create_Object("Invisible_Object", (const Vector3 *)&v589))
					goto LABEL_190;
				v582 = "X2F_OrcaSam.txt";
			}
			else if (objId > 400991)
			{
				if (objId == 401001)
				{
					v589 = 0;
					v590 = 0;
					v591 = 0;
					if (!(int)Commands->Create_Object("Invisible_Object", (const Vector3 *)&v589))
						goto LABEL_190;
					v582 = "X2H_ApacheSpray.txt";
				}
				else if (objId > 401001)
				{
					if (objId != 401029)
						goto LABEL_193;
					v589 = 0;
					v590 = 0;
					v591 = 0;
					if (!(int)Commands->Create_Object("Invisible_Object", (const Vector3 *)&v589))
						goto LABEL_190;
					v582 = "X2D_MTank_Slip.txt";
				}
				else
				{
					if (objId != 400992)
						goto LABEL_193;
					v589 = 0;
					v590 = 0;
					v591 = 0;
					if (!(int)Commands->Create_Object("Invisible_Object", (const Vector3 *)&v589))
						goto LABEL_190;
					v582 = "X2G_AirCrash.txt";
				}
			}
			else
			{
				if (objId == 400989)
					goto LABEL_190;
				if (objId != 400990)
					goto LABEL_193;
				v589 = 0;
				v590 = 0;
				v591 = 0;
				if (!(int)Commands->Create_Object("Invisible_Object", (const Vector3 *)&v589))
					goto LABEL_190;
				v582 = "X2E_C130Drop.txt";
			}
			v581 = "Test_Cinematic";
		LABEL_189:
			Commands->Attach_Script(v579, v581, v582);
			goto LABEL_190;
		}
		if (objId == 401196)
		{
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 11, 0.0);
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 14, 0.0);
			Commands->Get_Position((Vector3 *)&v589, obj);
			Commands->Create_Logical_Sound(obj, 1061, (const Vector3 *)&v589, 1000.0);
			v526 = Commands;
			v527 = Commands->Find_Object(401071);
			v526->Send_Custom_Event(obj, v527, 0, 0, 0.0);
			v528 = Commands;
			v529 = Commands->Find_Object(401072);
			v528->Send_Custom_Event(obj, v529, 0, 0, 0.0);
			v530 = Commands;
			v531 = Commands->Find_Object(401073);
			v530->Send_Custom_Event(obj, v531, 0, 0, 0.0);
			v532 = Commands;
			v533 = Commands->Find_Object(401074);
			v532->Send_Custom_Event(obj, v533, 0, 0, 0.0);
			v534 = Commands;
			v535 = Commands->Find_Object(401075);
			v534->Send_Custom_Event(obj, v535, 0, 0, 0.0);
			v536 = Commands;
			v537 = Commands->Find_Object(401076);
			v536->Send_Custom_Event(obj, v537, 0, 0, 0.0);
			v538 = Commands;
			v539 = Commands->Find_Object(401077);
			v538->Send_Custom_Event(obj, v539, 0, 0, 0.0);
			v540 = Commands;
			v541 = Commands->Find_Object(401078);
			v540->Send_Custom_Event(obj, v541, 0, 0, 0.0);
			v542 = Commands;
			v543 = Commands->Find_Object(401192);
			v542->Send_Custom_Event(obj, v543, 0, 0, 0.0);
			v544 = Commands;
			v545 = Commands->Find_Object(401193);
			v544->Send_Custom_Event(obj, v545, 0, 0, 0.0);
			v546 = Commands;
			v547 = Commands->Find_Object(401194);
			v546->Send_Custom_Event(obj, v547, 0, 0, 0.0);
			v548 = Commands;
			v549 = Commands->Find_Object(401195);
			v548->Send_Custom_Event(obj, v549, 0, 0, 0.0);
			v587 = 401070;
			goto LABEL_192;
		}
		if (objId <= 401196)
		{
			if (objId == 401130)
			{
				v464 = Commands->Find_Object(1157690);
				if (v464)
				{
					Commands->Get_Health(v464);
					v466 = v3 < 0.0;
					v467 = 0;
					v468 = v3 == 0.0;
					if ((HIBYTE(v465) & 0x45) != 64)
					{
						Commands->Stop_All_Conversations();
						v469 = Commands->Create_Conversation("M02_SECONDARY_01_GUIDE", 100, 300.0, 1);
						Commands->Join_Conversation(0, v469, 1, 1, 1);
						v470 = Commands;
						v471 = this->base.base.base.vPtr->base.Owner((ScriptClass *)this);
						v470->Get_Position((Vector3 *)&v589, v471);
						v472 = v470->Get_A_Star((Vector3 *)&v589);
						v470->Join_Conversation(v472, v473, v469, 1, 0);
						Commands->Start_Conversation(v469, 0);
					}
				}
				v474 = Commands;
				v475 = Commands->Find_Object(401132);
				v474->Send_Custom_Event(obj, v475, 0, 0, 0.0);
				v476 = Commands;
				v477 = Commands->Find_Object(401133);
				v476->Send_Custom_Event(obj, v477, 0, 0, 0.0);
				v478 = Commands;
				v479 = Commands->Find_Object(401134);
				v478->Send_Custom_Event(obj, v479, 0, 0, 0.0);
				v480 = Commands;
				v481 = Commands->Find_Object(401135);
				v480->Send_Custom_Event(obj, v481, 0, 0, 0.0);
				v482 = Commands;
				v483 = Commands->Find_Object(401136);
				v482->Send_Custom_Event(obj, v483, 0, 0, 0.0);
				v484 = Commands;
				v485 = Commands->Find_Object(401137);
				v484->Send_Custom_Event(obj, v485, 0, 0, 0.0);
				v486 = Commands;
				v487 = Commands->Find_Object(401138);
				v486->Send_Custom_Event(obj, v487, 0, 0, 0.0);
				v488 = Commands;
				v489 = Commands->Find_Object(401139);
				v488->Send_Custom_Event(obj, v489, 0, 0, 0.0);
				v587 = 401131;
				goto LABEL_192;
			}
			if (objId > 401130)
			{
				if (objId == 401186)
				{
					M02_Objective_Zone::Create_Apache(this, 15);
					Commands->Stop_All_Conversations();
					v516 = Commands->Create_Conversation("M02_SECONDARY_14_GUIDE", 100, 300.0, 1);
					Commands->Join_Conversation(0, v516, 1, 1, 1);
					v517 = Commands;
					v518 = this->base.base.base.vPtr->base.Owner((ScriptClass *)this);
					v517->Get_Position((Vector3 *)&v589, v518);
					v519 = v517->Get_A_Star((Vector3 *)&v589);
					v517->Join_Conversation(v519, v520, v516, 1, 0);
					Commands->Start_Conversation(v516, 0);
					v587 = 401187;
					goto LABEL_185;
				}
				if (objId > 401186)
				{
					if (objId != 401187)
						goto LABEL_193;
					M02_Objective_Zone::Create_Apache(this, 15);
					Commands->Stop_All_Conversations();
					v521 = Commands->Create_Conversation("M02_SECONDARY_14_GUIDE", 100, 300.0, 1);
					Commands->Join_Conversation(0, v521, 1, 1, 1);
					v522 = Commands;
					v523 = this->base.base.base.vPtr->base.Owner((ScriptClass *)this);
					v522->Get_Position((Vector3 *)&v589, v523);
					v524 = v522->Get_A_Star((Vector3 *)&v589);
					v522->Join_Conversation(v524, v525, v521, 1, 0);
					Commands->Start_Conversation(v521, 0);
					v587 = 401186;
					goto LABEL_185;
				}
				if (objId != 401131)
					goto LABEL_193;
				v490 = Commands->Find_Object(1157690);
				if (v490)
				{
					Commands->Get_Health(v490);
					v492 = v3 < 0.0;
					v493 = 0;
					v494 = v3 == 0.0;
					if ((HIBYTE(v491) & 0x45) != 64)
					{
						Commands->Stop_All_Conversations();
						v495 = Commands->Create_Conversation("M02_SECONDARY_01_GUIDE", 100, 300.0, 1);
						Commands->Join_Conversation(0, v495, 1, 1, 1);
						v496 = Commands;
						v497 = this->base.base.base.vPtr->base.Owner((ScriptClass *)this);
						v496->Get_Position((Vector3 *)&v589, v497);
						v498 = v496->Get_A_Star((Vector3 *)&v589);
						v496->Join_Conversation(v498, v499, v495, 1, 0);
						Commands->Start_Conversation(v495, 0);
					}
				}
				v500 = Commands;
				v501 = Commands->Find_Object(401132);
				v500->Send_Custom_Event(obj, v501, 0, 0, 0.0);
				v502 = Commands;
				v503 = Commands->Find_Object(401133);
				v502->Send_Custom_Event(obj, v503, 0, 0, 0.0);
				v504 = Commands;
				v505 = Commands->Find_Object(401134);
				v504->Send_Custom_Event(obj, v505, 0, 0, 0.0);
				v506 = Commands;
				v507 = Commands->Find_Object(401135);
				v506->Send_Custom_Event(obj, v507, 0, 0, 0.0);
				v508 = Commands;
				v509 = Commands->Find_Object(401136);
				v508->Send_Custom_Event(obj, v509, 0, 0, 0.0);
				v510 = Commands;
				v511 = Commands->Find_Object(401137);
				v510->Send_Custom_Event(obj, v511, 0, 0, 0.0);
				v512 = Commands;
				v513 = Commands->Find_Object(401138);
				v512->Send_Custom_Event(obj, v513, 0, 0, 0.0);
				v514 = Commands;
				v515 = Commands->Find_Object(401139);
				v514->Send_Custom_Event(obj, v515, 0, 0, 0.0);
				v587 = 401130;
			}
			else
			{
				if (objId != 401114)
				{
					if (objId != 401123)
						goto LABEL_193;
					v453 = Commands;
					v454 = Commands->Find_Object(401124);
					v453->Send_Custom_Event(obj, v454, 0, 0, 0.0);
					v455 = Commands;
					v456 = Commands->Find_Object(401125);
					v455->Send_Custom_Event(obj, v456, 0, 0, 0.0);
					v457 = Commands;
					v458 = Commands->Find_Object(401126);
					v457->Send_Custom_Event(obj, v458, 0, 0, 0.0);
					v459 = Commands;
					v460 = Commands->Find_Object(401127);
					v459->Send_Custom_Event(obj, v460, 0, 0, 0.0);
					v461 = Commands;
					v462 = Commands->Find_Object(401128);
					v461->Send_Custom_Event(obj, v462, 0, 0, 0.0);
					v37 = Commands;
					v576 = 0.0;
					v566 = 401129;
					goto LABEL_164;
				}
				v437 = Commands;
				v438 = Commands->Find_Object(401115);
				v437->Send_Custom_Event(obj, v438, 0, 0, 0.0);
				v439 = Commands;
				v440 = Commands->Find_Object(401116);
				v439->Send_Custom_Event(obj, v440, 0, 0, 0.0);
				v441 = Commands;
				v442 = Commands->Find_Object(401117);
				v441->Send_Custom_Event(obj, v442, 0, 0, 0.0);
				v443 = Commands;
				v444 = Commands->Find_Object(401118);
				v443->Send_Custom_Event(obj, v444, 0, 0, 0.0);
				v445 = Commands;
				v446 = Commands->Find_Object(401119);
				v445->Send_Custom_Event(obj, v446, 0, 0, 0.0);
				v447 = Commands;
				v448 = Commands->Find_Object(401120);
				v447->Send_Custom_Event(obj, v448, 0, 0, 0.0);
				v449 = Commands;
				v450 = Commands->Find_Object(401121);
				v449->Send_Custom_Event(obj, v450, 0, 0, 0.0);
				v451 = Commands;
				v452 = Commands->Find_Object(401122);
				v451->Send_Custom_Event(obj, v452, 0, 0, 0.0);
				v588 = 401113;
			}
			goto LABEL_192;
		}
		if (objId == 405118)
			goto LABEL_190;
		if (objId > 405118)
		{
			if (objId == 405120)
			{
				v589 = 0x440E83D7;
				v590 = 0x4382BC29;
				v591 = 0xC25C0000;
				v558 = Commands->Create_Object("GDI_Mammoth_Tank_Player", (const Vector3 *)&v589);
				v559 = v558;
				if (!v558)
					goto LABEL_190;
				Commands->Set_Facing(v558, 45.0);
				v582 = "2";
				v581 = "M02_Player_Vehicle";
				v579 = v559;
				goto LABEL_189;
			}
			if (objId >= 405120)
			{
				if (objId != 405122)
					goto LABEL_193;
				Commands->Stop_All_Conversations();
				v560 = Commands->Create_Conversation("M02_SECONDARY_11_START", 100, 300.0, 1);
				Commands->Join_Conversation(0, v560, 1, 1, 1);
				v561 = Commands;
				v562 = this->base.base.base.vPtr->base.Owner((ScriptClass *)this);
				v561->Get_Position((Vector3 *)&v589, v562);
				v563 = v561->Get_A_Star((Vector3 *)&v589);
				v561->Join_Conversation(v563, v564, v560, 1, 0);
				Commands->Start_Conversation(v560, 0);
				Commands->Start_Timer(obj, retaddr, 6.0, 12);
				goto LABEL_192;
			}
			Commands->Stop_All_Conversations();
			v583 = 1;
			v575 = "M02_SECONDARY_03_START";
		}
		else if (objId == 405116)
		{
			Commands->Stop_All_Conversations();
			v583 = 1;
			v575 = "M02_OBELISK_APPROACH";
		}
		else
		{
			if (objId <= 405116)
			{
				if (objId != 401982)
					goto LABEL_193;
				Commands->Stop_All_Conversations();
				v550 = Commands->Create_Conversation("M02_SECONDARY_13_START", 100, 300.0, 1);
				Commands->Join_Conversation(0, v550, 1, 1, 1);
				v551 = Commands;
				v552 = this->base.base.base.vPtr->base.Owner((ScriptClass *)this);
				v551->Get_Position((Vector3 *)&v589, v552);
				v553 = v551->Get_A_Star((Vector3 *)&v589);
				v551->Join_Conversation(v553, v554, v550, 1, 0);
				Commands->Start_Conversation(v550, 0);
				v588 = 0x44687A3D;
				v589 = 0x448D8AE1;
				v590 = 0x428D0000;
				if (!(int)Commands->Create_Object("M02_Nod_Jet" + 4, (const Vector3 *)&v589))
					goto LABEL_190;
				v581 = &s1;
				v579 = (ScriptableGameObj *)"M02_Nod_Jet";
				goto LABEL_189;
			}
			Commands->Stop_All_Conversations();
			v583 = 1;
			v575 = "M02_PRIMARY_02_REMIND";
		}
	LABEL_183:
		v187 = Commands->Create_Conversation(v575, 100, 300.0, v583);
		Commands->Join_Conversation(0, v187, 1, 1, 1);
		v188 = Commands;
		v573 = 0;
		v571 = 1;
		v569 = v187;
		v189 = &v589;
	LABEL_184:
		v555 = this->base.base.base.vPtr->base.Owner((ScriptClass *)this);
		v188->Get_Position((Vector3 *)v189, v555);
		v556 = v188->Get_A_Star((Vector3 *)v189);
		v188->Join_Conversation(v556, v567, v569, v571, v573);
		Commands->Start_Conversation(v187, 0);
		goto LABEL_185;
	}
	if (objId == 400196)
	{
		this->field_1C = 1;
		Commands->Get_Position((Vector3 *)&v589, obj);
		Commands->Create_Logical_Sound(obj, 1000, (const Vector3 *)&v589, 70.0);
		Commands->Start_Timer(obj, retaddr, 1.0, 4);
		goto LABEL_193;
	}
	if (objId > 400196)
	{
		if (objId == 400270)
		{
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 9, 0.0);
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 8, 0.0);
			v190 = Commands;
			v191 = Commands->Find_Object(400275);
			v190->Send_Custom_Event(obj, v191, 0, 0, 0.0);
			v192 = Commands;
			v193 = Commands->Find_Object(401046);
			v192->Send_Custom_Event(obj, v193, 0, 0, 0.0);
			v194 = Commands;
			v195 = Commands->Find_Object(401047);
			v194->Send_Custom_Event(obj, v195, 0, 0, 0.0);
			v196 = Commands;
			v197 = Commands->Find_Object(401048);
			v196->Send_Custom_Event(obj, v197, 0, 0, 0.0);
			v198 = Commands;
			v199 = Commands->Find_Object(401049);
			v198->Send_Custom_Event(obj, v199, 0, 0, 0.0);
			v200 = Commands;
			v201 = Commands->Find_Object(401050);
			v200->Send_Custom_Event(obj, v201, 0, 0, 0.0);
			v202 = Commands;
			v203 = Commands->Find_Object(401051);
			v202->Send_Custom_Event(obj, v203, 0, 0, 0.0);
			v204 = Commands;
			v205 = Commands->Find_Object(401052);
			v204->Send_Custom_Event(obj, v205, 0, 0, 0.0);
			v206 = Commands;
			v207 = Commands->Find_Object(401053);
			v206->Send_Custom_Event(obj, v207, 0, 0, 0.0);
			Commands->Get_Position((Vector3 *)&v589, obj);
			Commands->Create_Logical_Sound(obj, 1059, (const Vector3 *)&v589, 1000.0);
			v587 = 400271;
			goto LABEL_185;
		}
		if (objId <= 400270)
		{
			if (objId == 400201)
			{
				this->field_1C = 1;
				Commands->Get_Position((Vector3 *)&v589, obj);
				Commands->Create_Logical_Sound(obj, 1000, (const Vector3 *)&v589, 70.0);
				Commands->Start_Timer(obj, retaddr, 1.0, 7);
				goto LABEL_193;
			}
			if (objId <= 400201)
			{
				if (objId == 400197)
				{
					this->field_1C = 1;
					Commands->Get_Position((Vector3 *)&v589, obj);
					Commands->Create_Logical_Sound(obj, 1000, (const Vector3 *)&v589, 70.0);
					Commands->Start_Timer(obj, retaddr, 1.0, 5);
				}
				else if (objId == 400198)
				{
					this->field_1C = 1;
					Commands->Get_Position((Vector3 *)&v589, obj);
					Commands->Create_Logical_Sound(obj, 1000, (const Vector3 *)&v589, 70.0);
					Commands->Start_Timer(obj, retaddr, 1.0, 6);
				}
				goto LABEL_193;
			}
			if (objId != 400268)
			{
				if (objId <= 400268)
				{
					if (objId == 400267)
					{
						Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 211, 0, 0.0);
						Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 3, 0.0);
						Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 4, 0.0);
						Commands->Get_Position((Vector3 *)&v589, obj);
						Commands->Create_Logical_Sound(obj, 1053, (const Vector3 *)&v589, 1000.0);
						Commands->Create_Logical_Sound(obj, 1102, (const Vector3 *)&v589, 1000.0);
						v139 = Commands;
						v140 = Commands->Find_Object(400210);
						v139->Send_Custom_Event(obj, v140, 0, 0, 0.0);
						v141 = Commands;
						v142 = Commands->Find_Object(400451);
						v141->Send_Custom_Event(obj, v142, 0, 0, 0.0);
						v143 = Commands;
						v144 = Commands->Find_Object(400452);
						v143->Send_Custom_Event(obj, v144, 0, 0, 0.0);
						v145 = Commands;
						v146 = Commands->Find_Object(400453);
						v145->Send_Custom_Event(obj, v146, 0, 0, 0.0);
						v147 = Commands;
						v148 = Commands->Find_Object(400499);
						v147->Send_Custom_Event(obj, v148, 0, 0, 0.0);
						v149 = Commands;
						v150 = Commands->Find_Object(400500);
						v149->Send_Custom_Event(obj, v150, 0, 0, 0.0);
						v151 = Commands;
						v152 = Commands->Find_Object(401025);
						v151->Send_Custom_Event(obj, v152, 0, 0, 0.0);
						v153 = Commands;
						v154 = Commands->Find_Object(401026);
						v153->Send_Custom_Event(obj, v154, 0, 0, 0.0);
						v155 = Commands;
						v156 = Commands->Find_Object(401027);
						v155->Send_Custom_Event(obj, v156, 0, 0, 0.0);
						v157 = Commands;
						v158 = Commands->Find_Object(401028);
						v157->Send_Custom_Event(obj, v158, 0, 0, 0.0);
						Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 112, 4, 0.0);
						Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 111, 4, 0.0);
						M02_Objective_Zone::Create_Buggy((M02_Objective_Zone *)retaddr, 4);
						v159 = Commands;
						v160 = Commands->Find_Object(401018);
						v159->Send_Custom_Event(obj, v160, 0, 0, 0.0);
						Commands->Start_Timer(obj, retaddr, 0.1, 10);
						v5 = 1;
						v587 = 400187;
					}
					goto LABEL_193;
				}
				Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 202, 0, 0.0);
				Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 203, 0, 0.0);
				Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 217, 0, 0.0);
				Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 6, 0.0);
				Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 9, 0.0);
				v179 = Commands;
				v180 = Commands->Find_Object(400212);
				v179->Send_Custom_Event(obj, v180, 0, 0, 0.0);
				v181 = Commands;
				v182 = Commands->Find_Object(400981);
				v181->Send_Custom_Event(obj, v182, 0, 0, 0.0);
				v183 = Commands;
				v184 = Commands->Find_Object(400982);
				v183->Send_Custom_Event(obj, v184, 0, 0, 0.0);
				v185 = Commands;
				v186 = Commands->Find_Object(400983);
				v185->Send_Custom_Event(obj, v186, 0, 0, 0.0);
				Commands->Get_Position((Vector3 *)&v589, obj);
				Commands->Create_Logical_Sound(obj, 1056, (const Vector3 *)&v589, 1000.0);
				M02_Objective_Zone::Call_GDI_Soldiers((M02_Objective_Zone *)retaddr, 9);
				Commands->Stop_All_Conversations();
				v187 = Commands->Create_Conversation("M02_PRIMARY_01_GUIDE", 100, 300.0, 1);
				Commands->Join_Conversation(0, v187, 1, 1, 1);
				v188 = Commands;
				v573 = 0;
				v571 = 0;
				v569 = 1;
				v567 = v187;
				v189 = &v592;
				goto LABEL_184;
			}
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 4, 0.0);
			Commands->Get_Position((Vector3 *)&v589, obj);
			((void(__cdecl *)(ScriptableGameObj *))Commands->Create_Logical_Sound)(obj);
			v161 = Commands;
			v162 = Commands->Find_Object(400552);
			v161->Send_Custom_Event(obj, v162, 0, 0, 0.0);
			v163 = Commands;
			v164 = Commands->Find_Object(400553);
			v163->Send_Custom_Event(obj, v164, 0, 0, 0.0);
			v165 = Commands;
			v166 = Commands->Find_Object(400560);
			v165->Send_Custom_Event(obj, v166, 0, 0, 0.0);
			v167 = Commands;
			v168 = Commands->Find_Object(400555);
			v167->Send_Custom_Event(obj, v168, 0, 0, 0.0);
			v169 = Commands;
			v170 = Commands->Find_Object(400558);
			v169->Send_Custom_Event(obj, v170, 0, 0, 0.0);
			v171 = Commands;
			v172 = Commands->Find_Object(400559);
			v171->Send_Custom_Event(obj, v172, 0, 0, 0.0);
			v173 = Commands;
			v174 = Commands->Find_Object(400557);
			v173->Send_Custom_Event(obj, v174, 0, 0, 0.0);
			v175 = Commands;
			v176 = Commands->Find_Object(400554);
			v175->Send_Custom_Event(obj, v176, 0, 0, 0.0);
			v177 = Commands;
			v178 = Commands->Find_Object(400556);
			v177->Send_Custom_Event(obj, v178, 0, 0, 0.0);
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 24, 0, COERCE_FLOAT(1054));
		LABEL_192:
			v5 = 1;
			goto LABEL_193;
		}
		if (objId == 400274)
		{
			v255 = Commands->Find_Object(1153846);
			v585 = v255;
			if (!v255)
				goto LABEL_193;
			Commands->Get_Health(v255);
			if (v3 <= 0.0)
			{
				Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 11, 0.0);
				Commands->Get_Position((Vector3 *)&v589, obj);
				Commands->Create_Logical_Sound(obj, 1061, (const Vector3 *)&v589, 1000.0);
				Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 114, 10, 0.0);
				Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 102, 10, 0.0);
				v5 = 1;
			}
			v572 = 0;
			v570 = 1;
			v568 = v585;
			goto LABEL_136;
		}
		if (objId > 400274)
		{
			if (objId == 400501)
			{
				v260 = Commands;
				v261 = Commands->Find_Object(400503);
				v260->Send_Custom_Event(obj, v261, 0, 0, 0.0);
				v262 = Commands;
				v263 = Commands->Find_Object(400504);
				v262->Send_Custom_Event(obj, v263, 0, 0, 0.0);
				v264 = Commands;
				v265 = Commands->Find_Object(400505);
				v264->Send_Custom_Event(obj, v265, 0, 0, 0.0);
				v266 = Commands;
				v267 = Commands->Find_Object(400506);
				v266->Send_Custom_Event(obj, v267, 0, 0, 0.0);
				v268 = Commands;
				v269 = Commands->Find_Object(400507);
				v268->Send_Custom_Event(obj, v269, 0, 0, 0.0);
				v270 = Commands;
				v271 = Commands->Find_Object(400508);
				v270->Send_Custom_Event(obj, v271, 0, 0, 0.0);
				v272 = Commands;
				v273 = Commands->Find_Object(400509);
				v272->Send_Custom_Event(obj, v273, 0, 0, 0.0);
				v274 = Commands;
				v275 = Commands->Find_Object(400510);
				v274->Send_Custom_Event(obj, v275, 0, 0, 0.0);
				Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 24, 0.0);
				Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 214, 0, 0.0);
				v588 = 400502;
			}
			else
			{
				if (objId <= 400501)
				{
					if (objId != 400316)
						goto LABEL_193;
					Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 0, 0.0);
					Commands->Get_Position((Vector3 *)&v589, obj);
					Commands->Create_Logical_Sound(obj, 1050, (const Vector3 *)&v589, 1000.0);
					v256 = Commands;
					v257 = Commands->Find_Object(400317);
					v256->Send_Custom_Event(obj, v257, 0, 0, 0.0);
					v258 = Commands;
					v259 = Commands->Find_Object(400318);
					v258->Send_Custom_Event(obj, v259, 0, 0, 0.0);
					v37 = Commands;
					goto LABEL_164;
				}
				if (objId != 400502)
					goto LABEL_193;
				Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 24, 0.0);
				v276 = Commands;
				v277 = Commands->Find_Object(400503);
				v276->Send_Custom_Event(obj, v277, 0, 0, 0.0);
				v278 = Commands;
				v279 = Commands->Find_Object(400504);
				v278->Send_Custom_Event(obj, v279, 0, 0, 0.0);
				v280 = Commands;
				v281 = Commands->Find_Object(400505);
				v280->Send_Custom_Event(obj, v281, 0, 0, 0.0);
				v282 = Commands;
				v283 = Commands->Find_Object(400506);
				v282->Send_Custom_Event(obj, v283, 0, 0, 0.0);
				v284 = Commands;
				v285 = Commands->Find_Object(400507);
				v284->Send_Custom_Event(obj, v285, 0, 0, 0.0);
				v286 = Commands;
				v287 = Commands->Find_Object(400508);
				v286->Send_Custom_Event(obj, v287, 0, 0, 0.0);
				v288 = Commands;
				v289 = Commands->Find_Object(400509);
				v288->Send_Custom_Event(obj, v289, 0, 0, 0.0);
				v290 = Commands;
				v291 = Commands->Find_Object(400510);
				v290->Send_Custom_Event(obj, v291, 0, 0, 0.0);
				Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 214, 0, 0.0);
				v588 = 400501;
			}
			goto LABEL_192;
		}
		if (objId == 400272)
		{
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 8, 0.0);
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 10, 0.0);
			Commands->Get_Position((Vector3 *)&v589, obj);
			Commands->Create_Logical_Sound(obj, 1058, (const Vector3 *)&v589, 1000.0);
			v227 = Commands;
			v228 = Commands->Find_Object(400213);
			v227->Send_Custom_Event(obj, v228, 0, 0, 0.0);
			v229 = Commands;
			v230 = Commands->Find_Object(400984);
			v229->Send_Custom_Event(obj, v230, 0, 0, 0.0);
			v231 = Commands;
			v232 = Commands->Find_Object(401054);
			v231->Send_Custom_Event(obj, v232, 0, 0, 0.0);
			v233 = Commands;
			v234 = Commands->Find_Object(401055);
			v233->Send_Custom_Event(obj, v234, 0, 0, 0.0);
			v235 = Commands;
			v236 = Commands->Find_Object(401056);
			v235->Send_Custom_Event(obj, v236, 0, 0, 0.0);
			v37 = Commands;
			goto LABEL_164;
		}
		if (objId > 400272)
		{
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 10, 0.0);
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 11, 0.0);
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 114, 11, 0.0);
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 102, 11, 0.0);
			v237 = Commands;
			v238 = Commands->Find_Object(400276);
			v237->Send_Custom_Event(obj, v238, 0, 0, 0.0);
			v239 = Commands;
			v240 = Commands->Find_Object(400199);
			v239->Send_Custom_Event(obj, v240, 0, 0, 0.0);
			v241 = Commands;
			v242 = Commands->Find_Object(401065);
			v241->Send_Custom_Event(obj, v242, 0, 0, 0.0);
			v243 = Commands;
			v244 = Commands->Find_Object(401058);
			v243->Send_Custom_Event(obj, v244, 0, 0, 0.0);
			v245 = Commands;
			v246 = Commands->Find_Object(401059);
			v245->Send_Custom_Event(obj, v246, 0, 0, 0.0);
			v247 = Commands;
			v248 = Commands->Find_Object(401060);
			v247->Send_Custom_Event(obj, v248, 0, 0, 0.0);
			v249 = Commands;
			v250 = Commands->Find_Object(401061);
			v249->Send_Custom_Event(obj, v250, 0, 0, 0.0);
			v251 = Commands;
			v252 = Commands->Find_Object(401063);
			v251->Send_Custom_Event(obj, v252, 0, 0, 0.0);
			v253 = Commands;
			v254 = Commands->Find_Object(401064);
			v253->Send_Custom_Event(obj, v254, 0, 0, 0.0);
			Commands->Get_Position((Vector3 *)&v589, obj);
			v580 = (const Vector3 *)&v589;
			v577 = 1060;
			goto LABEL_131;
		}
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 9, 0.0);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 8, 0.0);
		v208 = Commands;
		v209 = Commands->Find_Object(400275);
		v208->Send_Custom_Event(obj, v209, 0, 0, 0.0);
		v210 = Commands;
		v211 = Commands->Find_Object(401046);
		v210->Send_Custom_Event(obj, v211, 0, 0, 0.0);
		v212 = Commands;
		v213 = Commands->Find_Object(401047);
		v212->Send_Custom_Event(obj, v213, 0, 0, 0.0);
		v214 = Commands;
		v215 = Commands->Find_Object(401048);
		v214->Send_Custom_Event(obj, v215, 0, 0, 0.0);
		v216 = Commands;
		v217 = Commands->Find_Object(401049);
		v216->Send_Custom_Event(obj, v217, 0, 0, 0.0);
		v218 = Commands;
		v219 = Commands->Find_Object(401050);
		v218->Send_Custom_Event(obj, v219, 0, 0, 0.0);
		v220 = Commands;
		v221 = Commands->Find_Object(401051);
		v220->Send_Custom_Event(obj, v221, 0, 0, 0.0);
		v222 = Commands;
		v223 = Commands->Find_Object(401052);
		v222->Send_Custom_Event(obj, v223, 0, 0, 0.0);
		v224 = Commands;
		v225 = Commands->Find_Object(401053);
		v224->Send_Custom_Event(obj, v225, 0, 0, 0.0);
		Commands->Get_Position((Vector3 *)&v589, obj);
		Commands->Create_Logical_Sound(obj, 1059, (const Vector3 *)&v589, 1000.0);
		v226 = Commands->Find_Object(303203);
		if (v226)
			Commands->Destroy_Object(v226);
		v587 = 400270;
	LABEL_190:
		v5 = 1;
		goto LABEL_193;
	}
	if (objId == 400187)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 211, 0, 0.0);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 3, 0.0);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 4, 0.0);
		Commands->Get_Position((Vector3 *)&v589, obj);
		Commands->Create_Logical_Sound(obj, 1102, (const Vector3 *)&v589, 1000.0);
		Commands->Create_Logical_Sound(obj, 1053, (const Vector3 *)&v589, 1000.0);
		v78 = Commands;
		v79 = Commands->Find_Object(400210);
		v78->Send_Custom_Event(obj, v79, 0, 0, 0.0);
		v80 = Commands;
		v81 = Commands->Find_Object(400451);
		v80->Send_Custom_Event(obj, v81, 0, 0, 0.0);
		v82 = Commands;
		v83 = Commands->Find_Object(400452);
		v82->Send_Custom_Event(obj, v83, 0, 0, 0.0);
		v84 = Commands;
		v85 = Commands->Find_Object(400453);
		v84->Send_Custom_Event(obj, v85, 0, 0, 0.0);
		v86 = Commands;
		v87 = Commands->Find_Object(400499);
		v86->Send_Custom_Event(obj, v87, 0, 0, 0.0);
		v88 = Commands;
		v89 = Commands->Find_Object(400500);
		v88->Send_Custom_Event(obj, v89, 0, 0, 0.0);
		v90 = Commands;
		v91 = Commands->Find_Object(401025);
		v90->Send_Custom_Event(obj, v91, 0, 0, 0.0);
		v92 = Commands;
		v93 = Commands->Find_Object(401026);
		v92->Send_Custom_Event(obj, v93, 0, 0, 0.0);
		v94 = Commands;
		v95 = Commands->Find_Object(401027);
		v94->Send_Custom_Event(obj, v95, 0, 0, 0.0);
		v96 = Commands;
		v97 = Commands->Find_Object(401028);
		v96->Send_Custom_Event(obj, v97, 0, 0, 0.0);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 112, 4, 0.0);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 111, 4, 0.0);
		M02_Objective_Zone::Create_Buggy((M02_Objective_Zone *)retaddr, 4);
		v98 = Commands;
		v99 = Commands->Find_Object(401018);
		v98->Send_Custom_Event(obj, v99, 0, 0, 0.0);
		Commands->Start_Timer(obj, retaddr, 0.1, 10);
		v5 = 1;
		v587 = 400267;
		goto LABEL_193;
	}
	if (objId <= 400187)
	{
		if (objId == 304006)
		{
			if (!this->field_1D)
			{
				this->field_1D = 1;
				v587 = Commands->Create_Conversation("MX2DSGN_DSGN0001", 100, 300.0, 1);
				Commands->Join_Conversation(0, v587, 1, 1, 1);
				v15 = Commands;
				v16 = this->base.base.base.vPtr->base.Owner((ScriptClass *)this);
				v15->Get_Position((Vector3 *)&v589, v16);
				v17 = v15->Get_A_Star((Vector3 *)&v589);
				v15->Join_Conversation(v17, v18, v587, 1, 0);
				Commands->Start_Conversation(v586, 0);
				v588 = 0x3E48B439;
				v589 = 0x3F61CAC1;
				v590 = 0x3E48B439;
				Commands->Set_HUD_Help_Text(8287, (Vector3 *)&v589);
			}
			goto LABEL_193;
		}
		if (objId <= 304006)
		{
			if (objId == 301601)
			{
				v7 = Commands;
				v8 = this->base.base.base.vPtr->base.Owner((ScriptClass *)this);
				v7->Get_Position((Vector3 *)&v593, v8);
				v9 = v7->Get_A_Star((Vector3 *)&v593);
				if (v7->Has_Key(v9, v574) == 1)
					goto LABEL_193;
				Commands->Stop_All_Conversations();
				v10 = Commands->Create_Conversation("M02_BAY_DOOR_WARNING", 100, 300.0, 1);
				Commands->Join_Conversation(0, v10, 1, 1, 1);
				v11 = Commands;
				v12 = (int)retaddr->base.vPtr->base.Owner(retaddr);
				v11->Get_Position((Vector3 *)&v588, (ScriptableGameObj *)v12);
				v13 = v11->Get_A_Star((Vector3 *)&v588);
				v11->Join_Conversation(v13, v14, v10, 1, 0);
				Commands->Start_Conversation(0, v557);
				goto LABEL_185;
			}
			if (objId != 303203)
				goto LABEL_193;
			Commands->Stop_All_Conversations();
			v583 = 1;
			v575 = "M02_PRIMARY_03_GUIDE";
			goto LABEL_183;
		}
		if (objId != 400185)
		{
			if (objId <= 400185)
			{
				if (objId != 400184)
					goto LABEL_193;
				Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 206, 0, 0.0);
				Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 207, 0, 0.0);
				Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 0, 0.0);
				M02_Objective_Zone::Call_GDI_Soldiers(this, 0);
				M02_Objective_Zone::Create_Apache(this, 0);
				v19 = Commands;
				v20 = Commands->Find_Object(400286);
				v19->Send_Custom_Event(obj, v20, 0, 0, 0.0);
				v21 = Commands;
				v22 = Commands->Find_Object(400287);
				v21->Send_Custom_Event(obj, v22, 0, 0, 0.0);
				v23 = Commands;
				v24 = Commands->Find_Object(400288);
				v23->Send_Custom_Event(obj, v24, 0, 0, 0.0);
				v25 = Commands;
				v26 = Commands->Find_Object(400292);
				v25->Send_Custom_Event(obj, v26, 0, 0, 0.0);
				v27 = Commands;
				v28 = Commands->Find_Object(400293);
				v27->Send_Custom_Event(obj, v28, 0, 0, 0.0);
				v29 = Commands;
				v30 = Commands->Find_Object(400285);
				v29->Send_Custom_Event(obj, v30, 0, 0, 0.0);
				v31 = Commands;
				v32 = Commands->Find_Object(400206);
				v31->Send_Custom_Event(obj, v32, 0, 0, 0.0);
				v33 = Commands;
				v34 = Commands->Find_Object(401004);
				v33->Send_Custom_Event(obj, v34, 0, 0, 0.0);
				v35 = Commands;
				v36 = Commands->Find_Object(401005);
				v35->Send_Custom_Event(obj, v36, 0, 0, 0.0);
				v37 = Commands;
				v576 = 0.0;
				v463 = Commands->Find_Object(401006);
				goto LABEL_165;
			}
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 210, 0, 0.0);
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 221, 0, 0.0);
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 1, 0.0);
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 2, 0.0);
			Commands->Get_Position((Vector3 *)&v589, obj);
			Commands->Create_Logical_Sound(obj, 1051, (const Vector3 *)&v589, 1000.0);
			v60 = Commands;
			v61 = Commands->Find_Object(400372);
			v60->Send_Custom_Event(obj, v61, 0, 0, 0.0);
			v62 = Commands;
			v63 = Commands->Find_Object(400373);
			v62->Send_Custom_Event(obj, v63, 0, 0, 0.0);
			v64 = Commands;
			v65 = Commands->Find_Object(400374);
			v64->Send_Custom_Event(obj, v65, 0, 0, 0.0);
			v66 = Commands;
			v67 = Commands->Find_Object(401167);
			v66->Send_Custom_Event(obj, v67, 0, 0, 0.0);
			v68 = Commands;
			v69 = Commands->Find_Object(400208);
			v68->Send_Custom_Event(obj, v69, 0, 0, 0.0);
			v70 = Commands;
			v71 = Commands->Find_Object(401014);
			v70->Send_Custom_Event(obj, v71, 0, 0, 0.0);
			v72 = Commands;
			v73 = Commands->Find_Object(401015);
			v72->Send_Custom_Event(obj, v73, 0, 0, 0.0);
			v74 = Commands;
			v75 = Commands->Find_Object(401016);
			v74->Send_Custom_Event(obj, v75, 0, 0, 0.0);
			v76 = Commands;
			v77 = Commands->Find_Object(401017);
			v76->Send_Custom_Event(obj, v77, 0, 0, 0.0);
			M02_Objective_Zone::Create_Buggy((M02_Objective_Zone *)retaddr, 2);
			M02_Objective_Zone::Create_Sakura((M02_Objective_Zone *)retaddr, 2);
		LABEL_185:
			v5 = 1;
			goto LABEL_193;
		}
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 208, 0, 0.0);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 209, 0, 0.0);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 1, 0.0);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 112, 1, 0.0);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 111, 1, 0.0);
		Commands->Get_Position((Vector3 *)&v589, obj);
		Commands->Create_Logical_Sound(obj, 1100, (const Vector3 *)&v589, 1000.0);
		Commands->Create_Logical_Sound(obj, 1073, (const Vector3 *)&v589, 1000.0);
		v38 = Commands;
		v39 = Commands->Find_Object(400342);
		v38->Send_Custom_Event(obj, v39, 0, 0, 0.0);
		v40 = Commands;
		v41 = Commands->Find_Object(400343);
		v40->Send_Custom_Event(obj, v41, 0, 0, 0.0);
		v42 = Commands;
		v43 = Commands->Find_Object(400347);
		v42->Send_Custom_Event(obj, v43, 0, 0, 0.0);
		v44 = Commands;
		v45 = Commands->Find_Object(400332);
		v44->Send_Custom_Event(obj, v45, 0, 0, 0.0);
		v46 = Commands;
		v47 = Commands->Find_Object(400340);
		v46->Send_Custom_Event(obj, v47, 0, 0, 0.0);
		v48 = Commands;
		v49 = Commands->Find_Object(400348);
		v48->Send_Custom_Event(obj, v49, 0, 0, 0.0);
		v50 = Commands;
		v51 = Commands->Find_Object(400344);
		v50->Send_Custom_Event(obj, v51, 0, 0, 0.0);
		v52 = Commands;
		v53 = Commands->Find_Object(400345);
		v52->Send_Custom_Event(obj, v53, 0, 0, 0.0);
		v54 = Commands;
		v55 = Commands->Find_Object(400333);
		v54->Send_Custom_Event(obj, v55, 0, 0, 0.0);
		v56 = Commands;
		v57 = Commands->Find_Object(400207);
		v56->Send_Custom_Event(obj, v57, 0, 0, 0.0);
		v58 = Commands;
		v59 = Commands->Find_Object(400371);
		v58->Send_Custom_Event(obj, v59, 0, 0, 0.0);
		v37 = Commands;
	LABEL_164:
		v463 = v37->Find_Object(v566);
	LABEL_165:
		v37->Send_Custom_Event(obj, v463, 0, 0, v576);
		goto LABEL_192;
	}
	if (objId == 400192)
	{
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 11, 0.0);
		Commands->Get_Position((Vector3 *)&v589, obj);
		v580 = (const Vector3 *)&v589;
		v577 = 1061;
	LABEL_131:
		Commands->Create_Logical_Sound(obj, v577, v580, 1000.0);
		goto LABEL_185;
	}
	if (objId <= 400192)
	{
		if (objId != 400188)
		{
			if (objId != 400189)
				goto LABEL_193;
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 215, 0, 0.0);
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 216, 0, 0.0);
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 6, 0.0);
			v114 = Commands;
			v115 = Commands->Find_Object(400211);
			v114->Send_Custom_Event(obj, v115, 0, 0, 0.0);
			v116 = Commands;
			v117 = Commands->Find_Object(400976);
			v116->Send_Custom_Event(obj, v117, 0, 0, 0.0);
			v118 = Commands;
			v119 = Commands->Find_Object(400975);
			v118->Send_Custom_Event(obj, v119, 0, 0, 0.0);
			v120 = Commands;
			v121 = Commands->Find_Object(400977);
			v120->Send_Custom_Event(obj, v121, 0, 0, 0.0);
			v122 = Commands;
			v123 = Commands->Find_Object(400978);
			v122->Send_Custom_Event(obj, v123, 0, 0, 0.0);
			v124 = Commands;
			v125 = Commands->Find_Object(400979);
			v124->Send_Custom_Event(obj, v125, 0, 0, 0.0);
			v126 = Commands;
			v127 = Commands->Find_Object(400980);
			v126->Send_Custom_Event(obj, v127, 0, 0, 0.0);
			v128 = Commands;
			v129 = Commands->Find_Object(401036);
			v128->Send_Custom_Event(obj, v129, 0, 0, 0.0);
			v130 = Commands;
			v131 = Commands->Find_Object(401037);
			v130->Send_Custom_Event(obj, v131, 0, 0, 0.0);
			v132 = Commands;
			v133 = Commands->Find_Object(401038);
			v132->Send_Custom_Event(obj, v133, 0, 0, 0.0);
			v37 = Commands;
			v576 = 0.0;
			v463 = Commands->Find_Object(401045);
			goto LABEL_165;
		}
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 212, 0, 0.0);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 213, 0, 0.0);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 105, 2, 0.0);
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 104, 3, 0.0);
		Commands->Get_Position((Vector3 *)&v589, obj);
		Commands->Create_Logical_Sound(obj, 1052, (const Vector3 *)&v589, 1000.0);
		v592 = 0x43E8D28F;
		v593 = 0x4432F70A;
		v594 = 0xC16EF5C3;
		v100 = Commands->Create_Object("Nod_Light_Tank", (const Vector3 *)&v592);
		v101 = v100;
		if (v100)
		{
			Commands->Set_Facing(v100, -10.0);
			Commands->Attach_Script(v101, "M02_Nod_Vehicle", "3");
		}
		v102 = Commands;
		v103 = Commands->Find_Object(400410);
		v102->Send_Custom_Event(obj, v103, 0, 0, 0.0);
		v104 = Commands;
		v105 = Commands->Find_Object(400209);
		v104->Send_Custom_Event(obj, v105, 0, 0, 0.0);
		v106 = Commands;
		v107 = Commands->Find_Object(400411);
		v106->Send_Custom_Event(obj, v107, 0, 0, 0.0);
		v108 = Commands;
		v109 = Commands->Find_Object(400412);
		v108->Send_Custom_Event(obj, v109, 0, 0, 0.0);
		v110 = Commands;
		v111 = Commands->Find_Object(400413);
		v110->Send_Custom_Event(obj, v111, 0, 0, 0.0);
		v112 = Commands;
		v113 = Commands->Find_Object(400414);
		v112->Send_Custom_Event(obj, v113, 0, 0, 0.0);
		v37 = Commands;
		goto LABEL_164;
	}
	if (objId != 400194)
	{
		if (objId > 400194)
		{
			this->field_1C = 1;
			Commands->Get_Position((Vector3 *)&v589, obj);
			Commands->Create_Logical_Sound(obj, 1000, (const Vector3 *)&v589, 70.0);
			Commands->Start_Timer(obj, retaddr, 1.0, 3);
		}
		else
		{
			v589 = 0;
			v590 = 0;
			v591 = 0;
			v134 = Commands->Create_Object("Invisible_Object", (const Vector3 *)&v589);
			if (v134)
			{
				v135 = Commands;
				v136 = this->base.base.base.vPtr->base.Owner((ScriptClass *)this);
				v135->Get_Position((Vector3 *)&v589, v136);
				v137 = v135->Get_A_Star((Vector3 *)&v589);
				v135->Control_Enable(v137, v578);
				Commands->Start_Timer(obj, retaddr, 1.0, 9);
				v138 = Commands->Find_Object(1111112);
				if (v138)
					Commands->Send_Custom_Event(obj, v138, 1000, 1002, 25.0);
				Commands->Attach_Script(v584, "Test_Cinematic", "X2K_Midtro.txt");
			}
		}
		goto LABEL_193;
	}
	if (!this->field_1D)
	{
		this->field_1D = 1;
		Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, 205, 1, 0.0);
		Commands->Mission_Complete(1);
		v572 = 21;
		v570 = 105;
		v568 = M02ObjectiveControllerObj;
	LABEL_136:
		Commands->Send_Custom_Event(obj, v568, v570, v572, 0.0);
	}
LABEL_193:
	if (v587)
	{
		v565 = Commands->Find_Object(v587);
		if (v565)
			Commands->Destroy_Object(v565);
	}
	if (v5)
		Commands->Destroy_Object(obj);
		*/
}

void M02_Objective_Zone::Send_and_Destroy(GameObject *obj, int type)
{
	if (this->field_1C)
	{
		GameObject *M02ObjectiveControllerObj = Commands->Find_Object(1111112);
		if (M02ObjectiveControllerObj)
		{
			Commands->Send_Custom_Event(obj, M02ObjectiveControllerObj, type, 1, 0.0f);

			this->field_1C = false;

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
	else if (!this->field_1E)
	{
		this->field_1E = true;

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