#include "WPILib.h"
#include "../ADBLib/src/ADBLib.h"
#include "../lib/navx_frc_cpp/include/AHRS.h"
using ADBLib::TractionDrive;
using ADBLib::MultiVision;

#define RADCONV (3.141592653 / 180.0)
#define MAX_ROT 336.0

class Robot: public IterativeRobot
{
private:
	AHRS* ahrs;
	TractionDrive* drivebase;
	CANTalon* motors[5];
	Joystick* jys;
	Preferences* prefs;
	double start_rots[3];
	Timer timer;
	MultiVision* vision;

	double last;

	void RobotInit()
	{
		ahrs = new AHRS(SPI::Port::kMXP);
		jys = new Joystick(0);
		for (int i = 1; i < 5; i++)
		{
			motors[i] = new CANTalon(i);
			motors[i]->SetControlMode(CANTalon::kPercentVbus);
			motors[i]->SetVoltageRampRate(0.5);
		}
		motors[2]->SetInverted(true);
		motors[1]->SetInverted(true);
		drivebase = new TractionDrive(motors[4], motors[2], motors[3], motors[1]);
		prefs = Preferences::GetInstance();
		last = 0;

		vision = new MultiVision;
		vision->switchCamera("cam0");
	}

	void AutonomousInit()
	{
		ahrs->ZeroYaw();
	}

	void AutonomousPeriodic()
	{
		drivebase->drive(0, -0.6, -(ahrs->GetYaw() / 18.0)); //y and x axis flipped?
		output_stats();
	}

	void TeleopInit()
	{
	}

	void TeleopPeriodic()
	{
		const double ctrlVal = jys->GetRawAxis(4);
		double error = ctrlVal - (ahrs->GetRawGyroZ() / MAX_ROT);
		double setpoint = ctrlVal;// + error + (prefs->GetDouble("P", 1) * (error - last));

		drivebase->drive(jys->GetRawAxis(0), -jys->GetRawAxis(1), setpoint);
		last = error;
		output_stats();
		vision->postImage();
	}

	void DisabledInit()
	{
	}

	void TestInit()
	{

	}

	void TestPeriodic()
	{

	}

	void output_stats()
	{

	}
};

START_ROBOT_CLASS(Robot);
