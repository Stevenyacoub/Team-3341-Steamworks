#include <Commands/AutonomousLeft.h>
#include "TurnAndDrive.h"
#include "NavigateCV.h"
#include "../CommandBase.h"
#include "CVDrive.h"

using namespace std;
AutonomousLeft::AutonomousLeft(){

	//AddSequential(new TurnAndDrive(50,0));
	AddSequential(new CVDrive());
	//AddSequential(new Turn(45));
}


