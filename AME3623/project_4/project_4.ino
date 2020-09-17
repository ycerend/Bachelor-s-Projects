/*Project 4
  3/16/2018
  Group 7
  Hannah Phillips, Travis Barnett and Yagmur Dardagan

  Coder: Yagmur Dardagan
*/
//includes PeriodicAction library for finite state machine
#include <PeriodicAction.h>


//includes "project.h" to use states for finite state machine
#include "project.h"

// Promising for implementing fsm_step function later in the code
void fsm_step();

// Creating a task that will be executed once per 50 ms
PeriodicAction fsm_task(50, fsm_step);

// Gains to be used for reverse thrust
const float FAN_GAIN[] = { 1.0, 1.0, 1.0};

//magnitudes array for storing thrust magnitude of lateral fans'
float magnitudes[] = { 0 , 0 , 0 };


//initializing pins of the fans as global variable
const int CENTER_PWM = 29;
const int RIGHT_PWM = 3;
const int LEFT_PWM = 2;
const int BACK_PWM = 4;


void setup() {
  // put your setup code here, to run once:

  //initializing switch
  PORTD_PCR1 = PORT_PCR_MUX(0x1);

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
}


/* The method clip takes the inputs, value-min_value-max_value, and checks the magnitude value in case of exceeding the range between min_ value and max value.

   Input=float value, float min_value, float max_value
   Output= int16_t value
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
   Input=float magnitude
*/

void set_lift_fan_magnitude(float magnitude) {
  //If magnitude does not fit within the range,run this code
  if (!(magnitude > 0 && magnitude < 178)) {
    //call clip method to clip to this range
    magnitude = clip(magnitude, 0, 178);
  }
  //sets the thrust magnitude for the lift fan
  analogWrite(CENTER_PWM, magnitude);

}

/*The method set_lateral_fan_magnitudes sets the thrust magnitude for the lateral fans (the order in the array is LEFT, RIGHT and BACK) and ensures
   that each of the input magnitudes fall within the range of -127... 127. If any do not, then the offending value clips to this range via calling clip( value, min_value, max_value).

   Input = float magnitudes[3]
*/

