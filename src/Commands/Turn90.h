#ifndef Turn90_H
#define Turn90_H

#include "../CommandBase.h"

class Turn90 : public CommandBase {
public:
	Turn90();
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
private:
	double angle;
};

#endif  // Turn90_H
