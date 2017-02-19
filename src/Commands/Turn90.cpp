#include "Turn90.h"

Turn90::Turn90() {
	Requires(drive);
}

// Called just before this Command runs the first time
void Turn90::Initialize() {
	//drive->resetGyro();
	drive->arcadeDrive(0,0.4);
}

// Called repeatedly when this Command is scheduled to run
void Turn90::Execute() {
	std::cout<<"angle: " << drive->getGyroAngle()<<std::endl;

}

// Make this return true when this Command no longer needs to run execute()
bool Turn90::IsFinished() {
	return drive->getGyroAngle() > 90 || drive->getGyroAngle() < -90;

}

// Called once after isFinished returns true
void Turn90::End() {
	drive->arcadeDrive(0,0);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void Turn90::Interrupted() {

}