void set_lateral_fan_magnitudes(float magnitudes[3]) {
  //to ensure that each of the input magnitudes fall within the range of -127... 127 and arranges digital pins for the lateral fans
  for (int i = 0; i <= 2; i++) {
    //If magnitudes do not fit within the range,run this code
    if (!(magnitudes[i] > -127 && magnitudes[i] < 127)) {
      //call clip method to clip to this range
      magnitudes[i] = clip(magnitudes[i], -127, 127);
    }
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
  Serial.println( magnitudes[0]);
  Serial.println( magnitudes[1]);
  Serial.println( magnitudes[2]);

  //sets the thrust magnitude for the lateral fans
  analogWrite(LEFT_PWM, magnitudes[0]);
  analogWrite(BACK_PWM, magnitudes[1]);
  analogWrite(RIGHT_PWM, magnitudes[2]);



}
/*The method, fsm_step(), adjusts what states are going to do and implements the finite state machine.

*/
void fsm_step()
{
  //initializing state
  static State state = STATE_START;
  //temporary variable for magnitudes
  static float ramp_state;

  //increment rate
  const float RAMP_INCREMENT = 0.2;

  //implementing states and their functions
  switch (state)
  {
    //initial state
    case STATE_START:
      // If the switch is pressed, run this code
      if ( GPIOD_PDIR & 0x2 )
      {
        // Go to STATE_CENTER_RAMP_UP state, if the switch is pressed
        state = STATE_CENTER_RAMP_UP ;
        //initializing temporary variable for magnitudes
        ramp_state = 0.0;

      }
      // If the switch is  not pressed, run this code
      else
      {
        // Go to lateral fans
        state = STATE_LEFT_RAMP_UP;
        //initializing temporary variable for magnitudes
        ramp_state = 0.0;
        //adjust the thrust magnitude for central fan
        set_lift_fan_magnitude(ramp_state);

      }
      break;
    //center_ramp_up state
    case STATE_CENTER_RAMP_UP:
      // Add a bit more to the current state
      ramp_state += RAMP_INCREMENT;
      //if the duty cycle have reached 25%,run this code
      if (ramp_state > 64)
      {
        // change the state WAIT
        state = WAIT;
      }
      else
      {

        // Adjust the magnitude
        set_lift_fan_magnitude(ramp_state);

      }
      break;


    case WAIT:
      // if the switch is pressed, run this code
      if (GPIOD_PDIR & 0x2 )
      {
        // adjustin the state
        state = WAIT;
        //adjust the magnitude of lift fan
        set_lift_fan_magnitude(ramp_state);
      }
      else
      {
        // Adjust the state
        state = STATE_CENTER_RAMP_DOWN;
      }
      break;
    //center_ramp_down state
    case STATE_CENTER_RAMP_DOWN:
      // Add a bit more to the current state
      ramp_state -= RAMP_INCREMENT;
      //if the motor needs to stop, run this code
      if (ramp_state <= 0)
      {
        // transition to initial state STATE_START
        state = STATE_START ;
      }
      else
      {
        // Adjust the magnitude
        set_lift_fan_magnitude(ramp_state);
      }
      break;

    case STATE_LEFT_RAMP_UP:
      // Add a bit more to the current state
      ramp_state += RAMP_INCREMENT;
      //adjusting the magnitude of left fan
      magnitudes[0] = ramp_state;
      //if the duty cycle have reached 25%,run this code
      if (ramp_state > 64)
      {
        // We have reached 25%: transition
        state = STATE_LEFT_RAMP_DOWN;
      }
      else
      {
        // Adjust the magnitude

        set_lateral_fan_magnitudes(magnitudes) ;

      }
      break;
    //left_ramp_down state
    case STATE_LEFT_RAMP_DOWN:
      // Add a bit more to the current state
      ramp_state -= RAMP_INCREMENT;
      //adjusting the magnitude of left fan
      magnitudes[0] = ramp_state;
      //if the duty cycle have reached -25%,run this code
      if (ramp_state <= -64)
      {
        // We have reached -25%: transition
        state = STATE_LEFT_RAMP_STOP;
      }
      else
      {
        // Adjust the magnitude
        set_lateral_fan_magnitudes(magnitudes) ;
      }
      break;
    //left_ramp_stop state
    case STATE_LEFT_RAMP_STOP:
      // Add a bit more to the current state
      ramp_state += RAMP_INCREMENT;
      //adjusting the magnitude of left fan
      magnitudes[0] = ramp_state;
      //if motor needs to stop, run this code
      if (ramp_state >= 0)
      {
        //  transition to back fan
        state = STATE_BACK_RAMP_UP;
      }
      else
      {
        // Adjust the magnitude
        set_lateral_fan_magnitudes(magnitudes) ;
      }
      break;
    //back_ramp_up state
    case STATE_BACK_RAMP_UP:
      // Add a bit more to the current state
      ramp_state += RAMP_INCREMENT;
      //adjusting the magnitude of back fan
      magnitudes[1] = ramp_state;
      //if the duty cycle have reached 25%,run this code
      if (ramp_state > 64)
      {
        // We have reached 25%: transition
        state = STATE_BACK_RAMP_DOWN;
      }
      else
      {
        // Adjust the magnitude
        set_lateral_fan_magnitudes(magnitudes) ;
      }
      break;

    //back_ramp_down state
    case STATE_BACK_RAMP_DOWN:
      // Add a bit more to the current state
      ramp_state -= RAMP_INCREMENT;
      //adjusting the magnitude of back fan
      magnitudes[1] = ramp_state;
      //if the duty cycle have reached -25%,run this code
      if (ramp_state <= -64)
      {
        // We have reached -25%: transition
        state = STATE_BACK_RAMP_STOP;
      }
      else
      {
        // Adjust the magnitude
        set_lateral_fan_magnitudes(magnitudes) ;
      }
      break;

    //back_ramp_stop state
    case STATE_BACK_RAMP_STOP:
      // Add a bit more to the current state
      ramp_state += RAMP_INCREMENT;
      //adjusting the magnitude of back fan
      magnitudes[1] = ramp_state;
      //if motor needs to stop, run this code
      if (ramp_state >= 0)
      {
        // transition to right fan
        state = STATE_RIGHT_RAMP_UP;
      }
      else
      {
        // Adjust the magnitude
        set_lateral_fan_magnitudes(magnitudes) ;
      }
      break;
    //right_ramp_up state
    case STATE_RIGHT_RAMP_UP:
      // Add a bit more to the current state
      ramp_state += RAMP_INCREMENT;
      //adjusting the magnitude of right fan
      magnitudes[2] = ramp_state;
      //if the duty cycle have reached 25%,run this code
      if (ramp_state > 64)
      {
        // We have reached 25%: transition
        state = STATE_RIGHT_RAMP_DOWN;
      }
      else
      {
        // Adjust the magnitude
        set_lateral_fan_magnitudes(magnitudes) ;
      }
      break;

    //right_ramp_down state
    case STATE_RIGHT_RAMP_DOWN:
      // Add a bit more to the current state
      ramp_state -= RAMP_INCREMENT;
      //adjusting the magnitude of right fan
      magnitudes[2] = ramp_state;
      //if the duty cycle have reached -25%,run this code
      if (ramp_state <= -64)
      {
        // We have reached -25%: transition
        state = STATE_RIGHT_RAMP_STOP;
      }
      else
      {
        // Adjust the magnitude
        set_lateral_fan_magnitudes(magnitudes) ;
      }
      break;
    //right_ramp_stop state
    case STATE_RIGHT_RAMP_STOP:
      // Add a bit more to the current state
      ramp_state += RAMP_INCREMENT;
      //adjusting the magnitude of right fan
      magnitudes[2] = ramp_state;
      //if motor needs to stop, run this code
      if (ramp_state >= 0)
      {
        //transition to initial state, STATE_START
        state = STATE_START;

      }
      else
      {
        // Adjust the magnitude
        set_lateral_fan_magnitudes(magnitudes) ;

      }
      break;

  }
}

void loop() {
  // runs fsm_task method repeatedly to implement finite state machine
  fsm_task.step();


}
