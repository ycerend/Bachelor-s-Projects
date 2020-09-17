
/*Project 6
  4/5/2018
  Group 7
  Hannah Phillips, Travis Barnett and Yagmur Dardagan

  Coder: Hannah Phillips
*/
#include "project.h"
#include "ImuUtils.h"
#include "PeriodicAction.h"

// Promising for implementing fsm_step function later in the code
void fsm_step();

//Promising for implementing display_heading_velocity later in the code
void  display_heading_velocity(int16_t velocity);

//Promising for implementing display_heading later in the code
void display_heading(float heading);

// Promise that we will implement this function later
void control_step();

// Create a task that will be executed once per 10 ms
PeriodicAction control_task(10, control_step);

// Promise that we will implement this function later
void sensor_step();

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

//magnitudes array for storing thrust magnitude of lateral fans
float magnitudes[] = { 0, 0 , 0 };

//initializing pins of the fans as global variable
const int CENTER_PWM = 29;
const int RIGHT_PWM = 3;
const int LEFT_PWM = 2;
const int BACK_PWM = 4;

//Initialize a global variable that will give us our desired heading.
float heading_goal;

/*
 * setup will be run once and will prepare to run the arduino software.
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
  //initializing switch
  PORTD_PCR1 = PORT_PCR_MUX(0x1);

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

}

/*
   sensor_step will query the sensors, check for a character and display the velocity
   and heading.

   Input: 
   Output: Will change the LED's
*/
void sensor_step()
{
  //Update IMU
  imu_update();

  //Check if a char has been outputed.
  if(Serial.available() > 0)
  {
    //Declare a variable to hold the character.
    char character=Serial.read();
    
    /*Check if the char is a C. If so call the function
     * imu_calibrate_magbias(), to recalibrate the hoverboard.
     */
     if(character == 'c' || character == 'C')
     {
      //Call the imu calibration function
      imu_calibrate_magbias();
     }

     /* 
      *  Check if the char is g. If so set the heading goal as the current orientation.
      */
     else if(character == 'g' || character =='G')
     {
      // Set the heading goal at the current orientation
      heading_goal=IMU.yaw;
     }
  }
  //Display the rotation on the Heading LED's
  display_heading(IMU.yaw);  
  
  //Display the rotational velocity on the LED bar
  display_heading_velocity(IMU.gz);
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
  magnitudes[0] = 0.577 * (ddx) - 0.3333* (ddy) - 2.4416* (ddtheta);
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
  float Kp = 1.4;

  //Declare two variables that will hold the values for the deadband and saturation range
  float deadband = 2;
  float saturation = 35;

  // Declare a varialble ret to hold a value from the function compute_rotation_error
  float ret = compute_rotation_error(heading_goal, IMU.yaw);

  // Declare the variable modified error to hold a value from the function deadband_and_saturation
  float modified_error = deadband_and_saturation(ret, deadband, saturation);
  
  //Set the x and y desired locations to 0
  float ddx = 0.0;
  float ddy = 0.0;
  float ddtheta = Kp * modified_error;
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
  magnitude = clip(magnitude, 0, 150);
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
   Output: Adjustment of the speed of the lift fan based upon the state.
*/
void fsm_step()
{
  //initializing state
  //static State state = STATE_START;
  static State state = P5_START;
  //temporary variable for magnitudes
  static float ramp_state;
  //increment rate
  const float RAMP_INCREMENT_P5 = 1.78;
  //Integer counter to be used for time in P5
  static int counter;

  //New function of FSM: Ramp up to 70% in 5s, wait 30s, ramp down in 5s
  //implementing states and their functions
  switch (state)
  {
    // initial state for project 5
    case P5_START:
      // Set the ramp state
      ramp_state = 0.0;
      //Set the new state to ramp up the center fan
      state = STATE_RAMP_CENTER_UP_P5;
      break;

    //Project 5 ramp up state
    case STATE_RAMP_CENTER_UP_P5:
      //Need an increment to reach 70% power in 5 seconds
      ramp_state += RAMP_INCREMENT_P5;
      // Check the magnitude of the state
      if (ramp_state > 177)
      {
        // change the state to WAIT for Project 5
        state = WAIT_P5;
      }
      else
      {
        // Adjust the magnitude
        set_lift_fan_magnitude(ramp_state);
      }
      break;

    //WAIT state for Project 5
    case WAIT_P5:
      // Check the counter to see if 30 seconds has past
      if (counter < 300)
      {
        counter++;
        //Keep the state in WAIT_P5
        state = WAIT_P5;
        //Set the lift fan to the same power level
        set_lift_fan_magnitude(ramp_state);
      }
      else
      {
        //When done waiting, set to ramp down state
        state = STATE_CENTER_RAMP_DOWN_P5;
      }
      break;

    case STATE_CENTER_RAMP_DOWN_P5:
      //Need an increment to reach 70% power in 5 seconds
      ramp_state -= RAMP_INCREMENT_P5;
      // Check the magnitude of the state
      if (ramp_state < 5)
      {
        // change the state to WAIT for Project 5
        state = WAIT_P5;
      }
      else
      {
        // Adjust the magnitude
        set_lift_fan_magnitude(ramp_state);
      }
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
 * report_step function.
 * This function will run every 250 ms.  
 * Input:
 * Output: Print out the current state of IMU.yaw
 */
void report_step()
{
  // Print out the current heading to two decimal places.
  Serial.println(IMU.yaw,2);
}

/*
 * function compute_rotation_error
 * This function will return the error between our goal orrientation, 
 * and out current orientation.
 * 
 * Input: Our goal theta, float theta_goal, and our current theta, float theta
 * 
 * Output: Error in theta, will be between -180 and 180.
 */
float compute_rotation_error(float theta_goal, float theta)
{
  float error=0;
  // Check if the theta goal is too large.
  while (theta_goal >180)
  {
    // If theta goal is too large subtract 360.
    theta_goal = theta_goal - 360;
  }
  // Check if the theta is too large.
  while (theta >180)
  {
    // If theta is too large subtract 360.
    theta = theta - 360;
  }
  // Check if the theta goal is too small.
  while(theta_goal < -180)
  {
    // If theta goal is too small add 360
    theta_goal = theta_goal + 360;
  }
  // Check if the theta is too small.
  while(theta < -180)
  {
    // If theta is too small add 360
    theta = theta + 360;
  }  
  // Calculates the error from theta and theta goal.
  error = theta-theta_goal;

  while(error > 180 )
  {
    error=error-360;
  }
  while(error < -180 )
  {
    error=error+360;
  }
  //Return the error
  return error;
}

/*
 * deadband_and_saturation function will return a modified error.
 * 
 * Input: Error, Deadband, Saturation
 * 
 * Output: Modified error
 */
 float deadband_and_saturation(float error, float deadband, float saturation)
 {
  // declare a variabe to hold the error.
  float modified_error;

  //Checks where the error is within the deadband and saturation.
  //Error is between deadband and negative deadband.
  if(error < deadband && error > -deadband)
  {
    modified_error=0;
  }
  //If error is greater then saturation then modified error is saturation-deadband
  if(error > saturation)
  {
    modified_error = saturation-deadband;
  }
  //If error is less then -saturation then error is -saturation+deadband
  if(error < -saturation)
  {
    modified_error = -saturation+deadband;
  }
  // If error is between deadband and saturation then the modified error increases linearly
  if (error > deadband && error < saturation)
  {
    modified_error = error-deadband;
  }
  
  // If error is between deadband and saturation then the modified error decrease linearly
  if(error < -deadband && error > -saturation)
  {
    modified_error = error + deadband;
  }
  // Return the modified error.
  return modified_error;
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
}
