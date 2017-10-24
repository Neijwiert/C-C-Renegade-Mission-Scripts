#pragma once

#include <scripts.h>

/*
M03 -> 1111000
*/
class M00_Object_Create_RMV : public ScriptImpClass
{
	public:
		virtual	void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);

		bool field_1C; // Never used
};