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
bool stopLeft = true;
bool stopRight = true;

bool brakable = false;

// define a task that will handle monitoring inputs from Controller1
int rc_auto_loop_callback_Controller1() {
  Brain.Screen.render(true, false);

  Claw.setStopping(hold);
  Arm.setStopping(hold);
  Arm.setVelocity(20, percent);

  // process the controller input every 20 milliseconds
  // update the motors based on the input values
  while(true) {
    // dynamic braking types
    if(Controller1.ButtonR1.pressing())
    {
      Drivetrain.setStopping(brake);
      brakable = true;
    }
    else if(Controller1.ButtonR2.pressing())
    {
      Drivetrain.setStopping(hold);
      brakable = true;
    }
    else
    {
      Drivetrain.setStopping(coast);
      brakable = false;
    }


    // claw manipulation
    if(Controller1.ButtonRight.pressing() && Controller1.ButtonLeft.pressing())
    {
      Claw.stop();
    }
    else if(Controller1.ButtonRight.pressing())
    {
      Claw.spin(forward);
    }
    else if(Controller1.ButtonLeft.pressing())
    {
      Claw.spin(reverse);
    }
    else {
      Claw.stop();    
    }

    // arm manipulation
    if(Controller1.ButtonUp.pressing() && Controller1.ButtonDown.pressing())
    {
      Arm.stop();
    }
    else if(Controller1.ButtonUp.pressing())
    {
      Arm.spin(forward);
    }
    else if(Controller1.ButtonDown.pressing())
    {
      Arm.spin(reverse);
    }
    else {
      Arm.stop();
    }

    int leftWheelSpeed = 0;
    int rightWheelSpeed = 0;
    // calculate the drivetrain motor velocities from the controller joystick axies
    int x = Controller1.Axis3.position();
    if(x > 0){
      x -= 1;
    } else if(x < 0) {
      x += 1;
    }
    x *= 0.8;

    leftWheelSpeed = x;
    rightWheelSpeed = x;
  

    // left side 
    float turnAxis = Controller1.Axis1.position();
    float turnAmount = fabsf(turnAxis) * 0.4;
    if(turnAxis > -0.3 && turnAxis < 0.3)
    {} // do nothing
    else
    {
      int combinedSpeed = leftWheelSpeed + rightWheelSpeed;
      // still
      if(combinedSpeed == 0)
      {
        // max turning
        if(turnAmount > 35)
        {
          // turning left
          if(turnAxis > 0) {
            rightWheelSpeed = -turnAmount * 0.75;
            leftWheelSpeed = turnAmount * 0.75;
          }
          else {
            rightWheelSpeed = turnAmount * 0.75;
            leftWheelSpeed = -turnAmount * 0.75;
          }
        }
        else {
          if(turnAxis < 0) {
            rightWheelSpeed = turnAmount;
          }
          if(turnAxis > 0) {
            leftWheelSpeed = turnAmount;
          }
        }
      }
      // going forwards
      else if(combinedSpeed > 0)
      {
        // if turning left
        if(turnAxis < 0) {
          rightWheelSpeed += turnAmount;
        }
        else {
          leftWheelSpeed += turnAmount;
        }
      }
      // going backwards
      else if(combinedSpeed < 0)
      {
        // if turning left
        if(turnAxis < 0) {
          rightWheelSpeed += turnAmount;
        }
        else {
          leftWheelSpeed += turnAmount;
        }
      }
     }
    
    
    // TODO: rewrite drivetrain backend
    // check if the value is inside of the deadband range
    if(leftWheelSpeed < 1 && leftWheelSpeed > -1) {
      // check if the left motor has already been stopped
      if (stopLeft) {
        // stop the left drive motor
        if(brakable) {
          LeftDriveSmart.stop();
        }
        else {
          LeftDriveSmart.setVelocity(0, percent);        
        }
        // tell the code that the left motor has been stopped
        stopLeft = false;
      }
    } else {
      // reset the toggle so that the deadband code knows to stop the left motor next time the input is in the deadband range
      stopLeft = true;
    }
    // check if the value is inside of the deadband range
    if (rightWheelSpeed < 1 && rightWheelSpeed > -1) {
      // check if the right motor has already been stopped
      if (stopRight) {
        // stop the right drive motor
        if(brakable) {
          RightDriveSmart.stop();
        }
        else {
          RightDriveSmart.setVelocity(0, percent);
        }
        // tell the code that the right motor has been stopped
        stopRight = false;
      }
    } else {
      // reset the toggle so that the deadband code knows to stop the right motor next time the input is in the deadband range
      stopRight = true;
    }
    // only tell the left drive motor to spin if the values are not in the deadband range
    if (stopLeft) {
      LeftDriveSmart.setVelocity(leftWheelSpeed, percent);
      LeftDriveSmart.spin(forward);
    }
    // only tell the right drive motor to spin if the values are not in the deadband range
    if (stopRight) {
      RightDriveSmart.setVelocity(rightWheelSpeed, percent);
      RightDriveSmart.spin(forward);
    }

    // tick speed
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
