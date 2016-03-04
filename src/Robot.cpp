#include "WPILib.h"
#include "../ADBLib/src/ADBLib.h"
#include "auton/AutoBot.h"
#include "DreadbotDIO.h"
#include <unistd.h>
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
	AHRS* ahrs;
	MultiVision* mv;

	SimplePneumatic* shooterPiston;
	SimplePneumatic* liftArm;
	SimplePneumatic* extendArm;
	SimplePneumatic* tail;
	SimplePneumatic* mandibles;
	CANTalon* fan;

	bool armElevAlt; //Stores if the alternate control was used last or not. true = alt, false=regular
	enum {UP, DOWN} armPos;
	bool fanOn;

	AutoBot* autobot;

	void RobotInit()
	{
		//Just in case the above doesn't work, what's below is an awful hack.
		//system("if [ -f \"/sysLog.txt\" ] ; then i=0; while [ -f \"/robologs/sysLog.$i.txt\" ] ; do ((i++)) ; done ; mv \"/sysLog.txt\" \"/robologs/sysLog.$i.txt\" ; fi ;");

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
		ahrs = new AHRS(SPI::Port::kMXP);
		mv = new MultiVision;
		mv->switchCamera("cam0");

		liftArm = new SimplePneumatic(new DoubleSolenoid(0, 1));
		shooterPiston = new SimplePneumatic(new Solenoid(3));
		extendArm = new SimplePneumatic(new Solenoid(2));
		tail = new SimplePneumatic(new Solenoid(5));
		mandibles = new SimplePneumatic(new Solenoid(4));
		fan = new CANTalon(5);

		autobot = new AutoBot;
		autobot->init(drivebase, shooterPiston, liftArm, extendArm, ahrs);

		armElevAlt = true;
		armPos = DOWN; //This might result in odd behavior at start
		fanOn = false;

		Logger::log("Finished initializing robot; starting GRIP...", "sysLog");

		if (fork() == 0)
			system("/home/lvuser/grip &");

		Logger::log("Successfully started GRIP!", "sysLog");
	}

	void AutonomousInit()
	{
		Logger::log("Started AUTONOMOUS with mode" + to_string(AutoBot::BREACH) + "!", "sysLog");
		autobot->switchMode(getAutonMode());
		compressor->Start();
	}

	void AutonomousPeriodic()
	{
		fan->Set(1);
		autobot->update();
	}

	void TeleopInit()
	{
		Logger::log("Started TELEOP!", "sysLog");
		compressor->Start();
		jys->SetRumble(Joystick::kLeftRumble, 1024);
	}

	void TeleopPeriodic()
	{
		//Vision during teleop
		mv->postImage();

		//Primary driver controls
		mandibles->set(gpd["mandibles"]);
		if (gpd["mandibles"])
			fanOn = true;

		if (gpd["armUp"] || gpd["armDown"])
		{
			armElevAlt = false;
			if (gpd["armUp"])
			{
				armPos = UP;
				liftArm->set(1);
			}
			else if (gpd["armDown"])
			{
				armPos = DOWN;
				liftArm->set(-1);
			}
		}
		if (gpd2["armElevAlt"] != 0 || armElevAlt)
		{
			double input = gpd2["armElevAlt"];
			if (input < 0) //No, these comparisons are NOT flipped. I don't know why though.
			{
				armPos = UP;
				liftArm->set(1);
			}
			else if (input > 0)
			{
				armPos = DOWN;
				liftArm->set(-1);
			}
			else
				liftArm->set(0);
			armElevAlt = true;
		}

		if (gpd["shooter"])
		{ //Shooter behavior varies with arm position
			if (armPos == UP)
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
		Logger::log("DISABLING robot!", "sysLog");
		Logger::flushLogBuffers();
		compressor->Stop();
		fan->Set(0);
	}

	void TestInit()
	{
		Logger::log("Started TEST!", "sysLog");
		compressor->Start();
	}

	void TestPeriodic()
	{

	}
};

START_ROBOT_CLASS(Robot);
