
#pragma once
#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "WeaponClass.h"
#include "WeaponBagClass.h"
#include "SoldierGameObj.h"
#include "PhysicsSceneClass.h"
#include "MoveablePhysClass.h"
#define PI 3.14159265f
#define PI180 PI/180

/*!
* \brief Displays a message if the script named isn't in the scripts build on the server.
* \ScriptName - The name of the script you want to check for
* \CppName - Name of the cpp file that it is located in, you don't need the extension
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Check_If_Script_Is_In_Library : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Sends a custom when customs are sent in a certain order (kind of like a combo lock)
* \Success_Custom - Message to send when combo is entered correctly
* \Correct_Step_Custom - Message to send when one part of the combo is correct
* \Partial_Failure_Custom - Message to send when a digit is entered wrong
* \Failure_Custom - Message to send when the who combo is considered failed
* \Send_To_ID - ID of the object to send customs to
* \Custom_0 - First message needed in the combo
* \Custom_1 - Second message needed in the combo
* \Custom_2 - Third message needed in the combo
* \Custom_3 - Fourth message needed in the combo
* \Custom_4 - Fifth message needed in the combo
* \Custom_5 - Sixth message needed in the combo
* \Custom_6 - Seventh message needed in the combo
* \Custom_7 - Eighth message needed in the combo
* \Custom_8 - Ninth message needed in the combo
* \Custom_9 - Tenth message needed in the combo
* \Disable_On_Success - Only works once
* \Disable_On_Failure - Quits working after failure
* \Starts_Enabled - Is it enabled on script attach
* \Enable_Custom - Custom to enable this script
* \Correct_Step_Saftey - Does it send a correct step custom on the Custom0 step
* \Failure_Saftey - Does it count a wrong Custom0 as a failure
* \Max_Failures - How many times you can fail before its a complete failure
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Send_Custom_When_Custom_Sequence_Matched : public ScriptImpClass {
	int depth;
	int failCount;
	bool enabled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Send_Custom(GameObject *obj,int custom,int param);
};

/*!
* \brief Changes a objects w3d model on a timer
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Change_Model_On_Timer : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Turns an object into an object that emulates the damange system of DamageableStaticPhysics tiles
* \Animation - Animation to use for the damage animations
* \Killed_Explosion - Explosion to create when health hits 0
* \Live_Loop_Start - Animation frame to start on when alive
* \Live_Loop_End - Animation frame to stop on when alive
* \Live_Twitch_Start - Animation frame to start on when twitching while alive
* \Live_Twitch_End - Animation frame to stop on when twitching while alive
* \Death_Transition_Start - Animation frame to start on when transitioning to death
* \Death_Transition_End - Animation frame to stop on when transitioning to death
* \Death_Loop_Start - Animation frame to start on when dead
* \Death_Loop_End - Animation frame to stop on when dead
* \Death_Twitch_Start - Animation frame to start on when twitching while dead
* \Death_Twitch_End - Animation frame to stop on when twitching while dead
* \Live_Transition_Start - Animation frame to start on when transitioning back to life
* \Live_Transition_End - Animation frame to stop on when transitioning back to life
* \Repaired_Explosion - Explosion to create when fully repaired
* \Play_Twitches_To_Completion - Can the twitch animations be interrupted
* \Send_To_ID - ID to send custom messages to
* \Death_Custom - Message to send when object dies
* \Repaired_Custom - Message to send when object repaired
* \Damage_Points - Points you get for damaging the object
* \Death_Points - Points you get for killing the object
* \Heal_Points - Points you get for healing the object
* \Repair_Points - Points you get for completely repairing the object
* \Neutral_When_Dead - Does the object's team switch to neutral when dead
* \Disable_Collision_When_Dead - Disable the object's collision while dead
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Emulate_DamageableStaticPhys : public ScriptImpClass {
	int team;
	bool alive;
	bool playingTransition;
	bool playingTwitch;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Animation_Complete(GameObject *obj,const char *anim);
	void Play_Animation(GameObject *obj,bool loop,float start,float end);
};
/*!
* \brief Displays a hud message to all players on custom
* \Custom - Custom message to trigger on
* \StringId - ID of the string in strings.tbl to display
* \ColorRGB - Color of the hud message, 0.0-1.0 RGB
* \Repeatable - Can this show up multiple times
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Display_HUD_Info_Text_All_Players_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};
/*!
* \brief Displays a hud message to a player on custom
* \Custom - Custom message to trigger on
* \StringId - ID of the string in strings.tbl to display
* \ColorRGB - Color of the hud message, 0.0-1.0 RGB
* \Repeatable - Can this show up multiple times
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Display_HUD_Info_Text_To_Sender_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};
/*!
* \brief Soldier well enter the nearest vehicle on custom
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Soldier_Transition_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};
/*!
* \brief On Poke sends custom to self
* \Custom - Message to send when poked
* \Param - Param to send when poked
* \Delay - How long to delay the send
* \LockoutTime - Amount of time that it has to wait before it can send again
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Poke_Send_Self_Custom : public ScriptImpClass {
	bool poked;
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
	void Timer_Expired(GameObject *obj,int number);
};
/*!
* \brief Basic turret attach script, turrets match team of vehicle attached to, turrets are destroyed by destroy event
* \Turret_Preset - Preset of the turret
* \Bone_Name - Bone to hook the turret to
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Turret_Spawn : public ScriptImpClass
{
  int turretId;
  bool hasDriver;
  void Created(GameObject *obj);
  void Custom(GameObject *obj,int message,int param,GameObject *sender);
  void Killed(GameObject *obj,GameObject *killer);
  void Destroyed(GameObject *obj);
};

/*!
* \brief Basic Engineer AI that will try to repair all OBJECTs in the patrol range, if an enemy gets close they will also shoot at them with their weapon preset. 
* \ AI is granted the weapons if it does not have them.
* \RepaiarGun_Preset - Weapon preset to use to repair objects
* \Weapon_Preset - Weapon preset to use to attack enemies, if null the AI cannot attack enemies
* \PatrolRange - Area they can wander around in and search for things to repair
* \BaseCenterPoint - Center of the area they can wander in, if 0 0 0 it defaults to their create location
* \MinHP[Soldiers|Vehicles|Turrets] - HP must be below this value to be a repaireable target 0 disables the category
* \MinHP[EngineerTarget|C4|Beacon] - EngineerTarget is designated by any object with the script JMG_Utility_AI_Engineer_Repair_Target attached HP percent must be 
* \ below this value. Beacon C4 means that the engineer will repair enemy beacons and C4 that currently have over this much of their max health, 1 disables it 
* \ (the reason for this is is that you can make more engineers attempt to repair C4 or beacons with higher health this way)
* \Priority[RepairTargets|C4|Beacons] - This attempts to make some targets more valuable than others by tricking the AI into thinking they are further or nearer 
* \ to it, this would allow you to make the AI always think beacons were super close thus it would always choose those to remove first
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Engineer : public ScriptImpClass {
	bool canFight;
	Vector3 centerLocation;
	float maxRange;
	int aiIgnorePlayers[128];
	float repairGunRange;
	float repairGunEffectiveRange;
	float weaponRange;
	float weaponEffectiveRange;
	bool repairGun;
	int actionUpdate;
	int targetId;
	float targetDistance;
	int targetUpdate;
	int samePosition;
	Vector3 lastPos;
	Vector3 moveLocation;
	int repairTargetId;
	GameObject *lastTarget;
	GameObject *lastSecondaryTarget;
	bool lastRepairTarget;
	bool lastUseRepairGun;
	int wanderPointGroup;
	int randomResetAmount;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void AttackTarget(GameObject *obj,GameObject *target,GameObject *secondaryTarget,bool repairTarget,bool useRepairGun);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	bool inRange(GameObject *obj);
	inline bool Valid_Repair_Target(GameObject *obj,GameObject *target,int playerType);
	inline bool Valid_Repair_Target_C4(GameObject *obj,GameObject *target,int playerType);
	bool Get_Random_Wander_Point(Vector3 *position);
};
/*!
* \brief Will change the visibility of an object on custom for the specified player id
* \Custom - Custom to trigger this script on
* \PlayerID - What player to make it visible or invisible for
* \Visible - Should the object be visible or not
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Set_Object_Visibility_For_Player_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};
class JmgUtility
{
public:
	static bool IsInPathfindZone(Vector3 spot)
	{
		if (!Get_Random_Pathfind_Spot(spot,5.0f,&spot))
			return false;
		return true;
	}
	static void RotateZoneBox(float Angle,Matrix3 Basis)
	{
		Angle = Angle*3.14159265f/180.0f;
		float Sin = sin(Angle);
		float Cos = cos(Angle);
		Basis[0][0] = Cos;Basis[0][1] = Sin;Basis[0][2] = 0.0f;
		Basis[1][0] = Sin;Basis[1][1] = Cos;Basis[1][2] = 0.0f;
		Basis[2][0] = 0.0f;Basis[2][1] = 0.0f;Basis[2][2] = 1.0f;
	}
	static float rotationClamp(float startRotation,float addRotation)
	{
		startRotation += addRotation;
		while (startRotation > 180)
			startRotation -= 360;
		while (startRotation < -180)
			startRotation += 360;
		return startRotation;
	}
	static char *JMG_Get_Compass_Directions(float compassDegree)
	{
		static char name[4];
		sprintf(name,"%s",compassDegree <= -168.75 ? "W":
			compassDegree <= -146.25 ? "WSW":
			compassDegree <= -123.75 ? "SW":
			compassDegree <= -101.25 ? "SSW":
			compassDegree <= -78.75 ? "S":
			compassDegree <= -56.25 ? "SSE":
			compassDegree <= -33.75 ? "SE":
			compassDegree <= -11.25 ? "ESE":
			compassDegree <= 11.25 ? "E":
			compassDegree <= 33.75 ? "ENE":
			compassDegree <= 56.25 ? "NE":
			compassDegree <= 78.75 ? "NNE":
			compassDegree <= 101.25 ? "N":
			compassDegree <= 123.75 ? "NNW":
			compassDegree <= 146.25 ? "NW":
			compassDegree <= 168.75 ? "WNW":
			compassDegree <= 180 ? "W" : "ERROR");
		return name;
	}
	static char *formatDigitGrouping(double money)
	{
		static char groupedMoney[43];
		if (abs(money) > 100000000000000000000000000.0)
		{
			sprintf(groupedMoney,"%.2lf",money);
			return groupedMoney;
		}
		char sMoney[40],tMoney[53],fMoney[53];
		sprintf(sMoney,"%.0lf",abs(money));
		int length = strlen(sMoney),count = -1,pos;
		pos = length+1+(length-1)/3;
		tMoney[pos] = '\0';
		for (int x = length;x >= 0;x--)
		{
			pos--;
			tMoney[pos] = sMoney[x];
			count++;
			if (count == 3 && pos > 0)
			{
				count = 0;
				pos--;
				tMoney[pos] = ',';
			}
		}
		if (money < 0)
			sprintf(fMoney,"-%s",tMoney);
		else
			sprintf(fMoney,"%s",tMoney);

		sprintf(sMoney,"%.2lf",abs(money));
		int nLength = strlen(sMoney);
		pos = strlen(fMoney);
		for (int x = length;x < nLength;x++)
		{
			fMoney[pos] = sMoney[x];
			pos++;
		}
		fMoney[pos] = '\0';
		sprintf(groupedMoney,"%s",fMoney);
		return groupedMoney;
	}
	static char *formatDigitGrouping(unsigned long value)
	{
		static char groupedMoney[43];
		if (value > 10000000000000000000)
		{
			sprintf(groupedMoney,"%lu",value);
			return groupedMoney;
		}
		char sValue[40],tValue[53],fValue[53];
		sprintf(sValue,"%lu",value);
		int length = strlen(sValue),count = -1,pos;
		pos = length+1+(length-1)/3;
		tValue[pos] = '\0';
		for (int x = length;x >= 0;x--)
		{
			pos--;
			tValue[pos] = sValue[x];
			count++;
			if (count == 3 && pos > 0)
			{
				count = 0;
				pos--;
				tValue[pos] = ',';
			}
		}
		sprintf(fValue,"%s",tValue);
	
		sprintf(groupedMoney,"%s",fValue);
		return groupedMoney;
	}
	static char JMG_Get_Sex(GameObject *obj)
	{
		int TestID = Commands->Get_ID(obj);
		if (!obj || !TestID)
			return '\0';
		if (!Get_Model(obj) || !_stricmp(Get_Model(obj),"null"))
			return '\0';
		if (!Get_Sex(obj))
			return '\0';
		return Get_Sex(obj);
	}
	static const char *JMG_Get_Units_Sex(GameObject *Unit)
	{
		if (JMG_Get_Sex(Unit) == 'A')
			return "himself";
		else if (JMG_Get_Sex(Unit) == 'B')
			return "herself";
		else
			return "itself";
	}
	static const char *JMG_Get_Units_Sex2(GameObject *Unit)
	{
		if (JMG_Get_Sex(Unit) == 'A')
			return "his";
		else if (JMG_Get_Sex(Unit) == 'B')
			return "her";
		else
			return "its";
	}
	static const char *JMG_Get_Units_Sex3(GameObject *Unit)
	{
		if (JMG_Get_Sex(Unit) == 'A')
			return "he";
		else if (JMG_Get_Sex(Unit) == 'B')
			return "she";
		else
			return "it";
	}
	static char *Get_The_Units_Name(GameObject *Unit)
	{
		static char RetChar[512];
		if (Commands->Is_A_Star(Unit))
			sprintf(RetChar,"%s",Get_Player_Name(Unit));
		else
			sprintf(RetChar,"A %s",Get_Translated_Preset_Name(Unit));
		return RetChar;
	}
	static char *Get_The_Units_Name2(GameObject *Unit)
	{
		static char RetChar[512];
		if (Commands->Is_A_Star(Unit))
			sprintf(RetChar,"%s",Get_Player_Name(Unit));
		else
			sprintf(RetChar,"a %s",Get_Translated_Preset_Name(Unit));
		return RetChar;
	}
	static long JMG_Get_Player_ID(GameObject *obj)
	{
		if (!obj)
			return 0;
		SoldierGameObj *o = obj->As_SoldierGameObj();
		if (!o)
			return 0;
		if (!((cPlayer *)o->Get_Player_Data()))
			return 0;
		return ((cPlayer *)o->Get_Player_Data())->Get_Id();
	}
	static int JMG_Get_Current_Weapon_ID(GameObject *obj)
	{
		if (!obj)
			return -1;
		PhysicalGameObj *o2 = obj->As_PhysicalGameObj();
		if (!o2)
			return -1;
		ArmedGameObj *o3 = o2->As_ArmedGameObj();
		if (!o3)
			return -1;
		WeaponBagClass *w = o3->Get_Weapon_Bag();
		if ((w->Get_Index()) && (w->Get_Index() < w->Get_Count()))
			return w->Peek_Weapon(w->Get_Index())->Get_ID();
		return -1;
	}
	static float SimpleDistance(const Vector3 &Pos1,const Vector3 &Pos2)
	{
		float DiffX = Pos1.X-Pos2.X;
		float DiffY = Pos1.Y-Pos2.Y;
		float DiffZ = Pos1.Z-Pos2.Z;
		return (DiffX*DiffX+DiffY*DiffY+DiffZ*DiffZ);
	}
	static float SimpleFlatDistance(const Vector3 &Pos1,const Vector3 &Pos2)
	{
		float DiffX = Pos1.X-Pos2.X;
		float DiffY = Pos1.Y-Pos2.Y;
		return (DiffX*DiffX+DiffY*DiffY);
	}
	static float MathClamp(float Value,float Min,float Max)
	{
		if (Value > Max)
			return Max;
		if (Value < Min)
			return Min;
		return Value;
	}
	static double MathClampDouble(double Value,double Min,double Max)
	{
		if (Value > Max)
			return Max;
		if (Value < Min)
			return Min;
		return Value;
	}
	static int MathClampInt(int Value,int Min,int Max)
	{
		if (Value > Max)
			return Max;
		if (Value < Min)
			return Min;
		return Value;
	}
	static void DisplayChatMessage(GameObject *obj,int Red,int Green,int Blue,const char *Message)
	{
		if (!obj)
			return;
		int Strlen = strlen(Message);
		if (Strlen < 220)
		{
			Send_Message_Player(obj,Red,Green,Blue,Message);
			return;
		}
		int x;
		char msg[230];
		for (x = 0;x < 220 && x < Strlen;x++)
			msg[x] = Message[x];
		msg[x] = '\0';
		Send_Message_Player(obj,255,0,200,msg);
	}
	static void MessageAllPlayers(int Red,int Green,int Blue,const char *MSG)
	{
		for (int x = 0;x < 128;x++)
		{
			GameObject *Player = Get_GameObj(x);
			if (!Player)
				continue;
			DisplayChatMessage(Player,Red,Green,Blue,MSG);
		}
	}
	static void MessageTeamPlayers(int Red,int Green,int Blue,int Team,const char *MSG)
	{
		for (int x = 0;x < 128;x++)
		{
			GameObject *Player = Get_GameObj(x);
			if (!Player || Get_Team(x) != Team)
				continue;
			DisplayChatMessage(Player,Red,Green,Blue,MSG);
		}
	}
	static void MessageTeamPlayersType(int Red,int Green,int Blue,int Team,const char *MSG)
	{
		for (int x = 0;x < 128;x++)
		{
			GameObject *Player = Get_GameObj(x);
			if (!Player || Commands->Get_Player_Type(Player) != Team)
				continue;
			DisplayChatMessage(Player,Red,Green,Blue,MSG);
		}
	}
	static GameObject *FindNearestPlayer(const Vector3 &pos)
	{
		GameObject *nearestPlayer = NULL;
		float nearest = -1.0f;
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player || Get_Player_Type(player) == -4)
				continue;
			float tempDist = SimpleDistance(pos,Commands->Get_Position(player));
			if (nearest < 0 || tempDist < nearest)
			{
				nearest = tempDist;
				nearestPlayer = player;
			}
		}
		return nearestPlayer;
	}
	static char *Rp2Encrypt(const char *String,int EncryptionFactor,int Start)
	{
		static char FinalString[2000];
		sprintf(FinalString," ");
		int loops = strlen(String),x = 0;
		int CurrentAdd = Start;
		while (x < loops)
		{
			if (String[x] > 31 && String[x] < 127)
			{
				int Temp = (String[x]+CurrentAdd);
				while (Temp > 126)
					Temp -= 95;
				FinalString[x] = (char)Temp;
			}
			else
				FinalString[x] = String[x];
			x++;
			CurrentAdd++;
			if (CurrentAdd > EncryptionFactor)
				CurrentAdd = 1;
			FinalString[x] = '\0';
		}
		return FinalString;
	}
	static char *Rp2Encrypt2(const char *String,int EncryptionFactor,int Start)
	{
		static char FinalString[2000];
		sprintf(FinalString," ");
		int loops = strlen(String),x = 0;
		int CurrentAdd = Start;
		while (x < loops)
		{
			if (String[x] > 31 && String[x] < 127)
			{
				int Temp = (String[x]+CurrentAdd);
				while (Temp > 126)
					Temp -= 95;
				FinalString[x] = (char)Temp;
			}
			else
				FinalString[x] = String[x];
			x++;
			CurrentAdd++;
			if (CurrentAdd > EncryptionFactor)
				CurrentAdd = 1;
			FinalString[x] = '\0';
		}
		return FinalString;
	}
	static char *Rp2Decrypt(const char *String,int EncryptionFactor,int Start)
	{
		static char FinalString[2000];
		int loops = strlen(String),x = 0;
		int CurrentAdd = Start;
		while (x < loops)
		{
			if (String[x] > 31 && String[x] < 127)
			{
				int Temp = String[x]-CurrentAdd;
				while (!(Temp > 31))
					Temp += 95;
				FinalString[x] = (char)Temp;
			}
			else
				if (String[x] == '\n')
					FinalString[x] = '\0';
				else
					FinalString[x] = String[x];
			x++;
			CurrentAdd++;
			if (CurrentAdd > EncryptionFactor)
				CurrentAdd = 1;
			FinalString[x] = '\0';
		}
		return FinalString;
	}
	static bool IsTruePhysicalObject(GameObject *obj)
	{
		if (obj->As_ScriptableGameObj() && obj->As_PhysicalGameObj())
		{
			if (obj->As_SoldierGameObj())
				return true;
			if (obj->As_VehicleGameObj())
			{
				//TODO: asdfasdf
				/*if (Is_DecorationPhys(obj))
					return false;
				if (Is_DynamicAnimPhys(obj))
					return false;
				if (Is_TimedDecorationPhys(obj))
					return false;*/
				return true;
			}
		}
		return false;
	}
	static float MathClampDegrees(float Value)
	{
		while (Value < -180.0f)
			return Value+360.0f;
		while (Value > 180.0f)
			return Value-360.0f;
		return Value;
	}
	static bool hasStatedDeathMessage[128];
	static void setStatedDeathMessage(int playerId,bool value)
	{
		hasStatedDeathMessage[playerId] = value;
	}
	static void JMG_Player_Death_Message(GameObject *obj,GameObject *killer,const char *overrideMessage)
	{
		int playerId = JMG_Get_Player_ID(obj);
		if (hasStatedDeathMessage[playerId])
			return;
		char deathMsg[500];
		if (overrideMessage)
			sprintf(deathMsg,"%s",overrideMessage);
		else if (!killer)
			sprintf(deathMsg,"%s died",JmgUtility::Get_The_Units_Name(obj));
		else if (killer == obj)
			sprintf(deathMsg,"%s killed %s",JmgUtility::Get_The_Units_Name(obj),JmgUtility::JMG_Get_Units_Sex(obj));
		else
			sprintf(deathMsg,"%s killed %s",JmgUtility::Get_The_Units_Name(killer),JmgUtility::Get_The_Units_Name2(obj));
		if (Commands->Is_A_Star(obj))
			MessageAllPlayers(255,255,0,deathMsg);
		hasStatedDeathMessage[playerId] = true;
	}
	static void SetHUDHelpText(unsigned long stringId,Vector3 color)
	{
		for (int x = 0;x < 128;x++)
		{
			GameObject *Player = Get_GameObj(x);
			if (!Player)
				continue;
			Set_HUD_Help_Text_Player(Player,stringId,color);
		}
	}
	static void SetHUDHelpText(unsigned long stringId,const char *message,Vector3 color)
	{
		for (int x = 0;x < 128;x++)
		{
			GameObject *Player = Get_GameObj(x);
			if (!Player)
				continue;
			Set_HUD_Help_Text_Player_Text(Player,stringId,message,color);
		}
	}
	static bool DegreeDiff(float Deg1,float Deg2,float MaxDiff)
	{
		if (abs(Deg1-Deg2) <= MaxDiff)
			return true;
		if (360-abs(Deg1-Deg2) <= MaxDiff)
			return true;
		return false;
	}
};

