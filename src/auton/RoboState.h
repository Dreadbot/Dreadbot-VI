#pragma once

#include "WPILib.h"
#include "../ADBLib/src/ADBLib.h"
#include <stack>
using namespace ADBLib;
using std::stack;

//PID constants, just in case. Do **NOT** touch kI, it is hideously bugged!
#define DRIVE_KP 						0.007					//Proportional constant (actual change)
#define DRIVE_KI 						0.0						//Integral constant (sum of change)
#define DRIVE_KD 						0.003					//Derivative constant (rate of change)

//NavX MXP collision constants
#define COLLISION_THRESHOLD_DELTAG 	0.7 					//Acceleration must exceed this limit for a collision to be registered
#define COLLISION_TIMEOUT 				0.06 					//60ms timeout to ensure valid collision detection. May be too generous.
#define COLLISION_DELAY				0.5						//Time to wait until collisions are registered. Used in drive/guideddrive states

//Vision -- LifeCam 3000 USB camera values
#define CAM_FOV 						57.0					//Manually measured
#define CAM_XSIZE 						1280.0					//Obtained from image data
#define CAM_YSIZE 						720.0					//Obtained from image data
#define CAM_ANGRESO					(CAM_FOV / CAM_XSIZE)	//Calculated constant

//Autonomous constants
#define BREACH_TIME					2.55					//Time taken to breach terrain defenses
#define LOWBAR_TIME					2.20					//Time taken to breach the lowbar.
#define BREACH_SPEED					0.75					//Speed (-1-1) of driving while breaching.
#define LOWBAR_ANGLE					65.3					//Angle to rotate by after passing through the low bar
#define GOAL_TIME						0.50					//How long the robot drives to the low goal after rotating
#define SHOOT_TIME						0.30					//How long the shooter piston should be extending/extended in order to shoot
#define ARMUP_PULSETIME 				0.30 					//Time to pulse the arm to raise the bowl enough to rotate
#define ROTATE_SCALAR					(1.0 / 85.0 )			//Value to multiply error by to rotate at the right speed
#define DRIVE_SCALAR					(1.0 / 18.0)			//Value to multiply error by to drive straight
#define ARMUP_TIME						0.50					//Time to raise the arm from lowest to highest position
#define ARMOUT_SHOOTERDELAY			0.49					//Time delay until the shooter piston should eject the ball after the arm starts extending
#define ROT_STDSTATE_COR				0.075					//Baseline rotation rate, applied to fix steady state-ish error.
#define HIGH_SHOT						false					//Whether the robot shoots for a high goal or n

//Preferences grabbers
#define PREFS 							Preferences::GetInstance()
#define PREFS_BREACH_TIME				PREFS->GetDouble("BREACH_TIME", BREACH_TIME)
#define PREFS_LOWBAR_TIME				PREFS->GetDouble("LOWBAR_TIME", LOWBAR_TIME)
#define PREFS_LOWBAR_ANGLE				PREFS->GetDouble("LOWBAR_ANGLE", LOWBAR_ANGLE)
#define PREFS_GOAL_TIME				PREFS->GetDouble("GOAL_TIME", GOAL_TIME)
#define PREFS_HIGH_SHOT				PREFS->GetBoolean("HIGH_SHOT", HIGH_SHOT)

class RoboState : public FSMState
{
public:
	enum retcode {NO_UPDATE, ESTOP, TIMER_EXPIRED, COLLISION};

	virtual void enter() = 0; //Functions inherited from FSMState
	virtual int update() = 0;
	virtual void  exit() = 0;

protected:
	static void shoot();
	static void armUp(); //For RAISING the arm!
	static void armStop();
	static void armDown();
	static void armOut();
	static void armIn();
	static void drive(double transY, double rot);
	static double getYaw();
	static double getYawRate();
	static bool isColliding();
	static Log* autoLog;

private:
	static Drivebase* drivebase;
	static SimplePneumatic* shooterPiston;
	static SimplePneumatic* arm;
	static SimplePneumatic* extendArm;
	static AHRS* ahrs;

	friend class AutoBot;
};
