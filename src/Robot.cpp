#include "WPILib.h"
#include "../ADBLib/src/ADBLib.h"
using namespace ADBLib;

void rotate(double degrees, Drivebase* drivebase, AHRS* ahrs);

class Robot: public IterativeRobot
{
private:
	TractionDrive* drivebase;
	CANTalon* motors[5];
	Joystick* jys;
	ADBLib::Controller gpd;
	Compressor* compressor;
	Preferences* prefs;

	AHRS* ahrs;
	SimplePneumatic* shooterPiston;
	SimplePneumatic* liftArm;
	SimplePneumatic* extendArm;
	SimplePneumatic* tail;
	SimplePneumatic* mandibles;
	CANTalon* fan;

	Timer timer;
	double first_maneuver_time = 2.5;

	void RobotInit()
	{
		prefs = Preferences::GetInstance();
		compressor = new Compressor(1);
		jys = new Joystick(0);
		gpd.setJoystick(jys);
		gpd.parseConfig("/ControlConfig.xml");
		for (int i = 1; i < 5; i++)
		{
			motors[i] = new CANTalon(i);
			motors[i]->SetControlMode(CANTalon::kPercentVbus);
			motors[i]->SetVoltageRampRate(0.5);
		}
		motors[2]->SetInverted(true);
		motors[1]->SetInverted(true);
		drivebase = new TractionDrive(motors[4], motors[2], motors[3], motors[1]);

		ahrs = new AHRS(SPI::Port::kMXP);
		liftArm = new SimplePneumatic(new DoubleSolenoid(0, 1));
		shooterPiston = new SimplePneumatic(new Solenoid(3));
		extendArm = new SimplePneumatic(new Solenoid(2));
		tail = new SimplePneumatic(new Solenoid(5));
		mandibles = new SimplePneumatic(new Solenoid(4));
		fan = new CANTalon(5);

	}

	void AutonomousInit()
	{
		compressor->Start();
		ahrs->ZeroYaw();
	}

	void AutonomousPeriodic()
	{
		//if (prefs->GetBoolean("fan-on", true))
			//fan->Set(1);
		//else
		//fan->Set(0);
			//Using Gyroscope and dead reckoning to move from #3 to front of tower
		timer.Start();
		while(timer.Get() < first_maneuver_time)
		{
			drivebase->drive(0, 0.6, -(ahrs->GetYaw() / 18.0));
		}
		timer.Stop();
		drivebase->drive(0, 0, 0);
		Wait(0.2);

		rotate(45, drivebase, ahrs);
		drivebase->drive(0, 0.5, 0);
		Wait(1.0);
		rotate(-45.0, drivebase, ahrs);
		drivebase->drive(0, 0.5, 0);
		Wait(0.7);
		drivebase->drive(0, 0, 0);
		Wait(1000000000);
		/*while(fabs(ahrs->GetYaw() - fdest) > 5);
		{
			double diff = ahrs->GetYaw() - fdest;
			if (diff > 180.0)
				diff = 360.0 - diff;
			if (diff < -180)
				diff = 360 + diff;

			drivebase->drive(0, 0.6, -(diff / 90.0));
		}*/
	}

	void TeleopInit()
	{
		compressor->Start();
	}

	void TeleopPeriodic()
	{
		shooterPiston->set(gpd["shooter"]);
		liftArm->set(-gpd["liftArm"]);
		extendArm->set(gpd["extendArm"]);
		mandibles->set(gpd["mandibles"]);
		tail->set(gpd["tail"]);

		drivebase->drive(0, -gpd["transY"], gpd["rot"]);

		if (prefs->GetBoolean("fan-on", true))
			fan->Set(1);
		else
			fan->Set(0);
	}

	void DisabledInit()
	{
		fan->Set(0);
	}

	void TestInit()
	{

	}

	void TestPeriodic()
	{

	}
};

void rotate(double degrees, Drivebase* drivebase, AHRS* ahrs)
{
	SmartDashboard::PutNumber("yaw", ahrs->GetYaw());
	double startRot = ahrs->GetYaw();
	double fdest = degrees + startRot;
	if (fdest > 360.0)
		fdest -= 360.0;

	while(fabs(ahrs->GetYaw() - fdest) > 5)
	{
		double diff = ahrs->GetYaw() - fdest;
		if (diff > 180.0)
			diff = 360.0 - diff;
		if (diff < -180)
			diff = 360 + diff;

		SmartDashboard::PutNumber("diff", diff);
		drivebase->drive(0, 0, -diff / 45.0);
	}
	drivebase->drive(0, 0, 0);
}

START_ROBOT_CLASS(Robot);
