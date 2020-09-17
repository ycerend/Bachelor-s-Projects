/*Project 10
  5/3/2018
  Group 7
  Hannah Phillips, Travis Barnett and Yagmur Dardagan

  Coder: Yagmur Dardagan
*/

#include "project.h"
#include "ImuUtils.h"
#include "PeriodicAction.h"
#include <OpticalFlowCamera.h>

// Promising for implementing fsm_step function later in the code
void fsm_step();

//Promising for implementing display_heading_velocity later in the code
void  display_heading_velocity(int16_t velocity);

//Promising for implementing display_heading later in the code
void display_heading(float heading);

// Promise that we will implement this function later
void control_step();

// Promise that we will implement this function later
void sensor_step();

// Promise that we will implemetn this function later.
void camera_step();

// Create a task that will be executed once per 5 ms.
PeriodicAction camera_task(5, camera_step);

// Create a task that will be executed once per 10 ms
PeriodicAction control_task(10, control_step);

// Create a task that will be executed once per 5 ms
PeriodicAction sensor_task(5, sensor_step);

// Creating a task that will be executed once per 50 ms
PeriodicAction fsm_task(50, fsm_step);

// Promise that we will implement this function later
void report_step();

// Create a task that will be executed once per 250 ms
PeriodicAction report_task(250, report_step);

// Gains to be used for reverse thrust
const float FAN_GAIN[] = { 1.6, 1.8, 1.0};

//initializing pins of the fans as global variable
const int CENTER_PWM = 29;
const int RIGHT_PWM = 3;
const int LEFT_PWM = 2;
const int BACK_PWM = 4;

// Total number of cameras
const int NUM_CAMERAS = 3;

// Select pins for the 3 cameras
const uint8_t CAMERA_SELECT[NUM_CAMERAS] = {26, 27, 28};

// Common camera reset pin
const uint8_t RESET_PIN = 25;

//magnitudes array for storing thrust magnitude of lateral fans
float magnitudes[] = { 0, 0 , 0 };

//Initialize a global variable that will give us our desired heading.
float heading_goal;

//Initialize global variables for the filtered velocity and the velocity goal
float velocity_filtered[3] = {0, 0, 0}; // x_dot, y_dot, theta_dot
float velocity_goal[2] = {0, 0};   // x_dot, y_dot

// Camera interface object
OpticalFlowCamera cameras(RESET_PIN);

// Accumulated slip matrix for the x direction.
int32_t adx[3] = {0, 0, 0};

// Accumulated slip matrix for the x direction.
int32_t ady[3] = {0, 0, 0};

// Motion matrix for X, Y, and Theta. motion[0]=X, motion[1] = Y, and motion[2]=theta.
float motion[3] = {0, 0, 0};

//array for recording measurements from distance sensors
float distance_sensors[2];
// Define analog pin mapping for yellow and red
const int analogRightYellow = 37;
const int analogLeftRed = 38;


//to store orientation error
float ret;
//to store current orientation
float oldOrientation;
//to store number of turns according to the orientation of hoverboard
int countForTurn;
//to record state of B
int stateOfB ;
//to store which way hoverboard will turn
bool rightOrLeft = true;
//to store to decide to go to phase 2
bool phase2 = false;

