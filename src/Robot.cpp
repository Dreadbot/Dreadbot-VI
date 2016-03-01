#include "WPILib.h"
#include "../ADBLib/src/ADBLib.h"
using namespace ADBLib;

class Robot: public IterativeRobot
{
private:
	TractionDrive* drivebase;
	CANTalon* motors[5];
	Joystick* jys;
	Joystick* jys2;
	ADBLib::Controller gpd;
	ADBLib::Controller gpd2;
	Compressor* compressor;
	Preferences* prefs;

	SimplePneumatic* shooterPiston;
	SimplePneumatic* liftArm;
	SimplePneumatic* extendArm;
	SimplePneumatic* tail;
	SimplePneumatic* mandibles;
	CANTalon* fan;

	uint8_t lastControl; //The last joystick (0 or 1) to use the elevation control
	uint8_t armPos; //The elevation of the arm (0=low, 1=mid, 2=high)
	bool fanOn;

	void RobotInit()
	{
		prefs = Preferences::GetInstance();
		compressor = new Compressor(1);
		jys = new Joystick(0);
		jys2 = new Joystick(1);
		gpd.setJoystick(jys);
		gpd2.setJoystick(jys2);
		gpd.parseConfig("/ControlConfig.xml");
		gpd2.parseConfig("/ControlConfig.xml");
		gpd2.switchProfile("secondary");

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

		lastControl = 0;
		armPos = 0; //This might result in odd behavior at start
		fanOn = false;
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
		//Primary driver controls
		mandibles->set(gpd["mandibles"]);
		if (gpd["mandibles"])
			fanOn = true;

		//todo: FIGURE OUT WHY PUSHING A JOYSTICK UP MAKES IT GO NEGATIVE
		if (gpd["armUp"] || gpd2["armElevAlt"] < 0)
		{
			liftArm->set(1);
			armPos = 2;
		}
		else if (gpd["armDown"] || gpd2["armElevAlt"] > 0)
		{
			liftArm->set(-1);
			armPos = 0;
		}
		else if (gpd["armDown"] == 0 && gpd["armUp"] == 0 && gpd2["armElevAlt"] == 0)
			liftArm->set(0);

		if (gpd["shooter"])
		{ //Shooter behavior varies with arm position
			if (armPos == 1 || armPos == 2)
			{
				extendArm->set(1);
				Wait(0.49);
				shooterPiston->set(1);
				Wait(0.3);
				shooterPiston->set(0);
			}
			else
			{
				shooterPiston->set(1);
				Wait(0.3);
				shooterPiston->set(0);
			}
			fanOn = false;
		}
		drivebase->drive(0, -gpd["transY"], gpd["rot"]);

		extendArm->set(gpd2["extendArm"]);
		tail->set(gpd2["tail"]);

		//Fan control, uses the *cough* force *cough* fan macro
		if (!gpd2["fanOverride"])
			fan->Set(fanOn);
		else
			fan->Set(1);
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