class NewObjectiveSystem
{
	bool objectiveStringIdsLoaded;
	const char *objectiveNewString;
	const char *objectiveCancelledString;
	const char *objectiveStatusChangedString;
	const char *objectivePrioritieStrings[12];
	const char *objectiveStatusStrings[4];
	const char *objectiveListString;
	const char *objectiveUpdateObjectiveString;
	const char *objectiveCancelledStringNumbered;
	const char *objectiveStatusChangedStringNumbered;
	const char *objectiveUpdateObjectiveStringNumbered;
	char primaryObjectiveModel[32];
	char secondaryObjectiveModel[32];
	char tertiaryObjectiveModel[32];
	bool showRadarStars;
public:
	enum Priority{Undefined=-1,Unknown,Primary,Secondary,Tertiary,Quaternary,Quinary,Senary,Septenary,Octonary,Nonary,Denary,Bonus};
	enum Status{Removed=-2,NotDefined=-1,Pending,Accomplished,Failed,Hidden};
private:
	struct ObjectiveNode
	{
		int id;
		Priority priority;
		Status status;
		unsigned long nameId;
		char *soundFilename;
		unsigned long descriptionId;
		int radarMarkerId;
		bool active;
		int objectiveNumber;
		ObjectiveNode *next;
		ObjectiveNode(int id, Priority priority, Status status, unsigned long nameId, char *soundFilename, unsigned long descriptionId,int radarMarkerId,int objectiveNumber)
		{
			this->id = id;
			this->priority = priority;
			this->status = status;
			this->nameId = nameId;
			this->soundFilename = soundFilename;
			this->descriptionId = descriptionId;
			this->radarMarkerId = radarMarkerId;
			this->active = true;
			this->objectiveNumber = objectiveNumber;
			next = NULL;
		}
	};
	ObjectiveNode *objectiveNodeList;
	int objectiveCounts;
	int team;
	void initilizeStringIds()
	{
		objectiveNewString = Get_Translated_String(Get_String_ID_By_Desc("IDS_OBJ2_NEW_OBJ"));
		objectiveCancelledString = Get_Translated_String(Get_String_ID_By_Desc("IDS_OBJ2_CANCELLED"));
		objectiveStatusChangedString = Get_Translated_String(Get_String_ID_By_Desc("IDS_OBJ2_STATUS_CHANGED"));
		objectiveListString = Get_Translated_String(Get_String_ID_By_Desc("IDS_OBJ2_LIST"));
		objectiveUpdateObjectiveString = Get_Translated_String(Get_String_ID_By_Desc("IDS_OBJ2_UPDATED"));
		char descriptionString[512];
		for (int x = 0;x < 12;x++)
		{
			sprintf(descriptionString,"IDS_OBJ2_PRIORITY_%0d",x);
			objectivePrioritieStrings[x] = Get_Translated_String(Get_String_ID_By_Desc(descriptionString));
		}
		for (int x = 0;x < 4;x++)
		{
			sprintf(descriptionString,"IDS_OBJ2_STATE_%0d",x);
			objectiveStatusStrings[x] = Get_Translated_String(Get_String_ID_By_Desc(descriptionString));
		}
		objectiveCancelledStringNumbered = Get_Translated_String(Get_String_ID_By_Desc("IDS_OBJ2_CANCELLED_NUMBERED"));
		objectiveStatusChangedStringNumbered = Get_Translated_String(Get_String_ID_By_Desc("IDS_OBJ2_STATUS_CHANGED_NUMBERED"));
		objectiveUpdateObjectiveStringNumbered = Get_Translated_String(Get_String_ID_By_Desc("IDS_OBJ2_UPDATED_NUMBERED"));
		objectiveStringIdsLoaded = true;
	}
	void selectMessageAndColor(const char *format,Priority priority)
	{
		switch (priority)
		{
		case Primary: JmgUtility::MessageTeamPlayersType(50,255,50,team,format); break;
		case Secondary: JmgUtility::MessageTeamPlayersType(50,150,250,team,format); break;
		case Tertiary:case Unknown: JmgUtility::MessageTeamPlayersType(150,50,150,team,format); break;
		default: JmgUtility::MessageTeamPlayersType(125,150,150,team,format); break;
		}
	}
	void messagePlayerAndColor(GameObject *player,const char *format,Priority priority)
	{
		switch (priority)
		{
		case Primary: JmgUtility::DisplayChatMessage(player,50,255,50,format); break;
		case Secondary: JmgUtility::DisplayChatMessage(player,50,150,250,format); break;
		case Tertiary:case Unknown: JmgUtility::DisplayChatMessage(player,150,50,150,format); break;
		default: JmgUtility::DisplayChatMessage(player,125,150,150,format); break;
		}
	}
	char *formatObjectiveString(const char *format,...)
	{
		static char displayMsg[256];
		va_list args;
		va_start(args,format);
		vsprintf(displayMsg,format,args);
		va_end(args);
		return displayMsg;
	}
	bool addObjective(int id, Priority priority, Status status, unsigned long nameId, char *soundFilename, unsigned long descriptionId,int radarMarkerId,int objectiveNumber)
	{
		if (priority == Undefined)
			return false;
		if (!objectiveStringIdsLoaded)
			initilizeStringIds();
		ObjectiveNode *current = objectiveNodeList;
		if (!objectiveNodeList)
			objectiveNodeList = new ObjectiveNode(id,priority,status,nameId,soundFilename,descriptionId,radarMarkerId,objectiveNumber);
		while (current)
		{
			if (current->id == id)
				if (current->active)
				{
					Destroy_Radar_Marker(Commands->Find_Object(radarMarkerId));
					return false;
				}
				else
				{
					Destroy_Radar_Marker(Commands->Find_Object(current->radarMarkerId));
					current->id = id;
					current->priority = priority;
					current->status = status;
					current->nameId = nameId;
					current->soundFilename = soundFilename;
					current->descriptionId = descriptionId;
					current->radarMarkerId = radarMarkerId;
					current->objectiveNumber = objectiveNumber;
					current->active = true;
					break;
				}
			if (!current->next)
			{
				current->next = new ObjectiveNode(id,priority,status,nameId,soundFilename,descriptionId,radarMarkerId,objectiveNumber);
				break;
			}
			current = current->next;
		}
		objectiveCounts++;
		if (status != Hidden && descriptionId)
		{
			selectMessageAndColor(formatObjectiveString(objectiveNewString,objectivePrioritieStrings[priority]),priority);
			if (objectiveNumber)
				selectMessageAndColor(formatObjectiveString(Get_Translated_String(descriptionId),objectiveNumber),priority);
			else
				selectMessageAndColor(Get_Translated_String(descriptionId),priority);
		}
		return true;
	}
	void Destroy_Radar_Marker(GameObject *marker)
	{
		if (!marker)
			return;
		Commands->Destroy_Object(marker);
	}
	GameObject *Create_Radar_Marker(Vector3 pos, Priority priority,const char *modelOverride)
	{
		GameObject *radarMarker = Commands->Create_Object("Daves Arrow",pos);
		Commands->Set_Player_Type(radarMarker,team);
		Commands->Set_Is_Visible(radarMarker,false);
		if (modelOverride)
			Commands->Set_Model(radarMarker,modelOverride);
		else
			switch (priority)
			{
				case Priority::Primary:Commands->Set_Model(radarMarker,primaryObjectiveModel);break;
				case Priority::Secondary:Commands->Set_Model(radarMarker,secondaryObjectiveModel);break;
				case Priority::Tertiary:Commands->Set_Model(radarMarker,tertiaryObjectiveModel);break;
				default:Commands->Set_Model(radarMarker,"null");break;
			}
		if (showRadarStars)
		{
			Commands->Set_Obj_Radar_Blip_Shape(radarMarker,RADAR_BLIP_SHAPE_OBJECTIVE);
			Commands->Set_Obj_Radar_Blip_Color(radarMarker,priority == Primary ? RADAR_BLIP_COLOR_PRIMARY_OBJECTIVE : priority == Secondary ? RADAR_BLIP_COLOR_SECONDARY_OBJECTIVE : RADAR_BLIP_COLOR_TERTIARY_OBJECTIVE);
		}
		return radarMarker;
	}
public:
	NewObjectiveSystem(int team,bool showRadarStars = true,const char *primaryObjectiveModel = "null",const char *secondaryObjectiveModel = "null",const char *tertiaryObjectiveModel = "null")
	{
		this->team = team;
		objectiveStringIdsLoaded = false;
		objectiveCounts = 0;
		sprintf(this->primaryObjectiveModel,"%s",primaryObjectiveModel);
		sprintf(this->secondaryObjectiveModel,"%s",secondaryObjectiveModel);
		sprintf(this->tertiaryObjectiveModel,"%s",tertiaryObjectiveModel);
		this->showRadarStars = showRadarStars;
		objectiveNodeList = NULL;
	}
	~NewObjectiveSystem()
	{
		objectiveStringIdsLoaded = false;
		objectiveCounts = 0;
		ObjectiveNode *temp = objectiveNodeList,*die;
		while (temp)
		{
			die = temp;
			temp = temp->next;
			delete die;
		}
		objectiveNodeList = NULL;
	}
	bool Add_Objective(int objectiveId, Priority priority, Status status, unsigned long nameId, char *soundFilename, unsigned long descriptionId,GameObject *blipUnit,const char *modelOverride = NULL,int objectiveNumber = 0)
	{
		if (!blipUnit)
			return false;
		GameObject *radarMarker = Create_Radar_Marker(Vector3(),priority,modelOverride);
		if (!radarMarker)
			return false;
		Commands->Attach_To_Object_Bone(radarMarker,blipUnit,"origin");
		return addObjective(objectiveId,priority,status,nameId,soundFilename,descriptionId,Commands->Get_ID(radarMarker),objectiveNumber);
	}
	bool Add_Objective(int objectiveId, Priority priority, Status status, unsigned long nameId, char *soundFilename, unsigned long descriptionId,Vector3 blipPosition,const char *modelOverride = NULL,int objectiveNumber = 0)
	{
		GameObject *radarMarker = Create_Radar_Marker(blipPosition,priority,modelOverride);
		if (!radarMarker)
			return false;
		return addObjective(objectiveId,priority,status,nameId,soundFilename,descriptionId,Commands->Get_ID(radarMarker),objectiveNumber);
	}
	bool Get_Radar_Blip_Position(int objectiveId,Vector3 *position)
	{
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->id == objectiveId)
			{
				GameObject *objectiveMarker = Commands->Find_Object(current->radarMarkerId);
				if (objectiveMarker)
				{
					*position = Commands->Get_Position(objectiveMarker);
					return true;
				}
				return false;
			}
			current = current->next;
		}
		return false;
	}
	void Set_Radar_Blip(int objectiveId,GameObject *blipUnit,const char *modelOverride = NULL)
	{
		if (!blipUnit)
			return;
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->id == objectiveId)
			{
				Destroy_Radar_Marker(Commands->Find_Object(current->radarMarkerId));
				GameObject *radarMarker = Create_Radar_Marker(Vector3(),current->priority,modelOverride);
				if (!radarMarker)
					return;
				Commands->Attach_To_Object_Bone(radarMarker,blipUnit,"origin");
				current->radarMarkerId = Commands->Get_ID(radarMarker);
				return;
			}
			current = current->next;
		}
	}
	void Set_Radar_Blip(int objectiveId,Vector3 blipPosition,const char *modelOverride = NULL)
	{
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->id == objectiveId)
			{
				Destroy_Radar_Marker(Commands->Find_Object(current->radarMarkerId));
				GameObject *radarMarker = Create_Radar_Marker(blipPosition,current->priority,modelOverride);
				if (!radarMarker)
					return;
				current->radarMarkerId = Commands->Get_ID(radarMarker);
				return;
			}
			current = current->next;
		}
	}
	bool Add_Objective(int objectiveId, Priority priority, Status status, unsigned long nameId, char *soundFilename, unsigned long descriptionId,int objectiveNumber = 0)
	{
		return addObjective(objectiveId,priority,status,nameId,soundFilename,descriptionId,0,objectiveNumber);
	}
	long Get_Mission_Text_Id(int objectiveId)
	{
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->id == objectiveId)
				return current->nameId;
			current = current->next;
		}
		return 0;
	}
	bool Remove_Objective(int objectiveId)
	{
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->id == objectiveId)
				if (current->active)
				{
					objectiveCounts--;
					if (current->status == Pending && current->nameId)
						if (current->objectiveNumber)
							selectMessageAndColor(formatObjectiveString(objectiveCancelledStringNumbered,objectivePrioritieStrings[current->priority],current->objectiveNumber),current->priority);
						else
							selectMessageAndColor(formatObjectiveString(objectiveCancelledString,objectivePrioritieStrings[current->priority]),current->priority);
					Destroy_Radar_Marker(Commands->Find_Object(current->radarMarkerId));
					current->active = false;
					return true;
				}
				else
					return false;
			current = current->next;
		}
		return true;
	}
	bool Set_Objective_Status(int objectiveId,Status status)
	{
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->id == objectiveId)
				if (current->status != status)
				{
					if (status != Hidden && current->status != Hidden && current->nameId)
						if (current->objectiveNumber)
							selectMessageAndColor(formatObjectiveString(objectiveStatusChangedStringNumbered,objectivePrioritieStrings[current->priority],current->objectiveNumber,objectiveStatusStrings[status]),current->priority);
						else
							selectMessageAndColor(formatObjectiveString(objectiveStatusChangedString,objectivePrioritieStrings[current->priority],objectiveStatusStrings[status]),current->priority);
					GameObject *marker = Commands->Find_Object(current->radarMarkerId);
					if (marker)
					{
						if (status == Pending)
						{
							Commands->Set_Is_Rendered(marker,true);
							if (showRadarStars)
								Commands->Set_Obj_Radar_Blip_Shape(marker,RADAR_BLIP_SHAPE_OBJECTIVE);
						}
						else
						{
							Commands->Set_Is_Rendered(marker,false);
							if (showRadarStars)
								Commands->Set_Obj_Radar_Blip_Shape(marker,RADAR_BLIP_SHAPE_NONE);
						}
						Commands->Set_Player_Type(marker,status == Pending);
					}
					current->status = status;
					return true;
				}
				else
					return false;
			current = current->next;
		}
		return true;
	}
	bool Set_Objective_Mission(int objectiveId,unsigned int nameStringId,unsigned int descriptionStringId)
	{
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->id == objectiveId)
				if (current->nameId != nameStringId || current->descriptionId != descriptionStringId)
				{
					current->nameId = nameStringId;
					current->descriptionId = descriptionStringId;
					if (current->status != Hidden && descriptionStringId)
					{
						if (current->objectiveNumber)
						{
							selectMessageAndColor(formatObjectiveString(objectiveUpdateObjectiveStringNumbered,objectivePrioritieStrings[current->priority],current->objectiveNumber),current->priority);
							selectMessageAndColor(formatObjectiveString(Get_Translated_String(descriptionStringId),current->objectiveNumber),current->priority);
						}
						else
						{
							selectMessageAndColor(formatObjectiveString(objectiveUpdateObjectiveString,objectivePrioritieStrings[current->priority]),current->priority);
							selectMessageAndColor(Get_Translated_String(descriptionStringId),current->priority);
						}
					}
					return true;
				}
				else
					return false;
			current = current->next;
		}
		return true;
	}
	Status Get_Objective_Status(int objectiveId)
	{
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->id == objectiveId)
				if (current->active)
					return current->status;
				else
					return Removed;
			current = current->next;
		}
		return NotDefined;
	}
	Priority Get_Objective_Priority(int objectiveId)
	{
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->id == objectiveId)
				return current->priority;
			current = current->next;
		}
		return Undefined;
	}
	int Get_Objective_Status_Count(Status status,Priority requiredPriority = Undefined)
	{
		int count = 0;
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->active && current->status == status && (requiredPriority == Undefined || current->priority == requiredPriority))
				count++;
			current = current->next;
		}
		return count;
	}
	int Get_Objective_Priority_Count(Priority requiredPriority,Status status = NotDefined)
	{
		int count = 0;
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->active && current->priority == requiredPriority && (status == NotDefined || current->status == status))
				count++;
			current = current->next;
		}
		return count;
	}
	void Dispaly_First_Pending_Primary_Objective_On_Hud(GameObject *obj)
	{
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->active && current->status == Pending && current->priority == Priority::Primary && current->nameId)
			{
				Set_HUD_Help_Text_Player(obj,current->nameId,Vector3(0,1,0));
				return;
			}
			current = current->next;
		}
	}
	void Display_Current_Objectives(GameObject *player,Priority priority)
	{
		messagePlayerAndColor(player,formatObjectiveString(objectiveListString,objectivePrioritieStrings[priority]),priority);
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->active && current->status == Pending && current->priority == priority && current->nameId)
			{
				char objectiveMsg[220];
				if (current->objectiveNumber)
					sprintf(objectiveMsg,"*%s",formatObjectiveString(Get_Translated_String(current->nameId),current->objectiveNumber));
				else
					sprintf(objectiveMsg,"*%s",Get_Translated_String(current->nameId));
				messagePlayerAndColor(player,objectiveMsg,priority);
			}
			current = current->next;
		}
	}
	void Display_All_Objectives(GameObject *player)
	{
		int counts[12];
		for (int x = 0;x < 12;x++)
			counts[x] = Get_Objective_Status_Count(Pending,(Priority)x);
		for (int x = 0;x < 12;x++)
			if (counts[x])
				Display_Current_Objectives(player,(Priority)x);
	}
	int Get_First_Pending_Objective_Of_Priority(Priority priority)
	{
		ObjectiveNode *current = objectiveNodeList;
		while (current)
		{
			if (current->active && current->status == Pending && current->priority == priority && current->nameId)
				return current->id;
			current = current->next;
		}
		return 0;
	}
	Vector3 Get_Hud_Help_Text_Color(Priority priority)
	{
		switch (priority)
		{
		case Priority::Primary:
			return Vector3(0.196f,0.882f,0.196f);
		case Priority::Secondary:
			return Vector3(0.196f,0.588f,0.98f);
		case Priority::Tertiary:
			return Vector3(0.588f,0.196f,0.588f);
		default:
			return Vector3(1.0f,1.0f,1.0f);
		}
	}
};

class ClientNetworkObjectPositionSync
{
public:
	struct SyncObjectNode
	{
		int id;
		float facing;
		Vector3 position;
		struct SyncObjectNode *next;
		SyncObjectNode(GameObject *obj)
		{
			this->id = Commands->Get_ID(obj);
			this->facing = Commands->Get_Facing(obj);
			this->position = Commands->Get_Position(obj);
			this->next = NULL;
		}

	};
private:
	SyncObjectNode *syncObjectNodeList;
	struct SyncControl
	{
		bool syncedPlayers[128];
		SyncObjectNode *lastSyncNode[128];
		SyncControl()
		{
			for (int x = 0;x < 128;x++)
			{
				syncedPlayers[x] = false;
				lastSyncNode[x] = NULL;
			}
		}
		void clientNoLongerSynced(int playerId)
		{
			syncedPlayers[playerId] = false;
			lastSyncNode[playerId] = NULL;
		}
	};
	SyncControl syncControl;
public:
	ClientNetworkObjectPositionSync()
	{
		syncObjectNodeList = NULL;
		syncControl = SyncControl();
	}
	SyncObjectNode *addNode(GameObject *obj)
	{
		int id = Commands->Get_ID(obj);
		SyncObjectNode *current = syncObjectNodeList;
		if (!syncObjectNodeList)
			return syncObjectNodeList = new SyncObjectNode(obj);
		while (current)
		{
			if (!current->id)
			{
				current->id = id;
				current->facing = Commands->Get_Facing(obj);
				current->position = Commands->Get_Position(obj);
				return current;
			}
			if (current->id == id)
				return current;
			if (!current->next)
			{
				current->next = new SyncObjectNode(obj);
				return current->next;
			}
			current = current->next;
		}
		return NULL;
	};
	void checkForPlayersThatLeftTheGame()
	{
		for (int x = 1;x < 128;x++)
		{
			GameObject *player = Get_GameObj(x);
			if (!player)
				syncControl.clientNoLongerSynced(x);
		}
	}
	void triggerSingleNetworkSync()
	{
		for (int x = 1;x < 128;x++)
		{
			if (syncControl.syncedPlayers[x])
				continue;
			GameObject *player = Get_GameObj(x);
			if (!player)
				continue;
			if (!syncControl.lastSyncNode[x])
				syncControl.lastSyncNode[x] = syncObjectNodeList;
			if (syncControl.lastSyncNode[x])
			{
				if (syncControl.lastSyncNode[x]->id)
				{
					GameObject *syncObject = Commands->Find_Object(syncControl.lastSyncNode[x]->id);
					if (syncObject)
						Force_Position_Update_Player(player,syncObject);
				}
				syncControl.lastSyncNode[x] = syncControl.lastSyncNode[x]->next;
			}
			if (!syncControl.lastSyncNode[x])
				syncControl.syncedPlayers[x] = true;
		}
	}
	void Empty_List()
	{
		SyncObjectNode *temp = syncObjectNodeList,*die;
		while (temp)
		{
			die = temp;
			temp = temp->next;
			delete die;
		}
		syncObjectNodeList = NULL;
		syncControl = SyncControl();
	}
};

