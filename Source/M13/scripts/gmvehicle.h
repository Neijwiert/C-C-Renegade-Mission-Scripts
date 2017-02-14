/*	Renegade Scripts.dll
	Copyright 2013 Tiberian Technologies

	This file is part of the Renegade scripts.dll
	The Renegade scripts.dll is free software; you can redistribute it and/or modify it under
	the terms of the GNU General Public License as published by the Free
	Software Foundation; either version 2, or (at your option) any later
	version. See the file COPYING for more details.
	In addition, an exemption is given to allow Run Time Dynamic Linking of this code with any closed source module that does not contain code covered by this licence.
	Only the source code to the module(s) containing the licenced code has to be released.
*/
#ifndef TT_INCLUDE_GMVEHICLE_H
#define TT_INCLUDE_GMVEHICLE_H
class VehicleGameObj;
class SSGM_Vehicle : public ScriptImpClass {
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int type,int param,GameObject *sender);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Killed(GameObject *obj,GameObject *killer);
	void Destroyed(GameObject *obj);
	void Detach(GameObject *obj);
	void Timer_Expired(GameObject *obj, int number);
	bool triggerrepairing;
	bool triggerrepaired;
public:
	void Register_Auto_Save_Variables();
	int OwnerID;
	int IconModelID;
};

class SSGM_Vehicle_Wreckage : public ScriptImpClass {
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Timer_Expired(GameObject *obj, int number);
};

SCRIPTS_API VehicleGameObj *Find_My_Vehicle(GameObject *obj);
SCRIPTS_API void Bind_Vehicle(GameObject *obj);
SCRIPTS_API void Lock_Vehicle(GameObject *obj);
SCRIPTS_API void Unlock_Vehicle(GameObject *obj, bool silent = false);
SCRIPTS_API void Unbind_Vehicle(GameObject *obj, bool silent = false);
SCRIPTS_API void Kick_Vehicle_Driver(GameObject *obj);
#endif