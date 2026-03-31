#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep
#include "const.cpp"

using namespace pros;

// INITIALIZE CONSTANTS

// controller
Controller controller(E_CONTROLLER_MASTER);

// vision
Vision visionSensor(VIS_PORT);

vision_signature_s_t RED_BLOCK = Vision::signature_from_utility(RED_BLOCK_ID, 8897, 10143, 9520, -863, -129, -496, 3.000, 0);
vision_signature_s_t BLUE_BLOCK = Vision::signature_from_utility (BLUE_BLOCK_ID, -4197, -3603, -3900, 2015, 8441, 5228, 5.000, 0);

// vision_signature_s_t RED_BLOCK = Vision::signature_from_utility(RED_BLOCK_ID, 10225, 11583, -1849, -1327, -1588, -496, 3.000, 0);
// vision_signature_s_t BLUE_BLOCK = Vision::signature_from_utility (BLUE_BLOCK_ID, -4197, -3603, -3900, 2015, 8441, 5228, 5.000, 0);

// drive motor groups
MotorGroup leftMotors({LEFT_FRONT, LEFT_MIDDLE, LEFT_BACK}, MotorGearset::blue); // left motor group - ports 3 (reversed), 4, 5 (reversed)
MotorGroup rightMotors({RIGHT_FRONT, RIGHT_MIDDLE, RIGHT_BACK}, MotorGearset::blue); // right motor group - ports 6, 7, 9 (reversed)

// intake and outtake motors
MotorGroup intakeMotors({INT_CW, INT_CCW, INT_ASSIST}, MotorGearset::green);
Motor outTop(OUT_TOP, MotorGearset::green);  
Motor outBot(OUT_BOT, MotorGearset::green); 

// tracking wheels - NONE OF THIS IS ACTUALLY USED
// horizontal tracking wheel encoder. Rotation sensor, port 20, not reversed
Rotation horizontalEnc(20);
// vertical tracking wheel encoder. Rotation sensor, port 11, reversed
Rotation verticalEnc(-10);
// horizontal tracking wheel. 2.75" diameter, 5.75" offset, back of the robot (negative)
lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_275, -5.75);
// vertical tracking wheel. 2.75" diameter, 2.5" offset, left of the robot (negative)
lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_275, -2.5);
// Inertial Sensor on port 11
Imu imu(1);

// drivetrain settings
lemlib::Drivetrain drivetrain(&leftMotors, // left motor group
                              &rightMotors, // right motor group
                              10, // 10 inch track width
                              lemlib::Omniwheel::NEW_4, // using new 4" omnis
                              360, // drivetrain rpm is 360
                              2 // horizontal drift is 2. If we had traction wheels, it would have been 8
);

// lateral motion controller
lemlib::ControllerSettings linearController(10, // proportional gain (kP)
                                            0, // integral gain (kI)
                                            3, // derivative gain (kD)
                                            3, // anti windup
                                            1, // small error range, in inches
                                            100, // small error range timeout, in milliseconds
                                            3, // large error range, in inches
                                            500, // large error range timeout, in milliseconds
                                            20 // maximum acceleration (slew)
);

// angular motion controller
lemlib::ControllerSettings angularController(2, // proportional gain (kP)
                                             0, // integral gain (kI)
                                             10, // derivative gain (kD)
                                             3, // anti windup
                                             1, // small error range, in degrees
                                             100, // small error range timeout, in milliseconds
                                             3, // large error range, in degrees
                                             500, // large error range timeout, in milliseconds
                                             0 // maximum acceleration (slew)
);

// sensors for odometry
lemlib::OdomSensors sensors(&vertical, // vertical tracking wheel
                            nullptr, // vertical tracking wheel 2, set to nullptr as we don't have a second one
                            &horizontal, // horizontal tracking wheel
                            nullptr, // horizontal tracking wheel 2, set to nullptr as we don't have a second one
                            &imu // inertial sensor
);

// input curve for throttle input during driver control
lemlib::ExpoDriveCurve throttleCurve(3, // joystick deadband out of 127
                                     10, // minimum output where drivetrain will move out of 127
                                     1.019 // expo curve gain
);

// input curve for steer input during driver control
lemlib::ExpoDriveCurve steerCurve(3, // joystick deadband out of 127
                                  10, // minimum output where drivetrain will move out of 127
                                  1.019 // expo curve gain
);

// create the chassis
lemlib::Chassis chassis(drivetrain, linearController, angularController, sensors, &throttleCurve, &steerCurve);

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
    lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors
    leftMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
    rightMotors.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
}

