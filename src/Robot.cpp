
#include <memory>

#include "CommandBase.h"
#include <Commands/Command.h>
#include <Commands/Scheduler.h>
#include <IterativeRobot.h>
#include <LiveWindow/LiveWindow.h>
#include <SmartDashboard/SendableChooser.h>
#include <SmartDashboard/SmartDashboard.h>
#include "Utilities/NetworkTablesInterface.h"

#include "Subsystems/GyroL3GD20H.h"

class Robot: public frc::IterativeRobot {

private:
	wvrobotics::GyroL3GD20H* gyro;
	wvrobotics::GyroAxis axis;
public:
	void RobotInit() override {
		//chooser.AddDefault("Default Auto", new ExampleCommand());
		// chooser.AddObject("My Auto", new MyAutoCommand());
		frc::SmartDashboard::PutData("Auto Modes", &chooser);
	}

	/**
	 * This function is called once each time the robot enters Disabled mode.
	 * You can use it to reset any subsystem information you want to clear when
	 * the robot is disabled.
	 */
	void DisabledInit() override {
		gyro = new wvrobotics::GyroL3GD20H(I2C::kOnboard, 0x6b);
				gyro->initializeGyro();
				std::chrono::milliseconds timespan(5000); // 5000 milliseconds can be changed to a certain time
				//fix the below line of code for thread::sleep_for
				//std::this_thread::sleep_for(timespan);
				gyro->calibrateGyrodata(50); //50 is the calibration sample size; can change it as desired

	}

	void DisabledPeriodic() override {
		frc::Scheduler::GetInstance()->Run();
	}

	/**
	 * This autonomous (along with the chooser code above) shows how to select
	 * between different autonomous modes using the dashboard. The sendable
	 * chooser code works with the Java SmartDashboard. If you prefer the
	 * LabVIEW Dashboard, remove all of the chooser code and uncomment the
	 * GetString code to get the auto name from the text box below the Gyro.
	 *
	 * You can add additional auto modes by adding additional commands to the
	 * chooser code above (like the commented example) or additional comparisons
	 * to the if-else structure below with additional strings & commands.
	 */
	void AutonomousInit() override {
		/* std::string autoSelected = frc::SmartDashboard::GetString("Auto Selector", "Default");
		if (autoSelected == "My Auto") {
			autonomousCommand.reset(new MyAutoCommand());
		}
		else {
			autonomousCommand.reset(new ExampleCommand());
		} */

		autonomousCommand.reset(chooser.GetSelected());

		if (autonomousCommand.get() != nullptr) {
			autonomousCommand->Start();
		}
	}

	void AutonomousPeriodic() override {
		frc::Scheduler::GetInstance()->Run();
	}

	void TeleopInit() override {
		// This makes sure that the autonomous stops running when
		// teleop starts running. If you want the autonomous to
		// continue until interrupted by another command, remove
		// this line or comment it out.
		if (autonomousCommand != nullptr) {
			autonomousCommand->Cancel();
		}
	}

	void TeleopPeriodic() override {
		frc::Scheduler::GetInstance()->Run();

		static int count = 0;
				count++;
				if (count % 8 == 0)
				{
					Scheduler::GetInstance()->Run();
					//methods to read in Gyro Data
					axis = gyro->readGyroData(false, 0); //calibration mode is false
					axis.overrunofAxis();
				}
	}

	void TestPeriodic() override {
		frc::SmartDashboard::PutBoolean("CVGearFound", NetworkTablesInterface::gearFound());
		frc::SmartDashboard::PutNumber("CVGearDistance",NetworkTablesInterface::getGearDistance());
		frc::SmartDashboard::PutNumber("CVGearAltitude", NetworkTablesInterface::getGearAltitude());
		frc::SmartDashboard::PutNumber("CVGearAzimuth", NetworkTablesInterface::getGearAzimuth());
	    frc::SmartDashboard::PutBoolean("CVBoilerFound", NetworkTablesInterface::boilerFound());
		//frc::SmartDashboard::PutNumber("CVBoilerDistance",NetworkTablesInterface::getGearDistance());
		//frc::SmartDashboard::PutNumber("CVBoilerAltitude", NetworkTablesInterface::getGearAltitude());
		//frc::SmartDashboard::PutNumber("CVBoilerAzimuth", NetworkTablesInterface::getGearAzimuth());
		frc::LiveWindow::GetInstance()->Run();
	}

private:
	std::unique_ptr<frc::Command> autonomousCommand;
	frc::SendableChooser<frc::Command*> chooser;
};

START_ROBOT_CLASS(Robot)
