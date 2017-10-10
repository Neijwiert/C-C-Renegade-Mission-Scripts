#pragma once

#include <scripts.h>

/*
M03 -> 300052 1147534 1147533 1147531 1147530 1147529 1146701 1146700 300009 300028 2013085 1144724 1144723 1144722 1144721 1144720 1144719 1144718 1144717 1144716 1144715 1144714 1144713 1144712 1144711 1144653 300032 1141162 300043 300034 300033 1144452 1144451 1144450 1144672 1144673 1144682 1144500 300010
*/
class DLS_Volcano_Stumble : public ScriptImpClass
{
	public:
		virtual	void Register_Auto_Save_Variables();

	private:
		virtual void Created(GameObject *obj);
		virtual void Sound_Heard(GameObject *obj, const CombatSound & sound);
		virtual void Action_Complete(GameObject *obj, int action_id, ActionCompleteReason complete_reason);

		bool field_1C;
};