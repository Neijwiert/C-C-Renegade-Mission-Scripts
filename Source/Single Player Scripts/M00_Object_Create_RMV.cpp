#include "General.h"
#include "M00_Object_Create_RMV.h"

/*
M03 -> 1111000
*/
void M00_Object_Create_RMV::Register_Auto_Save_Variables()
{
	Auto_Save_Variable(&this->field_1C, sizeof(this->field_1C), 1);
}

void M00_Object_Create_RMV::Created(GameObject *obj)
{

}

void M00_Object_Create_RMV::Custom(GameObject *obj, int type, int param, GameObject *sender)
{
	if (type == Get_Int_Parameter("Receive_Type") && param == Get_Int_Parameter("Receive_Param_On"))
	{
		Vector3 location = Get_Vector3_Parameter("Location");
		const char *objectToCreate = Get_Parameter("Object_To_Create");

		GameObject *createdObject = Commands->Create_Object(objectToCreate, location);
		if (createdObject)
		{
			float facing = Get_Float_Parameter("Facing");
			Commands->Set_Facing(createdObject, facing);
		}
	}
}

ScriptRegistrant<M00_Object_Create_RMV> M00_Object_Create_RMVRegistrant("M00_Object_Create_RMV", "Start_Now=0:int, Receive_Type:int, Receive_Param_On=1:int, Object_To_Create:string, Location:vector3, Facing=0.00:float, Debug_Mode=0:int");