#ifndef CVDrive_H

#define CVDrive_H



#include "../CommandBase.h"

#include "Utilities/WVPIDController.h"

#include "WPILib.h"



class CVDrive: public CommandBase

{

public:

	CVDrive();

	void Initialize();

	void Execute();

	bool IsFinished();

	void End();

	void Interrupted();

private:

	double distance;

	double angle;

	WVPIDController* distancePid;

	WVPIDController* anglePid;

};



#endif