/*
   setup will be run once and will prepare to run the arduino software.
*/
void setup() {
  // Initialize the USB serial connection
  Serial.begin(115200);

  // Initialize the IMU
  imu_setup();

  // IMU mag bias from the calibration
  IMU.magbias[0] = 52.348995;
  IMU.magbias[1] = 291.341675;
  IMU.magbias[2] = -120.397156;

  //PROJECT SPECIFIC CODE:
  //initializing switch A
  PORTD_PCR1 = PORT_PCR_MUX(0x1);

  //nitializing switch B
  PORTC_PCR8 = PORT_PCR_MUX(0x1);

  //initializing i/o pins of bar LED
  PORTB_PCR0 = PORT_PCR_MUX(0x1);
  PORTB_PCR1 = PORT_PCR_MUX(0x1);
  PORTB_PCR10 = PORT_PCR_MUX(0x1);
  PORTB_PCR11 = PORT_PCR_MUX(0x1);
  PORTB_PCR19 = PORT_PCR_MUX(0x1);

  //Initializing i/o pins for heading LED's
  PORTC_PCR0 = PORT_PCR_MUX(0x1);
  PORTC_PCR1 = PORT_PCR_MUX(0x1);
  PORTC_PCR2 = PORT_PCR_MUX(0x1);
  PORTC_PCR9 = PORT_PCR_MUX(0x1);

  //initializing input pins of left fan
  PORTE_PCR24 = PORT_PCR_MUX(0x1);
  PORTE_PCR25 = PORT_PCR_MUX(0x1);

  //initializing input pins of right fan
  PORTD_PCR6 = PORT_PCR_MUX(0x1);
  PORTD_PCR5 = PORT_PCR_MUX(0x1);

  //initializing input pins of back fan
  PORTD_PCR4 = PORT_PCR_MUX(0x1);
  PORTD_PCR2 = PORT_PCR_MUX(0x1);

  // Configuring port E bits 24 & 25 and port D bits 2 & 4 & 5 & 6 to be outputs
  GPIOE_PDDR = 0x3000000;
  GPIOD_PDDR = 0x74;

  //Configuring port B bits 0, 1, 10, 11, and 19 to be outputs for bar LED
  GPIOB_PDDR = 0x80C03;

  //Configuring port C bits 0, 1, 2, and 9 to be outputs for heading LED's
  GPIOC_PDDR = 0x207;

  // Initialize camera 1
  int ret = cameras.addCamera(CAMERA_SELECT[0]);

  //Check if camera 1 is functioning. If not print an error code
  // and leave the program.
  if ( ret != 0)
  {

    // Print Error
    Serial.println("Error");

    // Return zero
    return;
  }

  // Initialize camera 2
  int ret2 = cameras.addCamera(CAMERA_SELECT[1]);

  //Check if camera 2 is functioning. If not print an error code
  // and leave the program.
  if ( ret2 != 0)
  {

    // Print Error
    Serial.println("Error2");

    // Return zero
    return;
  }


  // Initialize camera 3
  int ret3 = cameras.addCamera(CAMERA_SELECT[2]);

  //Check if camera 3 is functioning. If not print an error code
  // and leave the program.
  if ( ret3 != 0)
  {

    // Print Error
    Serial.println("Error3");

    // Return zero
    exit(0);
  }


}
/*The method read_distance takes the input, side,  for the sensors.
   Then it checks which sensor is being read, checks the value, and returns the distance in cm.
  If neıther DISTANCE_LEFT or DISTANCE_RIGHT ıs evaluated as true zero ıs returned.
  Input= DistanceSensor side
  Output= float in cm
*/

float read_distance(DistanceSensor side) {
  //if the DistanceSensor side is DISTANCE_LEFT, run this code.
  if ( side == DISTANCE_FRONT_RED) {
    //reads the distance from sensor
    float valueForRed = analogRead(analogLeftRed);
    //if the distance is greater than 80 cm , run this code.
    if (valueForRed < 178) {
      //If the distance is greater than 80 cm, the value of the sensor must be equal to 80

      return 80;
    }
    //calculates the distance in cm
    float distForLeftRed = 5867.2 / (valueForRed - 105.02);
    //returns the distance in cm
    return distForLeftRed;
  }

  //if the DistanceSensor side is DISTANCE_RIGHT, run this code.
  if ( side == DISTANCE_RIGHT_YELLOW) {
    //reads the distance from sensor
    float valueForYellow = analogRead(analogRightYellow);
    //if the distance is greater than 80 cm , run this code.
    if (valueForYellow < 137) {
      //If the distance is greater than 80 cm, the value of the sensor must be equal to 80
      return 80;
    }
    //calculates the distance in cm
    float distForRightYellow = 5847.2 / (valueForYellow - 64.425);

    //returns the distance in cm
    return distForRightYellow;

  }
  //If neıther DISTANCE_LEFT or DISTANCE_RIGHT ıs evaluated as true zero ıs returned.
  return 0;

}


