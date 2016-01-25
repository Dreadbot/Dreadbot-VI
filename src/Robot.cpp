#include "WPILib.h"
#include "../ADBLib/src/ADBLib.h"
using ADBLib::Logger;
using ADBLib::Drivebase;
using ADBLib::TractionDrive;

class Robot: public IterativeRobot
{
private:
	Joystick* jys;
	Compressor* compressor;
	BuiltInAccelerometer* bia;
	Drivebase* drivebase;
	CANTalon* motors[4];

	void RobotInit()
	{
		Logger::newLog("sysLog", "/sysLog.txt");
		compressor = new Compressor(0);
		jys = new Joystick(0);
		bia = new BuiltInAccelerometer;
		for (int i = 0; i < 4; i++)
		{
			motors[i] = new CANTalon(i + 1);
			motors[i]->SetControlMode(CANTalon::kPercentVbus);
		}

		drivebase = new TractionDrive(motors[0], motors[1], motors[2], motors[3]);
		motors[3]->SetInverted(true);
		motors[1]->SetInverted(true);



		Logger::log("Successfully initialized Robot", "sysLog");
	}

	void AutonomousInit()
	{
		compressor->Start();
	}

	void AutonomousPeriodic()
	{

	}

	void TestInit()
	{
		compressor->Start();
	}

	void TeleopInit()
	{
		compressor->Start();
	}

	void TeleopPeriodic()
	{
		double aX = 0, aY = 0, aZ = 0;
		aX = bia->GetX();
		aY = bia->GetY();
		aZ = bia->GetZ();
		SmartDashboard::PutNumber("aX", aX);
		SmartDashboard::PutNumber("aY", aY);
		SmartDashboard::PutNumber("aZ", aZ);

		drivebase->drive(jys->GetRawAxis(2), jys->GetRawAxis(1), jys->GetRawAxis(0));

	}

	void DisabledInit()
	{
		compressor->Stop();
	}
};

START_ROBOT_CLASS(Robot);
