#pragma once

#include "WPILib.h"
#include "../ADBLib/src/ADBLib.h"
#include <stack>
using namespace ADBLib;
using std::stack;

//PID constants, just in case. Do **NOT** touch kI, it is hideously bugged!
#define DRIVE_KP 0.007
#define DRIVE_KI 0.0
#define DRIVE_KD 0.003

//NavX MXP collision constants
#define COLLISION_THRESHOLD_DELTAG 	0.5 	//Taken from NavX library demo code, see note for isColliding()
#define COLLISION_TIMEOUT 			0.06 	//60ms timeout to ensure valid collision detection. May be too generous.

//Vision -- LifeCam 3000 USB camera values
#define CAM_FOV 	57.0					//Manually measured
#define CAM_XSIZE 	1280.0					//Obtained from image data
#define CAM_YSIZE 	720.0					//Obtained from image data
#define CAM_ANGRESO (CAM_FOV / CAM_XSIZE)	//Calculated constant

//Autonomous constants
#define BREACH_TIME			2.45			//Time taken to breach any given rampart
#define BREACH_SPEED		0.75			//Speed (-1-1) of driving while breaching.
#define LOWBAR_ANGLE		45.0			//Angle to rotate by after passing through the low bar
#define SHOOT_TIME			0.3				//How long the shooter piston should be extending/extended in order to shoot
#define ARMUP_PULSETIME 	0.25 			//Time to pulse the arm to raise cameras above the bumpers.
#define ROTATE_SCALAR		(1.0 / 90.0)	//Value to multiply error by to rotate at the right speed
#define DRIVE_SCALAR		(1.0 / 18.0)	//Value to multiply error by to drive straight
#define ARMUP_TIME			0.5				//Time to raise the arm from lowest to highest position
#define ARMOUT_SHOOTERDELAY	0.49			//Time delay until the shooter piston should eject the ball after the arm starts extending

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

	//Static stack thingies. I don't know what I'm doing anymore...
	static stack<double> globalDrive; 	//Used to pass in drive times when needed
	static stack<double> globalRotate;		//used to pass in rotations when needed; it's yet another awful hack.

private:
	static Drivebase* drivebase;
	static SimplePneumatic* shooterPiston;
	static SimplePneumatic* arm;
	static SimplePneumatic* extendArm;
	static AHRS* ahrs;

	friend class AutoBot;
};