/*
   sensor_step will query the sensors, check for a character and display the velocity

   Input:
   Output: Will change the LED's
*/
void sensor_step()
{
  //Update IMU
  imu_update();

  //Check if a char has been outputed.
  if (Serial.available() > 0)
  {
    //Declare a variable to hold the character.
    char character = Serial.read();

    //If the character received is a +, add 20 degrees to the heading goal
    if (character == '+')
    {
      heading_goal = IMU.yaw + 20;
    }
    //If the character received is a -, subtract 20 degrees from the heading goal
    if (character == '-')
    {
      heading_goal = IMU.yaw - 20;
    }

    /*Check if the char is a C. If so call the function
       imu_calibrate_magbias(), to recalibrate the hoverboard.
    */
    if (character == 'c' || character == 'C')
    {
      //Call the imu calibration function
      imu_calibrate_magbias();
    }

    /*
        Check if the char is g. If so set the heading goal as the current orientation.
    */
    else if (character == 'g' || character == 'G')
    {
      // Set the heading goal at the current orientation
      heading_goal = IMU.yaw;
    }
  }

  //Display the rotational velocity on the LED bar
  display_heading_velocity(IMU.gz);

  //calculating distances for both sensors in cm
  distance_sensors[0] = read_distance(DISTANCE_FRONT_RED);
  distance_sensors[1] = read_distance(DISTANCE_RIGHT_YELLOW);


}

/*
  Function accepts input as a desired acceleration, translates the desired acceleration
  to an appropriat thrust level for lateral fans, and changes the thrust state of the fans.

  input: ddx the desired location in the x direction
  input: ddy the desired location in the y direction
  input: ddtheta the desired angle with correction factor

  Ouput: Change the magnitudes, and thrust states of the lateral fans,
*/
void set_hovercraft_acceleration(float ddx, float ddy, float ddtheta)
{
  //Set the left fan magnitude
  magnitudes[0] = 0.577 * (ddx) - 0.3333 * (ddy) - 2.4416 * (ddtheta);
  //Set the back fan magnitude
  magnitudes[1] = 0 * (ddx) - 0.67778 * (ddy) + 2.4416 * (ddtheta);
  //Set the right fan magnitude
  magnitudes[2] = 0.577 * (ddx) + 0.333 * (ddy) + 2.4416 * (ddtheta);
  //Set the calculated magnitudes
  set_lateral_fan_magnitudes(magnitudes);
}

/*
  This method is for damping rotation of the hover craft. It sets desired location
  data and sends it to set_hovercraft_aceleration.

  Input: Nothing
  Output: Sets the desired location and theta and calls set_hovercraft_aceleration.
*/
void control_step() {
  //Starting Kv value is 1 (Guess)
  float Kv = -0.7;
  float Kp = 1.8;
  float Klv = -1500.0;

  //Declare two variables that will hold the values for the deadband and saturation range
  float deadband = 2;
  float saturation = 35;

  // Declare a varialble ret to hold a value from the function compute_rotation_error
  ret = compute_rotation_error(heading_goal, IMU.yaw);
  //Display the roational error to the heading LED's
  display_heading(ret);

  // Declare the variable modified error to hold a value from the function deadband_and_saturation
  float modified_error = deadband_and_saturation(ret, deadband, saturation);

  //Set the ddx and ddy values as a function of velocity goal and velocity filtered
  float ddx = Klv * (velocity_goal[0] - velocity_filtered[0]);
  float ddy = Klv * (velocity_goal[1] - velocity_filtered[1]);
  //Sum together proportional error and derivative
  float ddtheta = (Kp * modified_error) + (-Kv * IMU.gz);
  //Send location values to hovercraft acceleration method
  set_hovercraft_acceleration(ddx, ddy, ddtheta);
}

/*
  Clip ensures that magnitude values are not too high or too low. If they are, they are
  set and returned as the maximum or minimum value depending on case.

  Input: value A magnitude value for a fan
  Input: min_value The lowest value the magnitude can be
  Input: max_value The highest value the magnitude can be
  Ouput: float value that has been adjusted if out of bounds, or original
  value if in bounds.
*/
int16_t clip(float value, float min_value, float max_value) {
  //if the value is smaller than min_value, run this code
  if (value < min_value) {
    //returns min_value if the value is smaller than min_value
    return min_value;
  }
  //if the value is bigger than max_value, run this code
  if (value > max_value) {
    //returns max_value if the value is bigger than max_value
    return max_value;
  }
  //if value is between min_value and max_value, returns value itself
  return value;
}

