#include "WPILib.h"
#include "../ADBLib/src/ADBLib.h"
using namespace ADBLib;

class Robot: public IterativeRobot
{
private:
	TractionDrive* drivebase;
	CANTalon* motors[5];
	Joystick* jys;
	ADBLib::Controller gpd;
	Compressor* compressor;
	Preferences* prefs;

	SimplePneumatic* shooterPiston;
	SimplePneumatic* liftArm;
	SimplePneumatic* extendArm;
	SimplePneumatic* tail;
	SimplePneumatic* mandibles;
	CANTalon* fan;

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
	}

	void AutonomousPeriodic()
	{
		if (prefs->GetBoolean("fan-on", true))
			fan->Set(1);
		else
			fan->Set(0);
		//drivebase->drive(0, -0.6, -(ahrs->GetYaw() / 18.0)); //y and x axis flipped?
	}

	void TeleopInit()
	{
		compressor->Start();
		jys->SetRumble(Joystick::kLeftRumble, 1024);
	}

	void TeleopPeriodic()
	{
		SmartDashboard::PutNumber("extendArm", jys->GetRawButton(5));
		extendArm->set(gpd["extendArm"]);
		mandibles->set(gpd["mandibles"]);
		shooterPiston->set(gpd["shooterEject"]); //Shooter override
		tail->set(gpd["tail"]);

		if (gpd["armUp"])
			liftArm->set(1);
		else if (gpd["armDown"])
			liftArm->set(-1);
		else
			liftArm->set(0);
		if (gpd["shooter"])
		{
			extendArm->set(1);
			Wait(0.49);
			shooterPiston->set(1);
			Wait(0.3);
			shooterPiston->set(0);
		}

		drivebase->drive(0, -gpd["transY"], gpd["rot"] + gpd["rotAlt"]);

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

START_ROBOT_CLASS(Robot);
