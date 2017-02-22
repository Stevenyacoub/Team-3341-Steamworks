/*
 * NavigateCV.cpp
 *
 *  Created on: Jan 14, 2017
 *      Author: Sauhaarda Chowdhuri
 */

#include "NavigateCV.h"
#include <time.h>

using namespace std;

// #define DEBUGSC 1
#define DEBUGCV 1

NavigateCV::NavigateCV(){
	Requires(drive);
	Initialize();
}
void NavigateCV::Initialize()
{
	state = CV;

	// Temporary initial values for Kp, Ki, and Kd.
	distKp = distKi = distKd = 1;
	angleKp = 2;
	angleKi = 3;
	angleKd = 1;

	// Set the goal distance and angle to 0. We are at this point right now so nothing will happen.
	distGoal = angleGoal = 0;

	leftDistance = 0;
	rightDistance = 0;

	angle = 0;
	power = 0;
	lastGyroVal = newGyroVal = 0;
	distPID = 0;
	encoderVal = 0;
	anglePID = NULL;

	distPID = new WVPIDController(distKp, distKi, distKd, distGoal, false);
	anglePID = new WVPIDController(angleKp, angleKi, angleKd, angleGoal, false);

	drive->resetEncoders();
	drive->resetGyro();

	distPID = new WVPIDController(distKp, distKi, distKd, distGoal, false);
	anglePID = new WVPIDController(angleKp, angleKi, angleKd, angleGoal, false);

	drive->resetEncoders();
	drive->resetGyro();
}

void NavigateCV::Execute(){
	if(state == CV){
		if(NetworkTablesInterface::gearFound()){ // Wait Until Gear is Found
			// Get CV Data
			distGoal = NetworkTablesInterface::getGearDistance();
			angleGoal = NetworkTablesInterface::getGearAzimuth();

#ifdef DEBUGCV
			cout << "CV Distance to Target:\t" << distGoal << endl;
			cout << "CV Azimuth:\t" << angleGoal << endl;
#endif

			if(distGoal > 0.9){ // Need to continue
				// Initialize PIDs with CV Data
				distPID->SetSetPoint(distGoal);
				anglePID->SetSetPoint(angleGoal);

				// Reset Sensors
				drive->resetGyro();
				drive->resetEncoders();

				// Change states
				state = INNER_LOOP;

				// Start timer
				start = clock();
#ifdef DEBUGCV
				cout << "Clock Started at:\t" << (double)start << endl;
#endif

#ifdef DEBUGCV
				cout << "Moving from CV to INNER_LOOP" << endl;
#endif
			}else{ // We are done, close enough to the target to end loop.
				state = END;
#ifdef DEBUGCV
				cout << "Moving from CV to END" << endl;
#endif
			}
		}
#ifdef DEBUGCV
		else{
			cout << "Another cycle spent waiting for CV." << endl;
		}
#endif
	}else if(state == INNER_LOOP){
		cout << "SC TIME ELAPSED:\t" << (double(clock() - start)/CLOCKS_PER_SEC) << endl;
		if((double(clock() - start)/CLOCKS_PER_SEC) < DRIVE_TIME){ // until drive time is elapsed
			// Get Encoder Data
			leftDistance = drive->getLeftEncoderDistance();
			rightDistance = drive->getRightEncoderDistance();

#ifdef DEBUGSC
			cout << "Left Encoder Distance:\t" << leftDistance << endl;
			cout << "Right Encoder Distance:\t" << rightDistance << endl;
#endif

			// Get Gyro Data and use IIR Filter to Reduce Error
			newGyroVal = drive->getGyroAngle();
#ifdef DEBUGSC
			cout << "Raw Gyro Value:\t" << newGyroVal << endl;
#endif
			newGyroVal = (IIR_CONST * lastGyroVal) + ((1.0 - IIR_CONST) * newGyroVal);
			lastGyroVal = newGyroVal; // update last gyro for next cycle
#ifdef DEBUGSC
			cout << "Transformed Gyro Value:\t" << newGyroVal << endl;
#endif

			// Tick PIDs
			power =  abs(distPID->Tick((abs(leftDistance)+abs(rightDistance))/2.0));
			angle = anglePID->Tick(newGyroVal);

#ifdef DEBUGSC
			cout << "Dist PID Error:\t" << distPID->GetError() << endl;
			cout << "Angle PID Error:\t" << anglePID->GetError() << endl;
			cout << "Drive Power:\t" << power << endl;
			cout << "Steering Intensity:\t" << angle << endl;
#endif
			// Initiate Drive
			drive->arcadeDrive(power, angle);
		}else{
			drive->arcadeDrive(0,0); // Disable drive
			state = CV;
#ifdef DEBUGSC
			cout << "Moving from INNER_LOOP to CV" << endl;
#endif
		}
	}
}

bool NavigateCV::IsFinished()
{
	return state == END;
}

void NavigateCV::End()
{
	drive->arcadeDrive(0,0);
}

void NavigateCV::Interrupted(){
	drive->arcadeDrive(0,0);
}

NavigateCV::~NavigateCV()
{
	delete distPID;
	delete anglePID;
}
