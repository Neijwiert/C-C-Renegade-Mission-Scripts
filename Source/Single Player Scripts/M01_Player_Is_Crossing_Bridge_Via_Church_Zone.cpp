#include "General.h"
#include "M01_Player_Is_Crossing_Bridge_Via_Church_Zone.h"

/*
M01 -> 100972
*/
void M01_Player_Is_Crossing_Bridge_Via_Church_Zone::Entered(GameObject *obj, GameObject *enterer)
{
	Vector3 pos = Commands->Get_Position(obj);
	if (enterer == Commands->Get_A_Star(pos))
	{
		GameObject *M01MissionControllerJDGObj = Commands->Find_Object(100376);
		Commands->Send_Custom_Event(obj, M01MissionControllerJDGObj, 0, 153, 0.0f);

		GameObject *M01TailGunRunSpawnerControllerJDGObj = Commands->Find_Object(102346);
		if (M01TailGunRunSpawnerControllerJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01TailGunRunSpawnerControllerJDGObj, 0, 70, 0.0f);
		}

		GameObject *M01ChurchAreaSpawnerControllerJDGObj = Commands->Find_Object(102350);
		if (M01ChurchAreaSpawnerControllerJDGObj)
		{
			Commands->Send_Custom_Event(obj, M01ChurchAreaSpawnerControllerJDGObj, 0, 68, 0.0f);
		}

		GameObject *M01PlayerIsCrossingBridgeZoneObj = Commands->Find_Object(100922);
		if (M01PlayerIsCrossingBridgeZoneObj)
		{
			Commands->Destroy_Object(M01PlayerIsCrossingBridgeZoneObj);
		}

		GameObject *M01PlayerIsCrossingBridgeViaCaveZoneObj = Commands->Find_Object(100934);
		if (M01PlayerIsCrossingBridgeViaCaveZoneObj)
		{
			Commands->Destroy_Object(M01PlayerIsCrossingBridgeViaCaveZoneObj);
		}

		GameObject *M01PlayerIsCrossingBridgeViaChurchZoneObj = Commands->Find_Object(100972);
		if (M01PlayerIsCrossingBridgeViaChurchZoneObj)
		{
			Commands->Destroy_Object(M01PlayerIsCrossingBridgeViaChurchZoneObj);
		}
	}
}

ScriptRegistrant<M01_Player_Is_Crossing_Bridge_Via_Church_Zone> M01_Player_Is_Crossing_Bridge_Via_Church_ZoneRegistrant("M01_Player_Is_Crossing_Bridge_Via_Church_Zone", "");