#pragma once

#include <scripts.h>

class M02_Objective_Controller : public ScriptImpClass
{
	public:
		virtual	void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);
		virtual void Timer_Expired(GameObject *obj, int number);

		void Add_An_Objective(GameObject *obj, int a3);

		DWORD field_1C;
		DWORD field_20;
		DWORD field_24;
		bool field_28;
};