/**
 * Runs while the robot is disabled
 */
void disabled() {}

/**
 * runs after initialize if the robot is connected to field control
 */
void competition_initialize() {}

// get a path used for pure pursuit
// this needs to be put outside a function
ASSET(example_txt); // '.' replaced with "_" to make c++ happy

/*
 * Runs during auto
 *
 * This is an example autonomous routine which demonstrates a lot of the features LemLib has to offer
 */
void autonomous() {
    // Move to x: 20 and y: 15, and face heading 90. Timeout set to 4000 ms
    // chassis.moveToPose(20, 15, 90, 4000);
    // // Move to x: 0 and y: 0 and face heading 270, going backwards. Timeout set to 4000ms
    // chassis.moveToPose(0, 0, 270, 4000, {.forwards = false});
    // // cancel the movement after it has traveled 10 inches
    // chassis.waitUntil(10);
    // chassis.cancelMotion();
    // // Turn to face the point x:45, y:-45. Timeout set to 1000
    // // dont turn faster than 60 (out of a maximum of 127)
    // chassis.turnToPoint(45, -45, 1000, {.maxSpeed = 60});
    // // Turn to face a direction of 90º. Timeout set to 1000
    // // will always be faster than 100 (out of a maximum of 127)
    // // also force it to turn clockwise, the long way around
    // chassis.turnToHeading(90, 1000, {.direction = AngularDirection::CW_CLOCKWISE, .minSpeed = 100});
    // // Follow the path in path.txt. Lookahead at 15, Timeout set to 4000
    // // following the path with the back of the robot (forwards = false)
    // // see line 116 to see how to define a path
    // chassis.follow(example_txt, 15, 4000, false);
    // // wait until the chassis has traveled 10 inches. Otherwise the code directly after
    // // the movement will run immediately
    // // Unless its another movement, in which case it will wait
    // chassis.waitUntil(10);
    // // wait until the movement is done
    // chassis.waitUntilDone();
}

// drive
void drive() {
    // get amount to move from joysticks
    int rightX = controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
    int leftY = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);

    // if (controller.get_digital(E_CONTROLLER_DIGITAL_R1)) {
    //     rightX /= 2;
    //     leftY /= 2;
    // }

    // move the chassis with curvature drive
    chassis.arcade(rightX, leftY);
}

// intake based off of color
void intake() {
    // if R2 is pressed, spin slow
    if (controller.get_digital(E_CONTROLLER_DIGITAL_R2)) {
        intakeMotors.move(FAST);

        // get biggest object (doesn't work as expected)
        vision_object_s_t biggestObj = visionSensor.get_by_size(LARGEST);

        // if a color is sensed, spin faster in the correct direction
        // if (biggestObj.signature == VISION_OBJECT_ERR_SIG) {
        //     intakeMotors.move(FAST);
        // }
        // else 
        if (biggestObj.signature == OPP_ID) {
            intakeMotors.move(-FAST);
        }
        else if (biggestObj.signature == TEAM_ID) {
            intakeMotors.move(FAST);
        }
    }
    else if (controller.get_digital(E_CONTROLLER_DIGITAL_R1)) {
        intakeMotors.move(-FAST);
    }
    else {
        intakeMotors.move(HALT);
    }

    // if (controller.get_digital(E_CONTROLLER_DIGITAL_R2)) {
    //     intakeMotors.move(FAST);
    // }
    // else {
    //     intakeMotors.move(HALT);
    // }
}

// Does outake based off of button press
void outake() {
    if (controller.get_digital(E_CONTROLLER_DIGITAL_A)) {
        outBot.move(-FAST);
    }
    else if (controller.get_digital(E_CONTROLLER_DIGITAL_L1) && controller.get_digital(E_CONTROLLER_DIGITAL_L2)) {
        outTop.move(FAST);
        outBot.move(FAST);
    }
    else if (controller.get_digital(E_CONTROLLER_DIGITAL_L1)) {
        outTop.move(-FAST);
        outBot.move(FAST);
    }
    else if (controller.get_digital(E_CONTROLLER_DIGITAL_L2)) {
        outTop.move(FAST);
        outBot.move(FAST);
    }
    else {
        outTop.move(HALT);
        outBot.move(HALT);
    }
}

// 1 2 3 4 5 6 7 8 9 0

/*
 * Runs in driver control
 */
void opcontrol() {
    while (true) {
        drive();
        intake();
        outake();

        // delay to save resources
        delay(10);
    }
}
