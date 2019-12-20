# drive2

An improved driving program for the vex claw bot. Written in c++.

## Files
*main.cpp* is used for automated actions and may be edited freely if you wish to use drive2 as an add-on to an automated program.

*robot-config.cpp* is the primary file used for interpreting and enacting driver inputs. It is typically auto-generated code which has minimal computation, but has been heavily edited for better driving.

## How to drive

| Control  | Function |
| - | - |
| 🡅 | move arm upwards |
| 🡇 | move arm downwards |
| 🡄 | open claw |
| 🡆 | close claw |
| axis 3 | acceleration |
| axis 1 | turning |
| L1 | additive turning |
| L2 | tank controls |
| R1 | braking |
| R2 | highspeed |
