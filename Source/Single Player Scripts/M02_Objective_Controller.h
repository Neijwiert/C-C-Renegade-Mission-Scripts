#pragma once

#include <scripts.h>

/*
M02 -> 1111112
*/
class M02_Objective_Controller : public ScriptImpClass
{
	public:
		virtual	void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Custom(GameObject *obj, int type, int param, GameObject *sender);
		virtual void Timer_Expired(GameObject *obj, int number);

		void Add_An_Objective(GameObject *obj, int objectiveId);

		int nodConvoyTruckCount;
		int area0GDIDeathsSinceLastDrop;
		int area9GDIDeathsSinceLastDrop;
		bool havocCalledForReinforcementsArea0;
};