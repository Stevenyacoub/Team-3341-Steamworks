#ifndef Turn_H
#define Turn_H

#include "../CommandBase.h"
#include "Utilities/WVPIDController.h"

class Turn : public CommandBase {
public:
        Turn(double angle);
        void Initialize();
        void Execute();
        bool IsFinished();
        void End();
        void Interrupted();
        void ForceFinish();

    private:
        double angle;
        bool forceFinish;
        WVPIDController* anglePid;
};

#endif  // Turn_H
