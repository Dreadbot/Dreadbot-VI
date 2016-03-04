#include "../../ADBLib/src/ADBLib.h"
#include "RoboState.h"
#include "Drive.h"
#include "GuidedDrive.h"
#include "Rotate.h"
#include "Shoot.h"
#include "Stopped.h"
using ADBLib::FSMTransition;
using ADBLib::FiniteStateMachine;
using ADBLib::Log;
using ADBLib::Logger;
using ADBLib::Drivebase;
using ADBLib::SimplePneumatic;

class AutoBot
{
public:
	enum autonModes {NOP, BREACH, GUIDED, FULL};

	AutoBot();
	~AutoBot();
	void init(Drivebase* newDB, SimplePneumatic* newShooterPiston,
				SimplePneumatic* newArm, SimplePneumatic* newExtendArm, AHRS* ahrs);
	void switchMode(autonModes mode);
	void update();
	void reset() {} //@TODO Implement AutoBot::reset()

private:
	Log* log;
	FiniteStateMachine fsm;
	FSMTransition transitionTable[15];

	//States
	Drive* drive_breach;
	GuidedDrive* guidedDrive;
	Rotate* rotate;
	Stopped* stopped;
	Shoot* shoot;
};