/* The method set_lift_fan_magnitude sets the thrust magnitude for the lift fan and ensures that magnitude falls within the range of 0... 178.
   If the value does not fit within the range, then it clips to this range via calling clip( value, min_value, max_value).

   Input: float magnitude The number that corrseponds to the magnitude sent to the motor for lift.
   Output: CLips the magnitudes for the central fan
*/
void set_lift_fan_magnitude(float magnitude) {
  //Clip the magnitude of the fan
  magnitude = clip(magnitude, 0, 163);
  //sets the thrust magnitude for the lift fan
  analogWrite(CENTER_PWM, magnitude);

}

/*The method set_lateral_fan_magnitudes sets the thrust magnitude for the lateral fans
   (the order in the array is LEFT, BACK, RIGHT) and ensures that each of the input magnitudes
   fall within the range of -127... 127. If any do not, then the offending value clips to this
   range via calling clip( value, min_value, max_value).

   Input = float magnitudes[3] An array of the thrust magnitudes for each motor. [Left, Back, Right]
   Output: Sets the magnitude of the fans, and turns the approproiate fans on.
*/
void set_lateral_fan_magnitudes(float magnitudes[3]) {
  //to ensure that each of the input magnitudes fall within the range of -127... 127 and arranges digital pins for the lateral fans
  for (int i = 0; i <= 2; i++) {
    //call clip method to ensure proper magnitude ranges
    magnitudes[i] = clip(magnitudes[i], -127, 127);

    //If a negative value is specified for fan i, run this code
    if (magnitudes[i] < 0) {
      //multiplying by FAN_GAIN[i] to achieve approximately the same thrust in the positive and negative directions
      magnitudes[i] = magnitudes[i] * FAN_GAIN[i];
    }
    //if it is the left fan, run this code
    if (i == 0) {
      //if the direction is positive for left fan, run this code
      if (magnitudes[0] > 0) {
        //configuring outputs as E24 is 1 and E25 is 0
        GPIOE_PDOR = (GPIOE_PDOR & (~0x3000000)) | 0x1000000;
      }
      else {
        magnitudes[0] = magnitudes[0] * -1;
        //configuring outputs as E24 is 0 and E25 is 1
        GPIOE_PDOR = (GPIOE_PDOR & (~0x3000000)) | 0x2000000;
      }

    }
    //if it is the back fan, run this code
    if (i == 1) {
      if (magnitudes[1] > 0) {
        //configuring outputs as D4 is 1 and D2 is 0
        GPIOD_PDOR = (GPIOD_PDOR & (~0x14)) | 0x10;
      }
      else {
        magnitudes[1] = magnitudes[1] * -1;
        //configuring outputs as D2 is 1 and D4 is 0
        GPIOD_PDOR = (GPIOD_PDOR & (~0x14)) | 0x4;
      }

    }
    //if it is the right fan, run this code
    if (i == 2) {
      if (magnitudes[2] > 0) {
        //configuring outputs as D6 is 1 and D5 is 0
        GPIOD_PDOR = (GPIOD_PDOR & (~0x60)) | 0x40;
      }
      else {
        magnitudes[2] = magnitudes[2] * -1;
        //configuring outputs as D5 is 1 and D6 is 0
        GPIOD_PDOR = (GPIOD_PDOR & (~0x60)) | 0x20;
      }

    }

  }

  //sets the thrust magnitude for the lateral fans
  analogWrite(LEFT_PWM, magnitudes[0]);
  analogWrite(BACK_PWM, magnitudes[1]);
  analogWrite(RIGHT_PWM, magnitudes[2]);

}
/*
   This method contains every finite machine state and makes calls to adjust thrust.

   Input: Nothing
   Output: Adjustment of the speed of the lift fan, and of the velocity goal based upon the state.
*/
void fsm_step()
{

  //initializing state
  static State state = FSM_START_P10;
  //temporary variable for magnitudes
  static float ramping_up = 0.0;
  //increment rate
  const float RAMP_INCREMENT_P5 = 1.78;
  //implementing states and their functions
  switch (state) {


    //initial state
    case FSM_START_P10:
      //record the first orientation as goal orientation
      heading_goal = IMU.yaw;
      //initializing phase2 variable
      phase2 = false;
      //if switch A is pressed
      if (GPIOD_PDIR & 0x2)
      {
        //if the orientation is around 90 degrees,run this code
        if ((heading_goal < 135) && (heading_goal > 45))
        {
          //if the orientation is around 90 degrees,initialize countForTurn to 3
          countForTurn = 3;

        }
        //if not run this code
        else
        {
          //if the orientation is not around 90 degrees,initialize countForTurn to 2
          countForTurn = 2;

        }
        //if the switch B is pressed,run this code
        if (GPIOC_PDIR & 0x100) {
          //initialize stateOfB to 1 to indicate switch B is pressed
          stateOfB = 1;
        }
        //if the switch B is not pressed,run this code
        else {
          //initialize stateOfB to 0 to indicate switch B is not pressed
          stateOfB = 0;
        }
        //set new state to ramp up
        state = RAMP_UP_CENTRAL_FAN_P10;
        
      }
      break;

    ////state for ramping up the middle fan to a point where the craft begins to turn
    case RAMP_UP_CENTRAL_FAN_P10:
      //if the orientation error is bigger than 5, run this code
      if (abs(ret) > 5) {
        //set state STATE_MOVE_FORWARD_P9, after done with ramping up
        state = MOVE_FORWARD_P10;
      }
      //if orientation error is smaller than 5,increase ramping_up and central fan's magnitude
      else {
        //need an increment if error is smaller than 5
        ramping_up += RAMP_INCREMENT_P5;
        //adjust the magnitude
        set_lift_fan_magnitude(ramping_up);
      }
      break;

    //state for moving forward
    case MOVE_FORWARD_P10:

      // Set the velocity goal to 1 for the x and 0 for the y.
      velocity_goal[0] = 0.10;
      velocity_goal[1] = 0;
      //if distance to the wall is less than 30 cm, run this code
      Serial.println("move forward");
      if (distance_sensors[1] < 50) {
        //if the right sensor is less than 50 cm, run this code
        if (distance_sensors[0] < 50)
        {
          //this means we need to turn left
          rightOrLeft = false;
        }
        else {
          //this means we need to turn right
          rightOrLeft = true;
        }
        //if the countForTurn 0,this means we do not need to turn any more, run this code
        if (countForTurn == 0)
        {
          //if we are at the phase 1, run this code
          if (!phase2)
          {
                      //update the current orientation
          oldOrientation = IMU.yaw;
            //set state to arrange variables for phase 2
            state = PHASE_2_P10;

          }
          //if we are at the phase 2,run this code
          else
          {
            //set the state for ramping down the fan
            state = STATE_STOPPED_P10;
          }
        }
        //if the countForTurn is bigger than 0, we need to turn right or left, run this code
        else
        {          //update the current orientation
          oldOrientation = IMU.yaw;

          //if we need to run right,run this code
          if (rightOrLeft)
          {
            //set state to turn right
            state = TURN_RIGHT_10;
            Serial.println("right");
          }
          else
          {
            //set state to turn left
            state = TURN_LEFT_P10;
            Serial.println("left");
          }
        }
      }
      break;

    case TURN_LEFT_P10:
      //setting heading goal to current orientation plus 90 degrees to compute ret more precisely
      heading_goal = oldOrientation + 90;

      //set velocity goal to 0
      velocity_goal[0] = 0;
      velocity_goal[1] = 0;
      //if ret is smaller than 5,run this code
      if (abs(ret) < 5 ) {
        //set state to state = STATE_MOVE_FORWARD_2_P9, after turning left
        state = MOVE_FORWARD_P10;
        //decrease countForTurn by 1
        countForTurn--;
      }
      break;

    case TURN_RIGHT_10:
      //setting heading goal to current orientation plus 90 degrees to compute ret more precisely
      heading_goal = oldOrientation - 90;
      //set velocity goal to 0
      velocity_goal[0] = 0;
      velocity_goal[1] = 0;


      //if ret is smaller than 5,run this code
      if (abs(ret) < 5 ) {
        //set state to state = STATE_MOVE_FORWARD_2_P9, after turning left
        state = MOVE_FORWARD_P10;
        //decrease countForTurn by 1
        countForTurn--;

      }
      break;

    //state for implementing phase 2
    case PHASE_2_P10:
      //update phase2 variable to indicate we are at phase 2
      phase2 = true;
      //to turn 180 degrees,update heading_goal
      heading_goal = oldOrientation + 180;
      

      //set velocity goal to 0
      velocity_goal[0] = 0;
      velocity_goal[1] = 0;
      //if ret is smaller than 5,run this code
      if (abs(ret) < 5 ) {
        //if the swith B is pressed,run this code
        Serial.println("180");
        if (stateOfB == 1) {
          //set state to go to point C
          state = STATE_C_P10;

        }
        else {
          //set state to go to point D
          state = STATE_D_P10;
        }
      }

      break;
    //state for moving to point C
    case STATE_C_P10:
      //if we are turning right at the beginning this means we are at B right now, run this code
      if (rightOrLeft)
        //B
      {
        //set countForTurn to 2, to turn one time
        countForTurn = 2;
        //set rightOrLeft to false, we need to turn left
        rightOrLeft = false;
      }
      //A
      else
      {
        //set countForTurn to 0, we need to go straight
        countForTurn = 0;
        //set rightOrLeft to true, we need to turn right
        rightOrLeft = true;
      }
      //set new state to move forward
      state =  MOVE_FORWARD_P10;

      break;

    case STATE_D_P10:
      //A
      if (rightOrLeft)
      {
        //set countForTurn to 0, we need to go straight
        countForTurn = 0;
        //set rightOrLeft to true, we need to turn right
        rightOrLeft = true;
      }
      //B
      else
      {
        //set countForTurn to 2, to turn one time
        countForTurn = 2;
        //set rightOrLeft to false, we need to turn left
        rightOrLeft = false;
      }
      //set new state to move forward
      state =  MOVE_FORWARD_P10;
      break;

    //state for ramping down the central fan
    case STATE_STOPPED_P10:
      //set velocity goal to 0
      velocity_goal[0] = 0;
      velocity_goal[1] = 0;
      //set magnitude 0 for cenral fan, to shut down the central fan
      set_lift_fan_magnitude(0);
      break;

  }




}


