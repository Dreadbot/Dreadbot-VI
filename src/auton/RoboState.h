#pragma once

#include "WPILib.h"
#include "../ADBLib/src/ADBLib.h"
using namespace ADBLib;

//PID constants, just in case. Do **NOT** touch kI, it is hideously bugged!
#define DRIVE_KP 0.007
#define DRIVE_KI 0.0
#define DRIVE_KD 0.003

//NavX MXP collision constants
#define COLLISION_THRESHOLD_DELTAG 0.5 	//Taken from NavX library demo code, see note for isColliding()
#define COLLISION_TIMEOUT 0.06 			//60ms timeout to ensure valid collision detection. May be too generous.

//Vision -- LifeCam 3000 USB camera values
#define CAM_FOV 57.0		//Manually measured
#define CAM_XSIZE 1280.0	//Obtained from image data
#define CAM_YSIZE 720.0		//Obtained from image data
#define CAM_ANGRESO (CAM_FOV / CAM_XSIZE)	//Calculated constant

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

	static double globalDriveTime; //Used to pass in drive times when needed

private:
	static Drivebase* drivebase;
	static SimplePneumatic* shooterPiston;
	static SimplePneumatic* arm;
	static SimplePneumatic* extendArm;
	static AHRS* ahrs;

	friend class AutoBot;
};