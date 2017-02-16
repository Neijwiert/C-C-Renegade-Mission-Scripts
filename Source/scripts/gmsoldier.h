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
#ifndef TT_INCLUDE_GMSOLDIER_H
#define TT_INCLUDE_GMSOLDIER_H
class SSGM_Soldier : public ScriptImpClass {
public:
	unsigned int RefillTime;
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
	void Detach(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float amount);
	void Killed(GameObject *obj,GameObject *killer);
	void Custom(GameObject *obj, int message, int param, GameObject *sender);
	void Register_Auto_Save_Variables();
	void Timer_Expired(GameObject *obj, int number);
	int kills;
};

class SSGM_Log_Key : public JFW_Key_Hook_Base {
	void Created(GameObject *obj);
	void KeyHook();
};

class SSGM_C4_Key : public JFW_Key_Hook_Base {
	void Created(GameObject *obj);
	void KeyHook();
};

class SSGM_Bind_Key : public JFW_Key_Hook_Base {
	void Created(GameObject *obj);
	void KeyHook();
};

class SSGM_BL_Key : public JFW_Key_Hook_Base {
	void Created(GameObject *obj);
	void KeyHook();
};

#endif