/*
   Function to light up LED's for the given velocity value.

   input: float velocity- The value of velocity in degrees / second.
   Values range from -300.0 to 300.0.
   Output: Lights up the bar LED's based upon velocity.
*/
void display_heading_velocity(float velocity)
{
  //If the velocity is greater than 300
  if (velocity > 300)
  {
    //Swap value to negative
    velocity = velocity * -1;
  }
  //If the velocity is between -300 and -225, turn on the furthest LED left (B1)
  if ((velocity < -225) && (velocity > -300))
  {
    GPIOB_PDOR = (GPIOB_PDOR | 0x80C03) & 0x7F3FD;
  }

  //If velocity is between -225 and -150, turn on 2 LED's (B1 and B0)
  if ((velocity < -150) && (velocity > -225))
  {
    GPIOB_PDOR = (GPIOB_PDOR | 0x80C03) & 0x7F3FF;
  }

  //If velocity is between -150 and -75, light LED B0
  if ((velocity < -75) && (velocity > -150))
  {
    GPIOB_PDOR = (GPIOB_PDOR | 0x80C03) & 0x7F3FE;
  }

  //IF velocity is between -25 and -75, light LED B0 and B11
  if ((velocity < -25) && (velocity > -75))
  {
    GPIOB_PDOR = (GPIOB_PDOR | 0x80C03) & 0x7FBFE;
  }
  //If velocity is between 25 and -25, light LED B11
  if ((velocity < 25) && (velocity > -25))
  {
    GPIOB_PDOR = (GPIOB_PDOR | 0x80C03) & 0x7FBFC;
  }
  //If velocity is between 75 and 25, light LED B11 and B10
  if ((velocity < 75) && (velocity > 25))
  {
    GPIOB_PDOR = (GPIOB_PDOR | 0x80C03) & 0x7FFFC;
  }
  //If velocity is between 150 and 75, light LED B10
  if ((velocity < 150) && (velocity > 75))
  {
    GPIOB_PDOR = (GPIOB_PDOR | 0x80C03) & 0x7F7FC;
  }
  //If velocicty is between 150 and 225, light LED B10 and B19
  if ((velocity < 225) && (velocity > 150))
  {
    GPIOB_PDOR = (GPIOB_PDOR | 0x80C03) & 0xFF7Fc;
  }
  // If velocity is between 225 and 300, light LED B19
  if ((velocity < 300) && (velocity > 225))
  {
    GPIOB_PDOR = (GPIOB_PDOR | 0x80C03) & 0xFF3FC;
  }

}
/*
   Function to light up LED's for the given heading direction.

   input: float heading- The value of velocity in degrees.
   Values range from -179 to 180.
   Ouput: Lights up the Led's based upon heading.
*/
void display_heading(float heading)
{

  //If the heading is generally straight (+/- 10 degrees), light up LED on C2
  if ((heading < 10.00) && (heading > -10.00))
  {
    GPIOC_PDOR = (GPIOC_PDOR & ~0x207) | 0x4;
  }

  // Light up both LED on C2 and C1 if heading is between 10 and 80 degrees
  if ((heading < 80.00) && (heading > 10.00))
  {
    GPIOC_PDOR = (GPIOC_PDOR & ~0x207 ) | 0x6;
  }

  //Light up only LED on C1 if heading is between 80 and 100 degrees
  if ((heading < 100.00) && (heading > 80.00))
  {
    GPIOC_PDOR = (GPIOC_PDOR & ~0x207 ) | 0x2;
  }

  //Light up both C0 and C1 LED's if heading is between 100 and 170 degrees
  if ((heading < 170.00) && (heading > 100.00))
  {
    GPIOC_PDOR = (GPIOC_PDOR & ~0x207 ) | 0x3;
  }

  //Light up only C0 if the heading is greater than 170 degrees or less than -170
  if ((heading < -170.00) || (heading > 170.00))
  {
    GPIOC_PDOR = (GPIOC_PDOR & ~0x207 ) | 0x1;
  }

  //Light up C0 and C9 if the heading is greater than -170 degrees or less than -100
  if ((heading < -100.00) && (heading > -170.00))
  {
    GPIOC_PDOR = (GPIOC_PDOR & ~0x207 ) | 0x201;
  }

  //Light up only C9 if the heading is between -80 and -100 degrees
  if ((heading < -80.00) && (heading > -100.00))
  {
    GPIOC_PDOR = (GPIOC_PDOR & ~0x207 ) | 0x200;
  }

  //Light up C9 and C2 if the heading is between -10 and -80 degrees
  if ((heading < -10.00) && (heading > -80.00))
  {
    GPIOC_PDOR = (GPIOC_PDOR & ~0x207 ) | 0x204;
  }
}
/*
   report_step function.
   This function will run every 250 ms.
   Input:
   Output: Print out the current state of IMU.yaw
*/
void report_step()
{
  // Print out the current heading to two decimal places.
  //Serial.println(IMU.yaw, 2);
}

