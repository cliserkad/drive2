#include "vex.h"

using namespace vex;
using signature = vision::signature;
using code = vision::code;

// A global instance of brain used for printing to the V5 Brain screen
brain  Brain;

// VEXcode device constructors
bumper BackBumper = bumper(Brain.ThreeWirePort.B);
bumper ArmBumper = bumper(Brain.ThreeWirePort.H);
motor LeftDriveSmart = motor(PORT10, ratio18_1, false);
motor RightDriveSmart = motor(PORT1, ratio18_1, true);
motor Claw = motor(PORT3, ratio18_1, false);
motor Arm = motor(PORT8, ratio18_1, false);
drivetrain Drivetrain = drivetrain(LeftDriveSmart, RightDriveSmart, 319.19, 295, 130, mm, 1);
controller Controller1 = controller(primary);

// VEXcode generated functions
// define variables used for controlling motors based on controller inputs
bool DrivetrainLNeedsToBeStopped_Controller1 = true;
bool DrivetrainRNeedsToBeStopped_Controller1 = true;

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_callback_Controller1() {
  Brain.Screen.render(true, false);

  Drivetrain.setStopping(brake);
  Claw.setStopping(hold);
  Arm.setStopping(hold);
  Arm.setVelocity(20, percent);

  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while(true) {

    // do claw manipulation
    if(Controller1.ButtonL1.pressing() && Controller1.ButtonL2.pressing())
    {
      Claw.stop();
    }
    else if(Controller1.ButtonL1.pressing())
    {
      Claw.spin(forward);
    }
    else if(Controller1.ButtonL2.pressing())
    {
      Claw.spin(reverse);
    }
    else {
      Claw.stop();    
    }

    // do arm manipulation
    if(Controller1.ButtonR1.pressing() && Controller1.ButtonR2.pressing())
    {
      Arm.stop();
    }
    else if(Controller1.ButtonR1.pressing())
    {
      Arm.spin(forward);
    }
    else if(Controller1.ButtonR2.pressing())
    {
      Arm.spin(reverse);
    }
    else {
      Arm.stop();
    }



    // calculate the drivetrain motor velocities from the controller joystick axies
    // left = Axis3
    // right = Axis2
    int x = Controller1.Axis3.position();
    if(x > 0){
      x -= 1;
    } else if(x < 0) {
      x += 1;
    }
    x *= 0.8;

    // x = (0.2 * x) + (0.8 * (x * x));
    int drivetrainLeftSideSpeed = x;
    int drivetrainRightSideSpeed = x;

    // left side 
    float turnAxis = Controller1.Axis4.position();
    if(turnAxis > -1 && turnAxis < 1)
    {} // do nothing
    else
    {
      turnAxis *= 0.4;

      int combinedSpeed = drivetrainLeftSideSpeed + drivetrainRightSideSpeed;
      
      // still
      if(combinedSpeed == 0)
      {
        drivetrainLeftSideSpeed = turnAxis * 0.5;
        drivetrainRightSideSpeed = turnAxis * -0.5;
      }

      // going forwards
      if(combinedSpeed > 0)
      {
        drivetrainLeftSideSpeed += turnAxis;
      }

      // going backwards
      if(combinedSpeed < 0)
      {
        drivetrainRightSideSpeed -= turnAxis;
      }

     }
    
    
    // check if the value is inside of the deadband range
    if (drivetrainLeftSideSpeed < 1 && drivetrainLeftSideSpeed > -1) {
      // check if the left motor has already been stopped
      if (DrivetrainLNeedsToBeStopped_Controller1) {
        // stop the left drive motor
        LeftDriveSmart.stop();
        // tell the code that the left motor has been stopped
        DrivetrainLNeedsToBeStopped_Controller1 = false;
      }
    } else {
      // reset the toggle so that the deadband code knows to stop the left motor next time the input is in the deadband range
      DrivetrainLNeedsToBeStopped_Controller1 = true;
    }
    // check if the value is inside of the deadband range
    if (drivetrainRightSideSpeed < 1 && drivetrainRightSideSpeed > -1) {
      // check if the right motor has already been stopped
      if (DrivetrainRNeedsToBeStopped_Controller1) {
        // stop the right drive motor
        RightDriveSmart.stop();
        // tell the code that the right motor has been stopped
        DrivetrainRNeedsToBeStopped_Controller1 = false;
      }
    } else {
      // reset the toggle so that the deadband code knows to stop the right motor next time the input is in the deadband range
      DrivetrainRNeedsToBeStopped_Controller1 = true;
    }
    // only tell the left drive motor to spin if the values are not in the deadband range
    if (DrivetrainLNeedsToBeStopped_Controller1) {
      LeftDriveSmart.setVelocity(drivetrainLeftSideSpeed, percent);
      LeftDriveSmart.spin(forward);
    }
    // only tell the right drive motor to spin if the values are not in the deadband range
    if (DrivetrainRNeedsToBeStopped_Controller1) {
      RightDriveSmart.setVelocity(drivetrainRightSideSpeed, percent);
      RightDriveSmart.spin(forward);
    }
    // wait before repeating the process
    wait(1, msec);
  }
  return 0;
}

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Text.
 * 
 * This should be called at the start of your int main function.
 */
void vexcodeInit( void ) {
  task rc_auto_loop_task_Controller1(rc_auto_loop_callback_Controller1);
}