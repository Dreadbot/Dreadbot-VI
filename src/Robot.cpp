#include "WPILib.h"
#include "../ADBLib/src/ADBLib.h"
#include "auton/AutoBot.h"
#include "DreadbotDIO.h"
#include <unistd.h>
#include <dirent.h>
#include <regex>
#include <sys/stat.h>
#include <string>
using namespace ADBLib;
using std::string;

void moveLog();

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
	Log* robolog;

	SimplePneumatic* shooterPiston;
	SimplePneumatic* liftArm;
	SimplePneumatic* extendArm;
	SimplePneumatic* tail;
	SimplePneumatic* mandibles;
	CANTalon* fan;

	bool armElevAlt; //Stores if the alternate control was used last or not. true = alt, false=regular
	enum {UP, DOWN} armPos;

	AutoBot* autobot;

	void RobotInit()
	{
		//Takes the existing robolog.txt file in / and moves it to /logfiles/
		//Also increments the log file name by a number so old logs are preserved.
		moveLog();
		robolog = Logger::getLog("robolog");

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

		robolog->log("Finished initializing robot");// starting GRIP...");

		//if (fork() == 0)
		//	system("/home/lvuser/grip &");
		//robolog->log("Successfully started GRIP!");
	}

	void AutonomousInit()
	{
		robolog->log("Started AUTONOMOUS with mode" + to_string(AutoBot::BREACH) + "!");
		fan->Set(1);
		autobot->switchMode(getAutonMode());
		compressor->Start();
	}

	void AutonomousPeriodic()
	{
		autobot->update();
	}

	void TeleopInit()
	{
		robolog->log("Started TELEOP!");
		compressor->Start();
		fan->Set(1);
		jys->SetRumble(Joystick::kLeftRumble, 1024);
	}

	void TeleopPeriodic()
	{
		//Vision during teleop
		mv->postImage();

		//Primary driver controls
		mandibles->set(gpd["mandibles"]);

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
		}
		drivebase->drive(0, -gpd["transY"], gpd["rot"]);

		extendArm->set(gpd2["extendArm"]);
		tail->set(gpd2["tail"]);
	}

	void DisabledInit()
	{
		robolog->log("DISABLING robot!");
		Logger::flushLogBuffers();
		compressor->Stop();
		fan->Set(0);
	}

	void TestInit()
	{
		//Runs through a test sequence, the results of which are logged
		Logger::log("Started test sequence!", "robolog");

		testComponent("COMPRESSOR", [&](){compressor->Start();});	//Uses a lambda function to pass in
																	//the test sequence.
		testComponent("DRIVEBASE", [&](){
			drivebase->drive(0, 1, 0);
			Wait(1);
			drivebase->drive(0, -1, 0);
			Wait(1);
			drivebase->drive(0, 0, 1);
			Wait(1);
			drivebase->drive(0, 0, -1);
			Wait(1);
			drivebase->drive(0, 0, 0);
		});

		testComponent("SHOOTER PISTON", [&](){
			shooterPiston->set(1);
			Wait(1);
			shooterPiston->set(0);
		});

		testComponent("LIFT ARM", [&](){
			liftArm->set(1);
			Wait(1.5); //Leaves the lift arm up because arm extension needs it up.
		});

		testComponent("ARM EXTENSION", [&](){
			extendArm->set(1);
			Wait(1.5);
			extendArm->set(0);
			Wait(1.5);
			liftArm->set(-1);
			Wait(1.5); //Lowers the lift arm as left up during the previous test.
		});

		testComponent("TAIL", [&](){
			tail->set(1);
			Wait(1);
			tail->set(0);
		});

		testComponent("MANDIBLES", [&](){
			mandibles->set(1);
			Wait(1);
			mandibles->set(0);
		});

		testComponent("FAN", [&](){
			fan->Set(1);
			Wait(1);
			fan->Set(0);
		});
	}

	void TestPeriodic()
	{

	}

	inline bool getTestResponse()
	{
		//Gets a y/n (t/f) response on component functionality
		//Controls not in ControlConfig.xml because they are
		//DIAGNOSTIC controls, not robot controls
		while (true)
		{
			if (jys->GetRawButton(3)) //Button X
				return true;
			if (jys->GetRawButton(2)) //Button B
				return false;
		}
	}

	void testComponent(string compName, std::function<void(void)> testFunc)
	{
		robolog->log("Testing " + compName + "...");
		testFunc();
		if (getTestResponse())
			robolog->log(compName + " is FUNCTIONAL");
		else
			robolog->log(compName + " is NOT FUNCTIONAL");
	}
};

void moveLog()
{
	//Credit to Mr. King for most of this function. It's been tweaked a lot to
	//CPP-ify it (for the sake of making it easier to explain to new programmers)
	//but everything filesystem-y is from him. Thank you!

	const string logdir = "/";
	const string savedir = "/robologs/";
	const string prefix = "robolog";
	const string f_extens = ".txt";

	// Will just give harmless error if directory already exists.
	// If there's a real error we'll catch it when the opendir() call
	// below fails. (returns NULL in case of failure)
	mkdir(savedir.c_str(), 0777);
	DIR* dp = opendir(savedir.c_str());
	if (!dp)
		return;

	// Find the highest existing saved robolog
	int max = 0;
	std::regex reg("\\d+");
	dirent* ent = readdir(dp);
	while (ent)
	{
		std::smatch matches;
		if (std::regex_search(string(ent->d_name), matches, reg))
		{
			int n = stoi(matches.str(0));
			if (n > max)
				max = n;
		}

		ent = readdir(dp);
	}
	string dst = savedir + prefix + "_" + std::to_string(max + 1) + f_extens;
	rename((logdir + prefix + f_extens).c_str(), dst.c_str());
}

START_ROBOT_CLASS(Robot);