/*
   function compute_rotation_error
   This function will return the error between our goal orrientation,
   and out current orientation.

   Input: Our goal theta, float theta_goal, and our current theta, float theta

   Output: Error in theta, will be between -180 and 180.
*/
float compute_rotation_error(float theta_goal, float theta)
{
  float error = 0;
  // Check if the theta goal is too large.
  while (theta_goal > 180)
  {
    // If theta goal is too large subtract 360.
    theta_goal = theta_goal - 360;
  }
  // Check if the theta is too large.
  while (theta > 180)
  {
    // If theta is too large subtract 360.
    theta = theta - 360;
  }
  // Check if the theta goal is too small.
  while (theta_goal < -180)
  {
    // If theta goal is too small add 360
    theta_goal = theta_goal + 360;
  }
  // Check if the theta is too small.
  while (theta < -180)
  {
    // If theta is too small add 360
    theta = theta + 360;
  }
  // Calculates the error from theta and theta goal.
  error = theta - theta_goal;

  while (error > 180 )
  {
    error = error - 360;
  }
  while (error < -180 )
  {
    error = error + 360;
  }
  //Return the error
  return error;
}

/*
   deadband_and_saturation function will return a modified error.

   Input: Error, Deadband, Saturation

   Output: Modified error
*/
float deadband_and_saturation(float error, float deadband, float saturation)
{
  // declare a variabe to hold the error.
  float modified_error;

  //Checks where the error is within the deadband and saturation.
  //Error is between deadband and negative deadband.
  if (error < deadband && error > -deadband)
  {
    modified_error = 0;
  }
  //If error is greater then saturation then modified error is saturation-deadband
  if (error > saturation)
  {
    modified_error = saturation - deadband;
  }
  //If error is less then -saturation then error is -saturation+deadband
  if (error < -saturation)
  {
    modified_error = -saturation + deadband;
  }
  // If error is between deadband and saturation then the modified error increases linearly
  if (error > deadband && error < saturation)
  {
    modified_error = error - deadband;
  }

  // If error is between deadband and saturation then the modified error decrease linearly
  if (error < -deadband && error > -saturation)
  {
    modified_error = error + deadband;
  }
  // Return the modified error.
  return modified_error;
}