/*!
* \brief An object that will have its position synced by JMG_Utility_Sync_System_Controller
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Sync_System_Object : public ScriptImpClass
{
	ClientNetworkObjectPositionSync::SyncObjectNode *syncNode;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *killer);
	void Destroyed(GameObject *obj);
	void Detach(GameObject *obj);
public:
	JMG_Utility_Sync_System_Object()
	{
		syncNode = NULL;
	}
};

/*!
* \brief Controls all objects that have the script JMG_Utility_Sync_System_Object
* \Sync_Rate - Speed at which to sync up all the objects in the list when a player joins
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Sync_System_Controller : public ScriptImpClass
{
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
public:
	JMG_Utility_Sync_System_Controller();
};

/*!
* \brief Syncs object positions between the client and the server
* \Sync_Rate - How often does this object send out an update across the network
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Sync_Object_Periodically : public ScriptImpClass
{
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Turns a placed object into a renstyle spawner that can be sent customs like a normal object, don't put this on an object that will fall through the level!
* \SpawnPreset - Preset to spawn
* \RespawnTime - Time it takes between spawns after last object was killed
* \RespawnTimeRandom - Random time to add or subtract from the spawn time
* \StartsEnabled - Can the object spawn on script create
* \EnableOrDisableCustom - Message used to enable the spawner, the parameter sent with it tell whether to enable or disable (1 for enable, 0 for disable, -1 disables and kills the spawned object, -2 disables and destroys the spawned object)
* \SpawnLimit - How many objects can this script spawn
* \DelayOnStartup - Day to spawn the first object when enabled
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Basic_Spawner : public ScriptImpClass
{
	int spawnLimit;
	int spawnedId;
	bool enabled;
	float respawnTime;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void CalculateRespawnTime();
public:
	static Vector3 preSpawnLocation;
};

class JMG_Utility_Basic_Spawner_Spawned_Object : public ScriptImpClass
{
	void Destroyed(GameObject *obj);
};

struct JMGVehicleAction
{
	int targetId;
	Vector3 position;
	int useAmmo;
	bool backward;
	bool following;
	float arriveDistance;
	JMGVehicleAction()
	{
		targetId = 0;
		useAmmo = 0;
		position = Vector3();
		backward = false;
		following = false;
		arriveDistance = 0.0f;
	}
	bool operator == (JMGVehicleAction jva)
	{
		return (targetId == jva.targetId && position == jva.position && useAmmo == jva.useAmmo);
	}
	bool operator != (JMGVehicleAction jva)
	{
		return (!(*this == jva));
	}
	JMGVehicleAction &operator = (JMGVehicleAction jva)
	{
		targetId = jva.targetId;
		useAmmo = jva.useAmmo;
		position = jva.position;
		backward = jva.backward;
		following = jva.following;
		arriveDistance = jva.arriveDistance;
		return *this;
	}
};


/*!
* \brief Vehicle AI that can be used for both aircraft and wheeled vehicles, the AI will attempt to backup when stuck and can choose different weapons depending on what foe it faces
* \MaxRange - How close will the AI drive to a target.
* \MinAttackRange - How close can a target be before it can no longer be fired upon, if the target is too close the vehicle will attempt to back up to a previous position or return to a wander point/home position if either are available.
* \FollowTarget - Tells the AI follow its target around while it tries to kill it.
* \ReturnHome - Tells the AI return to the spot it was created at once it kills a target.
* \ForceFire - Tells the AI to fire weapons even if the turret isn't aimed at the target, useful for vehicles like the MRLS.
* \VTOLHover - Used for Aircraft, tells the AI how high to fly above its target spots, if 0 it will attempt to smoosh them.
* \vsSoldier - Tells the AI what fire mode to use against infantry, -1 means it can't attack them, 2 means auto, 0 is secondary fire, 1 is primary fire.
* \vsVehicles - Tells the AI what fire mode to use against vehicles, -1 means it can't attack them, 2 means auto, 0 is secondary fire, 1 is primary fire.
* \vsAircraft - Tells the AI what fire mode to use against aircraft, -1 means it can't attack them, 2 means auto, 0 is secondary fire, 1 is primary fire.
* \WanderingAIGroupID - Tells the vehicle that it can used JMG_Wandering_AI_Controller and JMG_Wandering_AI_Wander_Point scripts to move around the map randomly.
* \MinRandWander - The smallest amount of time the AI will wait before attempting to wander.
* \MaxRandWander - The longest amount of time the AI will wait before attempting to wander.
* \SquishInfantry - Tells the unit to drive over the enemy if it is an infantry.
* \EnableTransitions - Whether or not players can get in and out of the vehicle.
* \UsePathfind - Tells the unit whether or not to use pathfind.
* \DisableEngineOnArrival - Tells the unit whether it should shutdown its engine when it finishes moving, useful for wheeled vehicles so they don't go rolling away.
* \WeaponError - This tells the max weapon error the vehicle can use, if weapon error is -1 it dynamically determines a weapon error to use, this calculation has to do with bullet speed and enemy distance.
* \MaxHuntRange - This variable makes it so the AI can't see anything that is further than x meters from the wander point (or home position) it is currently moving too, if 0 this setting is ignored.
* \NextWanderPointWhenLastReached - When 0 MinRandWander and MaxRandWander are used, when greater than 0 this is the distance the Vehicle must get to a wander point before choosing its next one
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Vehicle : public ScriptImpClass {
	struct JMGVehicleAmmo
	{
		bool allowError;
		float range;
		float speed;
		JMGVehicleAmmo()
		{
			allowError = false;
			range = 0.0f;
			speed = 400.0f;
		}
	};
	JMGVehicleAmmo primary;
	JMGVehicleAmmo secondary;
	JMGVehicleAction currentAction;
	JMGVehicleAction lastAction;
	int retreatTime;
	int lastSeenCount;
	int reverseTime;
	int stuckCount;
	int useAmmo;
	int doNotUsePathfind;
	float lastHealth;
	float minDistanceSquared;
	bool moving;
	bool attacking;
	int badDestAttempt;
	Vector3 mypos;
	Vector3 homepos;
	Vector3 retreatPos;
	int myteam;
	bool inRange;
	bool drivingBackward;
	float maxHuntRangeSquared;
	Vector3 lastWanderPointSpot;
	float grabNextPointDistance;
	void Created(GameObject *obj);
	void Action_Complete(GameObject *obj,int action,ActionCompleteReason reason);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Timer_Expired(GameObject *obj,int number);
	void RunAttack(GameObject *obj,GameObject *target);
	int GetThreatRating(GameObject * obj);
	GameObject *GetAttackObject(GameObject * obj);
	GameObject *SelectTarget(GameObject *obj,GameObject *target);
	GameObject *SetTarget(GameObject *target);
	GameObject *GetClosest(GameObject *obj,GameObject *new_target,GameObject *old_target);
	int SelectAmmo(GameObject *target);
	void StuckCheck(GameObject *obj);
	void ReturnHome(GameObject * obj);
	void AttackMove(GameObject *obj,GameObject *target,bool followTarget,Vector3 targetLocation,int fireMode,float weaponError,bool forceUpdate,float arriveDistance);
	JMGVehicleAmmo DefineAmmo(const AmmoDefinitionClass *ammo);
	bool GetRandomPosition(Vector3 *position);
	bool GetRandomPositionOutsideOfRange(Vector3 *position);
};

class DynamicClockSystem
{
public:
	struct DynamicClockSystemNode
	{
		int clockId;
		GameObject *clock;
		bool alarmSet;
		int alarmHour;
		int alarmMinute;
		int alarmSounding;
		struct DynamicClockSystemNode *next;
		DynamicClockSystemNode(GameObject *obj)
		{
			clockId = Commands->Get_ID(obj);
			clock = obj;
			alarmSet = false;
			alarmHour = 12;
			alarmMinute = 0;
			alarmSounding = 0;
			next = NULL;
		}
	};
private:
	bool HasLoaded;
	struct DynamicClockSystemNode *DynamicClockSystemNodeList;
public:
	DynamicClockSystem()
	{
		HasLoaded = false;
		DynamicClockSystemNodeList = NULL;
	}
	DynamicClockSystem &operator += (GameObject *obj)
	{
		DynamicClockSystemNode *Current = DynamicClockSystemNodeList;
		if (!DynamicClockSystemNodeList)
			DynamicClockSystemNodeList = new DynamicClockSystemNode(obj);
		while (Current)
		{
			if (Current->clock == obj)
				return *this;
			if (!Current->clock)
			{
				Current->clockId = Commands->Get_ID(obj);
				Current->clock = obj;
				return *this;
			}
			if (!Current->next)
			{
				Current->next = new DynamicClockSystemNode(obj);
				return *this;
			}
			Current = Current->next;
		}
		return *this;
	}
	DynamicClockSystem &operator -= (GameObject *obj)
	{
		if (!DynamicClockSystemNodeList)
			return *this;
		DynamicClockSystemNode *Current = DynamicClockSystemNodeList;
		while (Current)
		{
			if (Current->clock == obj)
			{
				Current->clock = NULL;
				break;
			}
			Current = Current->next;
		}
		return *this;
	}
	void emptyList()
	{
		HasLoaded = false;
		DynamicClockSystemNode *temp,*die;
		temp = DynamicClockSystemNodeList;
		while (temp)
		{
			die = temp;
			temp = temp->next;
			delete die;
		}
		DynamicClockSystemNodeList = NULL;
	}
	inline void updateClocks()
	{
		time_t t = time(0);
		struct tm *ptm = localtime(&t);
		int curTime = ptm->tm_hour%24;
		curTime = (curTime < 0 ? curTime+24 : curTime);
		float frame = (curTime*60.0f+ptm->tm_min);
		DynamicClockSystemNode *Current = DynamicClockSystemNodeList;
		while (Current)
		{
			if (Current->clock)
				Commands->Set_Animation(Current->clock,"s_ecwClock.a_ecwClock",false,0,frame,frame,true);
			Current = Current->next;
		}
	}
};
class JMG_Utility_Dynamic_Clock_Control : public ScriptImpClass {
	int lastMinute;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
	int getMinute();
};

class JMG_Utility_Dynamic_Clock_Object : public ScriptImpClass {
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
};

/*!
* \brief Fades screen color to set color while in zone
* \Color[Red|Green|Blue] RGB value of the color you want it to fade to (0-1.0)
* \Opacity How opac should the screen get while in the zone
* \EnterFadeInTime - Amount of time it takes to fade in once zone has been entered
* \ExitFadeOutTime - Amount of time it takes to fade out once the zone has been exited
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Change_Screen_Color_While_In_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
};

/*!
* \brief Resets screen color and opacity on object destruction
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Reset_Screen_Color_When_Destroyed : public ScriptImpClass
{
  void Destroyed(GameObject *obj);
};

/*!
* \brief Plays music for a player on join, the music can be changed for all players by calling JMG_Utility_Play_Music_On_Join::Set_Music
* \Music - Name of the music file to play, you need the file extension.
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Play_Music_On_Join_Controller : public ScriptImpClass {
	bool playingMusic[128];
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
	static char musicFileName[256];
public:
	static bool controllerPlaced;
	SCRIPTS_API static void Set_Music(const char *musicFilName,int fadeOut,int fadeIn);
};

/*!
* \brief  Switches what music is playing, you must have a JMG_Utility_Play_Music_On_Join_Controller on the map!
* \Music - Name of the music file to play, you need the file extension.
* \FadeOut - Amount of time the last song takes to fade out.
* \FadeIn - Amount of time the last song takes to fade in.
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Play_Music_On_Join_Change_Music : public ScriptImpClass {
	void Created(GameObject *obj);
};

/*!
* \brief  Just like JFW_Toggle_Door except you can't toggle during transition, and the switch object can also be animated
* \DoorID - ID of the simple object that is the 'door'
* \Animation - Animation for the door (Hint: Dump a comma delimited string in here to populate all the parameters besides DoorID)
* \SwitchAnimation - Animation for the switch object
* \LastFrame - Last frame for the door to goto.
* \SwitchLastFrame - Last frame to use for the switch
* \Key - Key needed to open door, 0 for no key
* \PlayerType - Playertype needed to open door, 2 for anyone
* \DisableSwitchWhenNoShield - Whether or not the switch can be disabled by being damaged to the point of having 0 shield, can be reenabled by repairing back to full health again
* \SwitchDisabledModel - Model the switch switches to when destroyed (uses first frame of model's animation)
* \DoorSound - Sound the door makes when opening/closing, null for no sound
* \SwitchSound - Sound the switch makes when poked, null for no sound
* \SwitchBlockedSound - Sound the switch makes when poked and cannot accept command, null for no sound
* \SwitchDamagedPokeSound - Sound the switch makes when poked while damaged, null for no sound
* \WrongKeySound - Sound that is played if the person pressing the switch doesn't have the key, null means no sound
* \WrongPlayerTypeSound - Sound that is played if the palyer type isn't a match for the switch
* \TextColor[Red|Green|Blue] - Color of the messages that will be displayed when toggling the switch, values are from 0-1.0.
* \GotoLastFrameMessage - Message displayed when the door goes to the last frame, null for no message
* \GotoFirstFrameMessage - Message displayed when door goes to the first frame, null for no message
* \ActionBlockedMessage - Message displayed when door can't be toggled, null for no message
* \NoDoorMessage - Text message that is displayed if the door id cannot be found (useful if the door can be destroyed)
* \SwitchDamagedPokeMessage - Message that is displayed if the switch is damaged and cannot operate
* \WrongKeyMessage - Message that is displayed if the person pressing the switch doesn't have the key, null means no message
* \WrongPlayerTypeMessage - Message that is displayed if the palyer type isn't a match for the switch
* \DoorControlCustom - Send this message to the switch to open or close the door, switch must not be damaged, param of 1 goes to last frame 0 to first
* \SendCustomId - Sends a message to this id when user pokes the switch
* \SendCustom - Sends this message to the above id, param 0 = close, 1 = open, 2 = blocked, 3 = wrong player type, 4 = wrong key, 5 = damaged, 6 = door destroyed
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Toggle_Door : public ScriptImpClass {
	float preDamagedFrame;
	char originalModel[16];
	bool open;
	bool enabled;
	void Created(GameObject *obj);
	void Poked(GameObject *obj,GameObject *poker);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void SendCustom(GameObject *obj,int param);
};

/*!
* \brief  Sets the animation of an object to the frame specified on script attach
* \Animation - Animation to play
* \Frame - Animation frame to display
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Set_Animation_Frame_On_Creation : public ScriptImpClass {
	void Created(GameObject *obj);
};

/*!
* \brief  Switches the model to different models as the object is damaged (Hint: Use skeleton models to fully support animations being synced across models)
* \FullShieldModel - Model to display when shield hits 100%
* \HalfShieldModel - Model to display when shield crosses the 50% mark
* \NoShieldModel - Model to display when shield hits 0%.
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Set_Model_On_Damage_Percent_3_States : public ScriptImpClass {
	int damageState;
	float healthThresholds[3];
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void SetModel(GameObject *obj);
};

/*!
* \brief  Switches the model to different models as the object is damaged (Hint: Use skeleton models to fully support animations being synced across models)
* \FullShieldModel - Model to display when shield hits 100%
* \TwoThirdsShieldModel - Model to display when shield crosses the 66% mark
* \ThirdShieldModel - Model to display when shield crosses the 33% mark
* \NoShieldModel - Model to display when shield hits 0%.
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Set_Model_On_Damage_Percent_4_States : public ScriptImpClass {
	int damageState;
	float healthThresholds[4];
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void SetModel(GameObject *obj);
};

/*!
* \brief  Works just like a normal PCT except you can definie zones with JMG_Utility_PCT_Inaccessible_Zone in which the PCT cannot be accessed by players
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_PCT : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
public:
	static int pctInaccessible[128];
};

/*!
* \brief  If the player is in this zone they won't be able to access any PCT defined by JMG_Utility_PCT
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_PCT_Inaccessible_Zone : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
};

/*!
* \brief  Script is attached by JMG_Utility_PCT_Inaccessible_Zone, no need to do anything with this one
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_PCT_Inaccessible_Zone_Attach : public ScriptImpClass {
	void Destroyed(GameObject *obj);
};
/*!
* \brief Displays a hud message to all players on custom, allows user to override string with custom string
* \Custom - Custom message to trigger the script on
* \StringId - ID of the string to display (Don't use strings used by the game as this will be replaced)
* \ReplaceString - Message to replace the string ID with
* \ColorRGB - Color of the hud message to display 0-1
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Display_HUD_Info_Text_All_Players_Custom_Replace_String : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Displays a hud message to all players on custom, allows user to override string with custom string
* \Custom - Custom message to trigger the script on
* \Message - Message to display to all players
* \ColorRGB - Color of the hud message to display 0-255
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Display_Text_Message_To_All_Players_On_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Sets an object team on attach
* \PlayerType - Player type to set the object to.
* \Delay - Amount of time to wait before setting team, default 0
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Set_Team_On_Create : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Makes the AI run up to an enemy and attack it as close as possible, used for melee AI
* \ReturnHomeSpeed - How fast it runs home after all enemies are dead
* \ReturnHomeArriveDistance - How close it goes to its original home location before calling it a day
* \ChaseSpeed - The speed multiplier for how fast it runs after enemies
* \AttackArriveDistance - How close it gets to enemies when attempting to attack them
* \StartAttackDistance - The distance at which it can start attacking
* \MaxHuntDistance - The max distance it can hunt a target from its home location
* \MinLooseVisibilityTime - Min amount of time it takes it to forget a target it can't see
* \MaxLooseVisibilityTime - Max amount of time it takes it to forget a target it can't see
* \NoPathfindDistance - At what distance to quit using pathfind, useful for making it not take weird paths to get to the player when close
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Aggressive_Melee : public ScriptImpClass {
	float noPathfindRange;
	int LastSeen;
	int lastSeenSecondary;
	int currentTargetID;
	int secondaryTargetId;
	int huntorattack;
	int waitcount;
	Vector3 homelocation;
	float speed;
	int minVisibilityTime;
	int maxVisibilityTime;
	float maxHuntDistance;
	Vector3 lastPos;
	int stuckTime;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	bool chooseTarget(GameObject *obj,GameObject *damager,int *compareId,int *seenTimer);
};

/*!
* \brief Script used to make placeable/building objects for infantry
* \WeaponPreset – Weapon that must be selected to deploy/preview
* \PreviewPreset – Preset that is used to display the preview
* \DeployPreset - Preset that is placed when the deploy key is pressed
* \RepairedPreset – Preset that is used when the deployed preset has been fully repaired
* \PlaceablePreview - 3d model used to show the object can be placed
* \UnPlaceablePreview - 3d model used to show the object cannot be placed
* \EnableCustom - Custom message used to enable placement
* \DisableCustom - Custom message used to disable placement (used by db building checking and water)
* \DeployCustom – Custom message that is used to place the deploy preset and sets the ammo to 0, also starts the regain time.
* \RegainTimer - Timer used to give the player the next deployable object after the last was placed 
* \Cost - How much it costs the player to place this object
* \MessageColor[R|G|B] – Color of messages to display to the player
* \DeploymentBlockedMessage – Message to display when the object cannot be deployed
* \PlacementMessage – Message that appears when the reload time has finished 
* \GrantWeapon - If 1 the weapon defined will be granted as soon as the script is attached.
* \RegenStartsAfterRepair - If 0 regen countdown starts the second you place the object, otherwise it can't start until you repair the object
* \MatchTeam[Preview|Deployed|Repaired] - Match team controls what teams everything is hooked too when placed, if 1 it matches the player teams, if 0 it keeps it set to whatever the preset has for team
* \DestroyedRegenTime - This parameter controls how long it takes to get the object back in inventory if its destroyed before being reparied
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Infantry_Placed_Buildable_Object : public ScriptImpClass {
	bool canRegen;
	int previewObjectId;
	int placementBlocked;
	int reloadTime;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void Destroyed(GameObject *obj);
	void DestroyPreview();
public:
	JMG_Utility_Infantry_Placed_Buildable_Object()
	{
		placementBlocked = 0;
	}
};
class JMG_Utility_Infantry_Placed_Buildable_Object_Attached : public ScriptImpClass {
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj,GameObject *killer);
};


/*!
* \brief While this script is attached it will select the weapon in the player's inventory as long as they have that weapon and it isn't selected
* \WeaponPreset - Weapon to select while this script is attached.
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Lock_Weapon_Selection_While_Script_Attached : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Used to set the default fog values of the map if there is no water but there are soldiers with the swimming script JMG_Utility_Swimming_Zone
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Swimming_zDefault_Map_Fog_Values : public ScriptImpClass {
	void Created(GameObject *obj);
};
/*!
* \brief Used to trigger the simple swimming animation system this is to be used along side JMG_Utility_Swimming_Infantry, 
* \ any soldiers that enter the script zone without JMG_Utility_Swimming_Infantry attached will die.
* \WaterColor[R|G|B] - Color to fade the screen while in this swimming zone
* \WaterOpacity - How transparent the screen is while in this zone
* \WaterMinViewDistance - The min distance that the fog shows up when underwater
* \WaterMaxViewDistance - The max distance of the fog when underwater
* \SwimmingSpeedMultiplier - Speed multiplier when in water
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Swimming_Zone : public ScriptImpClass {
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enter);
	void Exited(GameObject *obj,GameObject *exiter);
public:
	struct PlayerWaterNode
	{
		Vector3 waterColor;
		float waterColorOpacity;
		float waterMinViewDistance;
		float waterMaxViewDistance;
		PlayerWaterNode()
		{
			this->waterColor = Vector3(0.28f,0.43f,0.55f);
			this->waterColorOpacity = 0.5f;
			this->waterMinViewDistance = 5.0f;
			this->waterMaxViewDistance = 15.0f;
		}
		PlayerWaterNode(Vector3 waterColor,float waterColorOpacity,float waterMinViewDistance,float waterMaxViewDistance)
		{
			this->waterColor = waterColor;
			this->waterColorOpacity = waterColorOpacity;
			this->waterMinViewDistance = waterMinViewDistance;
			this->waterMaxViewDistance = waterMaxViewDistance;
		}
	};
	static JMG_Utility_Swimming_Zone::PlayerWaterNode waterNode[128];
	static float fogMinDistance;
	static float fogMaxDistance;
	SCRIPTS_API static void Update_Fog_Settings(float minFog,float maxFog);
};

/*! 
* \brief This script allows a soldier to swim when in a swimming zone. Weapon that is granted should use the Launcher position as  
* \ Renegade never made use of that animation set. If using my swimming animations make sure there is a plane for infantry 
* \ to stand on 1.466 meters below the water surface. The underwater[playerId] can be accessed from anywhere, allowing you to disable  
* \ screen fading when underwater from other scripts. 
* \WeaponPreset - Weapon to lock the player to while swimming, make sure its type launcher to make use of my animations 
* \DrownTime - Time it takes before you start taking damage when crouched under water for long periods of time 
* \StarDrownSequence - How long before you start taking damage to start fading the screen red and the heart beat sound 
* \GaspForBreath - This sound is played when you surface from under water after long periods of time 
* \PantingSoundEmitterModel - This 3d object is attached to the player and should be a looped sound effect, it exists while getting close to drowning 
* \HeartBeatSoundEmitterModel - This 3d object is attached to the player and should be a looped sound effect, it exists while catching your breath 
* \DrownDamageRate - Damage applied 10 times a second while drowning
* \CatchBreathRate - Rate at which a character catches its breath when out of the water, 0.1 would recover 1 second of air every second
* \WaterDamageAmount - Amount of damage to apply to the character while swimming, is applied 10 tiems a second, default is 0
* \WaterDamageWarhead - Warhead to use when in the water, default is None
* \EnterWaterMessageStringId - ID of the string in strings.tbl to display when a player enters the water, the message only displays once every 10 seconds
* \WaterEnterMessageColor[R|G|B] - Color of the hud message when entering the water, 0.0-1.0 RGB
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Swimming_Infantry : public ScriptImpClass {
	int heartBeatSoundId;
	int pantSoundId;
	char enterWeapon[256];
	int playerId;
	bool startedFadeRed;
	float drownTime;
	bool underwater;
	int waterZoneCount;
	int lastWaterZoneId;
	time_t lastDisplayTime;
	float defaultSpeed;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Killed(GameObject *obj,GameObject *killer);
	void Destroyed(GameObject *obj);
	void Detach(GameObject *obj);
	void CreateSoundEmitter(GameObject *obj,const char *model,int *soundId);
	void DestroySoundEmitter(int *soundId);
public:
	static bool isUnderwater[128];
	static bool isInWater[128];
	static bool getInWater(int playerId){return isUnderwater[playerId];}
	static bool getIsUnderwater(int playerId){return isInWater[playerId];}
};

/*!
* \brief Used to enable or disable all spawners within an ID range on zone enter
* \StartID - ID to start at
* \EndID - ID to stop at
* \PlayerType - Player type the zone triggers for
* \Enable - Enable or disable all the spawners in the range
* \TriggerOnce - Allows the script only to trigger the first time the zone is entered
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Zone_Enable_Spawners_In_Range : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

/*!
* \brief Used to display a HUD message to a player that enters a vehicle
* \StringId - ID of the string to display the the player that entered
* \MessageOverride - Message to override the string with, leave null if you want to use the default string
* \Color[R|G|B] - Color of the string to display
* \DriverOnly - Only display this to the driver of the vehicle
* \ShowOnce - Only display this once to each player that enters this vehicle
* \PlayerType - Team required for message to display
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Display_Message_On_Vehicle_Enter : public ScriptImpClass {
	bool hasShownMessage[128];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Used to apply damage to an object id or the object that entered the zone, damager is the script zone
* \ID - ID of the object to apply damage to, if ID is 0 damage will be applied to the object entering the zone
* \DamageAmount - Amount of damage to apply to the object
* \Warhead - Warhead to use to damage the object
* \DamageOccupants - If the object is a vehicle the the occupants of the vehicle will be damaged, useful for making a deathzone that kills everything that enters it.
* \PlayerType - Player type required to trigger the script
* \OnlyOnce - Does the script fire one time only
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Zone_Apply_Damage_On_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

/*!
* \brief Makes an aircraft move between wander points, it'll strafe at the target while it has one
* \WanderingAIGroupID - Group of points to wander between
* \FlightHeight - How high it should stay above the points
* \FireRange - Max range that it can use it's guns
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Guardian_Aircraft : public ScriptImpClass {
	Vector3 dpPosition;
	int EnemyID;
	int EnemyTimeOutTime;
	Vector3 LastPos;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Goto_Location(GameObject *obj);
	bool Get_A_Defense_Point(Vector3 *position);
};

/*!
* \brief Makes a unit switch its gun to a secondary gun whenever the ammo count in its primary clip runs out
* \PrimaryWeapon - Weapon that the game watches to see if its clip is empty
* \SecondaryWeapon - Weapon that is switched to while the primary weapon is "reloading"
* \ReloadTime - How long it takes to reload the primary weapon
* \PrimaryToSecondaryTime - Time it takes to play the animation to switch to the secondary weapon
* \SecondaryToPrimaryTime - Time it takes to play the animation to switch to the primary weapon
* \IdlePrimaryAnim - Animation to play while using the primary weapon
* \IdleSecondaryAnim - Animation to play while using the secondary weapon
* \PrimaryToSecondaryAnim - Animation to play while switching to the secondary weapon
* \SecondaryToPrimaryAnim - Animation to play while switching to the primary weapon
* \PrimaryToSecondarySound - Sound to play while switching to the secondary weapon
* \SecondaryToPrimarySound - Sound to play while switching to the primary weapon
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Switch_Weapon_While_Primary_Empty : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Sends a custom when an object gets near a building or moves away from a building
* \SendCustomObjectID - ID of the object to send the message to, if 0 it sends the messages to itself
* \NearToBuildingCustom - Message to send when a building is within range
* \FarFromBuildingCustom - Message to send when buildings are no longer within range
* \CloseToBuildingDistance - Distance to the nearest POLYGONS CENTER POINT of the building to count as being in range
* \BuildingPlayerType - Player type required to detect the building
* \CheckDeadBuildings - If true dead buildings are also checked
* \CheckRate - How often should the range detection be preformend
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Send_Custom_When_Near_Building : public ScriptImpClass {
	bool nearBuilding;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Used to designate repair targets for the Engineer AI that are things other than vehicles, turrets, and soldiers
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Engineer_Repair_Target : public ScriptImpClass {
	void Created(GameObject *obj);
};

class JMG_Utility_Reset_Screen_Fade_And_Fog_On_Destroy : public ScriptImpClass {
	void Destroyed(GameObject *obj);
	void Detach(GameObject *obj);
};

/*!
* \brief This script makes the AI hunt down the nearest player, it works for infantry and vehicles
* \HuntSearchDistance - Distance to scan for players to magically find and move to, -1 means infinite range
* \HuntSpeed - Speed at which the bot moves once its moving to a player
* \HuntArriveDistance - Distance at which to arrive from the player
* \RandomHuntArriveDistance - Random amount to arrive from the player
* \HuntStealth - Can the bot magically know where invisible players are
* \AttackSpeed - Speed to move at once an attack is begun
* \AttackDistance - Distance to arrive from a target when attacking, -1 uses the weapon effective distance
* \RandomAttackDistance - Random amount to add or subtract from the Attack distance
* \ReturnHome - Return to the create location when the action completes? (If wanderpoints are enabled this will never be used)
* \ReturnHomeSpeed - Speed to move at when going home
* \WanderingAIGroupID - Group of wander points to use when nothing else is going on
* \WanderSpeed - Speed to move at between the points
* \CanSeeStealth - Can the AI see stealth targets? 0 = No at all, 1 is within the ranges set in LE globals file, 2 means it can see stealth everywhere, think of stock AI.
* \ShutdownEngineOnArrival - Used for vehicles, turn on if you have issues with your vehicle rolling away from its move positions after it arrives
* \AttackCheckBlocked - Defines whether they should check if they can actually hit the player before shooting
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Goto_Player : public ScriptImpClass {
	enum aiState{IDLE,HUNTING_STAR,ATTACKING_TARGET,RETURNING_HOME,WANDERING_GROUP,ACTION_BADPATH};
	struct LastAction
	{
		int targetId;
		Vector3 location;
		float speed;
		float distance;
		bool attack;
		bool overrideLocation;
		LastAction()
		{
		}
		LastAction(int targetId,Vector3 location,float speed,float distance,bool attack,bool overrideLocation)
		{
			this->targetId = targetId;
			this->location = location;
			this->speed = speed;
			this->distance = distance;
			this->attack = attack;
			this->overrideLocation = overrideLocation;
		}
	};
	LastAction lastAction;
	aiState state;
	Vector3 homeLocation;
	bool huntStealth;
	int targetId;
	int lastSeenTime;
	float weaponRange;
	float weaponEffectiveRange;
	int huntingStarId;
	int ignoreStarsTime[128];
	float huntSearchDistance;
	float huntArriveDistance;
	float attackArriveDistance;
	int stuckTime;
	int reverseTime;
	Vector3 lastPosition;
	bool moveBackward;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Attack_Move(GameObject *obj,GameObject *target,Vector3 location,float speed,float distance,bool attack,bool overrideLocation);
	GameObject *findClosestStar(GameObject *obj);
	void Return_Home(GameObject *obj);
	void Stuck_Check(GameObject *obj,Vector3 targetPos);
	void Cant_Get_To_target(GameObject *obj);
	bool GetRandomPosition(Vector3 *position);
	bool Choose_Target(GameObject *obj,GameObject *target);
};

class AggressiveAttackSpotSystem
{
public:
	struct AggressiveAttackSpotNode
	{
		int id;
		Vector3 position;
		Vector3 attackOffset;
		bool alive;
		int groupId;
		struct AggressiveAttackSpotNode *next;
		AggressiveAttackSpotNode(GameObject *obj,int groupId,Vector3 attackOffset)
		{
			this->id = Commands->Get_ID(obj);
			this->position = Commands->Get_Position(obj);
			this->attackOffset = attackOffset;
			this->alive = true;
			this->groupId = groupId;
			this->next = NULL;
		}
	};
private:
	int nodeCount;
	AggressiveAttackSpotNode *aggressiveAttackSpotNodeList;
public:
	AggressiveAttackSpotSystem()
	{
		nodeCount = 0;
		aggressiveAttackSpotNodeList = NULL;
	}
	AggressiveAttackSpotNode *addNode(GameObject *obj,int groupId,Vector3 attackOffset)
	{
		int id = Commands->Get_ID(obj);
		AggressiveAttackSpotNode *current = aggressiveAttackSpotNodeList;
		nodeCount++;
		if (!aggressiveAttackSpotNodeList)
			return aggressiveAttackSpotNodeList = new AggressiveAttackSpotNode(obj,groupId,attackOffset);
		while (current)
		{
			if (current->id == id)
			{
				nodeCount--;
				return current;
			}
			if (!current->next)
			{
				current->next = new AggressiveAttackSpotNode(obj,groupId,attackOffset);
				return current->next;
			}
			current = current->next;
		}
		return NULL;
	}
	void killNode(GameObject *obj)
	{
		int id = Commands->Get_ID(obj);
		AggressiveAttackSpotNode *current = aggressiveAttackSpotNodeList;
		while (current)
		{
			if (current->id == id)
				current->alive = false;
			current = current->next;
		}
	}
	AggressiveAttackSpotNode *GetRandomNode(int groupId)
	{
		if (The_Game()->Is_Game_Over())
			return NULL;
		int lastCount = -1;
		int random = nodeCount > 0 ? Commands->Get_Random_Int(0,nodeCount*2) : 0;
		AggressiveAttackSpotNode *current = aggressiveAttackSpotNodeList;
		while (current)
		{
			if (current->alive && (groupId == -1 || current->groupId == groupId))
			{
				if (random)
					random--;
				if (!random)
					return current;
			}
			current = current->next;
			if (!current && lastCount != random)
			{
				lastCount = random;
				current = aggressiveAttackSpotNodeList;
			}
		}
		return NULL;
	}
	void Empty_List()
	{
		nodeCount = 0;
		AggressiveAttackSpotNode *temp = aggressiveAttackSpotNodeList,*die;
		while (temp)
		{
			die = temp;
			temp = temp->next;
			delete die;
		}
		aggressiveAttackSpotNodeList = NULL;
	}
};

/*!
* \brief Used to clean up the attack point list at gameover
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Aggressive_Attack_Spot_Control : public ScriptImpClass
{
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
};

/*!
* \brief Used to specify a point that JMG_Utility_AI_Aggressive_Attack_Spot should use to attack
* \GroupId - defines a group id for this point to belong to
* \AttackOffset - defines an offset for the AI to aim at when it arrives at the point
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Aggressive_Attack_Spot_Point : public ScriptImpClass
{
	AggressiveAttackSpotSystem::AggressiveAttackSpotNode *node;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
public:
	JMG_Utility_AI_Aggressive_Attack_Spot_Point()
	{
		node = NULL;
	}
};

/*!
* \brief Used to specify a point that JMG_Utility_AI_Aggressive_Attack_Spot should use to attack, this one sets the absolute position to fire at
* \GroupId - defines a group id for this point to belong to
* \AbsolutePosition - defines the exact spot for the AI to aim at when it arrives at the point
* \ObjectID - ID of an object to use as the absolute position (WARNING: This object is destroyed by this script)
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Aggressive_Attack_Spot_Point2 : public ScriptImpClass
{
	AggressiveAttackSpotSystem::AggressiveAttackSpotNode *node;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
public:
	JMG_Utility_AI_Aggressive_Attack_Spot_Point2()
	{
		node = NULL;
	}
};

/*!
* \brief Used to designate repair targets for the Engineer AI that are things other than vehicles, turrets, and soldiers
* \GroupId - Group of attack points to move and fire at, -1 means it can use any attack point group
* \SecondaryGroupId - Secondary group to atttack if primary is gone
* \AttackPointSpeed - What speed to move at when moving to attack an attack point
* \AttackPointDistance - What distance to arrive at when attacking an attack point
* \RandomAttackPointDistance - What random distance to arrive at when attacking an attack point (only randomly generated once)
* \AttackSpeed - What speed to move at when moving to attacking a target
* \AttackDistance - What distance to arrive at when attacking a target
* \RandomAttackDistance - What random distance to arrive at when attacking a target (only randomly generated once)
* \ReturnHome - Should it return to its home location if it cannot find any attack points?
* \ReturnHomeSpeed - Speed it should move at when returning home
* \ReactToAttackChance - The chance that the AI will react to an attack, the flag to acknowledge the damage is updated once every time the timer expires below
* \ReactToAttackRate - Rate at which the AI will recalculate whether or not it'll acknowledge attack damage
* \ShutdownEngineOnArrival - used for vehicles to shut off their engines when they arrive at the point
* \EnableDebug - If true the bot will print out any points it cannot get to WARNING: this will print off both when the bot can't reach it due to the point being in a bad spot AND the bot being in a bad spot
* \UseSecondaryFire - Should secondary fire be used when attacking
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Aggressive_Attack_Spot : public ScriptImpClass {
	enum aiState{IDLE,ATTACKING_POINT,RETURNING_HOME,ATTACKING_ATTACKER,ACTION_BADPATH};
	struct LastAction
	{
		int targetId;
		Vector3 location;
		float speed;
		float distance;
		bool attack;
		bool overrideLocation;
		bool attackingPoint;
		LastAction()
		{
		}
		LastAction(int targetId,Vector3 location,float speed,float distance,bool attack,bool overrideLocation,bool attackingPoint)
		{
			this->targetId = targetId;
			this->location = location;
			this->speed = speed;
			this->distance = distance;
			this->attack = attack;
			this->overrideLocation = overrideLocation;
			this->attackingPoint = attackingPoint;
		}
	};
	AggressiveAttackSpotSystem::AggressiveAttackSpotNode *attackNode;
	LastAction lastAction;
	aiState state;
	Vector3 homeLocation;
	int targetId;
	int lastSeenTime;
	float weaponRange;
	float weaponEffectiveRange;
	float attackArriveDistance;
	float attackPointArriveDistance;
	int stuckTime;
	int reverseTime;
	bool reactToAttackChance;
	Vector3 lastPosition;
	bool moveBackward;
	bool usePrimaryFire;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Attack_Move(GameObject *obj,GameObject *target,Vector3 location,float speed,float distance,bool attack,bool overrideLocation,bool attackingPoint);
	void Stuck_Check(GameObject *obj,Vector3 targetPos);
	void IdleChoice(GameObject *obj,bool allowAttackPoint);
	bool Choose_Target(GameObject *obj,GameObject *target);
public:
	JMG_Utility_AI_Aggressive_Attack_Spot()
	{
		attackNode = NULL;
	}
};

class JMG_Utility_Destroy_Objects_In_ID_Range_On_Death : public ScriptImpClass
{
	void Destroyed(GameObject *obj);
};

/*!
* \brief Used to enable or disable all spawners within an ID range on zone enter
* \StartID - ID to start at
* \EndID - ID to stop at
* \Custom - Custom used to trigger the enable/disable
* \Enable - Enable or disable all the spawners in the range, if -1 the param is used to determin this
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Enable_Spawners_In_Range : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Sends a custom message when the player count condition is hit
* \PlayerCount - Player count to trigger on
* \ID - Id to send the message to, if 0 it sends to itself
* \Custom - Custom to send
* \Param - Parameter to send with the message
* \Delay - Time delay to add to the sent message
* \TriggerOn[eq|<|>] - Defines what to trigger on, putting a 1 in the parameter makes that value checked, (obviously = means equal to the player count, < means less than the player count, > means greater than the player count) 
* \SupressMatchSpam - Makes it so the condition can't trigger again until the condition has not been meet then is meet again, useful for < or >
* \DestroyAfterTrigger - Makes the script remove itself after it fires once
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Send_Custom_On_Player_Count : public ScriptImpClass
{
	int custom;
	int param;
	float delay;
	int id;
	bool supressMatchSpam;
	Vector3 triggerEq;
	int playerCountParam;
	int playerCount;
	bool conditionMatching;
	bool destroyAfterTrigger;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Send_The_Message(GameObject *obj,int tempCount);
};

/*!
* \brief Sends a custom when the player count condition is hit
* \TriggerCustom - Custom to trigger the script on
* \Preset - Preset group to send the message too
* \Custom - Custom to send to the presets
* \Param - Param to send to the presets
* \Delay - Delay to add to the message before sending
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Send_Custom_To_Preset_On_Message : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Attaches the script supplied to anything spawned by a basic spawner
* \Script - Name of the script to attach
* \Params - The parameters to use for the script
* \Delim - The character to use in place of a ',' swapped at script creation
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Basic_Spawner_Attach_Script : public ScriptImpClass {
	char *params;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Sends a custom message when a preset enters the zone
* \PresetName - preset to trigger on
* \ID - Id of the object to send the custom to, 0 sends to itself, -1 sends to enterer
* \Message - Custom to send
* \Param - parameter to send with the custom
* \Delay - Time amount to wait before sending the custom
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Send_Custom_On_Preset_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

/*!
* \brief Allows you to set where the spawner creates the original spawn (this allows you to hide the flash when an object is created before its moved to the actual spawn spot)
* \OverrideLocation - Allows you to manually set the location to use for the prespawn, if its <0.0 0.0 0.0> it uses the location of the attached object.
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Basic_Spawner_Set_Prespawn_Location : public ScriptImpClass
{
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
};


/*!
* \brief Teleports a player to a location when they pickup a powerup, will move the player within the range defined if the spot is blocked
* \ObjectID - Id to teleport to
* \Position - Position to teleport to if ID isn't supplied
* \MaxDistance - Max range allowed to move the object if the spot is blocked
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Teleport_On_Pickup : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Sets the animation of an object when the script zone is entered
* \ObjectID - ID of the object to animated
* \Animation - Animation to play
* \Looping - Loop the animation
* \StartFrame - First frame to play
* \EndFrame - Last frame to play
* \Blended - Should the animation be blended
* \TriggerOnce - if 1 the script will be destroyed after being triggered once
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Zone_Set_Animation : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

/*!
* \brief Only use this script once unless you want to risk crashing
* \Scale - Scale to set the infantry to
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Scale_Infantry : public ScriptImpClass
{
	void Created(GameObject *obj);
};

/*!
* \brief Allows you to Enable/Disable an AI's Innate behavior on create
* \Enable - Should it be enabled
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Set_Innate_On_Create : public ScriptImpClass
{
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Allows you to Enable/Disable a soldiers damage animations
* \Enable - Should it be enabled
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Set_Soldier_Damage_Animations : public ScriptImpClass
{
	void Created(GameObject *obj);
};

/*!
* \brief  Switches what music is playing when an object enters the attached zone, you must have a JMG_Utility_Play_Music_On_Join_Controller on the map!
* \Music - Name of the music file to play, you need the file extension.
* \FadeOut - Amount of time the last song takes to fade out.
* \FadeIn - Amount of time the last song takes to fade in.
* \PlayerType - The player type the script triggers on
* \TriggerOnce - Only allow the script to trigger once
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Play_Music_On_Join_Enter_Change_Music : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

/*!
* \brief  Switches what music is playing when the attached object is killed, you must have a JMG_Utility_Play_Music_On_Join_Controller on the map!
* \Music - Name of the music file to play, you need the file extension.
* \FadeOut - Amount of time the last song takes to fade out.
* \FadeIn - Amount of time the last song takes to fade in.
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Play_Music_On_Join_Killed_Change_Music : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

/*!
============================================================================================================================================================
The objective system makes use of the following hard-coded string definitions, make sure to add them to your strings library if you plan on using my system:
============================================================================================================================================================
IDS_OBJ2_CANCELLED					%s mission objective canceled
IDS_OBJ2_CANCELLED_NUMBERED			%s mission objective %d canceled
IDS_OBJ2_LIST						%s Mission Objectives:
IDS_OBJ2_NEW_OBJ					New %s mission objective:
IDS_OBJ2_PRIORITY_0					Unknown
IDS_OBJ2_PRIORITY_1					Primary
IDS_OBJ2_PRIORITY_2					Secondary
IDS_OBJ2_PRIORITY_3					Tertiary
IDS_OBJ2_PRIORITY_4					Quaternary
IDS_OBJ2_PRIORITY_5					Quinary
IDS_OBJ2_PRIORITY_6					Senary
IDS_OBJ2_PRIORITY_7					Septenary
IDS_OBJ2_PRIORITY_8					Octonary
IDS_OBJ2_PRIORITY_9					Nonary
IDS_OBJ2_PRIORITY_10				Denary
IDS_OBJ2_PRIORITY_11				Bonus
IDS_OBJ2_STATE_0					Pending
IDS_OBJ2_STATE_1					Accomplished
IDS_OBJ2_STATE_2					Failed
IDS_OBJ2_STATE_3					Hidden
IDS_OBJ2_STATUS_CHANGED				%s mission objective %s
IDS_OBJ2_STATUS_CHANGED_NUMBERED	%s mission objective %d %s
IDS_OBJ2_UPDATED					%s mission objective updated:
IDS_OBJ2_UPDATED_NUMBERED			%s mission objective %d updated:
============================================================================================================================================================
*/
/*!
* \brief Used to control a basic objective system
* \ShowMarkersOnRadar - Should stars be shown on the radar
* \PrimaryObjectiveModel - Visible 3d model to show for Primary Objectives
* \SecondaryObjectiveModel - Visible 3d model to show for Secondary Objectives
* \TertiaryObjectiveModel - Visible 3d model to show for Tertiary Objectives
* \Team - Team to show the objectives for
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Objective_System_Controller : public ScriptImpClass
{
	char playerNames[128][256];
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
public:
	static bool controllerPlaced;
};

/*!
* \brief Used to update your objectives
* \NewObjectiveID - ID of the new objective to add
* \NewObjectiveStringID - Strings.tbl id for the objective message
* \ObjectiveMarkerObjectID - ID of the object where the marker should be displayed (-1 sets the enter as the marker)
* \CompleteObjectiveID - Objective to complete when this is trigger
* \Delay - Delay for Objective before adding the new one
* \NewObjectivePriority - Priority of the new objective {Undefined=-1,Unknown = 0,Primary = 1,Secondary = 2,Tertiary = 3,Quaternary = 4,Quinary = 5,Senary = 6,Septenary = 7,Octonary = 8,Nonary = 9,Denary = 10,Bonus = 11}
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Objective_System_Objective_Update_Enter : public ScriptImpClass {
	bool triggered;
	void Entered(GameObject *obj,GameObject *enter);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
public:
	JMG_Utility_Objective_System_Objective_Update_Enter()
	{
		triggered = false;
	}
};

/*!
* \brief Used to update your objectives
* \NewObjectiveID - ID of the new objective to add
* \NewObjectiveStringID - Strings.tbl id for the objective message
* \ObjectiveMarkerObjectID - ID of the object where the marker should be displayed (-1 sets this object that picked it up as the marker)
* \CompleteObjectiveID - Objective to complete when this is trigger
* \Delay - Delay for Objective before adding the new one
* \NewObjectivePriority - Priority of the new objective {Undefined=-1,Unknown = 0,Primary = 1,Secondary = 2,Tertiary = 3,Quaternary = 4,Quinary = 5,Senary = 6,Septenary = 7,Octonary = 8,Nonary = 9,Denary = 10,Bonus = 11}
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Objective_System_Objective_Update_Pickup : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Used to update your objectives
* \NewObjectiveID - ID of the new objective to add
* \NewObjectiveStringID - Strings.tbl id for the objective message
* \ObjectiveMarkerObjectID - ID of the object where the marker should be displayed (-1 sets the killer as the marker)
* \CompleteObjectiveID - Objective to complete when this is trigger
* \Delay - Delay for Objective before adding the new one
* \NewObjectivePriority - Priority of the new objective {Undefined=-1,Unknown = 0,Primary = 1,Secondary = 2,Tertiary = 3,Quaternary = 4,Quinary = 5,Senary = 6,Septenary = 7,Octonary = 8,Nonary = 9,Denary = 10,Bonus = 11}
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Objective_System_Objective_Update_Killed : public ScriptImpClass {
	void Killed(GameObject *obj, GameObject *damager);
};


/*!
* \brief Used to update your objectives
* \NewObjectiveID - ID of the new objective to add
* \NewObjectiveStringID - Strings.tbl id for the objective message
* \ObjectiveMarkerObjectID - ID of the object where the marker should be displayed (-1 sets the sender as the marker)
* \CompleteObjectiveID - Objective to complete when this is trigger
* \Delay - Delay for Objective before adding the new one
* \NewObjectivePriority - Priority of the new objective {Undefined=-1,Unknown = 0,Primary = 1,Secondary = 2,Tertiary = 3,Quaternary = 4,Quinary = 5,Senary = 6,Septenary = 7,Octonary = 8,Nonary = 9,Denary = 10,Bonus = 11}
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Objective_System_Objective_Update_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Applies damage to the target when DESTROYED
* \ID - Id of the object to damage
* \Damage - Amount of damage to apply
* \Warhead - Warhead to use
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Destroyed_Apply_Damage : public ScriptImpClass {
	void Destroyed(GameObject *obj);
};

/*!
* \brief This script refunds damage on the attached object by a percentage of the number of players there are in game out of the max players supplied as a parameter
* \Math for this one basically is -PlayerCount/MaxPlayerCount*damage
* \MaxPlayersToScaleFor - This amount divides the players that are currently in game to get the base scale amount (The number of players is player count - 1, so when its just 1 player in game the scale is normal)
* \MaxScaleFactor - The maximum percent of damage that can be refunded
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Scale_Damage_By_Player_Count : public ScriptImpClass
{
	int maxPlayersToScaleFor;
	float maxScaleFactor;
	float damageRefund;
	int resurrectCount;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

/*!
* \brief This script refunds damage on the attached object by a percentage of the number of players there are in game out of the max players supplied as a parameter
* \Math for this one basically is -(PlayerCount*PlayerCount)/(MaxPlayerCount*MaxPlayerCount)*damage
* \MaxPlayersToScaleFor - This amount divides the players that are currently in game to get the base scale amount (The number of players is player count - 1, so when its just 1 player in game the scale is normal)
* \MaxScaleFactor - The maximum percent of damage that can be refunded
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Scale_Damage_Square_By_Player_Count : public ScriptImpClass
{
	int maxPlayersToScaleFor;
	float maxScaleFactor;
	float damageRefund;
	int resurrectCount;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

/*!
* \brief Regen's health via set health/shield  strength instead of Apply_Damage, useful for infantry if you don't want the Regen affect
* \ Health (if enabled) must regen all the way before the armor starts coming back, unless regen armor is -1, then armor must be regen'd full before health starts
* \RegenHealth - Can this regen health
* \HealthAmount - Amount of health to regen
* \HealthPerPlayer - Amount of health per player to regen (amount*player count)
* \RegenArmor - Can this regen armor, if the value here is -1 it'll regen armor first before health
* \ArmorAmount - Amount of armor to regen
* \ArmorPerPlayer - Amount of armor per player to regen (amount*player count)
* \Rate - Speed to regen
* \DamageDelay - Amount of time regen must wait to start again after taking damage
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Regen_HitPoints : public ScriptImpClass
{
	bool regenHealth;
	int regenArmor;
	float healthAmount;
	float armorAmount;
	float healthPerPlayer;
	float armorPerPlayer;
	float rate;
	float damageDelay;
	bool enabled;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

/*!
* \brief Toggles flight after delay completes
* \Delay - Time to wait before toggling flight
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Toggle_Flight_On_Delay : public ScriptImpClass
{
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};
	

/*!
* \brief Plays an animation and locks the soldier in place when they run out of armor, when armor is full again they stand back up
* \FaintAnimation - Animation to play when the armor hits 0
* \LayAnimation - Animation to play while laying on the ground
* \StandAnimation - Animation to play when armor is full
* \FaintSound - Sound to play when the soldier faints
* \StandSound - Sound to play when the soldier stands up
* \ChangeArmorTypeWhenKnockedOut - Should the armor type change while knocked out
* \ArmorTypeWhileKnockedOut - What to change the armor type to while knocked out
* \TeamWhileKnockedOut - What team to change to when knocked out (-99 to make the team not change)
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Fainting_Soldier : public ScriptImpClass
{
	int posLockId;
	bool fainted;
	char faintAnimation[32];
	char layAnimation[32];
	char standAnimation[32];
	char faintSound[256];
	char standSound[256];
	bool changeArmorTypeWhenKnockedOut;
	char armorTypeWhileKnockedOut[256];
	char armorType[256];
	int originalTeam;
	int teamWhileKnockedOut;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Animation_Complete(GameObject *obj,const char *animation);
	void Destroyed(GameObject *obj);
};

/*!
* \brief Makes a unit move between wander points, it'll strafe at the target while it has one, clone of JMG_Utility_AI_Guardian_Aircraft but with pathfind enabled
* \WanderingAIGroupID - Group of points to wander between
* \WanderSpeed - Speed the unit moves at 
* \FireRange - Max range that it can use it's guns
* \CheckBlocked - Whether to check if the target spot is blocked before firing
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Guardian_Infantry : public ScriptImpClass {
	Vector3 dpPosition;
	int EnemyID;
	int EnemyTimeOutTime;
	Vector3 LastPos;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Goto_Location(GameObject *obj);
	bool Get_A_Defense_Point(Vector3 *position);
};

/*!
* \brief Sets the maximum distance an AI can wander before returning home (the location it was created)
* \Distance - Max distance to wander to
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Set_Innate_Max_Wander_Distance : public ScriptImpClass
{
	void Created(GameObject *obj);
};

/*!
* \brief Selects empty hands on create, the weapon it was holding before script attach is counted as the primary weapon, 
* \ once a custom is received it switches back to the primary weapon.
* \Custom - Custom to trigger switching back to the primary weapon on.
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Switch_Weapon_To_Empty_Hands_Until_Custom : public ScriptImpClass {
	int custom;
	char weaponName[256];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};


/*!
* \brief Sets the skin type and armor type on a custom
* \Custom - Custom to trigger setting the armor and skin types
* \SkinType - Skin Type to switch to
* \ShieldType - Shield Type to switch to
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Set_Skin_And_Shield_Type_On_Custom : public ScriptImpClass {
	int custom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Used to damage all objects in an id range on enter
* \StartID - ID to start at
* \EndID - ID to stop at
* \PlayerType - Player type the zone triggers for
* \Damage - Amount of damage to apply
* \Warhead - Warhead to use
* \Damager - Object to do the damage (0 = NULL, 1 = The Enter, 2 = The Zone)
* \TriggerOnce - Allows the script only to trigger the first time the zone is entered
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Zone_Damage_All_In_ID_Range_On_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

/*!
* \brief Sends a custom to all objects with the specified script attached, sender is the enter
* \PlayerType - Player type the zone triggers for
* \AttachedScript - Name of the script that must be attached
* \Custom - Custom to send
* \Param - Parameter to send
* \Delay - Delay to wait before sending the customs
* \TriggerOnce - Allows the script only to trigger the first time the zone is entered
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Zone_Send_Custom_To_All_With_Script : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

/*!
* \brief Changes the model of an object when its total HitPoint Percent enters a range
* \LowerHitPointsPercent - Lower end of the range (percent of HitPoints)
* \UpperHitPointsPercent - Upper end of the range (percent of HitPoints)
* \Model - W3d model to switch to
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_HitPoints_In_Range_Change_Model : public ScriptImpClass {
	bool modelSet;
	float upperHP;
	float lowerHP;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Attaches a preset to the object while its total HitPoint Percent are inside a range (object is destroyed on controller death)
* \LowerHitPointsPercent - Lower end of the range (percent of HitPoints)
* \UpperHitPointsPercent - Upper end of the range (percent of HitPoints)
* \Preset - Name of the preset to attach
* \BoneName - Name of the bone to attach to
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_HitPoints_In_Range_Attach_Preset : public ScriptImpClass {
	int presetId;
	float upperHP;
	float lowerHP;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *killer);
	void Destroyed(GameObject *obj);
};

/*!
* \brief Enables a spawner while the HitPoint Percent is inside a range
* \LowerHitPointsPercent - Lower end of the range (percent of HitPoints)
* \UpperHitPointsPercent - Upper end of the range (percent of HitPoints)
* \SpawnerID - ID of the spawner to enable/disable
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_HitPoints_In_Range_Enable_Spawner : public ScriptImpClass {
	bool enabled;
	float upperHP;
	float lowerHP;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Creates a preset at the location specified while total HitPoint Percent are inside a range (object is destroyed on controller death)
* \LowerHitPointsPercent - Lower end of the range (percent of HitPoints)
* \UpperHitPointsPercent - Upper end of the range (percent of HitPoints)
* \Preset - Name of the preset to attach
* \Location - Location to place the object
* \Facing - Direction to face the object
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_HitPoints_In_Range_Create_Preset : public ScriptImpClass {
	int presetId;
	float upperHP;
	float lowerHP;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Killed(GameObject *obj,GameObject *killer);
	void Destroyed(GameObject *obj);
};

/*!
* \brief Sends a custom upon entering the HP range
* \LowerHitPointsPercent - Lower end of the range (percent of HitPoints)
* \UpperHitPointsPercent - Upper end of the range (percent of HitPoints)
* \ID - Id of the object to send the custom to, if 0 sends to self
* \Custom - Custom to send when within the range
* \Param - Parameter to send when in the range
* \Delay - Delay to add to the custom
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_HitPoints_In_Range_Send_Custom : public ScriptImpClass {
	bool inRange;
	float upperHP;
	float lowerHP;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Receiving a custom changes the Precipitation on the map
* \Custom - Custom event to trigger the change
* \Set[Rain|Snow|Ash|] - The value to set each type of precipitation (yes you can have more than one on at a time)
* \Change[Rain|Snow|Ash|] - Signals you want to change this value, if you leave it 0 the precipitation type will not be updated
* \TransitionTime - Amount of time it takes for the new value to be at full effect
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Set_Weather_Precipitation : public ScriptImpClass {
	int custom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Receiving a custom changes the Fog on the map
* \Custom - Custom event to trigger the change
* \StartDistance - Range at which the fog starts
* \EndDistance - Range at which the fog is at its thickest
* \TransitionTime - Amount of time it takes for the new value to be at full effect
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Set_Weather_Fog : public ScriptImpClass {
	int custom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Receiving a custom changes the Wind on the map
* \Custom - Custom event to trigger the change
* \Heading - Direction the wind blows
* \Speed - Speed the wind is blowing
* \Variability - How much the wind can change direction
* \TransitionTime - Amount of time it takes for the new value to be at full effect
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Set_Weather_Wind : public ScriptImpClass {
	int custom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Receiving a custom changes the Lightning on the map
* \Custom - Custom event to trigger the change
* \Intensity - How intense the lightning is
* \StartDistance - How far away is the cloest lightning strikes
* \EndDistance - How far away is the furthest lightning strikes
* \Heading - What direction is the lightning 
* \Distribution - How spread out is the lightning
* \TransitionTime - Amount of time it takes for the new value to be at full effect
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Set_Weather_Lightning : public ScriptImpClass {
	int custom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Receiving a custom changes the Warblitz on the map
* \Custom - Custom event to trigger the change
* \Intensity - How intense the warblitz is
* \StartDistance - How far away is the cloest warblitz strikes
* \EndDistance - How far away is the furthest warblitz strikes
* \Heading - What direction is the warblitz 
* \Distribution - How spread out is the warblitz
* \TransitionTime - Amount of time it takes for the new value to be at full effect
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Set_Weather_War_Blitz : public ScriptImpClass {
	int custom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Receiving a custom changes the Clouds on the map
* \Custom - Custom event to trigger the change
* \Cover - How thick are the clouds
* \Gloominess - How dark are the clouds
* \TransitionTime - Amount of time it takes for the new value to be at full effect
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Set_Weather_Clouds : public ScriptImpClass {
	int custom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Switches what music is playing on custom, you must have a JMG_Utility_Play_Music_On_Join_Controller on the map!
* \Custom - Custom event to trigger the change
* \Music - Name of the music file to play, you need the file extension.
* \FadeOut - Amount of time the last song takes to fade out.
* \FadeIn - Amount of time the last song takes to fade in.
* \Repeat - Can this be triggered more than once
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Play_Music_On_Join_Custom_Change_Music : public ScriptImpClass {
	int custom;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Sends a custom if the object entering the zone has a weapon
* \PlayerType - Player type the zone triggers for
* \WeaponName - Name of the weapon needed to trigger the zone
* \ID - ID to send the custom to, 0 sends to self, -1 sends to enter
* \Custom - Custom message to send
* \Param - Param to send
* \Delay - Delay to add before sending custom
* \TriggerOnce - Allows the script only to trigger the first time the zone is entered
* \RemoveWeapon - Removes the weapon from the player if true
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Zone_Send_Custom_If_Has_Weapon : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};


/*!
* \brief  Requires all customs to be received before custom can be sent (unless the custom has a value of 0)
* \Custom[0-9] - Customs needed, once all have been received send custom triggers
* \ID - ID to send the custom to, 0 sends to self, -1 sends to sender
* \Custom - Custom message to send
* \Param - Param to send
* \Delay - Delay to add before sending custom
* \TriggerOnce - Allows the script only to trigger the first time all customs are received
* \ResetCustom - Resets the current progress of the script
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Receive_All_Customs_To_Send_Custom : public ScriptImpClass {
	int resetCustom;
	int customs[10];
	bool receivedCustoms[10];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Teleports the object that enters the zone to a random wander point
* \WanderingAIGroupID - Group of points to teleport to
* \SafeTeleportDistance - How far can infantry be moved if the spot is blocked
* \ChangeGroupIDCustom - Changes the wander point group id to a new one, uses the parameter on the custom sent in
* \PlayerType - Playertype the zone triggers for
* \RetryOnFailure - If this is true a script will be attached that will continue to try to teleport the player until successful (Warning: Turning this on hides error messages)
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Zone_Teleport_To_Random_Wander_Point : public ScriptImpClass {
	bool retryOnFailure;
	int playerType;
	float safeTeleportDistance;
	int wanderPointGroup;
	int changeGroupIDCustom;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	bool Get_A_Defense_Point(Vector3 *position,float *facing);
	bool Grab_Teleport_Spot(GameObject *enter,int attempts);
};


/*!
* \brief Used by JMG_Utility_Zone_Teleport_To_Random_Wander_Point
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Zone_Teleport_To_Random_Wander_Point_Attach : public ScriptImpClass {
	float safeTeleportDistance;
	int wanderPointGroup;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	bool Get_A_Defense_Point(Vector3 *position,float *facing);
};

/*!
* \brief Changes the player type of the player's game object (not the player's team)
* \RequiredPlayerType - Player type that the player must be in order to be changed
* \SetPlayerType - Player type to change the player to once they enter the zone
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Zone_Set_Player_Type : public ScriptImpClass {
	int requiredPlayerType;
	int setPlayerType;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
};

/*!
* \brief Sends a custom to an object on entry
* \PlayerType - Player type the zone triggers for
* \ID - ID to send the custom to, 0 sends to self, -1 sends to enter
* \Custom - Custom message to send
* \Param - Param to send
* \Delay - Delay to add before sending custom
* \TriggerOnce - Allows the script only to trigger the first time the zone is entered
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Zone_Send_Custom_Enter : public ScriptImpClass {
	int playerType;
	int custom;
	int param;
	float delay;
	int id;
	bool triggerOnce;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
};

/*!
* \brief Destroys the closest preset to the position (works with powerups and other "dumb" objects)
* \Custom - Custom to trigger the script
* \PresetName - Name of the preset to hunt down and kill
* \Position - Position to search near
* \MaxDistance - Max distance to scan, 0 = infinite
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Destroy_Closest_Object_To_Position : public ScriptImpClass {
	int custom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Used to enable or disable a spawner, allows you to only have it trigger the script once
* \SpawnerID - ID of the spawner to enable/disable
* \PlayerType - Player type the zone triggers for
* \Enable - Enable or disable all the spawners in the range
* \TriggerOnce - Allows the script only to trigger the first time the zone is entered
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Zone_Set_Spawner : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

/*!
* \brief Attach this script to the powerup of the weapon you wish not to be lost throughout the game
* \WeaponName - Name of the weapon the powerup grants
* \PowerupNameOverride - Use this if you want a different powerup to be used after the initial pickup of this powerup (Default value = null)
* \DisarmCustom - Sending a soldier that currently has the weapon from this powerup will have the weapon remove and the script as well
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Persistant_Weapon_Powerup : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Script used by JMG_Utility_Persistant_Weapon_Powerup
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Persistant_Weapon_zStandin : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Script used by JMG_Utility_Persistant_Weapon_Powerup
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Persistant_Weapon_zAttached : public ScriptImpClass {
	int disarmCustom;
	char weaponName[256];
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
};

/*!
* \brief Teleports the object that enters the zone to a random wander point
* \Custom - Custom to trigger the teleport
* \ScanTeleportSpot - Center of the area for the distance
* \TeleportIgnoreDistance - Distance players must be outside of to be teleported
* \WanderingAIGroupID - Group of points to teleport to
* \SafeTeleportDistance - How far can infantry be moved if the spot is blocked
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Teleport_Players_Outside_Range_To_Wanderpoints : public ScriptImpClass {
	Vector3 scanTeleportSpot;
	int playerType;
	float safeTeleportDistance;
	float teleportIgnoreDistance;
	int wanderPointGroup;
	int custom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	bool Get_A_Defense_Point(Vector3 *position,float *facing);
	bool Grab_Teleport_Spot(GameObject *enter,int attempts);
};


/*!
* \brief Sets the animation frame of a tile when a custom is received
* \Custom - Custom to trigger the teleport
* \ID - Id of the tile to animate
* \Animation - Animation to use
* \Frame - Frame to goto
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Set_Tile_Frame : public ScriptImpClass {
	int custom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Sends a custom if the object entering the zone does not have a weapon
* \PlayerType - Player type the zone triggers for
* \WeaponName - Name of the weapon you don't want
* \ID - ID to send the custom to, 0 sends to self, -1 sends to enter
* \Custom - Custom message to send
* \Param - Param to send
* \Delay - Delay to add before sending custom
* \TriggerOnce - Allows the script only to trigger the first time the zone is entered
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Zone_Send_Custom_No_Weapon : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};



/*!
* \brief Reads text from a file and displays it to the screen, 
* \ the text in a file is broken every 150 characters (tries to end the current word) and then delayed by 6 seconds
* \ Note: This will only work once because of how it works
* \Custom - Custom to trigger this script on
* \FileName - Name of the file to read from
* \TextTag - Name of the tag to grab text from
* \MessageColor - Color to display the chat message in
* \LineDelayTime - Delay between each message displayed
* \UISound - 2D sound to play when custom received
* \Repeatable - Allows the script to trigger more than once
* \File Format Example
[Tag]
Text typed here will be displayed for this tag when the custom is received, the line can be up to 16384 characters long.
[AnotherTag]
Use this character ''(there is a character in there, you'll have to copy it out) to end the line, you can also use a bunch of them to add a delay to the message.
[LastTag]
The expected format of the file is always Tag Line Tag Line, never add blank lines or new line returns in tag text.
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Display_Briefing_Message : public ScriptImpClass {
	struct BriefingTextNode
	{
		char Text[256];
		float Delay;
		BriefingTextNode *next;
		BriefingTextNode(const char *text)
		{
			Delay = 0.0f;
			sprintf(Text,"%s",text); 
			next = NULL;
		}
		BriefingTextNode(const char *text,float delay)
		{
			Delay = delay;
			sprintf(Text,"%s",text); 
			next = NULL;
		}
		BriefingTextNode()
		{
			next = NULL;
		}
	};
	BriefingTextNode *BriefingText;
	BriefingTextNode *CurrentNode;
	bool triggered;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
	void AddNewTextNode();
	void RemoveTextNodes();
};

/*!
* \brief Changes the player's team the value
* \RequiredPlayerTeam - Player team that the player has to be in order to be changed
* \SetPlayerTeam - Player team to change the player to
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Zone_Set_Player_Team : public ScriptImpClass {
	int requiredPlayerTeam;
	int setPlayerTeam;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
};
/*!
* \brief Used to update your objectives
* \NewObjectiveID - ID of the new objective to add
* \NewObjectiveStringID - Strings.tbl id for the objective message
* \ObjectiveMarkerObjectID - ID of the object where the marker should be displayed (-1 sets this object as the marker)
* \CompleteObjectiveID - Objective to complete when this is trigger
* \Delay - Delay for Objective before adding the new one
* \NewObjectivePriority - Priority of the new objective {Undefined=-1,Unknown = 0,Primary = 1,Secondary = 2,Tertiary = 3,Quaternary = 4,Quinary = 5,Senary = 6,Septenary = 7,Octonary = 8,Nonary = 9,Denary = 10,Bonus = 11}
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Objective_System_Objective_Update_Create : public ScriptImpClass {
	bool triggered;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
public:
	JMG_Utility_Objective_System_Objective_Update_Create()
	{
		triggered = false;
	}
};

/*!
* \brief Used to remove an objective on custom
* \Custom - Custom to trigger this script
* \ObjectiveID - ID of the new objective to remove
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Objective_System_Objective_Remove_Custom : public ScriptImpClass {
	int custom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Rotates the player's camera after a delay
* \Time - Time amount to wait
* \Facing - Direction to rotate to
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Delay_Then_Rotate_Camera : public ScriptImpClass {
	bool triggered;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Applies damage to all smart game objects in range
* \Range - Range to damage objects inside of
* \Damage - Damage to do each update
* \Warhead - warhead to use to apply the damage
* \Rate - the speed at which to apply the damage
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Damage_SmartGameObjects_In_Range : public ScriptImpClass {
	float range;
	float damage;
	char warhead[128];
	float rate;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Use this script to get AI back out of the ground
* \Distance - Max distance to try to move the infantry if stuck in the ground
* \Rate - How often should it check if the infantry is stuck in the ground
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Unstick_Infantry_If_Stuck : public ScriptImpClass {
	float distance;
	float rate;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Sends a custom when a custom has been received x times
* \Custom - Custom to count
* \Count - how many customs needed
* \ID - ID to send to, 0 sends to self, -1 sends to sender
* \SendCustom - custom to send
* \Param - param to send
* \Delay - delay to add
* \ResetCustom - custom to reset the count
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Send_Custom_On_Count : public ScriptImpClass {
	int custom;
	int resetCustom;
	int count;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Destroy self when a custom is received
* \Custom - Custom to trigger on
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Destroy_Self : public ScriptImpClass {
	int custom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};
/*!
* \brief Changes the player's team to the value specified, then kills the player, finally subtracting 1 from the death count
* \RequiredPlayerTeam - Player team that the player has to be in order to be changed
* \SetPlayerTeam - Player team to change the player to
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Zone_Set_Player_Team2 : public ScriptImpClass {
	int requiredPlayerTeam;
	int setPlayerTeam;
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
};

/*!
* \brief Sends a custom message on poke, also enables the pokable indicator icon
* \ID - ID to send the custom to, 0 sends to self, -1 sends to poker
* \Custom - Custom message to send
* \Param - Param to send
* \Delay - Delay to add before sending custom
* \TriggerOnce - Allows the script only to trigger the first time all customs are received
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Poke_Send_Custom : public ScriptImpClass {
	void Created(GameObject *obj);
	void Poked(GameObject *obj, GameObject *poker);
};

/*!
* \brief Locks the collision mode of an object on create
* \CollisionGroupID - ID of the collision mode to use
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Set_Collision_Group : public ScriptImpClass {
	void Created(GameObject *obj);
};


/*!
* \brief Used to cap the credits of a team, can be updated by sending a custom
* \Credits - What is the max amount of credits the player can have
* \Team - Team that the script affects
* \Custom - A custom message that is used to update the max credits, parameter is the amount
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Cap_Credits : public ScriptImpClass {
	int team;
	float credits;
	int custom;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Applys damage to an object when custom is received
* \Custom - Custom to trigger on
* \ID - ID of the object to damage 0 = itself
* \Damage - Damage to do to the object
* \Warhead - Warhead to use
* \DamagerID - ID of what does the damage, -1 = sender, 0 = itself, -2 = object script is attached to
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Apply_Damage : public ScriptImpClass {
	int custom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief This script makes the AI hunt down the nearest enemy, it works for infantry and vehicles
* \HuntSearchDistance - Distance to scan for players to magically find and move to, -1 means infinite range
* \HuntSpeed - Speed at which the bot moves once its moving to a player
* \HuntArriveDistance - Distance at which to arrive from the player
* \RandomHuntArriveDistance - Random amount to arrive from the player
* \HuntStealth - Can the bot magically know where invisible players are
* \AttackSpeed - Speed to move at once an attack is begun
* \AttackDistance - Distance to arrive from a target when attacking, -1 uses the weapon effective distance
* \RandomAttackDistance - Random amount to add or subtract from the Attack distance
* \ReturnHome - Return to the create location when the action completes? (If wanderpoints are enabled this will never be used)
* \ReturnHomeSpeed - Speed to move at when going home
* \WanderingAIGroupID - Group of wander points to use when nothing else is going on
* \WanderSpeed - Speed to move at between the points
* \CanSeeStealth - Can the AI see stealth targets? 0 = No at all, 1 is within the ranges set in LE globals file, 2 means it can see stealth everywhere, think of stock AI.
* \ShutdownEngineOnArrival - Used for vehicles, turn on if you have issues with your vehicle rolling away from its move positions after it arrives
* \AttackCheckBlocked - Defines whether they should check if they can actually hit the player before shooting
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Goto_Enemy : public ScriptImpClass {
	enum aiState{IDLE,HUNTING_STAR,ATTACKING_TARGET,RETURNING_HOME,WANDERING_GROUP,ACTION_BADPATH};
	struct LastAction
	{
		int targetId;
		Vector3 location;
		float speed;
		float distance;
		bool attack;
		bool overrideLocation;
		LastAction()
		{
		}
		LastAction(int targetId,Vector3 location,float speed,float distance,bool attack,bool overrideLocation)
		{
			this->targetId = targetId;
			this->location = location;
			this->speed = speed;
			this->distance = distance;
			this->attack = attack;
			this->overrideLocation = overrideLocation;
		}
	};
	LastAction lastAction;
	aiState state;
	Vector3 homeLocation;
	bool huntStealth;
	int targetId;
	int lastSeenTime;
	float weaponRange;
	float weaponEffectiveRange;
	int huntingEnemyId;
	int removeIgnoreTime;
	int ignoreEnemyId;
	float huntSearchDistance;
	float huntArriveDistance;
	float attackArriveDistance;
	int stuckTime;
	int reverseTime;
	Vector3 lastPosition;
	bool moveBackward;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Attack_Move(GameObject *obj,GameObject *target,Vector3 location,float speed,float distance,bool attack,bool overrideLocation);
	GameObject *findClosestStar(GameObject *obj);
	void Return_Home(GameObject *obj);
	void Stuck_Check(GameObject *obj,Vector3 targetPos);
	void Cant_Get_To_target(GameObject *obj);
	bool GetRandomPosition(Vector3 *position);
	bool Choose_Target(GameObject *obj,GameObject *target);
};

/*!
* \brief This script makes the AI seek out the nearest enemy, players are ignored, it works for infantry and vehicles
* \HuntSearchDistance - Distance to scan for players to magically find and move to, -1 means infinite range
* \HuntSpeed - Speed at which the bot moves once its moving to a player
* \HuntArriveDistance - Distance at which to arrive from the player
* \RandomHuntArriveDistance - Random amount to arrive from the player
* \HuntStealth - Can the bot magically know where invisible players are
* \AttackSpeed - Speed to move at once an attack is begun
* \AttackDistance - Distance to arrive from a target when attacking, -1 uses the weapon effective distance
* \RandomAttackDistance - Random amount to add or subtract from the Attack distance
* \ReturnHome - Return to the create location when the action completes? (If wanderpoints are enabled this will never be used)
* \ReturnHomeSpeed - Speed to move at when going home
* \WanderingAIGroupID - Group of wander points to use when nothing else is going on
* \WanderSpeed - Speed to move at between the points
* \CanSeeStealth - Can the AI see stealth targets? 0 = No at all, 1 is within the ranges set in LE globals file, 2 means it can see stealth everywhere, think of stock AI.
* \ShutdownEngineOnArrival - Used for vehicles, turn on if you have issues with your vehicle rolling away from its move positions after it arrives
* \AttackCheckBlocked - Defines whether they should check if they can actually hit the player before shooting
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Goto_Enemy_Not_Star : public ScriptImpClass {
	enum aiState{IDLE,HUNTING_STAR,ATTACKING_TARGET,RETURNING_HOME,WANDERING_GROUP,ACTION_BADPATH};
	struct LastAction
	{
		int targetId;
		Vector3 location;
		float speed;
		float distance;
		bool attack;
		bool overrideLocation;
		LastAction()
		{
		}
		LastAction(int targetId,Vector3 location,float speed,float distance,bool attack,bool overrideLocation)
		{
			this->targetId = targetId;
			this->location = location;
			this->speed = speed;
			this->distance = distance;
			this->attack = attack;
			this->overrideLocation = overrideLocation;
		}
	};
	LastAction lastAction;
	aiState state;
	Vector3 homeLocation;
	bool huntStealth;
	int targetId;
	int lastSeenTime;
	float weaponRange;
	float weaponEffectiveRange;
	int huntingEnemyId;
	int removeIgnoreTime;
	int ignoreEnemyId;
	float huntSearchDistance;
	float huntArriveDistance;
	float attackArriveDistance;
	int stuckTime;
	int reverseTime;
	Vector3 lastPosition;
	bool moveBackward;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Attack_Move(GameObject *obj,GameObject *target,Vector3 location,float speed,float distance,bool attack,bool overrideLocation);
	GameObject *findClosestStar(GameObject *obj);
	void Return_Home(GameObject *obj);
	void Stuck_Check(GameObject *obj,Vector3 targetPos);
	void Cant_Get_To_target(GameObject *obj);
	bool GetRandomPosition(Vector3 *position);
	bool Choose_Target(GameObject *obj,GameObject *target);
};

/*!
* \brief Grants or removes a key on attach
* \Key - Key number to grant
* \Grant - 1 to grant, 0 to remove
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Grant_Key_On_Create : public ScriptImpClass
{
	AggressiveAttackSpotSystem::AggressiveAttackSpotNode *node;
	void Created(GameObject *obj);
};

/*!
* \brief Sends a custom on a custom
* \Custom - Custom to count
* \ID - ID to send to, 0 sends to self, -1 sends to sender
* \SendCustom - custom to send
* \Param - param to send
* \Delay - delay to add
* \ResetCustom - custom to reset the count
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Send_Custom : public ScriptImpClass {
	int recieveMessage;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Empty vehicles slowly die if not occupied by a player (a player must enter the vehicle once first)
* \Rate - how fast to damage the vehicle
* \Delay - how long must it be unoccupied before taking damage
* \DecreaseTick - how much should be subtracted each update from the delay when unoccupied
* \IncreaseTick - how much should be added when occupied
* \Damage - how much damage to do
* \Warhead - warhead to use
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Damage_Unoccupied_Vehicle : public ScriptImpClass {
	bool hasBeenOccupied;
	float rate;
	float damage;
	char warhead[128];
	float delay;
	float maxDelay;
	float decreaseTick;
	float increaseTick;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Kills all soldiers on a team when a custom is received
* \Custom - Custom to count
* \Team - team to wipe out
* \Damage - how much damage to apply
* \Warhead - warhead to use
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Damage_All_Soldiers_On_Team : public ScriptImpClass {
	int custom;
	int team;
	float damage;
	char warhead[128];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Makes a unit move between wander points, it'll strafe at the target while it has one, clone of JMG_Utility_AI_Guardian_Aircraft but with pathfind enabled and allows you to turn off the engine on arrival
* \WanderingAIGroupID - Group of points to wander between
* \WanderSpeed - Speed the unit moves at 
* \FireRange - Max range that it can use it's guns
* \CheckBlocked - Whether to check if the target spot is blocked before firing
* \AimAtFeet - Should the vehicle aim at the feet of infantry
* \TurnOffEngineOnArrival - Should the vehicle turn off its engine when it gets to its points (useful for wheeled vehicles that roll away)
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Guardian_Vehicle : public ScriptImpClass {
	bool aimAtFeet;
	Vector3 dpPosition;
	int EnemyID;
	int EnemyTimeOutTime;
	Vector3 LastPos;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Goto_Location(GameObject *obj);
	bool Get_A_Defense_Point(Vector3 *position);
};

/*!
* \brief Destroys the closest object with the 3d model to the position (works with powerups and other "dumb" objects)
* \Custom - Custom to trigger the script
* \Model - Model of the object to kill
* \Position - Position to search near
* \MaxDistance - Max distance to scan, 0 = infinite
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Destroy_Closest_Model_To_Position : public ScriptImpClass {
	int custom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Counts the deaths of any object with the reporter script attached, displays hud messages at a set interval, once the cap is reached a custom is sent
* \MaxDeaths - Once the death count reaches this a custom is sent
* \DeathReminder - A hud custom is displayed every time the death count hits a mod of this value
* \UrgentDeathReminder - Once the death count exceeds this the death messages show up at every death
* \StringID - ID of the string to replace for the death messages
* \ReminderMessage - Message to display on the hud as the player lives get low, use %d to display the remaining lives and %s to add a "s " or " " to the end of the noun.
* \Delim - Character to swap with a comma in the ReminderMessage
* \ReminderColor - Color of the warning HUD messages
* \ReminderMessageOrder - Specifies what order the kill arguments are handed out, 0 = deathCount remainingLives, 1 = remainingLives deathCount, 2 = remainingLives
* \DeathSingular - Specifies the word or letters to use when only one death has happened
* \DeathPlural - Specifies the word or letters to use when only many deaths have happened
* \RemainingSingular - Specifies the word or letters to use when only one life remains
* \RemainingPlural - Specifies the word or letters to use when only many lives remain
* \ID - Id to send the message to, if 0 it sends to itself
* \Custom - Custom to send
* \Param - Parameter to send with the message
* \Delay - Time delay to add to the sent message
* \ReminderCustom - custom to send every time a reminder message shows up, param is the remaining lives, no delay
* \AddDeathsWhenNoPlayers - Increases the death count (by one) at the rate specified here when no players are in game, if 0 no increase
* \NoPlayersAddDeathSaftyTime - This amount of time must pass before deaths will start to be added, if a player rejoins the time will reset
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Send_Custom_On_Deaths_Controller : public ScriptImpClass
{
	float addDeathsWhenNoPlayers;
	int noPlayersAddDeathSaftyTime;
	int currentNoPlayersAddDeathSaftyTime;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
public:
	static int deathCount;
	static int maxDeaths;
	static int deathReminder;
	static int urgentDeathReminder;
	static int stringId;
	static int myId;
	static Vector3 reminderColor;
	static char reminderMessage[220];
	static char deathSingular[220];
	static char deathPlural[220];
	static char remainingSingular[220];
	static char remainingPlural[220];
	static int reminderMessageOrder;
	static bool controllerPlaced;
};

/*!
* \brief Attach this to the objects you want the death tracking system applied to, requires JMG_Utility_Send_Custom_On_Deaths_Controller
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Send_Custom_On_Deaths_Reporter : public ScriptImpClass
{
	void Killed(GameObject *obj,GameObject *killer);
	char *formatReminderString(const char *format,...);
};

/*!
* \brief Tell objects with JMG_Utility_AI_Goto_Player to ignore this object
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Goto_Player_Ignore_Object : public ScriptImpClass {
	void Created(GameObject *obj);
};


/*!
* \brief Tell objects with JMG_Utility_AI_Goto_Enemy_Ignore_Object to ignore this object
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Goto_Enemy_Ignore_Object : public ScriptImpClass {
	void Created(GameObject *obj);
};


/*!
* \brief Tell objects with JMG_Utility_AI_Goto_Enemy_Not_Star_Ignore_Object to ignore this object
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Goto_Enemy_Not_Star_Ignore_Object : public ScriptImpClass {
	void Created(GameObject *obj);
};

/*!
* \brief Changes the model, team, and resets the action of an object on custom.
* \Custom - Custom to trigger the script
* \Model - Model to change to on custom
* \Team - Team to change to on custom
* \ResetActionPriority - Priority of the action to reset (usually 100)
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Set_Team_And_Model : public ScriptImpClass {
	int custom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Spawns an object if the last warhead that damaged the object matches the specified warhead
* \Warhead - Name of the warhead
* \Chance - Chance that object will be spawned (0 - 1.0)
* \Preset - Object to create
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Death_Warhead_Create_Object : public ScriptImpClass {
	unsigned int lastDamageWarhead;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Killed(GameObject *obj,GameObject *killer);
};

/*!
* \brief Changes the soldier's preset on enter if the original preset matches
* \EntererPreset - required preset of the soldier that enters
* \PlayerType - required player type of the enterer
* \Preset - name of the preset to change to
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Zone_Change_Character_On_Preset_Enter : public ScriptImpClass {
	int playerType;
	char entererPreset[256];
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
};

/*!
* \brief Syncs the health of an object with the ID of the object specified, object is destroyed if id object doesn't exist
* \ID - Id of the object to sync with
* \Rate - How often to sync the HP
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Sync_HP_With_Object : public ScriptImpClass {
	int id;
	float rate;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Sets the screen opacity and color of all people that join the game, also resets the screen fade for JMG_Utility_Swimming_Zone when exiting the water
* \Color - color to set the screen (0.0 - 1.0)
* \Opacity - opacity to set the screen (0.0 - 1.0)
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Set_Screen_Color_Fade_Controller : public ScriptImpClass {
	bool syncedScreen[128];
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
	static Vector3 controllerDefaultColor;
	static float controllerDefaultOpacity;
public:
	JMG_Utility_Set_Screen_Color_Fade_Controller()
	{
		controllerPlaced = true;
	}
	static bool controllerPlaced;
	static Vector3 color[128];
	static float opacity[128];
	SCRIPTS_API static void Update_Colors(Vector3 Color,float Opacity);
	SCRIPTS_API static void Update_Player_Colors(GameObject *player,Vector3 Color,float Opacity);
	SCRIPTS_API static void Reset_To_Default(GameObject *player);
	SCRIPTS_API static void Update_Player(GameObject *player,float transition);
	SCRIPTS_API static void Update_All_Players(float transition);
};

/*!
* \brief Switches the default screen color and opacity of the map on a custom and updates all players
* \Custom - custom to trigger on
* \Color - Color to set (0.0 - 1.0)
* \Opacity - Opacity to set (0.0 - 1.0)
* \Transition - how long to fade to the new settings
* \TriggerOnce- can this script trigger more than once
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Set_Screen_Color_Fade_On_Custom : public ScriptImpClass {
	int custom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Updates the screen of the attached player when the object is created
* \Transition - how long to fade to the new settings
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Set_Screen_Color_Fade_Reset_Player_Create : public ScriptImpClass {
	void Created(GameObject *obj);
};

/*!
* \brief Fades the screen back to the values set by JMG_Utility_Set_Screen_Color_Fade_Controller for a specific player
* \Transition - how long to fade to the new settings
* \PlayerType - team the player has to be on to trigger the script
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Set_Screen_Color_Fade_Reset_Player_Enter : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

/*!
* \brief A simplified script for mech walking animatons, use w3danimsound to provide synced walk sounds
* \ForwardAnimation - animation to play while going forward
* \BackwardAnimation - animation to play while going backward
* \IdleAnimation - animation to play while idle, leave this blank and the animtion will stop on the last frame used
* \TurnLeftAnimation - animation to play while turning left
* \TurnRightAnimation - animation to play while turning right
* \ReleaseDelay - amount of time in 10ths of a second before the animation can chance after a key is released (useful to keep the animaton from skipping)
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Simple_Mech : public ScriptImpClass {
	enum mvmtDir{IDLE,FORWARD,BACKWARD,LEFT,RIGHT};
	char idleAnimation[32];
	char forwardAnimation[32];
	char backwardAnimation[32];
	char turnLeftAnimation[32];
	char turnRightAnimation[32];
	int idleCooldown;
	int releaseDelay;
	bool hasIdleAnimation;
	mvmtDir currentDirection;
	mvmtDir lastMovementDirection;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Animation_Complete(GameObject *obj,const char *animation_name);
	void PlayAnimation(GameObject *obj,mvmtDir direction,float startFrame,float endFrame,bool looped);
};

/*!
* \brief Sends a custom message when a preset gets into range
* \Preset - Preset to detect
* \Range - Range to detect the preset
* \Rate - how often to scan for the preset
* \ID - Id to send the message to, if 0 it sends to itself, if -1 send to the object that triggered it
* \Custom - Custom to send
* \Param - Parameter to send with the message
* \Delay - Time delay to add to the sent message
* \TriggerOnce - Can the script only trigger once
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Send_Custom_When_Preset_In_Range : public ScriptImpClass {
	float range;
	char preset[128];
	float rate;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};
/*!
* \brief Basic turret attach script, turrets match team of vehicle attached to, turrets are destroyed by destroy event
* \Preset - Preset to create
* \Bone - Bone to create the preset at
* \MaxDistance - Once the object is created it can be displaced if another object is in its position, this is how far it can be moved
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Death_Create_Object_At_Bone_Position : public ScriptImpClass
{
	void Killed(GameObject *obj,GameObject *killer);
};

/*!
* \brief Enables loiter animations
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Enable_Loiter : public ScriptImpClass {
	void Created(GameObject *obj);
};

/*!
* \brief Switches the current weapon to a different weapon on custom
* \Custom - custom to trigger on
* \Weapon - name of the weapon to switch to
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Switch_Weapon : public ScriptImpClass {
	int custom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Displays HUD messages once at each hour, 30 min, 20 min, 10 min, 5 4 3 2 1 min, 30 sec 10 9 8 7 6 5 4 3 2 1 seconds. Custom messages can be 
* \ sent by attaching JMG_Utility_HUD_Count_Down_Messages
* \TimeInSeconds - Time in seconds that the countdown goes for
* \StringID - ID of the string to replace and use for the HUD message
* \Color - RGB Color to use for the HUD message
* \WarningMessage - Message to display on screen as the countdown goes down, format order goes Time then specifier
* \Delim - Character to swap with a comma in the WarningMessage
* \HourSingular - This is the part of the message that displays hours singular
* \HourPlural - This is the part of the message that displays hours plural
* \MinuteSingular - This is the part of the message that displays minute singular
* \MinutePlural - This is the part of the message that displays minutes plural
* \SecondSingular - This is the part of the message that displays second singular
* \SecondPlural - This is the part of the message that displays seconds plural
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_HUD_Count_Down : public ScriptImpClass {
public:
	struct SendCustomOnSecondNode
	{
		int triggerSecond;
		int id;
		int custom;
		int param;
		float delay;
		struct SendCustomOnSecondNode *next;
		SendCustomOnSecondNode(int triggerSecond,int id,int custom,int param,float delay)
		{
			this->triggerSecond = triggerSecond;
			this->id = id;
			this->custom = custom;
			this->param = param;
			this->delay = delay;
			this->next = NULL;
		}
	};
	static SendCustomOnSecondNode *sendCustomOnSecondController;
	static void AddSecondNode(int triggerSecond,int id,int custom,int param,float delay)
	{
		SendCustomOnSecondNode *current = sendCustomOnSecondController;
		if (!sendCustomOnSecondController)
			sendCustomOnSecondController = new SendCustomOnSecondNode(triggerSecond,id,custom,param,delay);
		while (current)
		{
			if (current->triggerSecond == triggerSecond)
			{
				Console_Input("msg ERROR: A custom for this trigger second already exists!");
				return;
			}
			if (!current->next)
			{
				current->next = new SendCustomOnSecondNode(triggerSecond,id,custom,param,delay);
				return;
			}
			current = current->next;
		}
	}
	static bool controllerPlaced;
private:
	int stringId;
	Vector3 color;
	char warningMessage[220];
	char hourSingular[220];
	char hourPlural[220];
	char minuteSingular[220];
	char minutePlural[220];
	char secondSingular[220];
	char secondPlural[220];
	int seconds;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
	char *formatReminderString(const char *format,...);
	
	SendCustomOnSecondNode *FindSecondNode(int second)
	{
		if (!sendCustomOnSecondController)
			return NULL;
		SendCustomOnSecondNode *current = sendCustomOnSecondController;
		while (current)
		{
			if (current->triggerSecond == second)
				return current;
			current = current->next;
		}
		return NULL;
	}
	void CleanupSecondNodes()
	{
		SendCustomOnSecondNode *temp = sendCustomOnSecondController,*die;
		while (temp)
		{
			die = temp;
			temp = temp->next;
			delete die;
		}
	}
};
/*!
* \brief Sends a custom when TimeInSeconds (on JMG_Utility_HUD_Count_Down) matches the TriggerTime 
* \TriggerTime - Time in which to send the custom
* \ID - ID to send the custom to, if 0 it sends it to the object this script is attached too
* \Custom - Custom to send
* \Param - Param to send
* \Delay - Delay to wait before sending the message
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_HUD_Count_Down_Send_Custom : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Fades the screen of a specific player
* \Color - color to fade to (if Red (X) is set to less than 0 color won't be set))
* \Opacity - opacity to fade to (if opacity is less than 0 it won't be set)
* \Transition - how long to fade to the new settings
* \PlayerType - team the player has to be on to trigger the script
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Zone_Screen_Fade : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enter);
};

/*!
* \brief Sets whether AI with Enemy Seen can see this object
* \Custom - custom to trigger on
* \Visible - Does enemy seen trigger on this object
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Triggers_Enemy_Seen : public ScriptImpClass {
	int custom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Fades the screen red when damage, mixes with colors provided by the swimming script and JMG_Utility_Set_Screen_Color_Fade_Controller (ported from ECW)
* \FadeColor - Color to fade to when damaged
* \FadeOpacity - Opacity to fade to when damaged
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Screen_Fade_Red_On_Damage : public ScriptImpClass {
	int playerId;
	Vector3 fadeColor;
	float fadeOpacity;
	float injuryRatio;
	float lastInjuryRatio;
	float screenOpacity;
	float lastScreenOpacity;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void ScreenFade(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Enables spawners when player count meets specified values, disables when outside of values
* \StartID - ID to start at
* \EndID - ID to stop at
* \PlayerCount[eq|<|>] - Set the player count for the conditions to enable the spawners, enter -1 ignores the condition (equal, less than, greater than)
* \PlayerCount[neq|between(|)] - Set the player count for the conditions to enable the spawners, enter -1 ignores the condition (not equal, between)
* \EnableOrDisableCustom - Custom pauses the script or unpauses it, param enables/disables (spawner current state is left enabled/disabled)
* \StartsEnabled - Should this script be running on map load
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Player_Count_Enable_Spawners : public ScriptImpClass
{
	bool enabled;
	int enableCustom;
	Vector3 playerCount;
	Vector3 playerCount2;
	bool conditionMatching;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void EnableSpawners(bool enable);
};

/*!
* \brief Used to designate targets that the engineer AI should ignore
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Engineer_Ignore_Target : public ScriptImpClass {
	void Created(GameObject *obj);
};
/*!
* \brief Grants specified weapon to all players in range
* \WeaponName - Name of the weapon to grant to the player
* \Location - Location to grant the weapon at
* \Range - Range from the location to grant the weapon
* \EnableOrDisableCustom - Custom to enable the scanning, parameter determines if its enabled or disabled 1 = enabled, 0 = disabled
* \StartsEnabled - Should to script start running 1 = true, 0 = false
* \Rate - Speed at which to scan, 0.0 will disable the repeated scanning and then only enable custom will trigger a scan
* \SelectWeapon - Should the player pull out the weapon when its granted
* \WeaponAmmo - Amount of ammo to grant with initial weapon grant
* \RefillRate - How fast the gun should be refilled while in the zone
* \RefillAmount - How much ammo to refill
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Give_Weapon_In_Range : public ScriptImpClass
{
	float rate;
	float refillRate;
	float range;
	Vector3 location;
	bool enabled;
	int enableCustom;
	char weaponName[256];
	bool selectWeapon;
	int weaponAmmo;
	int refillAmount;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void GrantWeapon();
};

/*!
* \brief Needed for AI using JMG_Utility_AI_Follow_Player_On_Poke, this script controls the limits of how many bots each player can have following them
* \MaxFollowers - Any value greater than 0 makes it so a player can only have that many followers at a time
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Follow_Player_On_Poke_Controller : public ScriptImpClass {
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
public:
	static bool controllerPlaced;
	static int maxFollowers;
	static int followingPlayer[128];
	JMG_Utility_AI_Follow_Player_On_Poke_Controller()
	{
		controllerPlaced = true;
	}
};

/*!
* \brief Makes an AI follow the player that pokes it, if the player dies it goes to its original position
* \FollowDistance - How far should the AI follow the player
* \FollowNearSpeed - How fast should the AI follow the player when it is near by
* \FollowFarDistance - How far must the AI be away in order to run faster to the player
* \FollowFarSpeed - How fast should the AI follow the player when it is far away
* \FollowVeryFarDistance - How far must the AI be away in order to run faster to the player
* \FollowVeryFarSpeed - How fast should the AI follow the player when it is far away
* \FallBackWhenOutOfArmor - Should the AI return to its spawn location when it runs out of armor
* \RunHomeSpeed - How fast should the AI run home
* \HealWhileAtHomeLocation - The amount of health the AI regenerates while at its home location
* \HUDMessagesColor - Color of the HUD messages that are displayed to the player that the AI is following (0.0 - 1.0)
* \MessagePokeFollowerID - Message to display when a player pokes a follower, it displays the current number of followers out of total IE: %d/%d
* \MessageFollowingYouID - Message to display if the player pokes the bot when it is already following them
* \MessageFollowingPlayerID - Message to display to the player if they poke a bot that is following someone else
* \MessageMaxFollowersID - Message to display if the player attempts to have more followers than allowed (message is formatted: You can only have %d creature%sfollowing you.)
* \MessageHealingRequiredID - Message to display when the follower is hurt and has to heal before following again, only available when FallBackWhenOutOfArmor is on
* \MessageFollowerLostID - Message to display when the player the follower is following wanders out of the pathfind zone or if they follow the player for to long
* \MessageFollowerInjuredID - Message to display when the follower runs out of armor, FallBackWhenOutOfArmor must be on
* \MessageFollowerKilledID - Message to display when the follower dies following someone
* \SendCustomID - ID of the object customs should be sent to
* \PokedCustom - Custom to send when the follower is poked and starts following
* \LostCustom - Message to send if the follower gets lost or if they follow the player for to long
* \InjuredCustom - Message to send when the follower gets injured, requires FallBackWhenOutOfArmor be on
* \KilledCustom - Message to send if the follower is killed 
* \HealedCustom - Message to send when the follower gets to full health, requires FallBackWhenOutOfArmor be on
* \MaxFollowTime - Amount of time the AI can follow the player before getting bored and running home (in 10ths of a second)
* \MaxIdleTime - Max amount of time an AI can stand still while "following" a player before running home (in 10ths of a second)
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Follow_Player_On_Poke : public ScriptImpClass {
	GameObject *lastFollow;
	GameObject *lastTarget;
	Vector3 lastPos;
	int maxFollowTime;
	int maxIdleTime;
	int currentFollowTime;
	int currentIdleTime;
	bool detached;
	float followDistance;
	float followNearSpeed;
	float followFarSpeed;
	float followFarDistance;
	float followVeryFarSpeed;
	float followVeryFarDistance;
	float runHomeSpeed;
	int actionUpdate;
	float weaponRange;
	float targetDistance;
	int enemyId;
	int pokerPlayerId;
	int playerObjectId;
	Vector3 homeLocation;
	Vector3 lastLocation;
	int enemySeen;
	bool fallbackWithoutArmor;
	float healWhileAtHomeLocation;
	int sendCustomId;
	int pokedCustom;
	int lostCustom;
	int killedCustom;
	int healedCustom;
	int injuredCustom;
	Vector3 messageColor;
	int messagePokeFollowerId;
	char messagePokeFollower[220];
	int messageFollowingYouId;
	int messageFollowingPlayerId;
	int messageMaxFollowersId;
	char messageMaxFollowers[220];
	int messageHealingRequiredId;
	int messageFollowerInjuredId;
	int messageFollowerKilledId;
	int messageFollowerLostId;
	bool hurt;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Poked(GameObject *obj, GameObject *poker);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason);
	void Destroyed(GameObject *obj);
	void Detach(GameObject *obj);
	void AttackTarget(GameObject *obj,GameObject *follow,GameObject *target,Vector3 location,float speed,float distance);
	void GiveUpOnPlayer(GameObject *obj,bool pokeable);
	void ForgetEnemy(GameObject *obj);
	void SendCustom(GameObject *obj,int custom);
	char *formatMaxFollowingString(const char *format,...);
	void ShowPlayerMessage(GameObject *player,int stringId,Vector3 color);
public:
	JMG_Utility_AI_Follow_Player_On_Poke()
	{
		detached = false;
		pokerPlayerId = 0;
	}
};

/*!
* \brief Damages the attached object and teleports it after a timed delay
* \Delay - Amount of time in seconds to wait
* \TeleportToObjectID - ID of the object to teleport to
* \Location - Location to teleport to if no object ID was supplied 
* \DamageAmount - Amount of damage to apply
* \DamageWarhead - Warhead to use to damage the object
* \DamageSelf - Should the applied damage count for the object the script is attached to
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Timer_Damage_And_Teleport : public ScriptImpClass {
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Sends a custom if the object entering the zone has a weapon
* \PlayerType - Player type the zone triggers for
* \ScriptName - Name of the script needed to trigger the zone
* \ID - ID to send the custom to, 0 sends to self, -1 sends to enter
* \Custom - Custom message to send
* \Param - Param to send
* \Delay - Delay to add before sending custom
* \TriggerOnce - Allows the script only to trigger the first time the zone is entered
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Zone_Send_Custom_If_Has_Script : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

/*!
* \brief Fades the screen and applies damage as the player gets further from the spot on the map, kills player if they wander outside a max range, uses JMG_Utility_Set_Screen_Color_Fade_Controller for default screen colors
* \Warn_Distance - Distance at which the warning message appears on the HUD for infantry
* \Damage_Distance - Distance at which the infantry starts taking damage
* \Vehicle_Warn_Distance - Distance at which the warning message appears on the HUD for vehicle
* \Vehicle_Damage_Distance - Distance at which the vehicle starts taking damage
* \Aircraft_Warn_Distance - Distance at which the warning message appears on the HUD for aircraft
* \Aircraft_Damage_Distance - Distance at which the aircraft starts taking damage
* \Max_Survive_Distance - Distance at which anything the player is in (or the player their self) dies
* \Warning_StringID - String ID to use for the HUD warning message
* \Warning_Color - Color to display the warning HUD message
* \Screen_Fade_Color - Color to fade the screen to
* \DamageWarhead - Warhead to use to damage the units
* \InstantKillWarhead - Warhead to use to kill targets instantly 
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Damage_When_Outside_Of_Range : public ScriptImpClass {
	float warnDistance;
	float damageDistance;
	float vehicleWarnDistance;
	float vehicleDamageDistance;
	float aircraftWarnDistance;
	float aircraftDamageDistance;
	float maxSurviveDistance;
	Vector3 errorMessageColor;
	Vector3 screenFadeColor;
	int warnTime[128];
	bool screenEffectOn[128];
	int leavingFieldStringId;
	char damageWarhead[128];
	char instantKillWarhead[128];
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Sends a custom message when no more objects of the preset name exist on the map, then removes itself
* \PresetName - preset name to search for 
* \StartsEnabled - If 0 it doesn't start scanning until enabled, otherwise starts scanning on attach
* \EnableOrDisableCustom - Custom used to enable the script, a param of 0 disables the script again, anything besides 0 enables
* \ID - Id of the object to send the custom to, 0 sends to itself
* \Message - Custom to send
* \Param - parameter to send with the custom
* \Delay - Time amount to wait before sending the custom
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Send_Custom_When_No_More_Presets_Of_Type_Exist : public ScriptImpClass {
	char presetName[128];
	int enableCustom;
	bool enabled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Sends a custom message when a player goes beyond a certain range of this object
* \Distance - how far away can the plaeyrs go
* \ID - Id of the object to send the custom to, 0 sends to itself
* \Message - Custom to send
* \Param - parameter to send with the custom
* \Delay - Time amount to wait before sending the custom
* \TriggerOnce - If true the script can only fire once
* \SleepTime - Value in 10ths of a second the script must wait before sending the custom again
* \MaxRange - Players beyond this won't be detected, if 0 the param is ignored
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Send_Custom_When_Player_Outside_Of_Range : public ScriptImpClass {
	int sleepTime;
	int sleeping;
	bool triggerOnce;
	bool enabled;
	float distance;
	float delay;
	float maxRange;
	int id;
	int param;
	int custom;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Sets the player type to -2 when no armor, sets player type back to the original player type when armor is full
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Set_Team_To_Neutral_When_No_Armor : public ScriptImpClass {
	bool hurt;
	int originalPlayerType;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Sends a custom message when no more objects on the specified team exist 
* \Team - team that should be scanned for
* \StartsEnabled - If 0 it doesn't start scanning until enabled, otherwise starts scanning on attach
* \EnableOrDisableCustom - Custom used to enable the script, a param of 0 disables the script again, anything besides 0 enables
* \ID - Id of the object to send the custom to, 0 sends to itself
* \Message - Custom to send
* \Param - parameter to send with the custom
* \Delay - Time amount to wait before sending the custom
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist : public ScriptImpClass {
	int team;
	int enableCustom;
	bool enabled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Creates a 3D sound at the specified bone on custom
* \Sound - Sound preset to play
* \Bone - Bone to play the sound from
* \Custom - Custom message to trigger the sound
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Create_Sound_At_Object_Bone : public ScriptImpClass {
	char sound[128];
	char bone[32];
	int custom;
	bool enabled;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Sends a custom when a custom has been received x times with addition of a player count multiplier, script destroys itself after it triggers once
* \Custom - Custom to count
* \BaseCount - how many customs needed
* \PlayerCount - added to the base count, multiplied by the number of players in game
* \ID - ID to send to, 0 sends to self, -1 sends to sender
* \SendCustom - custom to send
* \Param - param to send
* \Delay - delay to add
* \ResetCustom - custom to reset the count
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Send_Custom_On_Player_Count : public ScriptImpClass {
	int custom;
	int resetCustom;
	int baseCount;
	int playerCount;
	int runningCount;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Sends a custom message when killed by a player
* \ID - Id of the object to send the custom to, 0 sends to itself, -1 to killer
* \Message - Custom to send
* \Param - parameter to send with the custom
* \Delay - Time amount to wait before sending the custom
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Killed_By_Player_Send_Custom : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

/*!
* \brief Makes this unit ignored by all the Guardian AI scripts
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Guardian_Ignored : public ScriptImpClass {
	void Created(GameObject *obj);
};

/*!
* \brief Makes this unit ignored by all JMG_Utility_AI_Vehicle
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Vehicle_Ignored : public ScriptImpClass {
	void Created(GameObject *obj);
};

/*!
* \brief Sends a custom message when killed by a specific PresetID
* \PresetID - Preset ID to detect
* \ID - Id of the object to send the custom to, 0 sends to itself, -1 to killer
* \Message - Custom to send
* \Param - parameter to send with the custom
* \Delay - Time amount to wait before sending the custom
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Killed_By_PresetID_Send_Custom : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

/*!
* \brief Makes an AI able to shoot directions the gun isn't aiming
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Created_Override_AI_Soldier_Muzzle_Direction : public ScriptImpClass {
	void Created(GameObject *obj);
};

/*!
* \brief Sends a custom when a custom has been received x times with addition of a player count multiplier, script destroys itself after it triggers once, also displays the current count on the HUD
* \Custom - Custom to count
* \BaseCount - how many customs needed
* \PlayerCount - added to the base count, multiplied by the number of players in game
* \ID - ID to send to, 0 sends to self, -1 sends to sender
* \SendCustom - custom to send
* \Param - param to send
* \Delay - delay to add
* \ResetCustom - custom to reset the count
* \HudStringId - stringId for the string to be parsed and displayed on the HUD, formatting goes %d/%d (current count out of calculated total)
* \HudStringColor - RGB value for the HUD message (0.0-1.0)
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Send_Custom_On_Player_Count_HUD : public ScriptImpClass {
	Vector3 hudStringColor;
	char hudMessage[220];
	int hudStringId;
	int custom;
	int resetCustom;
	int baseCount;
	int playerCount;
	int runningCount;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	char *formatHUDMessage(const char *format,...);
};

/*!
* \brief Sends a custom if the object entering the zone has a weapon, one custom is sent for each ammo count the weapon has
* \PlayerType - Player type the zone triggers for
* \WeaponName - Name of the weapon needed to trigger the zone
* \ID - ID to send the custom to, 0 sends to self, -1 sends to enter
* \Custom - Custom message to send
* \Param - Param to send
* \Delay - Delay to add before sending custom
* \RemoveWeapon - Should the weapon be removed when entering the zone
* \TriggerOnce - Allows the script only to trigger the first time the zone is entered
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Zone_Send_Custom_If_Has_Weapon_Ammo : public ScriptImpClass {
	void Entered(GameObject *obj,GameObject *enterer);
};

/*!
* \brief Removes a weapon from all the players in game when a custom is received
* \Custom - Custom to count
* \WeaponName - name of the weapon preset to remove
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Remove_Weapon_From_All_Players : public ScriptImpClass {
	int custom;
	char weaponName[128];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Destroys all the objects on the map with matching preset name
* \Custom - Custom to trigger the script
* \PresetName - Name of the preset to hunt down and destroy
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Destroy_All_Presets_By_Name : public ScriptImpClass {
	int custom;
	char presetName[128];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Sends a custom when an object has been damaged over a specified amount, also can have a delay how often the custom can be sent again
* \DamageThreshold - Amount of damage that must be done to trigger the script
* \LockoutTime - Amount of time that must go by before the script can trigger again
* \ID - ID to send the custom to, 0 sends to self, -1 sends to enter
* \Custom - Custom message to send
* \Param - Param to send
* \Delay - Delay to add before sending custom
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Send_Custom_When_Damage_Exceedes_Threshold : public ScriptImpClass {
	int id;
	int custom;
	int param;
	float delay;
	float damageThreshold;
	float lockoutTime;
	float timeRemaining;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
};

/*!
* \brief Sends a chat message to all players on custom
* \Custom - Custom message to trigger the script on
* \StringId - String ID to send through the chat console to all players
* \ColorRGB - Color of the HUD message to display 0-255
* \Repeatable - Can the script trigger more than once
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Send_Translated_String_To_All_Players : public ScriptImpClass {
	int custom;
	Vector3 color;
	int stringId;
	bool repeatable;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Creates a powerup at the objects origin when it is destroyed if it has that weapon
* \WeaponPreset - Weapon that is needed to do the drop
* \PowerupName - Powerup to create
* \HeightAdjust - Height to add to the object's origin to create the powerup at
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Destroyed_Drop_Powerup : public ScriptImpClass
{
	void Destroyed(GameObject *obj);
};

/*!
* \brief Attaches a script to the soldier that picked up the powerup
* \Script - Script to attach
* \Params - Params to use
* \Delim - char to use in place of ,
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Pickup_Attach_Script : public ScriptImpClass
{
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Used to update the status of an objective
* \Custom - Custom to trigger on
* \ObjectiveID - ID of the new objective to update
* \NewObjectiveStringID - Strings.tbl id for the objective message
* \NewObjectiveMarkerObjectID - ID of the object where the marker should be displayed (-1 sets the sender as the marker)
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Objective_System_Objective_Status_Update_Custom : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Used to make an objective show up as failed
* \Custom - Custom to trigger this script
* \ObjectiveID - ID of the new objective to remove
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Objective_System_Objective_Failed_Custom : public ScriptImpClass {
	int custom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Places all players on the same team at the end of the round
* \Team - Team to force all players to
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Force_Player_Team_At_Gameover : public ScriptImpClass {
	int team;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Makes a unit move between wander points, it'll strafe at the target while it has one, clone of JMG_Utility_AI_Guardian_Aircraft but with pathfind enabled
* \WanderingAIGroupID - Group of points to wander between
* \WanderSpeed - Speed the unit moves at 
* \FireRange - Max range that it can use it's guns
* \CheckBlocked - Whether to check if the target spot is blocked before firing
* \ArriveDistance - Distance to get close to
* \FlightHeight - How high should the aircraft fly (use 0 if not an aircraft)
* \TurnOffEngineOnArrival - Should the vehicle kill its engine when it arrives
* \UseSecondaryAttack - Should the unit attack with its secondary fire instead
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Guardian_Generic : public ScriptImpClass {
	float flightHeight;
	float arriveDistance;
	float arriveDistanceSq;
	Vector3 dpPosition;
	int EnemyID;
	int EnemyTimeOutTime;
	Vector3 LastPos;
	bool primaryFire;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Goto_Location(GameObject *obj);
	bool Get_A_Defense_Point(Vector3 *position);
};

/*!
* \brief Creates an object in front of this object
* \Custom - Custom to trigger the script
* \PresetName - Name of the preset to create
* \Distance - Distance in front of the object to place the new object
* \Height - Height to add to the new object
* \Rotation - Rotation to add to the new object (base rotation is the same as the old object)
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Create_Object_In_Front_Of_Self : public ScriptImpClass {
	int custom;
	char presetName[128];
	float distance;
	float height;
	float rotation;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Makes this unit ignored by JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist
* \Ignore - Does nothing, just here so you can tell which script this is beings the name is cut off
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Send_Custom_When_No_More_Units_On_Team_Exist_Ignore : public ScriptImpClass {
	void Created(GameObject *obj);
};

/*!
* \brief Damages all vehicles on a team when a custom is received
* \Custom - Custom to count
* \Team - team to wipe out
* \Damage - how much damage to apply
* \Warhead - warhead to use
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Damage_All_Vehicles_On_Team : public ScriptImpClass {
	int custom;
	int team;
	float damage;
	char warhead[128];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Kills a unit if it doesn't manage to move more than the specified distance in the specified time
* \Time - amount of time the unit has to move
* \Distance - the distance the unit has to move
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Kill_Unit_If_Not_Moving_Enough : public ScriptImpClass {
	int time;
	int resetTime;
	float distance;
	Vector3 lastPos;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};


/*!
* \brief Reads text from a file and displays it to the screen, 
* \ the text in a file is broken every 150 characters (tries to end the current word) and then delayed by 6 seconds
* \ Note: This will only work once because of how it works
* \Custom - Custom to trigger this script on
* \StringID - ID of the string from strings.tbl to parse and play out
* \MessageColor - Color to display the chat message in
* \LineDelayTime - Delay between each message displayed
* \UISound - 2D sound to play when custom received
* \Repeatable - Allows the script to trigger more than once
* \Special Notes:
-String max length is 16384 characters long
-Line break/delay - Use this character ''(there is a character in there, you'll have to copy it out) to end the line, you can also use a bunch of them to add a delay to the message.
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Display_Briefing_Message_StringID : public ScriptImpClass {
	struct BriefingTextNode
	{
		char Text[256];
		float Delay;
		BriefingTextNode *next;
		BriefingTextNode(const char *text)
		{
			Delay = 0.0f;
			sprintf(Text,"%s",text); 
			next = NULL;
		}
		BriefingTextNode(const char *text,float delay)
		{
			Delay = delay;
			sprintf(Text,"%s",text); 
			next = NULL;
		}
		BriefingTextNode()
		{
			next = NULL;
		}
	};
	BriefingTextNode *BriefingText;
	BriefingTextNode *CurrentNode;
	bool triggered;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Destroyed(GameObject *obj);
	void AddNewTextNode();
	void RemoveTextNodes();
};

/*!
* \brief Same as JMG_Utility_Destroy_Objects_In_ID_Range_On_Destroy but tiggers on death instead of destroy
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Destroy_Objects_In_ID_Range_On_Killed : public ScriptImpClass
{
	void Killed(GameObject *obj,GameObject *killer);
};

/*!
* \brief Basically exactly the same as JFW_Timer_Custom except an id of 0 sends to itself
* \Time - delay to send the custom
* \ID - Id of the object to send the custom to, 0 sends to itself
* \Message - Custom to send
* \Param - parameter to send with the custom
* \Repeat - 1 to make it trigger again
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Timer_Custom : public ScriptImpClass {
	int id;
	int message;
	int param;
	bool repeat;
	float time;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Changes the model of any soldier that enters the zone
* \NewModel - W3D Model to change it to
* \PlayerType - required player type of the enterer
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Zone_Change_Character_Model : public ScriptImpClass {
	int playerType;
	char newModel[16];
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
};

/*!
* \brief Changes the model of any soldier that enters the zone
* \Time - delay to send the custom
* \NewModel - W3D Model to change it to
* \PlayerType - required player type of the enterer
* \Repeat - 1 to make it trigger again
* \StartsEnabled - Should the script be running from the get go?
* \EnableCustom - custom that should enable/disable the script, non 0 param = enable
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Timer_Change_Character_Models_On_Team : public ScriptImpClass {
	bool enabled;
	int enableCustom;
	bool repeat;
	float time;
	int playerType;
	char newModel[16];
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Changes the model of any soldier that enters the zone
* \Time - delay to send the custom
* \NewModel - W3D Model to change it to
* \PlayerType - required player type of the enterer
* \MaxHP - Min Hitpoints required to match to trigger for this character
* \MinHP - Max Hitpoints required to match to trigger for this character
* \Repeat - 1 to make it trigger again
* \StartsEnabled - Should the script be running from the get go?
* \EnableCustom - custom that should enable/disable the script, non 0 param = enable
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Timer_Change_Character_Models_On_HP : public ScriptImpClass {
	bool enabled;
	int enableCustom;
	bool repeat;
	float time;
	int playerType;
	float maxHp;
	float minHp;
	char newModel[16];
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Sends a custom when an object is destroyed
* \ID - Id of the object to send the custom to, 0 sends to itself
* \Message - Custom to send
* \Param - parameter to send with the custom
* \Delay - Delay to add to the send
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Destroy_Send_Custom : public ScriptImpClass {
  void Destroyed(GameObject *obj);
};

/*!
* \brief Sets the Vehicle's collision to soldier ghost when all players exit (excluding on build) and then sets it back to its original when a player gets in
* \ Useful on maps that you desire to prevent players from blocking the bots coming out of tunnels.
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Set_Vehicle_Collisions_Ghost_When_Unoccupied : public ScriptImpClass {
	bool attemptingCollidable;
	bool ghost;
	Collision_Group_Type myCollisionGroup;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Attaches a script to all players in game (can only attach the designated distinct script name once)
* \Script - Script to attach
* \Params - Params to use for that script, use a delim instead of a ,
* \Delim - Character to parse out of the params and replace with a ,
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Attach_Script_To_All_Players : public ScriptImpClass {
	char scriptName[256];
	char params[256];
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Sends a custom when an object is destroyed
* \ID - Id of the object to send the custom to, 0 sends to itself, -1 sends it to the person who picked it up
* \Message - Custom to send
* \Param - parameter to send with the custom
* \Delay - Delay to add to the send
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Send_Custom_On_Powerup_Pickup : public ScriptImpClass {
  void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Sets the objects bullet count when it is damaged or receives a custom
* \WeaponName - Name of the weapon to change, if "null" it uses the current weapon on script create
* \Custom - custom message to trigger on
* \TriggerOnDamage - Can this script trigger on damage
* \SetBackpackBullets - bullet count to put in the backpack when damaged or custom received
* \StartWithNoBullets - If one the object gets set to have no bullets when the script is attached
* \Repeat - Can it trigger more than once
* \FullClip - Does the unit automatically have a full clip when the script is triggered
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Set_Bullets_On_Custom_Or_Damage : public ScriptImpClass {
	char setWeapon[128];
	bool repeat;
	int custom;
	int setBackpackBullets;
	bool triggerOnDamage;
	bool fullClip;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void UpdateBullets(GameObject *obj);
};

/*!
* \brief Damages all units of a preset type when a custom is received
* \Custom - Custom to count
* \PresetName - presets to damage
* \Damage - how much damage to apply
* \Warhead - warhead to use
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Damage_All_Presets : public ScriptImpClass {
	int custom;
	char preset[128];
	float damage;
	char warhead[128];
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Spawns an object if the weapon that killed the object matches the specified weapon preset (warhead script is a better choice, less exploitable)
* \WeaponPreset - Name of the weapon
* \Chance - Chance that object will be spawned (0 - 1.0)
* \Preset - Object to create
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Death_Weapon_Create_Object : public ScriptImpClass {
	void Killed(GameObject *obj,GameObject *killer);
};

/*!
* \brief Sends a custom if the object's speed reaches or exceeds a certain amount
* \Speed - speed to watch for
* \Rate - how long to wait between each speed check
* \ID - Id of the object to send the custom to, 0 sends to itself
* \Message - Custom to send
* \Param - parameter to send with the custom
* \Repeat - 1 to make it trigger again after sending a custom
* \StartsEnabled - Non-zero makes the script run on create
* \EnableCustom - Custom to enable or disable the script, send a non-zero param to enable
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Send_Custom_When_Speed_Exceeds_Amount : public ScriptImpClass {
	float speed;
	int id;
	int message;
	int param;
	bool repeat;
	float rate;
	bool enabled;
	int enableCustom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Sends a custom if the object's speed is below a certain amount
* \Speed - speed to watch for
* \Rate - how long to wait between each speed check
* \ID - Id of the object to send the custom to, 0 sends to itself
* \Message - Custom to send
* \Param - parameter to send with the custom
* \Repeat - 1 to make it trigger again after sending a custom
* \StartsEnabled - Non-zero makes the script run on create
* \EnableCustom - Custom to enable or disable the script, send a non-zero param to enable
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Send_Custom_When_Speed_Below_Amount : public ScriptImpClass {
	float speed;
	int id;
	int message;
	int param;
	bool repeat;
	float rate;
	bool enabled;
	int enableCustom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Sends a custom if the object's speed is below a certain amount when going a direction
* \MinVelocity[Forward|Backward|Left] - Speed that must be exceeded when going these directions
* \MinVelocity[Right|Up|Down] - Speed that must be exceeded when going these directions
* \OnlyTriggerOn - Use this to make it so the script can only trigger if its moving faster horizontal than vertical or vice versia (0 doesn't matter, 1 for horizontal, 2 for vertical)
* \OnlyTriggerOnMin[Horizontal|Vertical] - Min Speeds required for Only Trigger On to be able to work
* \OnlyTriggerOnMax[Horizontal|Vertical] - Max Speeds required for Only Trigger On to be able to work
* \Rate - how long to wait between each speed check
* \ID - Id of the object to send the custom to, 0 sends to itself
* \Message - Custom to send
* \Param - parameter to send with the custom, it param is -1 it will send a differnt param for each direction (1 = forward, 2 = backward, 3 = left, 4 = right, 5 = up, 6 = down)
* \Repeat - 1 to make it trigger again after sending a custom
* \StartsEnabled - Non-zero makes the script run on create
* \EnableCustom - Custom to enable or disable the script, send a non-zero param to enable
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Send_Custom_When_Velocity_Exceeds_Amount : public ScriptImpClass {
	enum SpecialCondition{DOES_NOT_MATTER,HORIZANTAL_IS_GREATER,VERTICAL_IS_GREATER,DOES_NOT_MEET_CONDITION};
	Vector3 velocityFBL;
	Vector3 velocityRUD;
	int id;
	int message;
	int param;
	bool repeat;
	float rate;
	bool enabled;
	int enableCustom;
	SpecialCondition onlyTriggerOn;
	Vector3 onlyTriggerOnMinHV;
	Vector3 onlyTriggerOnMaxHV;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
	void Timer_Expired(GameObject *obj,int number);
	void SendCustom(GameObject *obj,int paramOverride);
	Vector3 SquareVectorSpecial(Vector3 in);
	bool OnlyTriggerOnTest(float horizontalSpeed,float verticalSpeed);
};

/*!
* \brief Changes the character of the sender object (only works on infantry)
* \Custom - custom to trigger on (Hint: Picking up a powerup is 1000000025)
* \Preset - preset to switch to
* \Cost - price it costs to change character
* \SuccessSound - Sound to play when purchase was successful 
* \FailSound - Sound to play when purchase failed
* \AllowRepurchase - If this is 0 the failed sound will play if you already have the char
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Change_Character : public ScriptImpClass {
	int custom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Applies damage while in the zone by attaching the script below (JMG_Utility_Apply_Damage_While_In_Zone_Attached)
* \Rate - How often to apply the damage
* \DamageAmount - How much damage to apply
* \Warhead - What warhead to use
* \DamagerId - ID of what is applying the damage, 0 = nothing, -1 = enter, -2 = zone
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Apply_Damage_While_In_Zone : public ScriptImpClass {
	char params[512];
	void Created(GameObject *obj);
	void Entered(GameObject *obj,GameObject *enterer);
	void Exited(GameObject *obj,GameObject *exiter);
};

/*!
* \brief Applies damage to object at a specified rate (JMG_Utility_Apply_Damage_On_Timer)
* \Rate - How often to apply the damage
* \DamageAmount - How much damage to apply
* \Warhead - What warhead to use
* \DamagerId - ID of what is applying the damage, 0 = nothing, -1 = self
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Apply_Damage_On_Timer_Base : public ScriptImpClass {
	float rate;
	int damagerId;
	char warhead[128];
	float damageAmount;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Sends a custom to an object on exit
* \PlayerType - Player type the zone triggers for
* \ID - ID to send the custom to, 0 sends to self, -1 sends to enter
* \Custom - Custom message to send
* \Param - Param to send
* \Delay - Delay to add before sending custom
* \TriggerOnce - Allows the script only to trigger the first time the zone is entered
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Zone_Send_Custom_Exit : public ScriptImpClass {
	int playerType;
	int custom;
	int param;
	float delay;
	int id;
	bool triggerOnce;
	void Created(GameObject *obj);
	void Exited(GameObject *obj,GameObject *exiter);
};

/*!
* \brief Used to enable or disable all spawners within an ID range on zone enter but only enables the IDs that Mod to 0
* \StartID - ID to start at
* \EndID - ID to stop at
* \Custom - Custom used to trigger the enable/disable
* \Enable - Enable or disable all the spawners in the range, if -1 the param is used to determin this
* \Mod - Mod to use, if -1 param will be used instead
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Enable_Spawners_In_Range_Modular : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Creates an explosion at the bone specified on the attached object
* \Custom - Custom to trigger the explosion on
* \Explosion - Explosion to create 
* \Bone - Bone to create the explosion at
* \Owner - Who own's the explosion (-1 = sender, 0 = self, positive numbers = id of object)
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Create_Explosion_At_Bone : public ScriptImpClass {
	char explosion[256];
	char bone[32];
	int owner;
	int custom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Sends a custom when a custom is recieved, ignore time makes it unable to send another custom until timer expires 
* \Custom - Custom to trigger on
* \ID - ID to send the custom to, 0 sends to self, -1 sends to sender
* \SendCustom - Custom message to send
* \Param - Param to send
* \Delay - Delay to add before sending custom
* \IgnoreTime - Time during which customs can't be sent again
* \StartsEnabled - Does the script start enabled
* \EnableCustom - Custom enables or disables this script, param of 1 = enable
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Send_Custom_Repeat_Ignore_Time : public ScriptImpClass {
	int custom;
	int sendCustom;
	int params;
	float delay;
	int id;
	time_t lastTriggerTime;
	int ignoreTime;
	bool enable;
	int enableCustom;
	void Created(GameObject *obj);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Sends a custom when damaged
* \MinDamage - min damage required to trigger the script
* \ID - ID to send the custom to, 0 sends to self, -1 sends to sender
* \Custom - Custom message to send
* \Param - Param to send
* \Delay - Delay to add before sending custom
* \SenderID - ID of the object that sent the custom, 0 for itself, -1 for damager
* \StartsEnabled - Does the script start enabled
* \EnableCustom - Custom enables or disables this script, param of 1 = enable
* \Repeat - Can the script trigger more than once
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Damage_Send_Custom : public ScriptImpClass {
	int custom;
	int params;
	float delay;
	int id;
	int senderId;
	bool enable;
	int enableCustom;
	bool repeat;
	float minDamage;
	void Created(GameObject *obj);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Sends a custom message when a player comes into range of this object
* \Distance - how far away can the plaeyrs go
* \ID - Id of the object to send the custom to, 0 sends to itself
* \Message - Custom to send
* \Param - parameter to send with the custom
* \Delay - Time amount to wait before sending the custom
* \TriggerOnce - If true the script can only fire once
* \SleepTime - Value in 10ths of a second the script must wait before sending the custom again
* \MaxRange - Players beyond this won't be detected, if 0 the param is ignored
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Send_Custom_When_Player_Inside_Of_Range : public ScriptImpClass {
	int sleepTime;
	int sleeping;
	bool triggerOnce;
	bool enabled;
	float distance;
	float delay;
	float maxRange;
	int id;
	int param;
	int custom;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
};

/*!
* \brief Works exactly like JMG_Utility_Custom_Enable_Spawners_In_Range_Mod but player count subtracts from the mod (min is 1)
* \StartID - ID to start at
* \EndID - ID to stop at
* \Custom - Custom used to trigger the enable/disable
* \Enable - Enable or disable all the spawners in the range, if -1 the param is used to determin this
* \Mod - Mod to use, if -1 param will be used instead
* \PlayerMulti - Multiply the player count by this before subtracting the mod
* \Invert - if non zero it will disable or enable the ones that don't fit the mod's criteria but are in range
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Custom_Enable_Spawners_In_Range_ModPlayer : public ScriptImpClass {
	void Custom(GameObject *obj,int message,int param,GameObject *sender);
};

/*!
* \brief Needs to be placed on the map to make JMG_Utility_Spawn_With_Last_Selected_Gun_Player work
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Spawn_With_Last_Selected_Gun_Control : public ScriptImpClass {
	void Created(GameObject *obj);
	void Destroyed(GameObject *obj);
public:
	static bool controllerPlaced;
	static char playerWeapons[128][256];
	static char playerNames[128][256];
};

/*!
* \brief When attached to a player preset, it tracks what weapon the player was holding when they died and tries to select it again when they respawn
* \GrantWeapon - Grants the weapon they had last with full ammo if they don't have it
* \GrantDelay - Time to wait before granting the weapon again
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Spawn_With_Last_Selected_Gun_Player : public ScriptImpClass {
	int playerId;
	void Created(GameObject *obj);
	void Timer_Expired(GameObject *obj,int number);
	void Destroyed(GameObject *obj);
	void InitialSetup(GameObject *obj);
	void GrantPlayersWeapon(GameObject *obj);
public:
	JMG_Utility_Spawn_With_Last_Selected_Gun_Player()
	{
		playerId = 0;
	}
};

/*!
* \brief Gives a weapon to the player without need of a powerup
* \WeaponName - Name of the weapon to give
* \GrantWeapon - Should the weapon be given or just rounds for the weapon
* \GrantRounds - How many rounds to give (-1 for infintie) 
* \GrantClips - Should rounds be put in your "backpack"
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_Created_Give_Weapon : public ScriptImpClass {
	void Created(GameObject *obj);
};