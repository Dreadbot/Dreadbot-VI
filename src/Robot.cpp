#include "WPILib.h"
#include "../ADBLib/src/ADBLib.h"
//using ADBLib::Controller;
using ADBLib::Drivebase;
using ADBLib::TractionDrive;
using Hydra::Logger; //TODO: Fix hydra namespace (???)

class Robot: public IterativeRobot
{
private:
	Joystick* jys;
	ADBLib::Controller gamepad; //Pleases eclipse for some reason

	Drivebase* drivebase;
	CANTalon* motors[4];

	void RobotInit()
	{
		Logger::newLog("sysLog", "sysLog"); //TODO: check if newLog() appends file extension. if so, fix.

		jys = new Joystick(0);
		gamepad.setJoystick(jys);
		try
		{
			gamepad.parseConfig("/ControlConfig.xml");
		}
		catch (const std::exception &ex)
		{
			Logger::log(ex.what(), "sysLog", Hydra::error);
		}

		for (int i = 0; i < 4; i++)
		{
			motors[i] = new CANTalon(i+1, 3); //Everything below this in this loop is ripped from last year's mecanum drive code
			motors[i]->SetControlMode(CANSpeedController::ControlMode::kSpeed);
			motors[i]->SetPosition(0.0);
			motors[i]->SelectProfileSlot(0);
			//motors[i]->SetFeedbackDevice(CANTalon::QuadEncoder);
			motors[i]->SetPID(0.5, 0, 0, 0);
			motors[i]->SetVoltageRampRate(0.5); //Ramp up for drive motors
		}
		drivebase = new TractionDrive(motors[0],
				motors[1],
				motors[3],
				motors[2]);

		Logger::log("Successfully initialized Robot", "sysLog");
	}

	void AutonomousInit()
	{

	}

	void AutonomousPeriodic()
	{

	}

	void TeleopInit()
	{

	}

	void TeleopPeriodic()
	{

	}

	void TestPeriodic()
	{

	}
};

START_ROBOT_CLASS(Robot);
