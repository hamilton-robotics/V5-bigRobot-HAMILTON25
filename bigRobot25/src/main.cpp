#include "main.h"
#include "lemlib/api.hpp" // IWYU pragma: keep

using namespace pros;

// TEST
// controller
Controller controller(E_CONTROLLER_MASTER);

const int RED_BLOCK_IND = 1;
const int BLUE_BLOCK_IND = 2;
const int NUM_VISION_OBJECTS = 4;

Vision visionSensor(10);

// RED_BLOCK: (1, 8897, 10143, 9520, -863, -129, -496, 8.400, 0);
// BLUE_BLOCK: (2, -4197, -3603, -3900, 2015, 8441, 5228, 4.800, 0);
vision_signature_s_t RED_BLOCK = Vision::signature_from_utility(RED_BLOCK_IND, 8897, 10143, 9520, -863, -129, -496, 8.400, 0);
vision_signature_s_t BLUE_BLOCK = Vision::signature_from_utility(BLUE_BLOCK_IND, -4197, -3603, -3900, 2015, 8441, 5228, 5.000, 0);

//Vision RED_BLOCK (1, 8897, 10143, 9520, -863, -129, -496, 8.400, 0);
//Vision::signature BLUE_BLOCK (2, -4197, -3603, -3900, 2015, 8441, 5228, 5.000, 0);
// Vision::signature SIG_3 (3, 0, 0, 0, 0, 0, 0, 3.000, 0);
// vision::signature SIG_4 (4, 0, 0, 0, 0, 0, 0, 3.000, 0);
// vision::signature SIG_5 (5, 0, 0, 0, 0, 0, 0, 3.000, 0);
// vision::signature SIG_6 (6, 0, 0, 0, 0, 0, 0, 3.000, 0);
// vision::signature SIG_7 (7, 0, 0, 0, 0, 0, 0, 3.000, 0);
// vex::vision vision1 ( vex::PORT1, 50, RED_BLOCK, BLUE_BLOCK, SIG_3, SIG_4, SIG_5, SIG_6, SIG_7 );




// motor groups
MotorGroup leftMotors({12, 19},
                            MotorGearset::blue); // left motor group - ports 3 (reversed), 4, 5 (reversed)
MotorGroup rightMotors({13, 11}, MotorGearset::blue); // right motor group - ports 6, 7, 9 (reversed)

// intake testing
MotorGroup intake({-2, 3}, MotorGearset::blue);

//Motor intake_right(3);

// Inertial Sensor on port 10
Imu imu(10);

// tracking wheels
// horizontal tracking wheel encoder. Rotation sensor, port 20, not reversed
Rotation horizontalEnc(20);
// vertical tracking wheel encoder. Rotation sensor, port 11, reversed
Rotation verticalEnc(-11);
// horizontal tracking wheel. 2.75" diameter, 5.75" offset, back of the robot (negative)
lemlib::TrackingWheel horizontal(&horizontalEnc, lemlib::Omniwheel::NEW_275, -5.75);
// vertical tracking wheel. 2.75" diameter, 2.5" offset, left of the robot (negative)
lemlib::TrackingWheel vertical(&verticalEnc, lemlib::Omniwheel::NEW_275, -2.5);

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
    visionSensor.clear_led();
    lcd::initialize(); // initialize brain screen
    chassis.calibrate(); // calibrate sensors

    // the default rate is 50. however, if you need to change the rate, you
    // can do the following.
    // lemlib::bufferedStdout().setRate(...);
    // If you use bluetooth or a wired connection, you will want to have a rate of 10ms

    // for more information on how the formatting for the loggers
    // works, refer to the fmtlib docs

    // thread to for brain screen and position logging
    Task screenTask([&]() {
        while (true) {
            // print robot location to the brain screen
            lcd::print(0, "X: %f", chassis.getPose().x); // x
            lcd::print(1, "Y: %f", chassis.getPose().y); // y
            lcd::print(2, "Theta: %f", chassis.getPose().theta); // heading
            // log position telemetry
            lemlib::telemetrySink()->info("Chassis pose: {}", chassis.getPose());
            // delay to save resources
            delay(50);
        }
    });
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

/**
 * Runs during auto
 *
 * This is an example autonomous routine which demonstrates a lot of the features LemLib has to offer
 */
void autonomous() {
    // Move to x: 20 and y: 15, and face heading 90. Timeout set to 4000 ms
    chassis.moveToPose(20, 15, 90, 4000);
    // Move to x: 0 and y: 0 and face heading 270, going backwards. Timeout set to 4000ms
    chassis.moveToPose(0, 0, 270, 4000, {.forwards = false});
    // cancel the movement after it has traveled 10 inches
    chassis.waitUntil(10);
    chassis.cancelMotion();
    // Turn to face the point x:45, y:-45. Timeout set to 1000
    // dont turn faster than 60 (out of a maximum of 127)
    chassis.turnToPoint(45, -45, 1000, {.maxSpeed = 60});
    // Turn to face a direction of 90º. Timeout set to 1000
    // will always be faster than 100 (out of a maximum of 127)
    // also force it to turn clockwise, the long way around
    chassis.turnToHeading(90, 1000, {.direction = AngularDirection::CW_CLOCKWISE, .minSpeed = 100});
    // Follow the path in path.txt. Lookahead at 15, Timeout set to 4000
    // following the path with the back of the robot (forwards = false)
    // see line 116 to see how to define a path
    chassis.follow(example_txt, 15, 4000, false);
    // wait until the chassis has traveled 10 inches. Otherwise the code directly after
    // the movement will run immediately
    // Unless its another movement, in which case it will wait
    chassis.waitUntil(10);
    lcd::print(4, "Traveled 10 inches during pure pursuit!");
    // wait until the movement is done
    chassis.waitUntilDone();
    lcd::print(4, "pure pursuit finished!");
}

/**
 * Runs in driver control
 */

void opcontrol() {
    // controller
    // loop to continuously update motors

    visionSensor.set_signature(RED_BLOCK_IND, &RED_BLOCK);
    visionSensor.set_signature(BLUE_BLOCK_IND, &BLUE_BLOCK);

    vision_object_s_t object_arr[NUM_VISION_OBJECTS];
    vision_color_code_t comp = visionSensor.create_color_code(RED_BLOCK_IND, BLUE_BLOCK_IND);

     
    while (true) {
        //get joystick positions?
        vision_object_s_t rtn = visionSensor.get_by_sig(0, RED_BLOCK_IND);
        visionSensor.read_by_sig(0, RED_BLOCK_IND, NUM_VISION_OBJECTS, object_arr);
        if (rtn.signature == RED_BLOCK_IND) {
            //test.move(100);
            lcd::print (3, "red found");
        }

        lcd::print (4, "RTN sig: %f", rtn.signature);
        lcd::print (5, "Red sig: %f", RED_BLOCK_IND);

        int rightX = controller.get_analog(E_CONTROLLER_ANALOG_RIGHT_X);
        int leftY = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_Y);
        int leftX = controller.get_analog(E_CONTROLLER_ANALOG_LEFT_X);

        // move the chassis with curvature drive
        chassis.arcade(rightX, leftY);

        // intake_left.move(-leftX);
        // intake_right.move(leftX);

        intake.move(leftX);

        //test
        //test.move(100);

        // delay to save resources
        delay(10);
    }
}