/* The accumulate slip function will measure the slip of the cameras
    and calculate the change in y and the change in x of each camera.

    Input: array adx[3], and an array ady[3].

    Ouput: the total change in adx and ady.
*/

void accumulate_slip(int32_t adx[3], int32_t ady[3])
{
  //Declare a variable dx and dy to store the change in y and x
  static int8_t dx, dy;

  //Declare a variable quality that will be an output of camera.readSlip.
  static uint8_t quality;

  //Call the function cameras.readslip for camera 1 that will output
  //the change in y and x for camera 1.
  for (static uint8_t i = 0; i <= 2; i++)
  {
    // Calls the function from the camera library, that will read the slip of the camera.
    int result = cameras.readSlip(CAMERA_SELECT[i], dx, dy, quality);

    //Check what result the function outputs
    // Check if we have moved, if we have increment adx[i] and ady[i]
    if (result == 0)
    {
      // Increment adx[i] by the change in x.
      adx[i] = adx[i] + dx;

      // Increment ady[i] by the change in y.
      ady[i] = ady[i] + dy;
    }

    //Check for overflow
    else if (result == -1)
    {
      Serial.println("Overflow");
    }

    // Check if we have moved.
    else if (result == -2)
    {

    }
  }
}

/*
   The compute_chasis_motion function will output the movement in the x,y, and theta
   from the accumulated slip values.

   Input: the arrays adx, ady, and motion.

   Output: the motion array, this function will rewrite the values contained in motion.
            Motion is outputed in meters and degrees.
*/
void compute_chasis_motion(int32_t adx[3], int32_t ady[3], float motion[3])
{
  // Calculate the motion in the X direction, and place it in the motion array.
  motion[0] = 0 + 0.0000056787 * adx[0] + 0.00000591865 * ady[0] + 0.000025318 * adx[1] - 0.0000478656 * ady[1] - 0.00002982269 * adx[2] + 0.000307718 * ady[2];

  // Calculate the motion in the Y direction, and place it in the motion array.
  motion[1] = 0 + 0.00003 * adx[0] - 0.00003 * ady[0] + 0.000209 * adx[1] - 0.00000023545949 * ady[1] - 0.00023052426984 * adx[2] - 0.00015667929637 * ady[2];

  // Calculate the motion in the Theta direction, and place it in the motion array.
  motion[2] = 0 + 0.06385921452992 * adx[0] + 0.02805974066932 * ady[0] + 0.07603386759616 * adx[1] + 0.00957765906546 * ady[1] + 0.05059129038337 * adx[2] + 0.00430608481015 * ady[2];
}

/*
   This function query the cameras, calculate the distance traveled and calculate a
   velocity.

   Input:
   Output: This function will change the values contained within the filtered velocity.
*/
void camera_step()
{
  // Set the values for tau, and dt
  static float tau = 0.05;
  static float dt = 0.005;

  // Call the accumulate slip function to read the slip from the cameras
  accumulate_slip(adx, ady);

  // Compute the motion from the cameras
  compute_chasis_motion(adx, ady, motion);

  //From the motion, calculate the filtered velocity
  velocity_filtered [0] = velocity_filtered [0] * (1 - dt / tau) + motion[0] / tau;
  velocity_filtered [1] = velocity_filtered [1] * (1 - dt / tau) + motion[1] / tau;
  velocity_filtered [2] = velocity_filtered [2] * (1 - dt / tau) + motion[2] / tau;
}

/*
   This method is an infinite loop to call (directly or indirectly) all above tasks/ methods and functions.
*/
void loop() {
  // Check to see if it is time to execute the various tasks


  sensor_task.step();
  fsm_task.step();
  control_task.step();
  report_task.step();
  camera_task.step();
}
