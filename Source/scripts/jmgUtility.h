
#pragma once
#include "general.h"
#include "scripts.h"
#include "engine.h"
#include "WeaponClass.h"
#include "WeaponBagClass.h"
#include "SoldierGameObj.h"
#include "PhysicsSceneClass.h"
#include "MoveablePhysClass.h"

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
* \Death_Message - Message to send when object dies
* \Repaired_Message - Message to send when object repaired
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
  void Custom(GameObject *obj,int type,int param,GameObject *sender);
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
	int repairTargetId;
	Vector3 lastPos;
	Vector3 moveLocation;
	int lastRepairTargetId;
	GameObject *lastTarget;
	GameObject *lastSecondaryTarget;
	bool lastRepairTarget;
	bool lastUseRepairGun;
	void Created(GameObject *obj);
	void Enemy_Seen(GameObject *obj,GameObject *seen);
	void Timer_Expired(GameObject *obj,int number);
	void AttackTarget(GameObject *obj,GameObject *target,GameObject *secondaryTarget,bool repairTarget,bool useRepairGun);
	void Action_Complete(GameObject *obj,int action_id,ActionCompleteReason reason);
	void Damaged(GameObject *obj,GameObject *damager,float damage);
	bool inRange(GameObject *obj);
	inline bool Valid_Repair_Target(GameObject *obj,GameObject *target,int playerType);
	inline bool Valid_Repair_Target_C4(GameObject *obj,GameObject *target,int playerType);
};
/*!
* \brief Will change the visibility of an object on custom for the specified player id
* \Message - Message to trigger this script on
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
			syncObjectNodeList = new SyncObjectNode(obj);
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
* \brief Turns a placed object into a renstyle spawner that can be sent customs like a normal object
* \SpawnPreset - Preset to spawn
* \RespawnTime - Time it takes between spawns after last object was killed
* \RespawnTimeRandom - Random time to add or subtract from the spawn time
* \StartsEnabled - Can the object spawn on script create
* \EnableMessage - Message used to enable the spawner
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
};

class JMG_Utility_Basic_Spawner_Attach : public ScriptImpClass
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
* \author jgray
* \ingroup JmgUtility
*/
class JMG_Utility_AI_Vehicle : public ScriptImpClass {
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
	int togglePathfind;
	bool pathfindOn;
	bool forcePathfindOverride;
	int stuckReturnHome;
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
* \brief Used to trigger the simple swimming animation system this is to be used along side JMG_Utility_Swimming_Infantry, 
* \ any soldiers that enter the script zone without JMG_Utility_Swimming_Infantry attached will die.
* \WaterColor[R|G|B] - Color to fade the screen while in this swimming zone
* \WaterOpacity - How transparent the screen is while in this zone
* \WaterMinViewDistance - The min distance that the fog shows up when underwater
* \WaterMaxViewDistance - The max distance of the fog when underwater
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
};

/*!
* \brief Used to enable or disable all spawners within an ID range on zone enter
* \StartID - ID to start at
* \EndID - ID to stop at
* \PlayerType - Player type the zone triggers for
* \Enable - Enable or disable all the spawners in the range
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
* \SendMessageObjectID - ID of the object to send the message to, if 0 it sends the messages to itself
* \NearToBuildingMessage - Message to send when a building is within range
* \FarFromBuildingMessage - Message to send when buildings are no longer within range
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